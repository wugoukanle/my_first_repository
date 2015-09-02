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
 与电子锁通信的IO口的初始化
 将此IO口设置为高电平状态，当检测到
 插入电子锁后，应转换为低电平状态,以此来检测
 锁的状态
*/
 void Key_IO_Init_In(void)
 {
 
 }
 void Key_IO_Init_Out(void)
 {
 
 }
 /**
电子锁的电源控制脚
 **/
  void Key_Power_Init(void)
 {
  
 }
 /**
电子锁的OE控制脚
 **/
  void Key_OE_Init(void)
 {
  
 }
/*
* function:Read_KeyInfo()
* 读取电子锁内部的信息，返回
* 1 -读取成功
* 0 -读取失败
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
  SetTimer(TIMER_TIMEOUT,1,Do_TimeOut,THREE_MIN);//等待电子锁发送过来的数据信息
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

模拟串口的定时器的初始化
时序要求比较严格
***********/
 /***
void Second_WByte(uchar input)
{
       //发送启始位
     uchar j=8;
	 uchar tempdata;
     //TR0=1;
     TXD_S_LOW;
     WaitTF0(TEN_MS);
	  WaitTF0(TEN_MS);
	   WaitTF0(TEN_MS);
     //发送8位数据位
     while(j--)
     {
			 TXD_S_LOW;
			 WaitTF0(TEN_MS);
			 tempdata =(input &0x01);
			 if(tempdata ==0x00)    //当为逻辑0
		 {
		 TXD_S_HIGH;
		 WaitTF0(SIX_MS);
		 TXD_S_LOW;
		 WaitTF0(FOUR_MS);
		 }
		 else if(tempdata ==0x01)  //当为逻辑1
		 {
		 TXD_S_HIGH;
		 WaitTF0(TEN_MS);
		 }
		 
         input=input>>1;
     }

     //发送校验位(无)

     //发送结束位
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
     //发送8位数据位
     while(j--)
     {
	     TXD_S_LOW;
         WaitTF0(TEN_MS);
	     tempdata =(input &0x01);
         if(tempdata ==0x00)    //当为逻辑0
		 {
		 TXD_S_HIGH;
		 WaitTF0(THREE_MS);
		 TXD_S_LOW;
		 WaitTF0(TOW_MS);
		 }
		 else if(tempdata ==0x01)  //当为逻辑1
		 {
		 TXD_S_HIGH;
		 WaitTF0(FIVE_MS);
		 }
		 
         input=input>>1;
     }

     //发送校验位(无)

     //发送结束位
     TXD_S_HIGH;
     WaitTF0(FIVE_MS);


}

void WByte(uchar input)
{
     //发送启始位
     uchar j=8;
	   uchar tempdata;
     //TR0=1;
     TXD_S_LOW;
     WaitTF0(FIVE_MS);
     //发送8位数据位
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

     //发送校验位(无)

     //发送结束位
     TXD_S_HIGH;
     WaitTF0(FIVE_MS);
     //TR0=0;
}    
 /*
  发送一组数据的函数

 */
void Key_Sendata(uchar dir[],uchar len)
{

     uchar i;
     for(i=0;i<len;i++)//外层循环，遍历数组
     {
					Clear_WDT();
          WByte(dir[i]);
					msDelay(5);  //这个延时是否可以再短些
     }
}

//接收一个字符
/*************
串口数据一般为10位，1位起始位，8位数据位，1位结束位
判断起始位的时候，采用一个超时，防止进入死的循环等待状态
接收数的时候尽可能的扫描位电平的中间处

______|----|_____|----|______|-----|_____
*************/
/***
 测试是否连接上锁
****/
uchar Test_RByte(void)
{
     uchar state=0;
		 KillTimer(TIMER_TIMEOUT);
  	 g_WaitTimeOut = 0;
     SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
     while( g_WaitTimeOut == 0 )  //等待返回的GPS头段的字节
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
     while( g_WaitTimeOut == 0 )  //等待返回的GPS头段的字节
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
	  WaitTF0(OVER_MS);//等过起始位
   // RED_BIT_HIGH;while(1);
     //接收8位数据位
	// OCR0 =90;
	 /*********测试程序段*****/
	  if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  //Output=>>1;
		 // rx_buff[0]=rx_buff[0]>>6;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>5;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>4;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>3;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>2;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>1;
	  }
	  Output=Output>>1;
      WaitTF0(FIVE_MS);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//先收低位
	  }
	  Clear_WDT();
      //WaitTF0(0X50);//位间延时,是否取消掉，来检测，最后一位
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
是否将接收第一个字节 与接收随后的字节区分开
测试接收第二个字节的函数
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
     while( g_WaitTimeOut == 0 )  //等待返回的GPS头段的字节
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
	  WaitTF0(0x0200);//等过起始位
  //  RED_BIT_HIGH;while(1);
     //接收8位数据位
	 //OCR0 =90;
	  Clear_WDT();
	 /*********测试程序段*****/
	  if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  //Output=>>1;
		 // rx_buff[0]=rx_buff[0]>>6;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>5;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>4;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>3;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		  Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>2;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//先收低位
		  //rx_buff[0]=rx_buff[0]>>1;
	  }
	  Output=Output>>1;
      WaitTF0(0x0200);//位间延时
	   if(RXD_S_DATA ==(GPIO_Pin_13))
	  {
		   Output|=0x80 ;//先收低位
	  }
     // WaitTF0();//位间延时
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
 *读钥匙编号
 *4个字节8位
 *回应帧
 * 接收电子锁发来的数据信息，暂时定为8个字节
 * 55+7a+01+编号_1+编号_2+编号_3+编号_4+xor(异或检验) 
 *level_data[2],level_data[3],level_data[4],level_data[5]存储的为电子锁的编号
********/
uchar Re_leveldata(uchar data_bit)
{
/**/
  uchar level_data[8];
  uchar nChar;
  uchar i;//开一个定时器，来进行超时控制
  uchar xor =0;
  data_state = 0;  //先将状态设定为0
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
   level_data[i] =RByte();  //全部接收完16个数据之后，再判定
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
 读取电子锁的前导码及锁的编号 ,如果收到前导码，
 并判断发送过来的锁的编号在本钥匙的控制范围内则继续，
 否则继续读取对方发送的前导码，
 如果发现锁不在本钥匙管理的范围内，如何处理？
 是否出现有上电抖动情况
 需要不停的转变IO口的工作模式，在输入和输出之间来回切换
 在电子锁的接收做的为死循环结构，如果通信异常，则本
 电子钥匙进行对电子锁的断电重启工作。
 读取到锁的编号后，需要判断是否在钥匙的管理权限之内，如果发现锁不是在此钥匙的管理范围内 如何处理？

 前端是否加一个判断是否有锁插入的函数？？
****/
/***


***/
//锁的数据处理函数
void Key_Trismit(void)
{
	 //uchar  Rfid_Code[4];  //RFID内码缓冲区
	 OLED_Clear();
	 Clear_WDT();
	 //often_show(16,56,"检测钥匙信息..",0,0x0fff);
	 often_show(16,40,"reading RFID",0,0x0fff);
	 Read_LeadCode();  	       //读前导码，对读取到的RFID数据进行分析处理
	 Clear_WDT();
	 g_State =ST_IDLE;
	 return;
}


/***
 读RFID阅读器（读卡器）传过来的数据，判断编号是否在控制的范围之内
 
 就是读RFID

****/
uchar  Read_FirstCode(void)
{
	uchar nChar,state;
	often_show(36,56,"读取钥匙信息..",0,0x0fff);		//这是锁，不是钥匙
 	while(1)
	{
		 Clear_WDT();
		 g_WaitTimeOut = 0;
		 SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
		 while( g_WaitTimeOut == 0 )  //等待返回的GPS头段的字节
	   {
	     Clear_WDT();
			 nChar =Read_RfidData(1,Rfid_Buff,0xff);		//从串口读取读卡器读到的卡片的信息
			 if(nChar >=3)
					break; 
     }		//退出这个while循环，有两种情况，1是break中断并退出循环，此时g_WaitTimeOut == 0，未超时，2是g_WaitTimeOut=true,即超时
	   KillTimer(TIMER_TIMEOUT);
	   if( g_WaitTimeOut == 0)		//表示是第一种情况
	   {
			 state =1;
			 Clear_WDT();
			 return state;		//一直读直到
	   }
	}
}
/**

正式读取锁中（EEPROM中）的信息，并判断锁是否符合要求

3个字节的锁的区位码
1个字节的锁的类型信息

即读取EEPROM中锁的区位码和锁的类型并与key_in_code[4]比较

**/

uchar Cheak_Lock_Local(void)
{
   uchar local_buff[4];
   uchar i;
//   uchar state =0;
   Clear_WDT();
   local_buff[0] =ReadData(LOCK_TYPE);		//读锁的类型，1个字节
   for(i =0;i<3;i++)
			local_buff[1+i] = ReadData(LOCK_LOCAL+i);		//读锁的区号，3个字节
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
	读取前导码

**/
void Read_LeadCode(void)
{
   uchar i,nChar;
	 //uchar flag=0;
	
//    for(i =0;i<4;i++)		//Lock_number[4]和Rfid_Buff[6]都是全局变量
//      Lock_number[i] =Rfid_Buff[i]; //在ST_Idle()中已经将数据放入Rfid_Buff[]
	
// 		while(1)
// 		{

// 				nChar =Read_RfidData(2,Rfid_Buff,0xff);

// 				if(nChar ==5) 
// 				{
// 					if(Rfid_Buff[0]==0&&Rfid_Buff[1]==0&&Rfid_Buff[2]==0&&Rfid_Buff[3]==0)
// 						;
// 					else		//至少有一个不为零
// 					{
// 						UARTnSendChar(1,Rfid_Buff,5);
// // 						flag=flag+1;
// // 						if(flag==2)
// 							break;
// 					}
// 				}
// 		}	
// 				UARTnSendChar(1,"55555",5);					

//在ST_Idle（）中已经检测到了RFID，才会调到这里

   for(i =0;i<4;i++)		//Lock_number[4]和Rfid_Buff[6]都是全局变量
     Lock_number[i] =Rfid_Buff[i]; //在ST_Idle()中已经将数据放入Rfid_Buff[]	
	
	
	//传递RFID锁的编号,靠UART2串口中断，将RFID数据放到放到缓冲区，
	//再由read_RfidData将数据读入Rfid_Buff[],这个函数在ST_Idle（）中调用过
   Clear_WDT();
   // if(xor ==g_Buff[8])  //当收到正确的数据的时候，发送确认码给电子锁
	    
			 
		//nChar= Cheak_New_LockNum();//Cheak_lock_Num();这个也是一个关键点
		nChar= check_suoma();		//匹配就为1；不匹配就为0	
		//UARTnSendChar(1,&nChar,1);			 
		UARTnSendChar(1,"22222",5);				  
		//nChar =1;		//只有读取的锁码在授权文件中，才返回1
		if(nChar ==0)
		{
			OLED_Clear();
			often_show(36,56,"未授权",0,0x0fff);						
			
			delay_ms(1500);
			Clear_WDT();
			
			g_State =ST_IDLE;
			shouquan_jieguo=2;		//返回授权中需要的状态结果			
			
		  OperateKey();                        //将锁码及开锁状态写入存储芯片中
			
			g_State =ST_IDLE;
			shouquan_jieguo=2;		//返回授权中需要的状态结果
			return;
		}		
		else if(nChar==1)
		{ 				
			Clear_WDT();
			//Lcd_Show_IDinfo();  //显示打开的锁的ID信息	
			often_show(16,64,"被打开锁的ID:",0,0x0fff);
      display_shuzi(32,80, Lock_number, Green, Yellow);

			KEY_POWER_ON;
			often_show(36,100,"已授权",0,0x0fff);					
			
			delay_ms(1500);
			
			shouquan_jieguo=1;
			//Order_Transmit();  //通过指令对电子锁进行各种操作
			OperateKey();                        //将锁码及开锁状态写入存储芯片中
			
			KEY_POWER_OFF;//关闭SPX2941电源，即关闭钥匙动作过程
			
			UARTnSendChar(1,"33333",5);						
			g_State =ST_IDLE;
			Clear_WDT();
			return;
		}
		
}
/****
 电子锁和电子钥匙之间的命令通信

 在此时IO口已经是输出状态，不需要重新设定
 电子钥匙打开锁之后，则延时一段时间返回空闲状态

 电子锁收到打开的指令后，则进入无限的空操作
 等待电子锁--------断电
****/
void Order_Transmit(void)
{

  Clear_WDT();
  {
			KEY_POWER_ON;		 //打开SPX2941电源，即TPS79633的SP-EN，J8，其功能就是开锁
			{
				Clear_WDT();
				//Save_Lock_doing();							   //将锁码及开锁状态写入存储芯片中
				OperateKey();                        //将锁码及开锁状态写入存储芯片中
			}
			KillTimer(TIMER_TIMEOUT);                         //少了一句关定时器
			/**/
			Clear_WDT();
			g_WaitTimeOut = 0;
			SetTimer(TIMER_TIMEOUT,20,Do_TimeOut,THREE_MIN);   //等待钥匙拔出的过程  此时间需要重新计算
			while(g_WaitTimeOut == 0)
			{
				Clear_WDT();
			}
			KillTimer(TIMER_TIMEOUT);
			/***/
			KEY_POWER_OFF;//关闭SPX2941电源，即关闭钥匙动作过程
			g_State =ST_IDLE;		//返回空闲状态
			Clear_WDT();
			return;
   }
}
     
/***
 增加的写锁的区位码的函数
****/ 
void  Write_LockNum(void)
{
		uchar nChar;
		Clear_WDT();
		nChar = Read_FirstCode();
		if(nChar ==0)		//表示没有读到锁的ＲＦＩＤ
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
		//Shengya_Power_Off();		//不使能TPS97633，即J8无输出
		for(i =0;i<0xff;i++)
			msDelay(0xfff);
		TXD_S_HIGH;
		Key_IO_Init_In();  //将IO口置为输入口
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
		if(j >=10)        //如果超时没有收到前导码的话，则返回
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
		Lock_number[i] =g_Buff[i]; //传递锁的编号
		for(i =0;i<4;i++)
		key_in_code[i] =g_Buff[4+i]; //存储锁的区位码及类型
		Clear_WDT();
		if(xor ==g_Buff[8])  //当收到正确的数据的时候，发送确认码给电子锁
		{
			Clear_WDT();
			Key_IO_Init_Out();
			TXD_S_HIGH;

			 for(i =0;i<0xff;i++)
			 {
				 Clear_WDT();
				 msDelay(0xff);    //将IO口转换为输出态，发送确定码给对方
				 msDelay(0xff); 
				 msDelay(0xff); 
				 msDelay(0xff); 
				 msDelay(0xff); 
			 }
			 //WByte(0xaa);
			 Second_WByte(0xaa);
			 Clear_WDT();
			 /***/
			 Write_order();  //通过指令对电子锁进行各种操作
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
效验码为异或的结果
0x01 --开锁指令
0x02 --写锁编号
0X03 --写锁的区位码
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
	 msDelay(0xff);    //将IO口转换为输出态，发送确定码给对方
	 }
	 //while(1)
	  //Second_WByte( 0xaa);
  Clear_WDT();
  for(i =0;i<7;i++)
  Second_WByte( lock_data[i]);
  Clear_WDT();
 }
/****
检测锁是否在钥匙的管理范围内
lock_num1 --- 最大值 
lock_num2 --- 最小值
lock_num3 --- 读取到的锁的编号值
50个用户作为最小的查询间隔，需要测试看效果如何
 这是500用户刚好存满的情况，如果不够500户 则如何处理？
 第一种方案： 不够500户的补齐到500进行检测
****/

// uchar Cheak_New_LockNum(void)
// {
//    uchar i,nChar;
//    uchar Mid_text[6];
//    uint i2c_addr;		//整数转字符的地址
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
//    i2c_addr =Ti[i+1];		//Ti在main()函数中初始化时从SANBIAO_START中读出放入Ti
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
// 	 //读取授权
//    for(i =0;i<6;i++)
// 			Mid_text[i] = ReadData(i2c_addr+i);//外部eeprom中通过SPI总线取数据
// 	 
// 	 
// 	 
// 	 
//    if((Mid_text[4] ==0xfe) &&(Mid_text[5] ==0xfe))  //检测注销的情况
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
//    nChar = XunHuan();			//为什么还需要循环的检测了
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

自定义函数
检查读取到的锁码是否在授权文件当中

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
// 			return 1;		//读到了就返回1
// 		else		
// 			return 0;

// }



/*

自定义函数
检查读取到的锁码是否在授权文件当中

*/
uchar check_suoma(void)
{
		uchar i,j,total_row;
		uchar known_suoma[4];
		uint addr_end,basic_addr;

		//addr= ReadData(AUTHOR_START);  

		addr_end= ReadData(AUTHOR_POINT_H);   
		addr_end =((addr_end<<8)&0xff00)+ReadData(AUTHOR_POINT_L);		//在界面的采码功能里会改变采码指针的值
		
		total_row=(addr_end-AUTHOR_START)/24;
		
		if(total_row==0)
		{
			UARTnSendChar(1,"00000",5);		//如果total_row=0，	
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
					
					return 1;		//读到了就返回1
				}
		}
		
		return 0;

}





/**
 定义两个全局变量
MI_High
MI_Low
是否也需要加一个超时的判断
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
  {				       //检测锁的区位码和类型是否符合要求
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
		Lock_number[1] =0x00;	   //测试部分
		Lock_number[2] =0x06;
		Lock_number[3] =0x50;
  }
  for(i =0;i<4;i++)
		lock_num3[i] =Lock_number[i];
  for(i =0;i<4;i++)
		lock_num1[i] =ReadData(KEY_NAME_MAX+i);
  nChar  =Float_subtract(lock_num1,lock_num3,term);	  //比较最大值
  if(nChar ==0)
		return 0;
  term =2;
  for(i =0;i<4;i++)
		lock_num2[i] =ReadData( KEY_NAME_MIN+i);
  nChar  =Float_subtract(lock_num3,lock_num2,term);	 //比较最小值
  if( nChar ==0)
		return 0;
  term =3;
  for(i =0;i<4;i++)
		lock_num1[i] =ReadData(KEY_NAME_TEN+i);
  nChar  =Float_subtract(lock_num3,lock_num1,term);
  if(nChar ==0)       //小于中间值
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
  else if(nChar ==3)  //大于中间值
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
 在50个用户中进行对比，判断 最小的间隔
 如果查到有此锁的编号则返回1
 否则返回0
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
检测锁的编号是否在钥匙控制的范围内
在钥匙内部的锁的编号的排列
必须是有序的，最好是从小到大的顺序
先比较 最大和最小 然后比较 最小和中间  最大和中间
str2 <str1时，返回的值不为0
str1 = 00 00 02 46
str2 = 00 00 01 01
term =1;
*****/

uchar Float_subtract(uchar str1[],uchar str2[],uchar term) //ok
{
  uchar State =0;
  if(str2[0] <str1[0])  //第一种情况，整数部分超限
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

存储锁的编号及开锁时间到钥匙中

主要是为了存储操作记录,现在这个函数可以不要

****/
void  Save_Lock_doing(void)
{
  //uchar mid_buff[10];
  //uchar i;
  /**
   {
  Lock_number[0] =0x00;
  Lock_number[1] =0x00;	   //测试部分
  Lock_number[2] =0x06;
  Lock_number[3] =0x50;
  }
  ***/
	
  OperateKey();
	
  Clear_WDT();
}



 /**

// 初始化钥匙操作的头指针 ---如何设定在什么情况下重新设置 y

// 初始化EEPROM中定义的宏的初始值

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
// 		WritEDAta(SANBIAO_START+j,0xff);		//相当于复位

// 	}
// 	Clear_WDT();
// 	for(k =0;k<80;k++)
// 		msDelay(500);
// 	for(k =0;k<246;k++)
// 		Ti[k] =ReadData(SANBIAO_START+k);	//开机的时候将，数据缓冲区给读出来，放在这里有意义吗？
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
//   WritEDAta(HEAD_BIT,0x01);		//HEAD_BIT头指针初始化标志位
// 	
//   for(k =0;k<WRITE_ZHI;k++)
// 		msDelay(500);
//   Clear_WDT();
//  
//   
// }

/*

改写上面的Init_KeyRecord（）

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
  WritEDAta(KEY_ENCODE,0x00);		//KEY_ENCODE头指针初始化标志位
	
	
	
  for(k =0;k<WRITE_ZHI;k++)
		msDelay(500);
  Clear_WDT();
 
  
}


/***
 如何检验错误
 目前存储的数据为200户
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
// 		nChar =ReadData(KEY_RECORD_START+j);	  //测试读写部分是否有误--测试
// 		UARTSendChar(1,nChar);
// 	}


// }
 /***

  检测存储的头指针是否有误	,可能有问题
 ****/
void Cheak_I2cHead(void)
{

  uint  head,consult,row;
  uchar left,k,new_row;
  Clear_WDT();
  row= ReadData(KEY_RECORD_ROW_H);   //已存储的组数
  row =((row<<8)&0xff00)+ReadData(KEY_RECORD_ROW_L);
  head= ReadData(KEY_RECORD_INDEX_H);   //已存储的组数
  head =((head<<8)&0xff00)+ReadData(KEY_RECORD_INDEX_L);
  if(head >=0x1782)
  consult =(head -0x1782);   //0x1782为开锁信息的存储的头地址
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
  left =consult%10;     //left为余数 ，看是否为零
  if(left ==0)
  {
    Clear_WDT();
	  return;
  }
  else if(left !=0)
  {
    Clear_WDT();
	  head =consult-left+0x1782;
	  if( row >=2000)   //存储满2000组的情况
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
	  else if(row <2000) //存储未满2000组的情况
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
测试锁信息存储
****/
// void Test_SaveSuo(void)
// {
//   uchar k;
//   uint j;
// //  uint row;
//   //uint head;
//   uchar test_buff[10] ={0x55,0x7a,0x44,0x44,0x33,0x22,0x11,0x01,0x02,0x77}; //测试的用户数据
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
// 	g_Buff[0]= ReadData(KEY_RECORD_ROW_H);   //已存储的组数
// 	g_Buff[1] =	ReadData(KEY_RECORD_ROW_L);
// 	g_Buff[2]= ReadData(KEY_RECORD_INDEX_H);   //已存储的组数
// 	g_Buff[3]=	ReadData(KEY_RECORD_INDEX_L);
// 	Clear_WDT();
// 	while(1);


// }
/****

将4（RFID）+6（time）字节的操作记录写入EEPROM

****/
void OperateKey(void)  //操作钥匙
{
	uint row;
	uint head;
	uchar i,k;
	uchar new_row;
	Clear_WDT();
	
	//Cheak_I2cHead();   //此函数有问题
	
	for(k =0;k<WRITE_ZHI;k++)
    msDelay(500);
	Clear_WDT();
  row= ReadData(RECORD_ROW_H);   //已存储的组数
	row =((row<<8)&0xff00)+ReadData(RECORD_ROW_L);
	
//   head= ReadData(RECORD_POINT_H);   //已存储的组数的指针
// 	head =((head<<8)&0xff00)+ReadData(RECORD_POINT_L);
	head=RECORD_START;
	
	for(i=0;i<row;i++)		//现在最多有25条授权
	{
			if(ReadData(head)==Lock_number[0]&&ReadData(head+1)==Lock_number[1]&&ReadData(head+2)==Lock_number[2]&&ReadData(head+3)==Lock_number[3])		//根据用户编号查找用户的地址
				break;
			head=head+5;		//现在一条授权的总长度为24字节
	}	
	
	if(i<row)	//表示找到了相同锁码的记录，只是重写一遍
	{
			//写读到的RFID
			Clear_WDT();
			for( i=0;i<4;i++)	//str[]的大小就为10
			{
				Clear_WDT();
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(head+i,Lock_number[i]);		//通过SPI总线写数据到eeprom中
			}	
			//写授权检验结果
			Clear_WDT();	
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(head+i,shouquan_jieguo);		//通过SPI总线写数据到eeprom中	


// 			head=head+5;		//KEY_RECORD的下一个地址	
// 			if(head >=RECORD_END)
// 				head =RECORD_START;		//如果到了	
// 			
// 			Clear_WDT();
// 			//更新RECORD_ROW的值
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			if(row <256)		//只用到了RECORD_ROW_L
// 				row++;		//行表示操作记录的条数
// 			new_row =(row&0x00ff);		//低8位
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			WritEDAta(RECORD_ROW_L,new_row);
// 			
// 			
// 			Clear_WDT();	
// 			new_row =(row&0xff00)>>8;		//高8位
// 			for(k =0;k<WRITE_ZHI;k++)
// 				msDelay(500);
// 			WritEDAta(RECORD_ROW_H,new_row);
// 			
// 			Clear_WDT();	
// 			//更新RECORD_POINT的值
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
	else if(i>=row)		//表示没有找到,就在指针处写（即新位置写）
	{
			head= ReadData(RECORD_POINT_H);   //已存储的组数的指针
			head =((head<<8)&0xff00)+ReadData(RECORD_POINT_L);			
		
			//写读到的RFID
			Clear_WDT();
			for( i=0;i<4;i++)	//str[]的大小就为10
			{
				Clear_WDT();
				for(k =0;k<WRITE_ZHI;k++)
					msDelay(500);
				WritEDAta(head+i,Lock_number[i]);		//通过SPI总线写数据到eeprom中
			}	
			//写授权检验结果
			Clear_WDT();	
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(head+i,shouquan_jieguo);		//通过SPI总线写数据到eeprom中	


			head=head+5;		//KEY_RECORD的下一个地址	
			if(head >=RECORD_END)
				head =RECORD_START;		//如果到了	
			
			Clear_WDT();
			//更新RECORD_ROW的值
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			if(row <256)		//只用到了RECORD_ROW_L
				row++;		//行表示操作记录的条数
			new_row =(row&0x00ff);		//低8位
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(RECORD_ROW_L,new_row);
			
			
			Clear_WDT();	
			new_row =(row&0xff00)>>8;		//高8位
			for(k =0;k<WRITE_ZHI;k++)
				msDelay(500);
			WritEDAta(RECORD_ROW_H,new_row);
			
			Clear_WDT();	
			//更新RECORD_POINT的值
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
 读取新的通信协议的模式
 用低电平代表起始
 用高电平的长度代表 0 1

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
         KillTimer(TIMER_TIMEOUT);                         //少了一句关定时器
	  	   g_WaitTimeOut = 0;
         SetTimer(TIMER_TIMEOUT,1,Do_TimeOut,THREE_MIN);   //等待钥匙拔出的过程
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
   SetTimer(TIMER_TIMEOUT,30,Do_TimeOut,THREE_MIN);   //等待钥匙拔出的过程
   while((RXD_S_DATA ==0)&&(g_WaitTimeOut == 0))	   //增加一个超时处理纠错机制
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
   WaitTF0(FOUR_MS);//等过起始位
   Clear_WDT();
   //if(RXD_S_DATA ==0)
   {
	 //return 0xff;
   }

  // WaitTF0(FOUR_MS);//等过起始位
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
 一定注意在端口切换时的电平抖动。切记，切记
 此处是否有问题
***/
uchar Read_New_Code(void)
 {

   uchar i;
	 for(i =0;i<5;i++)
	 {
	   Clear_WDT();
		 g_WaitTimeOut = 0;
     SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
     while( g_WaitTimeOut == 0 )  //等待返回的GPS头段的字节
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
	 WaitTF0(TOW_MS);//等过起始位
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
	uchar residue;   //余数
	uchar quotient;  //商
    uchar head;
	 Clear_WDTDOG();
	globle_row= SubI2c_Read(0x50);   //已存储的组数
    globle_head= SubI2c_Read(0x51);  //数据块头指针
    globle_page= SubI2c_Read(0x52);  //页指示
	head =SubI2c_Read(0x51); 
	if((globle_page ==0x00)||(globle_page ==0x01))
	{
	  if((globle_row <=20)&&(globle_page ==0x00))   //未存满，在第一页
	  {
	    if((head >=0x60)&&(head <=0xff))  //头指针大于初始指针
		{
		  Clear_WDTDOG();
		 quotient =(head-0x60)/8; //商的情况
		 globle_head =0x60+quotient*8;
		 globle_row =quotient;
		  Clear_WDTDOG();
		}
		else           //若小于初始指针则有误
		{
		 globle_head =0x60;
		 globle_row =0;
		 globle_page=0;
		}
	  }
	  else if((globle_row >=20)&&(globle_row <40)&&(globle_page ==0x01))//未存满。在第二页
	  {
	    if((head >=0x00)&&(head <=0x9e))  //头指针大于初始指针
		{
		  Clear_WDTDOG();
		 globle_head =head+0x100;
		 quotient =(globle_head-0x60)/8; //商的情况
		 globle_head =0x60+quotient*8;
		 globle_row =quotient;
		  Clear_WDTDOG();
		}
		else           //若小于初始指针则有误
		{
		 globle_head =0x60;
		 globle_row =0;
		 globle_page=0;
		}
	  }
	  else if(globle_row >=40)   //已存满
	  {
	    globle_row =40;
		if((globle_page ==0x00)&&(globle_head >=0x60)&&(globle_head <=0xff))
		{
		  Clear_WDTDOG();
		 quotient =(globle_head-0x60)/8; //商的情况
		 globle_head =0x60+quotient*8;
		}
		else if((globle_page ==0x01)&&(head <=0x9e)&&(head >=0x00))
		{
		  Clear_WDTDOG();
		 globle_head =head+0x100;
		 quotient =(globle_head-0x60)/8; //商的情况
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
