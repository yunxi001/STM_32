/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "motor.h"

/* ���� -----------------------------------------------------------------------------------------------------------------*/

/**
 * ������:Motor_GPIO_Init
 * ����:�����ӦGPIO���ų�ʼ��
 * ����:��
 * ���:��
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
 * ������:Motor_Left_DIR
 * ����:���ֵ�����ƺ���
 * ����:Direction=(FORWARD,BACK,STOP)
 * ���:��
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
 * ������:Motor_Right_DIR
 * ����:���ֵ�����ƺ���
 * ����:Direction=(FORWARD,BACK,STOP)
 * ���:��
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
 * @description: �߼���ʱ�����PWM���ú���
 * @param {uint16_t} psc Ԥ��Ƶֵ
 * @param {uint16_t} arr Ԥװ����װֵ
 * @return {*}
 */
void TIM1_PWM_Init(uint16_t psc, uint16_t arr)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // ����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    MY_GPIO_Init(GPIOA, GPIO_Pin_8 | GPIO_Pin_11, GPIO_Mode_AF_PP); // ���츴�����

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // ���ô�װ�벶��ȽϼĴ���������ֵ
    TIM_OCInitStructure.TIM_Pulse = 0;
    // �Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // ʹ������Ƚ�N״̬
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    // �� MOE=0 ���� TIM1 ����ȽϿ���״̬
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    // �� MOE=0 ���� TIM1 ����Ƚ� N ����״̬
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    // �������:TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // ����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    // ����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    // CH1Ԥװ��ʹ��
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    // CH4Ԥװ��ʹ��
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    // �������ָͨ���ϵĵ��¼�����ʱ�����޸ĵ�CCRx������Ƚϣ�����ֵ����Ӱ�ӼĴ���

    // ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ������޸ĵ�arr��ֵҪ���¼�����(���ڹ���)���ٽ���ֵ����Ĵ���
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    // MOE �����ʹ��
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    // ʹ��TIM1
    TIM_Cmd(TIM1, ENABLE);
}

/**
 * ������:Motor_All_Init
 * ����:�����ӦGPIO���ų�ʼ��  PWM���������ʼ��
 * ����:��
 * ���:��
 */
void Motor_All_Init(void)
{
    Motor_GPIO_Init();      // ���IO��ʼ��
    TIM1_PWM_Init(0, 7200); // PWM���������ʼ��

    TIM2_Encoder_Init(0, 65535);
    TIM3_Encoder_Init(0, 65535);
}

/**
 * ������:Load_Motor_PWM
 * ����:����PWM,������д��ʱ��
 * ����:MOTOR1_PWM-���ֵ������ó���PWMֵ��MOTOR2_PWM-���ֵ������ó���PWMֵ
 * ���:��
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
 * ������:set_motor_enable
 * ����:ʹ�ܵ��
 * ����:��
 * ���:��
 */
void set_motor_enable(void)
{
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_4, TIM_CCx_Enable);
}

/**
 * ������:set_motor_disable
 * ����:ʧ�ܵ��
 * ����:��
 * ���:��
 */
void set_motor_disable(void)
{
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
    TIM_CCxCmd(TIM1, TIM_Channel_4, TIM_CCx_Disable);
}

/*****************************************************END OF FILE*********************************************************/
