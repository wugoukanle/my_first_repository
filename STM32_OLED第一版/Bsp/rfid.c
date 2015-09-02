  /***
	1.采用中断端口读取维根码的数据
	2.采用串口中断模式读取数据
  ****/

#include "rfid.h"
#include "usb.h"

UARTBUFF1  m_UB1;
unsigned char byte_cnt=0;
unsigned char bit_cnt=0;
unsigned int wg_data[4]={0xff,0xff,0xff,0xff};
unsigned int wg_buf[4]={0xff,0xff,0xff,0xff};
unsigned char wg_mask=0x80;
void resume_wg26_data(void);
unsigned char a;
void store_tag_bit(unsigned char tag_data_bit );

unsigned char tmr_wg; //定时器，5ms 没有收到信号，认为一帧信号结束
unsigned char port_temp;//端口电平记忆
unsigned char wg_cunt;//接维根数据收位数
union{
    unsigned long data;//维根数据
    unsigned char buff[4];
}wg;



/**
两个端口  输入口，必须接在中断脚上
中断模式 暂时不采用
**/

 void Rfid_IO_Init_In(void)
 {
		GPIO_InitTypeDef GPIO_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    //Key_IO--通信管脚，PA1，接EXITA，即WG0
		
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);    //Key_IO--通信管，PB1,接EXITB，即WG1
 }
  /****
   uart2 的配置

  ****/
void USART2_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate =  9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
 //同步通信时用的
 // USART_InitStructure.USART_Clock = USART_Clock_Disable;
 // USART_InitStructure.USART_CPOL = USART_CPOL_Low;
 // USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
 // USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

//  USART_StructInit(USART_InitStructure);
  USART_Init(USART2, &USART_InitStructure);
  /* Enable USART1 */
  USART_Cmd(USART2, ENABLE);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); /*接收中断使能*/
}

void UART2SendChar(char uart,char data)
{
	
	{  while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
 	   USART_SendData(USART2,data);
	   while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);  
	}
}

void UART2nSendChar(char uart,u8 *str,u8 len)
{
	u8 idx;
	{
		for (idx=0;idx<len;idx++)
		{
				USART_SendData(USART2,str[idx]);
	      while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
		}
	}
}


void resume_wg26_data(void)
{
  unsigned int i;
  wg_data[0] = wg_buf[0] << 1;
  wg_data[0] &=0xfe;
  i = wg_buf[1] >>7 & 0x01;
  wg_data[0] |= i;		//将2~9位存于wg_data[0],即HID的低八位
  
  wg_data[1] = wg_buf[1] << 1;
  wg_data[1] &=0xfe;
  i = wg_buf[2] >>7 & 0x01;
  wg_data[1] |= i;		//将10~17位存于wg_data[1]，即PID的高八位
  
  wg_data[2] = wg_buf[2] << 1;
  wg_data[2] &=0xfe;
  i = wg_buf[3] >>7& 0x01;
  wg_data[2] |= i;		//将wg_data存于wg_data[2]，即PID的低八位
}


void store_tag_bit(unsigned char tag_data_bit )		//这个函数应该是为中断准备的
{
        //  ( mask = 0x80 ; mask > 0 ; mask >>= 1 )   //1000 0000=0x80
        if( tag_data_bit == 1 )
					{wg_buf[byte_cnt] |= wg_mask;}		//置1
        else
					{wg_buf[byte_cnt] &=~wg_mask;}    //清0              //0111 1111=~0x80   =0        

				wg_mask>>= 1;		//wg_mask右移1位
				bit_cnt ++;
				
				if(wg_mask == 0)		//	如果wg_mask右移1位之后为0
				{
						wg_mask = 0x80;
						byte_cnt++; 
				}
								//
				if(bit_cnt==26)		//接受完26位之后，重新清理wg_buf为wg_data
				{
					resume_wg26_data();		//将26位韦根数的HID存入wg_data[0],PID存入wg_data[1~2]
					byte_cnt=0;
					wg_mask=0x80;
					bit_cnt=0;
					//for(a=0;a<4;a++)
					//{uart_transmit(wg_data[a]);}        
				}
}  





/*******
 串口2的接收中断函数
********/
// void USART2_IRQHandler(void)
// {
//   u8 nChar;
//   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//   {
//     nChar =USART_ReceiveData(USART2);
//     m_UB1.RBuff[m_UB1.cRHead] = nChar;		//将接受的字符放在缓冲区中
// 		++m_UB1.cRHead;
// 		if (m_UB1.cRHead >= UARTRXLEN1)
// 			m_UB1.cRHead = 0;		//让缓冲区成为一个环
//     USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//   }
// }


/***
  读取RFID串口发送过来的数据包,判断是否正确
  最少为5个字节,进行判断
  当接收缓冲区，接收到一定数目的字节时，进行判断
	数据的接受过程在USART2_IRQHandler（）中处理


	串口中断将接受到的数据全都放到缓冲区，数据长度不管；
	然后Read_RfidData（）函数通过检验查找ID数据的开头，
		如果不对就再向后移动一位，继续校验
		如果正确就将从此位子开始的5个字节放入pdst中

***/
uchar Read_RfidData(char uart, uchar *pdst, char max)
{
	//uart没有用到，直接用的是USART2
	
  uchar count = 0,total;
  uchar idx,off;
  
  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//关中断
  if(m_UB1.cRHead != m_UB1.cRTail)
  {
      //计算有效数据总长度
      if (m_UB1.cRHead >= m_UB1.cRTail)
      {
        total  = m_UB1.cRHead - m_UB1.cRTail;
      } 
			else
			{
        total  = UARTRXLEN + m_UB1.cRHead - m_UB1.cRTail ;
      }
      //找结束符
      if (total>=5)//帧长度至少为5，这里是正确的，保证缓冲区能容下ID
      {
        
						//数据长度满足一帧的要求
						total = 0;//纪录校验和
						for (idx=0; idx<5;idx++)
						{
							off = (m_UB1.cRTail + idx) % UARTRXLEN;		//偏移地址
							pdst[idx] = m_UB1.RBuff[off];		//将缓冲区数据放到目标数组中
							total ^= pdst[idx];//计算校验和，应该是偶校验
						}
						if (total==0)//校验正确
					  {
							m_UB1.cRTail = (off + 1) % UARTRXLEN;
						  count =5;
					  }
						else		//通过校验来找到ID数据的头，重点
						{
								//校验错误，抛弃1个字节，其实为本帧的头
								m_UB1.cRTail = (m_UB1.cRTail + 1) % UARTRXLEN;
								count = 0;
						}
       }
	 }
// 	 m_UB1.cRHead=0;
// 	 m_UB1.cRTail=0;
	 
	 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开中断
	 
	 
		if(count ==5) 
		{
			if(pdst[0]==0&&pdst[1]==0&&pdst[2]==0&&pdst[3]==0)
				count=0;		//此时的count=5无效
			else		//至少有一个不为零
			{
				UARTnSendChar(1,pdst,5);
				UART2Reset(0);
			}
		}	 
	 
	   
   return count;		//count要么为5要么为0，返回0表示校验失败


}



void UART2Reset(char uart)
{		
		//先关接受中断
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);            // Disable USART1 RX interrupt
		//UARTBUFF  m_UB0;
		m_UB1.cFlag = 0;
		m_UB1.cRHead = 0;
		m_UB1.cRTail = 0;
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            // Enable USART1 RX interrupt
}

