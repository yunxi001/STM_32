#include "steer.h"	   
#include "delay.h"
#include "all.h"
/*-----------------------------------------
舵机A-y轴方向舵机，就是上层第一个舵机，通道4控制这个舵机
测试角度代码：
TIM_SetCompare4(TIM3,1500);   // 舵机旋转135度停在正中间
TIM_SetCompare4(TIM3,2500);   // 舵机往上旋转到270度 
TIM_SetCompare4(TIM3,500);    // 舵机往下旋转到0度

舵机B-x轴方向舵机，就是下层第二个舵机，通道3控制这个舵机
测试角度代码：
TIM_SetCompare3(TIM3,1500);   // 舵机旋转135度停在正中间
TIM_SetCompare3(TIM3,2500);   // 舵机往左旋转到270度 
TIM_SetCompare3(TIM3,500);    // 舵机往右旋转到0度
-------------------------------------------*/
char source[10];   
int tractdata_x=0; 
int tractdata_y=0;   
int real_data_x=0;     
int real_data_y=0;    
int tractdata=0;   
int real_data=0;  
uint8_t t,i;         /* 变量用于循环 */
uint16_t len;        /* 变量用于串口接收数据的字节数 */
/**
 *@brief 驱动舵机（SG90舵机 0-180 °）
 *@breif 舵机B-PB0 TIM3_CH3通道 舵机A-PB1 TIM3_CH4通道
 *@param arr：自动重转载值 psc：预分频系数
 */
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); //GPIO-PB0、GPIO-PB1复用为定时器3
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //GPIOB0 GPIOB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB0 PB1
    
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器23
	
	//初始化TIM3 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3 TIM3 OC4
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR4上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3									  
}  
/**
 @brief :设置舵机角度函数
 @brief ：分别为X轴舵机和Y轴舵机
 */
void set_steer_angle(uint16_t steer_X,uint16_t steer_Y)
{
     TIM_SetCompare4(TIM3,steer_Y/0.135+500); //y轴舵机
     TIM_SetCompare3(TIM3,steer_X/0.135+500); //x轴舵机
}
/**
 @brief :设置舵机角度函数
 @brief ：分别为X轴舵机和Y轴舵机
 */
void set_steer_angle_pwm(uint16_t steer_X,uint16_t steer_Y)
{
     TIM_SetCompare4(TIM3,steer_Y); //y轴舵机
     TIM_SetCompare3(TIM3,steer_X); //x轴舵机
}


/**
 *@brief STM32从OPENMV串口x和y轴偏差
 *@retval 返回中线偏差值
 *@param x_error:接收X轴数据偏差 y_error:接收Y轴数据偏差 
 */
/*这里的接收串口数据，你可以去正点原子的串口视频了解，不多说，反正能接受数据就行*/
void get_steer_mid_err(int *x_error,int *y_error)
{
    if(USART_RX_STA&0x8000) //接收到一帧数据
    {					   
        len=USART_RX_STA&0x3fff;/* 得到此次接收到的数据长度 */
        for(t=0;t<len;t++) //取值，MV发了8个数字字符过来，现在解析它
        {
            source[t]=USART_RX_BUF[t];  /* 将BUF的串口数据转移到source数组 */
            //千位*1000+百位*100+十位*10+个位，是这个原理
            tractdata_x=(source[0]-'0')*1000+(source[1]-'0')*100+(source[2]-'0')*10+(source[3]-'0'); //接收色块x轴偏差
            tractdata_y=(source[4]-'0')*1000+(source[5]-'0')*100+(source[6]-'0')*10+(source[7]-'0'); //接收色块y轴偏差            
        }
        
        /* 因为发送来的数据是+100来的，所以跟100比较可以判断发来的是不是负数，以下代码就是这个作用 */
         if(tractdata_x>100)
        {
            real_data_x=-(tractdata_x-100);
        }
        if(real_data_x==100)
        {
            real_data_x=0;
        }
        else if(tractdata_x<100)
        {
            real_data_x=tractdata_x;
        }
        if(real_data_x>=0)
        {
        LCD_ShowString(38+30+32,0+20,"+",RED,WHITE,16,0);
        LCD_ShowIntNum(38+30+32+8,0+20,real_data_x,2,GREEN,WHITE,16); 
        }
        else if(real_data_x<0)
        {
        LCD_ShowString(38+30+32,0+20,"-",RED,WHITE,16,0);
        LCD_ShowIntNum(38+30+32+8,0+20,-real_data_x,2,GREEN,WHITE,16); 
        }
        LCD_ShowIntNum(38,0+20,tractdata_x,3,GREEN,WHITE,16);
        
        /* 因为发送来的数据是+100来的，所以跟100比较可以判断发来的是不是负数，以下代码就是这个作用 */
         if(tractdata_y>100)
        {
            real_data_y=-(tractdata_y-100);
        }
        if(real_data_y==100)
        {
            real_data_y=0;
        }
        else if(tractdata_y<100)
        {
            real_data_y=tractdata_y;
        }
        if(real_data_y>=0)
        {
        LCD_ShowString(38+30+32,40,"+",RED,WHITE,16,0);
        LCD_ShowIntNum(38+30+32+8,40,real_data_y,2,GREEN,WHITE,16); 
        }
        else if(real_data_y<0)
        {
        LCD_ShowString(38+30+32,40,"-",RED,WHITE,16,0);
        LCD_ShowIntNum(38+30+32+8,40,-real_data_y,2,GREEN,WHITE,16); 
        }
        LCD_ShowIntNum(38,40,tractdata_y,3,GREEN,WHITE,16);        
//          printf("real=%d len=%d\r\n",real_data_x,len);
//			printf("\r\n\r\n");/* 插入换行 */
        *x_error=real_data_x; 返回值
        *y_error=real_data_y;
        USART_RX_STA=0; //接收完成，开启下一轮的接收
        for(i=0;i<10;i++)  /* 数据清零 */
        {
            source[i]=0;
        }
    }
}







