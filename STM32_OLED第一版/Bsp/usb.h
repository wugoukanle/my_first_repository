#ifndef __USB_H
#define __USB_H


#include "general.h"
#define UARTRXLEN       254
#define UARTTXLEN       0

#define ERR_UART_OK     0x00   //正常
#define ERR_UART_EMPTY  0x01   //缓冲区空

/*****
定义一个接收数据缓冲区
******/
typedef struct tgUARTBUFF
{
  char   cFlag;
  u16  cRHead; //要处理的字符的序号
  u16  cRTail; //当前缓冲区有效长度
  char  RBuff[UARTRXLEN];//缓冲区
} UARTBUFF;

#define BR_9600         0
#define BR_115200       1
#define BR_1200         2
#define BR_2400         3
#define BR_4800         4

#define  USB_CHEAK          (GPIOC->IDR  & GPIO_Pin_12)  //检测USB接口是否连接到计算机上,即PC12


#define  USB_RST_HIGH          (GPIOB->BSRR = GPIO_Pin_10)
#define  USB_RST_LOW           (GPIOB->BRR  = GPIO_Pin_10) 

#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0




extern   unsigned char    m_Uart_State;
uchar  USB_Cheak_init(void);
void Usb_IO_Init_In(void);
void USB_transmit(void);
void USART_Configuration(void);
void UARTReset(char uart);
void Second_UART_rest(void);
void UARTnSendChar(char uart,u8 *str,u8 len);
void UARTSendChar(char uart,char data);
char UARTReadChar(char uart);
u16 UARTnReadChar(char uart, u8 *dst, u16 len);
void Usb_Rst_Init_Out(void);
uchar UARTReadFrm(char uart, uchar *pdst, unsigned int max);
u16 UARTnReadChar(char uart, u8 *dst, u16 len);
char UARTReadChar(char uart);
u16 UARTGetCount(char uart);
void WaitUartCharCount0(u8 cnt);
void WaitUartChar(u8 ch);
void Usb_Power_Init_In(void);
void USB_info1(void);
void USB_info2(void);
void USB_info3(void);
void USB_info4(void);
void USB_info5(void);
void Deal_Usbdata(void);
void USB_info6(void);

void USB_info10(void);
void USB_info11(void);
void USB_info12(void);
void USB_info13(void);
void Read_info4(void);

uchar Mode(uint K);
void New_Save(uint row_len);
void HashInsert(uchar t[],uint nChar,uchar nm[],uint addr);

void  Read_Text(void);

void  Delete_LockNum(uchar buff[]);
uchar Delete_OneLockNum(void);
uchar Increase_Onelocknum(void);
void  increase_LockNum(uchar buff[],uchar de_len);
void  Ask_LeftyhNum(void);
void  Test_USB_Record(void);
void  Send_YongHu(void);
uchar UARTReadLine(uchar uart, uchar *pdst, uchar max);
void  UARTSend(char uart,const char *str);
void  UARTSendNewLine(char uart,const char *str);
void  Get2LongPara(uchar *str,long *pa, long *pb);
void  Test_Usb_Data(void);

void USB_transmit_test(void);


void download_yonghu(void);
void key_encode_query(void);
void left_suo_space(void);
void time_verify(void);

void authorization(void);
void auto_back_authorization(void);
void caima(void);

void auto_back_caima(void);

void delete_suo(void);

void download_xunjian(void);
void auto_back_xunjian(void);

void clear_all_author(void);

void pc_to_slave(void);

void pc_end_com(void);

void reset_eeprom(void);

uchar key_state(void);


#endif

