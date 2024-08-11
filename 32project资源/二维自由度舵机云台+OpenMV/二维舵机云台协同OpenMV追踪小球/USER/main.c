/*
 * @Author: _oufen
 * @Date: 2023-07-26 16:37:52
 * @LastEditTime: 2023-07-29 15:58:01
 * @Description:  二维舵机云台  根据OpenMv传回的数据进行跟踪小球
 */

/**
 * @attention
 * 1、这是一个二维舵机云台的小项目，两种型号的舵机都试过了，效果极佳
 * 2、舵机分别是SG90舵机云台，DS3218舵机云台
 * 3、两种不同的区别就是后者比前者精度更高   控制更为精细   但是实测小舵机也够用了
 * 4、接收OpenMV传过来的X轴的数据和Y轴的数据，做PID计算，使云台准确无误的时刻打在小球中心，跟随小球的运动而运动
 */

#include "sys.h"

float pitch, roll, yaw; // 欧拉角
u8 keynum;

float my_yaw; // 滤波后的数据

// volatile int16_t URxData = 0;
// volatile int16_t URyData = 0;

// float volx, voly;

uint8_t data[3] = {01, 02, 03};

int main(void)
{
    Servo_Init();        // 舵机初始化
    OpenMv_init(115200); // OpenMV初始化

    Timer6_Init(7200 - 1, 100 - 1); // 10ms
    PID_param_init();               // PID参数初始化

    delay_init(); // 延时初始化
    uart_init(115200);
    Other_Init(); // LED及蜂鸣器初始化
    OLED_Init();  // OLED初始化
                  // Key_Init();

    // Rocker_Init();  //摇杆初始化

    // delay_ms(500);  // 延时0.5s 使MPU6050初始化稳定
    // MPU_Init();     // 初始化MPU6050
    // mpu_dmp_init(); // 初始化DMP

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

        //  u1_printf("%d\r\n", 0x01 << 8 | 0x1D);   8位数据转16位数据
        //  OpenMV_sendData(data);

        // 获取摇杆的x轴方向  y轴方向  ADC值  0-4095
        //  初始化   3067  Rx   2832 RY
        //  URxData = AD_GetValue(1);
        //  URyData = AD_GetValue(2);

        // 将数字量转换为电压  0- 3.3
        //  volx = (float)URxData * (3.3 / 4096);
        //  voly = (float)URyData * (3.3 / 4096);

        // while (mpu_dmp_get_data(&pitch, &roll, &yaw) != 0) // 不断读取欧拉角  不等于0 就一直循环  等于0 就跳出  主程序中循环读取  这个会飘
        // {
        // } // 返回值:0,正常	//    其他,失败1
        // my_yaw = KLM(yaw);

        // if (URxData <= 160) // 左
        // {
        //     TIM4->CCR2 += 50;
        //     if (TIM4->CCR2 >= 2500)
        //     {
        //         TIM4->CCR2 = 2500;
        //     }
        // }
        // else if (URxData >= 3500) // 右
        // {
        //     TIM4->CCR2 -= 50;
        //     if (TIM4->CCR2 <= 500)
        //     {
        //         TIM4->CCR2 = 500;
        //     }
        // }

        // if (URyData <= 20) // 上
        // {
        //     TIM4->CCR1 += 50;
        //     if (TIM4->CCR1 >= 2500)
        //     {
        //         TIM4->CCR1 = 2500;
        //     }
        // }
        // else if (URyData >= 3500) // 上
        // {
        //     TIM4->CCR1 -= 50;
        //     if (TIM4->CCR1 <= 500)
        //     {
        //         TIM4->CCR1 = 500;
        //     }
        // }

        // u1_printf("RxData = %d, RyData = %d, CCR1 = %d, CCR2 = %d, volx = %f, voly = %f\r\n", URxData, URyData, TIM4->CCR1, TIM4->CCR2, volx, voly);

        // TIM4->CCR1 = 700 + 7 * (int)roll;    // 400   最下
        // TIM4->CCR2 = 1500 + 7 * (int)my_yaw; // 1000

        // TIM4->CCR1 = 700 + 7 * (int)roll;    // 400   最下
        // TIM4->CCR2 = 1500 + 7 * (int)my_yaw; // 1000

        // u1_printf("pitch = %0.2f, roll = %0.2f, yaw = %0.2f, my_yaw = %0.2f, CCR1 = %d. CCR2 = %d, RxData = %d, RyData = %d\r\n", pitch, roll, yaw, my_yaw, TIM4->CCR1, TIM4->CCR2, URxData, URyData);
        //     u1_printf("%f,%f,%f\n", pitch, roll, yaw);
        //   u1_printf("%f,%f\n", yaw, my_yaw);

        //        OLED_ShowNum(1, 7, (int)(pitch * 10), 4);
        //        OLED_ShowNum(2, 6, (int)(roll * 10), 4);
        //        OLED_ShowNum(3, 5, (int)(yaw * 10), 4);

        /************************ 测试电机 正反转 以及 PWM驱动电机 *******************************/
        // Motor_Left_DIR(FORWARD);
        // Load_Motor_PWM(3000, 0);

        // Motor_Right_DIR(FORWARD);
        // Load_Motor_PWM(0, 3000);

        // Motor_Left_DIR(FORWARD);
        // Motor_Right_DIR(FORWARD);
        // Load_Motor_PWM(1500, 1500);
        /***************************** 结束  ********************************************************/
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
