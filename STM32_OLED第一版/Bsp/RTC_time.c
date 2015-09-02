#include "RTC_time.h"

#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "Globle_Var.h"

#include "App.h"

 date_tYPE date_s;
 unsigned char hex_bcd(unsigned char hex){return (((hex/10)<<4)+(hex%10));}
/******
 说明：
	对RTC时钟进行校正，通过软件来处理年月日时分秒的计算。
	同时在CPU断电的情况下，对时钟的运行进行校正。

*******/

/*****************************************************************************************
** 函数名称：
** 函数功能：
** 入口参数：
** 返 回 值：
** 编    写：        日    期：      版 本 号：
** 修改历史：
******************************************************************************************/	 
void RTC_init(void)
{
  /* PWR and BKP clocks selection --------------------------------------------*/
 
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xAAAA)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */
    /* RTC Configuration */
    RTC_Configuration();
    BKP_WriteBackupRegister(BKP_DR1, 0xAAAA);
    //set_time(2010,8,21,13,45,5);   
	set_time(0x12,0x10,0x10,0x15,0x35,0x23);  
  }
  else
  {
  PWR_BackupAccessCmd(ENABLE);
  RTC_WaitForSynchro();         //等待RTC时钟同步
  RTC_WaitForLastTask();        //等待RTC写操作完成
  }

// Enable the RTC Second 
RTC_ITConfig(RTC_IT_SEC, ENABLE);
// Wait until last write operation on RTC registers has finished */
RTC_WaitForLastTask();
//Clear reset flags */
RCC_ClearFlag(); 
//set_time(2011,8,12,12,20,0); 
}
/*****************************************************************************************
** 函数名称：static uint8_t IsLeapYear(uint16_t nYear)
** 函数功能：判断平年润年
** 入口参数：
** 返 回 值：
** 编    写：        日    期：      版 本 号：
** 修改历史：
******************************************************************************************/	 
static u8 IsLeapYear(u16 nYear)
{
  if(nYear % 4 != 0) return 0;
  if(nYear % 100 != 0) return 1;
  return (u8)(nYear % 400 == 0);
}
/*****************************************************************************************
** 函数名称：void Date_Update(void)
** 函数功能：更新时间
** 入口参数：
** 返 回 值：
** 编    写：        日    期：      版 本 号：
** 修改历史：
******************************************************************************************/	 
void Date_Update(void)
{
  u32 tmp = 0;

  if(date_s.month == 1 || date_s.month == 3 || date_s.month == 5 || date_s.month == 7 ||
     date_s.month == 8 || date_s.month == 10 || date_s.month == 12)
  {
    if(date_s.day < 31)
    {
      date_s.day++;
    }
    /* Date structure member: date_s.day = 31 */
    else
    {
      if(date_s.month != 12)
      {
        date_s.month++;
        date_s.day = 1;
      }
      /* Date structure member: date_s.day = 31 & date_s.month =12 */
      else
      {
        date_s.month = 1;
        date_s.day = 1;
        date_s.year++;
      }
    }
  }
  else if(date_s.month == 4 || date_s.month == 6 || date_s.month == 9 ||
          date_s.month == 11)
  {
    if(date_s.day < 30)
    {
      date_s.day++;
    }
    /* Date structure member: date_s.day = 30 */
    else
    {
      date_s.month++;
      date_s.day = 1;
    }
  }
  else if(date_s.month == 2)
  {
    if(date_s.day < 28)
    {
      date_s.day++;
    }
    else if(date_s.day == 28)
    {
      /* Leap year */
      if(IsLeapYear(date_s.year))
      {
        date_s.day++;
      }
      else
      {
        date_s.month++;
        date_s.day = 1;
      }
    }
    else if(date_s.day == 29)
    {
      date_s.month++;
      date_s.day = 1;
    }
  }

  BKP_WriteBackupRegister(BKP_DR2, date_s.year);

  tmp = date_s.month << 8;
  tmp |= date_s.day; 
  BKP_WriteBackupRegister(BKP_DR3, tmp);
}
/*****************************************************************************************
** 函数名称：void set_time(u16 year,u8 month,u8 day,u8 hour,u8 min,u8 sec)
** 函数功能：设定时间
** 入口参数：
** 返 回 值：
** 编    写：        日    期：      版 本 号：
** 修改历史：
******************************************************************************************/	 
void set_time(u16 year,u8 month,u8 day,u8 hour,u8 min,u8 sec)
{
u16 buf=0;
u32 RTC_CONTER=0;
Clear_WDT();
buf = month<< 8;
buf |= day; 
hour =((hour >>4)&0x0f)*10+(hour &0x0f);
min =((min >>4)&0x0f)*10+(min &0x0f);
sec =((sec >>4)&0x0f)*10+(sec &0x0f);
BKP_WriteBackupRegister(BKP_DR3, buf);  //写入,月,日	 (备份寄存器)
BKP_WriteBackupRegister(BKP_DR2, year); //写入年
Clear_WDT();
RTC_CONTER=hour*3600+min*60+sec;
RTC_SetCounter(RTC_CONTER);             //写入小时分钟秒
Clear_WDT();

//RTC_WaitForLastTask();
}
/*****************************************************************************************
** 函数名称oid GregorianDay(struct rtc_time * tm)
** 函数功能：读取星期
** 入口参数：
** 返 回 值：
** 编    写：        日    期：      版 本 号：
** 修改历史：
******************************************************************************************/	 
void GetWeekDay( date_tYPE * tm)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };
	 Clear_WDT();
	lastYear=tm->year-1;

	/*
	 * Number of leap corrections to apply up to end of last year
	 */
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;

	/*
	 * This year is a leap year if it is divisible by 4 except when it is
	 * divisible by 100 unless it is divisible by 400
	 *
	 * e.g. 1904 was a leap year, 1900 was not, 1996 is, and 2000 will be
	 */
	if((tm->year%4==0) &&
	   ((tm->year%100!=0) || (tm->year%400==0)) &&
	   (tm->month>2)) {
		/*
		 * We are past Feb. 29 in a leap year
		 */
		day=1;
	} else {
		day=0;
	}

	day += lastYear*365 + leapsToDate + MonthOffset[tm->month-1] + tm->day;
	Clear_WDT();
	tm->wday=day%7;
	Clear_WDT();
}

/*****************************************************************************************
** 函数名称：void read_time(void)
** 函数功能：读取时间
** 入口参数：
** 返 回 值：
** 编    写：        日    期：      版 本 号：
** 修改历史：
** LUO的程序中，应该已经包括如何对付长久停电的情况???
*没有读取日期的函数
******************************************************************************************/	 
void read_time(void)
{
	u32 RTC_CONTER=0;
	u32 i=0;
	Clear_WDT();
	/* Initialize Date structure */
	date_s.month = (BKP_ReadBackupRegister(BKP_DR3) & 0xFF00) >> 8;
	date_s.day = (BKP_ReadBackupRegister(BKP_DR3) & 0x00FF);
	date_s.year = BKP_ReadBackupRegister(BKP_DR2);
	RTC_CONTER=RTC_GetCounter();
	RTC_CONTER=RTC_CONTER%86399;
	date_s.hour=hex_bcd(RTC_CONTER / 3600);
	date_s.min=hex_bcd((RTC_CONTER % 3600) / 60);
	date_s.sec=hex_bcd((RTC_CONTER % 3600) % 60);
	if(RTC_GetCounter() / 86399 != 0)
	{//时间大于一天，更新日期
		 for(i = 0; i < (RTC_GetCounter() / 86399); i++)
		 {
			Clear_WDT();
		  Date_Update();
		 }
		 RTC_SetCounter(RTC_GetCounter() % 86399);   //把天以上的计数清0 ，保持时分秒的计数
		 //RTC_WaitForLastTask();
	}
 Clear_WDT();
 time_buff[0] =date_s.year;		//time_buff[6]是全局变量
 time_buff[1] =date_s.month;
 time_buff[2] =date_s.day;
 time_buff[3] =date_s.hour;
 time_buff[4] =date_s.min;
 time_buff[5] =date_s.sec;
 Clear_WDT();
}


/*****************************************************************************************
** 函数名称：void RTC_Configuration(void)
** 函数功能：第一次使用RTC时配置RTC
** 入口参数：
** 返 回 值：
** 编    写：        日    期：      版 本 号：
** 修改历史：
******************************************************************************************/	 
void RTC_Configuration(void)
{
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
  /* Allow access to BKP Domain */

  PWR_BackupAccessCmd(ENABLE);
  /* Reset Backup Domain */
  //BKP_DeInit();	//好像会造成复位的情况

  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  
  /* Enable the RTC Second */  
  // RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  //RTC_WaitForLastTask();
  
  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
  
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/*******************************************************************************
本装置的校时，是通过USB通信进行的
* Function Name  : Time_Regulate
* Description    : Returns the time entered by user, using Hyperterminal.
* Input          : None
* Output         : None
* Return         : Current time RTC counter value
*******************************************************************************/
u32 Time_Regulate(void)
{
 
  u32 Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
   Clear_WDT();
 while (Tmp_HH == 0xFF)
  {
    Tmp_HH =23; //= USART_Scanf(23);
  }
   while (Tmp_MM == 0xFF)
  {
    Tmp_MM =59; //= USART_Scanf(59);
  }
 while (Tmp_SS == 0xFF)
  {
    Tmp_SS =59; //= USART_Scanf(59);
  }
 /* Return the value to store in RTC counter register */
 Clear_WDT();
  return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}
 /*****************************************************************************************
** 函数名称：void RTC_Configuration(void)
** 函数功能：在电钥匙重新上电的时候，对RTC时钟进行校正，通过存储于BKP备份寄存器中的数值
** 入口参数：
** 返 回 值：
** 编    写：        日    期：      版 本 号：
** 修改历史：
******************************************************************************************/	
void RCT_InitTime(void)
{



}
