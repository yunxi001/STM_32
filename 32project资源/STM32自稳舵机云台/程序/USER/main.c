#include "PID.h"

	float pitch,roll,yaw; 	//ŷ����	
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;//��Ԫ��
	float hy=0,hp=0;   			//�ο�ϵ����̨��
	float by,bp,br;         //�ο�ϵ����̬�ǣ�Ҳ����ŷ���ǣ�
	float bhy=0,bhp=0,bhr=0;//����ϵ����̨��
	float Kd1=10.0;					//���1����ϵ��	
	float Kd2=10.0;					//���2����ϵ��	
 /*********************************************************/
 

int main(void)
{	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(19200);	 //���ڳ�ʼ��Ϊ19200	 
	delay_init();			 //��ʱ��ʼ��
	LED_Init();		  		//��ʼ��LED
	Servo_Init();				//�����ʼ��
	MPU_Init();					//��ʼ��MPU6050
	
	printf("mpu6050 set OK!\r\n");	
	
	while(mpu_dmp_init());
	printf("dmp set OK!\r\n");
	
	TIM2_Int_Init(99,7199);//10Khz�ļ���Ƶ�ʣ�������100Ϊ10ms   	 
	while(1);
}

 /*********************************************************/
//  �ⲿ�ִ�����Ի�ȡ������ԭʼ����
//	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
//	short gyrox,gyroy,gyroz;	//������ԭʼ����
//	short temp;					//�¶�	

//	temp=MPU_Get_Temperature();	//�õ��¶�ֵ
//	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
//	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
/********************************************/

//��ʱ��2�жϷ������ ���ڣ�10ms  Ƶ�ʣ�100Hz
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	static int t=0;
	u8 button;
	float bhylast=0, bhplast=0;	     //����ϵ����һʱ�̵���̨��
	float bhyincre=0, bhpincre=0;      //����ϵ����һʱ�̵���̨������
	float bhycomp = 0, bhrcomp = 0;                 //���ת�ǶȲ�����������ٶ��й�
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)     //���TIM2�����жϷ������
	{	 TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );    //��������жϱ�־ 	
			
			bhylast=bhy, bhplast=bhp;		   					//��¼����ϵ����һʱ�̵���̨��
			if(mpu_dmp_get_data(&roll,&pitch,&yaw)==0)   //������̬��(���ڰ�װ����ʵ��roll��pitch������)
			{ 
				yaw=-yaw, pitch=-pitch, roll=-roll;     //���ڰ�װ����ʵ��yaw,roll,pitch������			
				
				CalHeadDegree();               //����ŷ������ת����������ϵ�µ���̨��:hby,hbp
				
				bhyincre = bhy - bhylast;      //������̨���ٶ�
				bhpincre = bhp - bhplast;    
						
				bhycomp = Kd1*bhyincre;         //������1������
				bhrcomp = Kd2*bhpincre;  				//������2������
				
				HeadTurnToXY(bhy+bhycomp ,bhp+bhrcomp);    //���нǶȲ������
				
				t++;
			  if(t>=10)t=0,	LED0=!LED0;  //ÿ10*10=100ms����
				
//				printf("%0.2f\t %0.2f\t %0.2f\t %0.2f \r\n",q0,q1,q2,q3);	
//				printf("bhy:%3.0f\t bhp:%3.0f\t",bhy,bhp);
//				printf("yaw��%3.0f\t pitch��%3.0f\t roll��%3.0f \r\n",yaw,pitch,roll);		
//				printf("bhp:%f\tbhy:%f\r\n",bhp,bhy);	
			}	
			
		 if(USART_RX_STA&0x8000)    //ͨ�����ڵ���Kdֵ�Ͳο�ϵ����̨��
			{					   		
				button=USART_RX_BUF[0];
				switch(button)
				{
					case 'Q':Kd1+=0.1;break;
					case 'E':Kd1-=0.1;break;
					case 'Z':Kd2+=0.1;break;
					case 'C':Kd2-=0.1;break;
					case 'A':hy-=10;  break;
					case 'D':hy+=10;  break;
					case 'W':hp+=10;  break;
					case 'S':hp-=10;  break;
				}
//				printf("hy:%3.1f\t hp:%3.1f\t Kd1:%2.1f\tKd2:%2.1f\r\n",hy,hp,Kd1,Kd2);						//��ӡ���������̨�Ǻ�Kdֵ
				USART_RX_STA=0;
			}

	}
}
 