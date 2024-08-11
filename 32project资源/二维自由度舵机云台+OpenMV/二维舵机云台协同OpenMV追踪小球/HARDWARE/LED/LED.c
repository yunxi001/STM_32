/*
 * @Author: _oufen
 * @Date: 2023-07-03 13:31:06
 * @LastEditTime: 2023-07-03 14:29:22
 * @Description:
 */
#include "stm32f10x.h" // Device header
#include "LED.h"

//LED和按键引脚切换  V2.0 板子

/**
 * @brief 	LED初始化 开启时钟 选择端口 初始化GPIO口
 * @param 	无
 * @retval  无
 */
void LED_Init(void)
{
  //MY_GPIO_Init(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12, GPIO_Mode_Out_PP); // LED
	
	MY_GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2, GPIO_Mode_Out_PP); 
                                                                                  // GPIO配置好了之后默认就是低电平

  GPIO_SetBits(GPIOC,  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);   //关闭LED 保持熄灭状态
}

/**
 * @brief 	蜂鸣器初始化 开启时钟 选择端口 初始化GPIO口
 * @param 	无
 * @retval  无
 */
void Buzzer_Init(void)
{
  MY_GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_Out_PP);
  // GPIO配置好了之后默认就是低电平

  // GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2);   //关闭LED 保持熄灭状态
}

/**
 * @brief 	蜂鸣器及LED 初始化 开启时钟 选择端口 初始化GPIO口
 * @param 	无
 * @retval  无
 */
void Other_Init(void)
{
  LED_Init();    // LED初始化
  Buzzer_Init(); // 蜂鸣器初始化
}
