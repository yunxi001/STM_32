#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// ALIENTEK Mini STM32������
// ϵͳ�жϷ������û�
// ����ԭ��@ALIENTEK
// ������̳:www.openedv.com
// �޸�����:2012/9/10
// �汾��V1.4
// ��Ȩ���У�����ؾ���
// Copyright(C) ����ԭ�� 2009-2019
// All rights reserved
//********************************************************************************

/*�������˲���ֵ����*/
float P = 1;
float P_; // ��Ӧ��ʽ�е�p'
float X = 0;
float X_; // X'
float K = 0;
float Q = 0.01; // ����
// float R=0.2;  //R����ܴ�˵������ֵ���ɿ���������Ԥ��ֵ����ô��������Ӧ�ͻ�ٶۣ���֮�෴
float R = 0.35; // R�����С���������Ǵ�������ֵ 	�Ʋ��ֵ�Ͳ�̫����	�������ͻ�����������������������״�ͻ��������		R=0 �Ļ� ˵������ֵ��ȫ�ɿ�
// float distance = 0;
// float distance1 = 0;

// һά�������˲�

/**
 * @brief  �������˲�
 * @param	Z	��ʹ�õ������ݽ����˲�
 * @retval ��
 */
/*����������*/ // ��yaw�ǽ����˲�
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
 * @brief  GPIO��ʼ��������GPIO ģʽ  Pin
 * @param  GPIOx  		ѡ��GPIO�� GPIOA~GPIOG
 * @param  GPIO_Pin  	ѡ���ĸ��˿�
 * @param  Mode        ѡ��˿�ģʽ
 * @retval ��
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
 * @brief ����GPIO��״̬���ߵ�ƽ���͵�ƽ����ת��ƽ
 * @param  GPIOx  		ѡ��GPIO�� GPIOA~GPIOG
 * @param  GPIO_Pin  	ѡ���ĸ��˿�  0~16
 * @param  Mode        ѡ��˿�ģʽ  ����ģʽ
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

/*//�ú���һ��ֻ������1��IO��,���IO��,���ε���
//�ú������Զ�������Ӧ�ж�,�Լ�������
//����ֱ��һ����λ   RCC����AFIO��ʱ��-->�жϳ�ʼ��(�ж���ѡ�� | �����ж� | �ж�ģʽ���жϴ��� | ������ʽ�� ��ѡ�������½��ش���|)
//ע���ж��ⲿ���ŵ� GPIO��ʱ��δ����NVIC�ж�δ��ʼ��
  */
/****   �൱���·�����

	//��ʼ��IO��Ϊ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //����AFIO��ʱ��

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);  //����GPIOE���ж���  AFIO�ж�����ѡ�� ѡ��E3 E4

	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_StructInit(&EXTI_InitStruct);   //Ĭ������
	EXTI_InitStruct.EXTI_Line=EXTI_Line3;		//PE3
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;   //�жϴ���
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //�½��ش���
	EXTI_Init(&EXTI_InitStruct);
**/
/**
 * @brief �ⲿ�ж����ú���
 * @param GPIOx  ֻ���GPIOA~G;������PVD,RTC��USB����������
 * @param BITx:��Ҫʹ�ܵ�λ;   0-16
 * @param TRIM:����ģʽ,1,������;2,�Ͻ���;3�������ƽ����
 * @retval ��
 **/
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM)
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR = BITx / 4; // �õ��жϼĴ�����ı��
	EXTOFFSET = (BITx % 4) * 4;
	RCC->APB2ENR |= 0x01;							 // ʹ��io����ʱ��
	AFIO->EXTICR[EXTADDR] &= ~(0x000F << EXTOFFSET); // ���ԭ�����ã�����
	AFIO->EXTICR[EXTADDR] |= GPIOx << EXTOFFSET;	 // EXTI.BITxӳ�䵽GPIOx.BITx
	// �Զ�����
	EXTI->IMR |= 1 << BITx; //  ����line BITx�ϵ��ж�
							// EXTI->EMR|=1<<BITx;//������line BITx�ϵ��¼� (������������,��Ӳ�����ǿ��Ե�,��������������ʱ���޷������ж�!)
	if (TRIM & 0x01)
		EXTI->FTSR |= 1 << BITx; // line BITx���¼��½��ش���
	if (TRIM & 0x02)
		EXTI->RTSR |= 1 << BITx; // line BITx���¼��������ش���
}

// ����������ƫ�Ƶ�ַ
// NVIC_VectTab:��ַ
// Offset:ƫ����
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
{
	SCB->VTOR = NVIC_VectTab | (Offset & (u32)0x1FFFFF80); // ����NVIC��������ƫ�ƼĴ���
														   // ���ڱ�ʶ����������CODE��������RAM��
}
// ����NVIC����
// NVIC_Group:NVIC���� 0~4 �ܹ�5��
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)
{
	u32 temp, temp1;
	temp1 = (~NVIC_Group) & 0x07; // ȡ����λ
	temp1 <<= 8;
	temp = SCB->AIRCR;	// ��ȡ��ǰ������
	temp &= 0X0000F8FF; // �����ǰ����
	temp |= 0X05FA0000; // д��Կ��
	temp |= temp1;
	SCB->AIRCR = temp; // ���÷���
}
// ����NVIC
// NVIC_PreemptionPriority:��ռ���ȼ�
// NVIC_SubPriority       :��Ӧ���ȼ�
// NVIC_Channel           :�жϱ��
// NVIC_Group             :�жϷ��� 0~4
// ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
// �黮��:
// ��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
// ��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
// ��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�  ȡֵΪ
// ��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
// ��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
// NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����
void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group)
{
	u32 temp;
	MY_NVIC_PriorityGroupConfig(NVIC_Group); // ���÷���
	temp = NVIC_PreemptionPriority << (4 - NVIC_Group);
	temp |= NVIC_SubPriority & (0x0f >> NVIC_Group);
	temp &= 0xf;											   // ȡ����λ
	NVIC->ISER[NVIC_Channel / 32] |= (1 << NVIC_Channel % 32); // ʹ���ж�λ(Ҫ����Ļ�,�෴������OK)
	NVIC->IP[NVIC_Channel] |= temp << 4;					   // ������Ӧ���ȼ����������ȼ�
}

// THUMBָ�֧�ֻ������
// �������·���ʵ��ִ�л��ָ��WFI
void WFI_SET(void)
{
	__ASM volatile("wfi");
}
// �ر������ж�
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}
// ���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");
}
// ����ջ����ַ
// addr:ջ����ַ
__asm void MSR_MSP(u32 addr)
{
	MSR MSP, r0 // set Main Stack value
				 BX r14
}
