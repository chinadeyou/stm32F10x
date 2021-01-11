/*!
    \file  adc.h
    \brief the header file of adc.c
*/

#ifndef __ADC_H
#define __ADC_H

#include <stm32f10x.h>

extern __IO uint16_t adc_value[];

void adc_init(void);

#endif	  /* __ADC_H */
