#include "bsp.h"

//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"






void Bsp_Init(void)
{
	
	SystemTick_Init();
	//NVIC_Group_Init(NVIC_PriorityGroup_2);//ʹ���жϷ��� 2
// 	Uart1_Init();													//����1��ʼ��
// 	Usart1_NVIC_Configuration();					//����1 �ж�����
	OLED_Init();

	//LCD_GPIO_Init();
	
}






