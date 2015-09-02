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
//   IWDG_ReloadCounter();	  //清看门狗数值
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
采用的为串口1
*****/

/**
USB是否连接管脚

**/
 void Usb_Power_Init_In(void)
 {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);    //Key_IO--通信管脚		PC12即USB_BIT
  
 }
/**
 USB控制芯片的复位脚

**/
 void Usb_Rst_Init_Out(void)
 {

 }
/*
利用USB转串口的芯片波特率大概设置为115200bps

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
	//下面是同步模式下才用的
  //USART_InitStructure.USART_Clock = USART_Clock_Disable;
  //USART_InitStructure.USART_CPOL = USART_CPOL_Low;
  //USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
  //USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

//  USART_StructInit(USART_InitStructure);
  USART_Init(USART1, &USART_InitStructure);
  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /*接收中断使能*/
}



 void Usb_IO_Init_In(void)
 {

 }
/*******
 串口1的接收中断函数
********/
// void USART1_IRQHandler(void)
// {
//   u8 nChar;
//   if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//NE就表示有数据；表示RDR有数据
//   {
//     nChar =USART_ReceiveData(USART1);
//     m_UB0.RBuff[m_UB0.cRHead] = nChar;
// 		++m_UB0.cRHead;
// 		if (m_UB0.cRHead >= UARTRXLEN)
// 			m_UB0.cRHead = 0;
//     USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//   }
// }

//复位串口缓冲区
void UARTReset(char uart)
{		
		//先关接受中断
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


void UARTnSendChar(char uart,u8 *str,u8 len)  //串口发送字符串
{
	u8 idx;
	{
		for (idx=0;idx<len;idx++)
		{
			USART_SendData(USART1,str[idx]);		
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 		//等待发送完成
		}
	}
}

 //等待发送，直到完成
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
	m_Uart_State = ERR_UART_OK;	    //检测串口缓冲区是否读到数据
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

//读取一行,'\r'结束,返回读取的字符串长度
uchar UARTReadLine(uchar uart, uchar *pdst, uchar max)
{
  uchar count = 0,total;
  int idx,off;
 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
   if (m_UB0.cRHead != m_UB0.cRTail)
   {
      //计算有效数据总长度
      if (m_UB0.cRHead >= m_UB0.cRTail)
      {
        total  = m_UB0.cRHead - m_UB0.cRTail;
      } else{
        total  = UARTRXLEN + m_UB0.cRHead - m_UB0.cRTail ;
      }
      //找结束符
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
            //增加内容，可以保证"\r\n"结束不影响下一行处理
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

//按新行发送，等待发送，直到完成
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
//     *pa = atol((const char*)str);      //内部函数的问题，强制定义？
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
  检测USB是否成功的进行了连接
  等待上位机发送
  aa 75 0a  00  00 00 D5   的确认帧	  此时串口转接器和钥匙读卡器连接已经成功
  最后一位为异或的值
 */

uchar  USB_Cheak_init(void)
{
  //uchar i;
	uchar nChar;
	uchar  usb_code[6]={0x55,0x7A,0xBB,0x01,0x00,0x01};//下位机反复的向上位机发送此命令，用以建立初始化连接
	uchar  usb_code2[6]={0x55,0x7A,0xCC,0x01,0x00,0x01};//下位机回复上位机的命令，原因是上位机需要
	uchar state =0;
	Clear_WDT();

	//for(i =0;i<20;i++)
	while(1)
	{
	   //void UARTnSendChar(char uart,u8 *str,u8 len)  //串口发送字符串
	   UARTnSendChar(1,usb_code,6);
	   Clear_WDT();
		 g_WaitTimeOut = 0;
		 //u8 SetTimer(u8 timer_id, u16 interval, TimerFun pTask,u16 Time_Top )
		 SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut将g_WaitTimeOut置为true
		 while( g_WaitTimeOut == 0 )  //等待返回的GPS头段的字节
	   {
	     Clear_WDT();
			 //uchar UARTReadFrm(char uart, uchar *pdst, uint max)
			 nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//g_Buff是一个全局变量；#define  MAX_BUFF   520 
			 if(nChar>=6&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0xaa&&g_Buff[3]==0x01&&g_Buff[4]==0x00&&g_Buff[5]==0x01)		//为什么是3？	3表示接收到上位机的返回命令的命令字
			 {
				 UARTnSendChar(1,usb_code2,6);
				 break;
			 }		//没有检查其它的数据位和没有清接收缓冲区
			 if((USB_CHEAK&(GPIO_Pin_12))==0)
				 return state;
		 }		//程序还可能靠定时器超时退出循环
		 //接受到上位机的数据之后，且没有超时
	   KillTimer(TIMER_TIMEOUT);
	   if( g_WaitTimeOut == 0)		//表示未超时退出，即表示收到了上位机的回复
	   {
			 state =1;
			 Clear_WDT();
			 return state;		//正确的出口应该在这里，只要某次尝试成功就返回state=1
	   }
		 

		 
		 
		 
	}
  //return state;		//20次尝试都失败了，此处state是0

}


// uchar  USB_Cheak_init(void)
// {
//  uchar i;
// 	uchar nChar;
// 	uchar  usb_code[7]={0x55,0x7a,0xbb,0x00,0x00,0x00,0x94};//下位机反复的向上位机发送此命令，用以建立初始化连接
// 	uchar state =0;
// 	Clear_WDT();

// 	for(i =0;i<20;i++)
// 	{
// 	   //void UARTnSendChar(char uart,u8 *str,u8 len)  //串口发送字符串
// 	   UARTnSendChar(1,usb_code,7);
// 	   Clear_WDT();
// 		 g_WaitTimeOut = 0;
// 		 //u8 SetTimer(u8 timer_id, u16 interval, TimerFun pTask,u16 Time_Top )
// 		 SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut将g_WaitTimeOut置为true
// 		 while( g_WaitTimeOut == 0 )  //等待返回的GPS头段的字节
// 	   {
// 	     Clear_WDT();
// 			 //uchar UARTReadFrm(char uart, uchar *pdst, uint max)
// 			 nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//g_Buff是一个全局变量
// 			 if(nChar >=3)		//为什么是3？可能表示重发命令
// 			 break;
// 		 }
// 	   KillTimer(TIMER_TIMEOUT);
// 	   if( g_WaitTimeOut == 0)
// 	   {
// 			 state =1;
// 			 Clear_WDT();
// 			 return state;		//正确的出口应该在这里
// 	   }
// 	}
//   return state;		//state要么是0要么是1
// }



/*
 USB通信程序函数
 void USB_transmit(void)
*/ 
void USB_transmit(void)
{
  uchar nChar =0;
	Clear_WDT();
	//检测USB是否成功的进行了连接  等待上位机发送  aa 75 0a  00  00 00 D5   的确认帧	  
	//此时串口转接器和钥匙读卡器连接已经成功  最后一位为异或的值
  nChar =USB_Cheak_init();		//连接初始化
	
  if(nChar ==1)		//表示等待超时
  {
		OLED_Clear();
		often_show(36,56,"PC机通信中..");
		//Deal_Usbdata();
		Test_Usb_Data();
		Clear_WDT();
		MOS_OFF;   //关闭MAX3232电源
		g_State =ST_IDLE;return;
  }
  else if(nChar ==0)
  {
		Clear_WDT();
		MOS_OFF;  //关闭MAX3232电源
		g_State =ST_IDLE;return;
  }

}


void USB_transmit_test(void)
{
  uchar nChar =0;
	Clear_WDT();
	
	//often_show(36,56,"初始化通信中..",0,0x0fff);	
	//检测USB是否成功的进行了连接  等待上位机发送  aa 75 0a  00  00 00 D5   的确认帧	  
	//此时串口转接器和钥匙读卡器连接已经成功  最后一位为异或的值
// 	OLED_Clear();
// 	often_show(10,56,"初始化通信中...",0,0x0fff);	

	
	if(USB_Cheak_init_flag==1)
	{
		OLED_Clear();
		often_show(10,56,"初始化通信中...");			
		nChar =USB_Cheak_init();		//连接初始化
	}
// 	else if(USB_Cheak_init_flag==0)
// 	{
// 		g_State=ST_IDLE;
// 		return;
// 	}
// 	else
// 		nChar=1;
	
	
  if(nChar ==1||(USB_Cheak_init_flag==0))		//表示初始化返回了正确值或者是非初始化标示
  {
		 OLED_Clear();
		 often_show(36,56,"PC机通信中..");
		
		 Test_Usb_Data();
		
		 Clear_WDT();
		 //MOS_OFF;   //关闭MAX3232电源
		
		 OLED_Clear();
		 often_show(36,56,"通信结束");
		 delay_ms(2000);
		 OLED_Clear();		

		 g_State =ST_IDLE;return;
		
  }
  else if(nChar ==0)
  {
		Clear_WDT();
		MOS_OFF;  //关闭MAX3232电源
		g_State =ST_IDLE;return;
  }

}



/****
 对USB通信的数据包进行处理
 判断什么时候退出的程序，
 1.通过判断USB口上的电压来检测
 2.通过退出指令来检测
 下载500个用户信息的时候 分为连个命令段
 第一个提示 
 1.用户的量 2.及每个50用户传个具体的号码
 第二个传递具体的数据包
 aa 75 04  00  00 00 DB
*****/
// void Deal_Usbdata(void)
// {
//    uchar nChar;
//    uchar order ;   //命令字
//    while(USB_CHEAK ==(GPIO_Pin_12))
//    {
// 			Clear_WDT();
// 			UARTReset(0);				 
// 			while(USB_CHEAK ==(GPIO_Pin_12))  //等待返回的USb头段的字节
// 			{
// 				 Clear_WDT();
// 				 nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);
// 				 if(nChar >=3)
// 				 break;
// 			}
// 			order =g_Buff[2];  //取出来命令字
// 			{
// 					switch(order)
// 					{
// 						case 0x04:USB_info1();break;  //读取区号和设备代码--方便管理
// 						case 0x05:USB_info2();break;  //对电子钥匙进行校时--必须要
// 						case 0x06:up_loadInfo();break;	//读取开锁的信息给后台--开锁时间和锁的编号及操作---必须在连接到后台的时候，先执行这个命令，将已下载的任务的执行情况，上传
// 						case 0x07:USB_info4();break;	//往钥匙里面下载可控制锁的信息
// 						case 0x08:USB_info5();break;	//确定帧
// 						case 0x0c:USB_info6();break;	//复位数据存储的头指针--在系统上显示为初始化钥匙
// 						case 0x01:USB_info10();break;	//设置钥匙的操作人员的信息--1.带字模 2.不带字模--[强制为汉字 】
// 						case 0x0e:USB_info11();break;  //设置钥匙中的信息
// 						case 0x02:USB_info12();break;  //删除锁的编号 --不需要
// 						case 0x03:USB_info13();break;  //增加锁的编号 --不需要
// 						case 0x0f:Send_YongHu();break;  //传输已经存储的锁的编号信息给后台
// 						default:break;
// 					}

// 			}
//    }

// }
/****
测试钥匙和PC机的通信，通过转接板

****/
void Test_Usb_Data(void)
{
	
	 uchar nChar;
   uchar order ;   //命令字
	 end_flag=0;	//改为全局变量
   while(USB_CHEAK ==(GPIO_Pin_12))
   {
				Clear_WDT();
				UARTReset(0);		//复位串口接受缓冲区				 
				while(1)  //等待返回的USb头段的字节
				{
					Clear_WDT();
					nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  读取按协议格式发送过来的数据，并进行解析
					if(nChar >=3)		//这里不用再判断头两个字符了
						break;
					if(end_flag==1)
						return;	//直接返回，此时不管USB_BIT是否已经拔出
					if(USB_CHEAK !=(GPIO_Pin_12))		//如果USB_BIT已经拔掉，就直接返回
						return;
				}
				
				
				order =g_Buff[2];  //取出来命令字
				
				switch(order)
				{
					case 0x01:download_yonghu();break;
					case 0x02:key_encode_query();break;
					case 0x03:left_suo_space();break;
					case 0x04:time_verify();break;
					
					case 0x05:authorization();break;
					case 0x06:auto_back_authorization();break;
					
					case 0x07:caima();break;
					case 0x08:auto_back_caima();break;		//自动回传授权和自动回传采码都是钥匙主动的上发
					
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
0x01上位机传给下位机用户信息
2字节用户编号+8字节用户名+8字节所在组名+1字节钥匙数字编号

用户编号用于查找授权文件中对应的用户编号

*/
void download_yonghu(void)
{
		uchar i,ID,k,Len;
		uchar xor =0;		//校验和
		uchar mid_buff[8];
		uint addr;
	  uint row;
		//data_len=g_Buff[3];		//g_Buff是全局变量
		ID=g_Buff[5];
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x01;
		mid_buff[3] =0x02;
		mid_buff[4] =0x00;
		mid_buff[5] =ID;	
	
		ID=g_Buff[5];
	
		Len=g_Buff[3]+5;		//表示整个命令的长度
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[Len-1])		//判断接受命令校验是否正确
		{
				addr=AUTHOR_START;
				Clear_WDT();

				row = ReadData(AUTHOR_ROW_H);   
				row = ((row<<8)&0xff00)+ReadData(AUTHOR_ROW_L);
			
				addr=AUTHOR_START;		//从起始地址开始检查		
			
				for(i=0;i<row;i++)		//在已有的授权中检查是否与当前命令中用户编号相同
				{
						if(ReadData(addr)==g_Buff[6]&&ReadData(addr+1)==g_Buff[7])		//根据用户编号查找用户的地址
							break;
						addr=addr+24;		//现在一条授权的总长度为24字节
				}
				
				//如果找到，addr就是找到的当前地址，如果没有找到就是在已有授权之后的新的地址
				
			  //假设是先传授权再传用户，则肯定能找到
				if(i<row)
				{
						for(i=0;i<16;i++)		//将用户编号对应的用户名和组名存入授权文件中对应的授权信息中，总共16字节
						{
								for(k =0;k<WRITE_ZHI;k++)
									msDelay(500);
								WritEDAta(addr+8+i,g_Buff[8+i]);			//偏移地址是8个字节=2用户编号+2锁编号+4锁码
								Clear_WDT();
						}
			
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);
						WritEDAta(KEY_ENCODE,g_Buff[i+8]);			//存储钥匙编号
				}
				 
				mid_buff[6]= 0x01;		//返回成功
				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
				mid_buff[7]=xor;
				Clear_WDT();
				UARTnSendChar(1,mid_buff,8);	 //返回成功的确定码		
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
		UARTnSendChar(1,mid_buff,8);	 //返回失败码		
	}	
	

}


/*
0x02钥匙编号查询

*/
void key_encode_query(void)
{	
		uchar i,k,Len;
		uchar xor =0;		//校验和
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
		if(xor==g_Buff[Len-1])		//判断接受命令校验是否正确
		{	
				ChipUniqueID[2] = *( u32*)(0X1FFFF7E8); // 低字节
				ChipUniqueID[1] = *( u32*)(0X1FFFF7EC); //  
				ChipUniqueID[0] = *( u32*)(0X1FFFF7F0); // 高字节
			
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
			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
		}	
}

/*
0x03锁存储空间查询
*/
void left_suo_space(void)
{		
		uchar i,k,Len;
		uchar mid_buff[11];
		u16 left_space,addr;
		uchar xor =0;		//校验和
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x03;

		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];			
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[Len-1])		//判断接受命令校验是否正确
		{
				addr= ReadData(AUTHOR_POINT_H);   
				addr=((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
			
				//left_space=(AUTHOR_END-addr);

				left_space=(25-ReadData(AUTHOR_ROW_L));		//返回剩余空间的字节数,这样的空间更准确（考虑到删除的授权）
				
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
			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
		}	
}

/*
0x04对电子钥匙进行校时
6个字节的时间（YYMMDDHHMMSS）
*/
void time_verify(void)
{
  uchar time_date[6];
  uchar mid_buff[13];
  uchar i,k,Len;
  uchar xor =0;		//校验和
  Clear_WDT();
  mid_buff[0] =0x55;		//表示下位机发给上位机
  mid_buff[1] =0x7a;
  mid_buff[2] =0x04;		//命令字

  mid_buff[4] =0x00;
  mid_buff[5] =0x01;		//ID
  Clear_WDT();
	
	Len=g_Buff[3]+5;
	
	for(i =0;i<Len-1;i++)
		xor =xor^g_Buff[i];	
	if(xor==g_Buff[Len-1])		//判断接受命令校验是否正确
	{
			for(i =0;i<6;i++)
				time_date[i] =g_Buff[6+i];		//g_Buff[7~12] 6个数组单元
			
			set_time(time_date[0],time_date[1],time_date[2],time_date[3],time_date[4],time_date[5]);//根据上位机的时间设置钥匙的时间
			msDelay(500);
			Clear_WDT();
			read_time();    //返回实时的时间,存于time_buff中
			for( i =0;i<6;i++)
				mid_buff[6+i] =time_buff[i];
		  mid_buff[3] =0x08;		//长度
			xor=0;
			for(i =0;i<12;i++)
				xor =xor^mid_buff[i];
			mid_buff[12]=xor;
			Clear_WDT();
			UARTnSendChar(1,mid_buff,13);	 //返回确定码；返回实时时间（写入备份数据寄存器的时间）
	}
	else
	{
		for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
		Clear_WDT();
		mid_buff[3] =0x03;		//长度
		mid_buff[6]= 0x00;
		xor=0;
		xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
		mid_buff[7]=xor;
		
		Clear_WDT();
		UARTnSendChar(1,mid_buff,8);	 //返回失败码		
	}
}

/*
0x05将授权文件的信息写入EEPROM
每次写一条授权文件
回复接受成功命令
2个字节用户编号+2字节锁编号 +4字节锁RFID

假设相同的授权（此条命令）不重复
*/
void authorization(void)
{
		uchar data_len,i,high_addr,low_addr,k,total_row,Len;
		uchar xor=0;
		uchar mid_buff[8];
		uint addr;
		data_len=g_Buff[3];		//g_Buff是全局变量
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x05;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[Len-1])		//判断接受命令校验是否正确
		{	
			
				addr= ReadData(AUTHOR_POINT_H);   
				addr =((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
				Clear_WDT();
				msDelay(500);
				for(i=0;i<data_len-2;i++)		//怎么传进来的就怎么放
				{
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);
						WritEDAta(addr+i,g_Buff[i+6]);		
						Clear_WDT();
				}
				
				YH_BH_H=g_Buff[6];		//用户编号的高和低
				YH_BH_L=g_Buff[7];
							
				high_addr=((addr+24)&0xff00)>>8;   //取高8位，再放低
				low_addr =(addr+24)&0x00ff;				 //每条授权24字节
				//更改授权用户信息指针的位置
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();
				WritEDAta(AUTHOR_POINT_H,high_addr);		//#define AUTHOR_START  0x0b4f 
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(AUTHOR_POINT_L,low_addr);		
				
			
				//要区分宏地址里存放的内容到底是表示地址还是数据
				//由于授权信息条数只有50条，所以只用总行数的低8位
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();			
				total_row= ReadData(AUTHOR_ROW_L);   		
				total_row=total_row+1;		//授权信息条数加一
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
				UARTnSendChar(1,mid_buff,8);	 //返回确定码
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
			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
		}
		
}

// void authorization(void)
// {
// 		uchar data_len,i,k,Len;
// 		uchar xor=0;
// 		uchar mid_buff[8];
// 		uint addr;
// 		data_len=g_Buff[3];		//g_Buff是全局变量
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
// 	  if(xor==g_Buff[Len-1])		//判断接受命令校验是否正确
// 		{	
// 				addr= ReadData(AUTHOR_POINT_H);   
// 				addr =((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
// 				Clear_WDT();
// 				msDelay(500);
// 				for(i=0;i<data_len-2;i++)		//怎么传进来的就怎么放
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
// 				UARTnSendChar(1,mid_buff,8);	 //返回确定码
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
// 			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
// 		}
// 		
// }

/*
0x06	发送所有操作记录信息；
每帧: 55 +7A +06+08(数据长度)+00+ID+2字节员工编号+4字节锁码+1字节开锁结果+1字节校验位


*/
void auto_back_authorization(void)
{
		uchar i,j,k,m,ID,total_row,nChar,Len;
		const uchar data_structure_len=5;		//数据格式的长度为5,4个字节的RFID和1个字节的开锁结果
		uchar xor=0;
		uchar mid_buff[14];
		uint addr,start_addr;
		
		ID=0x01;//假设ID等于01
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x06;

		mid_buff[4] =0x00;
		mid_buff[5] =ID;		
			
		//测试
// 		temp_row= ReadData(AUTHOR_ROW_L);
// 		UARTnSendChar(1,&temp_row,1);		
	
		Len=g_Buff[3]+5;		//命令长度为7
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[Len-1]&&Len==7)		//判断接受命令校验是否正确,并且命令长度为7，确定命令长度为8
		{			
			
				addr = ReadData(RECORD_POINT_H);   
				addr = ((addr<<8)&0xff00)+ReadData(RECORD_POINT_L);
				
				total_row=(addr-RECORD_START)/data_structure_len;		//每条授权24字节
			
				//测试，进入for循环之前
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
						for(i=0;i<total_row;i++)		//此时i>=1
						{
							
								start_addr=RECORD_START+i*data_structure_len;		

								mid_buff[6]=YH_BH_H;
								mid_buff[7]=YH_BH_L;
							
							
								for(j=0;j<5;j++)		//4个字节锁码+1个字节开锁结果
								{
									msDelay(500);	
									mid_buff[8+j]= ReadData(start_addr+j);
								}
								
								mid_buff[12]=shouquan_jieguo;				//开锁结果
								
								mid_buff[3] =0x09;
								xor=0;
								for(m =0;m<13;m++)
									xor =xor^mid_buff[m];						
								mid_buff[13]=xor;
															
								UARTnSendChar(1,mid_buff,14);	 //发送一条授权文件

								Clear_WDT();
								UARTReset(0);		//复位串口接受缓冲区										
								while(1)  //发送完一条授权之后必须等待上位机的返回命令
								{
//											UARTReset(0);		//复位串口接受缓冲区		(串口复位不能放在这里)										
									
											g_WaitTimeOut = 0;
											SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut将g_WaitTimeOut置为true
											while( g_WaitTimeOut == 0 );
											KillTimer(TIMER_TIMEOUT);		
									
											msDelay(500);									
											nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520 
											//如果没有数据则nChar=0
																												
											if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xDB)//如果返回0x01就发下一条授权
											{
												Clear_WDT();
												//UARTnSendChar(1,"44444",5);	
												
												break;		//只是跳出内部等待循环，发送下一条授权
											}	
											else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xDA)//如果返回0x01就删除这条授权，再发下一条
											{


												i=i-1;		//跳出内部等待循环之后，会直接再跳出发送循环，直接结束整个自动回传
												break;
// 												Clear_WDT();
// 												UARTnSendChar(1,"11111",5);
// 												for(k =0;k<WRITE_ZHI;k++)
// 													msDelay(500);														
// 												;		//什么也不做继续等待
											}
											

								}//内部while循环

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
			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
		}
	

}



// void auto_back_authorization(void)
// {
// 		uchar i,j,k,m,ID,total_row,row,nChar,Len;
// 		uchar xor=0;
// 		uchar mid_buff[12];
// 		uint addr,start_addr;
// 		
// 		ID=0x01;//假设ID等于01
// 		
// 		mid_buff[0] =0x55;
// 		mid_buff[1] =0x7a;
// 		mid_buff[2] =0x06;

// 		mid_buff[4] =0x00;
// 		mid_buff[5] =ID;		
// 			
// 		//测试
// // 		temp_row= ReadData(AUTHOR_ROW_L);
// // 		UARTnSendChar(1,&temp_row,1);		
// 	
// 		Len=g_Buff[3]+5;
// 		
// 		for(i =0;i<Len-1;i++)
// 			xor =xor^g_Buff[i];	
// 	  if(xor==g_Buff[Len-1]&&Len==7)		//判断接受命令校验是否正确,并且命令长度为7，确定命令长度为8
// 		{			
// 			
// 				addr= ReadData(AUTHOR_POINT_H);   
// 				addr =((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
// 				
// 				total_row=(addr-AUTHOR_START)/24;		//每条授权24字节
// 			
// 				//测试，进入for循环之前
// 				//UARTnSendChar(1,&total_row,1);				
// 			
// 				if(total_row==0)
// 				{
// 						Clear_WDT();
// 						UARTnSendChar(1,"00000",5);
// 				}
// 				else
// 				{
// 						for(i=0;i<total_row;i++)		//如果没有授权就什么也不做
// 						{
// 							
// 								start_addr=AUTHOR_START+i*24;																		
// 								for(j=0;j<4;j++)		//4个字节：用户编号、锁编号
// 								{
// 									msDelay(500);	
// 									mid_buff[6+j]= ReadData(start_addr+j);
// 								}
// 																												
// 								if(mid_buff[6]==0&&mid_buff[7]==0)		//如果用户编号为零表示此条授权已经被删了
// 										continue;		//继续下一条授权
// 								
// 								mid_buff[10]=key_state();		//key_state函数返回开锁状态
// 								
// 								mid_buff[3] =0x07;
// 								xor=0;
// 								for(m =0;m<11;m++)
// 									xor =xor^mid_buff[m];						
// 								mid_buff[11]=xor;
// 															
// 								UARTnSendChar(1,mid_buff,12);	 //发送一条授权文件

// 								Clear_WDT();
// 								UARTReset(0);		//复位串口接受缓冲区										
// 								while(1)  //发送完一条授权之后必须等待上位机的返回命令
// 								{
// //											UARTReset(0);		//复位串口接受缓冲区		(串口复位不能放在这里)										
// 									
// 											g_WaitTimeOut = 0;
// 											SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut将g_WaitTimeOut置为true
// 											while( g_WaitTimeOut == 0 );
// 											KillTimer(TIMER_TIMEOUT);		
// 									
// 											msDelay(500);									
// 											nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520 
// 											//如果没有数据则nChar=0
// 																												
// 											if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x00&&g_Buff[7]==0xDB)//如果返回0x00就发下一条授权
// 											{
// 												Clear_WDT();
// 												//UARTnSendChar(1,"44444",5);	
// 												
// 												break;		//只是跳出内部等待循环，发送下一条授权
// 											}	

// 											else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xDA)//如果返回0x01就删除这条授权，再发下一条
// 											{
// 												for(j=0;j<24;j++)		//将当前地址所在授权中所有字节都置为0
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
// 												//测试
// 												//UARTnSendChar(1,&row,1);		//为什么这里会读到0								
// 												
// 												Clear_WDT();												
// 												
// 												row=row-1;
// 												for(k =0;k<WRITE_ZHI;k++)
// 													msDelay(500);
// 												WritEDAta(AUTHOR_ROW_L,row);		//将总行数减一，再写回
// 												
// 												for(k =0;k<WRITE_ZHI;k++)		//这里的延时很重要，不然数据没有写进去
// 													msDelay(500);												
// 											
// 												row=ReadData(AUTHOR_ROW_L);
// 												msDelay(500);												
// 												//测试
// 												//UARTnSendChar(1,&row,1);	
// 												
// 												Clear_WDT();
// 												
// 												break;		//跳出内部循环，结束等待
// 											}
// 											else//(nChar==0)
// 											{																					
// 												//UARTnSendChar(1,"22222",5);	
// 											
// // 												g_WaitTimeOut = 0;
// // 												SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut将g_WaitTimeOut置为true
// // 												while( g_WaitTimeOut == 0 );
// // 												KillTimer(TIMER_TIMEOUT);													
// 																							
// 												Clear_WDT();												
// 											}
// // 											else
// // 											{
// // 												//i=total_row;		//跳出内部等待循环之后，会直接再跳出发送循环，直接结束整个自动回传
// // 												//break;
// // 												Clear_WDT();
// // 												UARTnSendChar(1,"11111",5);
// // 												for(k =0;k<WRITE_ZHI;k++)
// // 													msDelay(500);														
// // 												;		//什么也不做继续等待
// // 											}  
// 											

// 								}//内部while循环

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
// 			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
// 		}
// 	

// }










/*
0x07设备采码
2个字节员工编号+2字节锁编号 
*/
void caima(void)
{
		uchar data_len,i,high_addr,low_addr,k,total_row,Len;
		uchar xor;
		uchar mid_buff[8];
		uint addr;
		data_len=g_Buff[3];		//g_Buff是全局变量
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x07;
		mid_buff[3] =0x06;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		

		
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[Len-1])		//判断接受命令校验是否正确
		{	

				addr= ReadData(CAIMA_POINT_H);   
				addr =((addr<<8)&0xff00)+ReadData(CAIMA_POINT_L);
				Clear_WDT();
				msDelay(500);
				for(i=0;i<data_len-2;i++)		//data_len-1,1表示ID是一个字节
				{
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);
						WritEDAta(addr+i,g_Buff[6+i]);		
						Clear_WDT();
				}
				
				high_addr=((addr+8)&0xff00)>>8;   //采码区每项8个字节,将地址取高8位，再放低
				low_addr =(addr+8)&0x00ff;		
				//更改授权用户信息指针的位置
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();
				WritEDAta(CAIMA_POINT_H,high_addr);		//#define AUTHOR_START  0x0b4f 
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(CAIMA_POINT_L,low_addr);		
				
				
				//要区分宏地址里存放的内容到底是表示地址还是数据
				//由于授权信息条数只有50条，所以只用总行数的低8位
				total_row= ReadData(CAIMA_ROW_L);   		
				total_row=total_row+1;
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(CAIMA_ROW_L,total_row);				//由于最多只有30行
				
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				Clear_WDT();
				
				mid_buff[6]= 0x01;
				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
				mid_buff[7]=xor;
				
				Clear_WDT();
				UARTnSendChar(1,mid_buff,8);	 //返回确定码
				
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
				UARTnSendChar(1,mid_buff,8);	 //返回失败码		
		}

}





/*
0x08自动回传采码

2个字节员工编号+2字节锁编号 +4个字节的锁码

这4个字节的锁码是钥匙采码时新增的

*/
void auto_back_caima(void)
{
		uchar i,j,k,m,ID,total_row,nChar,Len;
		uchar xor=0;
		uchar mid_buff[15];
		uint addr,start_addr;
		
		ID=0x01;//假设ID等于01
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x08;

		mid_buff[4] =0x00;
		mid_buff[5] =ID;		
			
		//total_row= ReadData(AUTHOR_ROW_L);
		Len=g_Buff[3]+5;
		
		for(m =0;m<Len-1;m++)
			xor =xor^g_Buff[m];	
	  if(Len==7&&xor==g_Buff[Len-1])		//判断接受命令校验是否正确,将这条命令与其确定命令区别开来
		{		

				addr= ReadData(CAIMA_POINT_H);   
				addr =((addr<<8)&0xff00)+ReadData(CAIMA_POINT_L);		//在界面的采码功能里会改变采码指针的值
				
				total_row=(addr-CAIMA_START)/8;
				
			  if(total_row==0)
					 UARTnSendChar(1,"00000",5);		//如果total_row=0，

				for(i=0;i<total_row;i++)
				{
						start_addr=CAIMA_START+i*8;
						
						for(j=0;j<8;j++)		//8个字节：用户编号、锁编号、锁码
							mid_buff[6+j]= ReadData(start_addr+j);
					
						mid_buff[3] =0x0a;		
						xor=0;
						for(m =0;m<14;m++)
							xor =xor^mid_buff[m];	
						mid_buff[14] =xor;	
					
						msDelay(500);
						Clear_WDT();
						UARTnSendChar(1,mid_buff,15);	 //发送一条授权文件
						
						for(k =0;k<WRITE_ZHI;k++)
							msDelay(500);
						Clear_WDT();

						UARTReset(0);		//复位串口接受缓冲区				 
						while(1)  
						{
							Clear_WDT();
							nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  
							if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x08&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xD4)
								break;		//如果收到确认，结束等待，传下一条采码数据
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
			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
		}
		
		
}



/*
0x09删除设备，即删除锁
2字节锁编
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
	  if(xor==g_Buff[Len-1])		//判断接受命令校验是否正确
		{
				addr2= ReadData(AUTHOR_POINT_H);   
				addr2=((addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
				Clear_WDT();
				msDelay(500);
			
				max_row=(addr2-AUTHOR_START)/24;		//只需查AUTHOR_START~AUTHOR_POINT之间的所有授权，其中删除的授权全部为0
			
			
				addr=AUTHOR_START;
				for(i=0;i<max_row;i++)		//其实可以查完所有的授权
				{
						if(ReadData(addr+2)==g_Buff[6])		//根据锁编号查找用户，且该用户没有被删除
						{		
								for(k =0;k<WRITE_ZHI;k++)
									msDelay(500);
								if(ReadData(addr+2)==g_Buff[7])
										break;
						}	
						
						addr=addr+24;		//每条授权24个字节
				}
				if(i<=max_row)
				{
						for(i=0;i<12;i++)		//这一授权中所有字节都置为0
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
						UARTnSendChar(1,mid_buff,8);	 //返回成功的确定码				
				} 
				else		//没查到对应的锁编号
				{

						mid_buff[6]= 0x00;
						xor=0;
						xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
						mid_buff[7]=xor;
						Clear_WDT();
						UARTnSendChar(1,mid_buff,8);	 //返回失败的确定码		
					
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
			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
		}
}


/*
0x0a上位机传给下位机巡检任务
2字节锁编号+2字节巡检任务编号+14个字节具体任务 +1字节巡检结果
*/
void download_xunjian(void)
{
		uchar data_len,i,high_addr,low_addr,k,total_row,Len;
		uchar xor=0;
		uchar mid_buff[8];
		uint addr;
		data_len=g_Buff[3];		//g_Buff是全局变量

		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0a;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		
	
		Len=g_Buff[3]+5;
		
		for(i =0;i<Len-1;i++)
			xor =xor^g_Buff[i];	
	  if(Len>8&&xor==g_Buff[Len-1])		//判断接受命令校验是否正确
		{
			addr= ReadData(XUNJIAN_POINT_H);   
			addr =((addr<<8)&0xff00)+ReadData(XUNJIAN_POINT_L);
			Clear_WDT();
			msDelay(500);
			for(i=0;i<data_len-2;i++)		//怎么传进来的就怎么放,注意是按数据长度来放的
			{
					for(k =0;k<WRITE_ZHI;k++)
						msDelay(500);
					WritEDAta(addr+i,g_Buff[i+6]);		
					Clear_WDT();
			}
			
			high_addr=((addr+19)&0xff00)>>8;   //取高8位，再放低
			low_addr =(addr+19)&0x00ff;				 //每一条巡检任务19个字节
			
			//更改巡检小任务指针的位置
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
			WritEDAta(XUNJIAN_POINT_H,high_addr);		//#define AUTHOR_START  0x0b4f 
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(XUNJIAN_POINT_L,low_addr);		
			
		
			//要区分宏地址里存放的内容到底是表示地址还是数据
			//由于巡检小任务条数最多只有53条，所以只用总行数的低8位
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();			
			total_row= ReadData(XUNJIAN_ROW_L) ;   		
			total_row=total_row+1;		//巡检小任务条数加一
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
			UARTnSendChar(1,mid_buff,8);	 //返回确定码
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
			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
		}
	
}


/*

0xob自动回传巡检任务结果
2字节锁编号+2字节巡检任务编号+0x22(具体任务起始码)+巡检结果 +0x23(具体任务终止码) 

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
	  if(Len==7&&xor==g_Buff[Len-1])		//判断接受命令校验是否正确
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
					mid_buff[6+j]=ReadData(start_xunjian+18);		//取巡检小任务末尾的巡检结果
					
					xor=0;
					for(m =0;m<11;m++)
						xor =xor^mid_buff[m];	
					mid_buff[11]=xor;

					msDelay(500);
					Clear_WDT();
					UARTnSendChar(1,mid_buff,12);	 //发送巡检结果				
					
					Clear_WDT();
					UARTReset(0);		//复位串口接受缓冲区				 
					while(1)  //发送完一条授权之后必须等待上位机的返回命令
					{
						
						for(k =0;k<WRITE_ZHI;k++)
								msDelay(500);
						Clear_WDT();
						nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  
						
						if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0b&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xD7)//如果返回0x01就发下一条授权
								break;		//只是跳出内部等待循环
						else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0b&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x00&&g_Buff[7]==0xD6)
						{
								i--;		//相当于重发
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
			UARTnSendChar(1,mid_buff,8);	 //返回失败码		
		}
} 

/*
oxoc清除所有授权

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
	
	  if(xor==g_Buff[Len-1])		//判断接受命令校验是否正确
		{
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();
			WritEDAta(AUTHOR_ROW_H,0x00);		 //授权信息行数为0
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
			UARTnSendChar(1,mid_buff,8);	 //返回成功码	

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
			UARTnSendChar(1,mid_buff,8);	 //返回失败码	
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
		  UARTnSendChar(1,mid_buff,8);	 //返回确定码	
		}
	  else
		{
			mid_buff[6]= 0x00;	
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //返回失败码				
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
			
			end_flag=1;		//通信结束，直接退出等待命令状态
			
			mid_buff[6]= 0x01;	//表示成功
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //返回确定码	
			USB_Cheak_init_flag=0;		//表示非初始化链接状态
		}
	  else
		{
			mid_buff[6]= 0x00;	//表示失败
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //返回失败码				
		}
		
}



/*发送此条命令清EEPROM*/
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
			
			mid_buff[6]= 0x01;	//表示成功
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //返回确定码	
			USB_Cheak_init_flag=0;		//表示非初始化链接状态
		}
	  else
		{
			mid_buff[6]= 0x00;	//表示失败
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  Clear_WDT();
		  UARTnSendChar(1,mid_buff,8);	 //返回失败码				
		}		



}




/*
开锁状态函数
返回状态
*/
uchar key_state(void)
{
		return 1;

}




/***
上传操作员的信息及开锁记录给后台
***/
void up_loadInfo(void)
{
	uchar ack[7] ={0x55,0x7A,0x06,0x00,0x00,0x00,0x29};
	uchar nChar ;
  //ReadData是通过SPI读eeprom的函数，放回所在地址的数据
	nChar =ReadData(TASK_BIT);//TASK_BIT任务标志位，当为 '1' 时表示有一个任务未完成，当为 '0' 时标志任务已结束
	if(nChar ==1)
	{
		Send_Opretar();
		USB_info3();
	}
	else if(nChar ==0)
	{
	  UARTnSendChar(1,ack,7);	 //返回确定码
	}
	return;
}

/***
将操作人员的信息发送给后台
与开锁的记录绑定发送
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
	
		//发送整个mid_Buff
	  for(i =0;i<6;i++)
			UARTSendChar(1,mid_Buff[i]);
	  for(i =0;i<Len;i++)
			UARTSendChar(1,g_Buff[i]);
	  UARTSendChar(1,xor);
	  return;
}

/****
设定操作人员的长度最长为16个汉字
武汉惠合科技公司
郑州办事处研发部
一个汉字的字模长度为32个字节
最大的数据包长度为32*16 =512个字节
分为两个命令帧来执行
1.一个发送需要显示的汉字的内码
 发送回应帧给后台
2.一个发送需要显示的汉字的字模
发送回应帧给后台
HANZI_FIRST_START
HANZI_SECOND_START
g_Buff[3] =len_high
g_Buff[4] =len_Low

将写入的操作员或者管理单位的信息在LCD屏上进行显示
在更改操作员的情况下，将头指针等信息进行一次复位，清空以前的记录
在输入新的操作员信息的情况下，才将任务的标志位置 0
*****/
// void  USB_info10(void)
// {
// 	uchar ack[7] ={0x55,0x7A,0x01,0x00,0x00,0x00,0x2e};
//   uint hanzi_Len,i,old_Len,j;
//   
//   uchar head[3];
// 	hanzi_Len =g_Buff[4]-1;  //消除掉ID
// 	for( i =0;i<hanzi_Len;i++)
// 	{
// 		 for(j =0;j<40;j++)
// 			 msDelay(500);
// 		 WritEDAta(HANZI_FIRST_START+i,g_Buff[7+i]);
// 	}
//   for(j =0;j<40;j++)
//   msDelay(500);
//   WritEDAta(Operate_name_Len,hanzi_Len);    //存储内码的长度
//   UARTnSendChar(1,ack,7);	 //返回确定码
//   UARTReset(0); 
//   Clear_WDT();            //复位接收缓冲区


// 	KillTimer(TIMER_TIMEOUT);
// 	g_WaitTimeOut = 0;
// 	SetTimer(TIMER_TIMEOUT,20,Do_TimeOut,THREE_MIN);
// 	WaitUartChar(0xaa);
// 	WaitUartChar(0x75);
// 	WaitUartChar(0x01);	  //后续帧
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
// 	while(( hanzi_Len !=0)&& (g_WaitTimeOut == 0)) //是否需要用时间来限制
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
// 		WritEDAta(HANZI_SECOND_START+i,g_Buff[i]); //存储汉字的字模
// 	}
// 	old_Len =old_Len/32;
// 	OLED_Clear();
// 	Show_Operater(0,0,g_Buff,old_Len,0,0x0fff);
// 	for(j =0;j<40;j++)
// 		msDelay(500);
// 	WritEDAta(TASK_BIT ,1);
// 	Init_KeyRecord();   //复位链接表的头存储指针
// 	UARTnSendChar(1,ack,7);	 //返回确定码
// 	return;

// }
//设置钥匙中的区位码等信息
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
//   // mid_buff[6+i] =time_buff[i];   //时间去掉
// 		for(i =0;i<12;i++)
// 			xor =xor^mid_buff[i];
// 		mid_buff[12]=xor;
// 		Clear_WDT();
// 		UARTnSendChar(1,mid_buff,13);	 //返回确定码

//   }
//   Clear_WDT();
//   return;
// }

/***
清空数据存储的所有信息
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
//   UARTnSendChar(1,ack,7);	 //返回确定码
//   return;

// }
/********************
 钥匙返回区号、设备代码、钥匙编号、时间和电池电量。后台判定是否有效
 区号：03 71
 设备代码：34 56 78
 钥匙编号：01
 时间：月 日 时 分
 电池电量：4.1V
 55 7a 01  00  0b +data+cs 0d
 aa 75 0a  00  00 cs 0d  --确认码
 上位机后台应该作为主动设备，
 下位机作为被动设备	 ok
*********************/
void USB_info1(void)
{
  uchar i;
  uchar mid_buff[21];
  uchar xor =0;       //效验和
  Clear_WDT();
  for(i =0;i<10;i++)
   msDelay(500);
  Clear_WDT();
  mid_buff[0] =0x55;
  mid_buff[1] =0x7a;
  mid_buff[2] =0x04;		//表示命令字
  mid_buff[3] =0x00;
  mid_buff[4] =0x0c;		//表示数据长度，从第6位到17位，不包括校验位
  mid_buff[5] =0x00;
  for(i =0;i<3;i++)		//#define   LOCAL_NUM	  0X01	 /*01 02 03 04*区位码*/
		mid_buff[6+i] =ReadData(LOCAL_NUM+i); //mid_buff 678  区号
  mid_buff[9] =ReadData(KEY_CODE);		//设备代码  #define   KEY_CODE		   0X05	 /*05*/
  for(i =0;i<2;i++)
		mid_buff[10+i] =ReadData(KEY_NUM+i);		//钥匙编号  #define   KEY_NUM		   0X06	 /* 06 07 08*/
  Clear_WDT();
  read_time();
  for( i =0;i<6;i++)
		mid_buff[12+i] =time_buff[i];		//mid_buff 12~17  时间：月 日 时 分
  for(i =0;i<18;i++)
		xor =xor^mid_buff[i];
  mid_buff[18] =xor;		//校验和
  Clear_WDT();
  UARTnSendChar(1,mid_buff,19);
  Clear_WDT();
  return;

}
/****
 设置RTC时钟 ok
 回个实时时钟给后台？问题
 多了一位ID
*****/
void USB_info2(void)
{
  uchar time_date[6];
  uchar mid_buff[13];
  uchar i;
  uchar xor =0;		//校验和
  Clear_WDT();
  mid_buff[0] =0x55;		//表示下位机发给上位机
  mid_buff[1] =0x7a;
  mid_buff[2] =0x04;		//命令字
  mid_buff[3] =0x07;		//长度
  mid_buff[4] =0x00;
  mid_buff[5] =0x01;		//ID
  Clear_WDT();
	
  for(i =0;i<6;i++)
		time_date[i] =g_Buff[7+i];		//g_Buff[7~12] 6个数组单元
  set_time(time_date[0],time_date[1],time_date[2],time_date[3],time_date[4],time_date[5]);//根据上位机的时间设置钥匙的时间
  msDelay(500);
  Clear_WDT();
  read_time();    //返回实时的时间,存于time_buff中
  for( i =0;i<6;i++)
		mid_buff[6+i] =time_buff[i];
  for(i =0;i<12;i++)
		xor =xor^mid_buff[i];
  mid_buff[12]=xor;
  Clear_WDT();
  UARTnSendChar(1,mid_buff,13);	 //返回确定码；返回实时时间（写入备份数据寄存器的时间）
  Clear_WDT();
  return;

}











/*
ok
向后台传输存储的开锁记录的情况
*/
//测试开锁记录是否有误的函数
void Test_USB_Record(void)
{
  Clear_WDT();
  g_Buff[0]= ReadData(KEY_RECORD_ROW_H);   //已存储的组数
  g_Buff[1] =ReadData(KEY_RECORD_ROW_L);
  g_Buff[2] = ReadData(KEY_RECORD_INDEX_H);   //已存储的组数
  g_Buff[3] =ReadData(KEY_RECORD_INDEX_L);
  Clear_WDT();
  UARTnSendChar(1,g_Buff,4);
  Clear_WDT();
  //while(1);
}
/***
 是不是没有等到回应的确认帧？
****/
void USB_info3(void)
{
  uint  row,i,head,total_data;
  uchar mid_buff[9],each_data,j;
  uchar nChar,zhen,left,code;
  uchar xor =0;
  Clear_WDT();
  code = ReadData(KEY_CODE);
  row= ReadData(KEY_RECORD_ROW_H);   //已存储的组数
  row =((row<<8)&0xff00)+ReadData(KEY_RECORD_ROW_L);
  head= ReadData(KEY_RECORD_INDEX_H);   //已存储的组数
  head =((head<<8)&0xff00)+ReadData(KEY_RECORD_INDEX_L);
 // Test_USB_Record();
 // row =2000;  //测试
  total_data =row*10;   //所有的数据量的大小
  each_data =200;       //20户数据
  Clear_WDT();
  if(row <2000)
  {
		head =KEY_RECORD_START; //未存满的情况
  }
  else if(row >=2000)
  {
		head =head;	    //存满数据的情况

  }
   zhen =row/20;
   left =row%20;
   if(left ==0)	  //余数为0
   {
			zhen =zhen;

   }
   else if(left !=0)   //余数不为0
   {
			zhen =zhen+1;

   }
   Clear_WDT();
  //head =0x7e2;
  //row =10;   //测试
//  length =row*10;
  mid_buff[0] =0x55;
  mid_buff[1] =0x7a;
  mid_buff[2] =0x06;
  mid_buff[3] =0X00;		//高位地址
  mid_buff[4] =0X02;		//低位地址
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
  while( g_WaitTimeOut ==0)		 //等待上位机发送确定帧
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

  for(i =0;i<zhen;i++) //发送数据包给后台---锁的操作记录
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
		while( g_WaitTimeOut ==0)		 //等待上位机发送确定帧
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

设置1000户的信息是分段设置的
1.传递的数据包数
2.每个50用户的中间值 传递过来
 如果存满的话，应该传递21组数据

****/

void Read_info4(void)
{
  uchar nChar;
  uint k;
  	/****/
		 for(k =0;k<4000;k++)
	   {
	      Clear_WDT();
	     nChar =ReadData(KEY_NAME_START+k);	  //测试读写部分是否有误--测试
 	     UARTSendChar(1,nChar);
	    }
		while(1);
		/*****/


}
/***
求模的运算

***/
uchar Mode(uint K)

{

    return K%M;		//#define   M   173

}

/***
采用散列表的存储模式
4个字节的用户号，和两个字节的地址--地址为下一个同模的用户号的存储位置

情况1：已经存满了1000户
情况2：未存储满1000户

***/
void New_Save(uint row_len)
{
 // uchar Ti[246];
  uchar mid_name[4];  //用户号的中间暂存
  uchar i,k,nChar,new_row;
  uint j;
  uint last_addr;
  uint add =0;
  Clear_WDT();
  last_addr= ReadData(KEY_YONGHU_INDEX_H);   //已存储的组数
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
		last_addr =last_addr+6;  //每一组记录占据6个字节 4个号+2个连接地址
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
	  WritEDAta(SANBIAO_START+j,Ti[j]);		//有病吗？
  }
	for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
	Clear_WDT();
}
/****
 初始化的时候，将末地址设置为0XFFFF
*****/
void HashInsert(uchar t[],uint nChar,uchar nm[],uint addr)
{
   uchar i,subaddr_h,subaddr_l,old_addr_h,old_addr_l;
   uchar k;
   subaddr_h =  (addr&0x00ff);
   subaddr_l =  (addr&0xff00)>>8;
   Clear_WDT();
   i =Mode(nChar);		//#define   M   173;对137求模
   i=2*i;
   Clear_WDT();
   if((t[i] ==0xff)&&(t[i+1] ==0xff))
   {
			t[i] =subaddr_h;
			t[i+1] =subaddr_l;		//如果为首地址，则无需连接
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
返回剩余的用户空间

****/
void Ask_LeftyhNum(void)
{


}

/****
往钥匙里面下载可控制锁的信息

delete_row  --已经被删除的组数
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
//   total_row= ReadData(KEY_YONGHU_ROW_H);   //已存储的组数
//   total_row =((total_row<<8)&0xff00)+ReadData(KEY_YONGHU_ROW_L);
//   delete_row =ReadData(DETELE_ROW);		//delete_row  --已经被删除的组数
//   delete_row_tow =delete_row;
//   total_row =1000-total_row; //计算剩余的空间
//   for(i =0;i<246;i++)
// 		Ti[i] =ReadData(SANBIAO_START+i);	//开机的时候将，数据缓冲区给读出来
//   Clear_WDT();
//   if(total_row ==0x00)	 //如果已经存储满1000户，同时没有删除的用户的情况
//   {
//      Clear_WDT();
//      if(delete_row ==0)		
//      return ;      //如果全部存满的情况下，则不再存储

//   }
//  // for(j =0;j<53;j++)
//  // g_Buff[j] =test_row[j];
// // code =g_Buff[7];               //锁类型
//   row =g_Buff[6];	        //后续帧的数目
//  //old_row =g_Buff[8];       	//中间值的数目
// // old_row_tow =old_row*4;        //计算出数据的长度
//   Clear_WDT();
//    //Read_Text();
//    // for(j =0;j<246;j++)
//  // {
//    // nChar =  ReadData(SANBIAO_START+j);
//    // UARTSendChar(1,nChar);
//   //}
//    /**
//   if(old_row <21)
//   {							//将取出来的帧的中间值，存储起来 ，作为方便检测使用
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
// 	   name_second =name_first+4*j;	//有问题
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
// 	    max_name[j] =ReadData(KEY_NAME_ZERO+j);	   //最小的锁编号
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
// 		 WritEDAta(KEY_NAME_MAX+j,max_name[j]);     //最大的锁编号
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
// 	     nChar =ReadData(KEY_NAME_ZERO+j);	  //测试读写部分是否有误--测试
//  	     UARTSendChar(1,nChar);
// 	   }
// 		
//   }	 
//   *****/
//   UARTnSendChar(1,ack,7);  //发确定帧给后台
//   UARTReset(0); 
//   Clear_WDT();            //复位接收缓冲区
// 	row =1;
//   for(j =0;j<row;j++)
//   {
// 		Clear_WDT();
// 		delete_row =ReadData(DETELE_ROW);  //每帧都检测删除的组数
// 		KillTimer(TIMER_TIMEOUT);
// 		g_WaitTimeOut = 0;
// 		SetTimer(TIMER_TIMEOUT,20,Do_TimeOut,THREE_MIN);
// 		WaitUartChar(0xaa);
// 		WaitUartChar(0x75);
// 		WaitUartChar(0x09);	  //后续帧
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
// 		while(( length !=0)&& (g_WaitTimeOut == 0)) //是否需要用时间来限制
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
// 	//	if(xor ==0)	  //如果效验无误，数据正确	 ---效验有问题	一帧为50组用户号
// 		{
// 		   old_length =old_length-1;
// 		   row_length =old_length/4; //检测发送的组数
// 		   Clear_WDT();
// 		   if(delete_row !=0)   //删除的组数不为零
// 		   {
// 			    if( row_length <=delete_row)
// 			    {
// 			     for( k =0;k<old_length;k++)
// 						left_buff[k] =g_Buff[k];   //将数据移到中间缓冲区
// 			     increase_LockNum(left_buff,old_length);
// 				   Clear_WDT();
// 			     UARTnSendChar(1,ack,7);  //发确定帧给后台	 ---似乎有问题,写已存储的组数，是否有问题
// 		       UARTReset(0);
// 			     return; 
// 			    }
// 			    else if( row_length >delete_row)   //有问题???  
// 			    {
// 				     Clear_WDT();
// 			       delete_row =delete_row*4;
// 			       k =old_length-delete_row;
// 			       for(l =0;l<delete_row;l++)
// 							left_buff[l] =g_Buff[k+l];
// 			       increase_LockNum(left_buff,delete_row);
// 			       old_length = old_length -delete_row;
// 			       row_length =row_length-delete_row_tow;   //剩余的组数
// 			    }

// 		    }
// 		    
// 		    if( total_row >=row_length)
// 		    {
// 			     Clear_WDT();
// 		       total_row =total_row -row_length;
// 		       New_Save(old_length);   //新的存储方式
// 		       UARTnSendChar(1,ack,7);  //发确定帧给后台
// 		       UARTReset(0);
// 		    }
// 		    else if( total_row <row_length)
//         {
// 				    Clear_WDT();
// 		        old_length =total_row;
// 		        New_Save(old_length);   //新的存储方式
// 		        {
// 							Clear_WDT();
// 							for(k =0;k<WRITE_ZHI;k++)
// 								msDelay(500);
// 							WritEDAta(KEY_YONGHU_ROW_H,0x03);
// 							for(k =0;k<WRITE_ZHI;k++)
// 								msDelay(500);
// 							WritEDAta(KEY_YONGHU_ROW_L,0xe8);
// 							for(k =0;k<WRITE_ZHI;k++)	//回复下载已经满员提示给后台
// 								msDelay(500);
// 							Clear_WDT();		      

// 		        }
// 						UARTnSendChar(1,ack,7);  //发确定帧给后台
// 						UARTReset(0);
// 						Clear_WDT();
// 						return;
//           }
// 		   //UARTnSendChar(1,ack,7);  //发确定帧给后台
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
//   UARTnSendChar(1,ack,7);  //发确定帧给后台

// }
/*
 读取按协议格式发送过来的数据，并进行解析

*/

uchar UARTReadFrm(char uart, uchar *pdst, uint max)
{
  uchar count = 0,total;
  uchar idx,off;
  Clear_WDT();
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		//关接收中断，在接受中断USART1_IRQHandler()不断的将接受的数据放在m_UB0
  if (m_UB0.cRHead != m_UB0.cRTail)
   {
      //计算有效数据总长度;得到数据时head会增加
      if (m_UB0.cRHead >= m_UB0.cRTail)		//如果头大于尾
      {
        total  = m_UB0.cRHead - m_UB0.cRTail;
      } 
			else
			{
        total  = UARTRXLEN + m_UB0.cRHead - m_UB0.cRTail ;		//如果尾大于头
      }
      //找结束符
      if (total>4)//帧长度至少为4
      {
          if (m_UB0.RBuff[m_UB0.cRTail]!=(uchar)0xaa)		//
          {
              m_UB0.cRTail = (m_UB0.cRTail+1)% UARTRXLEN;		//UARTRXLEN为254,cRTail加1
          }
					else
					{   
              off = (m_UB0.cRTail + 1) % UARTRXLEN;		//cTail的下一个位置的地址
              if (m_UB0.RBuff[off]!=(uchar)0x75)
              {
                m_UB0.cRTail = (m_UB0.cRTail+2)% UARTRXLEN;		//再将cTail加1
              }
							else		//表示接收到的第一个数aa，第二个数75
							{
                  off = (m_UB0.cRTail + 3) % UARTRXLEN;	//off表示第3个数，cRTail没有变
									count=m_UB0.RBuff[off]+5;		//5表示开头2个字符，1个命令字，1个长度，0x00
//                   count = m_UB0.RBuff[off]<<8;//数据长度高字节		表示取第三个数的高8位
//                   off = (m_UB0.cRTail + 4) % UARTRXLEN;//off表示第4个数
//                   count = count + m_UB0.RBuff[off]+7;//数据长度低字节，两者相加后count表示整个数据长度，7表示2个开头字符，1个命令字，2个长度，0x00,检验位
                  if (count <= total && count < max)	//total是缓冲区有效数据的长度，count是整个命令帧的长度,所以永远有count<total
                  {
                      //数据长度满足一帧的要求
                      //total = 0;//纪录校验和,total换了一种含义
                      for (idx=0; idx < max-1 && idx<count;idx++)
                      {
                        off = (m_UB0.cRTail + idx) % UARTRXLEN;		//cRTail也没有变
                        pdst[idx] = m_UB0.RBuff[off];		////将接受缓冲区的按命令帧存入pdst数组
                        //total ^= pdst[idx];//计算校验和
                      }
//                      if (total==0)//校验正确，此处为奇校验
                          m_UB0.cRTail = (off + 1) % UARTRXLEN;		//校验正确就将cRTail放在整个数据帧的下一个位置
//                       else
//                       {
//                           //校验错误，抛弃2个字节，其实为本帧的头
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

//返回缓冲区内字节数
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
等待串口数据ch
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
存储可以控制的钥匙信息的编号数据包
*****/
uchar  Save_KeyNum(void)
{

  return 0;
}
/****
上传电子钥匙开锁的历史信息给后台
****/
uchar  Send_KeyData(void)
{

  return 0;
}

/****************
 通过后台给电子钥匙进行校时

***************/

uchar Cheak_KeyClock(void)
{
  return 0;
}

/***
 删除一个锁的编号
***/

//删除锁的编号,首先判断锁类型是否正确
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
// //此函数来查询锁用户剩余的存储空间
// void USB_info13(void)
// {
//    uchar i;
//    uchar delete_row,xor;
//    uint  total_row;
//    uchar  ack[7];  //回复的数据帧
//    xor =0;
// 	 Clear_WDT();
//    delete_row = ReadData(DETELE_ROW);
//    total_row= ReadData(KEY_YONGHU_ROW_H);   //已存储的组数
//    total_row =((total_row<<8)&0xff00)+ReadData(KEY_YONGHU_ROW_L);
//    total_row =(1000-total_row)+delete_row;  //剩余的存储空间
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
最多可以删除50组用户的信息
待测试
首先检测一下，删除的用户数据是否超过50组
**/
// void  Delete_LockNum(uchar buff[])
// {
//   uchar i,k,mid,row_len,len,nChar,j;
//   uchar delete_row,left_row;
//   uint  detele_addr;
//   Clear_WDT();
//   len =g_Buff[4];   //得到数据包的长度
//   row_len =len/4;
//   delete_row = ReadData(DETELE_ROW);
//   left_row =50 -delete_row;   //剩余的可删除的用户数
//   if( left_row <=row_len)
// 		row_len =left_row;
//   detele_addr= ReadData(DETELE_INDEX_H);   //已存储的组数
//   detele_addr =((detele_addr<<8)&0xff00)+ReadData(DETELE_INDEX_L);
//   Clear_WDT();
//   for(i =0;i<246;i++)
//    Ti[i] =ReadData(SANBIAO_START+i);	//开机的时候将，数据缓冲区给读出来
//   for( i =0;i<row_len;i++)
//   {
//     Clear_WDT();
//     for(k=0;k<4;k++)
// 			Lock_number[k] =g_Buff[7+4*i+k];   //数据包中有一个是锁的类型的字节
//     msDelay(500);
//     nChar =Delete_OneLockNum();
//     msDelay(500);
// 	  Clear_WDT();
//     if(nChar ==1)   //存储删除的锁号的地址
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
// 	WritEDAta(SANBIAO_START+i,Ti[i]);   //存储改变的散表结构

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
// 	WritEDAta(DETELE_INDEX_H,mid);  //存储头指针
//     for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(DETELE_ROW,delete_row);  //存储组数
//     for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	 Clear_WDT();
// }
/***
  首先检测此编号是否存在
  删除用户之后 将用户的连接从连接表中剔除
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
// 	WritEDAta(i2c_addr+5,0xfe);  //0xfe为注销标志位

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
  增加一个锁的编号
  是否检测锁的编号是否有重复存储的情况
  如果没有检测到此锁的编号的话 ，可以增加
*****/

// uchar Increase_Onelocknum(void)
// {
//   uchar nChar;
//    Clear_WDT();
//   nChar =Cheak_New_LockNum();  //检测锁的编号是否已经存在，如果存在则不再添加
//   if(nChar ==0)
//   {
//     Clear_WDT();
//    return 1;
//   }
//    Clear_WDT();
//   return 0;
// }
// /***
// 如何将新加入的锁的编号，写到连接表中
// 增加和删除的用户的最高上限为50户
// ***/
// void  increase_LockNum(uchar buff[],uchar de_len)
// {
//   uchar mid_buff[200],len,row_len,j;
//   uchar delete_row,now_addr;
//   uchar i,nChar,k;
//   uint i2c_addr;
//   uint  add =0;
//    Clear_WDT();
//   now_addr =0;   //取代数据的跳动指针
//   len =de_len;   //得到数据包的长度
//   row_len =len/4;
//   delete_row = ReadData(DETELE_ROW);
//   len =delete_row*2;
//    for(i =0;i<246;i++)
//    Ti[i] =ReadData(SANBIAO_START+i);	   //开机的时候将，数据缓冲区给读出来
//    for(i =0;i<len;i++)
//    mid_buff[i] =ReadData(DETELE_START+i);  //将删除的列表信息给读出来
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
// 	WritEDAta(SANBIAO_START+i,Ti[i]);   //存储改变的散表结构

//   }
//    Clear_WDT();
//   len =delete_row*2;
//   for(i =0;i<len;i++)
//   {
//          Clear_WDT();
//        	for(k =0;k<WRITE_ZHI;k++)
// 	msDelay(500);
// 	WritEDAta(DETELE_START+i,mid_buff[now_addr+i]);	     //有一个错误的地方，没有给出删除增加的地址指针
//   }
//     Clear_WDT();
//         i2c_addr = DETELE_START+len;   //得到现在的数据的位置指针
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
  分两帧
  第一帧：发送数据长度给后台
  第二帧：发送具体的数据给后台
*****/

// void  Send_YongHu(void)
// {
//   uint j;
//   uint  key_name,total_row,left_row;
//   uchar delete_row,i,nChar;
//   uchar shuzu[6];
//   uchar ack[7];
//   uchar xor =0;   //校验位
//   Clear_WDT();
//   key_name =  KEY_NAME_START;
//   total_row= ReadData(KEY_YONGHU_ROW_H);   //已存储的组数
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
// 	UARTnSendChar(1,ack,6);		//发送数据长度给后台
//   for( j=0;j<left_row;j++)		//将用户数据上传上位机
//   {
//     Clear_WDT();
//     for(i =0;i<6;i++)
// 			shuzu[i] = ReadData(key_name+i);
// 	  key_name =key_name+6;		//看来key_name的长度是6个字节
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









