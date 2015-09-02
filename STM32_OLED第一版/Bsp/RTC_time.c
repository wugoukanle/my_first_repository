#include "RTC_time.h"

#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "Globle_Var.h"

#include "App.h"

 date_tYPE date_s;
 unsigned char hex_bcd(unsigned char hex){return (((hex/10)<<4)+(hex%10));}
/******
 ˵����
	��RTCʱ�ӽ���У����ͨ�����������������ʱ����ļ��㡣
	ͬʱ��CPU�ϵ������£���ʱ�ӵ����н���У����

*******/

/*****************************************************************************************
** �������ƣ�
** �������ܣ�
** ��ڲ�����
** �� �� ֵ��
** ��    д��        ��    �ڣ�      �� �� �ţ�
** �޸���ʷ��
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
  RTC_WaitForSynchro();         //�ȴ�RTCʱ��ͬ��
  RTC_WaitForLastTask();        //�ȴ�RTCд�������
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
** �������ƣ�static uint8_t IsLeapYear(uint16_t nYear)
** �������ܣ��ж�ƽ������
** ��ڲ�����
** �� �� ֵ��
** ��    д��        ��    �ڣ�      �� �� �ţ�
** �޸���ʷ��
******************************************************************************************/	 
static u8 IsLeapYear(u16 nYear)
{
  if(nYear % 4 != 0) return 0;
  if(nYear % 100 != 0) return 1;
  return (u8)(nYear % 400 == 0);
}
/*****************************************************************************************
** �������ƣ�void Date_Update(void)
** �������ܣ�����ʱ��
** ��ڲ�����
** �� �� ֵ��
** ��    д��        ��    �ڣ�      �� �� �ţ�
** �޸���ʷ��
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
** �������ƣ�void set_time(u16 year,u8 month,u8 day,u8 hour,u8 min,u8 sec)
** �������ܣ��趨ʱ��
** ��ڲ�����
** �� �� ֵ��
** ��    д��        ��    �ڣ�      �� �� �ţ�
** �޸���ʷ��
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
BKP_WriteBackupRegister(BKP_DR3, buf);  //д��,��,��	 (���ݼĴ���)
BKP_WriteBackupRegister(BKP_DR2, year); //д����
Clear_WDT();
RTC_CONTER=hour*3600+min*60+sec;
RTC_SetCounter(RTC_CONTER);             //д��Сʱ������
Clear_WDT();

//RTC_WaitForLastTask();
}
/*****************************************************************************************
** �������ƣvoid GregorianDay(struct rtc_time * tm)
** �������ܣ���ȡ����
** ��ڲ�����
** �� �� ֵ��
** ��    д��        ��    �ڣ�      �� �� �ţ�
** �޸���ʷ��
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
** �������ƣ�void read_time(void)
** �������ܣ���ȡʱ��
** ��ڲ�����
** �� �� ֵ��
** ��    д��        ��    �ڣ�      �� �� �ţ�
** �޸���ʷ��
** LUO�ĳ����У�Ӧ���Ѿ�������ζԸ�����ͣ������???
*û�ж�ȡ���ڵĺ���
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
	{//ʱ�����һ�죬��������
		 for(i = 0; i < (RTC_GetCounter() / 86399); i++)
		 {
			Clear_WDT();
		  Date_Update();
		 }
		 RTC_SetCounter(RTC_GetCounter() % 86399);   //�������ϵļ�����0 ������ʱ����ļ���
		 //RTC_WaitForLastTask();
	}
 Clear_WDT();
 time_buff[0] =date_s.year;		//time_buff[6]��ȫ�ֱ���
 time_buff[1] =date_s.month;
 time_buff[2] =date_s.day;
 time_buff[3] =date_s.hour;
 time_buff[4] =date_s.min;
 time_buff[5] =date_s.sec;
 Clear_WDT();
}


/*****************************************************************************************
** �������ƣ�void RTC_Configuration(void)
** �������ܣ���һ��ʹ��RTCʱ����RTC
** ��ڲ�����
** �� �� ֵ��
** ��    д��        ��    �ڣ�      �� �� �ţ�
** �޸���ʷ��
******************************************************************************************/	 
void RTC_Configuration(void)
{
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
  /* Allow access to BKP Domain */

  PWR_BackupAccessCmd(ENABLE);
  /* Reset Backup Domain */
  //BKP_DeInit();	//�������ɸ�λ�����

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
��װ�õ�Уʱ����ͨ��USBͨ�Ž��е�
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
** �������ƣ�void RTC_Configuration(void)
** �������ܣ��ڵ�Կ�������ϵ��ʱ�򣬶�RTCʱ�ӽ���У����ͨ���洢��BKP���ݼĴ����е���ֵ
** ��ڲ�����
** �� �� ֵ��
** ��    д��        ��    �ڣ�      �� �� �ţ�
** �޸���ʷ��
******************************************************************************************/	
void RCT_InitTime(void)
{



}
