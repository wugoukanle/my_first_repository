#include "spi_lcd.h"
#include "ASC.h"
#include "spi2_flash.h"
#include "main.h"
#include "sw-key.h"
#include "Timer.h"
#include "Key_Transfer.h"
#include "spi_flash.h"
#include "RTC_time.h"
#include "hanzi.h"
#include "general.h"

struct GB1616 Oprater[]={
	
	0x08,0x00,0x08,0x00,0x08,0x04,0x7E,0x84,0x08,0x48,0x08,0x28,0xFF,0x10,0x08,0x10,
  0x28,0x28,0x2F,0x28,0x28,0x44,0x28,0x84,0x58,0x00,0x48,0x00,0x87,0xFE,0x00,0x00,"��",
	0x10,0x00,0x92,0xF8,0x92,0x88,0x92,0x88,0xFE,0x88,0x00,0x88,0xFE,0x88,0x02,0x88,
  0x7E,0x88,0x40,0x88,0x42,0x88,0x45,0x08,0x59,0x0A,0xE2,0x0A,0x44,0x06,0x00,0x00,"��",
	0x1F,0xF8,0x01,0x00,0x7F,0xFE,0x41,0x04,0x1D,0x70,0x01,0x00,0x1D,0x70,0x01,0x00,
  0x3F,0xF8,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x08,0x21,0x08,0x3F,0xF8,0x20,0x08,"��"
};
	
	

const uchar ASCII[];
const uchar asc_table[] = "0123456789ABCDEF"; 

extern uint GUI_GetBKColor =White;//Ĭ�ϱ���ɫ�����֣�	
extern uint GUI_GetColor =Black;  //Ĭ��ǰ��ɫ����ͼ�����֣�
extern uchar  GUI_SetDrawMode =1;	  //1����ʾ 0Ϊ������ʾ
extern uint posXl=0;
extern uint posYl=0;
extern uint posXr=128;
extern uint posYr=160;
//��ǰ��������λ��
extern uint posX=5;//0;	
extern uint posY=5;//0;
//�����ʽ
extern uint Sfont=0;
uchar f_CurSel = 1;
uchar  lcd_buf2[15];
struct LCD_MODULE
 {
   uchar color[8];            //��ʾA0A1 A2A0 A1A2 A2B0 B0B1 B2B0 B1B2 B2A0(AΪ��ʾɫ��BΪ����ɫ)
  
 }LCD_PART;
 // GUI_GetBKColor=White;//Ĭ�ϱ���ɫ�����֣�	
 // GUI_GetColor=Black;  //Ĭ��ǰ��ɫ����ͼ�����֣�
 //GUI_SetDrawMode=1;	  //1����ʾ
 /****
  �˵�ѡ��
 *****/
#define MAX_MENU      10
const MENU_ITEM HsMenu[MAX_MENU]={   //���˵�
    {5,6,"",0},	//��ṹ�ĵ�0��Ϊ�ر�ģ���ʾ��ѡ�˵������ʼ���������ʼ�ս���ѡ��������
    {0,0, "�ź�",0},
    //{32,0, lcd_buf2[15],0},
    {0,96, "����",0},
    {48,48,"��1",0},
		{64,36,"Կ��1",0},
		{112,0,"����",tool_menu},
		{112,96,"����",set_menu},
    };
//Ԫ��Ϊ�ṹ��MENU_ITEM

// const MENU_ITEM HsMenu[MAX_MENU]={   //���˵�
//     
//     {0,0, "��������Ϣ",this_Info},
//     {18,0, "�û����",User_Info},
//     {36,0, "�洢�ռ���Ϣ",Save_Info}
//     //{54,0,"����ʮ�β�����Ϣ",OPeration_Info}
//     };
// //Ԫ��Ϊ�ṹ��MENU_ITEM


		
//��ʾ���߲˵�
void tool_menu(void)
{
	Choose_MenuOp4();
	return;
}
		
		
		
//��ʾ���ò˵�
void set_menu(void)
{
	Choose_MenuOp5();
	return;
}

const MENU_ITEM TOOLMENU[MAX_MENU]={
		{2,3,"0",0},
		{48,48,"����",0},
		{112,0,"ȡ��",0},
		{112,96,"ȷ��",0}
};

const MENU_ITEM SETMENU[MAX_MENU]={
		{1,6,"0",0},
		{16,32,"����ʱ��",0},
		{32,48,"����",0},
		{48,32,"�̼�����",0},
		{64,48,"����",0},
		{112,0,"ȡ��",0},
		{112,96,"ȷ��",0}
};


		

		


/***
��ʾ��������λ�룬��ǰ����
void apoint_place_write(uchar s_column,uchar s_row,uchar *wr_sp,uint w_color,uint bk_color)	 .
often_show(36,56,"ͨ�ŵȴ���..",0,0x0fff);
����ģʽ�����ϼ��˵�����һ�� ȡ���� ����
�ڶ��� ��ʱ����
***/
void this_Info(uchar para) 
{
  uchar i,nChar,k;
  uchar temp_buff1[5],temp_buff2[7];
  GUI_Clear(White);
  for(i =0;i<2;i++)
  temp_buff1[i] =ReadData(LOCK_TYPE); //��ʾ��Ҫת��ΪASCII��
  {
	temp_buff1[0]=0x34;
	temp_buff1[1]=0x56;	   //����

  }
   i = temp_buff1[0];
   nChar = temp_buff1[1];
   temp_buff1[0] = ((i&0xf0)>>4)+0x30;
   temp_buff1[1] =(i&0x0f)+0x30;
   temp_buff1[2] = ((nChar&0xf0)>>4)+0x30;
   temp_buff1[3] = (nChar&0x0f)+0x30;
   temp_buff1[4] =0;
  for(i =0;i<3;i++)
  temp_buff2[i] = ReadData(LOCK_LOCAL+i);
  {
   	temp_buff2[0]=0x34;
		temp_buff2[1]=0x56;	   //����
		temp_buff2[2]=0x78;
	

  }
  i = temp_buff2[0];
  nChar = temp_buff2[1];
  k = temp_buff2[2];
  temp_buff2[0] = ((i&0xf0)>>4)+0x30;
  temp_buff2[1] =(i&0x0f)+0x30;
  temp_buff2[2] = ((nChar&0xf0)>>4)+0x30;
  temp_buff2[3] = (nChar&0x0f)+0x30;
  temp_buff2[4] = ((k&0xf0)>>4)+0x30;
  temp_buff2[5] = (k&0x0f)+0x30;
  temp_buff2[6] =0;
  often_show(0,0,"��λ��:",0,0x0fff);
  apoint_place_write(0,17,temp_buff1,0,0xfff);
  often_show(0,34,"ǰ����:",0,0x0fff);
  apoint_place_write(0,51,temp_buff2,0,0xfff);
  g_WaitTimeOut = FALSE;
  SetTimer(TIMER_TIMEOUT,8,Do_TimeOut,THREE_MIN);   //�ȴ�����ʱ��
  while( g_WaitTimeOut == FALSE )
  {
   nChar = GetKey();
   if(nChar ==KEYCLEAR)
   {
		 GUI_Clear(White);
		 InsertKey(KEYINIT);
		 KillTimer(TIMER_TIMEOUT);
     return;
   }else if(nChar !=KEYNO)
   {
     ResetTimer(TIMER_TIMEOUT);

   }


  }
	GUI_Clear(White);
	InsertKey(KEYINIT);
  KillTimer(TIMER_TIMEOUT);
	return;
 	
}
/***
��ʾ�û��ı����Ϣ
***/
void User_Info(uchar para) 
{
  uchar i,nChar;
  uchar temp_buff1[4];	   //�û��ı��Ϊ4λ����
  uchar temp_buff2[9];
  GUI_Clear(White);
  for(i =0;i<4;i++)
  temp_buff1[i] = ReadData(LOCK_LOCAL+i);
  {
			temp_buff1[0]=0x01;
	    temp_buff1[1]=0x23;	   //����
	   	temp_buff1[2]=0x45;
	    temp_buff1[3]=0x67;	   //����

  }
  temp_buff2[0] =((temp_buff1[0]&0xf0)>>4)+0x30;
  temp_buff2[1] =(temp_buff1[0]&0x0f)+0x30;
  temp_buff2[2] =((temp_buff1[1]&0xf0)>>4)+0x30;
  temp_buff2[3] =(temp_buff1[1]&0x0f)+0x30;
  temp_buff2[4] =((temp_buff1[2]&0xf0)>>4)+0x30;
  temp_buff2[5] =(temp_buff1[2]&0x0f)+0x30;
  temp_buff2[6] =((temp_buff1[3]&0xf0)>>4)+0x30;
  temp_buff2[7] =(temp_buff1[3]&0x0f)+0x30;
  temp_buff2[8]	=0;
  often_show(0,0,"�û����",0,0x0fff);
  apoint_place_write(0,17,temp_buff2,0,0xfff);
 
  g_WaitTimeOut = FALSE;
  SetTimer(TIMER_TIMEOUT,8,Do_TimeOut,THREE_MIN);   //�ȴ�����ʱ��
  while( g_WaitTimeOut == FALSE )
  {
   nChar = GetKey();
   if(nChar ==KEYCLEAR)
   {

   }else if(nChar !=KEYNO)
   {


   }


  }
  KillTimer(TIMER_TIMEOUT);

}
/***

  for(i =0;i<4;i++)
  mid_buff[i] =Lock_number[i];
  read_time();
  for(i =0;i<6;i++)
  mid_buff[4+i]=time_buff[i];
  ÿһ���¼Ϊ10���ֽ�
  ���ʮ�εĲ�����Ϣ
  �����ʮ�εĲ�����¼��ȡ��g_Buff����������
***/

void ShowOneOperation(uchar temp[])
{
  uchar temp_buff1[4],temp_buff2[6],i;
  uchar Lcd_temp_buff1[9],Lcd_temp_buff2[13];
  GUI_Clear(White);
  for(i =0;i<4;i++)
  temp_buff1[i] =temp[i];
  for(i =0;i<6;i++)
  temp_buff2[i] =temp[4+i];
  {
   	temp_buff1[0]=0x01;
		temp_buff1[1]=0x23;	   //����
    temp_buff1[2]=0x45;
		temp_buff1[3]=0x67;	   //����

    temp_buff2[0]=0x34;
		temp_buff2[1]=0x56;	   //����
		temp_buff2[2]=0x34;
		temp_buff2[3]=0x56;	   //����
		temp_buff2[4]=0x34;
		temp_buff2[5]=0x56;	   //����

 }
  Lcd_temp_buff1[0] =((temp_buff1[0]&0xf0)>>4)+0x30;
  Lcd_temp_buff1[1] =(temp_buff1[0]&0x0f)+0x30;
  Lcd_temp_buff1[2] =((temp_buff1[1]&0xf0)>>4)+0x30;
  Lcd_temp_buff1[3] =(temp_buff1[1]&0x0f)+0x30;
  Lcd_temp_buff1[4] =((temp_buff1[2]&0xf0)>>4)+0x30;
  Lcd_temp_buff1[5] =(temp_buff1[2]&0x0f)+0x30;
  Lcd_temp_buff1[6] =((temp_buff1[3]&0xf0)>>4)+0x30;
  Lcd_temp_buff1[7] =(temp_buff1[3]&0x0f)+0x30;
  Lcd_temp_buff1[8]	=0;
  Lcd_temp_buff2[0] =((temp_buff2[0]&0xf0)>>4)+0x30;
  Lcd_temp_buff2[1] =(temp_buff2[0]&0x0f)+0x30;
  Lcd_temp_buff2[2] =((temp_buff2[1]&0xf0)>>4)+0x30;
  Lcd_temp_buff2[3] =(temp_buff2[1]&0x0f)+0x30;
  Lcd_temp_buff2[4] =((temp_buff2[2]&0xf0)>>4)+0x30;
  Lcd_temp_buff2[5] =(temp_buff2[2]&0x0f)+0x30;
  Lcd_temp_buff2[6] =((temp_buff2[3]&0xf0)>>4)+0x30;
  Lcd_temp_buff2[7] =(temp_buff2[3]&0x0f)+0x30;
  Lcd_temp_buff2[8] =((temp_buff2[4]&0xf0)>>4)+0x30;
  Lcd_temp_buff2[9] =(temp_buff2[4]&0x0f)+0x30;
  Lcd_temp_buff2[10] =((temp_buff2[5]&0xf0)>>4)+0x30;
  Lcd_temp_buff2[11] =(temp_buff2[5]&0x0f)+0x30;
  Lcd_temp_buff2[12] =0;
  often_show(0,0,"������:",0,0x0fff);
  apoint_place_write(0,17,Lcd_temp_buff1,0,0xfff);
  often_show(0,34,"����ʱ��:",0,0x0fff);
  apoint_place_write(0,51,Lcd_temp_buff2,0,0xfff); 

}

void OPeration_Info(uchar para) 
{
  uchar i,idx,Lcd_temp[10],nChar;
  idx =0;
  InsertKey(KEYINIT);
  g_WaitTimeOut = FALSE;
  SetTimer(TIMER_TIMEOUT,8,Do_TimeOut,THREE_MIN);   //�ȴ�����ʱ��
  while( g_WaitTimeOut == FALSE )
  {
   nChar = GetKey();
   if(nChar ==KEYINIT)
   {
		 for(i =0;i<10;i++)
		 Lcd_temp[i] =g_Buff[idx+i];
		 ShowOneOperation(Lcd_temp);

   }else if(nChar ==KEYDOWN)
   {
    
     idx =idx+10;
		 if(idx >100)
		 idx =0;
		 for(i =0;i<10;i++)
		 Lcd_temp[i] =g_Buff[idx+i];
		 ShowOneOperation(Lcd_temp);

   }else if(nChar ==KEYUP)
   {
     if(idx >0)
     idx =idx-10;
		 else
		 idx =90;
		 for(i =0;i<10;i++)
		 Lcd_temp[i] =g_Buff[idx+i];
		 ShowOneOperation(Lcd_temp);

   }
   if(nChar ==KEYCLEAR)
   {

   }


  }

}
/***
��ʾ�洢�ռ���Ϣ
***/
void Save_Info(uchar para) 
{
  uchar i,nChar,k;
  uchar temp_buff1[5],temp_buff2[7];
  GUI_Clear(White);
  for(i =0;i<2;i++)
  temp_buff1[i] =ReadData(LOCK_TYPE);
  {
		temp_buff1[0]=0x34;
    	temp_buff1[1]=0x56;	   //����
  }
   i = temp_buff1[0];
   nChar = temp_buff1[1];
   temp_buff1[0] = ((i&0xf0)>>4)+0x30;
   temp_buff1[1] =(i&0x0f)+0x30;
   temp_buff1[2] = ((nChar&0xf0)>>4)+0x30;
   temp_buff1[3] = (nChar&0x0f)+0x30;
   temp_buff1[4] =0;
  for(i =0;i<3;i++)
  temp_buff2[i] = ReadData(LOCK_LOCAL+i);
  {
		temp_buff2[0]=0x34;
	    temp_buff2[1]=0x56;	   //����
		temp_buff2[2]=0x34;
	

  }
     i = temp_buff2[0];
   nChar = temp_buff2[1];
   k = temp_buff2[2];
   temp_buff2[0] = ((i&0xf0)>>4)+0x30;
   temp_buff2[1] =(i&0x0f)+0x30;
   temp_buff2[2] = ((nChar&0xf0)>>4)+0x30;
   temp_buff2[3] = (nChar&0x0f)+0x30;
   temp_buff2[4] = ((k&0xf0)>>4)+0x30;
   temp_buff2[5] = (k&0x0f)+0x30;
   temp_buff2[6] =0;
  often_show(0,0,"�Ѵ洢�û�",0,0x0fff);
  apoint_place_write(0,17,temp_buff1,0,0xfff);
  often_show(0,34,"ʣ��洢�ռ�",0,0x0fff);
  apoint_place_write(0,51,temp_buff2,0,0xfff);
  g_WaitTimeOut = FALSE;
  SetTimer(TIMER_TIMEOUT,8,Do_TimeOut,THREE_MIN);   //�ȴ�����ʱ��
  while( g_WaitTimeOut == FALSE )
  {
   nChar = GetKey();
   if(nChar ==KEYCLEAR)
   {

   }else if(nChar !=KEYNO)
   {


   }


  }
  KillTimer(TIMER_TIMEOUT);  
  
}
void ShowHsMenu(void) //���˵���ʾ
{
    uchar idx;
   //Lcd_Show(1,0, "����ѡ��˵�");
   // WATCHDOG_CLEAR;                   //������Ź�
    for (idx=5;idx<7;idx++)
    {
       if (idx==f_CurSel)
           Lcd_ShowN(HsMenu[idx].row,HsMenu[idx].col,(uchar*)HsMenu[idx].Name);		//�Ὣѡ�е��з�����ʾ
       else
           Lcd_Show(HsMenu[idx].row,HsMenu[idx].col,(uchar*)HsMenu[idx].Name);		//δѡ�е���������ʾ
    }
		//ѭ����֮�����в˵���ȫ����ʾ
}

void ShowMenu(const MENU_ITEM *Menu) //���˵���ʾ
{
    uchar idx;
   //Lcd_Show(1,0, "����ѡ��˵�");
   // WATCHDOG_CLEAR;                   //������Ź�
    for (idx=1;idx<=Menu[0].col;idx++)
    {
       if (idx==f_CurSel)
           Lcd_ShowN(Menu[idx].row,Menu[idx].col,(uchar*)Menu[idx].Name);		//�Ὣѡ�е��з�����ʾ
       else
           Lcd_Show(Menu[idx].row,Menu[idx].col,(uchar*)Menu[idx].Name);		//δѡ�е���������ʾ
    }
		//ѭ����֮�����в˵���ȫ����ʾ
}

/**
 	apoint_place_write(0,1,lcd_buf,0,0xfff);  //������
	apoint_place_write(0,17,lcd_buf1,0,0xfff);  //ʱ����
������ʾ,������û��д����
**/
void  Lcd_ShowN(uchar row, uchar col, uchar *str)
{
   GUI_SetDrawMode =0;  //������ʾ
   apoint_place_write(col,row,str,0,0xfff);
   GUI_SetDrawMode =1;

}

void Lcd_Show(uchar row, uchar col, uchar *str)
{
   apoint_place_write(col,row,str,0,0xfff);
}



/*��ʾ��������*/
void Choose_MenuOp(void)
{
  uchar i,nChar;
  GUI_Clear(White);
  ResetKey();			//Key_Num =KEYNO
  f_CurSel =0;
  g_WaitTimeOut = FALSE;
  //SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
  // while( g_WaitTimeOut == FALSE )  //��ʱ�ȴ�
	//void InsertKey(uchar nChar){ Key_Num =nChar;}
  InsertKey(KEYINIT);   //�ڰ���������������һ������ĳ�ʼ������
  while(1)
  {
		nChar = GetKey();   //��ȡ����������
		if(nChar ==KEYINIT)
		{
			Lcd_Clear(0);	    //����
			ShowHsMenu();		//��ʾ�˵�
			InsertKey(KEYNO);		//Key_Num =nChar;
		}
		else if(nChar == KEYUP)	//�Ϸ�	++
		{
				 
				ResetTimer(TIMER_TIMEOUT);		//��ûʵ��
				if (f_CurSel< MAX_MENU - 1)
						++f_CurSel;
				else
						f_CurSel = 0;
				Lcd_Clear(0);
				ShowHsMenu();		//��ʱf_CurSel�Ѿ��ı䣬����֮������ʾʱ���׵��л�ı�
				InsertKey(KEYNO); 

		}else if(nChar ==KEYDOWN)//�·�	--
		{
					
					ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel>0)
							--f_CurSel;
					else
							f_CurSel =  MAX_MENU - 1;
					Lcd_Clear(0);
					ShowHsMenu();
					InsertKey(KEYNO); 

		}else if(nChar ==KEYSURE) //ȷ��
		{
					 
					KillTimer(TIMER_TIMEOUT);
					g_WaitTimeOut = FALSE;
					Lcd_Clear(0);
					HsMenu[f_CurSel].pFunc(0);		//f_CurSel��ʾ��ǰ����
					InsertKey(KEYNO); 

		}else if( nChar ==KEYCLEAR)//ȡ��
		{
					KeyNum =KEYNO;
					Lcd_Clear(0);
					g_State = ST_IDLE;
					InsertKey(KEYNO);
					break;

		}
  }
  KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}

/*��ʾ��������*/
void Choose_MenuOp1(void)
{
  uchar i,nChar;
  GUI_Clear(White);
  ResetKey();			//Key_Num =KEYNO
  f_CurSel =HsMenu[0].row;
  g_WaitTimeOut = FALSE;
  SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
  while( g_WaitTimeOut == FALSE )  //��ʱ�ȴ�
	//void InsertKey(uchar nChar){ Key_Num =nChar;}
  InsertKey(KEYINIT);   //�ڰ���������������һ������ĳ�ʼ������
  while(1)
  {
		nChar = GetKey();   //��ȡ����������
		if(nChar ==KEYINIT)
		{
			Lcd_Clear(0);	    //����
			ShowMenu(HsMenu);		//��ʾ�˵�
			apoint_place_write(32,0,lcd_buf2,0,0x0fff);	
			InsertKey(KEYNO);		//Key_Num =nChar;
		}
		else if(nChar == KEYUP)	//�Ϸ�	++
		{
				 
				ResetTimer(TIMER_TIMEOUT);		//��ûʵ��
				if (f_CurSel<=HsMenu[0].col)
						++f_CurSel;
				else
						f_CurSel = HsMenu[0].row;
				Lcd_Clear(0);
				ShowMenu(HsMenu);		//��ʱf_CurSel�Ѿ��ı䣬����֮������ʾʱ���׵��л�ı�
				apoint_place_write(32,0,lcd_buf2,0,0x0fff);	
				InsertKey(KEYNO); 

		}else if(nChar ==KEYDOWN)//�·�	--
		{
					
					ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel>HsMenu[0].row)
							--f_CurSel;
					else
							f_CurSel =  HsMenu[0].col;
					Lcd_Clear(0);
					ShowMenu(HsMenu);
					apoint_place_write(32,0,lcd_buf2,0,0x0fff);	
					InsertKey(KEYNO); 

		}else if(nChar ==KEYSURE) //ȷ��
		{
					 
					KillTimer(TIMER_TIMEOUT);
					g_WaitTimeOut = FALSE;
					Lcd_Clear(0);
					HsMenu[f_CurSel].pFunc(0);		//f_CurSel��ʾ��ǰ����
					InsertKey(KEYNO); 

		}else if( nChar ==KEYCLEAR)//ȡ��
		{
					KeyNum =KEYNO;
					Lcd_Clear(0);
					g_State = ST_IDLE;
					InsertKey(KEYNO);
					break;

		}
  }
  KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}

/*��ʾ��������*/
void Choose_MenuOp4(void)
{
  uchar i,nChar;
  GUI_Clear(White);
  ResetKey();			//Key_Num =KEYNO
  f_CurSel =TOOLMENU[0].row;
  g_WaitTimeOut = FALSE;
  //SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
  // while( g_WaitTimeOut == FALSE )  //��ʱ�ȴ�
	//void InsertKey(uchar nChar){ Key_Num =nChar;}
  InsertKey(KEYINIT);   //�ڰ���������������һ������ĳ�ʼ������
  while(1)
  {
		nChar = GetKey();   //��ȡ����������
		if(nChar ==KEYINIT)
		{
			Lcd_Clear(0);	    //����
			ShowMenu(TOOLMENU);		//��ʾ�˵�
			InsertKey(KEYNO);		//Key_Num =nChar;
		}
		else if(nChar == KEYUP)	//�Ϸ�	++
		{
				 
				ResetTimer(TIMER_TIMEOUT);		//��ûʵ��
				if (f_CurSel<=TOOLMENU[0].col)
						++f_CurSel;
				else
						f_CurSel = TOOLMENU[0].row;
				Lcd_Clear(0);
				ShowMenu(TOOLMENU);		//��ʱf_CurSel�Ѿ��ı䣬����֮������ʾʱ���׵��л�ı�
				InsertKey(KEYNO); 

		}else if(nChar ==KEYDOWN)//�·�	--
		{
					
					ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel>TOOLMENU[0].row)
							--f_CurSel;
					else
							f_CurSel =  TOOLMENU[0].col;
					Lcd_Clear(0);
					ShowMenu(TOOLMENU);
					InsertKey(KEYNO); 

		}else if(nChar ==KEYSURE) //ȷ��
		{
					 
					KillTimer(TIMER_TIMEOUT);
					g_WaitTimeOut = FALSE;
					Lcd_Clear(0);
					TOOLMENU[f_CurSel].pFunc(0);		//f_CurSel��ʾ��ǰ����
					InsertKey(KEYNO); 

		}else if( nChar ==KEYCLEAR)//ȡ��
		{
					KeyNum =KEYNO;
					Lcd_Clear(0);
					g_State = ST_IDLE;
					InsertKey(KEYNO);
					break;

		}
  }
  KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}

/*��ʾ��������*/
void Choose_MenuOp5(void)
{
  uchar i,nChar;
  GUI_Clear(White);
  ResetKey();			//Key_Num =KEYNO
  f_CurSel =SETMENU[0].row;
  g_WaitTimeOut = FALSE;
  //SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
  // while( g_WaitTimeOut == FALSE )  //��ʱ�ȴ�
	//void InsertKey(uchar nChar){ Key_Num =nChar;}
  InsertKey(KEYINIT);   //�ڰ���������������һ������ĳ�ʼ������
  while(1)
  {
		nChar = GetKey();   //��ȡ����������
		if(nChar ==KEYINIT)
		{
			Lcd_Clear(0);	    //����
			ShowMenu(SETMENU);		//��ʾ�˵�
			InsertKey(KEYNO);		//Key_Num =nChar;
		}
		else if(nChar == KEYUP)	//�Ϸ�	++
		{
				 
				ResetTimer(TIMER_TIMEOUT);		//��ûʵ��
				if (f_CurSel<=SETMENU[0].col)
						++f_CurSel;
				else
						f_CurSel = SETMENU[0].row;
				Lcd_Clear(0);
				ShowMenu(SETMENU);		//��ʱf_CurSel�Ѿ��ı䣬����֮������ʾʱ���׵��л�ı�
				InsertKey(KEYNO); 

		}else if(nChar ==KEYDOWN)//�·�	--
		{
					
					ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel>SETMENU[0].row)
							--f_CurSel;
					else
							f_CurSel =  SETMENU[0].col;
					Lcd_Clear(0);
					ShowMenu(SETMENU);
					InsertKey(KEYNO); 

		}else if(nChar ==KEYSURE) //ȷ��
		{
					 
					KillTimer(TIMER_TIMEOUT);
					g_WaitTimeOut = FALSE;
					Lcd_Clear(0);
					SETMENU[f_CurSel].pFunc(0);		//f_CurSel��ʾ��ǰ����
					InsertKey(KEYNO); 

		}else if( nChar ==KEYCLEAR)//ȡ��
		{
					KeyNum =KEYNO;
					Lcd_Clear(0);
					g_State = ST_IDLE;
					InsertKey(KEYNO);
					break;

		}
  }
  KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}



void Lcd_Clear(uchar para)		//�����paraû��
{
	GUI_Clear(White);		//ֱ�ӵ��ãǣգɣߣã���������佫��Ļ�ϵ�ÿһ�㶼дΪ��

}

/**
����⵽��ص�ѹ���͵�ʱ��LCD��ʾ
��ص�ѹ���ͣ��뼰ʱ���
ÿ��5������ʾһ�ε�ص�ѹ���ͣ������⵽��ص�ѹ���ͣ���ֱ�ӹػ���
ÿ�δ��ֳֻ���ʱ���ȼ��һ�µ�صĵ�ѹ�������ص�ѹ���㣬�򲻴�

**/

void Lcd_Show_LowBattry(void)
{
 GUI_Clear(White);
 often_show(0,0,"��ѹ����!",0x00f,0xfff);
 often_show(0,17,"���Զ��ػ�",0x00f,0xfff);


}


/***
  ����ȡ��ID��ŵ�ʱ�� ��ID����ʾ��LCD��
Ϊ��ȷ�ı�������
Ϊ����ı�� ����ʾ ���ڹ���Χ��
�ٶ�ID��16����������С��0X0A�����

***/

void Lcd_Show_IDinfo(void)
{
   uchar temp;
   uchar Lcd_LockBuff[9];
   GUI_Clear(White);
   temp=Lock_number[0];
   Lcd_LockBuff[0] =((temp&0xf0)>>4)+0x30;		//��0x30�����ֱ���ַ�
   Lcd_LockBuff[1] =(temp&0x0f)+0x30;		
   temp=Lock_number[1];
   Lcd_LockBuff[2]=((temp&0xf0)>>4)+0x30;
   Lcd_LockBuff[3]=(temp&0x0f)+0x30;
   temp=Lock_number[2];
   Lcd_LockBuff[4]=((temp&0xf0)>>4)+0x30;
   Lcd_LockBuff[5]=(temp&0x0f)+0x30;
   temp=Lock_number[3];
   Lcd_LockBuff[6]=((temp&0xf0)>>4)+0x30;
   Lcd_LockBuff[7]=(temp&0x0f)+0x30;
   Lcd_LockBuff[8] =0;
   often_show(0,0,"��������ID:",0,0x0fff);
   apoint_place_write(1,17,Lcd_LockBuff,0,0xfff);

}
/*
data,clk ,rs,rst,cs
*/
void  Lcd_PortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	    //CONTROL
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //cs,rs,wr,rd
	GPIO_Init(GPIOB, &GPIO_InitStructure);//����������GPIOB��PB5,PB6,PB7,PB8,PB9��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	//DATA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
	GPIO_Init(GPIOC, &GPIO_InitStructure);//����������GPIOC��PC0~PC7��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	// LCD_LIGHT��PB13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}



   
/****************************************************************************
* ��    �ƣ�void DispSmallPic(u16 x, u16 y, u16 w, u16 h, const u8 *str)
* ��    �ܣ���ָ��λ����ʾһ��ͼƬ
* ��ڲ�����x:x������  y��y����  w����  h����  str:���� 
* ���ڲ�������
* ˵    ����2012.5.8 hxm  v1.0
* ���÷������� 
****************************************************************************/
void DispSmallPic(uint x, uint y, uint w, uint h, const uchar *str)
{ 
    uint i,j;
    for(j=0;j<h;j++)
    {
        Set_ramaddr(x,y+j);
        write_cmd(0X2C);
        for(i=0;i<w;i++)
        {       
            write_data8(str[(j*w+i)*2+1]);     //��λ��ǰ
            write_data8(str[(j*w+i)*2]);
        }
    }
}
/****************************************************************************
* ��    �ƣ�void GUI_Disphz(u8 *hz);
* ��    �ܣ��ڵ�ǰλ����ʾһ�������ַ�16*16
* ��ڲ�����*hz�ַ��������Ժ�����Ч��
* ���ڲ�������
* ˵    ����2012.5.3 hxm  v1.0
* ���÷������� 
****************************************************************************/
void GUI_Disphz(uchar *hz)
{
	GUI_DisphzAt(hz, posX, posY);
	GUI_GetPos(2);	
}

 /******

 ��SPT�ֳֻ����õ�LCD��ʾ����
 ��λ��  ��׼�ֿ�ģʽ

 *******/


 //������ɫ����
void bs_color_set(uint wr_color,uint blank_color)
{
    LCD_PART.color[0] = wr_color/16;                         //A0A1
    LCD_PART.color[1] = wr_color*16 + wr_color/256;          //A2A0
    LCD_PART.color[2] = wr_color;                            //A1A2
    LCD_PART.color[3] = wr_color*16 + blank_color/256;       //A2B0
    LCD_PART.color[4] = blank_color/16;                      //B0B1
    LCD_PART.color[5] = blank_color*16 + blank_color/256;    //B2B0
    LCD_PART.color[6] = blank_color;                         //B1B2
    LCD_PART.color[7] = blank_color*16 + wr_color/256;       //B2A0
}
//��ʾָ��λ�ú��ֺ��ַ�
void apoint_place_write(uchar s_column,uchar s_row,uchar *wr_sp,uint w_color,uint bk_color)
{
    uchar tempa,tempb,number=0;
    
    bs_color_set(w_color,bk_color);
    while((*wr_sp != 0)&&(number < 20))                                         //��⵽������ʾ�˳�
    {
        if(*wr_sp < 0x80)                                                       //ASCII�ַ�д����
        {
            GUI_DispCharAt(wr_sp,s_column,s_row);                                   //���˵��ո�
            wr_sp++;
            s_column += 8;
        }
        else                                                                    //����д
        {
            if(*wr_sp == 0xff)                                                  //д�����ַ�
            {
                wr_sp++;
                tempa = *wr_sp&0xf0;
                if(tempa == 0xa0)
                {
                    tempb = *wr_sp&0x0f;
                    //signal_write(s_column,s_row,tempb,SIGNAL_SN);
                    s_column += 16;
                }
                else
                {
                    if(tempa == 0xe0)
                    {
                        tempb = *wr_sp&0x0f;
                        //signal_write(s_column,s_row,tempb,CHARGE_SN);
                        s_column += 16;
                    }
                }
                wr_sp++;
            }
            else
            {
                if(*wr_sp >= 0x4e)
                {
                    GUI_DisphzAt(wr_sp,s_column,s_row);
                    wr_sp += 2;
                    s_column += 16;
                }
                else
                    wr_sp++;                                                                        //���˵���Ч���ַ�
            }
        }
        number++;
    }
    //BUS_DATA_IN;
}

 //ͨ��д����
void often_show(uchar s_column,uchar s_row,const uchar show_buf[32],uint wr_color,uint blank_color)
{
    //GUI_Clear(White);
    apoint_place_write(s_column,s_row,(uchar*)show_buf,wr_color,blank_color);
}




/****************************************************************************
* ��    �ƣ�void GUI_DisphzAt(u8 *hz, u16 x, u16 y);
* ��    �ܣ���ָ��λ����ʾһ�������ַ�16*16
* ��ڲ�����*hz�ַ�x��λ��y��λ��,�����Ժ�����Ч��
* ���ڲ�������
* ˵    ����2012.5.2 hxm  v1.0
* ���÷������� 
*��ʾһ�����ֵĺ���
****************************************************************************/
void GUI_DisphzAt(uchar hz_sp[], uint x, uint y)
{

	ulong tempaddr;
	uchar tempa,tempb;
	uint j=0,i=0,k=0;
	uint str,len;
	uint GetColor,GetBKColor;
	uint temp1, temp2, temp3, temp4;

	uchar info_buf[2];
	info_buf[0]	=hz_sp[0];
	info_buf[1] =hz_sp[1];
	if(x>112||y>144)return;
		
	if(GUI_SetDrawMode==1)
	{GetColor=GUI_GetColor;GetBKColor=GUI_GetBKColor;}
	else
	{GetColor=GUI_GetBKColor;GetBKColor=GUI_GetColor;}

	temp1 =  GetColor >> 8;
	temp2 =  GetColor & 0x00ff;
	temp3 =  GetBKColor >> 8;
	temp4 =  GetBKColor & 0x00ff;
                                         

//---------------------------�����Ƿ����ֿ��г��� 
	len=sizeof(HZ)/sizeof(HZ[0]);  
 	for(j=0;j<len;j++)
	{
    if(((uchar)HZ[j].Index[0]==info_buf[0])&&((uchar)HZ[j].Index[1]==info_buf[1]))	
		break;
	}
//--------------------------      
	if(j<len)
	{
			for (i=0;i<16;i++)
			{
				 Set_ramaddr(x,y+i);
				 write_cmd(0X2C);

				 str = (u8)HZ[j].Msk[i*2]<<8|(u8)HZ[j].Msk[i*2+1];

					for (k=0;k<16;k++)
					{
							if ( str & (0x8000>>k) )     //0x8000>>k
							{
								write_data8(temp1);
								write_data8(temp2);
							}
							else
							{                   
								write_data8(temp3);
								write_data8(temp4);                                                                                  
							}               
					} 
			}
	}
	posX=x;
	posY=y;
	GUI_GetPos(2);
}


/***
�����������ʾ����Ա���߹���λ����Ϣ
op_Buff[]�Ϊ16���ֽ�
����Ҫ���ֿ���ȡģ����
***/

void  Test_ShowOp(void)
{
	
const char ttt[] ={0x10,0x00,0x08,0x00,0x08,0x04,0x7E,0xFE,0x04,0x24,0x08,0x24,0x12,0x24,0x34,0x24,
                   0x58,0x24,0x94,0x24,0x12,0x44,0x10,0x44,0x10,0x84,0x11,0x14,0x12,0x08,0x00,0x00,//"��",
                   0x10,0x20,0x18,0x30,0x10,0x20,0x10,0x48,0xFC,0x84,0x25,0xFE,0x24,0x84,0x44,0x00,
                   0x44,0xFC,0x28,0x84,0x18,0x84,0x14,0x84,0x22,0x84,0x42,0xFC,0x80,0x84,0x00,0x00,//"ʼ",//"��"
                   0x08,0x80,0x0C,0x80,0x08,0x88,0x10,0x8C,0x10,0x98,0x30,0xB0,0x50,0xE0,0x90,0xC0,
                   0x11,0x80,0x12,0x80,0x14,0x80,0x10,0x84,0x10,0x84,0x10,0x84,0x10,0x7C,0x00,0x00
                  };
	Show_Operater(0,0,(uchar*)ttt,3,0,0x0fff);
  while(1);
	
}
void Show_Operater(uchar s_column,uchar s_row,uchar op_Buff[],uchar len,uint wr_color,uint blank_color)
{
  ulong tempaddr;
	uint  tempa_downaddr;
  uchar tempa,tempb;
	uint j=0,i=0,k=0,x=0,y=0;
	uint str;
	uint GetColor,GetBKColor;
	uint temp1, temp2, temp3, temp4;
	x=s_column;
	y=s_row;
	
	if(GUI_SetDrawMode==1)
	{GetColor=GUI_GetColor;GetBKColor=GUI_GetBKColor;}
	else
	{GetColor=GUI_GetBKColor;GetBKColor=GUI_GetColor;}

	temp1 =  GetColor >> 8;
	temp2 =  GetColor & 0x00ff;
	temp3 =  GetBKColor >> 8;
	temp4 =  GetBKColor & 0x00ff;
  for(j =0;j<len;j++)
  
    {
			  tempa_downaddr =32*j;
        for (i=0;i<16;i++)
        {
             Set_ramaddr(x,y+i);		//�ı��Դ��ַ
             write_cmd(0X2C);		//�Դ�д������������
             str = op_Buff[tempa_downaddr+i*2]<<8|op_Buff[tempa_downaddr+i*2+1];
             for (k=0;k<16;k++)
            {
                if ( str & (0x8000>>k) )     //0x8000>>k����д���λ�������1��д�ֵ���ɫ
                {
                    write_data8(temp1);
		                write_data8(temp2);
                }
                else		//�����0��д����ɫ
                {                   
                    write_data8(temp3);
		                write_data8(temp4);                                                                                  
                }               
            } 
        }
        x=x+16;		//�м�16
				y=y;
				if(x>114)   //������ʾ
				{
					x =0;
					y=y+16;
				}
				
	   
	 }
	
	
}
//��ʾһ���ַ���
void GUI_DispString(uchar str[],uchar len)
{
  uchar i;
  for(i =0;i<len;i++)
  {
    GUI_DispChar(str[i]);
  }
  
  
}
/****************************************************************************
* ��    �ƣ�void GUI_DispChar(u8 *ASCii);
* ��    �ܣ��ڵ�ǰλ����ʾһ���ַ�
* ��ڲ�����*ASCii�ַ�,�����Ժ�����Ч��
* ���ڲ�������
* ˵    ����2012.5.3 hxm  v1.0
* ���÷������� 
*��ʾһ���ַ��ĺ���

****************************************************************************/
void GUI_DispChar(uchar ASCii)
{
	//GUI_DispCharAt(ASCii,posX,posY);
	GUI_GetPos(1);	
}
/****************************************************************************
* ��    �ƣ�void GUI_GetPos(void);
* ��    �ܣ����㵱ǰ������������λ��
* ��ڲ�����
* ���ڲ�����
* ˵    ����2012.5.3 hxm  v1.0
* ���÷����� 

****************************************************************************/
void GUI_GetPos(uint font)
{
	posX=font*8+posX;		//posX��һ��ȫ�ֱ�������ʾ��8��ʾһ�����ֵĿ��
	if(posX>=128)
	{
	 posX=16+posY; 
	}
	if(posY>=160)
	{
	 posY=0;
	 posX=0;
	}	
}
/****************************************************************************
* ��    �ƣ�void GUI_DispCharAt(u8 *ASCii, u16 x, u16 y);
* ��    �ܣ���ָ��λ����ʾһ��ASCII�ַ�
* ��ڲ�����*ASCii�ַ�x��λ��y��λ��,�����Ժ�����Ч��
* ���ڲ�������
* ˵    ����2012.5.2 hxm  v1.0
* ���÷������� 
*��ʾһ���ַ��ĺ���
*���˷���ģʽ��Ϊ ���� 
****************************************************************************/
void GUI_DispCharAt(uchar *char_sp, uint x, uint y)
{
   
	uint j=0,i=0,k=0;
	uint str,len;
	uint GetColor,GetBKColor;
	uint temp1, temp2, temp3, temp4;
  ulong tempaddr;
	uchar *r_sp;
	uchar info_buf[16];
	tempaddr = *char_sp;
	if(x>120||y>144)return;

	if(GUI_SetDrawMode==1)
	{GetColor=GUI_GetColor;GetBKColor=GUI_GetBKColor;}
	else
	{GetColor=GUI_GetBKColor;GetBKColor=GUI_GetColor;}

	temp1 =  GetColor >> 8;
	temp2 =  GetColor & 0x00ff;
	temp3 =  GetBKColor >> 8;
	temp4 =  GetBKColor & 0x00ff;
  len=sizeof(codeASCII)/sizeof(codeASCII[0]);
   
 	for(j=0;j<len;j++)
	{
		 if((uchar)codeASCII[j].Index[0]==char_sp[0])	  //ָ������ã���Ҫ�ǲ�����
		 break;
	}

      
	if(j<len)
	{
			for (i=0;i<16;i++)
			{
					Set_ramaddr(x,y+i);
					write_cmd(0X2C);

					str = (uchar)codeASCII[j].Msk[i];		//ʹ����������������ģ


					for (k=0;k<8;k++)
					{
							if ( str & (0x80>>k) )     
							{
									write_data8(temp1);
									write_data8(temp2);
							}
							else
							{                   
									write_data8(temp3);
									write_data8(temp4);                                                                                  
							}               
					} 
			}
	 }
	posX=x;
	posY=y;
	//GUI_GetPos(1);
}


/****************************************************************************
* ��    �ƣ�void Set_ramaddr(u16 x,u16 y)
* ��    �ܣ�ָ�������
* ��ڲ�����x��y��
* ���ڲ�������
* ˵    ����2012.4.27 hxm  v1.0
* ���÷������� 
****************************************************************************/ 					  
void Set_ramaddr(uchar x,uchar y)
{
	write_cmd(0x2a);
	write_data8(0x00);
	write_data8(x);

	write_cmd(0x2b);
	write_data8(0x00);
	write_data8(y);
}

void lcd_reset(void)
{
   LCD_RST_DIS;		//��λPB5���͵�ƽ��Ч���ӵ���LCD_RST
   Delay(5);
   LCD_RST_EN;		//����PB5,��λPB5������LCD_RST�ǵ͵�ƽ��Ч
   Delay(2);
   LCD_RST_DIS;		//��λPB5
   Delay(12);
 }



/*д���ݰ汾һ*/
// void write_data8(uchar data)
// { 
// 	
//     Clr_Cs();		//Ƭѡ
//     Set_Rd();		//����Ч
//     Set_Rs();		//��ʾ����
//     GPIOC->ODR = 0xFFFF0000|data;
//     Clr_Wr();		//д������
//     Set_Wr();		
//     Set_Cs();		//ȡ��Ƭѡ

// }



/*д���ݰ汾��*/
// void write_data8(uchar data)
// { 	
//  	Set_Cs();
// 	Set_Rd();
// 	Set_Wr();
// 	Clr_Rs();
// 	GPIOC->ODR = 0xFFFF0000|data;
// 	Clr_Cs();
// 	Clr_Rd();  
// 	Set_Rs();
// 	Set_Rd();
// 	Set_Wr();
// 	Set_Cs();
// }



/*д���ݰ汾��*/
void write_data8(uchar data)
{ 
	
	Clr_Cs();
	Set_Rd();
	Set_Rs();
	//WR(0);
	//Clr_Wr();	
	
  GPIOC->ODR = ((GPIOC->ODR & 0xff00) | data );
	Clr_Wr();	
  //Delay(5);
	//delay_us(1);
	delay_us2(3);//��һ�������ĵ͵�ƽ����д����
	Set_Wr();
	Clr_Rs();
	Set_Rd();
	Set_Cs();
	
// 	CS(0);
// 	RD(1);
// 	RS(1);
// 	//WR(0);
// 	
// 	OutPut_Data(b);
// 	
// 	WR(0);
// 	delay_us(3);//��һ�������ĵ͵�ƽ����д����
// 	WR(1);
// 	RS(0);
// 	RD(1);
// 	CS(1);
	
}


//д��ɫ��iΪ��ɫ��8λ��jΪ��ɫ��8λ
void write_data16(u8 i,u8 j)
{

	Clr_Cs();
	Set_Rs();
	Set_Rd();
	
  GPIOC->ODR = ((GPIOC->ODR & 0xff00) | i );
	Clr_Wr();
	Set_Wr();
	
  GPIOC->ODR = ((GPIOC->ODR & 0xff00) | j );
	Clr_Wr();
	Set_Wr();
	Set_Cs();
	
	
// 	CS(0);
// 	RS(1);
// 	RD(1);
// 	
// 	OutPut_Data(i);
// 	WR(0);
// 	WR(1);
// 	
// 	OutPut_Data(j);
// 	WR(0);
// 	WR(1);
// 	CS(1);	
	
	
	
	
	
	
	

}




/*д����汾һ*/
// void write_cmd(uchar data)
// {
// 		Clr_Cs();		//Ƭѡ
// 		Set_Rd();		//���Ǹ�
// 		Clr_Rs();		//��ʾ����
// 		GPIOC->ODR = 0xFFFF0000|data;
// 		Clr_Wr();		//д������
// 		Set_Wr();
// 		Set_Cs();		//ȡ��Ƭѡ
//   
// }



/*д����汾��*/
// void write_cmd(uchar data)
// {
//   Set_Cs();
// 	Set_Rd();
// 	Set_Rs();
// 	Set_Wr();
// 	GPIOC->ODR = 0xFFFF0000|data;
// 	Clr_Rs();
// 	Clr_Cs();  
// 	Clr_Wr();
// 	Set_Rd();
// 	Set_Wr();
// 	Set_Cs();
// 	Set_Rs();
// }


/*д����汾��*/
void write_cmd(uchar data)
{

	  Clr_Cs();
    Set_Rd();
    Clr_Rs();
    Set_Wr();
    GPIOC->ODR = ((GPIOC->ODR & 0xff00) | data );
    Clr_Wr();   
	
	  //Delay(5);
		//delay_us(1);
	  delay_us2(3);   //1us��һ��,��һ�������ĵ͵�ƽ����д����
    Set_Wr(); 
    Clr_Rs();
    Set_Rd();    
    Set_Cs();  

// 	  CS(0);
//     RD(1);
//     RS(0);
//     WR(1);
//     OutPut_Data(m);
//     WR(0);   
// 	  delay_us(3);//��һ�������ĵ͵�ƽ����д����
//     WR(1); 
//     RS(0);
//     RD(1);    
//     CS(1);	
	
	
	
}







unsigned char read ()
{
 
 return 0;
}

void ReadID()
{	
	
	

	
}


void writePixel(uchar x_start,uchar y_start,uint color)

{
	write_cmd(0x2a);
	write_data8(0x00);
	write_data8(x_start);
	write_data8(0x00);
	write_data8(x_start);
	write_cmd(0x2b);
	write_data8(0x00);
	write_data8(y_start);
	write_data8(0x00);
	write_data8(y_start);
	write_cmd(0x2c);
	write_data8(color>>8);
	write_data8(color&0xff);
	
}


/*
�����������Ҫ

*/
void adress_set(uchar x_start,uchar y_start,uchar x_end,uchar y_end)

{
	write_cmd(0x2a);
	write_data8(0x00);
	write_data8(x_start);
	write_data8(0x00);
	write_data8(x_end);
	
	write_cmd(0x2b);
	write_data8(0x00);
	write_data8(y_start);
	write_data8(0x00);
	write_data8(y_end);
	write_cmd(0x2c);
}



void GUI_Clear(uint color)
{ 
	uchar i,j;
	uint temp1,temp2;
	adress_set(0,0,127,127);
	write_cmd(0X2C);		//Memory write
	temp1=color>>8;
	temp2=color&0x00ff;
	for(i=0;i<128;i++)
	{
	 for(j=0;j<160;j++)
	 {
	  write_data8(temp1);
	  write_data8(temp2);
	 }
	} 
}

static char hex2ch(char hex)
{
	return asc_table[hex];
}

void print_hex(uint x0, uint y0,uchar ForeColor, uchar hex)
{
  /*
	int i,j,k,x,y,xx;
	
	unsigned int qm;
	
	long ulOffset;
	uchar table[4],ywbuf[16];

	table[0] = '0';
	table[1] = 'x';
	table[2] = hex2ch(hex >> 4);
	table[3] = hex2ch(hex & 0x0f);
	for (i = 0;i < 4;i++)
	{
			qm = *(table+i);

			ulOffset = (unsigned int)(qm) * 16;
			
            for (j = 0; j < 16; j ++)
            {
				ywbuf[j]=ASCII[ulOffset+j];
           	}
             
            for(y = 0;y < 16;y++)
            {
	          	for(x=0;x<8;x++) 
	           	{
                	k=x % 8;
                	
				  	if(ywbuf[y]&(0x80 >> k))
				   	{
				  		xx=x0+x+i*8;
				    	writePixel(xx,y+y0,ForeColor);
				  	}
			   	}
           }
	}
  */
}


void DisplayHNumber(unsigned int x0, unsigned int y0, unsigned int color, unsigned char s)
{
   unsigned char  i,k;
   k=s;

	

	for(i=0;i<8;i++)
	{  
            if((k&0x80)==0x80)
                  {
             DisplayNumber( x0+i*10,y0,color,1);
                   }
            
             
            else
            {
                 DisplayNumber( x0+i*10,y0,color,0);
             }
                    k=k<<1;
        }
	
}




void DisplayNumber(unsigned int x0, unsigned int y0, unsigned int color, unsigned char s)
{
	//int  xx;
	int   i,j,x,y,xx;  
	unsigned char  qm;
	long int  ulOffset;
	char   ywbuf[16],temp[2];

	for(i = 0; i<1;i++)
	{
		if(s >= 161)
		{
			temp[0] = s+i;
			temp[1] = '\0';
			return;
		}
		else
		{
			qm =  s+48;
	    		//wm = *(s+i+1)-161;
			ulOffset = (long int)(qm) * 16;
			for (j = 0; j < 16; j ++)
			{
				ywbuf[j]=ASCII[ulOffset+j];
			}
			for(y = 0;y < 16;y++)
			{
				for(x=0;x<8;x++) 
				{
					//k=x % 8;
					if((ywbuf[y]&(0x80 >> x)))
					{
					
						xx=x0+x+i*8;
						writePixel(xx,y+y0,color);
					}
				}
			}
		    	//i++;
		}
	}     	

}


void show_pure_image(unsigned char DataH,unsigned char DataL)
{
  int i,j;
  adress_set(0,0,127,127);

  for(i=128;i>0;i--)
  {
   for(j=128;j>0;j--)
   {
   	 write_data8(DataH);
	 write_data8(DataL);
	
	}
  }

}


void display_gray()
{
	
	
	uint a;
	int i,j,k;

	adress_set(0,0,127,159);	
	for(i=0;i<80;i++)
	 {
			for(j=0;j<16;j++)
			 {
					for(k=0;k<8;k++)
				 {
						a=0xffff-0x1082*j;
			 write_data8(a>>8);
						 write_data8(a);
						 }
				}

		 }

   for(i=0;i<80;i++)
     {
        for(j=0;j<16;j++)
         {
            for(k=0;k<8;k++)
           {
              a=0x0000+0x1082*j;
               write_data8(a>>8);
               write_data8(a);
             }
           }
        }
}


void show_H_bar(void)
{
 int i,j;
 for(i=128;i>0;i--)
  for(j=128;j>0;j--)
   if(i%2==0)
     {write_data8(0xff);write_data8(0xff);}
   else
     {write_data8(0x00);write_data8(0x00);}
}

void show_V_bar(void)
{
 int i,j;
 for(i=128;i>0;i--)
  for(j=128;j>0;j--)
   if(j%2==0)
     {write_data8(0xff);write_data8(0xff);}
   else
     {write_data8(0x00);write_data8(0x00);}
}


void show_frame(void)
{
 int i,j;
 for(i=160;i>0;i--)
  for(j=128;j>0;j--)
   if(i==1||i==160||j==1||j==128)
    {write_data8(0xff);
     write_data8(0xff);}
   else
     {write_data8(0x00);
     write_data8(0x00);}     
}

void show_cs_open(void)
{
	int i,j;
  for(i=160;i>0;i--)
  for(j=128;j>0;j--)
   if((i+j)%2==0)
    {write_data8(0xff);
     write_data8(0xff);}
   else
     {write_data8(0x00);
      write_data8(0x00);}    
}


 /****************************************************************************
* ��    �ƣ�void GUI_DispCharAt(u8 *ASCii, u16 x, u16 y);
* ��    �ܣ���ָ��λ����ʾһ���ַ�
* ��ڲ�����*ASCii�ַ�x��λ��y��λ��,�����Ժ�����Ч��
* ���ڲ�������
* ˵    ����2012.5.2 hxm  v1.0
* ���÷������� 
****************************************************************************/
void GUI_DispCharAt_Test(uchar ASCii, uint x, uint y)
{
	uint j=0,i=0,k=0;
	uint str,len;
	uint GetColor,GetBKColor;
	uint temp1, temp2, temp3, temp4;

	if(x>120||y>144)return;

	if(GUI_SetDrawMode==1)
	{GetColor=GUI_GetColor;GetBKColor=GUI_GetBKColor;}
	else
	{GetColor=GUI_GetBKColor;GetBKColor=GUI_GetColor;}

	temp1 =  GetColor >> 8;
	temp2 =  GetColor & 0x00ff;
	temp3 =  GetBKColor >> 8;
	temp4 =  GetBKColor & 0x00ff;
 
  len=sizeof(codeASCII)/sizeof(codeASCII[0]);
   
 	for(j=0;j<len;j++)
	{
		  if((uchar)codeASCII[j].Index[0]==ASCii)		//�ҵ�������ģ
				break;
	}
      
	if(j<len)
	{
			for (i=0;i<16;i++)
			{
					Set_ramaddr(x,y+i);
					write_cmd(0X2C);		//�洢��д

					str = (uchar)codeASCII[j].Msk[i];

					for (k=0;k<8;k++)
					{
							if ( str & (0x80>>k) )     
							{
									write_data8(temp1);		//ÿ�����ص������ֽ�
									write_data8(temp2);
							}
							else
							{                   
									write_data8(temp3);
									write_data8(temp4);                                                                                  
							}               
					} 
			}
	}
	posX=x;
	posY=y;
	//GUI_GetPos(1);
}

void show_picture1()
{
  /*
  int i,j,k;
  k=0x00;
  
  for(i=80;i>0;i--)
   for(j=128;j>0;j--)
   {  
    write_data8(picture1[k++]);
    write_data8(picture1[k++]); 
   }
  */
}
void show_picture2()
{
  /*
  int i,j,k;
  k=0x00;
  
  for(i=80;i>0;i--)
   for(j=128;j>0;j--)
   {  
    write_data8(picture2[k++]);
    write_data8(picture2[k++]); 
   }
  */
}

void lcd_init(void)	
{ 
//#if (LCD==tianma)

  write_cmd(0x11);
  Delay(20);
   
	
	write_cmd(0xB1); //֡�ʿ���
	write_data8(0x01);  
	write_data8(0x2C);
	write_data8(0x2D);

	write_cmd(0xB2);
	write_data8(0x01);  
	write_data8(0x2C);
	write_data8(0x2D);

	write_cmd(0xB3);
	write_data8(0x01);   
	write_data8(0x2C);
	write_data8(0x2D);
	write_data8(0x01);
	write_data8(0x2C);
	write_data8(0x2D);
	
	write_cmd(0xB4); 
	write_data8(0x03); 	
	
	write_cmd(0xB6);   
	write_data8(0xB4); 
	write_data8(0xF0); 
	
	write_cmd(0xC0);
	write_data8(0xA2);
	write_data8(0x02);
	write_data8(0x84);

	write_cmd(0xC1);
	write_data8(0xC5);

	write_cmd(0xC2);
	write_data8(0x0A);
	write_data8(0x00);

	write_cmd(0xC3);
	write_data8(0x8A);
	write_data8(0x2A);

	write_cmd(0xC4);
	write_data8(0x8A);
	write_data8(0xEE);
	
	write_cmd(0xC5); 
	write_data8(0x0A);   
	
	write_cmd(0x36); //MX, MY, RGB mode
	write_data8(0xC8);   //0x68
	
	write_cmd(0xe0);
	write_data8(0x02);
	write_data8(0x1c);
	write_data8(0x07);
	write_data8(0x12);
	write_data8(0x37);
	write_data8(0x32);
	write_data8(0x29);
	write_data8(0x2d);
	write_data8(0x29);
	write_data8(0x25);
	write_data8(0x2b);
	write_data8(0x39);
	write_data8(0x00);
	write_data8(0x01);
	write_data8(0x03);
	write_data8(0x10);
	
	write_cmd(0xe1);
	write_data8(0x03);
	write_data8(0x1d);
	write_data8(0x07);
	write_data8(0x06);
	write_data8(0x2e);
	write_data8(0x2c);
	write_data8(0x29);
	write_data8(0x2d);
	write_data8(0x2e);
	write_data8(0x2e);
	write_data8(0x37);
	write_data8(0x3f);
	write_data8(0x00);
	write_data8(0x00);
	write_data8(0x02);
	write_data8(0x10);
	
	write_cmd(0x2a);
	write_data8(0x00);
	write_data8(0x00);
	write_data8(0x00);
	write_data8(0x7f);
		  
	write_cmd(0x2b);
	write_data8(0x00);
	write_data8(0x00);
	write_data8(0x00);
	write_data8(0x9f);
	
	write_cmd(0xF0); 
	write_data8(0x01);
	write_cmd(0xF6); 
	write_data8(0x00);
	
	write_cmd(0x3A); //65k mode
	write_data8(0x05);
	//write_data8(0x06);//256K ��ɫλ�д���ȶ	
	write_cmd(0x29); //Display on
  /** 
  write_cmd(0xb1);
  write_data8(0x01);
  write_data8(0x35);
  write_data8(0x36);

  write_cmd(0xb2);
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);

  write_cmd(0xb3);
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);

  write_cmd(0xb4);
  write_data8(0x03);
  
 // write_cmd(0xb6);
 // write_data8(0x84);
 // write_data8(0xf0);
 // write_data8(0x20);
    
  write_cmd(0xc0);
  write_data8(0xa2);
  write_data8(0x02);
  write_data8(0x84);
   
  write_cmd(0xc1);
  write_data8(0xc5);

  write_cmd(0xc2);
  write_data8(0x0d);
  write_data8(0x00);

  write_cmd(0xc3);
  write_data8(0x8a);
  write_data8(0x2a);

  write_cmd(0xc4);
  write_data8(0x8a);
  write_data8(0xee);
   
  write_cmd(0xc5);
  write_data8(0x03);
  
  
  write_cmd(0xe0);
  write_data8(0x12);
  write_data8(0x1c);
  write_data8(0x10);
  write_data8(0x18);
  write_data8(0x33);
  write_data8(0x2c);
  write_data8(0x25);
  write_data8(0x28);
  write_data8(0x28);
  write_data8(0x27);
  write_data8(0x2f);
  write_data8(0x3c);
  write_data8(0x00);
  write_data8(0x03);
  write_data8(0x03);
  write_data8(0x10); 
 
  write_cmd(0xe1);
  write_data8(0x12);
  write_data8(0x1d);
  write_data8(0x10);
  write_data8(0x18);
  write_data8(0x2d);
  write_data8(0x28);
  write_data8(0x23);
  write_data8(0x28);
  write_data8(0x28);
  write_data8(0x26);
  write_data8(0x2f);
  write_data8(0x3b);
  write_data8(0x00);
  write_data8(0x03);
  write_data8(0x03);
  write_data8(0x10);
  write_cmd(0x36);
  write_data8(0xc8);
  
 
  write_cmd(0xf0);
  write_data8(0x01);
  write_cmd(0xf6);
  write_data8(0x00);
 
  write_cmd(0x3a);
  write_data8(0x05);
  
  write_cmd(0x29);
  write_cmd(0x2c);
  ***/


			
}


void LCD_init2(void)
{
  uchar j,i; 

	write_cmd(0x01);   //�����λ
	Delay(5);
	Delay(20);
	write_cmd(0x11);//SLEEP OUT
	Delay(20);
	Delay(20);

	write_cmd(0xFF);		//ILI9163Cû��
	write_data8(0x40);
	write_data8(0x03);
	write_data8(0x1A);

	write_cmd(0xd9);      //ILI9163û��
	write_data8(0x60);
	write_cmd(0xc7);
	write_data8(0x90);
	Delay(20);

	write_cmd(0xB1);		//Frame Rate Control
	write_data8(0x04);
	write_data8(0x25);
	write_data8(0x18);

	write_cmd(0xB2);		//Frame Rate Control
	write_data8(0x04);
	write_data8(0x25);
	write_data8(0x18);

	write_cmd(0xB3);		//Frame Rate Control(In Partial mode/full colors)
	write_data8(0x04);
	write_data8(0x25);
	write_data8(0x18);
	write_data8(0x04);
	write_data8(0x25);
	write_data8(0x18);

	write_cmd(0xB4);		//Display Inversion Control
	write_data8(0x03);

	write_cmd(0xB6);		//Display Function Set
	write_data8(0x15);
	write_data8(0x02);

	write_cmd(0xC0);// POWER CONTROL 1 GVDD&VCI1
	write_data8(0x02);
	write_data8(0x70);

	write_cmd(0xC1);// POWER CONTROL 2 GVDD&VCI1
	write_data8(0x07);

	write_cmd(0xC2);// POWER CONTROL 3 GVDD&VCI1
	write_data8(0x01);
	write_data8(0x01);

	write_cmd(0xC3);// POWER CONTROL 4 GVDD&VCI1
	write_data8(0x02);
	write_data8(0x07);

	write_cmd(0xC4);// POWER CONTROL 5 GVDD&VCI1
	write_data8(0x02);
	write_data8(0x04);

	write_cmd(0xFC);// POWER CONTROL 6 GVDD&VCI1
	write_data8(0x11);
	write_data8(0x17);

	write_cmd(0xC5);//VCOMH&VCOML
	write_data8(0x3c);
	write_data8(0x4f);

	write_cmd(0x36);//MV,MX,MY,RGB
	write_data8(0xC8); 


	write_cmd(0x3a);//GAMMA SET BY REGISTER
	write_data8(0x05);

	//***********************GAMMA*************************
	write_cmd(0xE0);
	write_data8(0x06);
	write_data8(0x0E);
	write_data8(0x05);
	write_data8(0x20);
	write_data8(0x27);
	write_data8(0x23);
	write_data8(0x1C);
	write_data8(0x21);
	write_data8(0x20);
	write_data8(0x1C);
	write_data8(0x26);
	write_data8(0x2F);
	write_data8(0x00);
	write_data8(0x03);
	write_data8(0x00);
	write_data8(0x24);

	write_cmd(0xE1);
	write_data8(0x06);
	write_data8(0x10);
	write_data8(0x05);
	write_data8(0x21);
	write_data8(0x27);
	write_data8(0x22);
	write_data8(0x1C);
	write_data8(0x21);
	write_data8(0x1F);
	write_data8(0x1D);
	write_data8(0x27);
	write_data8(0x2F);
	write_data8(0x05);
	write_data8(0x03);
	write_data8(0x00);
	write_data8(0x3F);

	//***************************RAM ADDRESS*******************
	write_cmd(0x2A);
	write_data8(0x00);
	write_data8(0x02);
	write_data8(0x00);
	write_data8(0x81);

	write_cmd(0x2B);
	write_data8(0x00);
	write_data8(0x03);
	write_data8(0x00);
	write_data8(0x82);

	write_cmd(0x29);
	Delay(10);

	write_cmd(0x2C);
                             //��ʼ������
  /**
  for (i=0;i<128;i++)
    {
    for (j=0;j<128;j++)
       {
           write_data8(0xf8);
					 write_data8(0x00);  //display red
        }
     }
	 **/

}

void Lcd_Init3(void)
{
	write_cmd(0x11);		//sleep out
	Delay(20);
 
   
  write_cmd(0xb1);		//Frame Rate Control (In normal mode/Full colors)
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);

  write_cmd(0xb2);		//Frame Rate Control(In Idle mode/8-colors)
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);

  write_cmd(0xb3);		//Frame Rate Control(In Partial mode/full colors)
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);

  write_cmd(0xb4);		//Display Inversion Control
  write_data8(0x03);
  

  write_cmd(0xc0);		//Power_Control1
  write_data8(0xa2);
  write_data8(0x02);
  write_data8(0x84);
   
  write_cmd(0xc1);		//Power_Control2
  write_data8(0xc5);

  write_cmd(0xc2);		//Power_Control3
  write_data8(0x0d);
  write_data8(0x00);

  write_cmd(0xc3);		//Power_Control4
  write_data8(0x8a);
  write_data8(0x2a);

  write_cmd(0xc4);		//Power_Control 5
  write_data8(0x8a);
  write_data8(0xee);
   
  write_cmd(0xc5);		//VCOM_Control 1
  write_data8(0x03);
  
  
  write_cmd(0xe0);		//Positive Gamma Correction Setting
  write_data8(0x12);
  write_data8(0x1c);
  write_data8(0x10);
  write_data8(0x18);
  write_data8(0x33);
  write_data8(0x2c);
  write_data8(0x25);
  write_data8(0x28);
  write_data8(0x28);
  write_data8(0x27);
  write_data8(0x2f);
  write_data8(0x3c);
  write_data8(0x00);
  write_data8(0x03);
  write_data8(0x03);
  write_data8(0x10); 
 
  write_cmd(0xe1);		//Negative Gamma Correction Setting
  write_data8(0x12);
  write_data8(0x1d);
  write_data8(0x10);
  write_data8(0x18);
  write_data8(0x2d);
  write_data8(0x28);
  write_data8(0x23);
  write_data8(0x28);
  write_data8(0x28);
  write_data8(0x26);
  write_data8(0x2f);
  write_data8(0x3b);
  write_data8(0x00);
  write_data8(0x03);
  write_data8(0x03);
  write_data8(0x10);
	
  write_cmd(0x36);		//Memory Access Control
  write_data8(0xc8);
  
 
  write_cmd(0xf0);		//ILI9163C��û�����оƬ
  write_data8(0x01);
  write_cmd(0xf6);
  write_data8(0x00);		//ILI9163C��û�����оƬ
 
  write_cmd(0x3a);		//Interface Pixel Format
  write_data8(0x05);
  
  write_cmd(0x29);		//Display Off
  write_cmd(0x2c);		//Display On


}



void Lcd_Init4()
{
	
	// VCI=2.8V
//************* Reset LCD Driver ****************//
	
// 	write_cmd(0x01);   //�����λ
// 	Delay(5);
// 	Delay(20);
	
   Set_Rst();
   delay_ms2(67);
   Clr_Rst();
   delay_ms2(131);
   Set_Rst();
   delay_ms2(131);	
	
   Set_Rst();
   delay_ms2(12);
   Clr_Rst();
   delay_ms2(110);
   Set_Rst();
   delay_ms2(219);		
	 
	 
// RES(1);
// delay_ms(12); // Delay 1ms
// RES(0);
// delay_us(110); // Delay 10ms // This delay time is necessary
// RES(1);
// delay_ms(219);	
	
	
	
	//************* Start Initial Sequence **********//
	write_cmd(0x11); //Exit Sleep
  delay_ms2(44);
	write_cmd(0x26); //Set Default Gamma
	write_data8(0x04);
	
	write_cmd(0xF2); //E0h & E1h Enable/Disable
	write_data8(0x00);
	
  write_cmd(0xb1);		//Frame Rate Control (In normal mode/Full colors)
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);

  write_cmd(0xb2);		//Frame Rate Control(In Idle mode/8-colors)
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);

  write_cmd(0xb3);		//Frame Rate Control(In Partial mode/full colors)
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);
  write_data8(0x02);
  write_data8(0x35);
  write_data8(0x36);
	
	write_cmd(0xC0); //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
	write_data8(0x0C);
	write_data8(0x05);
	write_cmd(0xC1); //Set BT[2:0] for AVDD & VCL & VGH & VGL
	write_data8(0x02);
	write_cmd(0xC5); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
	write_data8(0x32);
	write_data8(0x3B);
	
	write_cmd(0xC7);// VCOM Offset Control
	write_data8(0x40);
	
	write_cmd(0x2A); //Set Column Address
	write_data8(0x00);
	write_data8(0x00);
	write_data8(0x00);
	write_data8(0x7F);
	
	write_cmd(0x2B); //Set Page Address
	write_data8(0x00);
	write_data8(0x00);
	write_data8(0x00);
	write_data8(0x9F);
	
	write_cmd(0x36); //Set Scanning Direction  //Memory Access Control
	write_data8(0xC0);
	
	write_cmd(0x3A); //Interface Pixel Format
	write_data8(0x55);
	write_cmd(0x29); // Display On

	//Display Off (28h)
	//write_cmd(0x28);


 }



 void Lcd_Init5()
 {
//************* Reset LCD Driver ****************//
	
	write_cmd(0x01);   //�����λ
	Delay(5);
	Delay(20);
//************* Start Initial Sequence **********//
write_cmd(0x11); //Exit Sleep
delay_ms(44);
write_cmd(0x26); //Set Default Gamma
write_data8(0x04);
write_cmd(0xF2); //E0h & E1h Enable/Disable
write_data8(0x00);
write_cmd(0xB1);
write_data8(0x0C);
write_data8(0x14);
write_cmd(0xC0); //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
write_data8(0x0C);
write_data8(0x05);
write_cmd(0xC1); //Set BT[2:0] for AVDD & VCL & VGH & VGL
write_data8(0x02);
write_cmd(0xC5); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
write_data8(0x32);
write_data8(0x3B);
write_cmd(0xC7);
write_data8(0x40);
write_cmd(0x2A); //Set Column Address
write_data8(0x00);
write_data8(0x00);
write_data8(0x00);
write_data8(0x7F);
write_cmd(0x2B); //Set Page Address
write_data8(0x00);
write_data8(0x00);
write_data8(0x00);
write_data8(0x9F);
write_cmd(0x36); //Set Scanning Direction
write_data8(0xC0);
write_cmd(0x3A); 
write_data8(0x55);
write_cmd(0x29); // Display On



}
 
 
 
 

 /****
  ����LCD��ʾ�ĺ���

 *****/
void Test_Plcd(void)
{
   uchar i,j;
   lcd_reset();

   //lcd_init();
   //LCD_init2();
   Lcd_Init3();
   Delay(20);
	
   GUI_Clear(White);
   GUI_DispCharAt_Test('a',0,0);
   //often_show(36,56,"��ʼ����...",0,0x0fff);
   while(1);

}


 /**
  ������LCD��ʾ����
       ��ʾʱ�䣺
      ������ ʱ����
      �û��ı�ţ�

 time_buff[0] =date_s.year;
 time_buff[1] =date_s.month;
 time_buff[2] =date_s.day;
 time_buff[3] =date_s.hour;
 time_buff[4] =date_s.min;
 time_buff[5] =date_s.sec;
 ��������ʾ����
 2014��05��05��
 10ʱ59��59��
 �ݺͿƼ�
 ��������Կ��
 ʱ --65F6
 �� --5206
 �� --79D2
 **/
void Main_Lcd_Show(void)
{
  uchar  tempa;
  uchar  lcd_buf[15],lcd_buf1[15];		//lcd_buf�������գ�lcd_buf1��ʱ����
  read_time();  //��ȡʵʱʱ��,�ŵ�time_buffer��
	lcd_buf[0] = '2';
	lcd_buf[1] = '0';
	tempa = (time_buff[0]>>4)&0x0f;
	lcd_buf[2] = tempa+0x30;		//ת��Ϊ�ַ�
	tempa = (time_buff[0]&0x0f);
	lcd_buf[3] = tempa + 0x30;
	lcd_buf[4] = 0xc4;
	lcd_buf[5] = 0xea;   //�������
	tempa = (time_buff[1]>>4)&0x0f;
	lcd_buf[6] = tempa+0x30;
	tempa = time_buff[1]&0x0f;
	lcd_buf[7] = tempa+0x30;
	lcd_buf[8] = 0xd4;
	lcd_buf[9] = 0xc2;   //�µ�����
	tempa = (time_buff[2]>>4)&0x0f;
	lcd_buf[10] = tempa+0x30;
	tempa = time_buff[2]&0x0f;
	lcd_buf[11] = tempa+0x30;
	lcd_buf[12] = 0xc8;
	lcd_buf[13] = 0xd5;   //�յ�����
	lcd_buf[14] = 0;   //������־

	tempa = (time_buff[3]>>4)&0x0f;
	lcd_buf1[0] = tempa+0x30;
	tempa = time_buff[3]&0x0f;
	lcd_buf1[1] = tempa + 0x30;
	lcd_buf1[2] = 0xCA;
	lcd_buf1[3] = 0xB1;   //ʱ
	tempa = (time_buff[4]>>4)&0x0f;
	lcd_buf1[4] = tempa+0x30;
	tempa = time_buff[4]&0x0f;
	lcd_buf1[5] = tempa+0x30;
	lcd_buf1[6] = 0xb7;
	lcd_buf1[7] = 0xd6;   //��
	tempa = (time_buff[5]>>4)&0x0f;
	lcd_buf1[8] = tempa+0x30;
	tempa = time_buff[5]&0x0f;
	lcd_buf1[9] = tempa+0x30;
	lcd_buf1[10] = 0xc3;
	lcd_buf1[11] = 0xeb;   //��
	lcd_buf1[12] = 0;   //������־
	
	lcd_buf2[0]=lcd_buf1[0];
	lcd_buf2[1]=lcd_buf1[1];
	lcd_buf2[2]=':';
	lcd_buf2[3]=lcd_buf1[4];
	lcd_buf2[4]=lcd_buf1[5];
	lcd_buf2[5]=':';
	lcd_buf2[6]=lcd_buf1[8];
	lcd_buf2[7]=lcd_buf1[9];
	lcd_buf2[8]=0;
	
	//void apoint_place_write(uchar s_column,uchar s_row,uchar *wr_sp,uint w_color,uint bk_color)
	apoint_place_write(0,0,"�ź�",0,0xfff);  //
 	apoint_place_write(32,0,lcd_buf2,0,0x0fff);	
	apoint_place_write(96,0,"����",0,0xfff);  //


 	apoint_place_write(48,48,"��1",0,0x0fff);
  apoint_place_write(36,64,"Կ��1",0,0x0fff);
	
	apoint_place_write(0,112,"����",0,0x0fff);
	apoint_place_write(96,112,"����",0,0x0fff);
}




void Main_Lcd_Show2(void)
{
  uchar  tempa;
  uchar  lcd_buf[15],lcd_buf1[15];		//lcd_buf�������գ�lcd_buf1��ʱ����
  read_time();  //��ȡʵʱʱ��,�ŵ�time_buffer��
	lcd_buf[0] = '2';
	lcd_buf[1] = '0';
	tempa = (time_buff[0]>>4)&0x0f;
	lcd_buf[2] = tempa+0x30;		//ת��Ϊ�ַ�
	tempa = (time_buff[0]&0x0f);
	lcd_buf[3] = tempa + 0x30;
	lcd_buf[4] = 0xc4;
	lcd_buf[5] = 0xea;   //��
	tempa = (time_buff[1]>>4)&0x0f;
	lcd_buf[6] = tempa+0x30;
	tempa = time_buff[1]&0x0f;
	lcd_buf[7] = tempa+0x30;
	lcd_buf[8] = 0xd4;
	lcd_buf[9] = 0xc2;   //��
	tempa = (time_buff[2]>>4)&0x0f;
	lcd_buf[10] = tempa+0x30;
	tempa = time_buff[2]&0x0f;
	lcd_buf[11] = tempa+0x30;
	lcd_buf[12] = 0xc8;
	lcd_buf[13] = 0xd5;   //��
	lcd_buf[14] = 0;   //������־

	tempa = (time_buff[3]>>4)&0x0f;
	lcd_buf1[0] = tempa+0x30;
	tempa = time_buff[3]&0x0f;
	lcd_buf1[1] = tempa + 0x30;
	lcd_buf1[2] = 0xCA;
	lcd_buf1[3] = 0xB1;   //ʱ
	tempa = (time_buff[4]>>4)&0x0f;
	lcd_buf1[4] = tempa+0x30;
	tempa = time_buff[4]&0x0f;
	lcd_buf1[5] = tempa+0x30;
	lcd_buf1[6] = 0xb7;
	lcd_buf1[7] = 0xd6;   //��
	tempa = (time_buff[5]>>4)&0x0f;
	lcd_buf1[8] = tempa+0x30;
	tempa = time_buff[5]&0x0f;
	lcd_buf1[9] = tempa+0x30;
	lcd_buf1[10] = 0xc3;
	lcd_buf1[11] = 0xeb;   //��
	lcd_buf1[12] = 0;   //������־
	
	
	
	//void apoint_place_write(uchar s_column,uchar s_row,uchar *wr_sp,uint w_color,uint bk_color)
	apoint_place_write(0,1,lcd_buf,0,0xfff);  //������
	apoint_place_write(0,17,lcd_buf1,0,0xfff);  //ʱ����
	apoint_place_write(36,56,"�ݺϿƼ�",0,0x0fff);
	apoint_place_write(2,74,"��������Կ��",0,0x0fff);


}
