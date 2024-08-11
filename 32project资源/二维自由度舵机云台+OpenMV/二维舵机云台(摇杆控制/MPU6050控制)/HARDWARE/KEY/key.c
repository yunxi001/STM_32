#include "key.h"
#include "delay.h"

void Key_Init(void)
{
	//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//
	//	GPIO_InitTypeDef GPIO_InitStructure;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//	GPIO_Init(GPIOB, &GPIO_InitStructure);

	MY_GPIO_Init(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12, GPIO_Mode_IPU);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) == 0)
	{
		delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) == 0)
			;
		delay_ms(20);
		KeyNum = 1;
	}
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == 0)
	{
		delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == 0)
			;
		delay_ms(20);
		KeyNum = 2;
	}
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == 0)
	{
		delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == 0)
			;
		delay_ms(20);
		KeyNum = 3;
	}

	return KeyNum;
}
