//////////////////////////////////////////////////////////////////////////////////	 
//�����콢�� 1.8����ֿ�TFT���Գ���   
//���̵�ַ��http://tmall.lvsn.com
//����ƽ̨��STM32F103RCT6
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��Դ������3.3-5V��
//              SCL   PB9��SCLK��
//              SDA   PB8��MOSI��
//              CS    PB7
//              SDO   PB6��MISO��
//              DC    PB5
//              BLK   PB4
//              ----------------------------------------------------------------
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
int main(void)
{
	float t=0;
	delay_init();
	LED_Init();//LED��ʼ��
	LCD_Init();//LCD��ʼ��
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	LED=0;
	while(1)
	{
		Display_GB2312_String(4,0,3, "�����콢��",BLUE,WHITE);
		LCD_ShowString(24,30,"LCD_W:",RED,WHITE,16,0);
		LCD_ShowIntNum(72,30,LCD_W,3,RED,WHITE,16);
		LCD_ShowString(24,50,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowIntNum(72,50,LCD_H,3,RED,WHITE,16);
		LCD_ShowFloatNum1(20,80,t,4,RED,WHITE,16);
		t+=0.11;
		LCD_ShowPicture(65,80,40,40,gImage_1);
		delay_ms(1000);
		LED=~LED;
		delay_ms(1000);
		LED=~LED;
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		Display_Asc_String(0,15,1, "ASCII_5x7",RED,WHITE);    //ASC 5X7����
		Display_Asc_String(0,25,2, "ASCII_7x8",RED,WHITE);    //ASC 7X8����
		Display_Asc_String(0,35,3, "ASCII_6x12",RED,WHITE);	  //ASC 6X12����
		Display_Asc_String(0,50,4, "ASCII_8x16",RED,WHITE);	  //ASC 8X16���� 
		Display_GB2312_String(0,70,1, "�����콢�꣱��������",RED,WHITE); //12x12����
		Display_GB2312_String(0,90,2, "�����콢��",RED,WHITE); //16x16����
		Display_Asc_String(0,110,4, "16 x 16",RED,WHITE);	  //ASC 8X16����
		delay_ms(1000);
		LED=~LED;
		delay_ms(1000);
		LED=~LED;
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
		Display_TimesNewRoman_String(0,15,1, "ASCII_8x12",RED,WHITE);   //ASC 8x12����(TimesNewRoman����)
		Display_TimesNewRoman_String(0,30,2, "ASCII",RED,WHITE);        //ASC 12x16����(TimesNewRoman����)
		Display_TimesNewRoman_String(0,50,3, "ASCII",RED,WHITE);	      //ASC 16x24����(TimesNewRoman����)
		Display_Arial_String(0,90,1, "ASCII_8x1",RED,WHITE);   //ASC 8x12����(Arial����)
		Display_Arial_String(0,110,2, "ASCII",RED,WHITE);      //ASC 12x16����(Arial����)
		Display_Arial_String(0,130,3, "ASCII",RED,WHITE);	 //ASC 16x24����(Arial����)
		delay_ms(1000);
		LED=~LED;
		delay_ms(1000);
		LED=~LED;
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		Display_GB2312_String(0,40,2, "����������ڤڤ��",BLUE,WHITE);
		LCD_ShowChinese(0,60,"�j",BLUE,WHITE,16,0);
		Display_GB2312_String(16,60,2, "����",BLUE,WHITE);
		LCD_ShowChinese(64,60,"��",BLUE,WHITE,16,0);
		Display_GB2312_String(80,60,2, "ҹ����",BLUE,WHITE);
		Display_GB2312_String(0,80,2, "������ľ������",BLUE,WHITE);
		Display_GB2312_String(0,100,2, "˼������ͽ���ǡ�",BLUE,WHITE);
		delay_ms(1000);
		LED=~LED;
		delay_ms(1000);
		LED=~LED;
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	}
}

