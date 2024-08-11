#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern uint8_t k210_num_flag; //����ʶ����ձ�־λ��������յ�����1����k210_num_flag=1,Ϊȫ�ֱ���
extern unsigned char selectSixAxis[];
extern unsigned char selectNineAxis[];
extern unsigned char resetZAxis[];
extern unsigned char setAngleReference[];
//����봮���жϽ��գ��벻Ҫע�����º궨��
extern int flag;/* ����С���ٶȺͷ���ı��� */
extern int SR04_FLAG;
extern uint8_t rec_data[11];//�����ǽǶ����ݰ�
void uart1_init(u32 bound); //������Openmvͨ�Ž�������
void uart2_init(u32 bound); //���ڽ��������ǽǶ�����
void uart3_init(u32 bound); //���ڽ���k210������
void get_yaw(uint8_t rec_data[],float *Yaw); //��ȡ��������ݺ���
void yaw_claer_to_0(void); //����ǻ�׼�Ƕ�����
void yaw_pid_go_straight(void); //����С����ֱ�ߺ���
void angle_to_turn(uint8_t direc,float angle); //ת��̶��ǶȺ���
void USART2_IRQHandler(void);
void USART1_IRQHandler(void);  
void USART3_IRQHandler(void);  
void usart_send_Ndata(unsigned char data[]);
void JY901S_yaw_to_0(void);
void K210_send_con(char command);
#endif


