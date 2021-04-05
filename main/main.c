/*!
    \file 	main.c
    \brief 	main function 
	
    \version 2020-04-01, V1.0.0, 
*/
/* Includes ------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "systick.h"
#include "usart.h"
#include "i2c-simulate.h"
#include "pwm.h"
#include "adc.h"

/* --------宏定义--------------*/
#define led_off() 	GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define led_on() 	GPIO_ResetBits(GPIOC, GPIO_Pin_13)

/* --------变量----------------*/

/* --------函数----------------*/

void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void led_flash(void)
{
	led_on();
	delay_1ms(100);
	led_off();
	delay_1ms(100);
}


void usart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	/* USART1 Tx */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	/* USART1 Rx */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void adc_calculation(void)
{
	int i;
	float voltage[2];
	for ( i = 0; i < 2; i++)
	{
		voltage[i] = adc_value[i] * 3.3/4096;
		printf("adc_value[%d] = %f\r\n", i, voltage[i]);
	}	
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	int i = 0;
	/* configure systick */
	systick_config();
	
	usart1_init();
	
	printf("hello\r\n");
	
	led_init();
	
//	timer1_init();

	adc_init();
	
	while(1)
	{
		delay_1ms(500);
//		led_flash();
//		adc_calculation();
		printf("hello\r\n");

	}
}

/* Retargets the C library printf function to the USART. */
int fputc(int ch, FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}

/****END OF FILE****/
