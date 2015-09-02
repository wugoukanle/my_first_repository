#include "usart.h"
#include "stm32f10x_usart.h"
#include "stm32f10x.h"

/*
*
*	串口1初始化函数
*
*/
void Uart1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	
	//配置UART1 TX（PA.9）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置UART1 RX（PA.10）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART1 模式配置
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	// 使能串口1接收中断 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1,ENABLE);//使能USART1

}

/*
*
*	串口1的中断配置
*
*/
void Usart1_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/// 配置USART1接收中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


/*
*
*	重定向c库函数printf到USART1
*
*/ 
int fputc(int ch,FILE *f)
{

	//发送一个字节数据到USART1
	USART_SendData(USART1,(uint8_t) ch);
	
	//等待发送完毕
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	
	return (ch);

}

/*
*
*	重定向c库函数scanf到USART1
*
*/ 
int fgetc(FILE *f)
{
		// 等待串口1输入数据 
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}


// /*******************************************************************************
// * Function Name  : NVIC_Configuration
// * Description    : Configures Vector Table base location.
// * Input          : None
// * Output         : None
// * Return         : None
// *******************************************************************************/
// void NVIC_Configuration(void)
// {
// // EXTI_InitTypeDef EXTI_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;   //定义一个结构体
// #ifdef  VECT_TAB_RAM  
//   /* Set the Vector Table base location at 0x20000000 */ 
//   NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
// #else  /* VECT_TAB_FLASH  */
//   /* Set the Vector Table base location at 0x08000000 */ 
//   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
// #endif
// 	
//  /**此段程序是初始化串口1的中断部分**/
// 	
//   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
//   //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	
// 	
//   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel; 	//中断向量
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//响应优先级
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//   NVIC_Init(&NVIC_InitStructure); 
// 	
//   /**UART2中断初始化部分**/
// 	
//   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//响应优先级
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
// 	
//   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;		//抢占优先级
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure); 
// 	
//       /* Configure and enable ADC interrupt */
// 			
//   NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
// 	

//      /* Enable the EXTI9_5 Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);

//        /* Enable the EXTI15_10 Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
//   
//  

// }



/*
* 函数名： USART1_Config
* 描述 ： USART1 GPIO 配置,工作模式配置。 115200 8-N-1
* 输入 ：无
* 输出 : 无
* 调用 ：外部调用
*/
void USART1_Config(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 USART_InitTypeDef USART_InitStructure;

 /* config USART1 clock */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

 /* USART1 GPIO config */
 /* Configure USART1 Tx (PA.09) as alternate function pushpull */
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 /* Configure USART1 Rx (PA.10) as input floating */
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 /* USART1 mode config */
 USART_InitStructure.USART_BaudRate = 115200;
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 USART_InitStructure.USART_Parity = USART_Parity_No ;
 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 USART_Init(USART1, &USART_InitStructure);
 USART_Cmd(USART1, ENABLE);
 }

