/**
 *************************************************************************************************************************
 * @file    control.h
 * @author  oufen
 * @version V1.0
 * @date    2022-09-22
 * @brief   control模块.h文件配置
 *************************************************************************************************************************
 * @attention
 *
 *
 *
 *************************************************************************************************************************
 */

/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef __CONTROL_H
#define __CONTROL_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "sys.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define RESOLUTION_RATIO 13                           // 编码器的分辨率 即编码器的线数 13线
#define RESOULTION_TOTAL_RATIO (RESOLUTION_RATIO * 4) // 经过备频后的分辨率  即四倍频后编码器的脉冲数为260个
#define REDUCTION_RATIO 34                            // 电机的减速比     MG370减速比 1：34  MG310减速比  1：20
#define PID_PERIOD 10                                 /* 定时器周期10ms */

// 舵机角度 初始角度  不是90°
#define SERVO_INIT 85.0

typedef struct // 变量定义结构体
{
    /* data */
    short UnitTime_Motor1Pluse; // 单位时间内电机1脉冲数
    short UnitTime_Motor2Pluse; // 单位时间内电机2脉冲数

    float Sigma_Motor1_Pluse; // Motor1累积脉冲
    float Sigma_Motor2_Pluse; // Motor2累积脉冲

    float Current_Motor1_Distance; // 左电机当前距离
    float Current_Motor2_Distance; // 右电机当前距离

    float Line_Out; // 巡线补偿

    int Motor1_PWM_Out; // 电机1PWM输出
    int Motor2_PWM_Out; // 电机2PWM输出

    uint16_t Timer_threshold_value; // 定时阈值   可以定时 蜂鸣器响啥的 灯亮啥的

} Param_InitTypedef;

typedef struct // 标志位结构体
{
    /* data */
    uint8_t Is_Enable_Motor; // 电机使能
    uint8_t Stop_Car;        // 小车已停下标志位

    uint8_t Start_Count; // 开始计时
    uint8_t Is_Timer_Up; // 是否定时时间到

} Flag_InitTypedef;

/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
extern Param_InitTypedef Param;
extern Flag_InitTypedef Flag;

#endif /* __XXX_H */

/*****************************************************END OF FILE*********************************************************/
