#include "oled.h"
#include "delay.h"
#include "sw-key.h"
//#include "rfid.h"
//#include "stm32f10x_it.h"
//#include "misc.h"
#include "misc.h"
#include "Timer.h"
//#include "usb.h" 
#include "RTC_time.h"

#include "flash.h"
#include "spi.h"
#include "Globle_Var.h"

//#include "boot_up.h"


//#define  KEY_POWER_ON	    	(GPIOA->BSRR = GPIO_Pin_8)   //PA8��SP-EN����λ��ť
//#define  KEY_POWER_OFF		  (GPIOA->BRR  = GPIO_Pin_8)  // (ʵ�ʵ�ԭ��ͼ��ΪTPS79633)

#define  MAX_BUFF      520     //255
#define  MAX_TI        246
#define  WRITE_ZHI     40

// #define White          0xFFFF
// #define Black          0x0000
// #define Grey           0xF7DE
// #define Blue           0x001F
// #define Blue2          0x051F
// #define Red            0xF800
// #define Magenta        0xF81F
// #define Green          0x07E0
// #define Cyan           0x7FFF
// #define Yellow         0xFFE0




// typedef struct
// {
//   u8 NVIC_IRQChannel;
//   u8 NVIC_IRQChannelPreemptionPriority;
//   u8 NVIC_IRQChannelSubPriority;
//   FunctionalState NVIC_IRQChannelCmd;
// } NVIC_InitTypeDef;

#define MAX_MENU      10


const u8 TEXT_Buffer[]={"MiniSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)

extern uint8_t g_State;
extern uint8_t KeyNum;
//extern UARTBUFF  m_UB0;
//extern UARTBUFF1  m_UB1;
//uchar  lcd_buf_batt0[64];
unsigned char rightcode1[4]="swxs";

extern u8 g_WaitTimeOut;
extern const MENU_ITEM HsMenu[MAX_MENU];

unsigned char m_Uart_State;

void ST_Idle(void);
void GPIO_Configuration(void);

uchar Rfid_Buff[6]; 
uchar g_Buff[MAX_BUFF];

void NVIC_Configuration(void);
void USART1_Config(void);



extern uint8_t lcd_buf_batt[1024];
extern uint8_t gImage_signal2[1024];

uchar m,n;
uint author_addr,author_row,row_num,temp_addr;
char author_data[24];

uint caima_addr,caima_row,row_num2,temp_addr2;
char caima_data[8];

uint record_addr,record_row,row_num3,temp_addr3;
char record_data[5];

u8 test_code[]={
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
		0x2A,0x2B,0x2C,0x2D,0x2E,0x2F
};


void Clear_WDT(void)
{
  IWDG_ReloadCounter();	  //�忴�Ź���ֵ
}


int main(void)
{ 
	u16 i=0;
	u8 key;
	u8 datatemp[SIZE];
	//uint8_t i;
	//uint8_t j,nChar;
	//u8 addr;
	uint8_t a;
	//uint author_addr,author_row,other_row;
	
	
	SystemTick_Init();
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_USART2|RCC_APB1Periph_SPI2, ENABLE);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1  | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1 | RCC_APB2Periph_SPI1 | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_USART2|RCC_APB1Periph_SPI2, ENABLE);		
	
	NVIC_Configuration();
	
//  	NVIC_Group_Init(NVIC_PriorityGroup_2);//ʹ���жϷ��� 2
// 	Uart1_Init();													//����1��ʼ��
//  	Usart1_NVIC_Configuration();					//����1 �ж�����
	
	SPI2_Init() ;
 	OLED_Init();		//�ȵ���LCD_GPIO_Init()��ʼ��,�ٵ���Initial_ILI9163��ʼ��TFT��
// 	OLED_ShowString(1,0, "0.96' OLED TEST");  
// 	OLED_ShowString(1,16,"mcudev.taobao  ");  
// 	OLED_ShowString(1,32,"2014-06-16");  
// 	OLED_ShowString(1,48,"ASCII: ");  
// 	OLED_ShowString(63,48,"CODE: ");   
  //often_show(16,1,"SF199ʼ");
  //GPIO_Configuration();
	Init_Sw_Key_Io();		//��ť��ʼ��

  Init_KeyNumuber();		//Key_Num =KEYINIT
	//GUI_DisphzAt(shi_hanzi,2,2);
	//void often_show(uchar s_column,uchar s_row,const uchar show_buf[32],uint wr_color,uint blank_color)
	
	//USART_Configuration();		//����1�����ã���������λ��ͨ��
	//USART2_Configuration();		//����2�����ã����ڲ����ж϶˿ڶ�ȡά��������ݣ������ж�ģʽ��ȡ����

	//Exit_Line_Init();		//�ⲿ�жϵ����� �½��ش����жϵ�ģʽ
	//USART1_Config();
	//ADC_GPIO_Configuration();
	//ADC_Configuration();
	
	RTC_Configuration();
	RTC_init();
	SPI_Flash_Init();   //SPI FLASH ��ʼ�� 
	//SPI_FLASH_Init();
//	eeprom_Init();
	
  //Rfid_IO_Init_In();
	
	
//	often_show(36,56,"��ʼ����...",0,0x0fff);	
	
	
// 	for(i=0;i<WRITE_ZHI;i++)
// 			msDelay(500);
// 	//WritEDAta(INIT_FLAG,1);   //�˴�������EEPROM
//  	WritEDAta(INIT_FLAG,0);		  //�˴����ز���EEPROM
// 	
// 	if(ReadData(INIT_FLAG) ==0x01)		//����ʼ����־����1���ͳ�ʼ��POINT��ROW
// 	{
// 		//Init_KeyRecord();
// 	  mykey_init();		//��ʼ��Կ�ײ�����ͷָ�� ---����趨��ʲô�������������
// 	}
// 	
// 	
// 	g_State=ST_IDLE;
	
 //	Usb_Power_Init_In();		//PC12��USB_BIT
//   Led_Init();		//LED�Ƶĳ�ʼ����û��ʵ��
  //Init_Timer0();
 // Init_Timer();
//  RTC_init();         //��ʼ��RTCʱ��
  OLED_WR_Byte(0xAE,OLED_CMD);      //�رո��������Ĺ��磬PA8����SP_EN,ͨ��J8�ӳ�ȥ
	
	MOS_ON;
	
	
	
/**
LCD_LIGHT���Ź��ܲ���	
**/	
	
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 // #define    LCD_LIGHT_ON	    	(GPIOB->BSRR  = GPIO_Pin_13)	��һ��0	
	
	
//  while(1);	
	
	
	
	
/**
��ʾͼ�����
**/
// 	GPIOC->IDR;
// 	
// 	//GetKey();
    OLED_Clear();     //����
// 		while(1)
// 		{
//   	Choose_MenuOp1();
// 		}
	while(SPI_Flash_ReadID()!=FLASH_ID)//��ⲻ��W25X16
	{	   
		i=SPI_Flash_ReadID();
		printf("ID:%d",i);
		OLED_ShowString(60,130,"W25X16 Check Failed!");
		delay_ms(500);
		OLED_ShowString(60,130,"   Please Check!    ");
		delay_ms(500);

	}
	OLED_ShowString(60,130,"W25X16 Ready!");
	//��ʾ��ʾ��Ϣ
	OLED_ShowString(60,150,"KEY0:Write KEY2:Read");

  
	while(1)
	{
		key=GetKey();
		if(key==KEY1)//KEY0����,д��SPI FLASH
		{
 			OLED_ShowString(0,0,"Start Write W25X16....");
			SPI_Flash_Write((u8*)TEXT_Buffer,1000,SIZE);//��1000�ֽڴ���ʼ,д��SIZE���ȵ�����
			OLED_ShowString(0,16,"W25X16 Write Finished!");//��ʾ�������
		}
		if(key==KEY2)//WK_UP����,��ȡд����ַ����ַ�������ʾ
		{
 			OLED_ShowString(0,0,"Start Read W25X16.... ");
			SPI_Flash_Read(datatemp,1000,SIZE);//��1000��ַ����ʼ,����SIZE���ֽ�
			OLED_ShowString(0,16,"The Data Readed Is:  ");//��ʾ�������
			OLED_ShowString(0,32,datatemp);//��ʾ�������ַ���
		}
// 		i++;
// 		delay_ms(10);
// 		if(i==20)
// 		{
// 			LED0=!LED0;//��ʾϵͳ��������	
// 			i=0;
// 		}		   
	}
// 	
// // 	show_pure_image(0xff,0xe0);
// // 	delay_ms(1500);
// 	
//  	show_picture1(0,0,31,15,lcd_buf_batt);

//  	show_picture1(96,0,127,15,gImage_signal2);
 
		
	    //show_picture1(0,0,lcd_buf_batt0);
		
      //display_shuzi(0,0,rightcode1);
	
	    //Lcd_ShowN(0, 0, rightcode1);
	     //DisplayNumber(0, 0, 0x80);
	
	
/**
RFID����
**/	
//	delay_ms(1500);		
	
	//show_picture1(0,0,127,159,gImage_concept5);
	
// 	often_show(36,56,"ͨ��",0,0x0fff);	
// 	
// 	
// 	while(1)
// 	{
// 			often_show(36,56,"RFID",0,0x0fff);
// 			delay_ms(1000);	
// 			OLED_Clear();	    //����
// 			nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 			m_UB1;
// 			if(nChar ==5) 
// 			{
// 				if(Rfid_Buff[0]==0&&Rfid_Buff[1]==0&&Rfid_Buff[2]==0&&Rfid_Buff[3]==0)
// 					;
// 				else		//������һ����Ϊ��
// 				{
// 					UARTnSendChar(1,Rfid_Buff,5);
// 					apoint_place_write(56,80,Rfid_Buff,Red,White);
// 					delay_ms(1000);						
// 					break;
// 				}
// 			}
// 	}

// while(1);	
	

/**
��ʾ���ֺ�������
**/ 	
// // #define White          0xFFFF
// // #define Black          0x0000
// // #define Grey           0xF7DE
// // #define Blue           0x001F
// // #define Blue2          0x051F
// // #define Red            0xF800
// // #define Magenta        0xF81F
// // #define Green          0x07E0
// // #define Cyan           0x7FFF
// // #define Yellow         0xFFE0
// 	
// // 	 OLED_Clear();	    //����	
// // 	 print_hex(32, 32,Red, Rfid_Buff[0]);
// // 	 DisplayHNumber(32, 48, Magenta, Rfid_Buff[1]);
// // 	 DisplayNumber(32, 64, Green, Rfid_Buff[2]);
// 	
//  display_shuzi(32,32, Rfid_Buff, Green, Yellow);



/**
���ڲ���
**/

//���������⣬��Ϊ��RFID����̫��
//��RFID�����ַ�ʽ�����ں�Τ�����ַ�ʽ����������õ��Ǵ���

	MOS_ON;
//	USB_Cheak_init();
// 	while(1)
// 	{
// 		nChar =UARTReadFrm(1,g_Buff,520);
// 		if(nChar!=0)
// 		UARTnSendChar(1,g_Buff,10);	
// 	}

//	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		//�ؽ����жϣ��ڽ����ж�USART1_IRQHandler()���ϵĽ����ܵ����ݷ���m_UB0
//	UARTnSendChar(1,g_Buff,10);


//	Test_Usb_Data();
	
	
// 	USB_transmit_test();
// 	while(1);	
	
	
// 	while(1)
// 	{
		
//  	printf("usart\r\n");
// 		MOS_ON;
		
// 		m_UB0;
// 		m_UB1;
// 		UARTnSendChar(1,test_code,10);
// 		delay_ms(1500);		
		
//		nChar =UARTReadFrm(1,g_Buff,520);
		
//		USB_Cheak_init();
//		USB_transmit_test();
// 		m_UB0;		
// 		m_UB1;

		
//	}



/**
��ʱ������
**/
 // MOS_ON;
// 	UARTnSendChar(1,test_code,10);
// 	UARTnSendChar(1,test_code,10);
// 			
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,100,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //��ʱ�ȴ�
// 	{
// 			UARTnSendChar(1,test_code,10);
// 	}


	
	
/**
SPI����EEPROM����
**/	
//***********************************************************
//EEPROM�����ݶ����������ڷ�װ�õ�Կ���ϲ��Գɹ�����������û��ͨ��
//����Э��û��ͨ����ΪЭ��Ҫ��ȡEEPROM������
//***********************************************************	 
// 	for(i=0;i<WRITE_ZHI;i++)
// 			msDelay(500);
// 	WritEDAta(0,0x09);
// 	
// 	for(i=0;i<WRITE_ZHI;i++)
// 			msDelay(500);
// 	
// 	a= ReadData(0); 

// 	i=0;
// 	while(i<100)
// 	{
// 		UARTnSendChar(1,&a,1);
// 		i++;
//  }

// 	while(1);


/**
RTC
**/

// time_verify();
//set_time(0x15,0x05,0x12,0x15,0x54,0x30);

// while(1);


/**
ֱ�Ӳ�����Ȩ����
**/

// //��һ��ͨ�Ų���Ȩ,����USB_BIT
// 	 often_show(10,56,"��ʼ��ͨ����...",0,0x0fff);	
// 	 delay_ms(1000);
// 	 USB_transmit_test();


// //�ε�USB_BIT�˳�ͨ��
// 	 OLED_Clear();
// 	 often_show(36,56,"ͨ�Ž���",0,0x0fff);
//    delay_ms(2000);
// 	 OLED_Clear();	 


/*eeprom���ݲ���ʧ����*/


//  		author_addr = ReadData(AUTHOR_POINT_H);   
//  		author_addr = ((author_addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
//  		row_num=(author_addr-AUTHOR_START)/24;		//ÿ����Ȩ24�ֽ�
//  		
//  		author_row = ReadData(AUTHOR_ROW_H);   
//  		author_row = ((author_row<<8)&0xff00)+ReadData(AUTHOR_ROW_L);
// 			
// 		temp_addr=AUTHOR_START;
// 		for(m=0;m<row_num;m++)		//���������25����Ȩ
// 		{
// 			temp_addr=temp_addr+m*24;		//����һ����Ȩ���ܳ���Ϊ24�ֽ�

// 			for(n=0;n<24;n++)
// 						author_data[n]=ReadData(temp_addr+n);
// 				author_data[0];author_data[1];
// 			  author_data[2];author_data[3];
// 			  author_data[4];author_data[5];author_data[6];author_data[7];
// 			  author_data[8];author_data[9];author_data[10];author_data[11];author_data[12];author_data[13];author_data[14];author_data[15];
// 			  author_data[16];author_data[17];author_data[18];author_data[19];author_data[20];author_data[21];author_data[22];author_data[23];		
// 		}



//  		caima_addr = ReadData(CAIMA_POINT_H);   
//  		caima_addr = ((caima_addr<<8)&0xff00)+ReadData(CAIMA_POINT_L);
//  		row_num2=(caima_addr-CAIMA_START)/8;		//ÿ����Ȩ8�ֽ�
//  		
//  		caima_row = ReadData(CAIMA_ROW_H);   
//  		caima_row = ((caima_row<<8)&0xff00)+ReadData(CAIMA_ROW_L);
// 			
// 		temp_addr2=CAIMA_START;
// 		for(m=0;m<row_num2;m++)		
// 		{
// 				temp_addr2=temp_addr2+m*8;				
// 				for(n=0;n<8;n++)
// 						caima_data[n]=ReadData(temp_addr2+n);
// 				caima_data;
// 		}
// 		
// 		
// 		
//  		record_addr = ReadData(RECORD_POINT_H);   
//  		record_addr = ((record_addr<<8)&0xff00)+ReadData(RECORD_POINT_L);
//  		row_num3=(record_addr-RECORD_START)/5;		//ÿ����Ȩ5�ֽ�
//  		
//  		record_row = ReadData(RECORD_ROW_H);   
//  		record_row = ((record_row<<8)&0xff00)+ReadData(RECORD_ROW_L);
// 			
// 		temp_addr3=RECORD_START;
// 		for(m=0;m<row_num3;m++)		
// 		{
// 				temp_addr3=temp_addr3+m*5;					
// 				for(n=0;n<5;n++)
// 						record_data[n]=ReadData(temp_addr3+n);
// 				record_data;
// 		}		
		
		



/*��ʾ��������*/		

// 	OLED_Clear();	
// 	
// 	print_hex(0,0,0x00,0xab);
// 	DisplayHNumber(16,16,0x00,0x11);
// 	DisplayNumber(32,32,0x00,11);
	
// 	show_V_bar();
//   delay_ms(1000);	
// 	
// 	show_H_bar();
//   delay_ms(1000);	
	
// 	display_gray();
//   delay_ms(1000);	
// 	
// 	show_cs_open();	
//   delay_ms(1000);	

// 	show_frame();
//   delay_ms(1000);


//  GUI_DispCharAt_Test(uchar ASCii, uint x, uint y)	
//	GUI_DispCharAt_Test('A',80,80);

// 		a=0x42;
// //  GUI_DispCharAt(uchar *char_sp, uint x, uint y);
// 		GUI_DispCharAt(&a,96,80);
// 	  while(1);



//����˵�����
     //OLED_Clear();
		 
//	g_State=ST_IDLE;
//	while(1)																					 
//	{   
		//switch (g_State)	//g_State��һ��ȫ�ֺ���
		//{
			
		 //case ST_USB: 		//#define  ST_USB	          0XFD         
		 //USB_transmit_test();	//USB����
		// break;
		 
		// case ST_IDLE:    //#define  ST_IDLE          0XFF
		// ST_Idle();		    //����ʱ��״̬��⣬�ɸı�g_State
		// break;
		 
		 //case ST_KEY:
		 //Key_Trismit();  //��Կ�׵�ǰ���룬�ж��Ƿ��Ѿ�������
		 //break;
		 
		 //case ST_LCDMENU:   //#define  ST_LCDMENU      0XFA			��ѡ��˵�����
		 //Choose_MenuOp1();
		 //break;
		 
		 //default:break;

	 	//}
//	 }	











// /**
// ��ѭ������
// **/
//***********************************************************
//����Ĵ�ѭ��û�в�ͨ����������ʱ����ȥ����ʾδ֪�Ĵ���	 
//***********************************************************	 
// 	OLED_Clear();		//����
// 	
// 	g_State =ST_IDLE;		//��ʼ��ȫ�ֱ���g_State��ֵ	
// 	
// //	ST_Idle();		   //����ʱ��״̬��⣬�ɸı�g_State	
// 	
// 	
//  while(1)																					 
// 	{   
// 	
// 		switch (g_State)	//g_State��һ��ȫ�ֺ���
// 		{
// 		 case ST_USB:          
// 		 USB_transmit_test();	//USB����
// 		 break;
// 		 
// 		 case ST_IDLE:           
// 		 ST_Idle();		   //����ʱ��״̬��⣬�ɸı�g_State
// 		 break;
// 		 
// 		 case ST_KEY:
// 		 Key_Trismit();  //��Կ�׵�ǰ���룬�ж��Ƿ��Ѿ�������
// 		 break;
// 		 
// 		 case ST_LCDMENU:   //��ѡ��˵�����
// 		 Choose_MenuOp1();
// 		 break;
// 		 
// 		 default:break;

// 	 	}
// 	}	
	
	
	
// 	while(1)
// 	{
// // 		CS(0);
// // 		RES(0);
// //		printf("usart\r\n");
// 		
// 		OLED_Clear();
// 		Main_Lcd_Show();
// 		
//  		delay_ms(1500);		
// 		
// 		
// // 		often_show(0,0,"ID:",0,0x0fff);
// // 		
// // 		delay_ms(1500);
// // 		
// // 		often_show(36,36,"ͨ�ųɹ�",0x00,0xffff);		
// // 		
// // 		delay_ms(1500);		
// 		
// // 	 display_red();
// //   delay_ms(1500);
// // 	
// //   display_green();
// //   delay_ms(1500);
// // 	
// //   display_white();
// //   delay_ms(1500);
// // 	
// // 	 display_blue();
// //   delay_ms(1500);		
// //	 OutPut_Data(0x01);
// 		
// 		
 	}
	





 /***********
* Function Name  : ST_Idle()
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
�ڿ���״̬�ļ�������
   *1.����Ƿ���USB���ӵ��������
   *2.����Ƿ����ӵ�Կ��
   *3.���﮵�ص�ѹ�Ƿ�͵�
   �������񰴸��Ե�ʱ����ת�����������޳�ͻ

   �ͳ����ͨ�ŵ�ģʽ�������а������µ�״̬�� ���е�
 ************/
void ST_Idle(void)
{
  
  uint8_t i;
	u16 usb_bit;
  //uint8_t j,k;
  uchar nChar;
	uchar flagshijian=0;
  i =0;
	//j=0;k=0;
	//l =0;h=0;
  OLED_Clear();
  KeyNum =KEYNO;		//KeyNum��һ��ȫ�ֱ���
	Init_KeyNumuber();		//Key_Num =KEYINIT
  Main_Lcd_Show();   //��������ʾ	
	Main_Lcd_Show2();
  //Choose_MenuOp1();//Main_Lcd_Show();   //��������ʾ	
// 	ShowMenu(HsMenu);
// 	show_picture1(0,0,31,15,lcd_buf_batt);
//  show_picture1(96,0,127,15,gImage_signal2);
  while(1)
  {
		if(flagshijian==20)
		{
	  OLED_Clear2(0,31,15,95);
    Main_Lcd_Show2();
		flagshijian=0;
		}
		delay_ms(50);
		//�������¼�⣬����а����Ͱ��¾ͽ���˵�����
		
    KeyNum =GetKey();	 //�ж��Ƿ��а�������		
    if(KeyNum !=KEYNO&&KeyNum !=KEYINIT)
    {
      KeyNum =KEYNO;  //���갴��ֵ�󣬽�����ֵ��λ
			if(guanjiflag==1)
			{
				 Init_KeyNumuber();
				 OLED_Clear();	 
				 OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
				 guanjiflag=0;
				 delay_ms(1000);
			   often_show(36,56,"������...");
			   delay_ms(1000);
				 OLED_Clear();
				 Main_Lcd_Show();
			}
			else if (guanjiflag==0)
			{
      g_State =ST_LCDMENU;return; 
			}				//�����ⰴ�������ѡ��˵�
    }

		
				
		//RFID���
		
		//uchar Read_RfidData(char uart, uchar *pdst, char max)
// 		nChar =Read_RfidData(2,Rfid_Buff,0xff);			//�ǳ���Ҫ��һ�㣬�����Trismit�����Ĺؼ�
// 		if(nChar >=3)
// 		{
// 		 g_State =ST_KEY;return;	//���ID��Ϣ����,��������ˣңƣɣ����ݣ��򽫣�ߣӣ��������Ϊ�ӣԣߣˣţ�
// 		}
		
		
		
		
		//�����λ��������λ������Ӳ˵��������USBͨ����
		
// 		nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//g_Buff��һ��ȫ�ֱ�����#define  MAX_BUFF   520 
// 		if(nChar==7&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0d&&g_Buff[3]==0x02&&g_Buff[4]==0x00&&g_Buff[5]==0x01&&g_Buff[6]==0xD1)//�������0x00�ͷ���һ����Ȩ
// 		{

// 			if((USB_CHEAK&(GPIO_Pin_12))!=0 && USB_Cheak_init_flag==0)
// 			{
// 				pc_to_slave();			//��λ��������λ���Ĵ���
// 			  g_State =ST_USB; return;		  //����USBͨ��
// 			}
// 		}		
// 		
// 		
// 		
// 		//USB���Ӽ��
// 		i++;
// 		if(i ==0x1)
// 		{	
// 			 i=0;
// 			 //nChar =USB_CHEAK;
// //			 if((USB_CHEAK&(GPIO_Pin_12))!=0 && USB_Cheak_init_flag==0)     //����Ƿ���USB���ӳɹ� --PC12��ΪUSB���Ӽ���
// 			 usb_bit=(USB_CHEAK&(GPIO_Pin_12))>>12;
// 			 if(usb_bit==1 && USB_Cheak_init_flag==1)     //����Ƿ���USB���ӳɹ� --PC12��ΪUSB���Ӽ���
// 			 {
// 				 MOS_ON;   //��MAX3232����ģ��--MOS�ܿ���
// 				 g_State =ST_USB; return;		  //���˿ڵĵ�ѹ�Ƿ�Ϊ�ߵ�ƽ�����ж��Ƿ��Ѿ����ӵ������
// 			 }
// 			 else if((USB_CHEAK&(GPIO_Pin_12))==0)
// 			 {
// 				 g_State=ST_IDLE;
// 				 USB_Cheak_init_flag=1;
// 				 //return;		  //���˿ڵĵ�ѹ�Ƿ�Ϊ�ߵ�ƽ�����ж��Ƿ��Ѿ����ӵ������
// 			 }
// 				 
// 		}
		

		//��ص������
		
//    j++;		
// 		if( j==0xfffe)
// 		{
// 			j =0;
// 			Clear_WDT();
// 			if(Power_Cheak() ==1)  //����ص�ѹ�������ص�ѹ���ͣ������ػ�ģʽ����������ʾ
// 			{
// 				Lcd_Show_LowBattry();
// 				g_WaitTimeOut = FALSE;
// 				SetTimer(TIMER_TIMEOUT,8,Do_TimeOut,THREE_MIN);   
// 				while( g_WaitTimeOut == FALSE )
// 				{
// 					Clear_WDT();
// 				}
// 				KillTimer(TIMER_TIMEOUT);
// 				while(1)
// 					Clear_WDT();
// 			}
// 		}
    ++flagshijian;
  }
}



// void ST_Idle(void)
// {
//   
//   uint8_t i,j,k;
//   uchar nChar;
//   i =0;j=0;k=0;//l =0;h=0;
//   OLED_Clear()(White);
//   KeyNum =KEYNO;		//KeyNum��һ��ȫ�ֱ���
// 	Init_KeyNumuber();		//Key_Num =KEYINIT
//   Main_Lcd_Show();   //��������ʾ	
//   while(1)
//   {

//     KeyNum =GetKey();	 //�ж��Ƿ��а�������
// 		
//     if(KeyNum !=KEYNO&&KeyNum !=KEYINIT)
//     {
//       KeyNum =KEYNO;  //���갴��ֵ�󣬽�����ֵ��λ
//       g_State =ST_LCDMENU;return;  //�����ⰴ�������ѡ��˵�
//     }
// 		
// //    l++;		//��֪���������ȫ�ֱ����Ǹ����õģ�
// 		i++;
// 		//j++;
// 		//k++;		//����û���õ�
// 		//uchar Read_RfidData(char uart, uchar *pdst, char max)
// 		nChar =Read_RfidData(2,Rfid_Buff,0xff);			//�ǳ���Ҫ��һ�㣬�����Trismit�����Ĺؼ�
// 		
// 		if(nChar >=3)
// 		{
// 		 g_State =ST_KEY;return;	//���ID��Ϣ����,��������ˣңƣɣ����ݣ��򽫣�ߣӣ��������Ϊ�ӣԣߣˣţ�
// 		}
// 		
// 		if(i ==0x1)
// 		{	
// 			 i=0;
// 			 nChar =USB_CHEAK;
// 			 if((nChar&(GPIO_Pin_12))!=0)     //����Ƿ���USB���ӳɹ� --PC12��ΪUSB���Ӽ���
// 			 {
// 				 MOS_ON;   //��MAX3232����ģ��--MOS�ܿ���
// 				 g_State =ST_USB; return;		  //���˿ڵĵ�ѹ�Ƿ�Ϊ�ߵ�ƽ�����ж��Ƿ��Ѿ����ӵ������
// 			 }
// 		}

// // 		if( j==0xfffe)
// // 		{
// // 			j =0;
// // 			Clear_WDT();
// // 			if(Power_Cheak() ==1)  //����ص�ѹ�������ص�ѹ���ͣ������ػ�ģʽ����������ʾ
// // 			{
// // 				Lcd_Show_LowBattry();
// // 				g_WaitTimeOut = 0;
// // 				SetTimer(TIMER_TIMEOUT,8,Do_TimeOut,THREE_MIN);   
// // 				while( g_WaitTimeOut == 0 )
// // 				{
// // 					Clear_WDT();
// // 				}
// // 				KillTimer(TIMER_TIMEOUT);
// // 				while(1)
// // 					Clear_WDT();
// // 			}
// // 		}
//   }
// }



// /*******************************************************************************
// * Function Name  : NVIC_Configuration
// * Description    : Configures Vector Table base location.
// * Input          : None
// * Output         : None
// * Return         : None
// *******************************************************************************/
// void NVIC_Configuration(void)
// {
// // EXTI_InitTypeDef EXTI_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;   //����һ���ṹ��
// #ifdef  VECT_TAB_RAM  
//   /* Set the Vector Table base location at 0x20000000 */ 
//   NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
// #else  /* VECT_TAB_FLASH  */
//   /* Set the Vector Table base location at 0x08000000 */ 
//   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
// #endif
// 	
//  /**�˶γ����ǳ�ʼ������1���жϲ���**/
// 	
//   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
//   //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	
// 	
//   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel; 	//�ж�����
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//��Ӧ���ȼ�
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//   NVIC_Init(&NVIC_InitStructure); 
// 	
//   /**UART2�жϳ�ʼ������**/
// 	
//   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//��Ӧ���ȼ�
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
// 	
//   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;		//��ռ���ȼ�
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure); 
// 	
//       /* Configure and enable ADC interrupt */
// 			
//   NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
// 	

//      /* Enable the EXTI9_5 Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);

//        /* Enable the EXTI15_10 Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
//   
//  

// }







/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);



    
  /*red*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
   /*speaker*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	/*SPX2941-EN*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 


/*	MAX3232-POWER MOS_ON ^MOS_OFF*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

     /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


// /*
// * �������� USART1_Config
// * ���� �� USART1 GPIO ����,����ģʽ���á� 115200 8-N-1
// * ���� ����
// * ��� : ��
// * ���� ���ⲿ����
// */
// void USART1_Config(void)
// {
//  GPIO_InitTypeDef GPIO_InitStructure;
//  USART_InitTypeDef USART_InitStructure;

//  /* config USART1 clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

//  /* USART1 GPIO config */
//  /* Configure USART1 Tx (PA.09) as alternate function pushpull */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  /* Configure USART1 Rx (PA.10) as input floating */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  /* USART1 mode config */
//  USART_InitStructure.USART_BaudRate = 115200;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_No ;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//  USART_Init(USART1, &USART_InitStructure);
//  USART_Cmd(USART1, ENABLE);
//  }

