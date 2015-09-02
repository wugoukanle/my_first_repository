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


//#define  KEY_POWER_ON	    	(GPIOA->BSRR = GPIO_Pin_8)   //PA8即SP-EN，置位按钮
//#define  KEY_POWER_OFF		  (GPIOA->BRR  = GPIO_Pin_8)  // (实际的原理图上为TPS79633)

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
  IWDG_ReloadCounter();	  //清看门狗数值
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
	
//  	NVIC_Group_Init(NVIC_PriorityGroup_2);//使用中断分组 2
// 	Uart1_Init();													//串口1初始化
//  	Usart1_NVIC_Configuration();					//串口1 中断配置
	
	SPI2_Init() ;
 	OLED_Init();		//先调用LCD_GPIO_Init()初始化,再调用Initial_ILI9163初始化TFT屏
// 	OLED_ShowString(1,0, "0.96' OLED TEST");  
// 	OLED_ShowString(1,16,"mcudev.taobao  ");  
// 	OLED_ShowString(1,32,"2014-06-16");  
// 	OLED_ShowString(1,48,"ASCII: ");  
// 	OLED_ShowString(63,48,"CODE: ");   
  //often_show(16,1,"SF199始");
  //GPIO_Configuration();
	Init_Sw_Key_Io();		//按钮初始化

  Init_KeyNumuber();		//Key_Num =KEYINIT
	//GUI_DisphzAt(shi_hanzi,2,2);
	//void often_show(uchar s_column,uchar s_row,const uchar show_buf[32],uint wr_color,uint blank_color)
	
	//USART_Configuration();		//串口1的配置，用于与上位机通信
	//USART2_Configuration();		//串口2的配置，用于采用中断端口读取维根码的数据，串口中断模式读取数据

	//Exit_Line_Init();		//外部中断的配置 下降沿触发中断的模式
	//USART1_Config();
	//ADC_GPIO_Configuration();
	//ADC_Configuration();
	
	RTC_Configuration();
	RTC_init();
	SPI_Flash_Init();   //SPI FLASH 初始化 
	//SPI_FLASH_Init();
//	eeprom_Init();
	
  //Rfid_IO_Init_In();
	
	
//	often_show(36,56,"初始化中...",0,0x0fff);	
	
	
// 	for(i=0;i<WRITE_ZHI;i++)
// 			msDelay(500);
// 	//WritEDAta(INIT_FLAG,1);   //此次下载清EEPROM
//  	WritEDAta(INIT_FLAG,0);		  //此次下载不清EEPROM
// 	
// 	if(ReadData(INIT_FLAG) ==0x01)		//当初始化标志等于1，就初始化POINT和ROW
// 	{
// 		//Init_KeyRecord();
// 	  mykey_init();		//初始化钥匙操作的头指针 ---如何设定在什么情况下重新设置
// 	}
// 	
// 	
// 	g_State=ST_IDLE;
	
 //	Usb_Power_Init_In();		//PC12即USB_BIT
//   Led_Init();		//LED灯的初始化，没有实现
  //Init_Timer0();
 // Init_Timer();
//  RTC_init();         //初始化RTC时钟
  OLED_WR_Byte(0xAE,OLED_CMD);      //关闭给电子锁的供电，PA8，即SP_EN,通过J8接出去
	
	MOS_ON;
	
	
	
/**
LCD_LIGHT引脚功能测试	
**/	
	
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 // #define    LCD_LIGHT_ON	    	(GPIOB->BSRR  = GPIO_Pin_13)	置一，0	
	
	
//  while(1);	
	
	
	
	
/**
显示图标测试
**/
// 	GPIOC->IDR;
// 	
// 	//GetKey();
    OLED_Clear();     //清屏
// 		while(1)
// 		{
//   	Choose_MenuOp1();
// 		}
	while(SPI_Flash_ReadID()!=FLASH_ID)//检测不到W25X16
	{	   
		i=SPI_Flash_ReadID();
		printf("ID:%d",i);
		OLED_ShowString(60,130,"W25X16 Check Failed!");
		delay_ms(500);
		OLED_ShowString(60,130,"   Please Check!    ");
		delay_ms(500);

	}
	OLED_ShowString(60,130,"W25X16 Ready!");
	//显示提示信息
	OLED_ShowString(60,150,"KEY0:Write KEY2:Read");

  
	while(1)
	{
		key=GetKey();
		if(key==KEY1)//KEY0按下,写入SPI FLASH
		{
 			OLED_ShowString(0,0,"Start Write W25X16....");
			SPI_Flash_Write((u8*)TEXT_Buffer,1000,SIZE);//从1000字节处开始,写入SIZE长度的数据
			OLED_ShowString(0,16,"W25X16 Write Finished!");//提示传送完成
		}
		if(key==KEY2)//WK_UP按下,读取写入的字符传字符串并显示
		{
 			OLED_ShowString(0,0,"Start Read W25X16.... ");
			SPI_Flash_Read(datatemp,1000,SIZE);//从1000地址处开始,读出SIZE个字节
			OLED_ShowString(0,16,"The Data Readed Is:  ");//提示传送完成
			OLED_ShowString(0,32,datatemp);//显示读到的字符串
		}
// 		i++;
// 		delay_ms(10);
// 		if(i==20)
// 		{
// 			LED0=!LED0;//提示系统正在运行	
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
RFID测试
**/	
//	delay_ms(1500);		
	
	//show_picture1(0,0,127,159,gImage_concept5);
	
// 	often_show(36,56,"通信",0,0x0fff);	
// 	
// 	
// 	while(1)
// 	{
// 			often_show(36,56,"RFID",0,0x0fff);
// 			delay_ms(1000);	
// 			OLED_Clear();	    //清屏
// 			nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 			m_UB1;
// 			if(nChar ==5) 
// 			{
// 				if(Rfid_Buff[0]==0&&Rfid_Buff[1]==0&&Rfid_Buff[2]==0&&Rfid_Buff[3]==0)
// 					;
// 				else		//至少有一个不为零
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
显示数字函数测试
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
// // 	 OLED_Clear();	    //清屏	
// // 	 print_hex(32, 32,Red, Rfid_Buff[0]);
// // 	 DisplayHNumber(32, 48, Magenta, Rfid_Buff[1]);
// // 	 DisplayNumber(32, 64, Green, Rfid_Buff[2]);
// 	
//  display_shuzi(32,32, Rfid_Buff, Green, Yellow);



/**
串口测试
**/

//这里有问题，因为读RFID不是太灵
//读RFID有两种方式，串口和韦根两种方式，这个程序用的是串口

	MOS_ON;
//	USB_Cheak_init();
// 	while(1)
// 	{
// 		nChar =UARTReadFrm(1,g_Buff,520);
// 		if(nChar!=0)
// 		UARTnSendChar(1,g_Buff,10);	
// 	}

//	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		//关接收中断，在接受中断USART1_IRQHandler()不断的将接受的数据放在m_UB0
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
定时器测试
**/
 // MOS_ON;
// 	UARTnSendChar(1,test_code,10);
// 	UARTnSendChar(1,test_code,10);
// 			
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,100,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待
// 	{
// 			UARTnSendChar(1,test_code,10);
// 	}


	
	
/**
SPI——EEPROM测试
**/	
//***********************************************************
//EEPROM的数据读不出来，在封装好的钥匙上测试成功过，后来就没有通，
//导致协议没有通，因为协议要存取EEPROM的数据
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
直接测试授权开锁
**/

// //第一步通信并授权,先连USB_BIT
// 	 often_show(10,56,"初始化通信中...",0,0x0fff);	
// 	 delay_ms(1000);
// 	 USB_transmit_test();


// //拔掉USB_BIT退出通信
// 	 OLED_Clear();
// 	 often_show(36,56,"通信结束",0,0x0fff);
//    delay_ms(2000);
// 	 OLED_Clear();	 


/*eeprom数据不丢失测试*/


//  		author_addr = ReadData(AUTHOR_POINT_H);   
//  		author_addr = ((author_addr<<8)&0xff00)+ReadData(AUTHOR_POINT_L);
//  		row_num=(author_addr-AUTHOR_START)/24;		//每条授权24字节
//  		
//  		author_row = ReadData(AUTHOR_ROW_H);   
//  		author_row = ((author_row<<8)&0xff00)+ReadData(AUTHOR_ROW_L);
// 			
// 		temp_addr=AUTHOR_START;
// 		for(m=0;m<row_num;m++)		//现在最多有25条授权
// 		{
// 			temp_addr=temp_addr+m*24;		//现在一条授权的总长度为24字节

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
//  		row_num2=(caima_addr-CAIMA_START)/8;		//每条授权8字节
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
//  		row_num3=(record_addr-RECORD_START)/5;		//每条授权5字节
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
		
		



/*显示函数测试*/		

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



//进入菜单界面
     //OLED_Clear();
		 
//	g_State=ST_IDLE;
//	while(1)																					 
//	{   
		//switch (g_State)	//g_State是一个全局函数
		//{
			
		 //case ST_USB: 		//#define  ST_USB	          0XFD         
		 //USB_transmit_test();	//USB传输
		// break;
		 
		// case ST_IDLE:    //#define  ST_IDLE          0XFF
		// ST_Idle();		    //空闲时做状态检测，可改变g_State
		// break;
		 
		 //case ST_KEY:
		 //Key_Trismit();  //读钥匙的前导码，判断是否已经连接上
		 //break;
		 
		 //case ST_LCDMENU:   //#define  ST_LCDMENU      0XFA			多选择菜单操作
		 //Choose_MenuOp1();
		 //break;
		 
		 //default:break;

	 	//}
//	 }	











// /**
// 主循环测试
// **/
//***********************************************************
//下面的大循环没有测通，单步调试时进不去，提示未知的错误	 
//***********************************************************	 
// 	OLED_Clear();		//清屏
// 	
// 	g_State =ST_IDLE;		//初始化全局变量g_State的值	
// 	
// //	ST_Idle();		   //空闲时做状态检测，可改变g_State	
// 	
// 	
//  while(1)																					 
// 	{   
// 	
// 		switch (g_State)	//g_State是一个全局函数
// 		{
// 		 case ST_USB:          
// 		 USB_transmit_test();	//USB传输
// 		 break;
// 		 
// 		 case ST_IDLE:           
// 		 ST_Idle();		   //空闲时做状态检测，可改变g_State
// 		 break;
// 		 
// 		 case ST_KEY:
// 		 Key_Trismit();  //读钥匙的前导码，判断是否已经连接上
// 		 break;
// 		 
// 		 case ST_LCDMENU:   //多选择菜单操作
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
// // 		often_show(36,36,"通信成功",0x00,0xffff);		
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
在空闲状态的几个任务
   *1.检测是否有USB连接到计算机上
   *2.检测是否连接到钥匙
   *3.检测锂电池电压是否低电
   三个任务按各自的时间轴转动，基本上无冲突

   和充电座通信的模式，是在有按键按下的状态下 进行的
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
  KeyNum =KEYNO;		//KeyNum是一个全局变量
	Init_KeyNumuber();		//Key_Num =KEYINIT
  Main_Lcd_Show();   //主界面显示	
	Main_Lcd_Show2();
  //Choose_MenuOp1();//Main_Lcd_Show();   //主界面显示	
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
		//按键按下检测，如果有按键就按下就进入菜单操作
		
    KeyNum =GetKey();	 //判断是否有按键操作		
    if(KeyNum !=KEYNO&&KeyNum !=KEYINIT)
    {
      KeyNum =KEYNO;  //读完按键值后，将按键值复位
			if(guanjiflag==1)
			{
				 Init_KeyNumuber();
				 OLED_Clear();	 
				 OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
				 guanjiflag=0;
				 delay_ms(1000);
			   often_show(36,56,"开机中...");
			   delay_ms(1000);
				 OLED_Clear();
				 Main_Lcd_Show();
			}
			else if (guanjiflag==0)
			{
      g_State =ST_LCDMENU;return; 
			}				//按任意按键进入可选择菜单
    }

		
				
		//RFID检测
		
		//uchar Read_RfidData(char uart, uchar *pdst, char max)
// 		nChar =Read_RfidData(2,Rfid_Buff,0xff);			//非常重要的一点，是理解Trismit（）的关键
// 		if(nChar >=3)
// 		{
// 		 g_State =ST_KEY;return;	//检测ID信息数据,如果读到了ＲＦＩＤ数据，则将ｇ＿Ｓｔａｔｅ置为ＳＴ＿ＫＥＹ
// 		}
		
		
		
		
		//检测上位机连接下位机命令，从菜单界面进入USB通信中
		
// 		nChar =UARTReadFrm(1,g_Buff,MAX_BUFF);		//g_Buff是一个全局变量；#define  MAX_BUFF   520 
// 		if(nChar==7&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0d&&g_Buff[3]==0x02&&g_Buff[4]==0x00&&g_Buff[5]==0x01&&g_Buff[6]==0xD1)//如果返回0x00就发下一条授权
// 		{

// 			if((USB_CHEAK&(GPIO_Pin_12))!=0 && USB_Cheak_init_flag==0)
// 			{
// 				pc_to_slave();			//上位机连接下位机的处理
// 			  g_State =ST_USB; return;		  //进入USB通信
// 			}
// 		}		
// 		
// 		
// 		
// 		//USB连接检测
// 		i++;
// 		if(i ==0x1)
// 		{	
// 			 i=0;
// 			 //nChar =USB_CHEAK;
// //			 if((USB_CHEAK&(GPIO_Pin_12))!=0 && USB_Cheak_init_flag==0)     //检测是否有USB连接成功 --PC12脚为USB连接检测脚
// 			 usb_bit=(USB_CHEAK&(GPIO_Pin_12))>>12;
// 			 if(usb_bit==1 && USB_Cheak_init_flag==1)     //检测是否有USB连接成功 --PC12脚为USB连接检测脚
// 			 {
// 				 MOS_ON;   //打开MAX3232供电模块--MOS管控制
// 				 g_State =ST_USB; return;		  //检测端口的电压是否为高电平。来判断是否已经连接到充电座
// 			 }
// 			 else if((USB_CHEAK&(GPIO_Pin_12))==0)
// 			 {
// 				 g_State=ST_IDLE;
// 				 USB_Cheak_init_flag=1;
// 				 //return;		  //检测端口的电压是否为高电平。来判断是否已经连接到充电座
// 			 }
// 				 
// 		}
		

		//电池电量检测
		
//    j++;		
// 		if( j==0xfffe)
// 		{
// 			j =0;
// 			Clear_WDT();
// 			if(Power_Cheak() ==1)  //检测电池电压，如果电池电压过低，则进入关机模式，并进行提示
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
//   KeyNum =KEYNO;		//KeyNum是一个全局变量
// 	Init_KeyNumuber();		//Key_Num =KEYINIT
//   Main_Lcd_Show();   //主界面显示	
//   while(1)
//   {

//     KeyNum =GetKey();	 //判断是否有按键操作
// 		
//     if(KeyNum !=KEYNO&&KeyNum !=KEYINIT)
//     {
//       KeyNum =KEYNO;  //读完按键值后，将按键值复位
//       g_State =ST_LCDMENU;return;  //按任意按键进入可选择菜单
//     }
// 		
// //    l++;		//不知道定义这个全局变量是干嘛用的？
// 		i++;
// 		//j++;
// 		//k++;		//根本没有用到
// 		//uchar Read_RfidData(char uart, uchar *pdst, char max)
// 		nChar =Read_RfidData(2,Rfid_Buff,0xff);			//非常重要的一点，是理解Trismit（）的关键
// 		
// 		if(nChar >=3)
// 		{
// 		 g_State =ST_KEY;return;	//检测ID信息数据,如果读到了ＲＦＩＤ数据，则将ｇ＿Ｓｔａｔｅ置为ＳＴ＿ＫＥＹ
// 		}
// 		
// 		if(i ==0x1)
// 		{	
// 			 i=0;
// 			 nChar =USB_CHEAK;
// 			 if((nChar&(GPIO_Pin_12))!=0)     //检测是否有USB连接成功 --PC12脚为USB连接检测脚
// 			 {
// 				 MOS_ON;   //打开MAX3232供电模块--MOS管控制
// 				 g_State =ST_USB; return;		  //检测端口的电压是否为高电平。来判断是否已经连接到充电座
// 			 }
// 		}

// // 		if( j==0xfffe)
// // 		{
// // 			j =0;
// // 			Clear_WDT();
// // 			if(Power_Cheak() ==1)  //检测电池电压，如果电池电压过低，则进入关机模式，并进行提示
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
//  NVIC_InitTypeDef NVIC_InitStructure;   //定义一个结构体
// #ifdef  VECT_TAB_RAM  
//   /* Set the Vector Table base location at 0x20000000 */ 
//   NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
// #else  /* VECT_TAB_FLASH  */
//   /* Set the Vector Table base location at 0x08000000 */ 
//   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
// #endif
// 	
//  /**此段程序是初始化串口1的中断部分**/
// 	
//   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
//   //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	
// 	
//   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel; 	//中断向量
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//响应优先级
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//   NVIC_Init(&NVIC_InitStructure); 
// 	
//   /**UART2中断初始化部分**/
// 	
//   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//响应优先级
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
// 	
//   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;		//抢占优先级
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
// * 函数名： USART1_Config
// * 描述 ： USART1 GPIO 配置,工作模式配置。 115200 8-N-1
// * 输入 ：无
// * 输出 : 无
// * 调用 ：外部调用
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

