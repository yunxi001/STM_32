#ifndef __SYS_H
#define __SYS_H

#include "stm32f10x.h"
#include "delay.h"
#include "LED.h"
#include "motor.h"
#include "encoder.h"
#include "oled.h"
#include "nvic.h"
#include "timer6.h"
#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "usart.h"
#include "pid.h"
#include "control.h"
#include "key.h"
#include "servo.h"
#include "rocker.h"

////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef enum
{
	LOW,
	HIGH,
	TURN
} GPIO_Level; // GPIO口的状态

// 0,不支持ucos
// 1,支持ucos
#define SYSTEM_SUPPORT_OS 0 // 定义系统文件夹是否支持UCOS

// 位带操作,实现51类似的GPIO控制功能
// 具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
// IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// IO口地址映射
#define GPIOA_ODR_Addr (GPIOA_BASE + 12) // 0x4001080C
#define GPIOB_ODR_Addr (GPIOB_BASE + 12) // 0x40010C0C
#define GPIOC_ODR_Addr (GPIOC_BASE + 12) // 0x4001100C
#define GPIOD_ODR_Addr (GPIOD_BASE + 12) // 0x4001140C
#define GPIOE_ODR_Addr (GPIOE_BASE + 12) // 0x4001180C
#define GPIOF_ODR_Addr (GPIOF_BASE + 12) // 0x40011A0C
#define GPIOG_ODR_Addr (GPIOG_BASE + 12) // 0x40011E0C

#define GPIOA_IDR_Addr (GPIOA_BASE + 8) // 0x40010808
#define GPIOB_IDR_Addr (GPIOB_BASE + 8) // 0x40010C08
#define GPIOC_IDR_Addr (GPIOC_BASE + 8) // 0x40011008
#define GPIOD_IDR_Addr (GPIOD_BASE + 8) // 0x40011408
#define GPIOE_IDR_Addr (GPIOE_BASE + 8) // 0x40011808
#define GPIOF_IDR_Addr (GPIOF_BASE + 8) // 0x40011A08
#define GPIOG_IDR_Addr (GPIOG_BASE + 8) // 0x40011E08

// IO口操作,只对单一的IO口!
// 确保n的值小于16!
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n) // 输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)	 // 输入

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n) // 输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)	 // 输入

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n) // 输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)	 // 输入

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr, n) // 输出
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr, n)	 // 输入

#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr, n) // 输出
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr, n)	 // 输入

#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr, n) // 输出
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr, n)	 // 输入

#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr, n) // 输出
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr, n)	 // 输入

/**************************************************************/
// Ex_NVIC_Config专用定义
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6
#define GPIO_H 7
#define GPIO_I 8

#define FTIR 1 // 下降沿触发
#define RTIR 2 // 上升沿触发

// GPIO设置专用宏定义
#define GPIO_MODE_IN 0	// 普通输入模式
#define GPIO_MODE_OUT 1 // 普通输出模式
#define GPIO_MODE_AF 2	// AF功能模式
#define GPIO_MODE_AIN 3 // 模拟输入模式

#define GPIO_SPEED_2M 0	  // GPIO速度2Mhz
#define GPIO_SPEED_25M 1  // GPIO速度25Mhz
#define GPIO_SPEED_50M 2  // GPIO速度50Mhz
#define GPIO_SPEED_100M 3 // GPIO速度100Mhz

#define GPIO_PUPD_NONE 0 // 不带上下拉
#define GPIO_PUPD_PU 1	 // 上拉
#define GPIO_PUPD_PD 2	 // 下拉
#define GPIO_PUPD_RES 3	 // 保留

#define GPIO_OTYPE_PP 0 // 推挽输出
#define GPIO_OTYPE_OD 1 // 开漏输出

// GPIO引脚编号定义
#define PIN0 1 << 0
#define PIN1 1 << 1
#define PIN2 1 << 2
#define PIN3 1 << 3
#define PIN4 1 << 4
#define PIN5 1 << 5
#define PIN6 1 << 6
#define PIN7 1 << 7
#define PIN8 1 << 8
#define PIN9 1 << 9
#define PIN10 1 << 10
#define PIN11 1 << 11
#define PIN12 1 << 12
#define PIN13 1 << 13
#define PIN14 1 << 14
#define PIN15 1 << 15

// 以下为汇编函数
void WFI_SET(void);		 // 执行WFI指令
void INTX_DISABLE(void); // 关闭所有中断
void INTX_ENABLE(void);	 // 开启所有中断
void MSR_MSP(u32 addr);	 // 设置堆栈地址

void MY_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef Mode);					// GPIO端口选择 模式设置
void GPIO_Set(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_Level LEVEL);							// GPIO端口电平设置
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM);													// 外部中断配置函数
void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group); // NVIC初始化 和NVIC分组

float KLM(float Z); // 卡尔曼滤波

#endif
