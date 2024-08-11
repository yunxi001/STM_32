/*
 * @Author: _oufen
 * @Date: 2023-07-11 20:23:19
 * @LastEditTime: 2023-07-13 09:21:56
 * @Description:  主要控制代码
 */
/**
 *************************************************************************************************************************
 * @file    control.c
 * @author  oufen
 * @version V1.0
 * @date    2023-07-10
 * @brief   Control模块.c文件配置
 *************************************************************************************************************************
 * @attention
 *
 *
 *************************************************************************************************************************
 */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "control.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/
Param_InitTypedef Param;
Flag_InitTypedef Flag;

extern float pitch, roll, yaw; // 欧拉角
/**
 * @brief 	TIM6中断   10ms
 * @param 	无
 * @retval  无
 */
float speed1, speed2;
void TIM6_IRQHandler(void) // 10ms
{
    static uint16_t Time_Cnt; // 定时变量
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        LED_RED = !LED_RED; // 测试程序是否卡死
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

        // 获取单位时间内脉冲数
        Param.UnitTime_Motor1Pluse = -(short)Read_Encoder(2);
        Param.UnitTime_Motor2Pluse = (short)Read_Encoder(3);

        // 获取累积脉冲数
        Param.Sigma_Motor1_Pluse += Param.UnitTime_Motor1Pluse;
        Param.Sigma_Motor2_Pluse += Param.UnitTime_Motor2Pluse;

        // u1_printf("%d %d\r\n", Param.UnitTime_Motor1Pluse, Param.UnitTime_Motor2Pluse);  打印两轮脉冲

        // 计算速度 rpm
        speed1 = ((float)Param.UnitTime_Motor1Pluse * 60000.0) / 17680;
        speed2 = ((float)Param.UnitTime_Motor2Pluse * 60000.0) / 17680;

        // u1_printf("%f , %f\r\n", speed1, speed2);        打印两轮速度  rpm

        // 循迹转向环
        //  pid_speed1.target_val = 100.0 + PID_output();
        //  pid_speed2.target_val = 100.0 - PID_output();

        // 角度闭环
        pid_speed1.target_val = 60.0 + Angle_pid_control();
        pid_speed2.target_val = 60.0 - Angle_pid_control();

        if (pid_speed1.target_val > 100) // 对基础速度进行限幅
        {
            pid_speed1.target_val = 100;
        }
        else if (pid_speed1.target_val < 0)
        {
            pid_speed1.target_val = 0;
        }

        if (pid_speed2.target_val > 100)
        {
            pid_speed2.target_val = 100;
        }
        else if (pid_speed2.target_val < 0)
        {
            pid_speed2.target_val = 0;
        }

        Param.Motor1_PWM_Out = speed1_pid_control();
        Param.Motor2_PWM_Out = speed2_pid_control();

        // u1_printf("%f,%f,%d\n", pid_speed1.target_val, Pid_speed1, Param.Motor1_PWM_Out);
        if (Param.Motor1_PWM_Out > 0)
        {

            Motor_Left_DIR(FORWARD);
        }
        else
        {
            Motor_Left_DIR(BACK);
        }

        if (Param.Motor2_PWM_Out > 0)
        {
            Motor_Right_DIR(FORWARD);
        }
        else
        {
            Motor_Right_DIR(BACK);
        }

        Load_Motor_PWM(Param.Motor1_PWM_Out, Param.Motor2_PWM_Out);

        // if (yaw != 0)
        // {
        //     pid_speed1.err = 0;
        //     pid_speed2.err = 0;
        //     Load_Motor_PWM(Param.Motor1_PWM_Out, Param.Motor2_PWM_Out);
        // }
        // else
        // {
        //     set_motor_disable();
        // }

        if (Flag.Start_Count == 1) // 开始计数
        {
            if (++Time_Cnt == Param.Timer_threshold_value) // 定时时间到
            {
                Time_Cnt = 0;         // 清零计数值
                Flag.Is_Timer_Up = 1; // 时间到  标志置1
            }
        }

        // if (L1 == 1 && R1 == 1 && M == 1)
        // {
        //     set_motor_disable();
        //     Flag.Start_Count = 1;              // 开始计时
        //     Param.Timer_threshold_value = 150; // 定时1.5s
        //     Buzzer_IN = 1;                     // 开蜂鸣器
        // }
        // if (Flag.Is_Timer_Up == 1) // 定时时间到
        // {
        //     Buzzer_IN = 0;        // 关蜂鸣器
        //     Flag.Start_Count = 0; // 不开启计数
        //     Flag.Is_Timer_Up = 0; // 时间到标志位清零
        // }
    }
}
/*****************************************************END OF FILE*********************************************************/
