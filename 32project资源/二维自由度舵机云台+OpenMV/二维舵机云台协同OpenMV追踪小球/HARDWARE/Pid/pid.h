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
    float target_val; // Ŀ��ֵ
    float actual_val; // ʵ��ֵ
    float err;        // ����ƫ��ֵ
    float err_last;   // ������һ��ƫ��ֵ
    float Kp, Ki, Kd; // ������������֡�΢��ϵ��
    float integral;   // �������ֵ
} _pid;

extern _pid pid_speed1, pid_speed2;
extern _pid pid_location; // Ѳ�߻� �ṹ�����
extern _pid pid_angle;    // �ǶȻ� �ṹ�����

extern _pid pid_servo_X, pid_servo_Y; // ���ת��  X��  Y��

extern float Pid_speed1,
    Pid_speed2, Pid_Actual_angle; // �ٶȻ���ʵ��ֵ

void PID_param_init(void);

float speed_pid_realize(_pid *pid, float actual_val);    // �ٶȻ�pidʵ��
float location_pid_realize(_pid *pid, float actual_val); // Ѳ�߻�pidʵ��
float Angle_pid_control(void);                           // �ǶȻ�pidʵ��

float speed1_pid_control(void);                       // �ٶȻ��������
float speed2_pid_control(void);                       // �ٶȻ��������
float Angle_pid_realize(_pid *pid, float actual_val); // �ǶȻ��������

float Servo_X_pid_control(void);
float Servo_Y_pid_control(void);

void set_pid_target(_pid *pid, float temp_val);

void set_p_i_d(_pid *pid, float p, float i, float d);

float get_pid_actual(_pid *pid); // ��ȡʵ��ֵ
float get_pid_target(_pid *pid); // ��ȡ����ֵ

#endif
