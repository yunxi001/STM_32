/*
 * @Author: _oufen
 * @Date: 2023-07-03 13:31:06
 * @LastEditTime: 2023-07-27 11:06:59
 * @Description:
 */

#include "stm32f10x.h" // Device header
#include "rocker.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @attention
 * 采用的是普通的摇杆模块
 * PS2游戏手柄
 * 对应引脚分别是 GND 5V  URX URY SW
 * 其中 URX-PA1  URY-PA2
 * 使用ADC采样电压值得出x轴和y轴的变化
 * 这里有个大坑   注意双方的参考电压要相同   多采样时需要采样校准
 */

// 初始时x轴为中点时

/**
 * @brief 	摇杆模块初始化
 * @param 	无
 * @retval  无
 */
void Rocker_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  RCC_ADCCLKConfig(RCC_PCLK2_Div6);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  while (ADC_GetResetCalibrationStatus(ADC1) == SET)
    ;
  ADC_StartCalibration(ADC1);
  while (ADC_GetCalibrationStatus(ADC1) == SET)
    ;
}

/**
 * @brief 	读取ADC采样后的数据
 * @param 	无
 * @retval  无
 */
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
  ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
    ;
  return ADC_GetConversionValue(ADC1);
}

/**
 * @brief 	将ADC采样后的数据转换为电压值
 * @param 	无
 * @retval  无
 */
float Voltage_Translate(uint16_t value)
{
  float Voltage;
  Voltage = (float)value * (3.3 / 4096);

  return Voltage;
}
