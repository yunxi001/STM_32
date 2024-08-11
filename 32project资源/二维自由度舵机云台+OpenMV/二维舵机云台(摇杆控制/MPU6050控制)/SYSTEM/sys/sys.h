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
} GPIO_Level; // GPIO�ڵ�״̬

// 0,��֧��ucos
// 1,֧��ucos
#define SYSTEM_SUPPORT_OS 0 // ����ϵͳ�ļ����Ƿ�֧��UCOS

// λ������,ʵ��51���Ƶ�GPIO���ƹ���
// ����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
// IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// IO�ڵ�ַӳ��
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

// IO�ڲ���,ֻ�Ե�һ��IO��!
// ȷ��n��ֵС��16!
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n) // ���
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)	 // ����

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n) // ���
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)	 // ����

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n) // ���
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)	 // ����

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr, n) // ���
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr, n)	 // ����

#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr, n) // ���
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr, n)	 // ����

#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr, n) // ���
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr, n)	 // ����

#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr, n) // ���
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr, n)	 // ����

/**************************************************************/
// Ex_NVIC_Configר�ö���
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6
#define GPIO_H 7
#define GPIO_I 8

#define FTIR 1 // �½��ش���
#define RTIR 2 // �����ش���

// GPIO����ר�ú궨��
#define GPIO_MODE_IN 0	// ��ͨ����ģʽ
#define GPIO_MODE_OUT 1 // ��ͨ���ģʽ
#define GPIO_MODE_AF 2	// AF����ģʽ
#define GPIO_MODE_AIN 3 // ģ������ģʽ

#define GPIO_SPEED_2M 0	  // GPIO�ٶ�2Mhz
#define GPIO_SPEED_25M 1  // GPIO�ٶ�25Mhz
#define GPIO_SPEED_50M 2  // GPIO�ٶ�50Mhz
#define GPIO_SPEED_100M 3 // GPIO�ٶ�100Mhz

#define GPIO_PUPD_NONE 0 // ����������
#define GPIO_PUPD_PU 1	 // ����
#define GPIO_PUPD_PD 2	 // ����
#define GPIO_PUPD_RES 3	 // ����

#define GPIO_OTYPE_PP 0 // �������
#define GPIO_OTYPE_OD 1 // ��©���

// GPIO���ű�Ŷ���
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

// ����Ϊ��ຯ��
void WFI_SET(void);		 // ִ��WFIָ��
void INTX_DISABLE(void); // �ر������ж�
void INTX_ENABLE(void);	 // ���������ж�
void MSR_MSP(u32 addr);	 // ���ö�ջ��ַ

void MY_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef Mode);					// GPIO�˿�ѡ�� ģʽ����
void GPIO_Set(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_Level LEVEL);							// GPIO�˿ڵ�ƽ����
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM);													// �ⲿ�ж����ú���
void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group); // NVIC��ʼ�� ��NVIC����

float KLM(float Z); // �������˲�

#endif
