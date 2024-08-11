/*
 * @Author: _oufen
 * @Date: 2023-07-03 13:31:06
 * @LastEditTime: 2023-07-29 17:56:55
 * @Description:
 */
#include "stm32f10x.h" // Device header
#include "servo.h"

// TIM4->CCR1 = 1400; // 初始化 舵机PWM大概在 700左右   初始化舵机  初始化角度在90 1.5ms 对应占空比为1400

// TIM4->CCR1 = 700;  舵机角度 左转最大角度
// TIM4->CCR1 = 2100; 舵机角度 右转最大角度

/**
 * @brief 	舵机初始化 开启时钟 选择端口 初始化GPIO口   使用PB6  TIM4的通道1
 * @param 	无
 * @retval  无
 */

void Servo_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // 开时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // TIM4开启时钟

    MY_GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_AF_PP); // PB0 GPIO  初始化
    MY_GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_AF_PP); // PB0 GPIO  初始化

    // 时基单元初始化
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 20000 - 1; // ARR   阈值 20000
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1; // PSC
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // PWM模式1
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0; // CCR 寄存器的值
    TIM_OC1Init(TIM4, &TIM_OCInitStruct);

    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // PWM模式1
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0; // CCR 寄存器的值
    TIM_OC2Init(TIM4, &TIM_OCInitStruct);

    TIM_Cmd(TIM4, ENABLE);

    // 舵机复位
    TIM4->CCR1 = 500;  // 400   中值		上下  大舵机  700   1000   500    400    小舵机  1800        2200       2500
    TIM4->CCR2 = 1500; // 1500	中值		左右  大舵机  1800     1500   1000   小舵机 2000          1500      1000
}

void Servo_SetAngle(float Angle)
{
    TIM4->CCR1 = Angle / 180 * 2000 + 500;
}
