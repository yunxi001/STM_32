#ifndef __PID_H
#define __PID_H
#include "sys.h"
typedef struct //λ��ʽPID�ṹ��
{

    float Sv;//�趨ֵ
	float Kp;//����
	float Kd;//΢��
	float Ki;//����
	float EK;//���
	float EK_NEXT;//�ϴ�ƫ��
	float SUM_EK;//��ʷƫ��֮�ͣ����ڻ����ۼ�
	float OUT;//PID�����������PWMռ�ձȣ������ǲ��ƫ�����������¶ȣ��¿�PID��
}PID;
typedef struct
{ 
    float Sv;//�趨ֵ
	float Kp;//����
	float Kd;//΢��
	float Ki;//����
	float EK;//���
	float EK_LAST;//�ϴ�ƫ��    
    float EK_LAST_LAST;//���ϴ�ƫ��
	float OUT; //PWM���
}PID_INC;
typedef struct //λ��ʽPID�ṹ��
{

    float Sv;//�趨ֵ
	float Kp;//����
	float Kd;//΢��
	float Ki;//����
	float EK;//���
	float EK_NEXT;//�ϴ�ƫ��
	float SUM_EK;//��ʷƫ��֮�ͣ����ڻ����ۼ�
	float OUT;//PID�����������PWMռ�ձȣ������ǲ��ƫ�����������¶ȣ��¿�PID��
}PID_POS;
float PID_Increase(PID_INC *pid_inc,float actual_data);
float PID_position(PID_POS *pid_pos,float actual_data);
void set_pid_inc_param(PID_INC *pid_inc,float p,float i,float d,float sv);
void set_pid_pos_param(PID_POS *pid_pos,float p,float i,float d,float sv);
void openmv_to_track(uint16_t v);
float gryo_to_turn(uint16_t v);
void distance_to_hold(uint16_t v);
void motor_speed_control_L(void);
void motor_speed_control_R(void);
void circle_to_turn(uint16_t v);
void steer_to_track(uint16_t angle);
float abs_s(float a);
extern PID_POS pid_openmv;      //����ͷλ��ʽPID�ṹ��
extern PID_POS pid_gyro;        //������λ��ʽPID�ṹ��
extern PID_POS pid_dis;         //���뱣��λ��ʽPID�ṹ��
extern PID_INC pid_speed_L;     //���ֵ���ٶ�����ʽPID�ṹ��
extern PID_INC pid_speed_R;     //���ֵ���ٶ�����ʽPID�ṹ��
extern PID_POS pid_encoder;     //���������Ȧ��λ�û�PID�ṹ��
extern PID_POS pid_steer;       //�����̨׷��ɫ��λ�û�PID�ṹ��
extern PID_POS pid_steer_y;     //�����̨׷��ɫ��λ�û�PID�ṹ��y��
extern int E_flag;//ѭ��ƫ��ֵ

#endif
