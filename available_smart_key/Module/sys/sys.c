// /**********************************************************************
// *												smart key
// *										 Module：低功耗模块
// *						Copyright(c) 2015-2020 武汉百景互动有限公司
// *										All Rights Reserved. 
// *
// * History:         
// *				<author>    <time>           <version >             <desc>
// *       沈涛     2015-07-20 15:18     1.0             build this file 
// *
// * 模块功能：
// *				低功耗有三种模式：睡眠，停机，待机
// *				本模块使用停机模式
// *
// **********************************************************************/ 



// #include "sys.h"




// void Sys_Standby(void)
// {
// 	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,ENABLE);	   
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR和BKP外设时钟    
// 	PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能

// 	PWR_EnterSTANDBYMode();	  //进入待命（STANDBY）模式 		 
// }


// //系统进入待机模式
// void Sys_Enter_Standby(void)
// {			 
// 	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);		//复位所有IO口
// 	
// 	
// 	
// 	
// 	
// 	Sys_Standby();
// }



// //检测WKUP脚的信号
// //返回值1:连续按下3s以上
// //      0:错误的触发	
// u8 Check_WKUP(void) 
// {
// 	u8 t=0;
// 	u8 tx=0;//记录松开的次数
// 	//LED0=0; //亮灯DS0 
// 	while(1)
// 	{
// 		if(WKUP_KD)//已经按下了
// 		{
// 			t++;
// 			tx=0;
// 		}
// 		else 
// 		{
// 			tx++; //超过300ms内没有WKUP信号
// 			if(tx>3)
// 			{
// 				//LED0=1;
// 				return 0;//错误的按键,按下次数不够
// 			}
// 		}
// 		
// 		delay_ms(30);
// 		if(t>=100)//按下超过3秒钟
// 		{
// 			//LED0=0;	  //点亮DS0 
// 			return 1; //按下3s以上了
// 		}
// 	}
// }  



// //中断,检测到PA0脚的一个上升沿.	  
// //中断线0线上的中断检测
// void EXTI0_IRQHandler(void)
// { 		    		    				     		    
// 	EXTI->PR=1<<0;  //清除LINE10上的中断标志位		  
// 	if(Check_WKUP())//关机?
// 	{		  
// 		Sys_Enter_Standby();  
// 	}
// }



// //PA0 WKUP唤醒初始化
// void WKUP_Init(void)
// {	
// 	GPIO_InitTypeDef  GPIO_InitStructure;  		  
// 	NVIC_InitTypeDef NVIC_InitStructure;
// 	EXTI_InitTypeDef EXTI_InitStructure;

// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
// 	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//上拉输入
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	
// 	
// 	
//     //使用外部中断方式
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//设置按键所有的外部线路
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);	

// 	
// 	
// 	
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //使能按键所在的外部中断通道
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级2级
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //从优先级2级
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
// 	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
// 	
// 	
// 	

// 	if(Check_WKUP()==0) Sys_Standby();    //不是开机,进入待机模式  
// }
