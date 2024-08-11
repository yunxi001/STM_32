/*
 * @Author: _oufen
 * @Date: 2023-07-26 16:37:52
 * @LastEditTime: 2023-07-29 15:58:01
 * @Description:  ��ά�����̨  ����OpenMv���ص����ݽ��и���С��
 */

/**
 * @attention
 * 1������һ����ά�����̨��С��Ŀ�������ͺŵĶ�����Թ��ˣ�Ч������
 * 2������ֱ���SG90�����̨��DS3218�����̨
 * 3�����ֲ�ͬ��������Ǻ��߱�ǰ�߾��ȸ���   ���Ƹ�Ϊ��ϸ   ����ʵ��С���Ҳ������
 * 4������OpenMV��������X������ݺ�Y������ݣ���PID���㣬ʹ��̨׼ȷ�����ʱ�̴���С�����ģ�����С����˶����˶�
 */

#include "sys.h"

float pitch, roll, yaw; // ŷ����
u8 keynum;

float my_yaw; // �˲��������

// volatile int16_t URxData = 0;
// volatile int16_t URyData = 0;

// float volx, voly;

uint8_t data[3] = {01, 02, 03};

int main(void)
{
    Servo_Init();        // �����ʼ��
    OpenMv_init(115200); // OpenMV��ʼ��

    Timer6_Init(7200 - 1, 100 - 1); // 10ms
    PID_param_init();               // PID������ʼ��

    delay_init(); // ��ʱ��ʼ��
    uart_init(115200);
    Other_Init(); // LED����������ʼ��
    OLED_Init();  // OLED��ʼ��
                  // Key_Init();

    // Rocker_Init();  //ҡ�˳�ʼ��

    // delay_ms(500);  // ��ʱ0.5s ʹMPU6050��ʼ���ȶ�
    // MPU_Init();     // ��ʼ��MPU6050
    // mpu_dmp_init(); // ��ʼ��DMP

    // OLED_ShowString(1, 1, "pitch:");
    // OLED_ShowString(2, 1, "roll:");
    // OLED_ShowString(3, 1, "yaw:");

    while (1)
    {

        OLED_ShowNum(1, 1, OpenMv_data1, 4);
        OLED_ShowNum(2, 1, OpenMv_data2, 4);
        OLED_ShowNum(3, 1, OpenMv_data3, 4);

        u1_printf("%.2f,%d,%d,%d,%d\n", (KLM((OpenMv_data1 << 8) | OpenMv_data2)), (OpenMv_data1 << 8) | OpenMv_data2, OpenMv_data3, Param.Servo_X_PWM, Param.Servo_Y_PWM);

        //  u1_printf("High_data = %x, Low_data = %x ,data1 = %d,data2 = %d\r\n", OpenMv_data1, OpenMv_data2, ((OpenMv_data1 << 8) | OpenMv_data2), OpenMv_data3);

        //  u1_printf("%d\r\n", 0x01 << 8 | 0x1D);   8λ����ת16λ����
        //  OpenMV_sendData(data);

        // ��ȡҡ�˵�x�᷽��  y�᷽��  ADCֵ  0-4095
        //  ��ʼ��   3067  Rx   2832 RY
        //  URxData = AD_GetValue(1);
        //  URyData = AD_GetValue(2);

        // ��������ת��Ϊ��ѹ  0- 3.3
        //  volx = (float)URxData * (3.3 / 4096);
        //  voly = (float)URyData * (3.3 / 4096);

        // while (mpu_dmp_get_data(&pitch, &roll, &yaw) != 0) // ���϶�ȡŷ����  ������0 ��һֱѭ��  ����0 ������  ��������ѭ����ȡ  �����Ʈ
        // {
        // } // ����ֵ:0,����	//    ����,ʧ��1
        // my_yaw = KLM(yaw);

        // if (URxData <= 160) // ��
        // {
        //     TIM4->CCR2 += 50;
        //     if (TIM4->CCR2 >= 2500)
        //     {
        //         TIM4->CCR2 = 2500;
        //     }
        // }
        // else if (URxData >= 3500) // ��
        // {
        //     TIM4->CCR2 -= 50;
        //     if (TIM4->CCR2 <= 500)
        //     {
        //         TIM4->CCR2 = 500;
        //     }
        // }

        // if (URyData <= 20) // ��
        // {
        //     TIM4->CCR1 += 50;
        //     if (TIM4->CCR1 >= 2500)
        //     {
        //         TIM4->CCR1 = 2500;
        //     }
        // }
        // else if (URyData >= 3500) // ��
        // {
        //     TIM4->CCR1 -= 50;
        //     if (TIM4->CCR1 <= 500)
        //     {
        //         TIM4->CCR1 = 500;
        //     }
        // }

        // u1_printf("RxData = %d, RyData = %d, CCR1 = %d, CCR2 = %d, volx = %f, voly = %f\r\n", URxData, URyData, TIM4->CCR1, TIM4->CCR2, volx, voly);

        // TIM4->CCR1 = 700 + 7 * (int)roll;    // 400   ����
        // TIM4->CCR2 = 1500 + 7 * (int)my_yaw; // 1000

        // TIM4->CCR1 = 700 + 7 * (int)roll;    // 400   ����
        // TIM4->CCR2 = 1500 + 7 * (int)my_yaw; // 1000

        // u1_printf("pitch = %0.2f, roll = %0.2f, yaw = %0.2f, my_yaw = %0.2f, CCR1 = %d. CCR2 = %d, RxData = %d, RyData = %d\r\n", pitch, roll, yaw, my_yaw, TIM4->CCR1, TIM4->CCR2, URxData, URyData);
        //     u1_printf("%f,%f,%f\n", pitch, roll, yaw);
        //   u1_printf("%f,%f\n", yaw, my_yaw);

        //        OLED_ShowNum(1, 7, (int)(pitch * 10), 4);
        //        OLED_ShowNum(2, 6, (int)(roll * 10), 4);
        //        OLED_ShowNum(3, 5, (int)(yaw * 10), 4);

        /************************ ���Ե�� ����ת �Լ� PWM������� *******************************/
        // Motor_Left_DIR(FORWARD);
        // Load_Motor_PWM(3000, 0);

        // Motor_Right_DIR(FORWARD);
        // Load_Motor_PWM(0, 3000);

        // Motor_Left_DIR(FORWARD);
        // Motor_Right_DIR(FORWARD);
        // Load_Motor_PWM(1500, 1500);
        /***************************** ����  ********************************************************/
    }
}

// void TIM6_IRQHandler(void) // 10ms
//{
//     if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
//     {
//         // Buzzer_IN = 1;
//         LED_GREEN = 0;
//         TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
//         LED_RED = !LED_RED;
//     }
// }
