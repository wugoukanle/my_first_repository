#include "sw-key.h"
#include "stm32f10x_exti.h"
#include "delay.h"


uchar Key_Num;

 /*���Ǵ��
   SW1---	PC12
   SW2--- PC9
   SW3---	PA8
   SW4---	PA11
	PC12  .PC9��ʱ������
	
	�����ǶԵ�
	sw1---PC8
	sw2---PC9
	sw3---PC10
	sw4---PC11
	
 */

void Init_KeyNumuber(void)
{
   Key_Num =KEYINIT;
	
// GPIOC->IDR= GPIO_Pin_8;
// GPIOC->IDR= GPIO_Pin_9;
// GPIOC->IDR= GPIO_Pin_10;
// GPIOC->IDR= GPIO_Pin_11;
	
	
	//GPIOC->BSRR = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
}

/***
 ��ȡ����ֵ�Ĵ洢������
***/
uchar GetKey(void)
{
	uchar KN;
  //if((Key_Num!=KEYNO) &&(Key_Num!=KEYINIT))		//KEYNO=0xFF,KEYINIT=0xFE
if(Key_Num!=KEYNO)		//KEYNO=0xFF,KEYINIT=0xFE
  {
    Scan_Key();		//ɨ�谴�����޸�Key_Num
  }
  KN =Key_Num;
  return KN;

}

void InsertKey(uchar nChar)
{
 Key_Num =nChar;

}

void ResetKey(void)
{
 Key_Num =KEYNO;

}

void Scan_Key(void)
{
	
	if(((GPIOC->IDR) & GPIO_Pin_1)==0) Key_Num=KEY1; //�����ǵ͵�ƽ��ʾ��������
	if(((GPIOC->IDR) & GPIO_Pin_13)==0) Key_Num=KEY2;	
	//if(((GPIOC->IDR) & GPIO_Pin_10)==0) Key_Num=KEY3; 
	//if(((GPIOC->IDR) & GPIO_Pin_11)==0) Key_Num=KEY4; 
	switch(Key_Num)		//��Ҫ���Ըı�Key_Numֵ�ĵط�
	{	
	 //��ʱ��Ϊ��ȥ����
		case KEY1: Delay(50);  if(((GPIOC->IDR) & GPIO_Pin_8)==0) Key_Num=KEY1;break;		//�����ǵ͵�ƽ��ʾ��������
		case KEY2:	Delay(50);	if(((GPIOC->IDR) & GPIO_Pin_9)==0) Key_Num=KEY2;break;		
		//case KEY3:	Delay(50);	if(((GPIOC->IDR) & GPIO_Pin_10)==0) Key_Num=KEY3;break;//
		//case KEY4: Delay(50);	if(((GPIOC->IDR) & GPIO_Pin_11)==0) Key_Num=KEY4;break;//
	}
		//�޸�Key_Num����ļ��е�ȫ�ֱ���

}




// void Scan_Key() 
// { 
//         unsigned char num_key=0;//������ 
//         unsigned char temp=0;//���ڶ�ȡP2���ϰ���ֵ 
//         static unsigned char temp_code=0;//���水��ֵ 
//         static unsigned char num_check=0;//�͵�ƽ��Ч���� 
//         static unsigned char key_flag=0;//������Ч��ʶ 

//         temp=GPIOC->IDR&0x0E00;//��ȡP2������ 

//         if(temp!=0x0E00)//�͵�ƽ�ж� 
//         { 
//                 num_check++; 
//                 if(num_check>9)//����10��(10ms)�͵�ƽ��Ч,����Ϊ������Ч 
//                 { 
//                         key_flag=1;//ʹ�ܰ�����Ч��ʶ 
//                         temp_code=temp;//���水��ֵ 
//                 } 
//         } 
//         else//����ʱ�ж� 
//         { 
//                 num_check=0; 

//                 if(key_flag==1)//������Ч 
//                 { 
//                         key_flag=0; 

//                         switch(temp_code)//��ȡ������ 
//                         { 
// //                                 case 0x0E00: num_key=1; 
// //                                            break; 
//                                 case 0x0C00: num_key=2; 
//                                            break; 
//                                 case 0x0A00: num_key=3; 
//                                            break; 
//                                 case 0x0600: num_key=4; 
//                                            break; 
//                         } 
//                 } 
//         } 
//         Key_Num=num_key; 
// } 













 /***
 �ⲿ�жϵ�����
 �½��ش����жϵ�ģʽ
 ****/
 void Exit_Line_Init(void)
 {	
  
   EXTI_InitTypeDef EXTI_InitStructure;
 /* Connect Key Button EXTI Line to Key Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_KEY1_BUTTON, GPIO_PIN_SOURCE_KEY1_BUTTON);

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
//   EXTI_InitStructure.EXTI_Line = EXTI_LINE_KEY1_BUTTON;
//   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Falling;
//   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//   EXTI_Init(&EXTI_InitStructure);

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  EXTI_GenerateSWInterrupt(EXTI_LINE_KEY2_BUTTON);

   /* Connect Key Button EXTI Line to Key Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_KEY2_BUTTON, GPIO_PIN_SOURCE_KEY2_BUTTON);

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_KEY2_BUTTON;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  EXTI_GenerateSWInterrupt(EXTI_LINE_KEY2_BUTTON);

   /* Connect Key Button EXTI Line to Key Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_KEY3_BUTTON, GPIO_PIN_SOURCE_KEY3_BUTTON);

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_KEY3_BUTTON;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

/* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  EXTI_GenerateSWInterrupt(EXTI_LINE_KEY3_BUTTON);

   /* Connect Key Button EXTI Line to Key Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_KEY4_BUTTON, GPIO_PIN_SOURCE_KEY4_BUTTON);

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_KEY4_BUTTON;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  EXTI_GenerateSWInterrupt(EXTI_LINE_KEY4_BUTTON);
}

 void Init_Sw_Key_Io(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY1_BUTTON;		//����KEY1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);//Initializes the GPIOx peripheral according to the specified
																				//parameters in the GPIO_InitStruct.

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY2_BUTTON;		//����KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    
// 	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY3_BUTTON;		//����KEY3
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
//  
//    
//  	
//   GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY4_BUTTON;		//����KEY4
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//   GPIO_Init(GPIOC, &GPIO_InitStructure);
// 	
//  


}

//  //11.12
//  /***
//   �˰����ж�Ϊ�����¼����ж�Ϊ�а�������������û�м�ⰴ������������
//   ���Ժ�Ĳ����м��ɿ���
// --PC8 --KEY1
// --PC9 --KEY2 
// --PC10 --KEY3 
// --PC11 --KEY4
//  ****/
// void EXTI15_10_IRQHandler(void)
// {
//   
//   if(EXTI_GetITStatus(EXTI_LINE_KEY3_BUTTON) != RESET)
//   {
//     /* Toggle GPIO_LED pin 6 */
// 	 {
// 	   Key_Num =KEY3;

// 	 }
//     /* Clear the Key Button EXTI line pending bit */
//     EXTI_ClearITPendingBit(EXTI_LINE_KEY3_BUTTON);
//   }
//   else if(EXTI_GetITStatus(EXTI_LINE_KEY4_BUTTON) != RESET)
//   {
//     {
// 	   Key_Num =KEY4;
// 		
// 	}
// 	EXTI_ClearITPendingBit(EXTI_LINE_KEY4_BUTTON);
//   }



// }

//  //8.9
// void EXTI9_5_IRQHandler(void)
// {
//  if(EXTI_GetITStatus(EXTI_LINE_KEY1_BUTTON) != RESET)
//   {
//     /* Toggle GPIO_LED pin 6 */
// 	 {
// 		Key_Num =KEY1;

// 	 }
//     /* Clear the Key Button EXTI line pending bit */
//     EXTI_ClearITPendingBit(EXTI_LINE_KEY1_BUTTON);
//   }
//   else if(EXTI_GetITStatus(EXTI_LINE_KEY2_BUTTON) != RESET)
//   {
//     {
// 		 Key_Num =KEY2;

//     }
// 	EXTI_ClearITPendingBit(EXTI_LINE_KEY2_BUTTON);
//   }
// }
