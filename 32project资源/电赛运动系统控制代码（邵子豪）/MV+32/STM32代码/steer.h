#ifndef __STEER_H
#define __STEER_H
/**** 头文件包含区 ****/
#include "sys.h"
/*********************/

/**** 全局变量区 ****/
/*******************/

/**** 函数声明区 ****/
void TIM3_PWM_Init(u32 arr,u32 psc);
void set_steer_angle(uint16_t steer_X,uint16_t steer_Y); //设置舵机角度
void set_steer_angle_pwm(uint16_t steer_X,uint16_t steer_Y);
/*******************/
#endif




