/*!
    \file  systick.c
    \brief the systick configuration file
    
*/

#include "systick.h"

static __IO uint32_t TimingDelay;


/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* setup systick timer for 1MHz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000000U)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay_1ms(__IO uint32_t count)
{ 
    TimingDelay = count*1000;

    while(TimingDelay != 0);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay_1us(__IO uint32_t count)
{ 
    TimingDelay = count;

    while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    { 
    	TimingDelay--;
    }
} 













