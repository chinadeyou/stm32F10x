/*!
    \file  systick.h
    \brief the header file of systick
*/

#ifndef SYS_TICK_H
#define SYS_TICK_H

#include <stdint.h>
#include "stm32f10x.h"

/* configure systick */
void systick_config(void);
/* delay a time in milliseconds */
void delay_1ms(__IO uint32_t count);
void delay_1us(__IO uint32_t count);
/* delay decrement */
void TimingDelay_Decrement(void);

#endif /* SYS_TICK_H */
