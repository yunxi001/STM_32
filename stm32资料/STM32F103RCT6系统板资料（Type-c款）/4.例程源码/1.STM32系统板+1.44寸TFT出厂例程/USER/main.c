/*
1.��Ŀ���ƣ������콢��-STM32�ۺϲ��԰�SPI�ӿ�1.44��LCD���Գ���
2.��ʾģ�飺1.44��LCD��
3.����APP����
4.������λ������
5.��Ŀ��ɣ�1.44��LCD������STM32�ۺϲ��԰�
6.��Ŀ���ܣ�1.44��LCD��ʾ��Ӣ�ġ����֡�ͼƬ����������뿴��ʾ��Ƶ
7.��Ҫԭ������ο�1.44��LCD�����ֲ�
8.�����ַ��https://lssz.tmall.com ���Ա��������������콢�ꡱ
9.��ʾ��Ƶ����������ҳ��������ͷ���Ҫ��
10.��Ȩ�����������콢�����г��������������Ȩ�����뱾���Ʒ���׳��ۣ��벻Ҫ����������׷���䷨�����Σ�
*/
#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"
#include "Picture.h"
#include "led.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_GPIO_Init();
	LCD_Init();
	LCD_BLK = 0;
	LCD_Clear(WHITE);
	LCD_Showimage(14, 0, 100, 50, gImage_11);
	LCD_DrawFont_GBK16(24, 60, BLUE, WHITE, "�����콢��");
	LCD_DrawFont_GBK16(28, 80, RED, WHITE, "1.44 LCD");
	LCD_DrawFont_GBK16(8, 100, BLACK, WHITE, "lssz.tmall.com");
	LED=0;
	
	while(1)
	{
		delay_ms(500);
		LED=!LED;
	}
}


