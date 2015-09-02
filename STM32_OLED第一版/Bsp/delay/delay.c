#include "delay.h"
#include "stm32f10x_rcc.h"


uint32_t delay_num;

void SystemTick_Init(void)        //1us�ж�һ��
{
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);  //���ñ�׼�⺯������ȡϵͳʱ��
//	SysTick_Config(rcc_clocks.HCLK_Frequency/1000000);//��ʼ����ʹ��SysTick	//HCLK_Frequency=72MHZ
	SysTick_Config(72);//��ʼ����ʹ��SysTick
	
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
	told=SysTick->VAL;			//SysTick->VAL��ֵ��С��72��
	
	while(1)
	{
		tnow=SysTick->VAL;		
		if(tnow<told)
			val+=told-tnow;
		else val+=reload-tnow+told;
		
		//val�Ǽ���������ÿ�ε�ʱ����1/72M
		
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
// //��ʱ�����ľ�ȷ
// //i =800��ʱ�򣬻���Ϊ1ms
// void delay_ms(uint32_t n)
// {
//     int i,j;
//     for(j =0;j<n;j++)
//     {
//       for(i=0;i<800;i++)
//       delay_sus(0x8000);   //1us��һ��
//     }
// }

void msDelay(unsigned int idx)
{
		unsigned int inc;
		for(inc =0;inc<idx;inc++);
}
