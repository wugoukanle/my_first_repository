#include "Key_Transfer.h"
#include "Timer.h"
#include "rtc_time.h"
#include "Li_power.h"
#include "usb.h" 
#include "spi_flash.h"
#include "rfid.h"
#include "delay.h"
#include "App.h"
#include "lcd.h"
#include "Globle_Var.h"


#define  MAX_BUFF      520     //255
#define  MAX_TI        246
#define  WRITE_ZHI     40

u8 data_state;
unsigned int h;
unsigned int l; 

unsigned char key_in_code[4];
extern unsigned char Lock_number[4];
extern uint8_t g_State;
extern uchar Rfid_Buff[6]; 
extern uchar g_Buff[MAX_BUFF];

extern uchar a_f[24];

 /*
 �������ͨ�ŵ�IO�ڵĳ�ʼ��
 ����IO������Ϊ�ߵ�ƽ״̬������⵽
 �����������Ӧת��Ϊ�͵�ƽ״̬,�Դ������
 ����״̬
*/
 void Key_IO_Init_In(void)
 {
 
 }
 void Key_IO_Init_Out(void)
 {
 
 }
 /**
�������ĵ�Դ���ƽ�
 **/
  void Key_Power_Init(void)
 {
  
 }
 /**
��������OE���ƽ�
 **/
  void Key_OE_Init(void)
 {
  
 }
/*
* function:Read_KeyInfo()
* ��ȡ�������ڲ�����Ϣ������
* 1 -��ȡ�ɹ�
* 0 -��ȡʧ��
* aa+75+01+DE
*/
uchar Read_KeyInfo(void)
{
  
  uchar nChar;
  uchar key_order[4] ={0xaa,0x75,0x01,0xde};
  Clear_WDT();
  Key_Sendata(key_order,4);
  KillTimer(TIMER_TIMEOUT);
  g_WaitTimeOut = 0;
  SetTimer(TIMER_TIMEOUT,1,Do_TimeOut,THREE_MIN);//�ȴ����������͹�����������Ϣ
  while(  g_WaitTimeOut == 0)
  {
    Clear_WDT();
		nChar = Re_leveldata(1);
		if(nChar ==0)
		break;
		else if(nChar ==1)
		break;
	
  }
  KillTimer(TIMER_TIMEOUT);
  if(g_WaitTimeOut == 1)
  {	 
     Clear_WDT();
		 return 0;
  }
  else if(nChar ==0)
  {
    Clear_WDT();
    return 0;
  }
  else if(nChar ==1)
  {
     Clear_WDT();
		 return 1;
  }
   Clear_WDT();
   return 0;
}
 /***********

ģ�⴮�ڵĶ�ʱ���ĳ�ʼ��
ʱ��Ҫ��Ƚ��ϸ�
***********/
 /***
void Second_WByte(uchar input)
{
       //������ʼλ
     uchar j=8;
	 uchar tempdata;
     //TR0=1;
     TXD_S_LOW;
     WaitTF0(TEN_MS);
	  WaitTF0(TEN_MS);
	   WaitTF0(TEN_MS);
     //����8λ����λ
     while(j--)
     {
			 TXD_S_LOW;
			 WaitTF0(TEN_MS);
			 tempdata =(input &0x01);
			 if(tempdata ==0x00)    //��Ϊ�߼�0
		 {
		 TXD_S_HIGH;
		 WaitTF0(SIX_MS);
		 TXD_S_LOW;
		 WaitTF0(FOUR_MS);
		 }
		 else if(tempdata ==0x01)  //��Ϊ�߼�1
		 {
		 TXD_S_HIGH;
		 WaitTF0(TEN_MS);
		 }
		 
         input=input>>1;
     }

     //����У��λ(��)

     //���ͽ���λ
     TXD_S_HIGH;
     WaitTF0(FIVE_MS);


}
  ***/
void  Second_WByte(uchar input)
{
     uchar j=8;
	 uchar tempdata;
     //TR0=1;
     TXD_S_LOW;
     WaitTF0(TEN_MS);
	  //WaitTF0(TEN_MS);
	   //WaitTF0(TEN_MS);
     //����8λ����λ
     while(j--)
     {
	     TXD_S_LOW;
         WaitTF0(TEN_MS);
	     tempdata =(input &0x01);
         if(tempdata ==0x00)    //��Ϊ�߼�0
		 {
		 TXD_S_HIGH;
		 WaitTF0(THREE_MS);
		 TXD_S_LOW;
		 WaitTF0(TOW_MS);
		 }
		 else if(tempdata ==0x01)  //��Ϊ�߼�1
		 {
		 TXD_S_HIGH;
		 WaitTF0(FIVE_MS);
		 }
		 
         input=input>>1;
     }

     //����У��λ(��)

     //���ͽ���λ
     TXD_S_HIGH;
     WaitTF0(FIVE_MS);


}

void WByte(uchar input)
{
     //������ʼλ
     uchar j=8;
	   uchar tempdata;
     //TR0=1;
     TXD_S_LOW;
     WaitTF0(FIVE_MS);
     //����8λ����λ
     while(j--)
     {
	     tempdata =(input &0x01);
       if(tempdata ==0x00)
					TXD_S_LOW;
			 else if(tempdata ==0x01)
			 TXD_S_HIGH;
		   WaitTF0(FIVE_MS);
       input=input>>1;
     }

     //����У��λ(��)

     //���ͽ���λ
     TXD_S_HIGH;
     WaitTF0(FIVE_MS);
     //TR0=0;
}    
 /*
  ����һ�����ݵĺ���

 */
void Key_Sendata(uchar dir[],uchar len)
{

     uchar i;
     for(i=0;i<len;i++)//���ѭ������������
     {
					Clear_WDT();
          WByte(dir[i]);
					msDelay(5);  //�����ʱ�Ƿ�����ٶ�Щ
     }
}

//����һ���ַ�
/*************
��������һ��Ϊ10λ��1λ��ʼλ��8λ����λ��1λ����λ
�ж���ʼλ��ʱ�򣬲���һ����ʱ����ֹ��������ѭ���ȴ�״̬
��������ʱ�򾡿��ܵ�ɨ��λ��ƽ���м䴦

______|----|_____|----|______|-----|_____
*************/
/***
 �����Ƿ���������
****/
uchar Test_RByte(void)
{
     uchar state=0;
		 KillTimer(TIMER_TIMEOUT);
  	 g_WaitTimeOut = 0;
     SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
     while( g_WaitTimeOut == 0 )  //�ȴ����ص�GPSͷ�ε��ֽ�
     {
			 Clear_WDT();
       if(RXD_S_DATA ==0)
			 {
					state =1;
					break;
			 }
     }
     KillTimer(TIMER_TIMEOUT);
	   Clear_WDT();
     return state;


}
uchar RByte(void)
{
     uchar Output=0 ;
	//for(i =0;i<8;i++)
	// rx_buff[i] =0x00;
   // while( RXD_S_DATA !=0);
	 Clear_WDT();
	 g_WaitTimeOut = 0;
     SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
     while( g_WaitTimeOut == 0 )  //�ȴ����ص�GPSͷ�ε��ֽ�
     {
	   Clear_WDT();
       if(RXD_S_DATA ==0)
	   break;
     }
     KillTimer(TIMER_TIMEOUT);
	 if(g_WaitTimeOut == 1)
	 {
	  Clear_WDT();
	  data_state = 1;//RED_BIT_HIGH;while(1);
	  return  0xff; 
	 }
	  Clear_WDT();
//	 OCR0 = 145;
	  WaitTF0(OVER_MS);//�ȹ���ʼλ
   // RED_BIT_HIGH;while(1);
     //����8λ����λ
	// OCR0 =90;
	 /*********���Գ����*****/
	  if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  //Output=>>1;
		 // rx_buff[0]=rx_buff[0]>>6;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>5;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>4;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>3;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>2;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>1;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//���յ�λ
	  }
	  Clear_WDT();
      //WaitTF0(0X50);//λ����ʱ,�Ƿ�ȡ����������⣬���һλ
	  while(RXD_S_DATA ==0)
	  {
	   Clear_WDT();
	  }
	  if(RXD_S_DATA ==(GPIO_Pin_13))
	 {
	   //EEPROM_write(0,Output);
	   //RED_BIT_HIGH;while(1);
	   Clear_WDT();
	   return Output;
	 }
	  Clear_WDT();
      return 0xff;
}
/***************
�Ƿ񽫽��յ�һ���ֽ� ����������ֽ����ֿ�
���Խ��յڶ����ֽڵĺ���
***************/
uchar RByte1(void)
{
     uchar Output=0 ;
//	 uchar rx_buff[8];
//     uchar i=7 ;
	// uchar j =0;
	 while(RXD_S_DATA !=0)
	 {
	   Clear_WDT();
	 }
	 //RED_BIT_HIGH; while(1);
	 /***
	 for(i =0;i<8;i++)
	 rx_buff[i] =0x00;
	 g_WaitTimeOut = 0;
     SetTimer(TIMER_TIMEOUT,1,Do_TimeOut,THREE_MIN);
     while( g_WaitTimeOut == 0 )  //�ȴ����ص�GPSͷ�ε��ֽ�
     {
       if(RXD_S_DATA ==0)
	   break;
     }
     KillTimer(TIMER_TIMEOUT);
	 if(g_WaitTimeOut == 1)
	 {
	  data_state = 1;//RED_BIT_HIGH;while(1);
	  return  0xff; 
	 }
	 *****/
	 //OCR0 = 145;
	  Clear_WDT();
	  WaitTF0(0x0200);//�ȹ���ʼλ
  //  RED_BIT_HIGH;while(1);
     //����8λ����λ
	 //OCR0 =90;
	  Clear_WDT();
	 /*********���Գ����*****/
	  if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  //Output=>>1;
		 // rx_buff[0]=rx_buff[0]>>6;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>5;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>4;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>3;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>2;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//���յ�λ
		  //rx_buff[0]=rx_buff[0]>>1;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//λ����ʱ
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//���յ�λ
	  }
     // WaitTF0();//λ����ʱ
	  while(RXD_S_DATA ==0)
	  {
	   Clear_WDT();
	  }
	  if(RXD_S_DATA ==(GPIO_Pin_13))
		{
	   Clear_WDT();
	   return Output;
		}
	 /**
	  if(RXD_S_DATA ==(1<<DDE0))
	 {
	   EEPROM_write(1,Output);
	   
	   return Output;
	 }
	 ***/
	  Clear_WDT();
      return 0xff;
}
/******
 *��Կ�ױ��
 *4���ֽ�8λ
 *��Ӧ֡
 * ���յ�����������������Ϣ����ʱ��Ϊ8���ֽ�
 * 55+7a+01+���_1+���_2+���_3+���_4+xor(������) 
 *level_data[2],level_data[3],level_data[4],level_data[5]�洢��Ϊ�������ı��
********/
uchar Re_leveldata(uchar data_bit)
{
/**/
  uchar level_data[8];
  uchar nChar;
  uchar i;//��һ����ʱ���������г�ʱ����
  uchar xor =0;
  data_state = 0;  //�Ƚ�״̬�趨Ϊ0
  Clear_WDT();
  nChar =RByte();
  if(data_state ==1)
  {
    Clear_WDT();
    return 0;
  }
  for(i =0;i<7;i++)
  {
   Clear_WDT();
   level_data[i] =RByte();  //ȫ��������16������֮�����ж�
   if(data_state ==1)
   {
    Clear_WDT();
    return 0;
   }
  }
  if((nChar !=0x55) || (level_data[0] !=0x7a))
  {
    Clear_WDT();
    return 0;
  }
  else
  {
   xor =xor^nChar;
   for(i =0;i<6;i++)
   xor=xor^level_data[i];
   if(xor !=level_data[6])
   {
    Clear_WDT();
    return 0;
   }
  }
   Clear_WDT();
   return 1;
   /***/
}
//initialize the X/Y module
void Init_levelModule(void)
{


}
// Read the X/Y data 
void Read_LevelData(void)
{
 
}

/***
 ��ȡ��������ǰ���뼰���ı�� ,����յ�ǰ���룬
 ���жϷ��͹��������ı���ڱ�Կ�׵Ŀ��Ʒ�Χ���������
 ���������ȡ�Է����͵�ǰ���룬
 ������������ڱ�Կ�׹���ķ�Χ�ڣ���δ���
 �Ƿ�������ϵ綶�����
 ��Ҫ��ͣ��ת��IO�ڵĹ���ģʽ������������֮�������л�
 �ڵ������Ľ�������Ϊ��ѭ���ṹ�����ͨ���쳣����
 ����Կ�׽��жԵ������Ķϵ�����������
 ��ȡ�����ı�ź���Ҫ�ж��Ƿ���Կ�׵Ĺ���Ȩ��֮�ڣ���������������ڴ�Կ�׵Ĺ���Χ�� ��δ���

 ǰ���Ƿ��һ���ж��Ƿ���������ĺ�������
****/
/***


***/
//�������ݴ�����
void Key_Trismit(void)
{
	 //uchar  Rfid_Code[4];  //RFID���뻺����
	 OLED_Clear();
	 Clear_WDT();
	 //often_show(16,56,"���Կ����Ϣ..",0,0x0fff);
	 often_show(16,40,"reading RFID",0,0x0fff);
	 Read_LeadCode();  	       //��ǰ���룬�Զ�ȡ����RFID���ݽ��з�������
	 Clear_WDT();
	 g_State =ST_IDLE;
	 return;
}


/***
 ��RFID�Ķ������������������������ݣ��жϱ���Ƿ��ڿ��Ƶķ�Χ֮��
 
 ���Ƕ�RFID

****/
uchar  Read_FirstCode(void)
{
	uchar nChar,state;
	often_show(36,56,"��ȡԿ����Ϣ..",0,0x0fff);		//������������Կ��
 	while(1)
	{
		 Clear_WDT();
		 g_WaitTimeOut = 0;
		 SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
		 while( g_WaitTimeOut == 0 )  //�ȴ����ص�GPSͷ�ε��ֽ�
	   {
	     Clear_WDT();
			 nChar =Read_RfidData(1,Rfid_Buff,0xff);		//�Ӵ��ڶ�ȡ�����������Ŀ�Ƭ����Ϣ
			 if(nChar >=3)
					break; 
     }		//�˳����whileѭ���������������1��break�жϲ��˳�ѭ������ʱg_WaitTimeOut == 0��δ��ʱ��2��g_WaitTimeOut=true,����ʱ
	   KillTimer(TIMER_TIMEOUT);
	   if( g_WaitTimeOut == 0)		//��ʾ�ǵ�һ�����
	   {
			 state =1;
			 Clear_WDT();
			 return state;		//һֱ��ֱ��
	   }
	}
}
/**

��ʽ��ȡ���У�EEPROM�У�����Ϣ�����ж����Ƿ����Ҫ��

3���ֽڵ�������λ��
1���ֽڵ�����������Ϣ

����ȡEEPROM��������λ����������Ͳ���key_in_code[4]�Ƚ�

**/

uchar Cheak_Lock_Local(void)
{
   uchar local_buff[4];
   uchar i;
//   uchar state =0;
   Clear_WDT();
   local_buff[0] =ReadData(LOCK_TYPE);		//���������ͣ�1���ֽ�
   for(i =0;i<3;i++)
			local_buff[1+i] = ReadData(LOCK_LOCAL+i);		//���������ţ�3���ֽ�
   Clear_WDT();
   if(local_buff[0] ==key_in_code[0])
   {
      if((local_buff[1] ==key_in_code[1])&&(local_buff[2] ==key_in_code[2])&&(local_buff[3] ==key_in_code[3]))
      {
				 Clear_WDT();
				 return 1;

      }
      else 
      {
				 Clear_WDT();
				 return 0;
      }
   }
   else
   {
      Clear_WDT();
      return 0;
   }
//	Clear_WDT();
  // return 0;


}
/**
	��ȡǰ����

**/
void Read_LeadCode(void)
{
   uchar i,nChar;
	 //uchar flag=0;
	
//    for(i =0;i<4;i++)		//Lock_number[4]��Rfid_Buff[6]����ȫ�ֱ���
//      Lock_number[i] =Rfid_Buff[i]; //��ST_Idle()���Ѿ������ݷ���Rfid_Buff[]
	
// 		while(1)
// 		{

// 				nChar =Read_RfidData(2,Rfid_Buff,0xff);

// 				if(nChar ==5) 
// 				{
// 					if(Rfid_Buff[0]==0&&Rfid_Buff[1]==0&&Rfid_Buff[2]==0&&Rfid_Buff[3]==0)
// 						;
// 					else		//������һ����Ϊ��
// 					{
// 						UARTnSendChar(1,Rfid_Buff,5);
// // 						flag=flag+1;
// // 						if(flag==2)
// 							break;
// 					}
// 				}
// 		}	
// 				UARTnSendChar(1,"55555",5);					

//��ST_Idle�������Ѿ���⵽��RFID���Ż��������

   for(i =0;i<4;i++)		//Lock_number[4]��Rfid_Buff[6]����ȫ�ֱ���
     Lock_number[i] =Rfid_Buff[i]; //��ST_Idle()���Ѿ������ݷ���Rfid_Buff[]	
	
	
	//����RFID���ı��,��UART2�����жϣ���RFID���ݷŵ��ŵ���������
	//����read_RfidData�����ݶ���Rfid_Buff[],���������ST_Idle�����е��ù�
   Clear_WDT();
   // if(xor ==g_Buff[8])  //���յ���ȷ�����ݵ�ʱ�򣬷���ȷ�����������
	    
			 
		//nChar= Cheak_New_LockNum();//Cheak_lock_Num();���Ҳ��һ���ؼ���
		nChar= check_suoma();		//ƥ���Ϊ1����ƥ���Ϊ0	
		//UARTnSendChar(1,&nChar,1);			 
		UARTnSendChar(1,"22222",5);				  
		//nChar =1;		//ֻ�ж�ȡ����������Ȩ�ļ��У��ŷ���1
		if(nChar ==0)
		{
			OLED_Clear();
			often_show(36,56,"δ��Ȩ",0,0x0fff);						
			
			delay_ms(1500);
			Clear_WDT();
			
			g_State =ST_IDLE;
			shouquan_jieguo=2;		//������Ȩ����Ҫ��״̬���			
			
		  OperateKey();                        //�����뼰����״̬д��洢оƬ��
			
			g_State =ST_IDLE;
			shouquan_jieguo=2;		//������Ȩ����Ҫ��״̬���
			return;
		}		
		else if(nChar==1)
		{ 				
			Clear_WDT();
			//Lcd_Show_IDinfo();  //��ʾ�򿪵�����ID��Ϣ	
			often_show(16,64,"��������ID:",0,0x0fff);
      display_shuzi(32,80, Lock_number, Green, Yellow);

			KEY_POWER_ON;
			often_show(36,100,"����Ȩ",0,0x0fff);					
			
			delay_ms(1500);
			
			shouquan_jieguo=1;
			//Order_Transmit();  //ͨ��ָ��Ե��������и��ֲ���
			OperateKey();                        //�����뼰����״̬д��洢оƬ��
			
			KEY_POWER_OFF;//�ر�SPX2941��Դ�����ر�Կ�׶�������
			
			UARTnSendChar(1,"33333",5);						
			g_State =ST_IDLE;
			Clear_WDT();
			return;
		}
		
}
/****
 �������͵���Կ��֮�������ͨ��

 �ڴ�ʱIO���Ѿ������״̬������Ҫ�����趨
 ����Կ�״���֮������ʱһ��ʱ�䷵�ؿ���״̬

 �������յ��򿪵�ָ�����������޵Ŀղ���
 �ȴ�������--------�ϵ�
****/
void Order_Transmit(void)
{

  Clear_WDT();
  {
			KEY_POWER_ON;		 //��SPX2941��Դ����TPS79633��SP-EN��J8���书�ܾ��ǿ���
			{
				Clear_WDT();
				//Save_Lock_doing();							   //�����뼰����״̬д��洢оƬ��
				OperateKey();                        //�����뼰����״̬д��洢оƬ��
			}
			KillTimer(TIMER_TIMEOUT);                         //����һ��ض�ʱ��
			/**/
			Clear_WDT();
			g_WaitTimeOut = 0;
			SetTimer(TIMER_TIMEOUT,20,Do_TimeOut,THREE_MIN);   //�ȴ�Կ�װγ��Ĺ���  ��ʱ����Ҫ���¼���
			while(g_WaitTimeOut == 0)
			{
				Clear_WDT();
			}
			KillTimer(TIMER_TIMEOUT);
			/***/
			KEY_POWER_OFF;//�ر�SPX2941��Դ�����ر�Կ�׶�������
			g_State =ST_IDLE;		//���ؿ���״̬
			Clear_WDT();
			return;
   }
}
     
/***
 ���ӵ�д������λ��ĺ���
****/ 
void  Write_LockNum(void)
{
		uchar nChar;
		Clear_WDT();
		nChar = Read_FirstCode();
		if(nChar ==0)		//��ʾû�ж������ģңƣɣ�
		{
			Clear_WDT();
			g_State =ST_IDLE;
			return;
		}
		Clear_WDT();
		Write_code();
		Clear_WDT();
		g_State =ST_IDLE;
}   


void  Write_code(void)
{
		uchar i,nChar,j;
		uchar xor =0;
		j =0;
		Clear_WDT();
		KEY_POWER_OFF;
		//Shengya_Power_Off();		//��ʹ��TPS97633����J8�����
		for(i =0;i<0xff;i++)
			msDelay(0xfff);
		TXD_S_HIGH;
		Key_IO_Init_In();  //��IO����Ϊ�����
		msDelay(0xff);
		KEY_POWER_ON;
		//Shengya_Power_On();
		Clear_WDT();
		msDelay(0xff);//
		Read_New_Code();
		Clear_WDT();
		while(j<10)
		{
			Clear_WDT();
			j++;
			nChar  = Read_New_One();
			if(nChar ==0x55)
			{
				for(i =0;i<9;i++)
					g_Buff[i] =Read_New_One();
				break;
			}
		}
		if(j >=10)        //�����ʱû���յ�ǰ����Ļ����򷵻�
		{
			Clear_WDT();
			g_State =ST_IDLE;
			return;
		}
		Clear_WDT();
		h =0; l=0;
		xor =xor^nChar;
		for(i =0;i<8;i++)
		xor =xor^g_Buff[i];
		for(i =0;i<4;i++)
		Lock_number[i] =g_Buff[i]; //�������ı��
		for(i =0;i<4;i++)
		key_in_code[i] =g_Buff[4+i]; //�洢������λ�뼰����
		Clear_WDT();
		if(xor ==g_Buff[8])  //���յ���ȷ�����ݵ�ʱ�򣬷���ȷ�����������
		{
			Clear_WDT();
			Key_IO_Init_Out();
			TXD_S_HIGH;

			 for(i =0;i<0xff;i++)
			 {
				 Clear_WDT();
				 msDelay(0xff);    //��IO��ת��Ϊ���̬������ȷ������Է�
				 msDelay(0xff); 
				 msDelay(0xff); 
				 msDelay(0xff); 
				 msDelay(0xff); 
			 }
			 //WByte(0xaa);
			 Second_WByte(0xaa);
			 Clear_WDT();
			 /***/
			 Write_order();  //ͨ��ָ��Ե��������и��ֲ���
			 g_State =ST_IDLE;
			 Clear_WDT();
			 return;
			 
		}
		else
		{
			Clear_WDT();
			g_State =ST_IDLE;
			return;
		}

}

//Save_Lock_doing
/**
Ч����Ϊ���Ľ��
0x01 --����ָ��
0x02 --д�����
0X03 --д������λ��
**/
void Write_order(void)
{
  uchar i;
  uchar lock_data[10];
  Clear_WDT();
  lock_data[0] =0xaa;
  lock_data[1] =0x01;
  lock_data[2] =0x00;
  lock_data[3] =0x00;
  lock_data[4] =0x00;
  lock_data[5] =0x00;
  lock_data[6] =0xab;
 
  Key_IO_Init_Out();
  TXD_S_HIGH;
  for(i =0;i<0xff;i++)
	 {
	 Clear_WDT();
	 msDelay(0xff);    //��IO��ת��Ϊ���̬������ȷ������Է�
	 }
	 //while(1)
	  //Second_WByte( 0xaa);
  Clear_WDT();
  for(i =0;i<7;i++)
  Second_WByte( lock_data[i]);
  Clear_WDT();
 }
/****
������Ƿ���Կ�׵Ĺ���Χ��
lock_num1 --- ���ֵ 
lock_num2 --- ��Сֵ
lock_num3 --- ��ȡ�������ı��ֵ
50���û���Ϊ��С�Ĳ�ѯ�������Ҫ���Կ�Ч�����
 ����500�û��պô�����������������500�� ����δ���
 ��һ�ַ����� ����500���Ĳ��뵽500���м��
****/

// uchar Cheak_New_LockNum(void)
// {
//    uchar i,nChar;
//    uchar Mid_text[6];
//    uint i2c_addr;		//����ת�ַ��ĵ�ַ
//    uint  add =0;
//    /**
//    Lock_number[0] =0x00;
//    Lock_number[1] =0x00;
//    Lock_number[2] =0x00;
//    Lock_number[3] =0x20;
//    ***/
//    Clear_WDT();
//    for(i =0;i<4;i++)
// 			add =add+ Lock_number[i];
//    i =Mode(add);
//    i=i*2;	
//    i2c_addr =Ti[i+1];		//Ti��main()�����г�ʼ��ʱ��SANBIAO_START�ж�������Ti
//    i2c_addr =((i2c_addr <<8)&0xff00)+Ti[i];
//    MI_High_Last =Ti[i+1];
//    MI_Low_Last  =Ti[i];
// 	
//    MI_High =MI_High_Last;//Mid_text[5];
//    MI_Low =MI_Low_Last ;//Mid_text[4];
//    Ti_INDEX =i;
//    Clear_WDT();
//    if((MI_High_Last ==0xff)&&(MI_Low_Last ==0xff)) //|| ((MI_High_Last ==0xfe)&&(MI_Low_Last ==0xfe)))
//    {
// 		 Clear_WDT();
//      return 0;
//    }
// 	 
// 	 
// 	 //��ȡ��Ȩ
//    for(i =0;i<6;i++)
// 			Mid_text[i] = ReadData(i2c_addr+i);//�ⲿeeprom��ͨ��SPI����ȡ����
// 	 
// 	 
// 	 
// 	 
//    if((Mid_text[4] ==0xfe) &&(Mid_text[5] ==0xfe))  //���ע�������
//    {
// 			Clear_WDT();
// 			return 0;
//    }
//    if((Mid_text[0] ==Lock_number[0])&&(Mid_text[1] ==Lock_number[1])&&(Mid_text[2] ==Lock_number[2])&&(Mid_text[3] ==Lock_number[3]))
//    {
// 		 Clear_WDT();
// 		 return 1;
//    }

//    MI_High =Mid_text[5];
//    MI_Low =Mid_text[4];
//    Clear_WDT();
//    nChar = XunHuan();			//Ϊʲô����Ҫѭ���ļ����
//    if(nChar ==1)
//    {
// 		 Clear_WDT();
// 		 return 1;
//    }
//    else
//    {
// 		 Clear_WDT();
// 		 return 0;
//    }
// }



/*

�Զ��庯��
����ȡ���������Ƿ�����Ȩ�ļ�����

*/
// uchar check_suoma(void)
// {
// 		//uchar i,j,k,max_row;
// 		//uchar known_suoma[4];
// 		//uint addr,basic_addr;


// // 		uint addr2;
// // 	
// // 		addr= ReadData(AUTHOR_START);   
// // 		Clear_WDT();
// // 		msDelay(500);
// // 	
// // 		max_row=ReadData(AUTHOR_ROW_L); 
// // 	
// // 		Clear_WDT();
// // 		msDelay(500);
// 	

// 	
// // 		UARTnSendChar(1,&max_row,1);				
// // 		UARTnSendChar(1,"66666",5);			


// 				
// 		if(Rfid_Buff[0]==a_f[4]&&Rfid_Buff[1]==a_f[5]&&Rfid_Buff[2]==a_f[6]&&Rfid_Buff[3]==a_f[7])
// 			return 1;		//�����˾ͷ���1
// 		else		
// 			return 0;

// }



/*

�Զ��庯��
����ȡ���������Ƿ�����Ȩ�ļ�����

*/
uchar check_suoma(void)
{
		uchar i,j,total_row;
		uchar known_suoma[4];
		uint addr_end,basic_addr;

		//addr= ReadData(AUTHOR_START);  

		addr_end= ReadData(AUTHOR_POINT_H);   
		addr_end =((addr_end<<8)&0xff00)+ReadData(AUTHOR_POINT_L);		//�ڽ���Ĳ��빦�����ı����ָ���ֵ
		
		total_row=(addr_end-AUTHOR_START)/24;
		
		if(total_row==0)
		{
			UARTnSendChar(1,"00000",5);		//���total_row=0��	
			return 0;
		}
		Clear_WDT();
		msDelay(500);
		for(i=0;i<total_row;i++)		//
		{		
				basic_addr=AUTHOR_START+i*24;
					
				for(j=0;j<4;j++)
				{
						msDelay(250);					
						known_suoma[j]= ReadData(basic_addr+4+j); 
						//msDelay(250);
				}
				Clear_WDT();
				
				if(Lock_number[0]==known_suoma[0]&&Lock_number[1]==known_suoma[1]&&Lock_number[2]==known_suoma[2]&&Lock_number[3]==known_suoma[3])
				{
					
					return 1;		//�����˾ͷ���1
				}
		}
		
		return 0;

}





/**
 ��������ȫ�ֱ���
MI_High
MI_Low
�Ƿ�Ҳ��Ҫ��һ����ʱ���ж�
**/
// uchar  XunHuan(void)
// {
// 	 uchar Mid_text[6];
// 	 uint  i2c_addr;
// 	 uchar i;
// 	 KillTimer(TIMER_TIMEOUT);
// 	 g_WaitTimeOut = 0;
// 	 SetTimer(TIMER_TIMEOUT,50,Do_TimeOut,THREE_MIN);
// 	 while(g_WaitTimeOut == 0)
// 	 {
// 		Clear_WDT();
// 		if( (MI_High !=0xff)||(MI_Low !=0xff))
// 		{
// 			Clear_WDT();
// 			i2c_addr =MI_High;
// 			i2c_addr =((i2c_addr <<8)&0xff00) +MI_Low;
// 			
// 			for(i =0;i<6;i++)
// 					Mid_text[i] = ReadData(i2c_addr+i);
// 			
// 			if((Mid_text[0] ==Lock_number[0])&&(Mid_text[1] ==Lock_number[1])&&(Mid_text[2] ==Lock_number[2])&&(Mid_text[3] ==Lock_number[3]))
// 			{
// 				 Clear_WDT();
// 				 KillTimer(TIMER_TIMEOUT);
// 				 return 1;
// 			}
// 			MI_High_Last =MI_High;
// 			MI_Low_Last  =MI_Low;
// 			MI_High =Mid_text[5];
// 			MI_Low =Mid_text[4];
// 		}
// 		else if( (MI_High ==0xff)&&(MI_Low ==0xff) )
// 		{
// 			 Clear_WDT();
// 			 KillTimer(TIMER_TIMEOUT);
// 			 return 0;
// 		}
// 	 }
// 	 Clear_WDT();
// 	 KillTimer(TIMER_TIMEOUT);
// 	 return 0;
// }


uchar Cheak_lock_Num(void)
{
  
//  uchar type[4];  
  uchar lock_num1[4],lock_num2[4],lock_num3[4];
  uchar i; 
  uchar nChar=1;
  uchar term =1;
  /*
  {				       //���������λ��������Ƿ����Ҫ��
		for(i =0;i<3;i++)
			type[i] =ReadData(LOCAL_NUM+i);
		type[3] =ReadData(KEY_CODE);
		if((type[0] !=key_in_code[0])|| (type[1] !=key_in_code[1])||(type[2] !=key_in_code[2])||(type[3] !=key_in_code[3]))
		{
			 return 0;
		}
  }
  */
  {
		Lock_number[0] =0x00;
		Lock_number[1] =0x00;	   //���Բ���
		Lock_number[2] =0x06;
		Lock_number[3] =0x50;
  }
  for(i =0;i<4;i++)
		lock_num3[i] =Lock_number[i];
  for(i =0;i<4;i++)
		lock_num1[i] =ReadData(KEY_NAME_MAX+i);
  nChar  =Float_subtract(lock_num1,lock_num3,term);	  //�Ƚ����ֵ
  if(nChar ==0)
		return 0;
  term =2;
  for(i =0;i<4;i++)
		lock_num2[i] =ReadData( KEY_NAME_MIN+i);
  nChar  =Float_subtract(lock_num3,lock_num2,term);	 //�Ƚ���Сֵ
  if( nChar ==0)
		return 0;
  term =3;
  for(i =0;i<4;i++)
		lock_num1[i] =ReadData(KEY_NAME_TEN+i);
  nChar  =Float_subtract(lock_num3,lock_num1,term);
  if(nChar ==0)       //С���м�ֵ
  {
//    nChar =Down_Midle();
		if(nChar ==1)
		{
		 return 1;
		}
	/**
     term =3;
	 for(i =0;i<4;i++)
     lock_num1[i] =ReadData(KEY_NAME_FIVE+i);
	 nChar =Float_subtract(lock_num3,lock_num1,term);
	 if(nChar ==0)	 //<5
	 {
	    term =3;
	    for(i =0;i<4;i++)
        lock_num1[i] =ReadData(KEY_NAME_SECOND+i);
	    nChar =Float_subtract(lock_num3,lock_num1,term);
        if(nChar ==0)  //<2
		{
		   term =3;
		   for(i =0;i<4;i++)
		   lock_num1[i] =ReadData(KEY_NAME_FIRST+i);
		   nChar =Float_subtract(lock_num3,lock_num1,term);
		   if(nChar ==0)  //<1
		   {
			  nChar =compare_Lock_num(KEY_NAME_INDEX_ZERO,lock_num3);
			  {
			   return nChar;
			  }
		   }
		   else if(nChar ==3)  //>1
		   {
			  nChar =compare_Lock_num(KEY_NAME_INDEX_FIRST,lock_num3);
			  {
			    return nChar;
			  }
		   }
		   
		  
		}
		else if(nChar ==3)  //>2
		{
		   nChar =compare_Lock_num(KEY_NAME_INDEX_SECOND,lock_num3);
		   {
			 return nChar;
		   }

		}

	 }
	 else if(nChar ==3)	//>5
	 {
	    for(i =0;i<4;i++)
        lock_num1[i] =ReadData(KEY_NAME_EIGHT+i);
	    nChar =Float_subtract(lock_num3,lock_num1,term);
        if(nChar ==0) //<8
	    {
		 for(i =0;i<4;i++)
         lock_num1[i] =ReadData(KEY_NAME_SEVEN+i);
	     nChar =Float_subtract(lock_num3,lock_num1,term);
		 if(nChar ==0) //<7
		 {
			 for(i =0;i<4;i++)
             lock_num1[i] =ReadData(KEY_NAME_SIX+i);
	         nChar =Float_subtract(lock_num3,lock_num1,term);
			 if(nChar ==0)
			 {
			  nChar =compare_Lock_num(KEY_NAME_INDEX_FIVE,lock_num3);
			  {
				return nChar;
			  }
			 }
			 else if(nChar ==3)
			 {
			   nChar =compare_Lock_num(KEY_NAME_INDEX_SIX,lock_num3);
			   {
				  return nChar;
			   }

			 }

		 }
		 else if(nChar ==3)	//>7
		 {
		   nChar =compare_Lock_num(KEY_NAME_INDEX_SEVEN,lock_num3);
		   {
			 return nChar;
		   }

		 }

	 }
	 else if(nChar ==3)	  //>8
	 {
		 	 for(i =0;i<4;i++)
             lock_num1[i] =ReadData(KEY_NAME_NINA+i);
	         nChar =Float_subtract(lock_num3,lock_num1,term);
			 if(nChar ==0)	//<9
			 {
			   compare_Lock_num(KEY_NAME_INDEX_EIGHT,lock_num3);
			   {
				 return nChar;
			   }

			 }
			 else if(nChar ==3)	//>9
			 {
			   compare_Lock_num(KEY_NAME_INDEX_NINA,lock_num3);
			   {
				 return nChar;
			   }

			 }

	 }


	 }

	 ****/
  }
  else if(nChar ==3)  //�����м�ֵ
  {
     //nChar = Up_Midle();
	 if(nChar ==1)
	 {
	   return 1;
	 }
     /*
	 for(i =0;i<4;i++)
     lock_num1[i] =ReadData(KEY_NAME_EIGHT+i);
	 nChar =Float_subtract(lock_num3,lock_num1,term);
     if(nChar ==0) //<8
	 {
		 for(i =0;i<4;i++)
         lock_num1[i] =ReadData(KEY_NAME_SEVEN+i);
	     nChar =Float_subtract(lock_num3,lock_num1,term);
		 if(nChar ==0) //<7
		 {
			 for(i =0;i<4;i++)
             lock_num1[i] =ReadData(KEY_NAME_SIX+i);
	         nChar =Float_subtract(lock_num3,lock_num1,term);
			 if(nChar ==0)
			 {
			  nChar =compare_Lock_num(KEY_NAME_INDEX_FIVE,lock_num3);
			  {
				return nChar;
			  }
			 }
			 else if(nChar ==3)
			 {
			   nChar =compare_Lock_num(KEY_NAME_INDEX_SIX,lock_num3);
			   {
				  return nChar;
			   }

			 }

		 }
		 else if(nChar ==3)	//>7
		 {
		   nChar =compare_Lock_num(KEY_NAME_INDEX_SEVEN,lock_num3);
		   {
			 return nChar;
		   }

		 }

	 }
	 else if(nChar ==3)	  //>8
	 {
		 	 for(i =0;i<4;i++)
             lock_num1[i] =ReadData(KEY_NAME_NINA+i);
	         nChar =Float_subtract(lock_num3,lock_num1,term);
			 if(nChar ==0)	//<9
			 {
			   compare_Lock_num(KEY_NAME_INDEX_EIGHT,lock_num3);
			   {
				 return nChar;
			   }

			 }
			 else if(nChar ==3)	//>9
			 {
			   compare_Lock_num(KEY_NAME_INDEX_NINA,lock_num3);
			   {
				 return nChar;
			   }

			 }

	 }
	 ***/


  }

	return 0;

}








 /***
 ��50���û��н��жԱȣ��ж� ��С�ļ��
 ����鵽�д����ı���򷵻�1
 ���򷵻�0
 ****/
uchar compare_Lock_num(uint i2caddr ,uchar str[])
{
  uchar i,j,k;
  uchar mid_buff[4];
  uchar state =0;
  Clear_WDT();
  for(i =0;i<200;i++)
  g_Buff[i] =ReadData(i2caddr+i);
  for(j =0;j<50;j++)
  {
     Clear_WDT();
     k =4*j;
  	 for(i =0;i<4;i++)
	 mid_buff[i] =g_Buff[k+i];
	 if( (str[0] ==mid_buff[0])&&(str[1] ==mid_buff[1])&&(str[2] ==mid_buff[2])&&(str[3] ==mid_buff[3]))
	 {
       Clear_WDT();
	   state =1;
	   return state;
	 }
  } 
   Clear_WDT();
  return state;


}
/*****
������ı���Ƿ���Կ�׿��Ƶķ�Χ��
��Կ���ڲ������ı�ŵ�����
����������ģ�����Ǵ�С�����˳��
�ȱȽ� ������С Ȼ��Ƚ� ��С���м�  �����м�
str2 <str1ʱ�����ص�ֵ��Ϊ0
str1 = 00 00 02 46
str2 = 00 00 01 01
term =1;
*****/

uchar Float_subtract(uchar str1[],uchar str2[],uchar term) //ok
{
  uchar State =0;
  if(str2[0] <str1[0])  //��һ��������������ֳ���
  {
   State =term;
   return State;
  }
  else if((str2[0] == str1[0])&&(str2[1] <str1[1]))
  {
     State =term;
     return State;
  }
  else if((str2[0] == str1[0])&&(str2[1] ==str1[1])&&(str2[2] <str1[2]))
  {
    State =term;
    return State;
  }
  else if((str2[0] == str1[0])&&(str2[1] ==str1[1])&&(str2[2] ==str1[2])&&(str2[3] <=str1[3]))
  {
    State =term;
    return State;
  }
  return State;
}
/***

�洢���ı�ż�����ʱ�䵽Կ����

��Ҫ��Ϊ�˴洢������¼,��������������Բ�Ҫ

****/
void  Save_Lock_doing(void)
{
  //uchar mid_buff[10];
  //uchar i;
  /**
   {
  Lock_number[0] =0x00;
  Lock_number[1] =0x00;	   //���Բ���
  Lock_number[2] =0x06;
  Lock_number[3] =0x50;
  }
  ***/
	
  OperateKey();
	
  Clear_WDT();
}



 /**

// ��ʼ��Կ�ײ�����ͷָ�� ---����趨��ʲô������������� y

// ��ʼ��EEPROM�ж���ĺ�ĳ�ʼֵ

//  **/
// void Init_KeyRecord(void)
// {
//   uchar k,j;
//   Clear_WDT();
//   for(k =0;k<WRITE_ZHI;k++)		//#define  WRITE_ZHI     40
// 		msDelay(500);
// 	//void WritEDAta(uint address,uchar data)
//   WritEDAta(KEY_RECORD_ROW_H,0x00);
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_RECORD_ROW_L,0x00);
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//   WritEDAta(KEY_RECORD_INDEX_H,0x17);		//0x17=23 #define   KEY_RECORD_START		 0x1782
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_RECORD_INDEX_L,0x82);		//0x82=130
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//   WritEDAta(KEY_YONGHU_INDEX_H,0x00);
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_YONGHU_INDEX_L,0x11);		//17
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_YONGHU_ROW_H,0x00);
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//   WritEDAta(KEY_YONGHU_ROW_L,0x00);
// 	
// 	for(j =0;j<246;j++)
// 	{
// 		Clear_WDT();
// 		for(k =0;k<WRITE_ZHI;k++)
// 			msDelay(500);
// 		WritEDAta(SANBIAO_START+j,0xff);		//�൱�ڸ�λ

// 	}
// 	Clear_WDT();
// 	for(k =0;k<80;k++)
// 		msDelay(500);
// 	for(k =0;k<246;k++)
// 		Ti[k] =ReadData(SANBIAO_START+k);	//������ʱ�򽫣����ݻ�������������������������������
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(DETELE_ROW,0x00);
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//   WritEDAta(DETELE_INDEX_H,0x67);		//0x67=103
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(DETELE_INDEX_L,0x07);		//0x07=7
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//   WritEDAta(HEAD_BIT,0x01);		//HEAD_BITͷָ���ʼ����־λ
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//  
//   
// }

/*

��д�����Init_KeyRecord����

*/

void mykey_init(void)
{
  uchar k;
  Clear_WDT();
	
	/*AUTHOR_ROW*/
  for(k =0;k<WRITE_ZHI;k++)		//#define  WRITE_ZHI     40
		msDelay(500);
  WritEDAta(AUTHOR_ROW_H,0x00);
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(AUTHOR_ROW_L,0x00);
	
	/*AUTHOR_POINT*/	
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  Clear_WDT();
  WritEDAta(AUTHOR_POINT_H,0x04);		//#define AUTHOR_START  0x0b4f 
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(AUTHOR_POINT_L,0x00);		
	
	
	/*CAIMA_ROW*/
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(CAIMA_ROW_H,0x00);
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(CAIMA_ROW_L,0x00);	
	
	/*CAIMA_POINT*/	
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  Clear_WDT();
  WritEDAta(CAIMA_POINT_H,0x06);		//
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(CAIMA_POINT_L,0x59);		//
	
	
	/*RECORD_ROW*/
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(RECORD_ROW_H,0x00);
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(RECORD_ROW_L,0x00);	
	
	/*RECORD_POINT*/	
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  Clear_WDT();
  WritEDAta(RECORD_POINT_H,0x08);		//
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(RECORD_POINT_L,0x00);		//	
	
	/*XUNJIAN_ROW*/
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(XUNJIAN_ROW_H,0x00);
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(XUNJIAN_ROW_L,0x00);	
	
	/*XUNJIAN_POINT*/	
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  Clear_WDT();
  WritEDAta(XUNJIAN_POINT_H,0x0c);		//
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  WritEDAta(XUNJIAN_POINT_L,0x00);		//	
	
	/*KEY_ENCODE*/
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  Clear_WDT();
  WritEDAta(KEY_ENCODE,0x00);		//KEY_ENCODEͷָ���ʼ����־λ
	
	
	
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  Clear_WDT();
 
  
}


/***
 ��μ������
 Ŀǰ�洢������Ϊ200��
****/
// void  Test_record(void)
// {
//   uchar i,k;
//   uint j;
//   uchar nChar;
//   Init_KeyRecord();
//   g_Buff[0] =0x22;
//   g_Buff[1] =0x22;
//   g_Buff[2] =0x22;
//   g_Buff[3] =0x22;


//   for(i =0;i<210;i++)
//   {
//     read_time();
//     for(k =0;k<6;k++)
//     g_Buff[4+k]=time_buff[k];
// 		// for(i =0;i<30;i++)
//     //msDelay(500);
//     OperateKey(g_Buff);

//   }
// 	for(j =0;j<2000;j++)
// 	{
// 		nChar =ReadData(KEY_RECORD_START+j);	  //���Զ�д�����Ƿ�����--����
// 		UARTSendChar(1,nChar);
// 	}


// }
 /***

  ���洢��ͷָ���Ƿ�����	,����������
 ****/
void Cheak_I2cHead(void)
{

  uint  head,consult,row;
  uchar left,k,new_row;
  Clear_WDT();
  row= ReadData(KEY_RECORD_ROW_H);   //�Ѵ洢������
  row =((row<<8)&0xff00)+ReadData(KEY_RECORD_ROW_L);
  head= ReadData(KEY_RECORD_INDEX_H);   //�Ѵ洢������
  head =((head<<8)&0xff00)+ReadData(KEY_RECORD_INDEX_L);
  if(head >=0x1782)
  consult =(head -0x1782);   //0x1782Ϊ������Ϣ�Ĵ洢��ͷ��ַ
  else 
  {
		head =0x1782;
		consult =0;
		Clear_WDT();
    for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
		WritEDAta(KEY_RECORD_ROW_H,0x00);
		for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
		WritEDAta(KEY_RECORD_ROW_L,0x00);
		for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
		Clear_WDT();
		WritEDAta(KEY_RECORD_INDEX_H,0x17);
		for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
		WritEDAta(KEY_RECORD_INDEX_L,0x82);
		for(k =0;k<WRITE_ZHI;k++)
			msDelay(500);
		Clear_WDT();
		return;
  }
  left =consult%10;     //leftΪ���� �����Ƿ�Ϊ��
  if(left ==0)
  {
    Clear_WDT();
	  return;
  }
  else if(left !=0)
  {
    Clear_WDT();
	  head =consult-left+0x1782;
	  if( row >=2000)   //�洢��2000������
	  {
	    Clear_WDT();
			new_row =(head&0x00ff);
			for(k =0;k<WRITE_ZHI;k++)
			   msDelay(500);
			WritEDAta(KEY_RECORD_INDEX_L,new_row);
			new_row =(head&0xff00)>>8;
			for(k =0;k<WRITE_ZHI;k++)
			   msDelay(500);
			Clear_WDT();
			WritEDAta(KEY_RECORD_INDEX_H,new_row);
			for(k =0;k<WRITE_ZHI;k++)
			   msDelay(500);
			Clear_WDT();

	  }
	  else if(row <2000) //�洢δ��2000������
	  {
			if(row >=1)
				row =row -1;
			else
				row =0;
		  Clear_WDT();
		  new_row =(row&0x00ff);
		 	for(k =0;k<WRITE_ZHI;k++)
			   msDelay(500);
			WritEDAta(KEY_RECORD_ROW_L,new_row);
			new_row =(row&0xff00)>>8;
			for(k =0;k<WRITE_ZHI;k++)
			   msDelay(500);
			Clear_WDT();
			WritEDAta(KEY_RECORD_ROW_H,new_row);
			new_row =(head&0x00ff);
			for(k =0;k<WRITE_ZHI;k++)
			   msDelay(500);
			WritEDAta(KEY_RECORD_INDEX_L,new_row);
			new_row =(head&0xff00)>>8;
			for(k =0;k<WRITE_ZHI;k++)
			   msDelay(500);
			Clear_WDT();
			WritEDAta(KEY_RECORD_INDEX_H,new_row);
			for(k =0;k<WRITE_ZHI;k++)
			   msDelay(500);
			Clear_WDT();

	  }

  }

  



}
/***
��������Ϣ�洢
****/
// void Test_SaveSuo(void)
// {
//   uchar k;
//   uint j;
// //  uint row;
//   //uint head;
//   uchar test_buff[10] ={0x55,0x7a,0x44,0x44,0x33,0x22,0x11,0x01,0x02,0x77}; //���Ե��û�����
//   Clear_WDT();
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_RECORD_ROW_H,0x00);
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_RECORD_ROW_L,0x00);
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//   WritEDAta(KEY_RECORD_INDEX_H,0x17);
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   WritEDAta(KEY_RECORD_INDEX_L,0x82);
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//   for(j =0;j<1900;j++)
//   {
//      Clear_WDT();
//      OperateKey(test_buff);
//   }
//   Clear_WDT();
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
// 	g_Buff[0]= ReadData(KEY_RECORD_ROW_H);   //�Ѵ洢������
// 	g_Buff[1] =	ReadData(KEY_RECORD_ROW_L);
// 	g_Buff[2]= ReadData(KEY_RECORD_INDEX_H);   //�Ѵ洢������
// 	g_Buff[3]=	ReadData(KEY_RECORD_INDEX_L);
// 	Clear_WDT();
// 	while(1);


// }
/****

��4��RFID��+6��time���ֽڵĲ�����¼д��EEPROM

****/
void OperateKey(void)  //����Կ��
{
	uint row;
	uint head;
	uchar i,k;
	uchar new_row;
	Clear_WDT();
	
	//Cheak_I2cHead();   //�˺���������
	
	for(k =0;k<WRITE_ZHI;k++)
    msDelay(500);
	Clear_WDT();
  row= ReadData(RECORD_ROW_H);   //�Ѵ洢������
	row =((row<<8)&0xff00)+ReadData(RECORD_ROW_L);
	
//   head= ReadData(RECORD_POINT_H);   //�Ѵ洢��������ָ��
// 	head =((head<<8)&0xff00)+ReadData(RECORD_POINT_L);
	head=RECORD_START;
	
	for(i=0;i<row;i++)		//���������25����Ȩ
	{
			if(ReadData(head)==Lock_number[0]&&ReadData(head+1)==Lock_number[1]&&ReadData(head+2)==Lock_number[2]&&ReadData(head+3)==Lock_number[3])		//�����û���Ų����û��ĵ�ַ
				break;
			head=head+5;		//����һ����Ȩ���ܳ���Ϊ24�ֽ�
	}	
	
	if(i<row)	//��ʾ�ҵ�����ͬ����ļ�¼��ֻ����дһ��
	{
			//д������RFID
			Clear_WDT();
			for( i=0;i<4;i++)	//str[]�Ĵ�С��Ϊ10
			{
				Clear_WDT();
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(head+i,Lock_number[i]);		//ͨ��SPI����д���ݵ�eeprom��
			}	
			//д��Ȩ������
			Clear_WDT();	
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(head+i,shouquan_jieguo);		//ͨ��SPI����д���ݵ�eeprom��	


// 			head=head+5;		//KEY_RECORD����һ����ַ	
// 			if(head >=RECORD_END)
// 				head =RECORD_START;		//�������	
// 			
// 			Clear_WDT();
// 			//����RECORD_ROW��ֵ
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			if(row <256)		//ֻ�õ���RECORD_ROW_L
// 				row++;		//�б�ʾ������¼������
// 			new_row =(row&0x00ff);		//��8λ
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			WritEDAta(RECORD_ROW_L,new_row);
// 			
// 			
// 			Clear_WDT();	
// 			new_row =(row&0xff00)>>8;		//��8λ
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			WritEDAta(RECORD_ROW_H,new_row);
// 			
// 			Clear_WDT();	
// 			//����RECORD_POINT��ֵ
// 			new_row =(head&0x00ff);
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			WritEDAta(RECORD_POINT_L,new_row);
// 			
// 			new_row =(head&0xff00)>>8;
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			WritEDAta(RECORD_POINT_H,new_row);
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			Clear_WDT();
	}
	else if(i>=row)		//��ʾû���ҵ�,����ָ�봦д������λ��д��
	{
			head= ReadData(RECORD_POINT_H);   //�Ѵ洢��������ָ��
			head =((head<<8)&0xff00)+ReadData(RECORD_POINT_L);			
		
			//д������RFID
			Clear_WDT();
			for( i=0;i<4;i++)	//str[]�Ĵ�С��Ϊ10
			{
				Clear_WDT();
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(head+i,Lock_number[i]);		//ͨ��SPI����д���ݵ�eeprom��
			}	
			//д��Ȩ������
			Clear_WDT();	
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(head+i,shouquan_jieguo);		//ͨ��SPI����д���ݵ�eeprom��	


			head=head+5;		//KEY_RECORD����һ����ַ	
			if(head >=RECORD_END)
				head =RECORD_START;		//�������	
			
			Clear_WDT();
			//����RECORD_ROW��ֵ
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			if(row <256)		//ֻ�õ���RECORD_ROW_L
				row++;		//�б�ʾ������¼������
			new_row =(row&0x00ff);		//��8λ
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(RECORD_ROW_L,new_row);
			
			
			Clear_WDT();	
			new_row =(row&0xff00)>>8;		//��8λ
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(RECORD_ROW_H,new_row);
			
			Clear_WDT();	
			//����RECORD_POINT��ֵ
			new_row =(head&0x00ff);
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(RECORD_POINT_L,new_row);
			
			new_row =(head&0xff00)>>8;
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(RECORD_POINT_H,new_row);
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			Clear_WDT();		
		
	}
}


/**
 ��ȡ�µ�ͨ��Э���ģʽ
 �õ͵�ƽ������ʼ
 �øߵ�ƽ�ĳ��ȴ��� 0 1

**/

uchar Read_New_One(void)
{
   
   uchar nChar,data;
   uchar i;
   nChar =0;
   data =0;
   for(i =0;i<8;i++)
   {
         Clear_WDT();
         KillTimer(TIMER_TIMEOUT);                         //����һ��ض�ʱ��
	  	   g_WaitTimeOut = 0;
         SetTimer(TIMER_TIMEOUT,1,Do_TimeOut,THREE_MIN);   //�ȴ�Կ�װγ��Ĺ���
   while((RXD_S_DATA !=0)&& (g_WaitTimeOut == 0))
   {
			Clear_WDT();
   }
   KillTimer(TIMER_TIMEOUT); 
   if(g_WaitTimeOut == 1)
   {
			KillTimer(TIMER_TIMEOUT);
			Clear_WDT();
      return  0xff;
   }
   /*****/
   g_WaitTimeOut = 0;
   SetTimer(TIMER_TIMEOUT,30,Do_TimeOut,THREE_MIN);   //�ȴ�Կ�װγ��Ĺ���
   while((RXD_S_DATA ==0)&&(g_WaitTimeOut == 0))	   //����һ����ʱ����������
   {
    Clear_WDT();
   }
   KillTimer(TIMER_TIMEOUT); 
   if(g_WaitTimeOut == 1)
   {
			KillTimer(TIMER_TIMEOUT);
			Clear_WDT();
      return  0xff;
   }
   /********/

   Clear_WDT();
   WaitTF0(FOUR_MS);//�ȹ���ʼλ
   Clear_WDT();
   //if(RXD_S_DATA ==0)
   {
	 //return 0xff;
   }

  // WaitTF0(FOUR_MS);//�ȹ���ʼλ
   if(RXD_S_DATA ==0)
   {
	 nChar =0;
   }
   else if(RXD_S_DATA ==(GPIO_Pin_13))
   {
    nChar =1;
   } 
   data=data>>1;
   if(nChar ==1)
   {
     data|=0x80;
   }
   	 
   }
   Clear_WDT();
   return data;
}
/**
 һ��ע���ڶ˿��л�ʱ�ĵ�ƽ�������мǣ��м�
 �˴��Ƿ�������
***/
uchar Read_New_Code(void)
 {

   uchar i;
	 for(i =0;i<5;i++)
	 {
	   Clear_WDT();
		 g_WaitTimeOut = 0;
     SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
     while( g_WaitTimeOut == 0 )  //�ȴ����ص�GPSͷ�ε��ֽ�
     {
	     Clear_WDT();
       if(RXD_S_DATA ==0)
					break;
     }
     KillTimer(TIMER_TIMEOUT);
	 //if(g_WaitTimeOut == 1)
	 {
	  //return  0xff; 
	 }
	 WaitTF0(TOW_MS);//�ȹ���ʼλ
	 if(RXD_S_DATA ==0)
	 {
	   break;
	 }

	 }

	 Clear_WDT();
	 return 0;

 }
/***
void Test_Head(void)
{
    uchar row,page;
	uchar residue;   //����
	uchar quotient;  //��
    uchar head;
	 Clear_WDTDOG();
	globle_row= SubI2c_Read(0x50);   //�Ѵ洢������
    globle_head= SubI2c_Read(0x51);  //���ݿ�ͷָ��
    globle_page= SubI2c_Read(0x52);  //ҳָʾ
	head =SubI2c_Read(0x51); 
	if((globle_page ==0x00)||(globle_page ==0x01))
	{
	  if((globle_row <=20)&&(globle_page ==0x00))   //δ�������ڵ�һҳ
	  {
	    if((head >=0x60)&&(head <=0xff))  //ͷָ����ڳ�ʼָ��
		{
		  Clear_WDTDOG();
		 quotient =(head-0x60)/8; //�̵����
		 globle_head =0x60+quotient*8;
		 globle_row =quotient;
		  Clear_WDTDOG();
		}
		else           //��С�ڳ�ʼָ��������
		{
		 globle_head =0x60;
		 globle_row =0;
		 globle_page=0;
		}
	  }
	  else if((globle_row >=20)&&(globle_row <40)&&(globle_page ==0x01))//δ�������ڵڶ�ҳ
	  {
	    if((head >=0x00)&&(head <=0x9e))  //ͷָ����ڳ�ʼָ��
		{
		  Clear_WDTDOG();
		 globle_head =head+0x100;
		 quotient =(globle_head-0x60)/8; //�̵����
		 globle_head =0x60+quotient*8;
		 globle_row =quotient;
		  Clear_WDTDOG();
		}
		else           //��С�ڳ�ʼָ��������
		{
		 globle_head =0x60;
		 globle_row =0;
		 globle_page=0;
		}
	  }
	  else if(globle_row >=40)   //�Ѵ���
	  {
	    globle_row =40;
		if((globle_page ==0x00)&&(globle_head >=0x60)&&(globle_head <=0xff))
		{
		  Clear_WDTDOG();
		 quotient =(globle_head-0x60)/8; //�̵����
		 globle_head =0x60+quotient*8;
		}
		else if((globle_page ==0x01)&&(head <=0x9e)&&(head >=0x00))
		{
		  Clear_WDTDOG();
		 globle_head =head+0x100;
		 quotient =(globle_head-0x60)/8; //�̵����
		 globle_head =0x60+quotient*8;
		}
		else
		{
		 globle_head =0x60;
		 globle_row =0;
		 globle_page=0;
		}
	  }
	}
	else
	{
	 globle_head =0x60;
	 globle_row =0;
	 globle_page=0;
	}
 Clear_WDTDOG();

}
 ****/
