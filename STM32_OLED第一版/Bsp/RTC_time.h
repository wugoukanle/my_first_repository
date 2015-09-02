#ifndef __RTC_TIME_H
#define __RTC_TIME_H

#include "general.h"


typedef struct date_t
{
  u8 hour;
  u8 min;
  u8 sec;
  u8 month;
  u8 day;
  u8 wday;
  u16 year;
} date_tYPE ;
extern  date_tYPE date_s;
void RTC_init(void);
static u8 IsLeapYear(u16 nYear);
void Date_Update(void);
void set_time(u16 year,u8 month,u8 day,u8 hour,u8 min,u8 sec);
void GetWeekDay( date_tYPE * tm);
void read_time(void);
void RTC_Configuration(void);
void RCT_InitTime(void);
unsigned char hex_bcd(unsigned char hex);
u32 Time_Regulate(void);

#endif
