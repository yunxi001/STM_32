#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern uint8_t k210_num_flag; //数字识别接收标志位，比如接收到数字1就是k210_num_flag=1,为全局变量
extern unsigned char selectSixAxis[];
extern unsigned char selectNineAxis[];
extern unsigned char resetZAxis[];
extern unsigned char setAngleReference[];
//如果想串口中断接收，请不要注释以下宏定义
extern int flag;/* 控制小车速度和方向的变量 */
extern int SR04_FLAG;
extern uint8_t rec_data[11];//陀螺仪角度数据包
void uart1_init(u32 bound); //用于与Openmv通信接收数据
void uart2_init(u32 bound); //用于接收陀螺仪角度数据
void uart3_init(u32 bound); //用于接收k210的数据
void get_yaw(uint8_t rec_data[],float *Yaw); //获取航向角数据函数
void yaw_claer_to_0(void); //航向角基准角度置零
void yaw_pid_go_straight(void); //控制小车走直线函数
void angle_to_turn(uint8_t direc,float angle); //转向固定角度函数
void USART2_IRQHandler(void);
void USART1_IRQHandler(void);  
void USART3_IRQHandler(void);  
void usart_send_Ndata(unsigned char data[]);
void JY901S_yaw_to_0(void);
void K210_send_con(char command);
#endif


