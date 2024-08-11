#include "PID.h"

	float pitch,roll,yaw; 	//欧拉角	
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;//四元数
	float hy=0,hp=0;   			//参考系下云台角
	float by,bp,br;         //参考系下姿态角（也就是欧拉角）
	float bhy=0,bhp=0,bhr=0;//机体系下云台角
	float Kd1=10.0;					//舵机1补偿系数	
	float Kd2=10.0;					//舵机2补偿系数	
 /*********************************************************/
 

int main(void)
{	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(19200);	 //串口初始化为19200	 
	delay_init();			 //延时初始化
	LED_Init();		  		//初始化LED
	Servo_Init();				//舵机初始化
	MPU_Init();					//初始化MPU6050
	
	printf("mpu6050 set OK!\r\n");	
	
	while(mpu_dmp_init());
	printf("dmp set OK!\r\n");
	
	TIM2_Int_Init(99,7199);//10Khz的计数频率，计数到100为10ms   	 
	while(1);
}

 /*********************************************************/
//  这部分代码可以获取陀螺仪原始数据
//	short aacx,aacy,aacz;		//加速度传感器原始数据
//	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
//	short temp;					//温度	

//	temp=MPU_Get_Temperature();	//得到温度值
//	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
/********************************************/

//定时器2中断服务程序 周期：10ms  频率：100Hz
void TIM2_IRQHandler(void)   //TIM2中断
{
	static int t=0;
	u8 button;
	float bhylast=0, bhplast=0;	     //机体系下上一时刻的云台角
	float bhyincre=0, bhpincre=0;      //机体系下上一时刻的云台角增量
	float bhycomp = 0, bhrcomp = 0;                 //舵机转角度补偿量，与角速度有关
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)     //检查TIM2更新中断发生与否
	{	 TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );    //清除更新中断标志 	
			
			bhylast=bhy, bhplast=bhp;		   					//记录机体系下上一时刻的云台角
			if(mpu_dmp_get_data(&roll,&pitch,&yaw)==0)   //更新姿态角(由于安装问题实际roll和pitch互换了)
			{ 
				yaw=-yaw, pitch=-pitch, roll=-roll;     //由于安装问题实际yaw,roll,pitch均倒向			
				
				CalHeadDegree();               //利用欧拉角旋转矩阵计算机体系下的云台角:hby,hbp
				
				bhyincre = bhy - bhylast;      //计算云台角速度
				bhpincre = bhp - bhplast;    
						
				bhycomp = Kd1*bhyincre;         //计算舵机1补偿量
				bhrcomp = Kd2*bhpincre;  				//计算舵机2补偿量
				
				HeadTurnToXY(bhy+bhycomp ,bhp+bhrcomp);    //进行角度补偿输出
				
				t++;
			  if(t>=10)t=0,	LED0=!LED0;  //每10*10=100ms闪动
				
//				printf("%0.2f\t %0.2f\t %0.2f\t %0.2f \r\n",q0,q1,q2,q3);	
//				printf("bhy:%3.0f\t bhp:%3.0f\t",bhy,bhp);
//				printf("yaw：%3.0f\t pitch：%3.0f\t roll：%3.0f \r\n",yaw,pitch,roll);		
//				printf("bhp:%f\tbhy:%f\r\n",bhp,bhy);	
			}	
			
		 if(USART_RX_STA&0x8000)    //通过串口调整Kd值和参考系下云台角
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
//				printf("hy:%3.1f\t hp:%3.1f\t Kd1:%2.1f\tKd2:%2.1f\r\n",hy,hp,Kd1,Kd2);						//打印调整后的云台角和Kd值
				USART_RX_STA=0;
			}

	}
}
 