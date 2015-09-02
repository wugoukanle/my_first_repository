
#include "general.h"
#include "timer.h"
#include "stm32f10x_tim.h"

extern u8 g_WaitTimeOut;
/*****
 ���õ�shi timer2  normal
******/
u8       TimeInitialized = 0;
TASKSTRUCT  m_TS[TIMER_TASK_COUNT];
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
 //��ʱ���������ϼ����ķ�ʽ
void Init_Timer(void)
{
    u16 idx;
    TASKSTRUCT *pTS;          //����һ���ṹָ�� 
		TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Prescaler = 0xFF;	 //���Ĵ˴����Խ���TIMER������Ƶ��
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFE;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
		TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
		TIM_Cmd(TIM2,DISABLE);    //�رն�ʱ������
    pTS = &m_TS[0];           //����Ķ�ʱ������
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
//    TASKSTRUCT *pTS;          //����һ���ṹָ�� 
		TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Prescaler = 0xFFF0;	 //���Ĵ˴����Խ���TIMER������Ƶ��
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFE;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
		TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
		TIM_Cmd(TIM2,DISABLE);    //�رն�ʱ������
	}


}
/**********************************/
/*��װ��ʱ����
	���õ���TIM2
	TimerFun��һ������ָ�룬
*/
u8 SetTimer(u8 timer_id, u16 interval, TimerFun pTask,u16 Time_Top )
{
    u16 idx;
    TASKSTRUCT *pTS;
    if (pTask==0 || interval==0 || timer_id == TIMER_NOUSE)//�������󷵻�
    {
        return ERR_TIMER_PARAMETER;
    }

    TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��                        //�ر��ж�
    pTS = &m_TS[0];		//m_TS[TIMER_TASK_COUNT]
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)   //----������󷵻�----//
    {
        if (pTS->timer_id == timer_id)
        {
            TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ��                 //���ж�
            return  ERR_TIMER_TASKEXIST;
        }else{
            if (pTS->pFun == 0)
            {
                if (!TimeInitialized)  //ʱ��ĳ�ʼ��
                {
                    Start_Timer(Time_Top);
                }
                pTS->flag = FLG_TASK_NORMAL;
                pTS->timer_id = timer_id;
                pTS->interval = interval;
                pTS->count = 0;
                pTS->pFun = pTask;
                TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ��     //���ж�
                return ERR_TIMER_OK;
            }
       }
       ++pTS;
    }
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ��      //���ж�
	// TIM_Cmd(TIM1,ENABLE);
    return ERR_TIMER_TOOMANYTASK;
}
/***********************************************/
void KillTimer(u8 timer_id)  //ֹͣ��ʱ����
{
    u16 idx,cnt=0;
    TASKSTRUCT *pTS;

    TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
    pTS = &m_TS[0];      //��������׵�ַ����ָ��
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
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ��      //���ж�
    if (cnt==0)
        Stop_Timer();
}
/********************************************/
void ResetTimer(u8 timer_id)
{/**
    uint idx;
    TASKSTRUCT *pTS;

    TACTL &= ~TAIE;      //�ر��ж�
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
    TACTL |= TAIE;      //���ж�
    ***/
}

//#endif

//��ʼ��timerA, 10ms һ��
void Start_Timer(u16 Interal)         //timer_A��ʼ��
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
   TIM_SetCounter(TIM2, 0x0000);     //����������ĳ�ʼֵ�������������ж�
   TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ��
   TIM_Cmd(TIM2,ENABLE);    //�رն�ʱ������
}

void Stop_Timer(void)
{
   	TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
	  TIM_Cmd(TIM2,DISABLE);    //�رն�ʱ������
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
  	  
       pTS = &m_TS[0];		//TIMER_TASK_COUNT=3��Ϊ�˳�����������ʱ��
       for (idx =0; idx<TIMER_TASK_COUNT;idx++)
       {
            if (pTS->pFun)
            {
                pTS->count++;
                if (pTS->count >= pTS->interval)		//�൱�ڽ�һ����ʱ����ʱʱ�䷭��interval
                {
                    pTS->pFun();		//��Ϊֻ�е��˸����ı����Ž�g_WaitTimeOut��Ϊtrue
                    pTS->count = 0;
                }
            }
            ++pTS;
       }
   
  }
}

//��ʼ����ʱ��0��Ϊ	Key
void Init_Timer0(void)
{
//    TASKSTRUCT *pTS;          //����һ���ṹָ�� 
	TIM_DeInit(TIM1);
    TIM_TimeBaseStructure.TIM_Prescaler = 0xFF;	 //���Ĵ˴����Խ���TIMER������Ƶ��
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period =0x0500; //0xFFFE;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
	TIM_Cmd(TIM1,DISABLE);    //�رն�ʱ������
}
void Start_Timer0(void)
{
   TimeInitialized = 1;
   TIM_SetCounter(TIM1, 0x0000);     //����������ĳ�ʼֵ�������������ж�
   TIM_ITConfig(TIM1,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ��
   TIM_Cmd(TIM1,ENABLE);
}
void Stop_Timer0(void)
{
    TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
	TIM_Cmd(TIM1,DISABLE);    //�رն�ʱ������
}
//�˶�ʱ������ɨ���ģʽ���������жϵ�ģʽ
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

/*  ��ʱ��g_WaitTimerOut��Ϊtrue   */
void Do_TimeOut(void)
{
   g_WaitTimeOut =1;
}




