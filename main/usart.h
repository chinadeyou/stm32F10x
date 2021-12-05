/*!
    \file  usart.h
    \brief the header file of usart.c
*/

#ifndef __USART_H
#define __USART_H

#include <stm32f10x.h>

void usart1_init(void);
void usart2_init(void);
void usart3_init(void);

void usart1_senddata(uint8_t ch);
void usart2_senddata(uint8_t ch);
void usart3_senddata(uint8_t ch);

int8_t usart1_getc(void);
#endif	  /* __USART_H */
