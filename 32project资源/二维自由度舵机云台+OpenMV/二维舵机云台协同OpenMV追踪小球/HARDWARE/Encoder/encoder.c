/*
 * @Author: _oufen
 * @Date: 2023-04-15 13:45:19
 * @LastEditTime: 2023-04-15 15:37:15
 * @Description: ���������ú��� ʹ��Timer2��Timer3�ı�����ģʽ����ȡ����������
 */

/* Includes -------------------------------------------------------------------------------------------------------------*/

#include "encoder.h"

/* Define -----------------------------------------------------------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------------------------------------------------------*/
/**
 * @description: TIM2��ʱ��������ģʽ PA0 ��PA1���ú���
 * @param {uint16_t} psc Ԥ��Ƶֵ
 * @param {uint16_t} arr Ԥװ����װֵ
 * @return {*}
 */
void TIM2_Encoder_Init(uint16_t psc, uint16_t arr)
{
    // ��ؽṹ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    // ����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // GPIO��ʼ��
    MY_GPIO_Init(GPIOA, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_IN_FLOATING);

    // ʱ����Ԫ����
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    // ������ģʽ3 ���±��ض�����  �ı�Ƶ
    TIM_EncoderInterfaceConfig(TIM2,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 10; // �˲�
    TIM_ICInit(TIM2, &TIM_ICInitStruct);

    // ���TIM�ĸ��±�־λ
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // Reset counter
    TIM_SetCounter(TIM2, 0);
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @description: TIM3��ʱ��������ģʽ PA6 ��PA7 ���ú���
 * @param {uint16_t} psc Ԥ��Ƶֵ
 * @param {uint16_t} arr Ԥװ����װֵ
 * @return {*}
 */
void TIM3_Encoder_Init(uint16_t psc, uint16_t arr)
{
    // ��ؽṹ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    // ����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // GPIO��ʼ��
    MY_GPIO_Init(GPIOA, GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_IN_FLOATING);

    // ʱ����Ԫ����
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    // ������ģʽ3 ���±��ض�����  �ı�Ƶ
    TIM_EncoderInterfaceConfig(TIM3,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 10; // �˲�
    TIM_ICInit(TIM3, &TIM_ICInitStruct);

    // ���TIM�ĸ��±�־λ
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // Reset counter
    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief ��λʱ���ȡ����������
 * @param TIMX ��ʱ��ѡ�� 2��4
 * @retval ���������ص�����ֵ
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
 * @brief TIM2�жϷ�����
 * @param ��
 * @retval ��
 */
void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // ����ж�
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // ����жϱ�־λ
    }
}

/**
 * @brief TIM3�жϷ�����
 * @param ��
 * @retval ��
 */
void TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) // ����ж�
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // ����жϱ�־λ
    }
}

/*****************************************************END OF FILE*********************************************************/
