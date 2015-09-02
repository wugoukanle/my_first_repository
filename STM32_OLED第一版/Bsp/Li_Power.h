#include "general.h"
//选取电压为7.2V的时候，认为是低电平
//
//#define  BATTERY_VALUE        0XA15    //电池检测界限

//#define  BATTERY_VALUE        0xA00

#define  BATTERY_VALUE        0x57a      //单节锂电池，认为的低压值
void ADC_GPIO_Configuration(void);
void ADC_Configuration(void);
u8 Power_Cheak(void);
void Spx_En_Out(void);

//#define  SPX_EN_HIGH          (GPIOB->BSRR = GPIO_Pin_5)
//#define  SPX_EN_LOW           (GPIOB->BRR  = GPIO_Pin_5) 
