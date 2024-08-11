#include "headfile.h"
int16 actual_speed=0;
int16 Expect_speed=0;
float PID_P=0;  //37
float PID_I=0 ; //   5.60;
int16 gradinets=0;
int16 motor_pulse_speed=0;
uint8 close_flag=0;
uint8 open_flag=0;
int16 right_count=0,left_count=0 ;
int16 Err_dev_speed=0;  //ƫ���ƫ��
int16 Err_speed=0;  //��ǰƫ��
int16 right_motor=0,left_motor=0 ;
extern uint8 stop ;
extern uint8 round_flag;
extern uint8 speed_switch;
void motor_count_pulse()
{
  uint16 temp1=0,temp2=0;
  
  
  //��ȡ�ٶ���Ϣ
  
  temp1 = ftm_count_get(ftm0);
  temp2 = ftm_count_get(ftm1);
  
  
  
  
  //���ݷ����ź��ж����������跽���ź��Ǹߵ�ƽʱΪ��ת
  if(gpio_get(E1))     left_motor= temp1;//�ٶ�ȡ��
  else                 left_motor= -temp1;
  if(gpio_get(H2))     right_motor=-temp2;//�ٶ�ȡ��
  else                right_motor=temp2;

  left_count+= left_motor;
  right_count+= right_motor;
  //����������
  ftm_count_clean(ftm0);
  ftm_count_clean(ftm1);
  
  
  
  //ת��Ϊ�ٶ� ������һȦΪ512 һȦΪ1160
  //        actual_speed_R=(int16)(right_count*50*20/1160.0);
  //        actual_speed_L=(int16)(left_count*50*20/1160.0);
  
  
}
void speed_control()
{
  
  static int16 Err_speed_last=0; //ǰһ��ƫ��
  actual_speed= (left_count+ right_count)/2;
  if(actual_speed<100&&stop==0&&close_flag==0)
  {
    open_flag=1; //���������ܣ�
    close_flag=0;
    switch(speed_switch)
    {
    case 1: motor_pulse_speed=2000;break;
    case 2:motor_pulse_speed=2500;break;
    case 3:motor_pulse_speed=3100;break;
    default:break;
    }
    //150�����ܵ�Ϊ1600~~1700
    
  }
  
  else if(actual_speed>=100||close_flag==1)
  {
    open_flag=0;
    close_flag=1;
    
    Err_speed=Expect_speed-actual_speed;
    
    Err_dev_speed=Err_speed-Err_speed_last;
    
    Err_speed_last=Err_speed;
    
    gradinets=(int) (PID_P*Err_dev_speed+PID_I*Err_speed);
    motor_pulse_speed+=gradinets;
  }
  switch(speed_switch)
  {
  case 1:
    {
      if(motor_pulse_speed>5500)
      {
        motor_pulse_speed=5500 ;
      }
      else if(motor_pulse_speed<-5500)
        motor_pulse_speed=-5500 ;
      break;
    }
  case 2:
    {
      if(motor_pulse_speed>5500)
      {
        motor_pulse_speed=5500 ;
      }
      else if(motor_pulse_speed<-5500)
        motor_pulse_speed=-5500 ;
      break;
    }
    case 3:
    {
      if(motor_pulse_speed>5000)
      {
        motor_pulse_speed=5000 ;
      }
      else if(motor_pulse_speed<-5000)
        motor_pulse_speed=-5000 ;
      break;
    }
    default:break;
  }
  left_count=0;
  right_count=0;
}