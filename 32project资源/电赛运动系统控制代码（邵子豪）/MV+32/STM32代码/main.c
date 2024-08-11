/**** 头文件包含区 ****/
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pid.h"
#include "steer.h"
#include "ultrasonic.h"
#include "tb6612.h"	 
#include "usart.h"
#include "openmv.h"	  
#include "stdlib.h"
#include "key.h"
#include "gray.h"
#include "encoder.h"
#include "string.h"
/*********************/
/**** 全局变量区 ****/
extern float A_D;//距离PID输出
extern int mode; //控制小车工作模式
uint8_t k210_num_start=0; //起始数字识别后存放在这个变量里
uint8_t flag_cross,main_load_cross,left_flag,right_flag,arrive,flag_back = 0;
uint8_t Destination = 0;
uint16_t bb = 40000;
int main(void)
{
    int car=0;
    int ppp=0;
    int st=50;
    float yaws=0;
    int yaws_f=0;
    int x_err=0,y_err=0;
    uint8_t yaw_ge=0;
    uint8_t yaw_shi=0;
    uint8_t flag=0,flag_inc=0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //初始化优先级分组 
	delay_init(168);
	LED_Init();   //LED初始化
    beep_init();  //蜂鸣器初始化
	LCD_Init();   //LCD初始化
    EXTIX_Init(); //按键初始化
    gray_Init();  //灰度传感器初始化
    speed_exti_Init(); //编码器外部中断初始化    
    uart1_init(115200);               //uart1初始化 用于串口上位机打印数据 
    uart2_init(9600);                 //uart2初始化 用于openmv与STM32通信 
    uart3_init(230400);                 //uart3初始化 用于陀螺仪建立串口通信 
    TIM3_PWM_Init(19999,84-1);        //f_hz=50hz T=20ms （专门用于控制舵机）   
    TIM5_Cap_Init(0XFFFFFFFF,84-1);   //超声波输入捕获定时器初始化
    TIM4_PWM_MOTOR_Init(1000-1,4-1);  //驱动电机的PWM定时器初始化
    set_pid_pos_param(&pid_steer,0.12,0,0,0);      //舵机云台追踪色块位置式PID参数设置   
    set_pid_pos_param(&pid_steer_y,0.12,0,0,0);      //舵机云台追踪色块位置式PID参数设置    
    motor_Init(); //TB6612引脚初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE); //清屏
    BEEP=0; 
    beep_o(1);
    LED0=1;
    LED1=1;
    JY901S_yaw_to_0();
    TIM_SetCompare3(TIM3,1500);   // 舵机旋转90度 舵机左转 
//    delay_ms(1000);    
//    TIM_SetCompare3(TIM3,100);   // 舵机旋转90度 舵机右转
//    TIM_SetCompare4(TIM3,244);   // 舵机旋转90度 舵机左转 
//    delay_ms(1000);    
    TIM_SetCompare4(TIM3,1500);   // 舵机旋转90度 舵机右转
Motion_control(stop);
//set_speed(400,0,500,700);
//LCD_ShowString(0,80,"speed_e:",RED,WHITE,16,0);
//LCD_ShowString(0+20,100,"KEY:",RED,WHITE,16,0);
//LCD_ShowString(0+20,60,"speed:",RED,WHITE,16,0);
//LCD_ShowString(0+20,40,"pwm:",RED,WHITE,16,0);
LCD_ShowString(0,20,"x_e:",RED,WHITE,16,0);
LCD_ShowString(0,40,"y_e:",RED,WHITE,16,0);
LCD_ShowString(0,60,"x_o:",RED,WHITE,16,0);
LCD_ShowString(0,80,"y_o:",RED,WHITE,16,0);
//LCD_ShowString(0,0,"OUT:",RED,WHITE,16,0);
//LCD_ShowString(0,40,"EK:",RED,WHITE,16,0);
//usart_send_Ndata(selectSixAxis); //设置成六轴
//usart_send_Ndata(resetZAxis); //偏航角置零
//printf("len=%d",strlen((char*)resetZAxis));
//set_steer_angle(195,135);//最左边60度位置
//set_steer_angle(75,135);//最右边60度位置
//set_steer_angle(135,195);//最上边60度位置
//set_steer_angle(135,75);//最下边60度位置
//set_steer_angle(135,135);//最上边60度位置
    while(1)
    {
        //get_openmv_mid_err();
//        if(abs(pid_steer.OUT)>25)
//        {
//            pid_steer.Kp=1.3;
//        }
//        else
//        {
//            pid_steer.Kp=0.55;
//        }
        //get_steer_mid_err(&x_err,&y_err);
        if(KEY_RES==1)
        {
            steer_to_track(1500); //舵机位置式PID         
        }
//        LCD_ShowIntNum(38,0,pid_steer.OUT,4,BLACK,WHITE,16);
//        LCD_ShowIntNum(38,40,pid_steer.EK,4,BLACK,WHITE,16);
    }
}

