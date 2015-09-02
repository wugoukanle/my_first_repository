//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;

//********************************************************************************


#ifndef __OLED_H
#define __OLED_H			  	 
//#include "sys.h"
//#include "stdlib.h"	 

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "general.h"
#include "ASC.h"
#include "sw-key.h"
#include "Key_Transfer.h"

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	 
		    						  
//-----------------OLED�˿ڶ���----------------  					   

#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_9)//CS  PB9
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_4)//RES  PB4
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//DC  PB12
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)



//ʹ��4�ߴ��нӿ�ʱʹ�� 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//CLK  PB13
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//DIN  PB15
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)


 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����




typedef  void (*HsMenuFun)(unsigned char para);

#define MAX_MENU_PERPAGE 4
#define MAX_MENUNAME    39
typedef struct tgMenuItem {
    unsigned char row;                  //��ʾ��ʼλ��
    unsigned char col;
    unsigned char Name[MAX_MENUNAME];   //�˵�����
    HsMenuFun pFunc;            //�˵�����
} MENU_ITEM;





//OLED�����ú���
void OLED_WR_Byte(unsigned char  dat,unsigned char  cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);

void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t);
void OLED_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot);
void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char  hz_sp[],unsigned char  mode);
void OLED_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode);
void OLED_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size);
void OLED_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p);	 

void LCD_Set_Pos(unsigned char  x, unsigned char  y) ;





void apoint_place_write(unsigned char s_column,unsigned char s_row,unsigned char *wr_sp,unsigned char  mode);
void often_show(unsigned char s_column,unsigned char s_row,const unsigned char show_buf[32]);
void display_shuzi(unsigned char s_column,unsigned char s_row,unsigned char *wr_sp);
void  Lcd_ShowN(unsigned char row, unsigned char col, unsigned char *str);
void  Lcd_Show(unsigned char row, unsigned char col, unsigned char *str);
void ShowMenu(const MENU_ITEM *Menu);
void Main_Lcd_Show(void);
void Main_Lcd_Show2(void);
void OLED_Clear2(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2);
unsigned char SPI2_ReadWriteByte(unsigned char TxData);
void show_picture1(unsigned char start_col ,unsigned char start_row,unsigned char * pic);
void DisplayNumber(unsigned int x0, unsigned int y0, unsigned char s);
void tool_menu(void);
void hs_menu(void);
void set_menu(void);
void open(void);
void close(void);
void code_menu(void);
void guanji(void);
void mima(void);
void guanping(void);
void time(void);
void Choose_MenuOp1(void);
void Choose_MenuOp3(void);
void Choose_MenuOp3_1(void);
void Choose_MenuOp3_21(void);
void Choose_MenuOp3_22(void);
void Choose_MenuOp4(void);
void Choose_MenuOp4_1(void);
void Choose_MenuOp4_2(void);
void Choose_MenuOp4_22(void);
void Choose_MenuOp4_3(void);
void Choose_MenuOp5(void);   
void Choose_MenuOp5_1(void);
void Choose_MenuOp5_3(void);
void banben(void);
void SPI2_Init(void); 
#endif  
	 



