/*
 * @Author: _oufen
 * @Date: 2023-04-15 09:06:56
 * @LastEditTime: 2023-07-03 19:50:48
 * @Description: TIM6定时函数
 */

/* Includes -------------------------------------------------------------------------------------------------------------*/

#include "timer6.h"
// #include "myconfig.h"

/* Define -----------------------------------------------------------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------------------------------------------------------*/
/**
 * @brief 	TIM6初始化 用作定时器10ms
 * @param 	无
 * @retval  无
 */
void Timer6_Init(uint16_t psc, uint16_t arr)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // TIM_DeInit(TIM6); // 定时器4恢复默认设置

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update); // 清除更新中断请求位        //这句话一定要放在使能更新中断前面，否则定时器初始化会直接进入一次更新中断
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM6, ENABLE);
}

/*****************************************************END OF FILE*********************************************************/
