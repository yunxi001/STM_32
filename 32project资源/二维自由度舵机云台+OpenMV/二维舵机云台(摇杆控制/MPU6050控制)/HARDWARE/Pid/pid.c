/*
 * @Author: _oufen
 * @Date: 2023-07-03 13:31:06
 * @LastEditTime: 2023-07-13 10:09:22
 * @Description:
 */
#include "stm32f10x.h" // Device header
#include "pid.h"
#include "sys.h"

_pid pid_speed1, pid_speed2; // 速度环结构体变量

_pid pid_location; // 巡线环 结构体变量

_pid pid_angle; // 角度环 结构体变量

float Pid_speed1, Pid_speed2, Pid_Actual_angle; // 速度环的实际值

extern float pitch, roll, yaw; // 欧拉角

/**
 * @brief 	pid
 * @param 	无
 * @retval  无
 */

void PID_param_init(void)
{
    /*左轮pid参数*/
    pid_speed1.target_val = 0.0; // 期望速度 rpm
    pid_speed1.actual_val = 0.0;
    pid_speed1.err = 0.0;
    pid_speed1.err_last = 0.0;
    pid_speed1.integral = 0.0;

    pid_speed1.Kp = 20.0;
    pid_speed1.Ki = 1.5;
    pid_speed1.Kd = 0.0;

    /*右轮pid参数*/
    pid_speed2.target_val = 0.0; // 期望速度  rpm
    pid_speed2.actual_val = 0.0;
    pid_speed2.err = 0.0;
    pid_speed2.err_last = 0.0;
    pid_speed2.integral = 0.0;

    pid_speed2.Kp = 20.0;
    pid_speed2.Ki = 1.5;
    pid_speed2.Kd = 0.0;

    /*巡线环pid参数*/
    pid_location.target_val = 0.0; // 巡线中心值 自己进行设置  期望巡线中心值
    pid_location.actual_val = 0.0;
    pid_location.err = 0.0;
    pid_location.err_last = 0.0;
    pid_location.integral = 0.0;

    pid_location.Kp = 0.0;
    pid_location.Ki = 0.0;
    pid_location.Kd = 0.0;

    /*角度环pid参数*/
    pid_angle.target_val = -0.4; // 角度环期望
    pid_angle.actual_val = 0.0;
    pid_angle.err = 0.0;
    pid_angle.err_last = 0.0;
    pid_angle.integral = 0.0;

    pid_angle.Kp = -5.0; // 注意极性
    pid_angle.Ki = 0.0;
    pid_angle.Kd = -80.0;
}

/**
 * @brief   获取目标值
 * @param   无
 *	@note 	无
 * @retval  目标值
 */
float get_pid_target(_pid *pid)
{
    return pid->target_val; // 设置当前的目标值
}

/**
 * @brief   获取实际值
 * @param   无
 *	@note 	无
 * @retval  实际值
 */
float get_pid_actual(_pid *pid)
{
    return pid->actual_val;
}

/**
 * @brief   设置 P I D
 * @param   无
 *	@note 	无
 * @retval  无
 */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
    pid->Kp = p; // 设置比例系数 P
    pid->Ki = i; // 设置积分系数 I
    pid->Kd = d; // 设置微分系数 D
}

/**
 * @brief   设置目标值
 * @param  val	目标值
 *	@note 	无
 * @retval 无
 */
void set_pid_target(_pid *pid, float temp_val)
{
    pid->target_val = temp_val; // 设置当前的目标值
}

/**
 * @brief  速度PID算法实现
 * @param  actual_val:实际值
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
float speed_pid_realize(_pid *pid, float actual_val)
{
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;

    // 消除机械误差
    if ((pid->err < 0.5f) && (pid->err > -0.5f)) // 假如以最大允许速度偏差运行1分钟，输出轴最大偏差为半圈
    {
        pid->err = 0.0f;
    }

    pid->integral += pid->err; // 误差累积

    /*积分限幅 pid_speed1.Ki *限幅值等于PWM输出最大值*/ // 调试的时候需要注释  待确i后进行限幅
    // if (pid->integral >= 4000)
    // {
    //     pid->integral = 4000;
    // }
    // else if (pid->integral < -4000)
    // {
    //     pid->integral = -4000;
    // }

    /*PID算法实现*/
    pid->actual_val = pid->Kp * pid->err + pid->Ki * pid->integral + pid->Kd * (pid->err - pid->err_last);

    /*误差传递*/
    pid->err_last = pid->err;

    /*返回当前实际值*/
    return pid->actual_val;
}

/**
 * @brief  循迹PID算法实现   通过接收Mv发送过来的巡线
 * @param  actual_val:实际值
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
// TIM4->CCR1 = 1400; // 初始化 舵机PWM大概在 700左右   初始化舵机  初始化角度在90 1.5ms 对应占空比为1400

// TIM4->CCR1 = 700;  舵机角度 左转最大角度
// TIM4->CCR1 = 2100; 舵机角度 右转最大角度
float location_pid_realize(_pid *pid, float actual_val)
{
    /*    计算偏差    这里的偏差是指 巡线偏差 设定的巡线期望值 和 MV传回的巡线实际值 得偏差 */
    pid->err = pid->target_val - actual_val;

    pid->integral += pid->err; // 误差累积

    // if (pid->err >= 1000) // 积分限幅
    // {
    //     pid->err = 0;
    // }

    // if (huidu.output >= 550)
    //     huidu.output = 550;
    // if (huidu.output <= -600)
    //     huidu.output = -600;

    /*PID算法实现*/
    pid->actual_val = pid->Kp * pid->err + pid->Ki * pid->integral + pid->Kd * (pid->err - pid->err_last);

    /*误差传递*/
    pid->err_last = pid->err;

    /*返回当前实际值*/
    return pid->actual_val;
}

/**
 * @brief  角度环pid输出函数
 * @param  actual_val:实际值  6050yaw
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
float Angle_pid_realize(_pid *pid, float actual_val)
{
    /*    计算偏差    这里的偏差是指 巡线偏差 设定的巡线期望值 和 MV传回的巡线实际值 得偏差 */
    pid->err = pid->target_val - actual_val;

    pid->integral += pid->err; // 误差累积

    // if (pid->err >= 1000) // 积分限幅
    // {
    //     pid->err = 0;
    // }

    // if (huidu.output >= 550)
    //     huidu.output = 550;
    // if (huidu.output <= -600)
    //     huidu.output = -600;

    /*PID算法实现*/
    pid->actual_val = pid->Kp * pid->err + pid->Ki * pid->integral + pid->Kd * (pid->err - pid->err_last);

    /*误差传递*/
    pid->err_last = pid->err;

    /*返回当前实际值*/
    return pid->actual_val;
}

/**
 * @brief  速度环pid输出函数
 * @param  actual_val:实际值
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
float speed1_pid_control(void)
{

    float Expect_Pwm = 0.0; // 当前控制值

    // pid_speed1.actual_val = ((float)Param.UnitTime_Motor1Pluse * 1000.0 * 60.0) / (RESOULTION_TOTAL_RATIO * REDUCTION_RATIO * PID_PERIOD);

    Pid_speed1 = ((float)Param.UnitTime_Motor1Pluse * 60000.0) / 17680;

    Expect_Pwm = speed_pid_realize(&pid_speed1, Pid_speed1); // 进行 PID 计算

    //  #if defined(PID_ASSISTANT_EN)
    //    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);                // 给通道 1 发送实际值
    //  #endif

    return Expect_Pwm;
}

/**
 * @brief  速度环pid输出函数
 * @param  actual_val:实际值
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
float speed2_pid_control(void)
{

    float Expect_Pwm = 0.0; // 当前控制值

    // pid_speed1.actual_val = ((float)Param.UnitTime_Motor1Pluse * 1000.0 * 60.0) / (RESOULTION_TOTAL_RATIO * REDUCTION_RATIO * PID_PERIOD);

    Pid_speed2 = ((float)Param.UnitTime_Motor2Pluse * 60000.0) / 17680;

    Expect_Pwm = speed_pid_realize(&pid_speed2, Pid_speed2); // 进行 PID 计算

    //  #if defined(PID_ASSISTANT_EN)
    //    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);                // 给通道 1 发送实际值
    //  #endif

    return Expect_Pwm;
}

/**
 * @brief  角度环pid输出函数
 * @param  actual_val:无
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
float Angle_pid_control(void)
{

    float Expect_Pwm = 0.0; // 当前控制值

    // pid_speed1.actual_val = ((float)Param.UnitTime_Motor1Pluse * 1000.0 * 60.0) / (RESOULTION_TOTAL_RATIO * REDUCTION_RATIO * PID_PERIOD);

    // Pid_speed2 = ((float)Param.UnitTime_Motor2Pluse * 60000.0) / 17680;

    Pid_Actual_angle = yaw; // 实际值 为yaw

    Expect_Pwm = Angle_pid_realize(&pid_angle, Pid_Actual_angle); // 进行 PID 计算

    //  #if defined(PID_ASSISTANT_EN)
    //    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);                // 给通道 1 发送实际值
    //  #endif

    return Expect_Pwm;
}
