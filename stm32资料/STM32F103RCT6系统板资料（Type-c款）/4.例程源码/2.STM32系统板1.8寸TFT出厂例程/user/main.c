/*
1.��Ŀ���ƣ������콢��-STM32�ۺϲ��԰�SPI�ӿ�1.8��LCD���Գ���
2.��ʾģ�飺1.8��LCD��
3.����APP����
4.������λ������
5.��Ŀ��ɣ�1.8��LCD������STM32���İ�
6.��Ŀ���ܣ�1.8��LCD��ʾ��Ӣ�ġ����֡�ͼƬ��
7.��Ҫԭ������ο�1.8 LCD�����ֲ�
8.�����ַ��https://lssz.tmall.com ���Ա��������������콢�ꡱ
*/
#include "stm32f10x.h"
#include "delay.h"
#include "QDTFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"


int main(void)
{

	SystemInit();	//System init.
	delay_init(72);//Delay init.
	while(1)
	{
		QDTFT_Test_Demo();	//See the test details in QDTFT_Demo.c
	}

}

