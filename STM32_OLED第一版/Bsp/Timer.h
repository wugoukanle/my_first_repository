#ifndef __TIMER_H
#define __TIMER_H


#include "general.h"


typedef unsigned short u16;
typedef unsigned char  u8;

extern u8 g_WaitTimeOut;

//Timer 部分的错误代码
#define ERR_TIMER_OK            0
#define ERR_TIMER_TOOMANYTASK   1
#define ERR_TIMER_NOSUCHTASK    2
#define ERR_TIMER_TASKEXIST     4
#define ERR_TIMER_PARAMETER     3
#define FLG_TASK_NORMAL         0x00
#define FLG_TASK_READY          0x01
#define FLG_TASK_DELAYED        0x02

#define TIMER_TASK_COUNT   3
#define TIMER_NOUSE        255  //禁止使用编号

//typedef  void (*TimerFun)(uchar para1, uint para2);
typedef  void (*TimerFun)(void);   //定义一个函数指针
void TIM2_IRQHandler(void);
typedef  struct tgTaskStuct 
{
    u8 flag;
    u8 timer_id;
    u16  interval;
    u16  count;
    TimerFun pFun;
} TASKSTRUCT;

void Init_Timer(void);
//安装定时任务
u8 SetTimer(u8 timer_id, u16 interval, TimerFun pTask,u16 Time_Top );
void KillTimer(u8 timer_id);//停止定时任务
void ResetTimer(u8 timer_id);//复位计时
extern TASKSTRUCT m_TS[TIMER_TASK_COUNT];


//#endif


void Start_Timer(u16 Interal);
void Stop_Timer(void);

#define  ONE_MIN    3907
#define  TOW_MIN    7814
#define  THREE_MIN  11721
#define  FOUR_MIN   15628
#define  HALF_MIN   1500
void second_timer(void);
void Do_TimeOut(void);
void Init_Timer0(void);
void Start_Timer0(void);
void Stop_Timer0(void);
void WaitTF0(uint interval);


#endif
