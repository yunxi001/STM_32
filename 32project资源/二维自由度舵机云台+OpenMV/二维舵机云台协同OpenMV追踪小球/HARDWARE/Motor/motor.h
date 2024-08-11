/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef __MOTOR_H
#define __MOTOR_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "sys.h"

/* ���� -----------------------------------------------------------------------------------------------------------------*/
// ����ĵ��������Ҫ����ʵ��������и���
#define M_left_IN1 PBout(15) // TIM1->CCR4   TIM3������
#define M_left_IN2 PBout(14)
#define M_right_IN3 PBout(12) //  TIM1->CCR1  TIM2������
#define M_right_IN4 PBout(13)

#define PWM_MAX 6500

typedef enum
{
    FORWARD,
    BACK,
    STOP
} MotorDIR_Choose;

/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/
void Motor_All_Init(void); // ���IO�Լ�PWM���������ʼ��

void Motor_Left_DIR(MotorDIR_Choose Direction);
void Motor_Right_DIR(MotorDIR_Choose Direction);
void Load_Motor_PWM(int MOTOR1_PWM, int MOTOR2_PWM);

void set_motor_enable(void);
void set_motor_disable(void);

#endif /* __MOTOR_H */

/*****************************************************END OF FILE*********************************************************/
