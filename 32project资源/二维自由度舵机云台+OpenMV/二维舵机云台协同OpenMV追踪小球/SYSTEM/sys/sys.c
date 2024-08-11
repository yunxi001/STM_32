#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// ALIENTEK Mini STM32开发板
// 系统中断分组设置化
// 正点原子@ALIENTEK
// 技术论坛:www.openedv.com
// 修改日期:2012/9/10
// 版本：V1.4
// 版权所有，盗版必究。
// Copyright(C) 正点原子 2009-2019
// All rights reserved
//********************************************************************************

/*卡尔曼滤波数值配置*/
float P = 1;
float P_; // 对应公式中的p'
float X = 0;
float X_; // X'
float K = 0;
float Q = 0.01; // 噪声
// float R=0.2;  //R如果很大，说明测量值不可靠，更相信预测值，那么传感器反应就会迟钝，反之相反
float R = 0.35; // R如果很小，更相信是传感器的值 	推测的值就不太相信	传感器就会更加灵敏，更加灵敏，锯齿状就会更加明显		R=0 的话 说明测量值完全可靠
// float distance = 0;
// float distance1 = 0;

// 一维卡尔曼滤波

/**
 * @brief  卡尔曼滤波
 * @param	Z	对使用到的数据进行滤波
 * @retval 空
 */
/*卡尔曼函数*/ // 对yaw角进行滤波
float KLM(float Z)
{
	X_ = X + 0;
	P_ = P + Q;
	K = P_ / (P_ + R);
	X = X_ + K * (Z - X_);
	P = P_ - K * P_;
	return X;
}

/**
 * @brief  GPIO初始化，设置GPIO 模式  Pin
 * @param  GPIOx  		选择GPIO口 GPIOA~GPIOG
 * @param  GPIO_Pin  	选择哪个端口
 * @param  Mode        选择端口模式
 * @retval 空
 */
void MY_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = Mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	if (GPIOx == GPIOA)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}
	else if (GPIOx == GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}
	else if (GPIOx == GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}

	else if (GPIOx == GPIOD)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	}
	else if (GPIOx == GPIOE)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	}
	else if (GPIOx == GPIOF)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	}
	else if (GPIOx == GPIOG)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	}

	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/**
 * @brief 设置GPIO的状态，高电平，低电平，翻转电平
 * @param  GPIOx  		选择GPIO口 GPIOA~GPIOG
 * @param  GPIO_Pin  	选择哪个端口  0~16
 * @param  Mode        选择端口模式  八种模式
 * @retval
 */
void GPIO_Set(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_Level LEVEL)
{
	switch (LEVEL)
	{
	case LOW:
		GPIOx->BRR |= GPIO_Pin;
		break;
	case HIGH:
		GPIOx->BSRR |= GPIO_Pin;
		break;
	case TURN:
		GPIOx->ODR ^= GPIO_Pin;
		break;
	}
}

/*//该函数一次只能配置1个IO口,多个IO口,需多次调用
//该函数会自动开启对应中断,以及屏蔽线
//这里直接一步到位   RCC开启AFIO的时钟-->中断初始化(中断线选择 | 开启中断 | 中断模式：中断触发 | 触发方式： 可选上升沿下降沿触发|)
//注意中断外部引脚的 GPIO的时钟未开，NVIC中断未初始化
  */
/****   相当于下方这样

	//初始化IO口为输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //开启AFIO的时钟

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);  //开启GPIOE的中断线  AFIO中断引脚选择 选择E3 E4

	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_StructInit(&EXTI_InitStruct);   //默认配置
	EXTI_InitStruct.EXTI_Line=EXTI_Line3;		//PE3
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;   //中断触发
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //下降沿触发
	EXTI_Init(&EXTI_InitStruct);
**/
/**
 * @brief 外部中断配置函数
 * @param GPIOx  只针对GPIOA~G;不包括PVD,RTC和USB唤醒这三个
 * @param BITx:需要使能的位;   0-16
 * @param TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
 * @retval 无
 **/
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM)
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR = BITx / 4; // 得到中断寄存器组的编号
	EXTOFFSET = (BITx % 4) * 4;
	RCC->APB2ENR |= 0x01;							 // 使能io复用时钟
	AFIO->EXTICR[EXTADDR] &= ~(0x000F << EXTOFFSET); // 清除原来设置！！！
	AFIO->EXTICR[EXTADDR] |= GPIOx << EXTOFFSET;	 // EXTI.BITx映射到GPIOx.BITx
	// 自动设置
	EXTI->IMR |= 1 << BITx; //  开启line BITx上的中断
							// EXTI->EMR|=1<<BITx;//不屏蔽line BITx上的事件 (如果不屏蔽这句,在硬件上是可以的,但是在软件仿真的时候无法进入中断!)
	if (TRIM & 0x01)
		EXTI->FTSR |= 1 << BITx; // line BITx上事件下降沿触发
	if (TRIM & 0x02)
		EXTI->RTSR |= 1 << BITx; // line BITx上事件上升降沿触发
}

// 设置向量表偏移地址
// NVIC_VectTab:基址
// Offset:偏移量
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
{
	SCB->VTOR = NVIC_VectTab | (Offset & (u32)0x1FFFFF80); // 设置NVIC的向量表偏移寄存器
														   // 用于标识向量表是在CODE区还是在RAM区
}
// 设置NVIC分组
// NVIC_Group:NVIC分组 0~4 总共5组
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)
{
	u32 temp, temp1;
	temp1 = (~NVIC_Group) & 0x07; // 取后三位
	temp1 <<= 8;
	temp = SCB->AIRCR;	// 读取先前的设置
	temp &= 0X0000F8FF; // 清空先前分组
	temp |= 0X05FA0000; // 写入钥匙
	temp |= temp1;
	SCB->AIRCR = temp; // 设置分组
}
// 设置NVIC
// NVIC_PreemptionPriority:抢占优先级
// NVIC_SubPriority       :响应优先级
// NVIC_Channel           :中断编号
// NVIC_Group             :中断分组 0~4
// 注意优先级不能超过设定的组的范围!否则会有意想不到的错误
// 组划分:
// 组0:0位抢占优先级,4位响应优先级
// 组1:1位抢占优先级,3位响应优先级
// 组2:2位抢占优先级,2位响应优先级  取值为
// 组3:3位抢占优先级,1位响应优先级
// 组4:4位抢占优先级,0位响应优先级
// NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先
void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group)
{
	u32 temp;
	MY_NVIC_PriorityGroupConfig(NVIC_Group); // 设置分组
	temp = NVIC_PreemptionPriority << (4 - NVIC_Group);
	temp |= NVIC_SubPriority & (0x0f >> NVIC_Group);
	temp &= 0xf;											   // 取低四位
	NVIC->ISER[NVIC_Channel / 32] |= (1 << NVIC_Channel % 32); // 使能中断位(要清除的话,相反操作就OK)
	NVIC->IP[NVIC_Channel] |= temp << 4;					   // 设置响应优先级和抢断优先级
}

// THUMB指令不支持汇编内联
// 采用如下方法实现执行汇编指令WFI
void WFI_SET(void)
{
	__ASM volatile("wfi");
}
// 关闭所有中断
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}
// 开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");
}
// 设置栈顶地址
// addr:栈顶地址
__asm void MSR_MSP(u32 addr)
{
	MSR MSP, r0 // set Main Stack value
				 BX r14
}
