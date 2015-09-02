  /***
	1.�����ж϶˿ڶ�ȡά���������
	2.���ô����ж�ģʽ��ȡ����
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

unsigned char tmr_wg; //��ʱ����5ms û���յ��źţ���Ϊһ֡�źŽ���
unsigned char port_temp;//�˿ڵ�ƽ����
unsigned char wg_cunt;//��ά��������λ��
union{
    unsigned long data;//ά������
    unsigned char buff[4];
}wg;



/**
�����˿�  ����ڣ���������жϽ���
�ж�ģʽ ��ʱ������
**/

 void Rfid_IO_Init_In(void)
 {
		GPIO_InitTypeDef GPIO_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    //Key_IO--ͨ�Źܽţ�PA1����EXITA����WG0
		
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);    //Key_IO--ͨ�Źܣ�PB1,��EXITB����WG1
 }
  /****
   uart2 ������

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
	
 //ͬ��ͨ��ʱ�õ�
 // USART_InitStructure.USART_Clock = USART_Clock_Disable;
 // USART_InitStructure.USART_CPOL = USART_CPOL_Low;
 // USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
 // USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

//  USART_StructInit(USART_InitStructure);
  USART_Init(USART2, &USART_InitStructure);
  /* Enable USART1 */
  USART_Cmd(USART2, ENABLE);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); /*�����ж�ʹ��*/
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
  wg_data[0] |= i;		//��2~9λ����wg_data[0],��HID�ĵͰ�λ
  
  wg_data[1] = wg_buf[1] << 1;
  wg_data[1] &=0xfe;
  i = wg_buf[2] >>7 & 0x01;
  wg_data[1] |= i;		//��10~17λ����wg_data[1]����PID�ĸ߰�λ
  
  wg_data[2] = wg_buf[2] << 1;
  wg_data[2] &=0xfe;
  i = wg_buf[3] >>7& 0x01;
  wg_data[2] |= i;		//��wg_data����wg_data[2]����PID�ĵͰ�λ
}


void store_tag_bit(unsigned char tag_data_bit )		//�������Ӧ����Ϊ�ж�׼����
{
        //  ( mask = 0x80 ; mask > 0 ; mask >>= 1 )   //1000 0000=0x80
        if( tag_data_bit == 1 )
					{wg_buf[byte_cnt] |= wg_mask;}		//��1
        else
					{wg_buf[byte_cnt] &=~wg_mask;}    //��0              //0111 1111=~0x80   =0        

				wg_mask>>= 1;		//wg_mask����1λ
				bit_cnt ++;
				
				if(wg_mask == 0)		//	���wg_mask����1λ֮��Ϊ0
				{
						wg_mask = 0x80;
						byte_cnt++; 
				}
								//
				if(bit_cnt==26)		//������26λ֮����������wg_bufΪwg_data
				{
					resume_wg26_data();		//��26λΤ������HID����wg_data[0],PID����wg_data[1~2]
					byte_cnt=0;
					wg_mask=0x80;
					bit_cnt=0;
					//for(a=0;a<4;a++)
					//{uart_transmit(wg_data[a]);}        
				}
}  





/*******
 ����2�Ľ����жϺ���
********/
// void USART2_IRQHandler(void)
// {
//   u8 nChar;
//   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//   {
//     nChar =USART_ReceiveData(USART2);
//     m_UB1.RBuff[m_UB1.cRHead] = nChar;		//�����ܵ��ַ����ڻ�������
// 		++m_UB1.cRHead;
// 		if (m_UB1.cRHead >= UARTRXLEN1)
// 			m_UB1.cRHead = 0;		//�û�������Ϊһ����
//     USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//   }
// }


/***
  ��ȡRFID���ڷ��͹��������ݰ�,�ж��Ƿ���ȷ
  ����Ϊ5���ֽ�,�����ж�
  �����ջ����������յ�һ����Ŀ���ֽ�ʱ�������ж�
	���ݵĽ��ܹ�����USART2_IRQHandler�����д���


	�����жϽ����ܵ�������ȫ���ŵ������������ݳ��Ȳ��ܣ�
	Ȼ��Read_RfidData��������ͨ���������ID���ݵĿ�ͷ��
		������Ծ�������ƶ�һλ������У��
		�����ȷ�ͽ��Ӵ�λ�ӿ�ʼ��5���ֽڷ���pdst��

***/
uchar Read_RfidData(char uart, uchar *pdst, char max)
{
	//uartû���õ���ֱ���õ���USART2
	
  uchar count = 0,total;
  uchar idx,off;
  
  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//���ж�
  if(m_UB1.cRHead != m_UB1.cRTail)
  {
      //������Ч�����ܳ���
      if (m_UB1.cRHead >= m_UB1.cRTail)
      {
        total  = m_UB1.cRHead - m_UB1.cRTail;
      } 
			else
			{
        total  = UARTRXLEN + m_UB1.cRHead - m_UB1.cRTail ;
      }
      //�ҽ�����
      if (total>=5)//֡��������Ϊ5����������ȷ�ģ���֤������������ID
      {
        
						//���ݳ�������һ֡��Ҫ��
						total = 0;//��¼У���
						for (idx=0; idx<5;idx++)
						{
							off = (m_UB1.cRTail + idx) % UARTRXLEN;		//ƫ�Ƶ�ַ
							pdst[idx] = m_UB1.RBuff[off];		//�����������ݷŵ�Ŀ��������
							total ^= pdst[idx];//����У��ͣ�Ӧ����żУ��
						}
						if (total==0)//У����ȷ
					  {
							m_UB1.cRTail = (off + 1) % UARTRXLEN;
						  count =5;
					  }
						else		//ͨ��У�����ҵ�ID���ݵ�ͷ���ص�
						{
								//У���������1���ֽڣ���ʵΪ��֡��ͷ
								m_UB1.cRTail = (m_UB1.cRTail + 1) % UARTRXLEN;
								count = 0;
						}
       }
	 }
// 	 m_UB1.cRHead=0;
// 	 m_UB1.cRTail=0;
	 
	 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���ж�
	 
	 
		if(count ==5) 
		{
			if(pdst[0]==0&&pdst[1]==0&&pdst[2]==0&&pdst[3]==0)
				count=0;		//��ʱ��count=5��Ч
			else		//������һ����Ϊ��
			{
				UARTnSendChar(1,pdst,5);
				UART2Reset(0);
			}
		}	 
	 
	   
   return count;		//countҪôΪ5ҪôΪ0������0��ʾУ��ʧ��


}



void UART2Reset(char uart)
{		
		//�ȹؽ����ж�
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);            // Disable USART1 RX interrupt
		//UARTBUFF  m_UB0;
		m_UB1.cFlag = 0;
		m_UB1.cRHead = 0;
		m_UB1.cRTail = 0;
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            // Enable USART1 RX interrupt
}

