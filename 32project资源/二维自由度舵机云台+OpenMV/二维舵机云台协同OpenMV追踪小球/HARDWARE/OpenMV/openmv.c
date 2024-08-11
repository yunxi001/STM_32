/*
 * @Author: _oufen
 * @Date: 2023-05-24 21:29:15
 * @LastEditTime: 2023-07-28 16:26:36
 * @Description:
 */
#include "openmv.h"

int openmv[6]; // stm32接收数据数组

int16_t OpenMv_data1; //
int16_t OpenMv_data2; //
int16_t OpenMv_data3; //

int i;

void OpenMv_init(u32 bound)
{
    // GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能USART2，GPIOA时钟

    // USART2_TX    RX      PA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);          // 初始化GPIOA2

    // USART2_RX	TX      PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;             // PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                // 初始化GPIOA3

    // USART2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           // 根据指定的参数初始化VIC寄存器

    // USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                                     // 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // 字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // 一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // 收发模式

    USART_Init(USART2, &USART_InitStructure);      // 初始化串口1
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 开启串口接受中断
    USART_Cmd(USART2, ENABLE);                     // 使能串口1
}

void u2_SendByte(uint8_t Byte) // 发送一个字节数据
{
    USART_SendData(USART2, Byte);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
        ;
}

void u2_SendArray(uint8_t *Array, uint16_t Length) // 发送一个数组  指定字长
{
    uint16_t i;
    for (i = 0; i < Length; i++)
    {
        u2_SendByte(Array[i]);
    }
}

void u2_SendString(char *String) // 发送一个字符串数据
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        u2_SendByte(String[i]);
    }
}

void USART_SendByte(USART_TypeDef *USARTx, char str) // 指定串口发送数据
{

    USART_SendData(USARTx, str); // 发送单个字符

    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
        ; // 判断是否发送完成
}

void Openmv_Data(void) // 处理Openmv接收的数据
{
    // u8 i1;
    //  if (i1++ > 10)
    //  {
    OpenMv_data1 = openmv[2]; // MV 判断发送的巡线值
    OpenMv_data2 = openmv[3]; // 这是MV判断是否是十字路口，如果是十字路口的话 内外圈交替
    OpenMv_data3 = openmv[4];
    // }
}

// void OpenMv_Receive_Data(int16_t data) // OpenMv数据处理函数
//{
//     static u8 state = 0;
//     if (state == 0 && data == 0xb3) // 第一个帧头 0xb3
//     {
//         state = 1;
//         openmv[0] = data;
//     }
//     else if (state == 1 && data == 0xb3) // 第二个帧头0xb3
//     {
//         state = 2;
//         openmv[1] = data;
//     }
//     else if (state == 2) // 第一个有效数据 传递给TargerNum
//     {
//         state = 3;
//         openmv[2] = data;
//     }
//     else if (state == 3) // 第二个有效数据  传递给Find_Task
//     {
//         state = 4;
//         openmv[3] = data;
//     }
//     else if (state == 4) // 第三个有效数据  传递给LoR
//     {
//         state = 5;
//         openmv[4] = data;
//     }
//     else if (state == 5) // 判断帧尾
//     {
//         if (data == 0x5B) // 如果帧尾为0x5B  返回原状态
//         {
//             state = 0;
//             openmv[5] = data;
//             Openmv_Data(); // 处理数据 讲数据赋值给三个变量
//         }
//         else if (data != 0x5B) // 如果帧尾不为0x5B  返回原状态
//         {
//             state = 0;
//             for (i = 0; i < 16; i++)
//             {
//                 openmv[i] = 0x00; // 核验不对将数据进行清零
//             }
//         }
//     }
//     else
//     {
//         state = 0;
//         for (i = 0; i < 16; i++)
//         {
//             openmv[i] = 0x00; // 核验不对将数据进行清零
//         }
//     }USART2
// }

void u1_sendData(u8 *str)
{

    u8 i = 0;
    for (i = 0; i < 2; i++) // 5b 5b
    {
        USART_SendData(USART1, 0x5b);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
            ;
    }

    for (i = 0; i < 3; i++) // 发送三位有效位
    {
        USART_SendData(USART1, str[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
            ;
    }

    USART_SendData(USART1, 0xb3); // 0xb3
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
        ;
}

void OpenMV_sendData(u8 *str)
{
    u8 i = 0;
    for (i = 0; i < 2; i++) // b3 b3
    {
        USART_SendData(USART2, 0x5b);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET)
            ;
    }

    for (i = 0; i < 3; i++) // 发送三位有效位
    {
        USART_SendData(USART2, str[i]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET)
            ;
    }

    USART_SendData(USART2, 0xb3); // 0x5b
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET)
        ;
}

void Openmv_Receive_Data(int16_t data) // 接收Openmv传过来的数据
{
    static u8 state = 0;
    if (state == 0 && data == 0xb3) // 第一个帧头
    {
        state = 1;
        openmv[0] = data;
    }
    else if (state == 1 && data == 0xb3) // 第二个帧头
    {
        state = 2;
        openmv[1] = data;
    }
    else if (state == 2) // 第一个有效数据
    {
        state = 3;
        openmv[2] = data;
    }
    else if (state == 3) // 第二个有效数据
    {
        state = 4;
        openmv[3] = data;
    }
    else if (state == 4) // 第三个有效数据
    {
        state = 5;
        openmv[4] = data;
    }
    else if (state == 5) // 检测是否接受到结束标志，检测接收帧尾
    {
        if (data == 0x5B)
        {
            state = 0;
            openmv[5] = data;
            Openmv_Data();
        }
        else if (data != 0x5B)
        {
            state = 0;
            for (i = 0; i < 6; i++)
            {
                openmv[i] = 0x00;
            }
        }
    }
    else
    {
        state = 0;
        for (i = 0; i < 6; i++)
        {
            openmv[i] = 0x00;
        }
    }
}

void USART2_IRQHandler(void) // 串口1中断服务程序
{
    uint8_t ReceiveData;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        // ReceiveData = USART_ReceiveData(USART2);
        ReceiveData = USART2->DR; // 接收数据
        // u2_SendByte(ReceiveData);
        Openmv_Receive_Data(ReceiveData); // 对接收到的数据进行解析  保证数据接收的合理性
        Openmv_Data();
    }
}

/******************* 通信测试 ********************/
// u8 j;
// u8 data[6] = {
//     0xb3,
//     0xb3,
//     0,
//     0,
//     0,
//     0x5b};

// 下方函数放入主函数中进行测试   打开两个串口  一个串口发送  另一个串口接收  看数据是否接收到  数据是否正确

// data[2] = Mv_Line;
// data[3] = Mv_ISRoad;
// data[4] = Mv_LoR;
// for (j = 0; j <= 6; j++)
// {
//     u1_printf("%x", data[j]);
//     u1_printf("\r\n");
// }

/******************* 结束 *********************************/
