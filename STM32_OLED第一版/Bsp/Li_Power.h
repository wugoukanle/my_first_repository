#include "general.h"
//ѡȡ��ѹΪ7.2V��ʱ����Ϊ�ǵ͵�ƽ
//
//#define  BATTERY_VALUE        0XA15    //��ؼ�����

//#define  BATTERY_VALUE        0xA00

#define  BATTERY_VALUE        0x57a      //����﮵�أ���Ϊ�ĵ�ѹֵ
void ADC_GPIO_Configuration(void);
void ADC_Configuration(void);
u8 Power_Cheak(void);
void Spx_En_Out(void);

//#define  SPX_EN_HIGH          (GPIOB->BSRR = GPIO_Pin_5)
//#define  SPX_EN_LOW           (GPIOB->BRR  = GPIO_Pin_5) 
