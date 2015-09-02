#include "usb.h" 
#include "stm32f10x_usart.h"
#include "Timer.h"
#include "rtc_time.h"
#include "Li_Power.h"
#include "Key_Transfer.h"
#include "spi_flash.h"
#include "stdio.h"
#include "string.h"
#include "general.h"
#include "oled.h"
#include "delay.h"
#include "App.h"

#include "Globle_Var.h"


//#include <stdlib.h>
#define   M   173

#define  MAX_BUFF      520     //255
#define  MAX_TI        246
#define  WRITE_ZHI     40

extern unsigned char m_Uart_State;
extern unsigned char time_buff[6];
// unsigned char key_in_code[4];
// unsigned int  battery[20];
uchar Ti[MAX_TI];


uchar a_f[24];

extern unsigned char Lock_number[4];

UARTBUFF  m_UB0;
void up_loadInfo(void);
void Send_Opretar(void);


extern uchar Rfid_Buff[6]; 
extern uchar g_Buff[MAX_BUFF];
extern uint8_t g_State;

// void Clear_WDT(void)
// {
//   IWDG_ReloadCounter();	  //�忴�Ź���ֵ
// }

// uchar ReadData(uint address)
// {

// }

// void WritEDAta(uint address,uchar data)
// {

// }

// void read_time(void)
// {

// }

// void set_time(u16 year,u8 month,u8 day,u8 hour,u8 min,u8 sec)
// {


// }


//Delete_LockNum
//USART1_IRQHandler

/****
���õ�Ϊ����1
*****/

/**
USB�Ƿ����ӹܽ�

**/
 void Usb_Power_Init_In(void)
 {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);    //Key_IO--ͨ�Źܽ�		PC12��USB_BIT
  
 }
/**
 USB����оƬ�ĸ�λ��

**/
 void Usb_Rst_Init_Out(void)
 {

 }
/*
����USBת���ڵ�оƬ�����ʴ������Ϊ115200bps

*/
/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures the USART1.
* Input          : None
* Output         : None
* Return         : None
 115200	bps	
*******************************************************************************/
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
  USART_InitStructure.USART_BaudRate =115200;//115200;//4800;//9600; //115200; //9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//������ͬ��ģʽ�²��õ�
  //USART_InitStructure.USART_Clock = USART_Clock_Disable;
  //USART_InitStructure.USART_CPOL = USART_CPOL_Low;
  //USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
  //USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

//  USART_StructInit(USART_InitStructure);
  USART_Init(USART1, &USART_InitStructure);
  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /*�����ж�ʹ��*/
}



 void Usb_IO_Init_In(void)
 {

 }
/*******
 ����1�Ľ����жϺ���
********/
// void USART1_IRQHandler(void)
// {
//   u8 nChar;
//   if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//NE�ͱ�ʾ�����ݣ���ʾRDR������
//   {
//     nChar =USART_ReceiveData(USART1);
//     m_UB0.RBuff[m_UB0.cRHead] = nChar;
// 		++m_UB0.cRHead;
// 		if (m_UB0.cRHead >= UARTRXLEN)
// 			m_UB0.cRHead = 0;
//     USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//   }
// }

//��λ���ڻ�����
void UARTReset(char uart)
{		
		//�ȹؽ����ж�
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);            // Disable USART1 RX interrupt
		//UARTBUFF  m_UB0;
		m_UB0.cFlag = 0;
		m_UB0.cRHead = 0;
		m_UB0.cRTail = 0;
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);            // Enable USART1 RX interrupt
}


void Second_UART_rest(void)
{
		 m_UB0.cFlag = 0;
		 m_UB0.cRHead = 0;
		 m_UB0.cRTail = 0;
}


void UARTnSendChar(char uart,u8 *str,u8 len)  //���ڷ����ַ���
{
	u8 idx;
	{
		for (idx=0;idx<len;idx++)
		{
			USART_SendData(USART1,str[idx]);		
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 		//�ȴ��������
		}
	}
}

 //�ȴ����ͣ�ֱ�����
void UARTSend(char uart,const char *str)
{
  char idx=0;
  
  {

    while (str[idx])
    {
      UARTSendChar(1,str[idx]);
      idx++;
    }
    
  }
}


void UARTSendChar(char uart,char data)
{
	
	{   
		 while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
 	   USART_SendData(USART1,data);
	   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  
	}
}


char UARTReadChar(char uart)
{

	char val='*';
	m_Uart_State = ERR_UART_OK;	    //��⴮�ڻ������Ƿ��������
	{

		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);                    // Disable USART0 RX interrupt
		if (m_UB0.cRHead != m_UB0.cRTail)
		{
			val = m_UB0.RBuff[m_UB0.cRTail++];
			if (m_UB0.cRTail>=UARTRXLEN) m_UB0.cRTail = 0;
		}
		else
		  m_Uart_State = ERR_UART_EMPTY;
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    // Enable USART0 RX interrupt

	}
	return val;
	
}

u16 UARTnReadChar(char uart, u8 *dst, u16 len)
{
	u16 idx;
	{

		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);                  // Disable USART0 RX interrupt
	    {
			for (idx=0;idx<len;idx++)
			{
				*(dst+idx) = m_UB0.RBuff[m_UB0.cRTail];
				m_UB0.cRTail = (m_UB0.cRTail+1)% UARTRXLEN;
			}
		}
	
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    // Enable USART0 RX interrupt

	}
	return len;
}

//��ȡһ��,'\r'����,���ض�ȡ���ַ�������
uchar UARTReadLine(uchar uart, uchar *pdst, uchar max)
{
  uchar count = 0,total;
  int idx,off;
 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
   if (m_UB0.cRHead != m_UB0.cRTail)
   {
      //������Ч�����ܳ���
      if (m_UB0.cRHead >= m_UB0.cRTail)
      {
        total  = m_UB0.cRHead - m_UB0.cRTail;
      } else{
        total  = UARTRXLEN + m_UB0.cRHead - m_UB0.cRTail ;
      }
      //�ҽ�����
      for (idx=0;idx<max-1 && idx<total;idx++)
      {
        off = m_UB0.cRTail + idx;
        if (off >= UARTRXLEN)  off -= UARTRXLEN;
        pdst[idx] = m_UB0.RBuff[off];
        if (m_UB0.RBuff[off]=='\r')
        {
            count = idx+1;
            pdst[count] = 0;
            m_UB0.cRTail = off + 1;
            if ( m_UB0.cRTail >= UARTRXLEN) m_UB0.cRTail =0;
            //�������ݣ����Ա�֤"\r\n"������Ӱ����һ�д���
			/*
            if (m_UB0.RBuff [m_UB0.cRTail]=='\n')
            {
                m_UB0.cRTail += 1;
                if ( m_UB0.cRTail >= UARTRXLEN) m_UB0.cRTail =0;
            }
			*/
            break;
        }
      }
   }
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  return count;
}

//�����з��ͣ��ȴ����ͣ�ֱ�����
void UARTSendNewLine(char uart,const char *str)
{
  char idx=0;
 

	UARTSendChar(1,'\r');
    UARTSendChar(1,'\n');
    while (str[idx])
    {
     UARTSendChar(1,str[idx]);
      idx++;
    }
    UARTSendChar(1,'\r');
    UARTSendChar(1,'\n');

  
}


 /***
 Get2LongPara(g_Buff + 6, &addr, &count);

 ****/
// void Get2LongPara(uchar *str,long *pa, long *pb)
// {
//     uchar count=0;
//     if (*str==0)
//     {
//         *pa = 0;
//         *pb = 0;
//         return;
//     }
//     *pa = atol((const char*)str);      //�ڲ����������⣬ǿ�ƶ��壿
//     while (str[count]==' ' && str[count]!=0) ++count;
//     if (str[count]==0) 
//     {
//         *pb = 0;
//         return;
//     }
//     ++count;
//     while (str[count]!=' ' && str[count]!=0) ++count;
//     if (str[count]==0) 
//         *pb = 0;
//     else
//         *pb = atol((const char*)str + count);
// }



 /*
* Function Name  : USB_Cheak_init()
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
  ���USB�Ƿ�ɹ��Ľ���������
  �ȴ���λ������
  aa 75 0a  00  00 00 D5   ��ȷ��֡	  ��ʱ����ת������Կ�׶����������Ѿ��ɹ�
  ���һλΪ����ֵ
 */

uchar  USB_Cheak_init(void)
{
  //uchar i;
	uchar nChar;
	uchar  usb_code[6]={0x55,0x7A,0xBB,0x01,0x00,0x01};//��λ������������λ�����ʹ�������Խ�����ʼ������
	uchar  usb_code2[6]={0x55,0x7A,0xCC,0x01,0x00,0x01};//��λ���ظ���λ�������ԭ������λ����Ҫ
	uchar state =0;
	Clear_WDT();

	//for(i =0;i<20;i++)
	while(1)
	{
	   //void UARTnSendChar(char uart,u8 *str,u8 len)  //���ڷ����ַ���
	   UARTnSendChar(1,usb_code,6);
	   Clear_WDT();
		 g_WaitTimeOut = 0;
		 //u8 SetTimer(u8 timer_id, u16 interval, TimerFun pTask,u16 Time_Top )
		 SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut��g_WaitTimeOut��Ϊtrue
		 while( g_WaitTimeOut == 0 )  //�ȴ����ص�GPSͷ�ε��ֽ�
	   {
	     Clear_WDT();
			 //uchar UARTReadFrm(char uart, uchar *pdst, uint max)
			 nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//g_Buff��һ��ȫ�ֱ�����#define  MAX_BUFF   520 
			 if(nChar>=6&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0xaa&&g_Buff[3]==0x01&&g_Buff[4]==0x00&&g_Buff[5]==0x01)		//Ϊʲô��3��	3��ʾ���յ���λ���ķ��������������
			 {
				 UARTnSendChar(1,usb_code2,6);
				 break;
			 }		//û�м������������λ��û������ջ�����
			 if((USB_CHEAK&(GPIO_Pin_12))==0)
				 return state;
		 }		//���򻹿��ܿ���ʱ����ʱ�˳�ѭ��
		 //���ܵ���λ��������֮����û�г�ʱ
	   KillTimer(TIMER_TIMEOUT);
	   if( g_WaitTimeOut == 0)		//��ʾδ��ʱ�˳�������ʾ�յ�����λ���Ļظ�
	   {
			 state =1;
			 Clear_WDT();
			 return state;		//��ȷ�ĳ���Ӧ�������ֻҪĳ�γ��Գɹ��ͷ���state=1
	   }
		 

		 
		 
		 
	}
  //return state;		//20�γ��Զ�ʧ���ˣ��˴�state��0

}


// uchar  USB_Cheak_init(void)
// {
//  uchar i;
// 	uchar nChar;
// 	uchar  usb_code[7]={0x55,0x7a,0xbb,0x00,0x00,0x00,0x94};//��λ������������λ�����ʹ�������Խ�����ʼ������
// 	uchar state =0;
// 	Clear_WDT();

// 	for(i =0;i<20;i++)
// 	{
// 	   //void UARTnSendChar(char uart,u8 *str,u8 len)  //���ڷ����ַ���
// 	   UARTnSendChar(1,usb_code,7);
// 	   Clear_WDT();
// 		 g_WaitTimeOut = 0;
// 		 //u8 SetTimer(u8 timer_id, u16 interval, TimerFun pTask,u16 Time_Top )
// 		 SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut��g_WaitTimeOut��Ϊtrue
// 		 while( g_WaitTimeOut == 0 )  //�ȴ����ص�GPSͷ�ε��ֽ�
// 	   {
// 	     Clear_WDT();
// 			 //uchar UARTReadFrm(char uart, uchar *pdst, uint max)
// 			 nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//g_Buff��һ��ȫ�ֱ���
// 			 if(nChar >=3)		//Ϊʲô��3�����ܱ�ʾ�ط�����
// 			 break;
// 		 }
// 	   KillTimer(TIMER_TIMEOUT);
// 	   if( g_WaitTimeOut == 0)
// 	   {
// 			 state =1;
// 			 Clear_WDT();
// 			 return state;		//��ȷ�ĳ���Ӧ��������
// 	   }
// 	}
//   return state;		//stateҪô��0Ҫô��1
// }



/*
 USBͨ�ų�����
 void USB_transmit(void)
*/ 
void USB_transmit(void)
{
  uchar nChar =0;
	Clear_WDT();
	//���USB�Ƿ�ɹ��Ľ���������  �ȴ���λ������  aa 75 0a  00  00 00 D5   ��ȷ��֡	  
	//��ʱ����ת������Կ�׶����������Ѿ��ɹ�  ���һλΪ����ֵ
  nChar =USB_Cheak_init();		//���ӳ�ʼ��
	
  if(nChar ==1)		//��ʾ�ȴ���ʱ
  {
		OLED_Clear();
		often_show(36,56,"PC��ͨ����..");
		//Deal_Usbdata();
		Test_Usb_Data();
		Clear_WDT();
		MOS_OFF;   //�ر�MAX3232��Դ
		g_State =ST_IDLE;return;
  }
  else if(nChar ==0)
  {
		Clear_WDT();
		MOS_OFF;  //�ر�MAX3232��Դ
		g_State =ST_IDLE;return;
  }

}


void USB_transmit_test(void)
{
  uchar nChar =0;
	Clear_WDT();
	
	//often_show(36,56,"��ʼ��ͨ����..",0,0x0fff);	
	//���USB�Ƿ�ɹ��Ľ���������  �ȴ���λ������  aa 75 0a  00  00 00 D5   ��ȷ��֡	  
	//��ʱ����ת������Կ�׶����������Ѿ��ɹ�  ���һλΪ����ֵ
// 	OLED_Clear();
// 	often_show(10,56,"��ʼ��ͨ����...",0,0x0fff);	

	
	if(USB_Cheak_init_flag==1)
	{
		OLED_Clear();
		often_show(10,56,"��ʼ��ͨ����...");			
		nChar =USB_Cheak_init();		//���ӳ�ʼ��
	}
// 	else if(USB_Cheak_init_flag==0)
// 	{
// 		g_State=ST_IDLE;
// 		return;
// 	}
// 	else
// 		nChar=1;
	
	
  if(nChar ==1||(USB_Cheak_init_flag==0))		//��ʾ��ʼ����������ȷֵ�����Ƿǳ�ʼ����ʾ
  {
		 OLED_Clear();
		 often_show(36,56,"PC��ͨ����..");
		
		 Test_Usb_Data();
		
		 Clear_WDT();
		 //MOS_OFF;   //�ر�MAX3232��Դ
		
		 OLED_Clear();
		 often_show(36,56,"ͨ�Ž���");
		 delay_ms(2000);
		 OLED_Clear();		

		 g_State =ST_IDLE;return;
		
  }
  else if(nChar ==0)
  {
		Clear_WDT();
		MOS_OFF;  //�ر�MAX3232��Դ
		g_State =ST_IDLE;return;
  }

}



/****
 ��USBͨ�ŵ����ݰ����д���
 �ж�ʲôʱ���˳��ĳ���
 1.ͨ���ж�USB���ϵĵ�ѹ�����
 2.ͨ���˳�ָ�������
 ����500���û���Ϣ��ʱ�� ��Ϊ���������
 ��һ����ʾ 
 1.�û����� 2.��ÿ��50�û���������ĺ���
 �ڶ������ݾ�������ݰ�
 aa 75 04  00  00 00 DB
*****/
// void Deal_Usbdata(void)
// {
//    uchar nChar;
//    uchar order ;   //������
//    while(USB_CHEAK ==(GPIO_Pin_12))
//    {
// 			Clear_WDT();
// 			UARTReset(0);				 
// 			while(USB_CHEAK ==(GPIO_Pin_12))  //�ȴ����ص�USbͷ�ε��ֽ�
// 			{
// 				 Clear_WDT();
// 				 nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);
// 				 if(nChar >=3)
// 				 break;
// 			}
// 			order =g_Buff[2];  //ȡ����������
// 			{
// 					switch(order)
// 					{
// 						case 0x04:USB_info1();break;  //��ȡ���ź��豸����--�������
// 						case 0x05:USB_info2();break;  //�Ե���Կ�׽���Уʱ--����Ҫ
// 						case 0x06:up_loadInfo();break;	//��ȡ��������Ϣ����̨--����ʱ������ı�ż�����---���������ӵ���̨��ʱ����ִ���������������ص������ִ��������ϴ�
// 						case 0x07:USB_info4();break;	//��Կ���������ؿɿ���������Ϣ
// 						case 0x08:USB_info5();break;	//ȷ��֡
// 						case 0x0c:USB_info6();break;	//��λ���ݴ洢��ͷָ��--��ϵͳ����ʾΪ��ʼ��Կ��
// 						case 0x01:USB_info10();break;	//����Կ�׵Ĳ�����Ա����Ϣ--1.����ģ 2.������ģ--[ǿ��Ϊ���� ��
// 						case 0x0e:USB_info11();break;  //����Կ���е���Ϣ
// 						case 0x02:USB_info12();break;  //ɾ�����ı�� --����Ҫ
// 						case 0x03:USB_info13();break;  //�������ı�� --����Ҫ
// 						case 0x0f:Send_YongHu();break;  //�����Ѿ��洢�����ı����Ϣ����̨
// 						default:break;
// 					}

// 			}
//    }

// }
/****
����Կ�׺�PC����ͨ�ţ�ͨ��ת�Ӱ�

****/
void Test_Usb_Data(void)
{
	
	 uchar nChar;
   uchar order ;   //������
	 end_flag=0;	//��Ϊȫ�ֱ���
   while(USB_CHEAK ==(GPIO_Pin_12))
   {
				Clear_WDT();
				UARTReset(0);		//��λ���ڽ��ܻ�����				 
				while(1)  //�ȴ����ص�USbͷ�ε��ֽ�
				{
					Clear_WDT();
					nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  ��ȡ��Э���ʽ���͹��������ݣ������н���
					if(nChar >=3)		//���ﲻ�����ж�ͷ�����ַ���
						break;
					if(end_flag==1)
						return;	//ֱ�ӷ��أ���ʱ����USB_BIT�Ƿ��Ѿ��γ�
					if(USB_CHEAK !=(GPIO_Pin_12))		//���USB_BIT�Ѿ��ε�����ֱ�ӷ���
						return;
				}
				
				
				order =g_Buff[2];  //ȡ����������
				
				switch(order)
				{
					case 0x01:download_yonghu();break;
					case 0x02:key_encode_query();break;
					case 0x03:left_suo_space();break;
					case 0x04:time_verify();break;
					
					case 0x05:authorization();break;
					case 0x06:auto_back_authorization();break;
					
					case 0x07:caima();break;
					case 0x08:auto_back_caima();break;		//�Զ��ش���Ȩ���Զ��ش����붼��Կ���������Ϸ�
					
					case 0x09:delete_suo();break;
					
					case 0x0a:download_xunjian();break;
					case 0x0b:auto_back_xunjian();break;
					
					case 0x0c:clear_all_author();break;
					
					case 0x0d:pc_to_slave();break;
					
					case 0x0e:pc_end_com();break;
					
					case 0x0f:reset_eeprom();break;
					
					default:break;
				}
				//UARTnSendChar(1,"11111",5);

	}
}




/*
0x01��λ��������λ���û���Ϣ
2�ֽ��û����+8�ֽ��û���+8�ֽ���������+1�ֽ�Կ�����ֱ��

�û�������ڲ�����Ȩ�ļ��ж�Ӧ���û����

*/
void download_yonghu(void)
{
		uchar i,ID,k,Len;
		uchar xor =0;		//У���
		uchar mid_buff[8];
		uint addr;
	  uint row;
		//data_len=g_Buff[3];		//g_Buff��ȫ�ֱ���
		ID=g_Buff[5];
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x01;
		mid_buff[3] =0x02;
		mid_buff[4] =0x00;
		mid_buff[5] =ID;	
	
		ID=g_Buff[5];
	
		Len=g_Buff[3]+5;		//��ʾ��������ĳ���
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
		{
				addr=AUTHOR_START;
				Clear_WDT();

				row = ReadData(AUTHOR_ROW_H);   
				row = ((row<<8)&0xff00)+ReadData(AUTHOR_ROW_L);
			
				addr=AUTHOR_START;		//����ʼ��ַ��ʼ���		
			
				for(i=0;i<row;i++)		//�����е���Ȩ�м���Ƿ��뵱ǰ�������û������ͬ
				{
						if(ReadData(addr)==g_Buff[6]&&ReadData(addr+1)==g_Buff[7])		//�����û���Ų����û��ĵ�ַ
							break;
						addr=addr+24;		//����һ����Ȩ���ܳ���Ϊ24�ֽ�
				}
				
				//����ҵ���addr�����ҵ��ĵ�ǰ��ַ�����û���ҵ�������������Ȩ֮����µĵ�ַ
				
			  //�������ȴ���Ȩ�ٴ��û�����϶����ҵ�
				if(i<row)
				{
						for(i=0;i<16;i++)		//���û���Ŷ�Ӧ���û���������������Ȩ�ļ��ж�Ӧ����Ȩ��Ϣ�У��ܹ�16�ֽ�
						{
								for(k =0;k<WRITE_ZHI;k++)
									msDelay(500);
								WritEDAta(addr+8+i,g_Buff[8+i]);			//ƫ�Ƶ�ַ��8���ֽ�=2�û����+2�����+4����
								Clear_WDT();
						}
			
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);
						WritEDAta(KEY_ENCODE,g_Buff[i+8]);			//�洢Կ�ױ��
				}
				 
				mid_buff[6]= 0x01;		//���سɹ�
				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
				mid_buff[7]=xor;
				Clear_WDT();
				UARTnSendChar(1,mid_buff,8);	 //���سɹ���ȷ����		
	}
	else
	{
		for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
		Clear_WDT();

		mid_buff[6]= 0x00;
		xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
		mid_buff[7]=xor;
		
		Clear_WDT();
		UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
	}	
	

}


/*
0x02Կ�ױ�Ų�ѯ

*/
void key_encode_query(void)
{	
		uchar i,k,Len;
		uchar xor =0;		//У���
		ulong ChipUniqueID[3];
		uchar CUID[19];
		uchar mid_buff[8];	
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x02;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];	
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
		{	
				ChipUniqueID[2] = *( u32*)(0X1FFFF7E8); // ���ֽ�
				ChipUniqueID[1] = *( u32*)(0X1FFFF7EC); //  
				ChipUniqueID[0] = *( u32*)(0X1FFFF7F0); // ���ֽ�
			
				CUID[0]=0x55;
				CUID[1]=0x7a;
				CUID[2]=0x02;
				CUID[3]=0x0e;	
				CUID[4]=0x00;
				CUID[5]=g_Buff[5];	
			
				CUID[6]=(ChipUniqueID[0]&0xff000000)>>24;
				CUID[7]=(ChipUniqueID[0]&0x00ff0000)>>16;
				CUID[8]=(ChipUniqueID[0]&0x0000ff00)>>8;
				CUID[9]=ChipUniqueID[0]&0x000000ff;	
				
				CUID[10]=(ChipUniqueID[1]&0xff000000)>>24;
				CUID[11]=(ChipUniqueID[1]&0x00ff0000)>>16;
				CUID[12]=(ChipUniqueID[1]&0x0000ff00)>>8;
				CUID[13]=ChipUniqueID[1]&0x000000ff;		
			
				CUID[14]=(ChipUniqueID[2]&0xff000000)>>24;
				CUID[15]=(ChipUniqueID[2]&0x00ff0000)>>16;
				CUID[16]=(ChipUniqueID[2]&0x0000ff00)>>8;
				CUID[17]=ChipUniqueID[2]&0x000000ff;		
				
				xor=0;
				for(i =0;i<18;i++)
					xor =xor^CUID[i];	
				CUID[18]=xor;
				Clear_WDT();
				UARTnSendChar(1,CUID,19);
		}
		else
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();

			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
		}	
}

/*
0x03���洢�ռ��ѯ
*/
void left_suo_space(void)
{		
		uchar i,k,Len;
		uchar mid_buff[11];
		u16 left_space,addr;
		uchar xor =0;		//У���
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x03;

		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];			
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
		{
				addr= ReadData(AUTHOR_POINT_H);   
				addr=((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
			
				//left_space=(AUTHOR_END-addr);

				left_space=(25-ReadData(AUTHOR_ROW_L));		//����ʣ��ռ���ֽ���,�����Ŀռ��׼ȷ�����ǵ�ɾ������Ȩ��
				
				mid_buff[3] =0x04;
			
				mid_buff[6]= (left_space&0xff00)>>8;		
				mid_buff[7]= (left_space&0x00ff);		

				xor=0;
				for(i =0;i<8;i++)
					xor =xor^mid_buff[i];
				mid_buff[8]=xor;
			
				Clear_WDT();
				UARTnSendChar(1,mid_buff,9);		
	  }
		else
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
			
			mid_buff[3] =0x03;
			
			mid_buff[6]= 0x00;
			
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
		}	
}

/*
0x04�Ե���Կ�׽���Уʱ
6���ֽڵ�ʱ�䣨YYMMDDHHMMSS��
*/
void time_verify(void)
{
  uchar time_date[6];
  uchar mid_buff[13];
  uchar i,k,Len;
  uchar xor =0;		//У���
  Clear_WDT();
  mid_buff[0] =0x55;		//��ʾ��λ��������λ��
  mid_buff[1] =0x7a;
  mid_buff[2] =0x04;		//������

  mid_buff[4] =0x00;
  mid_buff[5] =0x01;		//ID
  Clear_WDT();
	
	Len=g_Buff[3]+5;
	
	for(i =0;i<Len-1;i++)
		xor =xor^g_Buff[i];	
	if(xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
	{
			for(i =0;i<6;i++)
				time_date[i] =g_Buff[6+i];		//g_Buff[7~12] 6�����鵥Ԫ
			
			set_time(time_date[0],time_date[1],time_date[2],time_date[3],time_date[4],time_date[5]);//������λ����ʱ������Կ�׵�ʱ��
			msDelay(500);
			Clear_WDT();
			read_time();    //����ʵʱ��ʱ��,����time_buff��
			for( i =0;i<6;i++)
				mid_buff[6+i] =time_buff[i];
		  mid_buff[3] =0x08;		//����
			xor=0;
			for(i =0;i<12;i++)
				xor =xor^mid_buff[i];
			mid_buff[12]=xor;
			Clear_WDT();
			UARTnSendChar(1,mid_buff,13);	 //����ȷ���룻����ʵʱʱ�䣨д�뱸�����ݼĴ�����ʱ�䣩
	}
	else
	{
		for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
		Clear_WDT();
		mid_buff[3] =0x03;		//����
		mid_buff[6]= 0x00;
		xor=0;
		xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
		mid_buff[7]=xor;
		
		Clear_WDT();
		UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
	}
}

/*
0x05����Ȩ�ļ�����Ϣд��EEPROM
ÿ��дһ����Ȩ�ļ�
�ظ����ܳɹ�����
2���ֽ��û����+2�ֽ������ +4�ֽ���RFID

������ͬ����Ȩ������������ظ�
*/
void authorization(void)
{
		uchar data_len,i,high_addr,low_addr,k,total_row,Len;
		uchar xor=0;
		uchar mid_buff[8];
		uint addr;
		data_len=g_Buff[3];		//g_Buff��ȫ�ֱ���
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x05;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
		{	
			
				addr= ReadData(AUTHOR_POINT_H);   
				addr =((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
				Clear_WDT();
				msDelay(500);
				for(i=0;i<data_len-2;i++)		//��ô�������ľ���ô��
				{
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);
						WritEDAta(addr+i,g_Buff[i+6]);		
						Clear_WDT();
				}
				
				YH_BH_H=g_Buff[6];		//�û���ŵĸߺ͵�
				YH_BH_L=g_Buff[7];
							
				high_addr=((addr+24)&0xff00)>>8;   //ȡ��8λ���ٷŵ�
				low_addr =(addr+24)&0x00ff;				 //ÿ����Ȩ24�ֽ�
				//������Ȩ�û���Ϣָ���λ��
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();
				WritEDAta(AUTHOR_POINT_H,high_addr);		//#define AUTHOR_START  0x0b4f 
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(AUTHOR_POINT_L,low_addr);		
				
			
				//Ҫ���ֺ��ַ���ŵ����ݵ����Ǳ�ʾ��ַ��������
				//������Ȩ��Ϣ����ֻ��50��������ֻ���������ĵ�8λ
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();			
				total_row= ReadData(AUTHOR_ROW_L);   		
				total_row=total_row+1;		//��Ȩ��Ϣ������һ
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(AUTHOR_ROW_L,total_row);				
				
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();

				mid_buff[6]= 0x01;
				xor=0;
				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
				mid_buff[7]=xor;
				
				Clear_WDT();
				UARTnSendChar(1,mid_buff,8);	 //����ȷ����
		}
		else
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();

			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
		}
		
}

// void authorization(void)
// {
// 		uchar data_len,i,k,Len;
// 		uchar xor=0;
// 		uchar mid_buff[8];
// 		uint addr;
// 		data_len=g_Buff[3];		//g_Buff��ȫ�ֱ���
// 	
// 		mid_buff[0] =0x55;
// 		mid_buff[1] =0x7a;
// 		mid_buff[2] =0x05;
// 		mid_buff[3] =0x03;
// 		mid_buff[4] =0x00;
// 		mid_buff[5] =g_Buff[5];
// 	
// 		Len=g_Buff[3]+5;
// 		
// 		for(i =0;i<Len-1;i++)
// 			xor =xor^g_Buff[i];	
// 	  if(xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
// 		{	
// 				addr= ReadData(AUTHOR_POINT_H);   
// 				addr =((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
// 				Clear_WDT();
// 				msDelay(500);
// 				for(i=0;i<data_len-2;i++)		//��ô�������ľ���ô��
// 				{
// 						a_f[i]=g_Buff[i+6];
// 						
// 				}

// 				mid_buff[6]= 0x01;
// 				xor=0;
// 				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 				mid_buff[7]=xor;
// 				
// 				Clear_WDT();
// 				UARTnSendChar(1,mid_buff,8);	 //����ȷ����
// 		}
// 		else
// 		{
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			Clear_WDT();

// 			mid_buff[6]= 0x00;
// 			xor=0;
// 			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 			mid_buff[7]=xor;
// 			
// 			Clear_WDT();
// 			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
// 		}
// 		
// }

/*
0x06	�������в�����¼��Ϣ��
ÿ֡: 55 +7A +06+08(���ݳ���)+00+ID+2�ֽ�Ա�����+4�ֽ�����+1�ֽڿ������+1�ֽ�У��λ


*/
void auto_back_authorization(void)
{
		uchar i,j,k,m,ID,total_row,nChar,Len;
		const uchar data_structure_len=5;		//���ݸ�ʽ�ĳ���Ϊ5,4���ֽڵ�RFID��1���ֽڵĿ������
		uchar xor=0;
		uchar mid_buff[14];
		uint addr,start_addr;
		
		ID=0x01;//����ID����01
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x06;

		mid_buff[4] =0x00;
		mid_buff[5] =ID;		
			
		//����
// 		temp_row= ReadData(AUTHOR_ROW_L);
// 		UARTnSendChar(1,&temp_row,1);		
	
		Len=g_Buff[3]+5;		//�����Ϊ7
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[Len-1]&&Len==7)		//�жϽ�������У���Ƿ���ȷ,���������Ϊ7��ȷ�������Ϊ8
		{			
			
				addr = ReadData(RECORD_POINT_H);   
				addr = ((addr<<8)&0xff00)+ReadData(RECORD_POINT_L);
				
				total_row=(addr-RECORD_START)/data_structure_len;		//ÿ����Ȩ24�ֽ�
			
				//���ԣ�����forѭ��֮ǰ
				//UARTnSendChar(1,&total_row,1);				
			
				if(total_row==0)
				{
						Clear_WDT();
						mid_buff[3]=0x03;
						mid_buff[6]=0x00;
						mid_buff[7]=0x00;
					
						mid_buff[8]=0x00;
						mid_buff[9]=0x00;
						mid_buff[10]=0x00;						
						mid_buff[11]=0x00;
					
						mid_buff[12]=0x00;	
					
						mid_buff[13]=0x2B;									
					
						UARTnSendChar(1,mid_buff,14);
				}
				else
				{
						for(i=0;i<total_row;i++)		//��ʱi>=1
						{
							
								start_addr=RECORD_START+i*data_structure_len;		

								mid_buff[6]=YH_BH_H;
								mid_buff[7]=YH_BH_L;
							
							
								for(j=0;j<5;j++)		//4���ֽ�����+1���ֽڿ������
								{
									msDelay(500);	
									mid_buff[8+j]= ReadData(start_addr+j);
								}
								
								mid_buff[12]=shouquan_jieguo;				//�������
								
								mid_buff[3] =0x09;
								xor=0;
								for(m =0;m<13;m++)
									xor =xor^mid_buff[m];						
								mid_buff[13]=xor;
															
								UARTnSendChar(1,mid_buff,14);	 //����һ����Ȩ�ļ�

								Clear_WDT();
								UARTReset(0);		//��λ���ڽ��ܻ�����										
								while(1)  //������һ����Ȩ֮�����ȴ���λ���ķ�������
								{
//											UARTReset(0);		//��λ���ڽ��ܻ�����		(���ڸ�λ���ܷ�������)										
									
											g_WaitTimeOut = 0;
											SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut��g_WaitTimeOut��Ϊtrue
											while( g_WaitTimeOut == 0 );
											KillTimer(TIMER_TIMEOUT);		
									
											msDelay(500);									
											nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520 
											//���û��������nChar=0
																												
											if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xDB)//�������0x01�ͷ���һ����Ȩ
											{
												Clear_WDT();
												//UARTnSendChar(1,"44444",5);	
												
												break;		//ֻ�������ڲ��ȴ�ѭ����������һ����Ȩ
											}	
											else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xDA)//�������0x01��ɾ��������Ȩ���ٷ���һ��
											{


												i=i-1;		//�����ڲ��ȴ�ѭ��֮�󣬻�ֱ������������ѭ����ֱ�ӽ��������Զ��ش�
												break;
// 												Clear_WDT();
// 												UARTnSendChar(1,"11111",5);
// 												for(k =0;k<WRITE_ZHI;k++)
// 													msDelay(500);														
// 												;		//ʲôҲ���������ȴ�
											}
											

								}//�ڲ�whileѭ��

						 }
					}
		}
		else
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
			mid_buff[3] =0x03;
			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
		}
	

}



// void auto_back_authorization(void)
// {
// 		uchar i,j,k,m,ID,total_row,row,nChar,Len;
// 		uchar xor=0;
// 		uchar mid_buff[12];
// 		uint addr,start_addr;
// 		
// 		ID=0x01;//����ID����01
// 		
// 		mid_buff[0] =0x55;
// 		mid_buff[1] =0x7a;
// 		mid_buff[2] =0x06;

// 		mid_buff[4] =0x00;
// 		mid_buff[5] =ID;		
// 			
// 		//����
// // 		temp_row= ReadData(AUTHOR_ROW_L);
// // 		UARTnSendChar(1,&temp_row,1);		
// 	
// 		Len=g_Buff[3]+5;
// 		
// 		for(i =0;i<Len-1;i++)
// 			xor =xor^g_Buff[i];	
// 	  if(xor==g_Buff[Len-1]&&Len==7)		//�жϽ�������У���Ƿ���ȷ,���������Ϊ7��ȷ�������Ϊ8
// 		{			
// 			
// 				addr= ReadData(AUTHOR_POINT_H);   
// 				addr =((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
// 				
// 				total_row=(addr-AUTHOR_START)/24;		//ÿ����Ȩ24�ֽ�
// 			
// 				//���ԣ�����forѭ��֮ǰ
// 				//UARTnSendChar(1,&total_row,1);				
// 			
// 				if(total_row==0)
// 				{
// 						Clear_WDT();
// 						UARTnSendChar(1,"00000",5);
// 				}
// 				else
// 				{
// 						for(i=0;i<total_row;i++)		//���û����Ȩ��ʲôҲ����
// 						{
// 							
// 								start_addr=AUTHOR_START+i*24;																		
// 								for(j=0;j<4;j++)		//4���ֽڣ��û���š������
// 								{
// 									msDelay(500);	
// 									mid_buff[6+j]= ReadData(start_addr+j);
// 								}
// 																												
// 								if(mid_buff[6]==0&&mid_buff[7]==0)		//����û����Ϊ���ʾ������Ȩ�Ѿ���ɾ��
// 										continue;		//������һ����Ȩ
// 								
// 								mid_buff[10]=key_state();		//key_state�������ؿ���״̬
// 								
// 								mid_buff[3] =0x07;
// 								xor=0;
// 								for(m =0;m<11;m++)
// 									xor =xor^mid_buff[m];						
// 								mid_buff[11]=xor;
// 															
// 								UARTnSendChar(1,mid_buff,12);	 //����һ����Ȩ�ļ�

// 								Clear_WDT();
// 								UARTReset(0);		//��λ���ڽ��ܻ�����										
// 								while(1)  //������һ����Ȩ֮�����ȴ���λ���ķ�������
// 								{
// //											UARTReset(0);		//��λ���ڽ��ܻ�����		(���ڸ�λ���ܷ�������)										
// 									
// 											g_WaitTimeOut = 0;
// 											SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut��g_WaitTimeOut��Ϊtrue
// 											while( g_WaitTimeOut == 0 );
// 											KillTimer(TIMER_TIMEOUT);		
// 									
// 											msDelay(500);									
// 											nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520 
// 											//���û��������nChar=0
// 																												
// 											if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x00&&g_Buff[7]==0xDB)//�������0x00�ͷ���һ����Ȩ
// 											{
// 												Clear_WDT();
// 												//UARTnSendChar(1,"44444",5);	
// 												
// 												break;		//ֻ�������ڲ��ȴ�ѭ����������һ����Ȩ
// 											}	

// 											else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xDA)//�������0x01��ɾ��������Ȩ���ٷ���һ��
// 											{
// 												for(j=0;j<24;j++)		//����ǰ��ַ������Ȩ�������ֽڶ���Ϊ0
// 												{
// 														for(k =0;k<WRITE_ZHI;k++)
// 															msDelay(500);											
// 														WritEDAta(start_addr+j,0);
// 														Clear_WDT();
// 												}
// 									

// 												for(k =0;k<WRITE_ZHI;k++)
// 													msDelay(500);													
// 												row=ReadData(AUTHOR_ROW_L);
// 												
// 												//����
// 												//UARTnSendChar(1,&row,1);		//Ϊʲô��������0								
// 												
// 												Clear_WDT();												
// 												
// 												row=row-1;
// 												for(k =0;k<WRITE_ZHI;k++)
// 													msDelay(500);
// 												WritEDAta(AUTHOR_ROW_L,row);		//����������һ����д��
// 												
// 												for(k =0;k<WRITE_ZHI;k++)		//�������ʱ����Ҫ����Ȼ����û��д��ȥ
// 													msDelay(500);												
// 											
// 												row=ReadData(AUTHOR_ROW_L);
// 												msDelay(500);												
// 												//����
// 												//UARTnSendChar(1,&row,1);	
// 												
// 												Clear_WDT();
// 												
// 												break;		//�����ڲ�ѭ���������ȴ�
// 											}
// 											else//(nChar==0)
// 											{																					
// 												//UARTnSendChar(1,"22222",5);	
// 											
// // 												g_WaitTimeOut = 0;
// // 												SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut��g_WaitTimeOut��Ϊtrue
// // 												while( g_WaitTimeOut == 0 );
// // 												KillTimer(TIMER_TIMEOUT);													
// 																							
// 												Clear_WDT();												
// 											}
// // 											else
// // 											{
// // 												//i=total_row;		//�����ڲ��ȴ�ѭ��֮�󣬻�ֱ������������ѭ����ֱ�ӽ��������Զ��ش�
// // 												//break;
// // 												Clear_WDT();
// // 												UARTnSendChar(1,"11111",5);
// // 												for(k =0;k<WRITE_ZHI;k++)
// // 													msDelay(500);														
// // 												;		//ʲôҲ���������ȴ�
// // 											}  
// 											

// 								}//�ڲ�whileѭ��

// 						 }
// 					}
// 		}
// 		else
// 		{
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			Clear_WDT();
// 			mid_buff[3] =0x03;
// 			mid_buff[6]= 0x00;
// 			xor=0;
// 			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 			mid_buff[7]=xor;
// 			
// 			Clear_WDT();
// 			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
// 		}
// 	

// }










/*
0x07�豸����
2���ֽ�Ա�����+2�ֽ������ 
*/
void caima(void)
{
		uchar data_len,i,high_addr,low_addr,k,total_row,Len;
		uchar xor;
		uchar mid_buff[8];
		uint addr;
		data_len=g_Buff[3];		//g_Buff��ȫ�ֱ���
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x07;
		mid_buff[3] =0x06;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		

		
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
		{	

				addr= ReadData(CAIMA_POINT_H);   
				addr =((addr<<8)&0xff00)+ReadData(CAIMA_POINT_L);
				Clear_WDT();
				msDelay(500);
				for(i=0;i<data_len-2;i++)		//data_len-1,1��ʾID��һ���ֽ�
				{
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);
						WritEDAta(addr+i,g_Buff[6+i]);		
						Clear_WDT();
				}
				
				high_addr=((addr+8)&0xff00)>>8;   //������ÿ��8���ֽ�,����ַȡ��8λ���ٷŵ�
				low_addr =(addr+8)&0x00ff;		
				//������Ȩ�û���Ϣָ���λ��
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();
				WritEDAta(CAIMA_POINT_H,high_addr);		//#define AUTHOR_START  0x0b4f 
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(CAIMA_POINT_L,low_addr);		
				
				
				//Ҫ���ֺ��ַ���ŵ����ݵ����Ǳ�ʾ��ַ��������
				//������Ȩ��Ϣ����ֻ��50��������ֻ���������ĵ�8λ
				total_row= ReadData(CAIMA_ROW_L);   		
				total_row=total_row+1;
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(CAIMA_ROW_L,total_row);				//�������ֻ��30��
				
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();
				
				mid_buff[6]= 0x01;
				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
				mid_buff[7]=xor;
				
				Clear_WDT();
				UARTnSendChar(1,mid_buff,8);	 //����ȷ����
				
		}		
		else
		{
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();
				
				mid_buff[6]= 0x00;
				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
				mid_buff[7]=xor;
				
				Clear_WDT();
				UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
		}

}





/*
0x08�Զ��ش�����

2���ֽ�Ա�����+2�ֽ������ +4���ֽڵ�����

��4���ֽڵ�������Կ�ײ���ʱ������

*/
void auto_back_caima(void)
{
		uchar i,j,k,m,ID,total_row,nChar,Len;
		uchar xor=0;
		uchar mid_buff[15];
		uint addr,start_addr;
		
		ID=0x01;//����ID����01
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x08;

		mid_buff[4] =0x00;
		mid_buff[5] =ID;		
			
		//total_row= ReadData(AUTHOR_ROW_L);
		Len=g_Buff[3]+5;
		
		for(m =0;m<Len-1;m++)
			xor =xor^g_Buff[m];	
	  if(Len==7&&xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ,��������������ȷ������������
		{		

				addr= ReadData(CAIMA_POINT_H);   
				addr =((addr<<8)&0xff00)+ReadData(CAIMA_POINT_L);		//�ڽ���Ĳ��빦�����ı����ָ���ֵ
				
				total_row=(addr-CAIMA_START)/8;
				
			  if(total_row==0)
					 UARTnSendChar(1,"00000",5);		//���total_row=0��

				for(i=0;i<total_row;i++)
				{
						start_addr=CAIMA_START+i*8;
						
						for(j=0;j<8;j++)		//8���ֽڣ��û���š�����š�����
							mid_buff[6+j]= ReadData(start_addr+j);
					
						mid_buff[3] =0x0a;		
						xor=0;
						for(m =0;m<14;m++)
							xor =xor^mid_buff[m];	
						mid_buff[14] =xor;	
					
						msDelay(500);
						Clear_WDT();
						UARTnSendChar(1,mid_buff,15);	 //����һ����Ȩ�ļ�
						
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);
						Clear_WDT();

						UARTReset(0);		//��λ���ڽ��ܻ�����				 
						while(1)  
						{
							Clear_WDT();
							nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  
							if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x08&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xD4)
								break;		//����յ�ȷ�ϣ������ȴ�������һ����������
							//UARTnSendChar(1,&total_row,1);
						}
				}
		}		
		else
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
	  	mid_buff[3] =0x03;
			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
		}
		
		
}



/*
0x09ɾ���豸����ɾ����
2�ֽ�����
*/
void delete_suo(void)
{
		uchar i,ID,k,total_row,max_row,Len;
		uchar xor=0;
		uchar mid_buff[8];
		uint addr,addr2;
		
		ID=g_Buff[5];
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x09;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =ID;		
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
		{
				addr2= ReadData(AUTHOR_POINT_H);   
				addr2=((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
				Clear_WDT();
				msDelay(500);
			
				max_row=(addr2-AUTHOR_START)/24;		//ֻ���AUTHOR_START~AUTHOR_POINT֮���������Ȩ������ɾ������Ȩȫ��Ϊ0
			
			
				addr=AUTHOR_START;
				for(i=0;i<max_row;i++)		//��ʵ���Բ������е���Ȩ
				{
						if(ReadData(addr+2)==g_Buff[6])		//��������Ų����û����Ҹ��û�û�б�ɾ��
						{		
								for(k =0;k<WRITE_ZHI;k++)
									msDelay(500);
								if(ReadData(addr+2)==g_Buff[7])
										break;
						}	
						
						addr=addr+24;		//ÿ����Ȩ24���ֽ�
				}
				if(i<=max_row)
				{
						for(i=0;i<12;i++)		//��һ��Ȩ�������ֽڶ���Ϊ0
						{
								for(k =0;k<WRITE_ZHI;k++)
									msDelay(500);
								WritEDAta(addr+i,0);		
								Clear_WDT();
						}
						
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);		
						Clear_WDT();
						total_row= ReadData(AUTHOR_ROW_L);   		
						total_row=total_row-1;
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);
						WritEDAta(AUTHOR_ROW_L,total_row);			
	
						mid_buff[6]= 0x01;
						xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
						mid_buff[7]=xor;
						Clear_WDT();
						UARTnSendChar(1,mid_buff,8);	 //���سɹ���ȷ����				
				} 
				else		//û�鵽��Ӧ�������
				{

						mid_buff[6]= 0x00;
						xor=0;
						xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
						mid_buff[7]=xor;
						Clear_WDT();
						UARTnSendChar(1,mid_buff,8);	 //����ʧ�ܵ�ȷ����		
					
				}
		}
		else
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();

			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
		}
}


/*
0x0a��λ��������λ��Ѳ������
2�ֽ������+2�ֽ�Ѳ��������+14���ֽھ������� +1�ֽ�Ѳ����
*/
void download_xunjian(void)
{
		uchar data_len,i,high_addr,low_addr,k,total_row,Len;
		uchar xor=0;
		uchar mid_buff[8];
		uint addr;
		data_len=g_Buff[3];		//g_Buff��ȫ�ֱ���

		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0a;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(Len>8&&xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
		{
			addr= ReadData(XUNJIAN_POINT_H);   
			addr =((addr<<8)&0xff00)+ReadData(XUNJIAN_POINT_L);
			Clear_WDT();
			msDelay(500);
			for(i=0;i<data_len-2;i++)		//��ô�������ľ���ô��,ע���ǰ����ݳ������ŵ�
			{
					for(k =0;k<WRITE_ZHI;k++)
						msDelay(500);
					WritEDAta(addr+i,g_Buff[i+6]);		
					Clear_WDT();
			}
			
			high_addr=((addr+19)&0xff00)>>8;   //ȡ��8λ���ٷŵ�
			low_addr =(addr+19)&0x00ff;				 //ÿһ��Ѳ������19���ֽ�
			
			//����Ѳ��С����ָ���λ��
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
			WritEDAta(XUNJIAN_POINT_H,high_addr);		//#define AUTHOR_START  0x0b4f 
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(XUNJIAN_POINT_L,low_addr);		
			
		
			//Ҫ���ֺ��ַ���ŵ����ݵ����Ǳ�ʾ��ַ��������
			//����Ѳ��С�����������ֻ��53��������ֻ���������ĵ�8λ
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();			
			total_row= ReadData(XUNJIAN_ROW_L) ;   		
			total_row=total_row+1;		//Ѳ��С����������һ
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(XUNJIAN_ROW_L,total_row);				
			
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
	
			mid_buff[6]= 0x01;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ȷ����
		}
		else
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();

			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
		}
	
}


/*

0xob�Զ��ش�Ѳ��������
2�ֽ������+2�ֽ�Ѳ��������+0x22(����������ʼ��)+Ѳ���� +0x23(����������ֹ��) 

*/
void auto_back_xunjian(void)
{
		uchar i,j,k,m,total_row,nChar,ID,Len;
		uchar xor=0;
		uchar mid_buff[12];
		uint addr,start_xunjian;
	
		ID=g_Buff[5];
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0b;

		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(Len==7&&xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
		{
// 				addr= ReadData(XUNJIAN_POINT_H);   
// 				addr =((addr<<8)&0xff00)+ReadData(XUNJIAN_POINT_L);
// 				Clear_WDT();
// 				msDelay(500);
			
				addr= ReadData(XUNJIAN_POINT_H);   
				addr =((addr<<8)&0xff00)+ReadData(XUNJIAN_POINT_L);
				total_row=(addr-XUNJIAN_START)/19;			
			
				//UARTnSendChar(1,&total_row,1);
			
				//total_row= ReadData(XUNJIAN_ROW_L);   
				Clear_WDT();
				msDelay(500);	
			
				for(i=0;i<total_row;i++)		//
				{		
					start_xunjian=XUNJIAN_START+i*19;
					
					for(j=0;j<4;j++)
					{
						msDelay(500);
						mid_buff[6+j]=ReadData(start_xunjian+j);		
						Clear_WDT();				
					}
					
					msDelay(500);
					mid_buff[6+j]=ReadData(start_xunjian+18);		//ȡѲ��С����ĩβ��Ѳ����
					
					xor=0;
					for(m =0;m<11;m++)
						xor =xor^mid_buff[m];	
					mid_buff[11]=xor;

					msDelay(500);
					Clear_WDT();
					UARTnSendChar(1,mid_buff,12);	 //����Ѳ����				
					
					Clear_WDT();
					UARTReset(0);		//��λ���ڽ��ܻ�����				 
					while(1)  //������һ����Ȩ֮�����ȴ���λ���ķ�������
					{
						
						for(k =0;k<WRITE_ZHI;k++)
								msDelay(500);
						Clear_WDT();
						nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  
						
						if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0b&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xD7)//�������0x01�ͷ���һ����Ȩ
								break;		//ֻ�������ڲ��ȴ�ѭ��
						else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0b&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x00&&g_Buff[7]==0xD6)
						{
								i--;		//�൱���ط�
								break;
						}
			
					}
				}
	  }
		else
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
			
			mid_buff[3] =0x03;
			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ʧ����		
		}
} 

/*
oxoc���������Ȩ

*/
void clear_all_author(void)
{
		uchar i,k,Len;
	  uchar xor=0;
		uchar mid_buff[8];
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0c;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];	
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	
	  if(xor==g_Buff[Len-1])		//�жϽ�������У���Ƿ���ȷ
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
			WritEDAta(AUTHOR_ROW_H,0x00);		 //��Ȩ��Ϣ����Ϊ0
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(AUTHOR_ROW_L,0x00);			
		
		
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
			WritEDAta(AUTHOR_POINT_H,0x04);		//#define AUTHOR_START  0x0400
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(AUTHOR_POINT_L,0x00);				
		
		
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();

			mid_buff[6]= 0x01;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //���سɹ���	

		}
		else
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();

			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			Clear_WDT();
			UARTnSendChar(1,mid_buff,8);	 //����ʧ����	
		}


}




void pc_to_slave(void)
{
		uchar i,xor,Len;
		uchar mid_buff[8];
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0d;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		

		Len=g_Buff[3]+5;
		
		xor=0;
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
		
		if(xor==g_Buff[6])
		{
			mid_buff[6]= 0x01;	
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //����ȷ����	
		}
	  else
		{
			mid_buff[6]= 0x00;	
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //����ʧ����				
		}

}


void pc_end_com(void)
{
		uchar i,xor,Len;
		uchar mid_buff[8];
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0e;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		

		Len=g_Buff[3]+5;
		
		xor=0;
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
		
		if(xor==g_Buff[6])
		{
			
			end_flag=1;		//ͨ�Ž�����ֱ���˳��ȴ�����״̬
			
			mid_buff[6]= 0x01;	//��ʾ�ɹ�
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //����ȷ����	
			USB_Cheak_init_flag=0;		//��ʾ�ǳ�ʼ������״̬
		}
	  else
		{
			mid_buff[6]= 0x00;	//��ʾʧ��
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //����ʧ����				
		}
		
}



/*���ʹ���������EEPROM*/
void reset_eeprom(void)
{
		uchar i,xor,Len;
		uchar mid_buff[8];
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0f;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		

		Len=g_Buff[3]+5;
		
		xor=0;
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
		
		if(xor==g_Buff[6])
		{
			
			mykey_init();
			
			mid_buff[6]= 0x01;	//��ʾ�ɹ�
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //����ȷ����	
			USB_Cheak_init_flag=0;		//��ʾ�ǳ�ʼ������״̬
		}
	  else
		{
			mid_buff[6]= 0x00;	//��ʾʧ��
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //����ʧ����				
		}		



}




/*
����״̬����
����״̬
*/
uchar key_state(void)
{
		return 1;

}




/***
�ϴ�����Ա����Ϣ��������¼����̨
***/
void up_loadInfo(void)
{
	uchar ack[7] ={0x55,0x7A,0x06,0x00,0x00,0x00,0x29};
	uchar nChar ;
  //ReadData��ͨ��SPI��eeprom�ĺ������Ż����ڵ�ַ������
	nChar =ReadData(TASK_BIT);//TASK_BIT�����־λ����Ϊ '1' ʱ��ʾ��һ������δ��ɣ���Ϊ '0' ʱ��־�����ѽ���
	if(nChar ==1)
	{
		Send_Opretar();
		USB_info3();
	}
	else if(nChar ==0)
	{
	  UARTnSendChar(1,ack,7);	 //����ȷ����
	}
	return;
}

/***
��������Ա����Ϣ���͸���̨
�뿪���ļ�¼�󶨷���
****/
void Send_Opretar(void)
{
	  uchar mid_Buff[6];
	  uchar i,Len,xor =0;
	  Len =ReadData(Operate_name_Len);
	  for(i =0;i<Len;i++)
			g_Buff[i] =ReadData(HANZI_FIRST_START+i);		//#define   HANZI_FIRST_START    Operate_name_Start
	  mid_Buff[0] =0x55;
	  mid_Buff[1] =0x7A;
	  mid_Buff[2] =0x01;
	  mid_Buff[3] =0x00;
	  mid_Buff[4] =Len;
	  mid_Buff[5] =0x00;
	  xor =mid_Buff[0]^mid_Buff[1]^mid_Buff[2]^mid_Buff[3]^mid_Buff[4]^mid_Buff[5];
	  for(i =0;i<Len;i++)
			xor =xor^g_Buff[i];
	
		//��������mid_Buff
	  for(i =0;i<6;i++)
			UARTSendChar(1,mid_Buff[i]);
	  for(i =0;i<Len;i++)
			UARTSendChar(1,g_Buff[i]);
	  UARTSendChar(1,xor);
	  return;
}

/****
�趨������Ա�ĳ����Ϊ16������
�人�ݺϿƼ���˾
֣�ݰ��´��з���
һ�����ֵ���ģ����Ϊ32���ֽ�
�������ݰ�����Ϊ32*16 =512���ֽ�
��Ϊ��������֡��ִ��
1.һ��������Ҫ��ʾ�ĺ��ֵ�����
 ���ͻ�Ӧ֡����̨
2.һ��������Ҫ��ʾ�ĺ��ֵ���ģ
���ͻ�Ӧ֡����̨
HANZI_FIRST_START
HANZI_SECOND_START
g_Buff[3] =len_high
g_Buff[4] =len_Low

��д��Ĳ���Ա���߹���λ����Ϣ��LCD���Ͻ�����ʾ
�ڸ��Ĳ���Ա������£���ͷָ�����Ϣ����һ�θ�λ�������ǰ�ļ�¼
�������µĲ���Ա��Ϣ������£��Ž�����ı�־λ�� 0
*****/
// void  USB_info10(void)
// {
// 	uchar ack[7] ={0x55,0x7A,0x01,0x00,0x00,0x00,0x2e};
//   uint hanzi_Len,i,old_Len,j;
//   
//   uchar head[3];
// 	hanzi_Len =g_Buff[4]-1;  //������ID
// 	for( i =0;i<hanzi_Len;i++)
// 	{
// 		 for(j =0;j<40;j++)
// 			 msDelay(500);
// 		 WritEDAta(HANZI_FIRST_START+i,g_Buff[7+i]);
// 	}
//   for(j =0;j<40;j++)
//   msDelay(500);
//   WritEDAta(Operate_name_Len,hanzi_Len);    //�洢����ĳ���
//   UARTnSendChar(1,ack,7);	 //����ȷ����
//   UARTReset(0); 
//   Clear_WDT();            //��λ���ջ�����


// 	KillTimer(TIMER_TIMEOUT);
// 	g_WaitTimeOut = 0;
// 	SetTimer(TIMER_TIMEOUT,20,Do_TimeOut,THREE_MIN);
// 	WaitUartChar(0xaa);
// 	WaitUartChar(0x75);
// 	WaitUartChar(0x01);	  //����֡
// 	WaitUartCharCount0(4);
// 	KillTimer(TIMER_TIMEOUT);
// 	if(g_WaitTimeOut == 1)
// 	{
// 		Clear_WDT();
// 		return;
// 	}
// 	UARTnReadChar(1,head,4);
// 	hanzi_Len =(head[0]&0x00ff)<<8;
// 	hanzi_Len =hanzi_Len +(head[1]&(0x00ff));
// 	old_Len=hanzi_Len;
// 	i =0;
// 	Clear_WDT();
// 	g_WaitTimeOut = 0;
// 	SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
// 	while(( hanzi_Len !=0)&& (g_WaitTimeOut == 0)) //�Ƿ���Ҫ��ʱ��������
// 	{
// 		Clear_WDT();
// 		if(UARTGetCount(0)>=1)
// 		{
// 			g_Buff[i] =UARTReadChar(1);
// 			i++;
// 			hanzi_Len--;
// 		}   

// 	}
// 	KillTimer(TIMER_TIMEOUT);
// 	old_Len = old_Len-1;
// 	for(j =0;j<40;j++)
// 		msDelay(500);
// 	WritEDAta(Operate_danwei_Len,i);
// 	for( i =0;i<old_Len;i++)
// 	{
// 		for(j =0;j<40;j++)
// 			msDelay(500);
// 		WritEDAta(HANZI_SECOND_START+i,g_Buff[i]); //�洢���ֵ���ģ
// 	}
// 	old_Len =old_Len/32;
// 	OLED_Clear();
// 	Show_Operater(0,0,g_Buff,old_Len,0,0x0fff);
// 	for(j =0;j<40;j++)
// 		msDelay(500);
// 	WritEDAta(TASK_BIT ,1);
// 	Init_KeyRecord();   //��λ���ӱ��ͷ�洢ָ��
// 	UARTnSendChar(1,ack,7);	 //����ȷ����
// 	return;

// }
//����Կ���е���λ�����Ϣ
// void USB_info11(void)
// {
//   uchar time_date[6];
//   uchar mid_buff[13];
//   uchar i,k;
//   uchar xor =0;
//   mid_buff[0] =0x55;
//   mid_buff[1] =0x7a;
//   mid_buff[2] =0x0E;
//   mid_buff[3] =0x00;
//   mid_buff[4] =0x06;
//   mid_buff[5] =0x00;
//   {
// //	g_Buff[6]= 0x00;
// //	g_Buff[7]= 0x00;
// //	g_Buff[8]= 0x00;
// //	g_Buff[9]= 0x01;
// //	g_Buff[10]= 0x00;
// //	g_Buff[11]= 0x01;

//   }
//   Clear_WDT();
//   for(i =0;i<3;i++)
// 		time_date[i] =g_Buff[7+i];
//   for(i =0;i<3;i++)
//   {
//   	for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
// 		WritEDAta(LOCAL_NUM+i,time_date[i]);
//   }
//   Clear_WDT();
//   msDelay(500);
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_CODE,g_Buff[10]);
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_NUM,g_Buff[12]);
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_NUM+1,g_Buff[13]);
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//   {
// 		for(i =0;i<3;i++)
// 			mid_buff[6+i] =ReadData(LOCAL_NUM+i);
// 		mid_buff[9] =ReadData(KEY_CODE);
// 		mid_buff[10] =ReadData(KEY_NUM);
// 		mid_buff[11] =ReadData(KEY_NUM+1);
// 	// for( i =0;i<6;i++)
//   // mid_buff[6+i] =time_buff[i];   //ʱ��ȥ��
// 		for(i =0;i<12;i++)
// 			xor =xor^mid_buff[i];
// 		mid_buff[12]=xor;
// 		Clear_WDT();
// 		UARTnSendChar(1,mid_buff,13);	 //����ȷ����

//   }
//   Clear_WDT();
//   return;
// }

/***
������ݴ洢��������Ϣ
****/
// void USB_info6(void)
// {
// //  uchar time_date[6];
// //  uchar i;
//   uchar ack[7] ={0x55,0x7A,0x0A,0x00,0x00,0x00,0x25};
//   Clear_WDT();
//   Init_KeyRecord();
//   msDelay(500);
//   Clear_WDT();
//   UARTnSendChar(1,ack,7);	 //����ȷ����
//   return;

// }
/********************
 Կ�׷������š��豸���롢Կ�ױ�š�ʱ��͵�ص�������̨�ж��Ƿ���Ч
 ���ţ�03 71
 �豸���룺34 56 78
 Կ�ױ�ţ�01
 ʱ�䣺�� �� ʱ ��
 ��ص�����4.1V
 55 7a 01  00  0b +data+cs 0d
 aa 75 0a  00  00 cs 0d  --ȷ����
 ��λ����̨Ӧ����Ϊ�����豸��
 ��λ����Ϊ�����豸	 ok
*********************/
void USB_info1(void)
{
  uchar i;
  uchar mid_buff[21];
  uchar xor =0;       //Ч���
  Clear_WDT();
  for(i =0;i<10;i++)
   msDelay(500);
  Clear_WDT();
  mid_buff[0] =0x55;
  mid_buff[1] =0x7a;
  mid_buff[2] =0x04;		//��ʾ������
  mid_buff[3] =0x00;
  mid_buff[4] =0x0c;		//��ʾ���ݳ��ȣ��ӵ�6λ��17λ��������У��λ
  mid_buff[5] =0x00;
  for(i =0;i<3;i++)		//#define   LOCAL_NUM	  0X01	 /*01 02 03 04*��λ��*/
		mid_buff[6+i] =ReadData(LOCAL_NUM+i); //mid_buff 678  ����
  mid_buff[9] =ReadData(KEY_CODE);		//�豸����  #define   KEY_CODE		   0X05	 /*05*/
  for(i =0;i<2;i++)
		mid_buff[10+i] =ReadData(KEY_NUM+i);		//Կ�ױ��  #define   KEY_NUM		   0X06	 /* 06 07 08*/
  Clear_WDT();
  read_time();
  for( i =0;i<6;i++)
		mid_buff[12+i] =time_buff[i];		//mid_buff 12~17  ʱ�䣺�� �� ʱ ��
  for(i =0;i<18;i++)
		xor =xor^mid_buff[i];
  mid_buff[18] =xor;		//У���
  Clear_WDT();
  UARTnSendChar(1,mid_buff,19);
  Clear_WDT();
  return;

}
/****
 ����RTCʱ�� ok
 �ظ�ʵʱʱ�Ӹ���̨������
 ����һλID
*****/
void USB_info2(void)
{
  uchar time_date[6];
  uchar mid_buff[13];
  uchar i;
  uchar xor =0;		//У���
  Clear_WDT();
  mid_buff[0] =0x55;		//��ʾ��λ��������λ��
  mid_buff[1] =0x7a;
  mid_buff[2] =0x04;		//������
  mid_buff[3] =0x07;		//����
  mid_buff[4] =0x00;
  mid_buff[5] =0x01;		//ID
  Clear_WDT();
	
  for(i =0;i<6;i++)
		time_date[i] =g_Buff[7+i];		//g_Buff[7~12] 6�����鵥Ԫ
  set_time(time_date[0],time_date[1],time_date[2],time_date[3],time_date[4],time_date[5]);//������λ����ʱ������Կ�׵�ʱ��
  msDelay(500);
  Clear_WDT();
  read_time();    //����ʵʱ��ʱ��,����time_buff��
  for( i =0;i<6;i++)
		mid_buff[6+i] =time_buff[i];
  for(i =0;i<12;i++)
		xor =xor^mid_buff[i];
  mid_buff[12]=xor;
  Clear_WDT();
  UARTnSendChar(1,mid_buff,13);	 //����ȷ���룻����ʵʱʱ�䣨д�뱸�����ݼĴ�����ʱ�䣩
  Clear_WDT();
  return;

}











/*
ok
���̨����洢�Ŀ�����¼�����
*/
//���Կ�����¼�Ƿ�����ĺ���
void Test_USB_Record(void)
{
  Clear_WDT();
  g_Buff[0]= ReadData(KEY_RECORD_ROW_H);   //�Ѵ洢������
  g_Buff[1] =ReadData(KEY_RECORD_ROW_L);
  g_Buff[2] = ReadData(KEY_RECORD_INDEX_H);   //�Ѵ洢������
  g_Buff[3] =ReadData(KEY_RECORD_INDEX_L);
  Clear_WDT();
  UARTnSendChar(1,g_Buff,4);
  Clear_WDT();
  //while(1);
}
/***
 �ǲ���û�еȵ���Ӧ��ȷ��֡��
****/
void USB_info3(void)
{
  uint  row,i,head,total_data;
  uchar mid_buff[9],each_data,j;
  uchar nChar,zhen,left,code;
  uchar xor =0;
  Clear_WDT();
  code = ReadData(KEY_CODE);
  row= ReadData(KEY_RECORD_ROW_H);   //�Ѵ洢������
  row =((row<<8)&0xff00)+ReadData(KEY_RECORD_ROW_L);
  head= ReadData(KEY_RECORD_INDEX_H);   //�Ѵ洢������
  head =((head<<8)&0xff00)+ReadData(KEY_RECORD_INDEX_L);
 // Test_USB_Record();
 // row =2000;  //����
  total_data =row*10;   //���е��������Ĵ�С
  each_data =200;       //20������
  Clear_WDT();
  if(row <2000)
  {
		head =KEY_RECORD_START; //δ���������
  }
  else if(row >=2000)
  {
		head =head;	    //�������ݵ����

  }
   zhen =row/20;
   left =row%20;
   if(left ==0)	  //����Ϊ0
   {
			zhen =zhen;

   }
   else if(left !=0)   //������Ϊ0
   {
			zhen =zhen+1;

   }
   Clear_WDT();
  //head =0x7e2;
  //row =10;   //����
//  length =row*10;
  mid_buff[0] =0x55;
  mid_buff[1] =0x7a;
  mid_buff[2] =0x06;
  mid_buff[3] =0X00;		//��λ��ַ
  mid_buff[4] =0X02;		//��λ��ַ
  mid_buff[5] =0x00;
  mid_buff[6] =code;
  mid_buff[7] =zhen;
  for(i =0;i<8;i++)
		xor =xor^mid_buff[i];
  mid_buff[8] =xor;
	 
  UARTnSendChar(1,mid_buff,9);
  Clear_WDT();
  KillTimer(TIMER_TIMEOUT);
  g_WaitTimeOut = 0;
  SetTimer(TIMER_TIMEOUT,20,Do_TimeOut,THREE_MIN);
  while( g_WaitTimeOut ==0)		 //�ȴ���λ������ȷ��֡
  {
      Clear_WDT();
			nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);
			if(nChar >=3)
			break;

  }
  KillTimer(TIMER_TIMEOUT);
  Clear_WDT();
  if(g_WaitTimeOut ==1)
  {
     Clear_WDT();
	   return;
  }

  for(i =0;i<zhen;i++) //�������ݰ�����̨---���Ĳ�����¼
  {
    Clear_WDT();
    if(total_data >=each_data)
	  {
	   	total_data =total_data-each_data;
	  }
	  else 
	  {
	    each_data =total_data;
	  }
	  g_Buff[0] =0x55;
		g_Buff[1] =0x7a;
		g_Buff[2] =0x06;
		g_Buff[3] =0X00;
		g_Buff[4] =each_data;
		g_Buff[5] =0x00;
		Clear_WDT();
	  for(j =0;j<6;j++)
			xor=xor^g_Buff[i];
	  for(j =0;j<each_data;j++)
	  {
	     Clear_WDT();
			 g_Buff[6+j] =ReadData(head);
			 head++;
			 if(head >=KEY_RECORD_END)
			 head =KEY_RECORD_START;
			 xor =xor^g_Buff[6+j];
	  }
	  UARTnSendChar(1,g_Buff,each_data+6);
	  UARTSendChar(1,xor);
	  KillTimer(TIMER_TIMEOUT);
		g_WaitTimeOut = 0;
		SetTimer(TIMER_TIMEOUT,20,Do_TimeOut,THREE_MIN);
		while( g_WaitTimeOut ==0)		 //�ȴ���λ������ȷ��֡
		{
			 Clear_WDT();
			 nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);
			 if(nChar >=3)
			 break;

		}
		KillTimer(TIMER_TIMEOUT);
		if(g_WaitTimeOut ==1)
		{
			 Clear_WDT();
			 return;
		}


  }
}
/***

����1000������Ϣ�Ƿֶ����õ�
1.���ݵ����ݰ���
2.ÿ��50�û����м�ֵ ���ݹ���
 ��������Ļ���Ӧ�ô���21������

****/

void Read_info4(void)
{
  uchar nChar;
  uint k;
  	/****/
		 for(k =0;k<4000;k++)
	   {
	      Clear_WDT();
	     nChar =ReadData(KEY_NAME_START+k);	  //���Զ�д�����Ƿ�����--����
 	     UARTSendChar(1,nChar);
	    }
		while(1);
		/*****/


}
/***
��ģ������

***/
uchar Mode(uint K)

{

    return K%M;		//#define   M   173

}

/***
����ɢ�б�Ĵ洢ģʽ
4���ֽڵ��û��ţ��������ֽڵĵ�ַ--��ַΪ��һ��ͬģ���û��ŵĴ洢λ��

���1���Ѿ�������1000��
���2��δ�洢��1000��

***/
void New_Save(uint row_len)
{
 // uchar Ti[246];
  uchar mid_name[4];  //�û��ŵ��м��ݴ�
  uchar i,k,nChar,new_row;
  uint j;
  uint last_addr;
  uint add =0;
  Clear_WDT();
  last_addr= ReadData(KEY_YONGHU_INDEX_H);   //�Ѵ洢������
  last_addr =((last_addr<<8)&0xff00)+ReadData(KEY_YONGHU_INDEX_L);
  for(j =0;j<246;j++)
		Ti[j] =  ReadData(SANBIAO_START+j);
  row_len =row_len/4;
  Clear_WDT();
	
  for(i =0;i<row_len;i++)
  {
    Clear_WDT();
		add =0;
		for(k =0;k<4;k++)
		{
		 nChar =g_Buff[4*i+k];
		 add =add+nChar;
		 mid_name[k] = nChar;  
		}
		HashInsert(Ti,add ,mid_name,last_addr);
		last_addr =last_addr+6;  //ÿһ���¼ռ��6���ֽ� 4����+2�����ӵ�ַ
  }
	
  Clear_WDT();
  new_row =	(last_addr&0xff00)>>8;
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(KEY_YONGHU_INDEX_H,new_row);
  new_row =(last_addr&0x00ff);
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(KEY_YONGHU_INDEX_L,new_row);
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  Clear_WDT();
  for(j =0;j<246;j++)
  {
    Clear_WDT();
    for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
	  WritEDAta(SANBIAO_START+j,Ti[j]);		//�в���
  }
	for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
	Clear_WDT();
}
/****
 ��ʼ����ʱ�򣬽�ĩ��ַ����Ϊ0XFFFF
*****/
void HashInsert(uchar t[],uint nChar,uchar nm[],uint addr)
{
   uchar i,subaddr_h,subaddr_l,old_addr_h,old_addr_l;
   uchar k;
   subaddr_h =  (addr&0x00ff);
   subaddr_l =  (addr&0xff00)>>8;
   Clear_WDT();
   i =Mode(nChar);		//#define   M   173;��137��ģ
   i=2*i;
   Clear_WDT();
   if((t[i] ==0xff)&&(t[i+1] ==0xff))
   {
			t[i] =subaddr_h;
			t[i+1] =subaddr_l;		//���Ϊ�׵�ַ������������
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(addr+4,0xff);
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(addr+5,0xff);

   }
   else if((t[i] !=0xff)||(t[i+1]!=0xff))
   {
			old_addr_h =t[i];
			old_addr_l =t[i+1];
			t[i] =	subaddr_h;
			t[i+1] =subaddr_l;
			for(k =0;k<WRITE_ZHI;k++)
			 msDelay(500);
			WritEDAta(addr+4,old_addr_h);
			for(k =0;k<WRITE_ZHI;k++)
			 msDelay(500);
			WritEDAta (addr+5,old_addr_l);
   }
   Clear_WDT();
   for(i =0;i<4;i++)
   {
    for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
		WritEDAta(addr+i,nm[i]);
   }
	 Clear_WDT();
	 for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
	 Clear_WDT();

}

/****
����ʣ����û��ռ�

****/
void Ask_LeftyhNum(void)
{


}

/****
��Կ���������ؿɿ���������Ϣ

delete_row  --�Ѿ���ɾ��������
****/
// void USB_info4(void)
// {
//   uchar row,j,k,delete_row,l,delete_row_tow;
//   uint i;
//   uint length,old_length,row_length,total_row;
//   uchar head[4];
//   uchar nChar;
//   uchar left_buff[200];
// //  uint  key_name;
//   uchar xor =0;
//   uchar ack[7] ={0x55,0x7A,0x0A,0x00,0x00,0x00,0x25};
//   Clear_WDT();
// //  key_name =  KEY_NAME_START;
// //  name_first =KEY_NAME_ZERO;
//   total_row= ReadData(KEY_YONGHU_ROW_H);   //�Ѵ洢������
//   total_row =((total_row<<8)&0xff00)+ReadData(KEY_YONGHU_ROW_L);
//   delete_row =ReadData(DETELE_ROW);		//delete_row  --�Ѿ���ɾ��������
//   delete_row_tow =delete_row;
//   total_row =1000-total_row; //����ʣ��Ŀռ�
//   for(i =0;i<246;i++)
// 		Ti[i] =ReadData(SANBIAO_START+i);	//������ʱ�򽫣����ݻ�������������
//   Clear_WDT();
//   if(total_row ==0x00)	 //����Ѿ��洢��1000����ͬʱû��ɾ�����û������
//   {
//      Clear_WDT();
//      if(delete_row ==0)		
//      return ;      //���ȫ������������£����ٴ洢

//   }
//  // for(j =0;j<53;j++)
//  // g_Buff[j] =test_row[j];
// // code =g_Buff[7];               //������
//   row =g_Buff[6];	        //����֡����Ŀ
//  //old_row =g_Buff[8];       	//�м�ֵ����Ŀ
// // old_row_tow =old_row*4;        //��������ݵĳ���
//   Clear_WDT();
//    //Read_Text();
//    // for(j =0;j<246;j++)
//  // {
//    // nChar =  ReadData(SANBIAO_START+j);
//    // UARTSendChar(1,nChar);
//   //}
//    /**
//   if(old_row <21)
//   {							//��ȡ������֡���м�ֵ���洢���� ����Ϊ������ʹ��
// 	for(j =0;j<old_row_tow;j++)
// 	{
// 	   for(k =0;k<WRITE_ZHI;k++)
// 	   msDelay(500);
//       WritEDAta(name_first,g_Buff[9+j]);
// 	  name_first++;

// 	}
// 	max_name[0] =g_Buff[9+old_row_tow-4];
// 	max_name[1] =g_Buff[9+old_row_tow-3];
// 	max_name[2] =g_Buff[9+old_row_tow-2];
// 	max_name[3] =g_Buff[9+old_row_tow-1];
// 	old_row =21-old_row;
// 	for(j =0;j<old_row;j++)
// 	{
// 	   name_second =name_first+4*j;	//������
// 	   	   for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 	   WritEDAta(name_second+0,max_name[0]);
// 	   	   for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 	   WritEDAta(name_second+1,max_name[1]);
// 	   	   for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 	   WritEDAta(name_second+2,max_name[2]);
// 	   	   for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 	   WritEDAta(name_second+3,max_name[3]);
//     }
// 	   for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 	   for(j =0;j<4;j++)
// 	    max_name[j] =ReadData(KEY_NAME_ZERO+j);	   //��С�������
// 		for(j =0;j<4;j++)
// 		{
// 		  for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 		 WritEDAta(KEY_NAME_MIN+j,max_name[j]);
// 		 }
// 		 for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 	    for(j =0;j<4;j++)
// 	     max_name[j] =ReadData(KEY_NAME_TEN_TOW+j);	  
// 		for(j =0;j<4;j++)
// 		{
// 		  for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 		 WritEDAta(KEY_NAME_MAX+j,max_name[j]);     //���������
// 		 }

//   }
//   else if( old_row >=21)
//   {
// 		for(j =0;j<old_row_tow;j++)
// 	    {
// 		   for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
//           WritEDAta(name_first,g_Buff[9+j]);
// 	      name_first++;
// 	    }
// 		for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 	    for(j =0;j<4;j++)
// 	    max_name[j] =ReadData(KEY_NAME_ZERO+j);
// 		for(j =0;j<4;j++)
// 		{
// 		  for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 		 WritEDAta(KEY_NAME_MIN+j,max_name[j]);
// 		 }
// 		 for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 	    for(j =0;j<4;j++)
// 	    max_name[j] =ReadData(KEY_NAME_TEN_TOW+j);
// 		for(j =0;j<4;j++)
// 		{
// 		  for(k =0;k<WRITE_ZHI;k++)
// 	       msDelay(500);
// 		 WritEDAta(KEY_NAME_MAX+j,max_name[j]);
// 		}
// 		/****
// 		 for(j =0;j<44;j++)
// 	   {
// 	     nChar =ReadData(KEY_NAME_ZERO+j);	  //���Զ�д�����Ƿ�����--����
//  	     UARTSendChar(1,nChar);
// 	   }
// 		
//   }	 
//   *****/
//   UARTnSendChar(1,ack,7);  //��ȷ��֡����̨
//   UARTReset(0); 
//   Clear_WDT();            //��λ���ջ�����
// 	row =1;
//   for(j =0;j<row;j++)
//   {
// 		Clear_WDT();
// 		delete_row =ReadData(DETELE_ROW);  //ÿ֡�����ɾ��������
// 		KillTimer(TIMER_TIMEOUT);
// 		g_WaitTimeOut = 0;
// 		SetTimer(TIMER_TIMEOUT,20,Do_TimeOut,THREE_MIN);
// 		WaitUartChar(0xaa);
// 		WaitUartChar(0x75);
// 		WaitUartChar(0x09);	  //����֡
// 		WaitUartCharCount0(4);
// 		KillTimer(TIMER_TIMEOUT);
// 		if(g_WaitTimeOut == 1)
// 		{
// 		  Clear_WDT();
// 		  return;
// 		}
// 		UARTnReadChar(1,head,4);
// 		length =(head[0]&0x00ff)<<8;
// 		length =length +(head[1]&(0x00ff));
// 		old_length =length;
// 		i =0;
// 		Clear_WDT();
// 		g_WaitTimeOut = 0;
//     SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
// 		while(( length !=0)&& (g_WaitTimeOut == 0)) //�Ƿ���Ҫ��ʱ��������
// 		{
// 		  Clear_WDT();
// 		  if(UARTGetCount(0)>=1)
// 		  {
// 		  	g_Buff[i] =UARTReadChar(1);
// 				i++;
// 				length--;
// 		  }   

// 		}
// 		KillTimer(TIMER_TIMEOUT);
// 		if( g_WaitTimeOut == 1)
// 		{
// 		    Clear_WDT();
// 		    return;
// 		}
// 		xor = 0xd6;
// 		for( i=0;i<4;i++)
// 			xor =xor^head[i];
// 		
// 		for(i =0;i<old_length;i++)   
// 		{
// 		  xor =xor^g_Buff[i];
// 		}
// 	//	if(xor ==0)	  //���Ч������������ȷ	 ---Ч��������	һ֡Ϊ50���û���
// 		{
// 		   old_length =old_length-1;
// 		   row_length =old_length/4; //��ⷢ�͵�����
// 		   Clear_WDT();
// 		   if(delete_row !=0)   //ɾ����������Ϊ��
// 		   {
// 			    if( row_length <=delete_row)
// 			    {
// 			     for( k =0;k<old_length;k++)
// 						left_buff[k] =g_Buff[k];   //�������Ƶ��м仺����
// 			     increase_LockNum(left_buff,old_length);
// 				   Clear_WDT();
// 			     UARTnSendChar(1,ack,7);  //��ȷ��֡����̨	 ---�ƺ�������,д�Ѵ洢���������Ƿ�������
// 		       UARTReset(0);
// 			     return; 
// 			    }
// 			    else if( row_length >delete_row)   //������???  
// 			    {
// 				     Clear_WDT();
// 			       delete_row =delete_row*4;
// 			       k =old_length-delete_row;
// 			       for(l =0;l<delete_row;l++)
// 							left_buff[l] =g_Buff[k+l];
// 			       increase_LockNum(left_buff,delete_row);
// 			       old_length = old_length -delete_row;
// 			       row_length =row_length-delete_row_tow;   //ʣ�������
// 			    }

// 		    }
// 		    
// 		    if( total_row >=row_length)
// 		    {
// 			     Clear_WDT();
// 		       total_row =total_row -row_length;
// 		       New_Save(old_length);   //�µĴ洢��ʽ
// 		       UARTnSendChar(1,ack,7);  //��ȷ��֡����̨
// 		       UARTReset(0);
// 		    }
// 		    else if( total_row <row_length)
//         {
// 				    Clear_WDT();
// 		        old_length =total_row;
// 		        New_Save(old_length);   //�µĴ洢��ʽ
// 		        {
// 							Clear_WDT();
// 							for(k =0;k<WRITE_ZHI;k++)
// 								msDelay(500);
// 							WritEDAta(KEY_YONGHU_ROW_H,0x03);
// 							for(k =0;k<WRITE_ZHI;k++)
// 								msDelay(500);
// 							WritEDAta(KEY_YONGHU_ROW_L,0xe8);
// 							for(k =0;k<WRITE_ZHI;k++)	//�ظ������Ѿ���Ա��ʾ����̨
// 								msDelay(500);
// 							Clear_WDT();		      

// 		        }
// 						UARTnSendChar(1,ack,7);  //��ȷ��֡����̨
// 						UARTReset(0);
// 						Clear_WDT();
// 						return;
//           }
// 		   //UARTnSendChar(1,ack,7);  //��ȷ��֡����̨
// 		    //UARTReset(0); 
// 		 
// 		}
// 		
// 		//Read_Text();
// 		/*
// 		else if(xor !=0)
// 		{
// 		  return;
// 		}
// 		*/

//   }
// 	Clear_WDT();
// 	total_row =1000-total_row;
// 	nChar =(total_row &0xff00)>>8;
// 	for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
// 	WritEDAta(KEY_YONGHU_ROW_H,nChar);
// 	nChar =	(total_row &0x00ff);
// 	for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
// 	WritEDAta(KEY_YONGHU_ROW_L,nChar);
// 	for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
// 	Clear_WDT();
// 	return;

// }
 /***


 ****/
// void  Read_Text(void)
// {
//   uchar j,nChar;
//   uint k;
// 	Clear_WDT();
// 	for(k =0;k<WRITE_ZHI;k++)
//   msDelay(500);
//   /**/
//   for(j =0;j<246;j++)
//   {
//       Clear_WDT();
// 			nChar =  ReadData(SANBIAO_START+j);
// 			UARTSendChar(1,nChar);
//   }
//   /**/
//   Clear_WDT();
//   for(k =0;k<6000;k++)
//   {
//      Clear_WDT();
// 		 nChar =  ReadData(KEY_YONGHU_START+k);
// 		 UARTSendChar(1,nChar);

//   }
//   while(1);
//   
// }
// void USB_info5(void)
// {
//   uchar ack[7] ={0xAA,0x75,0x0A,0x00,0x00,0x00,0xD5};
//   UARTnSendChar(1,ack,7);  //��ȷ��֡����̨

// }
/*
 ��ȡ��Э���ʽ���͹��������ݣ������н���

*/

uchar UARTReadFrm(char uart, uchar *pdst, uint max)
{
  uchar count = 0,total;
  uchar idx,off;
  Clear_WDT();
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		//�ؽ����жϣ��ڽ����ж�USART1_IRQHandler()���ϵĽ����ܵ����ݷ���m_UB0
  if (m_UB0.cRHead != m_UB0.cRTail)
   {
      //������Ч�����ܳ���;�õ�����ʱhead������
      if (m_UB0.cRHead >= m_UB0.cRTail)		//���ͷ����β
      {
        total  = m_UB0.cRHead - m_UB0.cRTail;
      } 
			else
			{
        total  = UARTRXLEN + m_UB0.cRHead - m_UB0.cRTail ;		//���β����ͷ
      }
      //�ҽ�����
      if (total>4)//֡��������Ϊ4
      {
          if (m_UB0.RBuff[m_UB0.cRTail]!=(uchar)0xaa)		//
          {
              m_UB0.cRTail = (m_UB0.cRTail+1)% UARTRXLEN;		//UARTRXLENΪ254,cRTail��1
          }
					else
					{   
              off = (m_UB0.cRTail + 1) % UARTRXLEN;		//cTail����һ��λ�õĵ�ַ
              if (m_UB0.RBuff[off]!=(uchar)0x75)
              {
                m_UB0.cRTail = (m_UB0.cRTail+2)% UARTRXLEN;		//�ٽ�cTail��1
              }
							else		//��ʾ���յ��ĵ�һ����aa���ڶ�����75
							{
                  off = (m_UB0.cRTail + 3) % UARTRXLEN;	//off��ʾ��3������cRTailû�б�
									count=m_UB0.RBuff[off]+5;		//5��ʾ��ͷ2���ַ���1�������֣�1�����ȣ�0x00
//                   count = m_UB0.RBuff[off]<<8;//���ݳ��ȸ��ֽ�		��ʾȡ���������ĸ�8λ
//                   off = (m_UB0.cRTail + 4) % UARTRXLEN;//off��ʾ��4����
//                   count = count + m_UB0.RBuff[off]+7;//���ݳ��ȵ��ֽڣ�������Ӻ�count��ʾ�������ݳ��ȣ�7��ʾ2����ͷ�ַ���1�������֣�2�����ȣ�0x00,����λ
                  if (count <= total && count < max)	//total�ǻ�������Ч���ݵĳ��ȣ�count����������֡�ĳ���,������Զ��count<total
                  {
                      //���ݳ�������һ֡��Ҫ��
                      //total = 0;//��¼У���,total����һ�ֺ���
                      for (idx=0; idx < max-1 && idx<count;idx++)
                      {
                        off = (m_UB0.cRTail + idx) % UARTRXLEN;		//cRTailҲû�б�
                        pdst[idx] = m_UB0.RBuff[off];		////�����ܻ������İ�����֡����pdst����
                        //total ^= pdst[idx];//����У���
                      }
//                      if (total==0)//У����ȷ���˴�Ϊ��У��
                          m_UB0.cRTail = (off + 1) % UARTRXLEN;		//У����ȷ�ͽ�cRTail������������֡����һ��λ��
//                       else
//                       {
//                           //У���������2���ֽڣ���ʵΪ��֡��ͷ
//                           m_UB0.cRTail = (m_UB0.cRTail + 2) % UARTRXLEN;
//                           count = 0;
//                       }
                   }
									 else count =0;
              }
          }
  	   }
	  }
	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	  Clear_WDT();
    return count;
}

//���ػ��������ֽ���
u16 UARTGetCount(char uart)
{
	u16 count = 0 ;
    {

		 USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);           // Disable USART0 RX interrupt
		if (m_UB0.cRHead >= m_UB0.cRTail)
		{
			count  = m_UB0.cRHead - m_UB0.cRTail;
		}else{
			count  = UARTRXLEN + m_UB0.cRHead - m_UB0.cRTail;
		}
	
		 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);              // Enable USART0 RX interrupt

	}
	return count;
}

void WaitUartCharCount0(u8 cnt)
{
    while (g_WaitTimeOut == 0)
    { 
	     Clear_WDT();
        if (UARTGetCount(0)>=cnt)   
        break;
    }    
}

/*
�ȴ���������ch
*/
void WaitUartChar(u8 ch)
{
    u8  nChar;
    while (g_WaitTimeOut == 0)
    {  
				Clear_WDT();
				nChar = UARTReadChar(0);
				if ( nChar==ch)
						break;
    }    
}
/****
�洢���Կ��Ƶ�Կ����Ϣ�ı�����ݰ�
*****/
uchar  Save_KeyNum(void)
{

  return 0;
}
/****
�ϴ�����Կ�׿�������ʷ��Ϣ����̨
****/
uchar  Send_KeyData(void)
{

  return 0;
}

/****************
 ͨ����̨������Կ�׽���Уʱ

***************/

uchar Cheak_KeyClock(void)
{
  return 0;
}

/***
 ɾ��һ�����ı��
***/

//ɾ�����ı��,�����ж��������Ƿ���ȷ
// void  USB_info12(void)
// {
//    
//    
// 		uchar ack[7] ={0xAA,0x75,0x0A,0x00,0x00,0x00,0xD5};
// 		Clear_WDT();
// 		Delete_LockNum(g_Buff);
// 		Clear_WDT();
// 		UARTnSendChar(1,ack,7);
// 		Clear_WDT();

// }
// //�˺�������ѯ���û�ʣ��Ĵ洢�ռ�
// void USB_info13(void)
// {
//    uchar i;
//    uchar delete_row,xor;
//    uint  total_row;
//    uchar  ack[7];  //�ظ�������֡
//    xor =0;
// 	 Clear_WDT();
//    delete_row = ReadData(DETELE_ROW);
//    total_row= ReadData(KEY_YONGHU_ROW_H);   //�Ѵ洢������
//    total_row =((total_row<<8)&0xff00)+ReadData(KEY_YONGHU_ROW_L);
//    total_row =(1000-total_row)+delete_row;  //ʣ��Ĵ洢�ռ�
//    ack[0] =0xaa;
//    ack[1] =0x75;
//    ack[2] =0x03;
//    ack[3] =0x00;
//    ack[4] =(total_row&0xff00)>>8;
//    ack[5] =(total_row&0x00ff);
//    for(i =0;i<6;i++)
// 		xor =xor^ack[i];
//    ack[6] =xor;
//    Clear_WDT();
//    UARTnSendChar(1,ack,7);
//    Clear_WDT();
// }
/**
������ɾ��50���û�����Ϣ
������
���ȼ��һ�£�ɾ�����û������Ƿ񳬹�50��
**/
// void  Delete_LockNum(uchar buff[])
// {
//   uchar i,k,mid,row_len,len,nChar,j;
//   uchar delete_row,left_row;
//   uint  detele_addr;
//   Clear_WDT();
//   len =g_Buff[4];   //�õ����ݰ��ĳ���
//   row_len =len/4;
//   delete_row = ReadData(DETELE_ROW);
//   left_row =50 -delete_row;   //ʣ��Ŀ�ɾ�����û���
//   if( left_row <=row_len)
// 		row_len =left_row;
//   detele_addr= ReadData(DETELE_INDEX_H);   //�Ѵ洢������
//   detele_addr =((detele_addr<<8)&0xff00)+ReadData(DETELE_INDEX_L);
//   Clear_WDT();
//   for(i =0;i<246;i++)
//    Ti[i] =ReadData(SANBIAO_START+i);	//������ʱ�򽫣����ݻ�������������
//   for( i =0;i<row_len;i++)
//   {
//     Clear_WDT();
//     for(k=0;k<4;k++)
// 			Lock_number[k] =g_Buff[7+4*i+k];   //���ݰ�����һ�����������͵��ֽ�
//     msDelay(500);
//     nChar =Delete_OneLockNum();
//     msDelay(500);
// 	  Clear_WDT();
//     if(nChar ==1)   //�洢ɾ�������ŵĵ�ַ
//     {
// 	  Clear_WDT();
//     for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
//     WritEDAta(detele_addr ,MI_High);
// 	detele_addr =detele_addr+1;
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	 Clear_WDT();
// 	WritEDAta(detele_addr ,MI_Low);
// 	detele_addr =detele_addr+1;
// 	delete_row =delete_row+1;
// 	for(j =0;j<30;j++)
// 	msDelay(500);
// 	 Clear_WDT();
// 	if((MI_High ==Ti[Ti_INDEX+1])&&(MI_Low ==Ti[Ti_INDEX]))
// 	 {
// 	  Ti[Ti_INDEX+1] =0XFF;
// 	  Ti[Ti_INDEX] =0XFF;
//      }
//     }
//   }
//   Clear_WDT();
//   for(i =0;i<246;i++)
//   {
//          Clear_WDT();
//      	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(SANBIAO_START+i,Ti[i]);   //�洢�ı��ɢ��ṹ

//   }
//      Clear_WDT();
//     mid =(detele_addr&0x00ff);
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(DETELE_INDEX_L,mid);
// 	mid =(detele_addr&0xff00)>>8;
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	 Clear_WDT();
// 	WritEDAta(DETELE_INDEX_H,mid);  //�洢ͷָ��
//     for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(DETELE_ROW,delete_row);  //�洢����
//     for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	 Clear_WDT();
// }
/***
  ���ȼ��˱���Ƿ����
  ɾ���û�֮�� ���û������Ӵ����ӱ����޳�
***/
// uchar Delete_OneLockNum(void)
// {
//    uint i2c_addr,last_i2c_addr;
//    uchar i,k,nChar;
//    uchar Mid_text[6];
//    uchar mid_addr_h,mid_addr_l;
//    Clear_WDT();
//    nChar =Cheak_New_LockNum();
//    if(nChar ==1)
//    {
//         Clear_WDT();
//         i2c_addr =MI_High;
// 	i2c_addr =((i2c_addr <<8)&0xff00) +MI_Low;
// 	last_i2c_addr =i2c_addr;
// 	for(i =0;i<6;i++)
//         Mid_text[i] = ReadData(i2c_addr+i);
// 	mid_addr_h =Mid_text[5];
// 	mid_addr_l =Mid_text[4];
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	 Clear_WDT();
// 	WritEDAta(i2c_addr+4,0xfe);
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(i2c_addr+5,0xfe);  //0xfeΪע����־λ

// 	i2c_addr =MI_High_Last;
// 	i2c_addr =((i2c_addr <<8)&0xff00) +MI_Low_Last;
// 	if( i2c_addr !=last_i2c_addr)
// 	{
// 		for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
// 		 Clear_WDT();
// 		WritEDAta(i2c_addr+4,mid_addr_l);
// 		for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
// 		WritEDAta(i2c_addr+5,mid_addr_h);
// 		for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
// 		 Clear_WDT();
// 	 }
// 	return 1;
//   }
//    Clear_WDT();
//   return 0;
// }
/****
  ����һ�����ı��
  �Ƿ������ı���Ƿ����ظ��洢�����
  ���û�м�⵽�����ı�ŵĻ� ����������
*****/

// uchar Increase_Onelocknum(void)
// {
//   uchar nChar;
//    Clear_WDT();
//   nChar =Cheak_New_LockNum();  //������ı���Ƿ��Ѿ����ڣ���������������
//   if(nChar ==0)
//   {
//     Clear_WDT();
//    return 1;
//   }
//    Clear_WDT();
//   return 0;
// }
// /***
// ��ν��¼�������ı�ţ�д�����ӱ���
// ���Ӻ�ɾ�����û����������Ϊ50��
// ***/
// void  increase_LockNum(uchar buff[],uchar de_len)
// {
//   uchar mid_buff[200],len,row_len,j;
//   uchar delete_row,now_addr;
//   uchar i,nChar,k;
//   uint i2c_addr;
//   uint  add =0;
//    Clear_WDT();
//   now_addr =0;   //ȡ�����ݵ�����ָ��
//   len =de_len;   //�õ����ݰ��ĳ���
//   row_len =len/4;
//   delete_row = ReadData(DETELE_ROW);
//   len =delete_row*2;
//    for(i =0;i<246;i++)
//    Ti[i] =ReadData(SANBIAO_START+i);	   //������ʱ�򽫣����ݻ�������������
//    for(i =0;i<len;i++)
//    mid_buff[i] =ReadData(DETELE_START+i);  //��ɾ�����б���Ϣ��������
//     Clear_WDT();
//   for(i =0;i<row_len;i++)
//   {
//      Clear_WDT();
//     for(k=0;k<4;k++)
//     Lock_number[k] =buff[4*i+k];
//     nChar =Increase_Onelocknum(); 
//     if(nChar ==1)
//     {
// 	   Clear_WDT();
//       i2c_addr =mid_buff[now_addr];
//       i2c_addr =((i2c_addr<<8)&0xff00);
//       i2c_addr =i2c_addr+mid_buff[now_addr+1];
//       MI_High =mid_buff[now_addr];
//       MI_Low =mid_buff[now_addr+1];
//       now_addr =now_addr+2;  
//       delete_row =delete_row -1;
//       for( j =0;j<4;j++)
//       {
// 	     Clear_WDT();
// 		for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
// 		WritEDAta(i2c_addr+j,Lock_number[j]);
//       }
// 	   Clear_WDT();
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	add =0;
//         for(k =0;k<4;k++)
// 	add =add+ Lock_number[k];
// 	k =Mode(add);
// 	k=k*2;
// 	 Clear_WDT();
// 	MI_High_Last =Ti[k+1];
// 	MI_Low_Last  =Ti[k];
// 	Ti[k+1] = ((i2c_addr&0xff00)>>8);//MI_High;
// 	Ti[k]   =  (i2c_addr &0x00ff);//MI_Low;
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	 Clear_WDT();
// 	WritEDAta(i2c_addr+4,MI_Low_Last);
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(i2c_addr+5,MI_High_Last);
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	 Clear_WDT();

//     }
//   }
//   for(i =0;i<246;i++)
//   {
//      Clear_WDT();
//      	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(SANBIAO_START+i,Ti[i]);   //�洢�ı��ɢ��ṹ

//   }
//    Clear_WDT();
//   len =delete_row*2;
//   for(i =0;i<len;i++)
//   {
//          Clear_WDT();
//        	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(DETELE_START+i,mid_buff[now_addr+i]);	     //��һ������ĵط���û�и���ɾ�����ӵĵ�ַָ��
//   }
//     Clear_WDT();
//         i2c_addr = DETELE_START+len;   //�õ����ڵ����ݵ�λ��ָ��
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(DETELE_ROW,delete_row);
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	 Clear_WDT();
// 	WritEDAta(DETELE_INDEX_H,((i2c_addr &0xff00)>>8));
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(DETELE_INDEX_L,(i2c_addr&0x00ff));
// 	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	 Clear_WDT();
//   

// }
/****
  ����֡
  ��һ֡���������ݳ��ȸ���̨
  �ڶ�֡�����;�������ݸ���̨
*****/

// void  Send_YongHu(void)
// {
//   uint j;
//   uint  key_name,total_row,left_row;
//   uchar delete_row,i,nChar;
//   uchar shuzu[6];
//   uchar ack[7];
//   uchar xor =0;   //У��λ
//   Clear_WDT();
//   key_name =  KEY_NAME_START;
//   total_row= ReadData(KEY_YONGHU_ROW_H);   //�Ѵ洢������
//   total_row =((total_row<<8)&0xff00)+ReadData(KEY_YONGHU_ROW_L);
//   //delete_row =ReadData(DETELE_ROW);
//  // total_row =total_row+delete_row;
//   left_row =total_row; //-delete_row;
// 	total_row =total_row*4;
// 	Clear_WDT();
// 	ack[0] =0x55;
// 	ack[1] =0x7a;
// 	ack[2] =0x0f;
// 	ack[3] =(total_row&0xff00)>>8;
// 	ack[4] =(total_row&0x00ff);
// 	ack[5] =0x00;
// 	for(i =0;i<6;i++)
// 		xor =xor^ack[i];
// 	UARTnSendChar(1,ack,6);		//�������ݳ��ȸ���̨
//   for( j=0;j<left_row;j++)		//���û������ϴ���λ��
//   {
//     Clear_WDT();
//     for(i =0;i<6;i++)
// 			shuzu[i] = ReadData(key_name+i);
// 	  key_name =key_name+6;		//����key_name�ĳ�����6���ֽ�
// 	  if((shuzu[4] ==0xfe)&&(shuzu[5] ==0xfe))
// 	  {
// 	    continue;
// 	  }
// 	  else
// 		{
// 			xor =xor^shuzu[0]^shuzu[1]^shuzu[2]^shuzu[3];
// 	    UARTnSendChar(1,shuzu,4);
// 		}

//   }
// 	shuzu[0] =xor;
// 	UARTnSendChar(1,shuzu,1);
//   Clear_WDT();
// 	return;
// }









