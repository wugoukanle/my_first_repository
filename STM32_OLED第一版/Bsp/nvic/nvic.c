#include "nvic.h"
#include "misc.h"
#include "stm32f10x.h"

/*
*
*	中断分组设置函数
*
*/
void NVIC_Group_Init(uint32_t NVIC_PriorityGroup)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup);

}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
// EXTI_InitTypeDef EXTI_InitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;   //定义一个结构体
// #ifdef  VECT_TAB_RAM  
//   /* Set the Vector Table base location at 0x20000000 */ 
//   NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
// #else  /* VECT_TAB_FLASH  */
//   /* Set the Vector Table base location at 0x08000000 */ 
//   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
// #endif
	
 /**此段程序是初始化串口1的中断部分**/
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	//中断向量
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
	
  /**UART2中断初始化部分**/
	
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;		//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
      /* Configure and enable ADC interrupt */
			
//   NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
	

//      /* Enable the EXTI9_5 Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);

//        /* Enable the EXTI15_10 Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
  
 

}
