/*!
    \file 	retarget.c
    \brief 	输出重定向
    
    \version 2021-12-05
*/

#include <stdio.h>
#include "usart.h"
//#pragma import(__use_no_semihosting_swi)
#pragma import(__use_no_semihosting)  // 不使用半主机模式

struct __FILE {
    int handle;
    /* You want to do */
};

FILE __stdout;
FILE __stdin;

/* Retargets the C library printf function to the USART. */
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}

    return ch;
}

int fgetc(FILE *f)
{
    return ((int)(usart1_getc()));
}

void _ttywrch(int ch)
{
    usart1_senddata(ch);
}

int ferror(FILE *f)
{
    /* error */
    return EOF;
}

void _sys_exit(int status)
{
    while(1);
}
