#include "delay.h"
#include "stm32f10x_rcc.h"


uint32_t delay_num;

void SystemTick_Init(void)        //1us中断一次
{
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);  //调用标准库函数，获取系统时钟
//	SysTick_Config(rcc_clocks.HCLK_Frequency/1000000);//初始化并使能SysTick	//HCLK_Frequency=72MHZ
	SysTick_Config(72);//初始化并使能SysTick
	
}

void delay_us(uint32_t num)
{
	uint32_t tnow=0,told=0,val=0,reload=0;
	reload=SysTick->LOAD;		//71
	told=SysTick->VAL;
	
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow<told)
			val+=told-tnow;
		else val+=reload-tnow+told;
		if((num*72)<=val)
			break;
		told=tnow;
	}
}




void delay_ms(uint32_t num)
{
	uint32_t tnow=0,told=0,val=0,reload=0;
	reload=SysTick->LOAD;		//reload=71
	told=SysTick->VAL;			//SysTick->VAL的值是小于72的
	
	while(1)
	{
		tnow=SysTick->VAL;		
		if(tnow<told)
			val+=told-tnow;
		else val+=reload-tnow+told;
		
		//val是计数次数，每次的时间是1/72M
		
		if((num*72*1000)<=val)
			break;
		told=tnow;
	}	
}

void Delay(uint32_t num)
{
	delay_ms(num);
}

//  void delay_us(uint32_t n)
// {
//   int i;
//   for(i=0;i<n;i++)
//   delay_sus(0x8000);
//    	 
// }
// void delay_sus(uint32_t n)
// {
//   int i;
//   for(i=0;i<n;i++);	 
// }
// //延时尽量的精确
// //i =800的时候，基本为1ms
// void delay_ms(uint32_t n)
// {
//     int i,j;
//     for(j =0;j<n;j++)
//     {
//       for(i=0;i<800;i++)
//       delay_sus(0x8000);   //1us多一点
//     }
// }

void msDelay(unsigned int idx)
{
		unsigned int inc;
		for(inc =0;inc<idx;inc++);
}
