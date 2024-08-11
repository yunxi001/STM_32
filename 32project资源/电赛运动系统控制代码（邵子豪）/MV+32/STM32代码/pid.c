#include <pid.h>
#include <tb6612.h>
#include <lcd.h>
#include "ultrasonic.h"
#include "usart.h"
#include "openmv.h"
#include "steer.h"
#include "encoder.h"
/********** 位置环PID变量 ********/
PID_POS pid_steer;       //舵机云台追踪色块位置环PID结构体x轴
PID_POS pid_steer_y;     //舵机云台追踪色块位置环PID结构体y轴
/********** 位置环PID变量 ********/

/**
 *@brief 速度增量式PID函数
 *@param pid_inc:增量式PID参数结构体 actual_data:速度实际值 set_data：速度目标值
 *@retvl OUT:最终输出的PWM电压
 */
float PID_Increase(PID_INC *pid_inc,float actual_data)
{   
	pid_inc->EK =pid_inc->Sv - actual_data;	// 计算当前速度误差
	pid_inc->OUT +=  
                pid_inc->Kp * (pid_inc->EK - pid_inc->EK_LAST) //比例P
			  + pid_inc->Ki *  pid_inc->EK  //积分I
			  + pid_inc->Kd * (pid_inc->EK - 2 * pid_inc->EK_LAST + pid_inc->EK_LAST_LAST);  //微分D	
	pid_inc->EK_LAST_LAST = pid_inc->EK_LAST;	// 更新上上误差
	pid_inc->EK_LAST = pid_inc->EK;		  	// 更新上次误差
	return pid_inc->OUT;	// 返回增量
}

/**
 *@brief 位置式PID函数（可用于陀螺仪 摄像头中线 红外循迹 灰度循迹）
 *@param pid_pos:位置式PID参数结构体 actual_data:位置实际值
 *@retvl OUT:最终输出的PWM电压
 */
float PID_position(PID_POS *pid_pos,float actual_data)
{   
	pid_pos->EK =pid_pos->Sv - actual_data;	// 计算当前速度误差
    pid_pos->SUM_EK+=pid_pos->EK; //误差累加
	pid_pos->OUT = 
                pid_pos->Kp * (pid_pos->EK) //比例P
			  + pid_pos->Ki *  pid_pos->SUM_EK //积分I
			  + pid_pos->Kd * (pid_pos->EK - pid_pos->EK_NEXT);  //微分D	
	pid_pos->EK_NEXT = pid_pos->EK;		  	// 更新上次误差
	return pid_pos->OUT;	// 返回增量
}

/**
 *@brief 设置增量式PID的Kp,Ki,Kd数值以及目标值
 *@param pid_inc:增量式PID参数结构体 p,i,d：比例-积分-微分 sv:目标设定值
 */
void set_pid_inc_param(PID_INC *pid_inc,float p,float i,float d,float sv)
{
    pid_inc->Kp=p/1000000;
    pid_inc->Ki=i/1000000;
    pid_inc->Kd=d/1000000;
    pid_inc->Sv=sv;
}

/**
 *@brief 设置位置式PID的Kp,Ki,Kd数值以及目标值
 *@param pid_pos:位置式PID参数结构体 p,i,d：比例-积分-微分 sv:目标设定值
 */
void set_pid_pos_param(PID_POS *pid_pos,float p,float i,float d,float sv)
{
    pid_pos->Kp=p;
    pid_pos->Ki=i;
    pid_pos->Kd=d;
    pid_pos->Sv=sv;
}
/**
 *@brief 绝对值函数
 */
float abs_s(float a)
{
    float temp;
    if(a>=0)
    {
        temp=a;
    }
    if(a<0)
    {
        temp=-a;
    }
    return temp;
}

/*****************************这里是云台重点代码************/
/************** 5-舵机追踪色块位置环位置式PID **************/
//angle为舵机复位角度,中心位置
void steer_to_track(uint16_t angle)
{
    static int x_er=0,y_er=0; //定义两个变量去接收MV传送过来的偏差值
    static float pid_steer_out=0; //X轴PID输出量
    static float pid_steer_out_y=0; //Y轴PID输出量
    get_steer_mid_err(&x_er,&y_er); //调用数据接收函数，接收OPENMV发来的偏差
    pid_steer_out+=PID_position(&pid_steer,x_er); //X轴增量位置式PID输出
    if(pid_steer_out>=1900)    //舵机角度限幅，以防转多了卡住
		pid_steer_out=1900;
	if(pid_steer_out<-1000)
		pid_steer_out=-1000;	
     LCD_ShowIntNum(38,60,pid_steer_out,4,BLACK,WHITE,16);
    
    pid_steer_out_y+=PID_position(&pid_steer_y,y_er); 
    if(pid_steer_out_y>=1900)   //舵机角度限幅
		pid_steer_out_y=1900;
	if(pid_steer_out_y<-1000)
		pid_steer_out_y=-1000;	
     LCD_ShowIntNum(38,80,pid_steer_out_y,4,BLACK,WHITE,16);//Y轴增量位置式PID输出
     set_steer_angle_pwm(angle+pid_steer_out,angle+pid_steer_out_y); //将PID输出转为舵机角度使用   
    // set_steer_angle_pwm(angle,pid_steer_out_y); //将PID输出转为舵机角度使用      
}
/*---------------------
负偏差往左边拐
正偏差往右边拐
---------------------*/


