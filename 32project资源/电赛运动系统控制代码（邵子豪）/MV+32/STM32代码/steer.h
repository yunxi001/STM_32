#ifndef __STEER_H
#define __STEER_H
/**** ͷ�ļ������� ****/
#include "sys.h"
/*********************/

/**** ȫ�ֱ����� ****/
/*******************/

/**** ���������� ****/
void TIM3_PWM_Init(u32 arr,u32 psc);
void set_steer_angle(uint16_t steer_X,uint16_t steer_Y); //���ö���Ƕ�
void set_steer_angle_pwm(uint16_t steer_X,uint16_t steer_Y);
/*******************/
#endif




