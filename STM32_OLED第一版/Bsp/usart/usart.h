#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>



void Uart1_Init(void);
void Usart1_NVIC_Configuration(void);


int fputc(int ch,FILE *f);
int fgetc(FILE *f);



#endif
