/*
 * @Author: _oufen
 * @Date: 2023-04-15 13:45:19
 * @LastEditTime: 2023-04-15 15:37:15
 * @Description: 编码器配置函数 使用Timer2和Timer3的编码器模式，读取编码器脉冲
 */

/* Includes -------------------------------------------------------------------------------------------------------------*/

#include "encoder.h"

/* Define -----------------------------------------------------------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------------------------------------------------------*/
/**
 * @description: TIM2定时器编码器模式 PA0 和PA1配置函数
 * @param {uint16_t} psc 预分频值
 * @param {uint16_t} arr 预装载重装值
 * @return {*}
 */
void TIM2_Encoder_Init(uint16_t psc, uint16_t arr)
{
    // 相关结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // GPIO初始化
    MY_GPIO_Init(GPIOA, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_IN_FLOATING);

    // 时基单元配置
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    // 编码器模式3 上下边沿都计数  四倍频
    TIM_EncoderInterfaceConfig(TIM2,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 10; // 滤波
    TIM_ICInit(TIM2, &TIM_ICInitStruct);

    // 清除TIM的更新标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // Reset counter
    TIM_SetCounter(TIM2, 0);
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @description: TIM3定时器编码器模式 PA6 和PA7 配置函数
 * @param {uint16_t} psc 预分频值
 * @param {uint16_t} arr 预装载重装值
 * @return {*}
 */
void TIM3_Encoder_Init(uint16_t psc, uint16_t arr)
{
    // 相关结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // GPIO初始化
    MY_GPIO_Init(GPIOA, GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_IN_FLOATING);

    // 时基单元配置
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    // 编码器模式3 上下边沿都计数  四倍频
    TIM_EncoderInterfaceConfig(TIM3,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 10; // 滤波
    TIM_ICInit(TIM3, &TIM_ICInitStruct);

    // 清除TIM的更新标志位
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // Reset counter
    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief 单位时间读取编码器计数
 * @param TIMX 定时器选择 2或4
 * @retval 编码器返回的脉冲值
 */
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;
    switch (TIMX)
    {
    case 2:
        Encoder_TIM = (short)TIM2->CNT;
        // Encoder_TIM= (int)((int16_t)(TIM4->CNT));;
        TIM2->CNT = 0;
        break;
    case 3:
        Encoder_TIM = (short)TIM3->CNT;
        TIM3->CNT = 0;
        break;
    default:
        Encoder_TIM = 0;
    }
    return Encoder_TIM;
}

/**
 * @brief TIM2中断服务函数
 * @param 无
 * @retval 无
 */
void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 溢出中断
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位
    }
}

/**
 * @brief TIM3中断服务函数
 * @param 无
 * @retval 无
 */
void TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) // 溢出中断
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除中断标志位
    }
}

/*****************************************************END OF FILE*********************************************************/
