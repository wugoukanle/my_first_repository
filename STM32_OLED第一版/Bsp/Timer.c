
#include "general.h"
#include "timer.h"
#include "stm32f10x_tim.h"

extern u8 g_WaitTimeOut;
/*****
 调用的shi timer2  normal
******/
u8       TimeInitialized = 0;
TASKSTRUCT  m_TS[TIMER_TASK_COUNT];
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
 //定时器采用向上计数的方式
void Init_Timer(void)
{
    u16 idx;
    TASKSTRUCT *pTS;          //定义一个结构指针 
		TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Prescaler = 0xFF;	 //更改此处可以降低TIMER的输入频率
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFE;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
		TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
		TIM_Cmd(TIM2,DISABLE);    //关闭定时计数器
    pTS = &m_TS[0];           //定义的定时器数组
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)
    {
        pTS->flag = FLG_TASK_NORMAL;
        pTS->timer_id = TIMER_NOUSE;
        pTS->interval = 0;
        pTS->count = 0;
        pTS->pFun = 0;
        ++pTS;
    }
    TimeInitialized = 0;
}

void second_timer(void)
{
	{
//    TASKSTRUCT *pTS;          //定义一个结构指针 
		TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Prescaler = 0xFFF0;	 //更改此处可以降低TIMER的输入频率
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFE;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
		TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
		TIM_Cmd(TIM2,DISABLE);    //关闭定时计数器
	}


}
/**********************************/
/*安装定时任务
	配置的是TIM2
	TimerFun是一个函数指针，
*/
u8 SetTimer(u8 timer_id, u16 interval, TimerFun pTask,u16 Time_Top )
{
    u16 idx;
    TASKSTRUCT *pTS;
    if (pTask==0 || interval==0 || timer_id == TIMER_NOUSE)//参数错误返回
    {
        return ERR_TIMER_PARAMETER;
    }

    TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能                        //关闭中断
    pTS = &m_TS[0];		//m_TS[TIMER_TASK_COUNT]
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)   //----任务错误返回----//
    {
        if (pTS->timer_id == timer_id)
        {
            TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能                 //打开中断
            return  ERR_TIMER_TASKEXIST;
        }else{
            if (pTS->pFun == 0)
            {
                if (!TimeInitialized)  //时间的初始化
                {
                    Start_Timer(Time_Top);
                }
                pTS->flag = FLG_TASK_NORMAL;
                pTS->timer_id = timer_id;
                pTS->interval = interval;
                pTS->count = 0;
                pTS->pFun = pTask;
                TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能     //打开中断
                return ERR_TIMER_OK;
            }
       }
       ++pTS;
    }
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能      //打开中断
	// TIM_Cmd(TIM1,ENABLE);
    return ERR_TIMER_TOOMANYTASK;
}
/***********************************************/
void KillTimer(u8 timer_id)  //停止定时功能
{
    u16 idx,cnt=0;
    TASKSTRUCT *pTS;

    TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
    pTS = &m_TS[0];      //将数组的首地址发给指针
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)
    {
        if (pTS->timer_id == timer_id)
        {
            pTS->flag = FLG_TASK_NORMAL;
            pTS->timer_id = TIMER_NOUSE;
            pTS->interval = 0;
            pTS->count = 0;
            pTS->pFun = 0;
        }
        if (pTS->pFun) ++cnt;
        ++pTS;
    }
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能      //打开中断
    if (cnt==0)
        Stop_Timer();
}
/********************************************/
void ResetTimer(u8 timer_id)
{/**
    uint idx;
    TASKSTRUCT *pTS;

    TACTL &= ~TAIE;      //关闭中断
    pTS = &m_TS[0];
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)
    {
        if (pTS->timer_id == timer_id)
        {
            pTS->flag = FLG_TASK_NORMAL;
            pTS->count = 0;
            break;
        }
        ++pTS;
    }
    TACTL |= TAIE;      //打开中断
    ***/
}

//#endif

//初始化timerA, 10ms 一次
void Start_Timer(u16 Interal)         //timer_A初始化
{ 
    u8 idx;
    TASKSTRUCT *pTS;
    pTS = &m_TS[0];
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)
    {
        pTS->flag = FLG_TASK_NORMAL;
        pTS->timer_id = TIMER_NOUSE;
        pTS->interval = 0;
        pTS->count = 0;
        pTS->pFun = 0;
        ++pTS;
   }
   TimeInitialized = 1;
   TIM_SetCounter(TIM2, 0x0000);     //清除计数器的初始值，开计数，开中断
   TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能
   TIM_Cmd(TIM2,ENABLE);    //关闭定时计数器
}

void Stop_Timer(void)
{
   	TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
	  TIM_Cmd(TIM2,DISABLE);    //关闭定时计数器
    TimeInitialized = 0;       
}
  /***/
void TIM2_IRQHandler(void)
{
    u8 idx;	   
	  TASKSTRUCT *pTS;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
       TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  	  
       pTS = &m_TS[0];		//TIMER_TASK_COUNT=3是为了充分利用这个定时器
       for (idx =0; idx<TIMER_TASK_COUNT;idx++)
       {
            if (pTS->pFun)
            {
                pTS->count++;
                if (pTS->count >= pTS->interval)		//相当于将一个定时器定时时间翻倍interval
                {
                    pTS->pFun();		//因为只有到了给定的倍数才将g_WaitTimeOut置为true
                    pTS->count = 0;
                }
            }
            ++pTS;
       }
   
  }
}

//初始化定时器0作为	Key
void Init_Timer0(void)
{
//    TASKSTRUCT *pTS;          //定义一个结构指针 
	TIM_DeInit(TIM1);
    TIM_TimeBaseStructure.TIM_Prescaler = 0xFF;	 //更改此处可以降低TIMER的输入频率
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period =0x0500; //0xFFFE;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
	TIM_Cmd(TIM1,DISABLE);    //关闭定时计数器
}
void Start_Timer0(void)
{
   TimeInitialized = 1;
   TIM_SetCounter(TIM1, 0x0000);     //清除计数器的初始值，开计数，开中断
   TIM_ITConfig(TIM1,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能
   TIM_Cmd(TIM1,ENABLE);
}
void Stop_Timer0(void)
{
    TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
	TIM_Cmd(TIM1,DISABLE);    //关闭定时计数器
}
//此定时器采用扫描的模式，不采用中断的模式
void WaitTF0(uint interval)
{
      
	 
	    
	    {
		 TIM_SetAutoreload(TIM1,interval);
         Start_Timer0();
         while (TIM_GetITStatus(TIM1, TIM_IT_Update) == RESET);
		  TIM_ClearITPendingBit( TIM1, TIM_IT_Update);
         Stop_Timer0();
	    } 
	  

}

/*  超时将g_WaitTimerOut置为true   */
void Do_TimeOut(void)
{
   g_WaitTimeOut =1;
}




