/*
 * @Author: _oufen
 * @Date: 2023-07-03 13:31:06
 * @LastEditTime: 2023-07-13 09:47:41
 * @Description:
 */
#ifndef __PID_H__
#define __PID_H__

#include "sys.h"

typedef struct
{
    float target_val; // 目标值
    float actual_val; // 实际值
    float err;        // 定义偏差值
    float err_last;   // 定义上一个偏差值
    float Kp, Ki, Kd; // 定义比例、积分、微分系数
    float integral;   // 定义积分值
} _pid;

extern _pid pid_speed1, pid_speed2;
extern _pid pid_location; // 巡线环 结构体变量
extern _pid pid_angle;    // 角度环 结构体变量

extern _pid pid_servo_X, pid_servo_Y; // 舵机转向环  X轴  Y轴

extern float Pid_speed1,
    Pid_speed2, Pid_Actual_angle; // 速度环的实际值

void PID_param_init(void);

float speed_pid_realize(_pid *pid, float actual_val);    // 速度环pid实现
float location_pid_realize(_pid *pid, float actual_val); // 巡线环pid实现
float Angle_pid_control(void);                           // 角度换pid实现

float speed1_pid_control(void);                       // 速度环输出函数
float speed2_pid_control(void);                       // 速度环输出函数
float Angle_pid_realize(_pid *pid, float actual_val); // 角度环输出函数

float Servo_X_pid_control(void);
float Servo_Y_pid_control(void);

void set_pid_target(_pid *pid, float temp_val);

void set_p_i_d(_pid *pid, float p, float i, float d);

float get_pid_actual(_pid *pid); // 获取实际值
float get_pid_target(_pid *pid); // 获取期望值

#endif
