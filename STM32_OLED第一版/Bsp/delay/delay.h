#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void SystemTick_Init(void) ;
void delay_us(uint32_t num);
void delay_ms(uint32_t num);
void Delay(uint32_t num);
void delay_sus(uint32_t n);
void msDelay(unsigned int idx);

#endif


