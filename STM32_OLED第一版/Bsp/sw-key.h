#ifndef __SWKEY_H
#define __SWKEY_H

#include "general.h"


#define  KEY1	     0X01
#define  KEY2		 0X02
#define  KEY3		 0X03
#define  KEY4		 0X04
#define  KEYNO		 0XFF
#define  KEYINIT	 0XFE

// #define  KEYUP         KEY3		 //按键向上
// #define  KEYDOWN       KEY1		 //按键向下
// #define  KEYSURE       KEY4	   //按键确定
// #define  KEYCLEAR      KEY2		 //按键取消


#define  KEYDOWN        KEY3		 //按键向下
#define  KEYSURE       KEY1		 //按键取消
#define  KEYCLEAR      KEY4	 //按键确认
#define  KEYUP       KEY2		 //按键向上


#define GPIO_PIN_KEY1_BUTTON               GPIO_Pin_1
#define GPIO_PIN_KEY2_BUTTON               GPIO_Pin_13
//#define GPIO_PIN_KEY3_BUTTON               GPIO_Pin_10
//#define GPIO_PIN_KEY4_BUTTON               GPIO_Pin_11

#define EXTI_LINE_KEY1_BUTTON              EXTI_Line8
#define GPIO_PORT_SOURCE_KEY1_BUTTON       GPIO_PortSourceGPIOC
#define GPIO_PIN_SOURCE_KEY1_BUTTON        GPIO_PinSource8

#define EXTI_LINE_KEY2_BUTTON              EXTI_Line9
#define GPIO_PORT_SOURCE_KEY2_BUTTON       GPIO_PortSourceGPIOC
#define GPIO_PIN_SOURCE_KEY2_BUTTON        GPIO_PinSource9

#define EXTI_LINE_KEY3_BUTTON              EXTI_Line10
#define GPIO_PORT_SOURCE_KEY3_BUTTON       GPIO_PortSourceGPIOC
#define GPIO_PIN_SOURCE_KEY3_BUTTON        GPIO_PinSource10

#define EXTI_LINE_KEY4_BUTTON              EXTI_Line11
#define GPIO_PORT_SOURCE_KEY4_BUTTON       GPIO_PortSourceGPIOC
#define GPIO_PIN_SOURCE_KEY4_BUTTON        GPIO_PinSource11

void Init_KeyNumuber(void);
void Init_Sw_Key_Io(void);
void Exit_Line_Init(void);
uchar GetKey(void);
void InsertKey(uchar nChar);
void ResetKey(void);
void Scan_Key(void);


#endif
