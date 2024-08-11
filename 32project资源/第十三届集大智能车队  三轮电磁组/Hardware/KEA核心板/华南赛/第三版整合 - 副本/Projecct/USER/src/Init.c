#include "headfile.h"
uint8 ser_right =0;
uint8 ser_left =0;
uint8 ser_middle=0;
uint8 straight_left=0;
uint8 straight_right=0;

extern uint8 senmax[3];
extern uint8 straight[2];

void all_Init()
{
    LCD_Init();
    adc_init(ADC0_SE12);
    adc_init(ADC0_SE14);
    adc_init(ADC0_SE8);
    adc_init(ADC0_SE13);
    adc_init(ADC0_SE15);

    gpio_init(B5,GPI,0);//����1
    port_pull(B5);
    gpio_init(B4,GPI,0);//����2
    port_pull(B4);
    gpio_init(C3,GPI,0);//����3
    port_pull(C3);
    gpio_init(C2,GPI,0);//����4
    port_pull(C2);

    //���ᰴ��
    gpio_init(D7,GPI,0);
    port_pull(D7);
    gpio_init(D6,GPI,0);
    port_pull(D6);
    gpio_init(D5,GPI,0);
    port_pull(D5);
    gpio_init(I5,GPI,0);
    port_pull(I5);
    gpio_init(I6,GPI,0);
    port_pull(I6);
    //ͣ��
    gpio_init(F3,GPI,1);
    port_pull(F3);
    //������
    gpio_init(D0,GPO,1);
    port_pull(D0);
    //�����ʼ��
    ftm_pwm_init(ftm2,ftm_ch2,15000,0);
    ftm_pwm_init(ftm2,ftm_ch3,15000,0);
    ftm_pwm_init(ftm2,ftm_ch4,15000,0);
    ftm_pwm_init(ftm2,ftm_ch5,15000,0);
    //��ʱ�ж�
    pit_init_ms(pit0,1);
    //pit_init_ms(pit1,4);
    //enable_irq(PIT_CH0_IRQn);
    set_irq_priority(PIT_CH0_IRQn,0);//�������ȼ�,�����Լ����������� �����÷�ΧΪ 0 - 3  ԽС���ȼ�Խ��
    //��������
    uart_init(uart2,115200);//��ʼ������2��UART2_RX_PIN I0��UART2_TX_PIN I1;
  // ������
    ftm_count_init(ftm0);   //��E0���������������м���    E0�ӱ�����LSB
    gpio_init(E1,GPI,0);    //�����жϷ���                  E1�ӱ�����DIR
    ftm_count_init(ftm1);   //��E7���������������м���    E7�ӱ�����LSB
    gpio_init(H2,GPI,0);    //�����жϷ���                  H2�ӱ�����DIR
    //�������ֵ
    ftm_count_clean(ftm0);
    ftm_count_clean(ftm1);
    //IIC��ʼ��
 //  IIC_init()  ;
 //  InitMPU6050() ;// MPU6050��ʼ��
}
uint8 Bubble(uint8 *var)
{
    uint8 i,j,k,temp;
    for(i=0;i<20;i++)
    {
        for(j=0;j<19-j;j++)
        {
            if(var[j]>var[j+1])
            {
                temp=var[j];
                var[j]=var[j+1];
                var[j+1]=temp;
            }
        }
    }
    k=(var[8]+var[9]+var[10])/3;
    return k;
}

void senser_collect()
{
    uint8 var1[20]= {0};
    uint8 var0[20]= {0};
    uint8 var2[20]= {0};
    uint8 var3[20]= {0};
    uint8 var4[20]= {0};

        for(uint8 i=0;i<20; i++)
        var0[i]=adc_once(ADC0_SE8, ADC_8bit);
    ser_right=100*Bubble(var0)/senmax[1];
;

    for(uint8 i=0; i<20; i++)
        var1[i]=adc_once(ADC0_SE12,ADC_8bit);
    ser_left= 100*Bubble(var1)/senmax[0];

  for(uint8 i=0; i<20; i++)
        var2[i]=adc_once(ADC0_SE14,ADC_8bit);
    ser_middle= 100*Bubble(var2)/senmax[2];
   //��ֱ���ҵ�У�
   for(uint8 i=0; i<20; i++)
        var3[i]=adc_once(ADC0_SE13,ADC_8bit);
   straight_left=100*Bubble(var3)/straight[0];

   for(uint8 i=0; i<20; i++)
       var4[i]=adc_once(ADC0_SE15, ADC_8bit);
straight_right=100*Bubble(var4)/straight[1];

}
float Abs(float i)
{
    float a;
    a=i>0? i: -i;
    return a;
}
void vcan_sendware(uint8 *wareaddr, uint32 waresize)
{
#define CMD_WARE     3
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    uart_putbuff(DEBUG_PORT, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_putbuff(DEBUG_PORT, wareaddr, waresize);    //��������
    uart_putbuff(DEBUG_PORT, cmdr, sizeof(cmdr));    //���ͺ�����

}


