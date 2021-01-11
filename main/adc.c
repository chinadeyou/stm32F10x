/*!
    \file  adc.c
    \brief ADC 配置。定时器触发
*/

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

__IO uint16_t adc_value[2];


void adc_config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		//扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;	//ADC外部触发信号开启
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据对齐方式：右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;		//采样通道个数 ：1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channel14 configuration 采样通道配置*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 external trigger */ 
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	
}

void dma_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA1 Channel1 Configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void timer_config(void)
{
	/* ---------------------------------------------------------------
    TIM1 Configuration: Output Compare Timing Mode:
    TIM1CLK = 72 MHz, Prescaler = 7200, TIM1 counter clock = 10KHz
  --------------------------------------------------------------- */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = (10000 - 1);		//周期，自动装载值          
	TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);   //预分频，
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    	/* 定时器时钟与死区时间、采样时钟直接的分频比例 */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	/* TIM1 channel1 configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
	TIM_OCInitStructure.TIM_Pulse = 0x7F; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	
	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);
	/* TIM1 main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);  
}

/**
  * @brief  配置ADC GPIO.
  * @param  None
  * @retval None
  */
void adc_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOA, ADC1 and TIM1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Configure PA1 and PA2 (ADC Channel1 and Channel2) as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void adc_init(void)
{
	/* Enable ADC1 and TIM1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_TIM1, ENABLE);
	
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* ADCCLK = PCLK2/6 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	adc_gpio_config();

	timer_config();

	dma_config();

	adc_config();
}

