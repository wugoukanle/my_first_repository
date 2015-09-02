#ifndef __RFID_H
#define __RFID_H



#include "general.h"
#include "usart.h"


#define  UARTRXLEN1	    10
typedef struct tg1UARTBUFF
{
  char   cFlag;
  u16  cRHead; //要处理的字符的序号
  u16  cRTail; //当前缓冲区有效长度
  char  RBuff[UARTRXLEN1];//缓冲区
} UARTBUFF1;
 #define GPIO_PIN_KEY_BUTTON               GPIO_Pin_8
 #define EXTI_LINE_KEY_BUTTON              EXTI_Line8
 #define GPIO_PORT_SOURCE_KEY_BUTTON       GPIO_PortSourceGPIOG
 #define GPIO_PIN_SOURCE_KEY_BUTTON        GPIO_PinSource8
void resume_wg26_data(void);
void store_tag_bit(unsigned char tag_data_bit);
void Rfid_IO_Init_In(void);
void USART2_Configuration(void);
void UART2SendChar(char uart,char data);
uchar Read_RfidData(char uart, uchar *pdst, char max);

void UART2Reset(char uart);

#endif
