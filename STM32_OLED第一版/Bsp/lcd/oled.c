//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途

//********************************************************************************
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  
#include "sw-key.h"
#include "delay.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include "stdint.h"
#include "Timer.h"
#include <string.h>
#include "rfid.h"
#include "Li_Power.h"
#include "usb.h"
#include "Globle_Var.h"
//#include "delay.h"

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
unsigned char  OLED_GRAM[128][8];	
unsigned char  f_CurSel;
uchar  lcd_buf2[15];
unsigned char bijiao[39]=" ";
unsigned int flagi=0;//屏保标志
//uint8_t guanjiflag=0;//关机标志
unsigned char rightcode[]="1234";//正确密码
unsigned char code[4]="0000";
int choosetime=200;
uint8_t g_State;
uint8_t KeyNum;
uint8_t result=0;
uchar zuming[8];
uchar zuming1[2];
uchar zuming2[2];
uchar zuming3[2];
uchar zuming4[2];
uint8_t g_WaitTimeOut;

extern uchar g_Buff[520];
extern UARTBUFF1  m_UB1;
extern uchar Rfid_Buff[6];

unsigned char Lock_number[4];

uchar rfidrow=0;

uchar  lcd_buf_batt0[] = {0x00,0x00,0x7F,0xFE,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,
0x5F,0xFA,0x40,0x02,0x40,0x02,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,
0x5F,0xFA,0x5F,0xFA,0x40,0x02,0x40,0x02,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,
0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x7C,0x3E,0x04,0x20,0x04,0x20,0x04,0x20,0x07,0xE0};
uchar  lcd_buf_batt1[] = {0x00,0x00,0x7F,0xFE,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,
0x5F,0xFA,0x40,0x02,0x40,0x02,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,
0x5F,0xFA,0x5F,0xFA,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,
0x40,0x02,0x40,0x02,0x40,0x02,0x7C,0x3E,0x04,0x20,0x04,0x20,0x04,0x20,0x07,0xE0};
uchar  lcd_buf_batt2[] = {0x00,0x00,0x7F,0xFE,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,0x5F,0xFA,
0x5F,0xFA,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,
0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x7C,0x3E,0x04,0x20,
0x04,0x20,0x04,0x20,0x07,0xE0};
uchar  lcd_buf_batt3[] = {0x00,0x00,0x7F,0xFE,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,
0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,
0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x7C,0x3E,0x04,0x20,
0x04,0x20,0x04,0x20,0x07,0xE0};
uchar  lcd_buf_signal[]= {0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x00,0x00,0x1F,0xFF,0x1F,0xFF,0x1F,0xFF,0x1F,0xFF,0x1F,0xFF,0x00,0x00,0x03,0xFF,0x03,0xFF,0x03,0xFF,0x03,
0xFF,0x03,0xFF,0x00,0x00,0x00,0x7F,0x00,0x7F,0x00,0x7F,0x00,0x7F,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};
uchar  lcd_buf_nosignal[]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xC0,0x3F,0xF0,0x3F,0xFB,0x3F,0xFB,0x3F,0xF0,0x1F,
0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};
const unsigned char ASCII[]=
{
  0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,'0',  // -0-
	0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,'1',
	0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x04,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00,'2',
	0x00,0x00,0x00,0x3C,0x42,0x42,0x04,0x18,0x04,0x02,0x02,0x42,0x44,0x38,0x00,0x00,'3',
	0x00,0x00,0x00,0x04,0x0C,0x14,0x24,0x24,0x44,0x44,0x7E,0x04,0x04,0x1E,0x00,0x00,'4',
	0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00,'5',
	0x00,0x00,0x00,0x1C,0x24,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x24,0x18,0x00,0x00,'6',
	0x00,0x00,0x00,0x7E,0x44,0x44,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,'7',
	0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00,'8',
	0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x26,0x1A,0x02,0x02,0x24,0x38,0x00,0x00,'9',

	0x00,0x00,0x00,0x10,0x10,0x18,0x28,0x28,0x24,0x3C,0x44,0x42,0x42,0xE7,0x00,0x00,'A',
	0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x78,0x44,0x42,0x42,0x42,0x44,0xF8,0x00,0x00,'B',	
	0x00,0x00,0x00,0x3E,0x42,0x42,0x80,0x80,0x80,0x80,0x80,0x42,0x44,0x38,0x00,0x00,'C',	
	0x00,0x00,0x00,0xF8,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0xF8,0x00,0x00,'D',	
	0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x42,0x42,0xFC,0x00,0x00,'E',	
	0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x40,0x40,0xE0,0x00,0x00,'F',
	
};
const unsigned char asc_table[] = "0123456789ABCDEF"; 
#define MAX_MENU      10
const MENU_ITEM HsMenu[MAX_MENU]={   //主菜单
    {3,4,"",0},	//其结构的第0个为特别的，表示可选菜单项的起始项，和最后项，并始终将可选项放在最后
    //{0,0, "信号",ifauthorize},
    //{32,0, lcd_buf2[15],0},
    //{0,96, lcd_buf_batt[2][32],0},
    {48,48," ",0},
		{48,16,"钥匙",0},
		{0,48,"工具",tool_menu},//
		{96,48,"设置",set_menu}//
    };

void hs_menu(void)
{
	Choose_MenuOp1();
	return;
}
//显示工具菜单
void tool_menu(void)
{
	Choose_MenuOp4();
	return;
}
	
		
//显示设置菜单
void set_menu(void)
{
	Choose_MenuOp5();
	return;
}

const MENU_ITEM TOOLMENU[MAX_MENU]={
		{3,5,"",0},
    {0,48,"确定",0},
		{96,48,"取消",0},
    {36,16,"检查锁具",mima},
		{16,32,"采码",code_menu},
    {80,32,"关机",guanji},
    
    
		
};
void guanji(void)
{
	Choose_MenuOp4_3();;
	return;
}

void mima(void)
{
	Choose_MenuOp4_22();;
	return;
}
MENU_ITEM MIMA[MAX_MENU]={
		{5,8,"",0},  
		{24,16,"请输入密码",0},
    {0,48,"确定",0},
    {48,48,"选择",0},
		{96,48,"返回",0},
    {32,25,"0",0},
    {48,25,"0",0},
    {64,25,"0",0},
    {80,25,"0",0},		
};
const MENU_ITEM GUANJI[MAX_MENU]={
		{2,3,"",0},
		{35,16,"是否关机",0},
    {0,48,"是",guanping},
		{112,48,"否",0}
		
};
void guanping(void)
{
	OLED_Display_Off();
	guanjiflag=1;
	return;
}
const MENU_ITEM OPEN[MAX_MENU]={
		{2,2,"",0},
    {48,16,"开锁中",0},
    {48,144,"返回",close}
    
};

void code_menu(void)
{
	Choose_MenuOp4_1();
	return;
}	
void open(void)
{
	
	Choose_MenuOp4_2();
	return;
}
void close(void)
{
	OLED_Display_Off();
	Choose_MenuOp4();
	return;
}
	
const MENU_ITEM CODEMENU[MAX_MENU]={
		{3,4,"",0},
		{48,0,"读RFID",0},
		{32,16," ",0},
    {0,48,"开始",0},
		{96,48,"返回",tool_menu}
};
const MENU_ITEM SETMENU[MAX_MENU]={
		{3,5,"",0},
		{0,48,"确定",hs_menu},
		{96,48,"取消",hs_menu},
    {16,0,"屏保设置",time},
		{16,16,"亮度设置",0},
		{64,32,"关于",banben},//pfun
};
const MENU_ITEM TIME[MAX_MENU]={
		{3,5,"",0},
		{0,48,"确定",hs_menu},
		{96,48,"取消",hs_menu},
    {48,0,"10秒",0},
		{48,16,"30秒",0},
		{48,32,"50秒",0},//pfun
};
void time(void)
{
	Choose_MenuOp5_1();
	return;
}
void banben(void)
{
	Choose_MenuOp5_3();
	return;
}
const MENU_ITEM BANBEN[MAX_MENU]={
		{4,4,"",0},
		{0,0,"版本：V1.0",0},
		{80,0,"2015",0},
		{8,16,"武汉百景科技",0},
		{144,96,"返回",0}
};
static char hex2ch(char hex)
{
	return asc_table[hex];
}

/*************************************************
函数功能：硬件SPI口发送或者接收一个字节数据

*************************************************/

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
unsigned char SPI2_ReadWriteByte(unsigned char TxData)
{		
	unsigned int retry=0;				 
	while((SPI2->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>2000)return 0;
	}			  
	SPI2->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>2000)return 0;
	}	  						    
	return SPI2->DR;          //返回收到的数据				    
}


//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	unsigned char  i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(unsigned char  dat,unsigned char  cmd)
{	
	//unsigned char  i;			  
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();	//命令模式或者数据模式	  
	
	
	
	  OLED_CS_Clr();//片选
	
	//添加硬件读写SPI函数
	
	SPI2_ReadWriteByte(dat);
	 		  
	OLED_CS_Set();
	OLED_RS_Set();   	  
} 
	  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	unsigned char  i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}
void OLED_Clear2(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2)  
{  
  OLED_Fill(x1,y1,x2,y2,0);
}
//设置坐标点
void LCD_Set_Pos(unsigned char  x, unsigned char  y) 
{ 
		OLED_WR_Byte(0xb0+(7-y/8),OLED_CMD);
		OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
		OLED_WR_Byte((x&0x0f),OLED_CMD); 
} 
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t)
{
	unsigned char  pos,bx,temp=0;
		LCD_Set_Pos(x, y);  
		if(x>127||y>64)return;//超出范围了.
		pos=7-y/8;
		bx=y%8;
		temp=1<<(7-bx);
		if(t)OLED_GRAM[x][pos]|=temp;
		else OLED_GRAM[x][pos]&=~temp;	
	  
     //设置显示位置—列高地址   
	  OLED_WR_Byte(OLED_GRAM[x][pos],OLED_DATA); 
	
  //	OLED_WR_Byte(OLED_GRAM[x][pos],OLED_DATA); 
	  

}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot)  
{  
	unsigned char  x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode)
{      			    
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char  hz_sp[],unsigned char  mode)//显示中文
{      			    
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	//unsigned char  mode=1;
	unsigned char info_buf[2];
	unsigned int j,len;
	info_buf[0]	=hz_sp[0];
	info_buf[1] =hz_sp[1];
	//chr=chr-' ';//得到偏移后的值	
	len=sizeof(HZ)/sizeof(HZ[0]);  
 	for(j=0;j<len;j++)
	{
    if(((unsigned char)HZ[j].Index[0]==info_buf[0])&&((unsigned char)HZ[j].Index[1]==info_buf[1]))	
		break;
	}	
	if(j<len)
	{
    for(t=0;t<32;t++)
    {   
	
			temp= HZ[j].Msk[t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
      }   
		}			
}  
//m^n函数
unsigned int oled_pow(unsigned char  m,unsigned char  n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size)
{         	
	unsigned char  t,temp;
	unsigned char  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void OLED_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p)
{
#define MAX_CHAR_POSX 127
#define MAX_CHAR_POSY 63         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   
//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC,D,G端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	 //A0脚接到OLED的RST脚 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //A0脚接到OLED的RST脚 

  							  
//	OLED_RST_Clr();
	delay_ms(100);
//	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0

	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
		 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
	OLED_Clear();
}  


void Choose_MenuOp1(void)
{
  unsigned char flag=0,j=0,nChar,temp_f_CurSel,flagshijian=0,keycode;
  int i=0;
	//GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =HsMenu[0].row;
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待

	delay_ms(500);
	OLED_Clear();	    //清屏
	ShowMenu(HsMenu);
	/*keycode=ReadData(KEY_ENCODE);
	DisplayNumber(68, 70, 0, keycode);
	GUI_DisphzAt(zuming1,32,48);
	GUI_DisphzAt(zuming2,48,48);
	GUI_DisphzAt(zuming3,64,48);
	GUI_DisphzAt(zuming4,80,48);
	//apoint_place_write(32,0,lcd_buf2,0,0x0fff);
	//show_picture1(0,0,31,15,lcd_buf_batt);
 	//show_picture1(96,0,127,15,gImage_signal2);
	    if(Power_Cheak() ==0)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt0);	
					}
			else if(Power_Cheak() ==1)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt3);
					}
			     else if(Power_Cheak() ==2)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt1);	
					}
			          else if(Power_Cheak() ==3)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt2);	
					}
					
 			 if((USB_CHEAK&(GPIO_Pin_12))!=0)  //USB_CHEAK ==(GPIO_Pin_12)
			{
				show_picture1(96,0,127,15,lcd_buf_signal);
      }
			else if((USB_CHEAK&(GPIO_Pin_12))==0)
			{
				show_picture1(96,0,127,15,lcd_buf_nosignal);
      }
			GUI_Clear2(31,0,95,15,White);
			Main_Lcd_Show2();
	    //apoint_place_write(32,0,lcd_buf2,0,0x0fff);
			//显示菜单
	//void InsertKey(unsigned char nChar){ Key_Num =nChar;}	
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键*/
	
  while(1)
  {
		Init_KeyNumuber();			
		nChar = GetKey();
		//apoint_place_write(32,0,lcd_buf2,0,0x0fff);
			if(guanjiflag==1)
		{
			break;
    }
		else{
		/*if(result==1)
		{
			delay_ms(3000);
			Choose_MenuOp3();	
			OLED_Clear();	    //清屏
	    ShowMenu(HsMenu);
			DisplayNumber(64, 70, 0, keycode);
			//apoint_place_write(32,0,lcd_buf2,0,0x0fff);
	    //show_picture1(0,0,31,15,lcd_buf_batt);
 	    //show_picture1(96,0,127,15,gImage_signal2);
			if(Power_Cheak() ==0)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt0);	
					}
			else if(Power_Cheak() ==1)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt3);
					}
			     else if(Power_Cheak() ==2)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt1);	
					}
			          else if(Power_Cheak() ==3)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt2);	
					}
			if((USB_CHEAK&(GPIO_Pin_12))!=0)  //USB_CHEAK ==(GPIO_Pin_12)
			{
				show_picture1(96,0,127,15,lcd_buf_signal);
      }
			else if ((USB_CHEAK&(GPIO_Pin_12))==0)
			{
				show_picture1(96,0,127,15,lcd_buf_nosignal);
      }
			GUI_Clear2(31,0,95,15,White);
			Main_Lcd_Show2();
		}*/
		if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//Delay(100);
			delay_ms(100);
			      /*if(Power_Cheak() ==0)
								{
								show_picture1(0,0,31,15,lcd_buf_batt0);	
								}
						else if(Power_Cheak() ==1)
								{
								show_picture1(0,0,31,15,lcd_buf_batt3);
								}
								 else if(Power_Cheak() ==2)
								{
								show_picture1(0,0,31,15,lcd_buf_batt1);	
								}
											else if(Power_Cheak() ==3)
								{
								show_picture1(0,0,31,15,lcd_buf_batt2);	
								}
						if((USB_CHEAK&(GPIO_Pin_12))!=0)  //USB_CHEAK ==(GPIO_Pin_12)
						{
							show_picture1(96,0,127,15,lcd_buf_signal);
						}
						else if ((USB_CHEAK&(GPIO_Pin_12))==0)
						{
							show_picture1(96,0,127,15,lcd_buf_nosignal);
						}
						if(flagshijian==10)
						{
						GUI_Clear2(31,0,95,15,White);
						Main_Lcd_Show2();
						flagshijian=0;
						}*/
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
	 if(flag==0&&nChar == KEYUP)	//上翻	++
		{
				 i=0;
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel>HsMenu[0].row)		//HsMenu[0].col是最大
						--f_CurSel;
				else if(f_CurSel==HsMenu[0].row) //HsMenu[0].row是最小
						f_CurSel = HsMenu[0].col;		//HsMenu[0].row是最小
				OLED_Clear();
				ShowMenu(HsMenu);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				//DisplayNumber(68, 70, 0, keycode);
				//GUI_DisphzAt(zuming1,32,48);
				//GUI_DisphzAt(zuming2,48,48);
				//GUI_DisphzAt(zuming3,64,48);
				//GUI_DisphzAt(zuming4,80,48);
				/*if(Power_Cheak() ==0)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt0);	
					}
			  else if(Power_Cheak() ==1)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt3);
					}
			     else if(Power_Cheak() ==2)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt1);	
					}
			          else if(Power_Cheak() ==3)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt2);	
					}
			if((USB_CHEAK&(GPIO_Pin_12))!=0)  //USB_CHEAK ==(GPIO_Pin_12)
			{
				show_picture1(96,0,127,15,lcd_buf_signal);
      }
			else if ((USB_CHEAK&(GPIO_Pin_12))==0)
			{
				show_picture1(96,0,127,15,lcd_buf_nosignal);
      }
				GUI_Clear2(31,0,95,15,White);
				Main_Lcd_Show2();
				//apoint_place_write(32,0,lcd_buf2,0,0x0fff);	
				//show_picture1(0,0,31,15,lcd_buf_batt);
        //show_picture1(96,0,127,15,gImage_signal2);*/
				delay_ms(500);
				Init_KeyNumuber();

		}
 		 if(flag==0&&nChar ==KEYDOWN)//下翻	--
 		{
 					i=0;
 					//ResetTimer(TIMER_TIMEOUT);
 					if (f_CurSel<HsMenu[0].col)
 							++f_CurSel;
					else
							f_CurSel =  HsMenu[0].row;
					OLED_Clear();
					ShowMenu(HsMenu);
// 					apoint_place_write(32,0,lcd_buf2,0,0x0fff);	
// 					/*if(Power_Cheak() ==0)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt0,0,0x0fff);	
// 						}
// 					if(Power_Cheak() ==1)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt3,0,0x0fff);	
// 						}
// 					if(Power_Cheak() ==2)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt1,0,0x0fff);	
// 						}
// 						if(Power_Cheak() ==3)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt2,0,0x0fff);	
// 						}
// 			if(USB_Cheak_init()==1)
// 			{
// 				apoint_place_write(96,0,lcd_buf_signal,0,0x0fff);
//       }
// 			else
// 			{
// 				apoint_place_write(96,0,lcd_buf_nosignal,0,0x0fff);
//       }*/
 					Init_KeyNumuber();
					delay_ms(500);
// 					delay_ms(1);
//           GPIOC->IDR= GPIO_Pin_8;

 		}
		if(flag==0&&nChar ==KEYSURE) //确定
		{
					 i=0;
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
					temp_f_CurSel=f_CurSel;
					HsMenu[f_CurSel].pFunc(0);		//f_CurSel表示当前的行
					f_CurSel=temp_f_CurSel;				
					
			    ShowMenu(HsMenu);	
			    //DisplayNumber(68, 70, 0, keycode);
			    //GUI_DisphzAt(zuming1,32,48);
					//GUI_DisphzAt(zuming2,48,48);
					//GUI_DisphzAt(zuming3,64,48);
					//GUI_DisphzAt(zuming4,80,48);
			    //show_picture1(0,0,31,15,lcd_buf_batt);
          //show_picture1(96,0,127,15,gImage_signal2);
			/*if(Power_Cheak() ==0)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt0);	
					}
			else if(Power_Cheak() ==1)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt3);
					}
			     else if(Power_Cheak() ==2)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt1);	
					}
			          else if(Power_Cheak() ==3)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt2);	
					}
			if((USB_CHEAK&(GPIO_Pin_12))!=0)  //USB_CHEAK ==(GPIO_Pin_12)
			{
				show_picture1(96,0,127,15,lcd_buf_signal);
      }
			else if ((USB_CHEAK&(GPIO_Pin_12))==0)
			{
				show_picture1(96,0,127,15,lcd_buf_nosignal);
      }
			GUI_Clear2(31,0,95,15,White);
			Main_Lcd_Show2();*/
	     //apoint_place_write(32,0,lcd_buf2,0,0x0fff);
			    delay_ms(500);
			    Init_KeyNumuber();
			 //  break;

		} 
		if( flag==0&&nChar ==KEYCLEAR)//取消
		{
			    i=0;
			    j++;
					//KeyNum =KEYNO;
			    delay_ms(100);
		   

		}
		      if(j>=30)//按下超过3秒钟
		      {
		  	    j=0;
					  temp_f_CurSel=f_CurSel;
					  //Choose_MenuOp4_3();
						//if(f_CurSel==3)	
						//break;
						f_CurSel=temp_f_CurSel;				
						ShowMenu(HsMenu);	
						//DisplayNumber(68, 70, 0, keycode);
						//GUI_DisphzAt(zuming1,32,48);
						//GUI_DisphzAt(zuming2,48,48);
						//GUI_DisphzAt(zuming3,64,48);
						//GUI_DisphzAt(zuming4,80,48);
						/*if(Power_Cheak() ==0)
								{
								show_picture1(0,0,31,15,lcd_buf_batt0);	
								}
						else if(Power_Cheak() ==1)
								{
								show_picture1(0,0,31,15,lcd_buf_batt3);
								}
								 else if(Power_Cheak() ==2)
								{
								show_picture1(0,0,31,15,lcd_buf_batt1);	
								}
											else if(Power_Cheak() ==3)
								{
								show_picture1(0,0,31,15,lcd_buf_batt2);	
								}
						if((USB_CHEAK&(GPIO_Pin_12))!=0)  //USB_CHEAK ==(GPIO_Pin_12)
						{
							show_picture1(96,0,127,15,lcd_buf_signal);
						}
						else if ((USB_CHEAK&(GPIO_Pin_12))==0)
						{
							show_picture1(96,0,127,15,lcd_buf_nosignal);
						}
						GUI_Clear2(31,0,95,15,White);
						Main_Lcd_Show2();*/
		      }
					else if (nChar !=KEYCLEAR&&j>0&&j<20)
					{
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
				  break;
					}   

	}
	++flagshijian;
  }
  g_State =ST_IDLE;
  return;
}

void ShowMenu(const MENU_ITEM *Menu) //主菜单显示
{
    unsigned char idx;
   //Lcd_Show(1,0, "功能选择菜单");
   // WATCHDOG_CLEAR;                   //清除看门狗
    for (idx=1;idx<=Menu[0].col;idx++)
    {
       if (idx==f_CurSel)
           Lcd_ShowN(Menu[idx].row,Menu[idx].col,(unsigned char*)Menu[idx].Name);		//会将选中的行反白显示
       else
           Lcd_Show(Menu[idx].row,Menu[idx].col,(unsigned char*)Menu[idx].Name);		//未选中的行正常显示
    }
		//循环完之后所有菜单项全部显示
}

void  Lcd_ShowN(unsigned char row, unsigned char col, unsigned char *str)
{
   //GUI_SetDrawMode =0;  //反白显示
	 //OLED_WR_Byte(0xA7,OLED_CMD);
   apoint_place_write(col,row,str,0);
	 //GUI_SetDrawMode =1;

}

void Lcd_Show(unsigned char row, unsigned char col, unsigned char *str)
{
   apoint_place_write(col,row,(unsigned char*)str,1);
}
//显示指定位置汉字和字符
void apoint_place_write(unsigned char s_column,unsigned char s_row,unsigned char *wr_sp,unsigned char  mode)
{
    unsigned char tempa,tempb,number=0;
    
    //bs_color_set(w_color,bk_color);
    while((*wr_sp != 0)&&(number < 20))                                         //检测到结束标示退出
    {
        if(*wr_sp < 0x80)                                                       //ASCII字符写操作
        {
            OLED_ShowChar(s_row,s_column,wr_sp[0],16,mode);                                   //过滤掉空格
            wr_sp++;
            s_row += 8;
        }
        else                                                                    //汉字写
        {
            if(*wr_sp == 0xff)                                                  //写特殊字符
            {
                wr_sp++;
                tempa = *wr_sp&0xf0;
                if(tempa == 0xa0)
                {
                    tempb = *wr_sp&0x0f;
                    //signal_write(s_column,s_row,tempb,SIGNAL_SN);
                    s_row += 16;
                }
                else
                {
                    if(tempa == 0xe0)
                    {
                        tempb = *wr_sp&0x0f;
                        //signal_write(s_column,s_row,tempb,CHARGE_SN);
                        s_row += 16;
                    }
                }
                wr_sp++;
            }
            else
            {
                if(*wr_sp >= 0x4e)
                {
                    OLED_ShowCHinese(s_row,s_column,wr_sp,mode);
                    wr_sp += 2;
                    s_row += 16;
                }
                else
                    wr_sp++;                                                                        //过滤掉无效的字符
            }
        }
        number++;
    }
}
//通用写操作
void often_show(unsigned char s_column,unsigned char s_row,const unsigned char show_buf[32])
{
    apoint_place_write(s_column,s_row,(unsigned char*)show_buf,1);
}

void display_shuzi(unsigned char s_column,unsigned char s_row,unsigned char *wr_sp)
{
	
		unsigned char table[8];
		unsigned int i,j;
	
		j=0;
		for(i=0;i<4;i++)
		{

			table[j] = hex2ch(wr_sp[i] >> 4);
			j++;
			table[j] = hex2ch(wr_sp[i] & 0x0f);		
			j++;
		}
		
		apoint_place_write(s_column,s_row,(unsigned char*)table,1);


}
void Main_Lcd_Show(void)
{
  uchar i,keycode;
	OLED_Clear();	
  apoint_place_write(16,48,"钥匙",1);
	
	apoint_place_write(0,48,"工具",1);
	apoint_place_write(96,48,"设置",1);
	//keycode=ReadData(KEY_ENCODE);
	DisplayNumber(32,70,keycode);
	for(i=0;i<8;i++)
	{
	//zuming[i]=ReadData(AUTHOR_START+16+i);
	}
	zuming1[0]=zuming[0];
	zuming1[1]=zuming[1];
	zuming2[0]=zuming[2];
	zuming2[1]=zuming[3];
	zuming3[0]=zuming[4];
	zuming3[1]=zuming[5];
	zuming4[0]=zuming[6];
	zuming4[1]=zuming[7];
	apoint_place_write(32,48,zuming1,1);
	apoint_place_write(48,48,zuming2,1);
	apoint_place_write(64,48,zuming3,1);
	apoint_place_write(80,48,zuming4,1);
	/*if(Power_Cheak() ==0)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt0);	
					}
			else if(Power_Cheak() ==1)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt3);
					}
			     else if(Power_Cheak() ==2)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt1);	
					}
			          else if(Power_Cheak() ==3)
					{
				  show_picture1(0,0,31,15,lcd_buf_batt2);	
					}
					
 			 if((USB_CHEAK&(GPIO_Pin_12))!=0)  //USB_CHEAK ==(GPIO_Pin_12)
			{
				show_picture1(96,0,127,15,lcd_buf_signal);
      }
			else if((USB_CHEAK&(GPIO_Pin_12))==0)
			{
				show_picture1(96,0,127,15,lcd_buf_nosignal);
      }*/

}




void Main_Lcd_Show2(void)
{
	uchar  tempa;
  uchar  lcd_buf[15],lcd_buf1[15];		//lcd_buf存年月日，lcd_buf1存时分秒
  read_time();  //读取实时时间,放到time_buffer中 //2015-4-21 by xiaolong
	lcd_buf[0] = '2';
	lcd_buf[1] = '0';
	tempa = (time_buff[0]>>4)&0x0f;
	lcd_buf[2] = tempa+0x30;		//转化为字符
	tempa = (time_buff[0]&0x0f);
	lcd_buf[3] = tempa + 0x30;
	lcd_buf[4] = 0xc4;
	lcd_buf[5] = 0xea;   //年的内码
	tempa = (time_buff[1]>>4)&0x0f;
	lcd_buf[6] = tempa+0x30;
	tempa = time_buff[1]&0x0f;
	lcd_buf[7] = tempa+0x30;
	lcd_buf[8] = 0xd4;
	lcd_buf[9] = 0xc2;   //月的内码
	tempa = (time_buff[2]>>4)&0x0f;
	lcd_buf[10] = tempa+0x30;
	tempa = time_buff[2]&0x0f;
	lcd_buf[11] = tempa+0x30;
	lcd_buf[12] = 0xc8;
	lcd_buf[13] = 0xd5;   //日的内码
	lcd_buf[14] = 0;   //结束标志

	tempa = (time_buff[3]>>4)&0x0f;
	lcd_buf1[0] = tempa+0x30;
	tempa = time_buff[3]&0x0f;
	lcd_buf1[1] = tempa + 0x30;
	lcd_buf1[2] = 0xCA;
	lcd_buf1[3] = 0xB1;   //时
	tempa = (time_buff[4]>>4)&0x0f;
	lcd_buf1[4] = tempa+0x30;
	tempa = time_buff[4]&0x0f;
	lcd_buf1[5] = tempa+0x30;
	lcd_buf1[6] = 0xb7;
	lcd_buf1[7] = 0xd6;   //分
	tempa = (time_buff[5]>>4)&0x0f;
	lcd_buf1[8] = tempa+0x30;
	tempa = time_buff[5]&0x0f;
	lcd_buf1[9] = tempa+0x30;
	lcd_buf1[10] = 0xc3;
	lcd_buf1[11] = 0xeb;   //秒
	lcd_buf1[12] = 0;   //结束标志
	
	lcd_buf2[0]=lcd_buf1[0];
	lcd_buf2[1]=lcd_buf1[1];
	lcd_buf2[2]=':';
	lcd_buf2[3]=lcd_buf1[4];
	lcd_buf2[4]=lcd_buf1[5];
	lcd_buf2[5]=':';
	lcd_buf2[6]=lcd_buf1[8];
	lcd_buf2[7]=lcd_buf1[9];
	lcd_buf2[8]=0;
	apoint_place_write(32,0,lcd_buf2,1);	
}
void DisplayNumber(unsigned int x0, unsigned int y0, unsigned char s)
{
  unsigned int ge,shi,bai;
	bai=s/100;
	shi=s%100/10;
	ge=s%10;
	
	if(s>=100&&y0<136)
	{
		OLED_ShowNum(x0,y0,bai,1,16);
		OLED_ShowNum(x0+8,y0,shi,1,16);		
		OLED_ShowNum(x0+16,y0,ge,1,16);
	}
	else if(s<100&&s>=10&&y0<144) 
	{
		OLED_ShowNum(x0,y0,shi,1,16);		
		OLED_ShowNum(x0+8,y0,ge,1,16);
	}
	else
	{
				OLED_ShowNum(x0,y0,ge,1,16);
	}
		
}
/* 显示图片或图标  */
void show_picture1(unsigned char start_col ,unsigned char start_row,unsigned char * pic)
{
  
  int t1,t;
	unsigned char  y0=start_col,temp;
		for(t=0;t<64;t++)
    {   
    temp=pic[t];  
 		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(start_row,start_col,1);
			else OLED_DrawPoint(start_row,start_col,0);
			temp<<=1;
			start_col++;
			if((start_col-y0)==16)
			{
				start_col=y0;
				start_row++;
				break;
			}
		}  	 
	 }
 }


/*显示界面的入口*/
void Choose_MenuOp4(void)
{
  uchar flag=0,j=0,nChar,temp_f_CurSel;
	int i=0;
  //GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =TOOLMENU[0].row;
  //g_WaitTimeOut = 0;
  //SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
  // while( g_WaitTimeOut == 0 )  //超时等待
	//void InsertKey(uchar nChar){ Key_Num =nChar;}
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	delay_ms(500);
	OLED_Clear();	 
	ShowMenu(TOOLMENU);
  while(1)
  {
		Init_KeyNumuber();
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else{
			if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//Delay(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
		 if(nChar == KEYUP)	//上翻	++
		{
				i=0;
        j=0;
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel<TOOLMENU[0].col)
						++f_CurSel;
				else if(f_CurSel==TOOLMENU[0].col) 
						f_CurSel = TOOLMENU[0].row;	
				OLED_Clear();
				ShowMenu(TOOLMENU);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				//InsertKey(KEYNO); 
				Init_KeyNumuber();
				delay_ms(500);

		}if(nChar ==KEYDOWN)//下翻	--
		{
					i=0;
          j=0;
					//ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel<TOOLMENU[0].col)
							++f_CurSel;
					else
							f_CurSel =  TOOLMENU[0].row;
					OLED_Clear();
					ShowMenu(TOOLMENU);
					Init_KeyNumuber();
          delay_ms(500);					

		}
		 if(nChar ==KEYSURE) //确定
		{
					 i=0;
           j=0;
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
			    temp_f_CurSel=f_CurSel;
			    TOOLMENU[f_CurSel].pFunc(0);
			    f_CurSel=temp_f_CurSel;
			    //ShowMenu(OPEN);//f_CurSel表示当前的行
					Init_KeyNumuber();
			    ShowMenu(TOOLMENU);	
          delay_ms(500);
		} 
		if( flag==0&&nChar ==KEYCLEAR)//取消
		{
			    i=0;
			    j++;
					//KeyNum =KEYNO;
			    delay_ms(100);
		   

		}
		if(j>=30)//按下超过3秒钟
		      {
		  	    j=0;
					  temp_f_CurSel=f_CurSel;
					  Choose_MenuOp4_3();
						//if(f_CurSel==2)	
						//break;
						f_CurSel=temp_f_CurSel;				
						ShowMenu(TOOLMENU);	
		      }
		else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
		{
			    i=0;
          j=0;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
	}
  }
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}
void Choose_MenuOp4_1(void)
{
  uchar i,k,flag=0,j=0,nChar,temp_f_CurSel;
	  //GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =CODEMENU[0].row;
  //g_WaitTimeOut = 0;
  //SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
  // while( g_WaitTimeOut == 0 )  //超时等待
	//void InsertKey(uchar nChar){ Key_Num =nChar;}
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	OLED_Clear();	 
	ShowMenu(CODEMENU);
  g_WaitTimeOut = 0;
  SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
// 	while(g_WaitTimeOut == 0)
// 	{
// 			nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 			if(nChar ==5)
// 			{
// 					display_shuzi(32,80,Rfid_Buff);
// 					++rfidrow;
// 					delay_ms(1000);		
// 					for(i=0;i<4;i++)
// 					{
// 							for(k =0;k<40;k++)
// 								msDelay(500);
// 							WritEDAta(CAIMA_START+8*rfidrow+4+i,Rfid_Buff[i]);
// 					}				
// 					break;
// 			}

// 	}	
  KillTimer(TIMER_TIMEOUT);
					
	
  while(1)
  {
		Init_KeyNumuber();
		nChar = GetKey();   //读取按键缓冲区
		if(nChar ==KEYINIT)
		{
			Delay(100);
			
		}
		 if(nChar == KEYUP)	//上翻	++
		{
				
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel>CODEMENU[0].row)
						--f_CurSel;
				else if(f_CurSel==CODEMENU[0].row) 
						f_CurSel = CODEMENU[0].col;	
				OLED_Clear();
				ShowMenu(CODEMENU);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				display_shuzi(32,80,Rfid_Buff);
				//InsertKey(KEYNO); 
				Init_KeyNumuber();

		} if(nChar ==KEYDOWN)//下翻	--
		{
					
					//ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel<CODEMENU[0].col)
							++f_CurSel;
					else
							f_CurSel =  CODEMENU[0].row;
					OLED_Clear();
					ShowMenu(CODEMENU);
					Init_KeyNumuber(); 

		}
		if(nChar ==KEYSURE) //确定
		{
					 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
			    if(f_CurSel==3)
					{
						ShowMenu(CODEMENU);
						g_WaitTimeOut = 0;
            SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
// 						while(g_WaitTimeOut == 0)
// 						{
// 								nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 								if(nChar ==5) 
// 								{
// 										display_shuzi(32,80,Rfid_Buff);
// 										++rfidrow;
// 										delay_ms(1000);						
// 										for(i=0;i<4;i++)
// 										{
// 												for(k =0;k<40;k++)
// 													msDelay(500);
// 												WritEDAta(CAIMA_START+8*rfidrow+4+i,Rfid_Buff[i]);
// 										}										
// 										break;
// 								}
// 						}	

          KillTimer(TIMER_TIMEOUT);
          }
					else
				  {
						OLED_Clear();
						g_State = ST_IDLE;
						ResetKey();
						break;
					}
					Init_KeyNumuber();

		}
		 if( nChar ==KEYCLEAR)//取消
		{
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
  g_State =ST_IDLE;
  return;
}

void Choose_MenuOp4_2(void)
{
  uchar nChar, temp_f_CurSel;
  //GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =OPEN[0].row;
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待

	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(OPEN);		//显示菜单
	//void InsertKey(uchar nChar){ Key_Num =nChar;}	
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	
	
// 	while(1)
// 	{
// 			often_show(16,56,"reading RFID");
// 			delay_ms(1000);	
// 			nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 			//m_UB1;
// 			if(nChar ==5) 
// 			{
// 				if(Rfid_Buff[0]==0&&Rfid_Buff[1]==0&&Rfid_Buff[2]==0&&Rfid_Buff[3]==0)
// 					;
// 				else		//至少有一个不为零
// 				{
// 					//UARTnSendChar(1,Rfid_Buff,5);
// 					//apoint_place_write(56,80,Rfid_Buff,Red,White);
// 					display_shuzi(36,80, Rfid_Buff);
// 					delay_ms(1000);						
// 					break;
// 				}
// 			}
// 	}		
// 	
	
	
	//result=check_suoma();
	if(result==1)
	{
		KEY_POWER_ON;
		often_show(36,100,"已授权");			
	}
	else
		often_show(36,100,"未授权");	
	
  while(1)
  {
		
		//apoint_place_write(32,0,lcd_buf2,0,0x0fff);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(nChar ==KEYINIT)
		{
			
				
			//apoint_place_write(96,0,lcd_buf_batt0,0,0x0fff);	
			/*if(Power_Cheak() ==0)
					{
				  apoint_place_write(96,0,lcd_buf_batt0,0,0x0fff);	
					}
			else if(Power_Cheak() ==1)
					{
				  apoint_place_write(96,0,lcd_buf_batt3,0,0x0fff);	
					}
			     else if(Power_Cheak() ==2)
					{
				  apoint_place_write(96,0,lcd_buf_batt1,0,0x0fff);	
					}
			          else if(Power_Cheak() ==3)
					{
				  apoint_place_write(96,0,lcd_buf_batt2,0,0x0fff);	
					}
			if(USB_Cheak_init()==1)  //USB_CHEAK ==(GPIO_Pin_12)
			{
				apoint_place_write(96,0,lcd_buf_signal,0,0x0fff);
      }
			else
			{
				apoint_place_write(96,0,lcd_buf_nosignal,0,0x0fff);
      }*/
			//InsertKey(KEYNO);		//Key_Num =nChar;
			Delay(100);
		}
		else if(nChar == KEYUP)	//上翻	++
		{
				 
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel<OPEN[0].col)		//HsMenu[0].col是最大
						++f_CurSel;
				else if(f_CurSel==OPEN[0].col) //HsMenu[0].row是最小
						f_CurSel = OPEN[0].row;		//HsMenu[0].row是最小
				OLED_Clear();
				ShowMenu(OPEN);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				//apoint_place_write(32,0,lcd_buf2,0,0x0fff);	
				//apoint_place_write(96,0,lcd_buf_batt0,0,0x0fff);	
				/*if(Power_Cheak() ==0)
					{
				  apoint_place_write(96,0,lcd_buf_batt0,0,0x0fff);	
						}
					if(Power_Cheak() ==1)
					{
				  apoint_place_write(96,0,lcd_buf_batt3,0,0x0fff);	
						}
					if(Power_Cheak() ==2)
					{
				  apoint_place_write(96,0,lcd_buf_batt1,0,0x0fff);	
						}
						if(Power_Cheak() ==3)
					{
				  apoint_place_write(96,0,lcd_buf_batt2,0,0x0fff);	
						}
						if(USB_Cheak_init()==1)
			     {
				     apoint_place_write(96,0,lcd_buf_signal,0,0x0fff);
           }
			      else
			     {
				     apoint_place_write(96,0,lcd_buf_nosignal,0,0x0fff);
           }*/
				Init_KeyNumuber();

		}
// 		else if(nChar ==KEYDOWN)//下翻	--
// 		{
// 					
// 					//ResetTimer(TIMER_TIMEOUT);
// 					if (f_CurSel<HsMenu[0].col)
// 							++f_CurSel;
// 					else
// 							f_CurSel =  HsMenu[0].row;
// 					OLED_Clear();
// 					ShowMenu(HsMenu);
// 					apoint_place_write(32,0,lcd_buf2,0,0x0fff);	
// 					/*if(Power_Cheak() ==0)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt0,0,0x0fff);	
// 						}
// 					if(Power_Cheak() ==1)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt3,0,0x0fff);	
// 						}
// 					if(Power_Cheak() ==2)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt1,0,0x0fff);	
// 						}
// 						if(Power_Cheak() ==3)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt2,0,0x0fff);	
// 						}
// 			if(USB_Cheak_init()==1)
// 			{
// 				apoint_place_write(96,0,lcd_buf_signal,0,0x0fff);
//       }
// 			else
// 			{
// 				apoint_place_write(96,0,lcd_buf_nosignal,0,0x0fff);
//       }*/
// 					Init_KeyNumuber();
// 					delay_ms(1);
//           GPIOC->IDR= GPIO_Pin_8;

// 		}
		else if(nChar ==KEYSURE) //确定
		{
					 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
			    temp_f_CurSel=f_CurSel;
					OPEN[f_CurSel].pFunc(0);		//f_CurSel表示当前的行
			    f_CurSel=temp_f_CurSel;
					Init_KeyNumuber();

		}else if( nChar ==KEYCLEAR)//取消
		{
					//KeyNum =KEYNO;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}

void Choose_MenuOp4_22(void)
{
  uchar flag=0,j=0,nChar, temp_f_CurSel=0;
	int i=0;
  //GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =MIMA[0].row;
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待

	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(MIMA);		//显示菜单
	//void InsertKey(uchar nChar){ Key_Num =nChar;}	
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键	
	
  while(1)
  {
		
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else{
			if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//Delay(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
		 if(nChar == KEYUP)	//上翻	++
		{
				i=0;
        j=0;
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				//if (f_CurSel<MIMA[0].col)		//HsMenu[0].col是最大
					//	++f_CurSel;
				//else if(f_CurSel==MIMA[0].col) //HsMenu[0].row是最小
					//	f_CurSel = MIMA[0].row;		//HsMenu[0].row是最小
				//OLED_Clear();
				//ShowMenu(MIMA);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
			    ++MIMA[f_CurSel].Name[0];
			    if(MIMA[f_CurSel].Name[0]>57)
						MIMA[f_CurSel].Name[0]='0';
					code[f_CurSel-5]=MIMA[f_CurSel].Name[0];
				  Init_KeyNumuber();
			    ShowMenu(MIMA);	
          delay_ms(100);
		}
 if(nChar ==KEYDOWN)//下翻	--
 		{
 					i=0;
          j=0;
 					--MIMA[f_CurSel].Name[0];
			    if(MIMA[f_CurSel].Name[0]<48)
						MIMA[f_CurSel].Name[0]='9';
					code[f_CurSel-5]=MIMA[f_CurSel].Name[0];
          Init_KeyNumuber();
					ShowMenu(MIMA);	
          delay_ms(100);
 		}
		 if(nChar ==KEYSURE) //确定
		{
					 i=0;
           j=0;
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					//OLED_Clear();
			    //temp_f_CurSel=f_CurSel;
					//MIMA[f_CurSel].pFunc(0);		//f_CurSel表示当前的行
			    //f_CurSel=temp_f_CurSel;
			    //MIMA[6].Name[temp_f_CurSel]='3';
			    //++temp_f_CurSel;
					//Init_KeyNumuber();
			    //ShowMenu(MIMA);	
			  if (f_CurSel<MIMA[0].col)
				{					//HsMenu[0].col是最大
						++f_CurSel;
					  Init_KeyNumuber();
					  ShowMenu(MIMA);	
					  delay_ms(1000);
				}
				else if(f_CurSel==MIMA[0].col) //HsMenu[0].row是最小
				{   
					  if(strcmp(code,rightcode))
				    {
					OLED_Clear();
					often_show(16,0,"密码错误！");
				  often_show(16,20,"请重新输入");
					strcpy((char*)code,"    ");
					MIMA[5].Name[0]='0';
					MIMA[6].Name[0]='0';
					MIMA[7].Name[0]='0';
					MIMA[8].Name[0]='0';
				  delay_ms(3000);
					OLED_Clear();
				  f_CurSel = MIMA[0].row;		//HsMenu[0].row是最小
				  ShowMenu(MIMA);
					Init_KeyNumuber();
				    }
				    else if(strcmp(code,rightcode)==0)
				    {
					OLED_Clear();
					often_show(32,0,"密码正确！");
					often_show(16,20,"开锁中");
					Delay(100);
					KEY_POWER_ON;		
	        delay_ms(5000);		
          KEY_POWER_OFF;
					OLED_Clear();
					break;
				    }
					}
				}
		 if( flag==0&&nChar ==KEYCLEAR)//取消
		{
			    i=0;
			    j++;
					//KeyNum =KEYNO;
			    delay_ms(100);
		   
		}
		if(j>=30)//按下超过3秒钟
		      {
		  	    j=0;
					  temp_f_CurSel=f_CurSel;
					  Choose_MenuOp4_3();
						//if(f_CurSel==2)	
						//break;
						f_CurSel=temp_f_CurSel;				
						ShowMenu(MIMA);	
		      }
		else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
		{
					i=0;
          j=0;//KeyNum =KEYNO;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;
		}
  }
}
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}
void Choose_MenuOp4_3(void)
{
  uchar flag=0,j=0,nChar,temp_f_CurSel;
	int i=0;
  //GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =GUANJI[0].row;
  //g_WaitTimeOut = 0;
  //SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
  // while( g_WaitTimeOut == 0 )  //超时等待
	//void InsertKey(uchar nChar){ Key_Num =nChar;}
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	delay_ms(100);
	OLED_Clear();	 
	ShowMenu(GUANJI);
  while(1)
  {
		Init_KeyNumuber();
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else{
			if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//Delay(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
		 if(nChar == KEYUP)	//上翻	++
		{
				i=0;
			  j=0;
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel<GUANJI[0].col)
						++f_CurSel;
				else if(f_CurSel==GUANJI[0].col) 
						f_CurSel = GUANJI[0].row;	
				OLED_Clear();
				ShowMenu(GUANJI);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				//InsertKey(KEYNO); 
				Init_KeyNumuber();

		}if(nChar ==KEYDOWN)//下翻	--
		{
					i=0;
			    j=0;
					//ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel<GUANJI[0].col)
							++f_CurSel;
					else
							f_CurSel =  GUANJI[0].row;
					OLED_Clear();
					ShowMenu(GUANJI);
					Init_KeyNumuber(); 

		}
		 if(nChar ==KEYSURE) //确定
		{
					i=0;
			    j=0; 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
			if(f_CurSel==3)
				break;
			else
			    GUANJI[f_CurSel].pFunc(0);
			if(guanjiflag==1)
			break;
			    //ShowMenu(OPEN);
							//f_CurSel表示当前的行
					Init_KeyNumuber();
			//if(guanjiflag==0)
			    //ShowMenu(GUANJI);	
			//else 
				  //ShowMenu(KAIJI);

		}//else if( nChar ==KEYCLEAR)//取消
		//{
			//		OLED_Clear();
			//		g_State = ST_IDLE;
			//		ResetKey();
			//		break;

	//	}
	}
  }
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}
/*显示界面的入口*/
/*void Choose_MenuOp4_4(void)
{
  uchar flag=0,j=0,epi,epr,hangj,jj,k=0,nChar,temp_f_CurSel=0,tempdata[2],tempsuoma[4],recordsuoma[4];
	uchar row,recordrow;
	int i=0;
  //GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =JIANCHA[0].row;
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待

	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(JIANCHA);		//显示菜单
	//void InsertKey(uchar nChar){ Key_Num =nChar;}	
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	row=ReadData(AUTHOR_ROW_L);
	recordrow=ReadData(RECORD_ROW_L);
	for(epi=0,hangj=0;epi<row;epi++,hangj++)
	{
	tempdata[0]=ReadData(AUTHOR_START+2+epi*24);
	tempdata[1]=ReadData(AUTHOR_START+3+epi*24);
	tempsuoma[0]=ReadData(AUTHOR_START+4+epi*24);
  tempsuoma[1]=ReadData(AUTHOR_START+5+epi*24);
	tempsuoma[2]=ReadData(AUTHOR_START+6+epi*24);
	tempsuoma[3]=ReadData(AUTHOR_START+7+epi*24);
		for(epr=0;epr<recordrow;epr++)
		{
			recordsuoma[0]=ReadData(RECORD_START+0+epr*5);
			recordsuoma[1]=ReadData(RECORD_START+1+epr*5);
			recordsuoma[2]=ReadData(RECORD_START+2+epr*5);
			recordsuoma[3]=ReadData(RECORD_START+3+epr*5);
			if(tempsuoma[0]==recordsuoma[0]&&tempsuoma[1]==recordsuoma[1]&&tempsuoma[2]==recordsuoma[2]&&tempsuoma[3]==recordsuoma[3])
			{
				if(ReadData(RECORD_START+4+epr*5)==1)
					often_show(80,32+hangj*16,"已检查",0,0x0fff);
				else
					often_show(80,32+hangj*16,"未检查",0,0x0fff);
      }
			else
				  often_show(80,32+hangj*16,"未检查",0,0x0fff);
		}
	display_shuzi_zijie(16,32+hangj*16, tempdata, Green, Yellow);
	//hangj=hangj-48;
	if(epi>=6)
	{
		//temp_f_CurSel=1;
		k++;                                               //页数
		break;
		
	}
	}
  while(1)
  {
		
		//apoint_place_write(32,0,lcd_buf2,0,0x0fff);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else{
			if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//Delay(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
	  if(nChar == KEYUP)	//上翻	++
		{
			  i=0;
			  j=0;
				if (k!=0)
				{
					OLED_Clear();
				  ShowMenu(JIANCHA);
          for(j=0,epi=0+7*k,hangj=0;epi<row;epi++,hangj++)
	        {
	           tempdata[0]=ReadData(AUTHOR_START+2+(epi+k)*24);
	           tempdata[1]=ReadData(AUTHOR_START+3+(epi+k)*24);
						 tempsuoma[0]=ReadData(AUTHOR_START+4+(epi+k)*24);
             tempsuoma[1]=ReadData(AUTHOR_START+5+(epi+k)*24);
	           tempsuoma[2]=ReadData(AUTHOR_START+6+(epi+k)*24);
	           tempsuoma[3]=ReadData(AUTHOR_START+7+(epi+k)*24);
							for(epr=0;epr<recordrow;epr++)
									{
										recordsuoma[0]=ReadData(RECORD_START+0+epr*5);
										recordsuoma[1]=ReadData(RECORD_START+1+epr*5);
										recordsuoma[2]=ReadData(RECORD_START+2+epr*5);
										recordsuoma[3]=ReadData(RECORD_START+3+epr*5);
										if(tempsuoma[0]==recordsuoma[0]&&tempsuoma[1]==recordsuoma[1]&&tempsuoma[2]==recordsuoma[2]&&tempsuoma[3]==recordsuoma[3])
										{
											if(ReadData(RECORD_START+4+epr*5)==1)
												often_show(80,32+hangj*16,"已检查",0,0x0fff);
											else
												often_show(80,32+hangj*16,"未检查",0,0x0fff);
										}
										else
				                often_show(80,32+hangj*16,"未检查",0,0x0fff);
									}
	           display_shuzi_zijie(16,32+hangj*16, tempdata, Green, Yellow);
	           hangj=hangj-48;
	           if(epi>=6+7*k)
							{
								 k++;
								 jj=1;
								 //temp_f_CurSel=1;
								 Init_KeyNumuber();
								 break;
							}
		       }			 
							 if(epi<6+7*(k-jj))
									{
										//temp_f_CurSel=0;
										k=0;
										nChar = KEYINIT;
										delay_ms(1000);
									}	 
						}
		 if(k==0 && nChar == KEYUP)
		 {
					OLED_Clear();
				  ShowMenu(JIANCHA);
					for(epi=0,hangj=0;epi<row;epi++,hangj++)
						{
						tempdata[0]=ReadData(AUTHOR_START+2+epi*24);
						tempdata[1]=ReadData(AUTHOR_START+3+epi*24);
						tempsuoma[0]=ReadData(AUTHOR_START+4+epi*24);
            tempsuoma[1]=ReadData(AUTHOR_START+5+epi*24);
	          tempsuoma[2]=ReadData(AUTHOR_START+6+epi*24);
	          tempsuoma[3]=ReadData(AUTHOR_START+7+epi*24);
						for(epr=0;epr<recordrow;epr++)
									{
										recordsuoma[0]=ReadData(RECORD_START+0+epr*5);
										recordsuoma[1]=ReadData(RECORD_START+1+epr*5);
										recordsuoma[2]=ReadData(RECORD_START+2+epr*5);
										recordsuoma[3]=ReadData(RECORD_START+3+epr*5);
										if(!strcmp(tempsuoma,recordsuoma))
										{
											if(ReadData(RECORD_START+4+epr*5)==1)
												often_show(80,32+hangj*16,"已检查",0,0x0fff);
											else
												often_show(80,32+hangj*16,"未检查",0,0x0fff);
										}
										else
				                often_show(80,32+hangj*16,"未检查",0,0x0fff);
									}
						display_shuzi_zijie(16,32+hangj*16, tempdata, Green, Yellow);
							if(epi>=6)
							{
								//temp_f_CurSel=1;
								k++;      								//页数
								break;
							}
						}
						Init_KeyNumuber();
			}
		}
// 	 if(nChar ==KEYDOWN)//下翻	--
// 		{
// 					i=0;
//		      j=0;
// 					//ResetTimer(TIMER_TIMEOUT);
// 					if (f_CurSel<HsMenu[0].col)
// 							++f_CurSel;
// 					else
// 							f_CurSel =  HsMenu[0].row;
// 					OLED_Clear();
// 					ShowMenu(HsMenu);
// 					apoint_place_write(32,0,lcd_buf2,0,0x0fff);	
// 					/*if(Power_Cheak() ==0)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt0,0,0x0fff);	
// 						}
// 					if(Power_Cheak() ==1)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt3,0,0x0fff);	
// 						}
// 					if(Power_Cheak() ==2)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt1,0,0x0fff);	
// 						}
// 						if(Power_Cheak() ==3)
// 					{
// 				  apoint_place_write(96,0,lcd_buf_batt2,0,0x0fff);	
// 						}
// 			if(USB_Cheak_init()==1)
// 			{
// 				apoint_place_write(96,0,lcd_buf_signal,0,0x0fff);
//       }
// 			else
// 			{
// 				apoint_place_write(96,0,lcd_buf_nosignal,0,0x0fff);
//       }*/
// 					Init_KeyNumuber();
// 					delay_ms(1);
//           GPIOC->IDR= GPIO_Pin_8;

// 		}
/*		 if(nChar ==KEYSURE) //确定
		{
					i=0;
			    j=0; 
					//KillTimer(TIMER_TIMEOUT);
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
if( flag==0&&nChar ==KEYCLEAR)//取消
		{
			    i=0;
			    j++;
					//KeyNum =KEYNO;
			    delay_ms(100);
		   

		}
		if(j>=30)//按下超过3秒钟
		      {
		  	    j=0;
					  temp_f_CurSel=f_CurSel;
					  Choose_MenuOp4_3();
						//if(f_CurSel==2)	
						//break;
						f_CurSel=temp_f_CurSel;				
						ShowMenu(TOOLMENU);	
		      }
		else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
		{
			    i=0;
			    j=0;
					//KeyNum =KEYNO;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
}
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}*/
/*显示界面的入口*/
void Choose_MenuOp5(void)
{
  uchar flag=0,j=0,nChar,temp_f_CurSel;
	int i=0;
  //GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =SETMENU[0].row;
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待

	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(SETMENU);		//显示菜单
	//void InsertKey(uchar nChar){ Key_Num =nChar;}	
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	
  while(1)
  {
		
		//apoint_place_write(32,0,lcd_buf2,0,0x0fff);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else{
			if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//Delay(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
	 if(nChar == KEYUP)	//上翻	++
		{
				i=0;
        j=0;			
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel<SETMENU[0].col)		//HsMenu[0].col是最大
						++f_CurSel;
				else if(f_CurSel==SETMENU[0].col) //HsMenu[0].row是最小
						f_CurSel = SETMENU[0].row;		//HsMenu[0].row是最小
				OLED_Clear();
				ShowMenu(SETMENU);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				Init_KeyNumuber();

		}
 if(nChar ==KEYDOWN)//下翻	--
 		{
 					i=0;
          j=0;	
 					//ResetTimer(TIMER_TIMEOUT);
 					if (f_CurSel<SETMENU[0].col)
 							++f_CurSel;
 					else
 							f_CurSel =  SETMENU[0].row;
 					OLED_Clear();
 					ShowMenu(SETMENU);
					Init_KeyNumuber();
 		}
		 if(nChar ==KEYSURE) //确定
		{
					i=0;
          j=0;	 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
			    temp_f_CurSel=f_CurSel;
					SETMENU[f_CurSel].pFunc(0);				//f_CurSel表示当前的行
			    f_CurSel=temp_f_CurSel;
					ShowMenu(SETMENU);
					Init_KeyNumuber();

		}
		if( flag==0&&nChar ==KEYCLEAR)//取消
		{
			    i=0;
			    j++;
					//KeyNum =KEYNO;
			    delay_ms(100);
		   

		}
		if(j>=30)//按下超过3秒钟
		      {
		  	    j=0;
					  temp_f_CurSel=f_CurSel;
					  Choose_MenuOp4_3();
						//if(f_CurSel==2)	
						//break;
						f_CurSel=temp_f_CurSel;				
						ShowMenu(SETMENU);	
		      }
		else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
		{
			    i=0;
			    j=0;
					//KeyNum =KEYNO;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
}
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}
void Choose_MenuOp5_1(void)
{
  uchar flag=0,j=0,nChar,temp_f_CurSel;
	int i=0;
  //GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel=TIME[0].row;
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待

	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(TIME);		//显示菜单
	//void InsertKey(uchar nChar){ Key_Num =nChar;}	
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	
  while(1)
  {
		
		//apoint_place_write(32,0,lcd_buf2,0,0x0fff);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else{
			if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//Delay(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
	  if(nChar == KEYUP)	//上翻	++
		{
				i=0;
        j=0;			
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel<TIME[0].col)		//HsMenu[0].col是最大
						++f_CurSel;
				else if(f_CurSel==TIME[0].col) //HsMenu[0].row是最小
						f_CurSel = TIME[0].row;		//HsMenu[0].row是最小
				OLED_Clear();
				ShowMenu(TIME);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				Init_KeyNumuber();

		}
if(nChar ==KEYDOWN)//下翻	--
 		{
 					i=0;
          j=0;
 					//ResetTimer(TIMER_TIMEOUT);
 					if (f_CurSel<TIME[0].col)
 							++f_CurSel;
 					else
 							f_CurSel =  TIME[0].row;
 					OLED_Clear();
 					ShowMenu(TIME);
 					Init_KeyNumuber();
 		}
	  if(nChar ==KEYSURE) //确定
		{
					i=0;
          j=0; 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
			    if(f_CurSel==3)
			    choosetime=1000;
					else if(f_CurSel==4)
					     choosetime=300;
					     else if(f_CurSel==5)
						        choosetime=500;
          g_State = ST_IDLE;
					ResetKey();
					break;

		}
		if( flag==0&&nChar ==KEYCLEAR)//取消
		{
			    i=0;
			    j++;
					//KeyNum =KEYNO;
			    delay_ms(100);
		   

		}
		if(j>=30)//按下超过3秒钟
		      {
		  	    j=0;
					  temp_f_CurSel=f_CurSel;
					  Choose_MenuOp4_3();
						//if(f_CurSel==2)	
						//break;
						f_CurSel=temp_f_CurSel;				
						ShowMenu(TIME);	
		      }
		else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
		{
			    i=0;
          j=0;
					//KeyNum =KEYNO;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
}
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}
void Choose_MenuOp5_3(void)
{
  uchar flag=0,j=0,nChar,temp_f_CurSel;
	int i=0;
  //GUI_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =BANBEN[0].row;
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待

	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(BANBEN);		//显示菜单
	//void InsertKey(uchar nChar){ Key_Num =nChar;}	
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	
  while(1)
  {
		
		//apoint_place_write(32,0,lcd_buf2,0,0x0fff);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else{
			if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//Delay(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
		if(nChar == KEYUP)	//上翻	++
		{
				i=0;
        j=0;			
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel<BANBEN[0].col)		//HsMenu[0].col是最大
						++f_CurSel;
				else if(f_CurSel==BANBEN[0].col) //HsMenu[0].row是最小
						f_CurSel = BANBEN[0].row;		//HsMenu[0].row是最小
				OLED_Clear();
				ShowMenu(BANBEN);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				Init_KeyNumuber();

		}
    if(nChar ==KEYDOWN)//下翻	--
 		{
 					i=0;
          j=0;
 					//ResetTimer(TIMER_TIMEOUT);
 					if (f_CurSel<BANBEN[0].col)
 							++f_CurSel;
					else
 							f_CurSel =  BANBEN[0].row;
 					OLED_Clear();
 					ShowMenu(BANBEN);
					Init_KeyNumuber();
 		}
		 if(nChar ==KEYSURE) //确定
		{
					i=0;
          j=0; 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
          g_State = ST_IDLE;
					ResetKey();
					break;

		}
		if( flag==0&&nChar ==KEYCLEAR)//取消
		{
			    i=0;
			    j++;
					//KeyNum =KEYNO;
			    delay_ms(100);
		   

		}
		if(j>=30)//按下超过3秒钟
		      {
		  	    j=0;
					  temp_f_CurSel=f_CurSel;
					  Choose_MenuOp4_3();
						//if(f_CurSel==2)	
						//break;
						f_CurSel=temp_f_CurSel;				
						ShowMenu(BANBEN);	
		      }
		else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消 
		{
					//KeyNum =KEYNO;
			    i=0;
			    j=0;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
}
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}

void SPI2_Init(void)           //硬件SPI2口初始化,定义了4个引脚CS SCK MISO MOSI
{
  SPI_InitTypeDef  SPI_InitStructure;
  
	GPIO_InitTypeDef GPIO_InitStructure;
   
  /* Enable SPI2 and GPIOB clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE) ;//使能SPI2的时钟
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能IO口时钟
  
  /* Configure SPI2 pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure PB.9 as Output push-pull, used as Flash Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOB, GPIO_Pin_9);//片选信号为低
  
  /* SPI1 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 //设置为主SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//SPI发送接收8位帧结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //时钟悬空高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据捕获于第二个时钟沿
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	//内部NSS信号有SSI位控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频值为2
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7; //SPI_CRCPolynomial定义了用于CRC值计算的多项式
  SPI_Init(SPI2, &SPI_InitStructure);
  
  /* Enable SPI2  */
  SPI_Cmd(SPI2, ENABLE);
    
}
















