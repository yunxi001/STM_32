/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "motor.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/**
 * 函数名:Motor_GPIO_Init
 * 描述:电机对应GPIO引脚初始化
 * 输入:无
 * 输出:无
 */
void Motor_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruture;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruture.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruture.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruture);
}

/**
 * 函数名:Motor_Left_DIR
 * 描述:左轮电机控制函数
 * 输入:Direction=(FORWARD,BACK,STOP)
 * 输出:无
 */
void Motor_Right_DIR(MotorDIR_Choose Direction)
{
    if (BACK == Direction)
    {
        M_left_IN1 = 0;
        M_left_IN2 = 1;
    }
    else if (FORWARD == Direction)
    {

        M_left_IN1 = 1;
        M_left_IN2 = 0;
    }
    else if (STOP == Direction)
    {
        M_left_IN1 = 0;
        M_left_IN2 = 0;
    }
}

/**
 * 函数名:Motor_Right_DIR
 * 描述:右轮电机控制函数
 * 输入:Direction=(FORWARD,BACK,STOP)
 * 输出:无
 */
void Motor_Left_DIR(MotorDIR_Choose Direction)
{
    if (BACK == Direction)
    {
        M_right_IN3 = 1;
        M_right_IN4 = 0;
    }
    else if (FORWARD == Direction)
    {

        M_right_IN3 = 0;
        M_right_IN4 = 1;
    }
    else if (STOP == Direction)
    {
        M_right_IN3 = 0;
        M_right_IN4 = 0;
    }
}

/**
 * @description: 高级定时器输出PWM配置函数
 * @param {uint16_t} psc 预分频值
 * @param {uint16_t} arr 预装载重装值
 * @return {*}
 */
void TIM1_PWM_Init(uint16_t psc, uint16_t arr)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    MY_GPIO_Init(GPIOA, GPIO_Pin_8 | GPIO_Pin_11, GPIO_Mode_AF_PP); // 推挽复用输出

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    // 选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // 设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_Pulse = 0;
    // 比较输出使能
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // 使能输出比较N状态
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    // 当 MOE=0 重置 TIM1 输出比较空闲状态
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    // 当 MOE=0 重置 TIM1 输出比较 N 空闲状态
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    // 输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // 根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    // 根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    // CH1预装载使能
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    // CH4预装载使能
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    // 上面个是指通道上的当事件更新时，将修改的CCRx（输出比较）的数值放入影子寄存器

    // 使能TIMx在ARR上的预装载寄存器，修改的arr的值要等事件更新(周期过后)后再将数值放入寄存器
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    // MOE 主输出使能
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    // 使能TIM1
    TIM_Cmd(TIM1, ENABLE);
}

/**
 * 函数名:Motor_All_Init
 * 描述:电机对应GPIO引脚初始化  PWM驱动电机初始化
 * 输入:无
 * 输出:无
 */
void Motor_All_Init(void)
{
    Motor_GPIO_Init();      // 电机IO初始化
    TIM1_PWM_Init(0, 7200); // PWM驱动电机初始化

    TIM2_Encoder_Init(0, 65535);
    TIM3_Encoder_Init(0, 65535);
}

/**
 * 函数名:Load_Motor_PWM
 * 描述:更新PWM,并将其写定时器
 * 输入:MOTOR1_PWM-左轮电机计算得出的PWM值，MOTOR2_PWM-右轮电机计算得出的PWM值
 * 输出:无
 */
void Load_Motor_PWM(int MOTOR1_PWM, int MOTOR2_PWM)
{
    MOTOR1_PWM = (MOTOR1_PWM > 0) ? MOTOR1_PWM : (-MOTOR1_PWM);
    MOTOR2_PWM = (MOTOR2_PWM > 0) ? MOTOR2_PWM : (-MOTOR2_PWM);

    MOTOR1_PWM = (MOTOR1_PWM > PWM_MAX) ? PWM_MAX : MOTOR1_PWM;
    MOTOR2_PWM = (MOTOR2_PWM > PWM_MAX) ? PWM_MAX : MOTOR2_PWM;

    // if (MOTOR1_PWM < 0)
    // {
    //     MOTOR1_PWM = -MOTOR1_PWM;
    // }
    // if(MOTOR1_PWM>PWM_MAX)
    // {
    // 	MOTOR1_PWM=PWM_MAX;
    // }
    // if (MOTOR2_PWM < 0)
    // {
    //     MOTOR2_PWM = -MOTOR2_PWM;
    // }

    //    OLED_ShowNum(1, 1, MOTOR1_PWM, 4);
    //    OLED_ShowNum(2, 1, MOTOR2_PWM, 4);

    TIM1->CCR4 = MOTOR1_PWM;
    TIM1->CCR1 = MOTOR2_PWM;
}

/**
 * 函数名:set_motor_enable
 * 描述:使能电机
 * 输入:无
 * 输出:无
 */
void set_motor_enable(void)
{
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_4, TIM_CCx_Enable);
}

/**
 * 函数名:set_motor_disable
 * 描述:失能电机
 * 输入:无
 * 输出:无
 */
void set_motor_disable(void)
{
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
    TIM_CCxCmd(TIM1, TIM_Channel_4, TIM_CCx_Disable);
}

/*****************************************************END OF FILE*********************************************************/
