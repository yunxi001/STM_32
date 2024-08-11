/*
 * @Author: _oufen
 * @Date: 2023-07-11 20:23:19
 * @LastEditTime: 2023-07-29 19:08:54
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

void Limit(int16_t Servo_X_Pwm, int16_t Servo_Y_Pwm)
{
    if (Param.Servo_X_PWM > 2000)
    {
        Param.Servo_X_PWM = 2000;
    }
    else if (Param.Servo_X_PWM < 1000)
    {
        Param.Servo_X_PWM = 1000;
    }

    if (Param.Servo_Y_PWM > 2500)
    {
        Param.Servo_Y_PWM = 2500;
    }
    else if (Param.Servo_Y_PWM < 1800)
    {
        Param.Servo_Y_PWM = 1800;
    }
}

/**
 * @brief 	TIM6中断   10ms
 * @param 	无
 * @retval  无
 */
float speed1,
    speed2;

float X_pwm, Y_pwm;
void TIM6_IRQHandler(void) // 10ms
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        LED_RED = !LED_RED; // 测试程序是否卡死
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

        // 中心点坐标为  160 120
        // X轴方向
        if (((OpenMv_data1 << 8) | OpenMv_data2) != 0)
        {
            Param.Servo_X_PWM -= Servo_X_pid_control(); // 这里的+还是-  要根据摄像头的摆放情况来修改   由于摄像头是倒着放的

            if (Param.Servo_X_PWM > 1800)
            {
                Param.Servo_X_PWM = 1800;
            }
            else if (Param.Servo_X_PWM < 1000)
            {
                Param.Servo_X_PWM = 1000;
            }
        }
        else
        {
            pid_servo_X.err = 0;
            pid_servo_X.integral = 0;
        }
        // y轴方向
        if (OpenMv_data3 != 0)
        {
            Param.Servo_Y_PWM -= Servo_Y_pid_control();
            if (Param.Servo_Y_PWM > 600)
            {
                Param.Servo_Y_PWM = 600;
            }
            else if (Param.Servo_Y_PWM < 400)
            {
                Param.Servo_Y_PWM = 400;
            }
        }
        else
        {
            pid_servo_Y.err = 0;
            pid_servo_Y.integral = 0;
        }

        TIM4->CCR1 = Param.Servo_Y_PWM;
        TIM4->CCR2 = Param.Servo_X_PWM;
    }
}
/*****************************************************END OF FILE*********************************************************/
