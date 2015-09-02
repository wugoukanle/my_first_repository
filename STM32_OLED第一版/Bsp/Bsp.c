#include "bsp.h"

//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"






void Bsp_Init(void)
{
	
	SystemTick_Init();
	//NVIC_Group_Init(NVIC_PriorityGroup_2);//使用中断分组 2
// 	Uart1_Init();													//串口1初始化
// 	Usart1_NVIC_Configuration();					//串口1 中断配置
	OLED_Init();

	//LCD_GPIO_Init();
	
}






