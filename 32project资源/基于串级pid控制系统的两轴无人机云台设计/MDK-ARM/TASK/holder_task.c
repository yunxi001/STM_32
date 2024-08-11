#include "holder_task.h"
#include "Init.h"

extern moto_info_t motor_info[MOTOR_MAX_NUM];
extern pid_struct_t pitch_Angle_pid;
extern pid_struct_t pitch_speed_pid;
extern pid_struct_t yaw_Angle_pid;
extern pid_struct_t yaw_speed_pid;
extern pid_struct_t BODAN_speed_pid;
volatile short remote_yaw,remote_pitch,remote_BODAN;
extern float Speed_yaw,Speed_pit,Magnetic;
void holder_Task(void const * argument)
{
//	for(;;)
//	{
//		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//		
		if(remote_control.switch_left == 3)
		{
			laser_on();                         //激光开启
			/******************************yaw、pitch轴****************************************************/
			/*****************************角度环**************************************************/
						pid_calc(&yaw_Angle_pid  ,2250,motor_info[0].rotor_angle);
	          pid_calc(&pitch_Angle_pid,2400,motor_info[1].rotor_angle);
			/*****************************角度环**************************************************/
			
            if(remote_control.ch3 <-5|| remote_control.ch3 >5)
			{
				    		remote_yaw=(-remote_control.ch3*2000/660)+2250;     //yaw_control;//偏航角角度期望，直接来源于遥控器打杆量
                pid_calc(&yaw_Angle_pid  ,remote_yaw,motor_info[0].rotor_angle);
			}      
			      if(remote_control.ch4 <-5|| remote_control.ch4 >5) 
			{
				        remote_pitch=(-remote_control.ch4*1500/660)+2400;     //yaw_control;//偏航角角度期望，直接来源于遥控器打杆量
                pid_calc(&pitch_Angle_pid,remote_pitch,motor_info[1].rotor_angle);
			}
			
			/******************************角速度环***********************************************/
				    pid_calc(&yaw_speed_pid  ,yaw_Angle_pid.output,Speed_yaw);
	          pid_calc(&pitch_speed_pid,pitch_Angle_pid.output,Speed_pit);
			/******************************角速度环***********************************************/
		/********************************yaw、pitch轴*****************************************************/
			
			
			
	    /******************************拨弹轮电机*********************************************/
		if(remote_control.switch_right == 3)         
		{
			   remote_BODAN = 0;
				if(remote_control.ch1 > 5)
				{
					remote_BODAN = remote_control.ch1 *(-5);
					pid_calc(&BODAN_speed_pid ,remote_BODAN,motor_info[2].rotor_speed);      //需要测试
				}
		}
//		/********************************拨弹轮电机*********************************************/
//		
//	          
//	  set_holdermotor_voltage(-yaw_speed_pid.output,-pitch_speed_pid.output,BODAN_speed_pid.output);  //驱动电机	

//		
//	 }
	else
	{
    laser_off();		                     //关闭激光
		set_holdermotor_voltage(0,0,0);    	 //原状态停止所有电机
	} 

//	}
}
