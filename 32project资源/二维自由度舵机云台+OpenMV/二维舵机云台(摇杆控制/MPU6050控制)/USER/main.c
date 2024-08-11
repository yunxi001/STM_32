/*
 * @Author: _oufen
 * @Date: 2023-07-26 16:37:52
 * @LastEditTime: 2023-07-27 19:26:39
 * @Description:  ��ά�����̨
 */

/**
 * @attention
 * ����һ����ά�����̨��С��Ŀ������
 *
 */

#include "sys.h"

float pitch, roll, yaw; // ŷ����
u8 keynum;

float my_yaw; // �˲��������

volatile int16_t URxData = 0;
volatile int16_t URyData = 0;

float volx, voly;

int main(void)
{
    delay_init(); // ��ʱ��ʼ��
    uart_init(115200);
    Other_Init(); // LED����������ʼ��
    OLED_Init();  // OLED��ʼ��
    Key_Init();

    OLED_ShowChar(1, 1, 'B');

    Servo_Init(); // �����ʼ��
    Rocker_Init();

    // delay_ms(500);  // ��ʱ0.5s ʹMPU6050��ʼ���ȶ�
    // MPU_Init();     // ��ʼ��MPU6050
    // mpu_dmp_init(); // ��ʼ��DMP

    OLED_ShowString(1, 1, "pitch:");
    OLED_ShowString(2, 1, "roll:");
    OLED_ShowString(3, 1, "yaw:");

    while (1)
    {
        // ��ʼ��   3067  Rx   2832 RY
        URxData = AD_GetValue(1);
        URyData = AD_GetValue(2);

        volx = (float)URxData * (3.3 / 4096);
        voly = (float)URyData * (3.3 / 4096);

        // while (mpu_dmp_get_data(&pitch, &roll, &yaw) != 0) // ���϶�ȡŷ����  ������0 ��һֱѭ��  ����0 ������  ��������ѭ����ȡ  �����Ʈ
        // {
        // } // ����ֵ:0,����	//    ����,ʧ��1
        // my_yaw = KLM(yaw);

        if (URxData <= 160) // ��
        {
            TIM4->CCR2 += 50;
            if (TIM4->CCR2 >= 2500)
            {
                TIM4->CCR2 = 2500;
            }
        }
        else if (URxData >= 3500) // ��
        {
            TIM4->CCR2 -= 50;
            if (TIM4->CCR2 <= 500)
            {
                TIM4->CCR2 = 500;
            }
        }

        if (URyData <= 20) // ��
        {
            TIM4->CCR1 += 50;
            if (TIM4->CCR1 >= 2500)
            {
                TIM4->CCR1 = 2500;
            }
        }
        else if (URyData >= 3500) // ��
        {
            TIM4->CCR1 -= 50;
            if (TIM4->CCR1 <= 500)
            {
                TIM4->CCR1 = 500;
            }
        }

        u1_printf("RxData = %d, RyData = %d, CCR1 = %d, CCR2 = %d, volx = %f, voly = %f\r\n", URxData, URyData, TIM4->CCR1, TIM4->CCR2, volx, voly);

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

        //        if (keynum == 1) // ת��90
        //        {
        //            if (pid_angle.target_val <= 180)
        //            {
        //                pid_angle.target_val += 90;
        //            }
        //        }
        //        else if (keynum == 2)
        //        {
        //            if (pid_angle.target_val >= -180)
        //            {
        //                pid_angle.target_val -= 90;
        //            }
        //        }

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
