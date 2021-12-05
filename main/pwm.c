/*!
    \file    pwm.c
    \brief   PWM输出
*/
  
#include "pwm.h"

void timer1_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* TIM1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* GPIOA and GPIOB clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*!
    \brief 	定时器1配置
    \desc 	定时器默认时钟72MHz，计数时钟72MHz
    		timer1 通道2、通道3 PWM输出
*/
void timer1_config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 5000;	/* 自动装载值，设定PWM周期 */
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;	/* 计数器时钟分频系数 计数时钟 = Fck_psc/(TIM_Prescaler + 1) = 10K */ 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; /* 定时器时钟与死区时间、采样时钟直接的分频比例 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    /* 通道2 PWM模式配置 */
    TIM_OCInitStructure.TIM_Pulse = 400;	/* 脉冲宽度 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; /* 和TIM_OCPolarity配合决定了PWM的极性 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	/* 设置主输出状态 */
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	/* 设置互补输出状态 */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	/* 主输出极性 */
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; /* 互补输出极性*/
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;	/* 设置空闲时主输出状态 */
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	/* 设置空闲时互补输出状态 */	  

    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    
    /* 通道3 PWM模式配置 */	
    TIM_OCInitStructure.TIM_Pulse = 250;	/* 脉冲宽度 */  
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    
    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);		
}


/*!
    \brief 	调整占空比
    \param[in] 	duty:占空比，整数1-100
*/
void set_pwm_duty(uint8_t duty)
{	
    TIM_SetCompare2(TIM1, duty*50);
}

/*!
    \brief 	调整PWM频率
    \desc 	通过调整计数器预分频系数调整计数器时钟，从而调整PWM频率
    \param[in] freq:计数器预分频器分频系数
*/
void set_pwm_freq(uint16_t freq)
{
    TIM_PrescalerConfig(TIM1, freq - 1, TIM_PSCReloadMode_Immediate);
}

/*!
    \brief	定时器1初始化
    \desc	实现PWM功能
*/
void timer1_init(void)
{
    timer1_gpio_config();
    timer1_config();
}
