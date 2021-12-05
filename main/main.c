/*!
    \file   main.c
    \brief  main function 
    
    \version 2020-04-01, V1.0.0, 
*/
/* Includes ------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "systick.h"
#include "usart.h"
#include "i2c1.h"
#include "pwm.h"
#include "adc.h"
#include "bme280.h"

/* --------宏定义--------------*/
#define led_off()   GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define led_on()    GPIO_ResetBits(GPIOC, GPIO_Pin_13)

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
    /* configure systick */
    systick_config();
    
    usart1_init();
    
    printf("hello\r\n");
    
    led_init();
    
//  timer1_init();

//  adc_init();
    
    while(1)
    {
        printf("hello world\r\n");
        delay_1ms(500);
    }
}

/****END OF FILE****/
