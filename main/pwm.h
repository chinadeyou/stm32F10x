/*!
	\file    pwm.h
	\brief   the header of pwm.c
*/

#ifndef __PWM_H
#define __PWM_H

#include <stdio.h>
#include "stm32f10x.h"


void timer1_init(void);

void set_pwm_duty(uint8_t duty);
void set_pwm_freq(uint16_t freq);

#endif /* __PWM_H*/

