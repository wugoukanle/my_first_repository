#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x_gpio.h"
#include "general.h"
#include "ASC.h"
#include "sw-key.h"
#include "Key_Transfer.h"


//typedef  void (*HsMenuFun)(void);
typedef  void (*HsMenuFun)(uchar para);

#define MAX_MENU_PERPAGE 4
#define MAX_MENUNAME    39
typedef struct tgMenuItem {
    uchar row;                  //显示开始位置
    uchar col;
    uchar Name[MAX_MENUNAME];   //菜单名称
    HsMenuFun pFunc;            //菜单功能
} MENU_ITEM;

// typedef struct nwMenuItem{
// 		MENU_ITEM mi;
// 		uchar start;
// 		uchar end;
// }NEW_MENU_ITEM;

 
 #define    LCD_CS_EN           (GPIOB->BRR  = GPIO_Pin_6)
 #define    LCD_CS_DIS          (GPIOB->BSRR = GPIO_Pin_6)

 #define    LCD_RST_EN          (GPIOB->BRR  = GPIO_Pin_5)
 #define    LCD_RST_DIS         (GPIOB->BSRR = GPIO_Pin_5)

 #define    LCD_LIGHT_ON	    	(GPIOB->BSRR  = GPIO_Pin_13)
 #define    LCD_LIGHT_OFF				(GPIOB->BRR = GPIO_Pin_13)



#define Set_Rst()  GPIOB->BSRR  = GPIO_Pin_5;
#define Clr_Rst()  GPIOB->BRR   = GPIO_Pin_5;

#define Set_Cs()  GPIOB->BSRR  = GPIO_Pin_6;
#define Clr_Cs()  GPIOB->BRR   = GPIO_Pin_6;

#define Set_Rs()  GPIOB->BSRR  = GPIO_Pin_9;
#define Clr_Rs()  GPIOB->BRR   = GPIO_Pin_9;

#define Set_Wr()  GPIOB->BSRR  = GPIO_Pin_7;
#define Clr_Wr()  GPIOB->BRR   = GPIO_Pin_7;

#define Set_Rd()  GPIOB->BSRR  = GPIO_Pin_8;
#define Clr_Rd()  GPIOB->BRR   = GPIO_Pin_8; 

#define  KEY_POWER_ON	    	(GPIOA->BSRR = GPIO_Pin_8)   //PA8即SP-EN，置位按钮
#define  KEY_POWER_OFF		  (GPIOA->BRR  = GPIO_Pin_8)  // (实际的原理图上为TPS79633)



void tool_menu(void);
void set_menu(void);
void set_task(void);
void open(void);
void close(void);
void code_menu(void);
void ifauthorize(void);
void drawrectangle(void);
void novacancy (void);
void vacancy (void);
void guanji(void);
void mima(void);
void guanping(void);
void time(void);
void ShowMenu(const MENU_ITEM *Menu);
void Choose_MenuOp1(void);
//void Choose_MenuOp2(void);
void Choose_MenuOp3(void);
void Choose_MenuOp3_1(void);
void Choose_MenuOp3_21(void);
void Choose_MenuOp3_22(void);
void Choose_MenuOp4(void);
void Choose_MenuOp4_1(void);
void Choose_MenuOp4_2(void);
void Choose_MenuOp4_22(void);
void Choose_MenuOp4_3(void);
void Choose_MenuOp4_4(void);
void Choose_MenuOp5(void);   
void Choose_MenuOp5_1(void);
void Choose_MenuOp5_3(void);
void banben(void);
void jiancha(void);


void DisplayNumber(unsigned int x0, unsigned int y0, unsigned int color, unsigned char s);  
void DisplayHNumber(unsigned int x0, unsigned int y0, unsigned int color, unsigned char s);
void writePixel(uchar x_start,uchar y_start,uint color);
//void ReadID(); 
//void write_MTP();
void adress_set(uchar x_start,uchar y_start,uchar x_end,uchar y_end); 
void print_hex(uint x0, uint y0,uchar ForeColor, uchar hex); 
//void lcd_reset();
static char hex2ch(char hex);
//void display_gray(); 
void lcd_init(void);
void show_pure_image(unsigned char DataH,unsigned char DataL);
void GUI_Clear(uint color);
void show_H_bar(void);
void show_V_bar(void);
void show_frame(void);
void show_cs_open(void);
void GUI_DispCharAt_Test(uchar ASCii, uint x, uint y);



void GUI_GetPos(uint font);
void GUI_DispChar(uchar ASCii);
void GUI_DispCharAt(uchar char_sp[], uint x, uint y);
void Set_ramaddr(uchar x,uchar y);
void GUI_DispString(uchar str[],uchar len);
void GUI_Disphz(uchar *hz);
void GUI_DisphzAt(uchar *hz, uint x, uint y);
void DispSmallPic(uint x, uint y, uint w, uint h, const uchar *str);
void Test_image(void);
void often_show(uchar s_column,uchar s_row,const uchar show_buf[32],uint wr_color,uint blank_color);
void  Lcd_PortInit(void);
void Test_Plcd(void);

void this_Info(uchar para);
void User_Info(uchar para);
void ShowOneOperation(uchar temp[]);
void OPeration_Info(uchar para); 
void Save_Info(uchar para);
void ShowHsMenu(void);
void  Lcd_ShowN(uchar row, uchar col, uchar *str);
void Lcd_Show(uchar row, uchar col, uchar *str); 
void Choose_MenuOp(void);
void apoint_place_write(uchar s_column,uchar s_row,uchar *wr_sp,uint w_color,uint bk_color);
void Lcd_Clear(uchar para);
void Main_Lcd_Show(void);
void Lcd_Show_IDinfo(void);
void Lcd_Show_LowBattry(void);
void Lcd_Init3(void);
void lcd_reset(void);
void Show_Operater(uchar s_column,uchar s_row,uchar op_Buff[],uchar len,uint wr_color,uint blank_color);
void  Test_ShowOp(void);


void show_picture1(uint8_t start_row,uint8_t end_row,uint8_t start_col,uint8_t end_col,uint8_t * pic);
void display_shuzi_zijie(uchar s_column,uchar s_row,uchar *wr_sp,uint w_color,uint bk_color);

void GUI_Clear2(uint col_s,uint row_s,uint col_e,uint row_e,uint color);
void Main_Lcd_Show2(void);

/*

sbit RS=P3^0   ;					   
sbit SCL=P1^0  ;	
sbit SDA=P1^1  ;
sbit CS=P3^1   ;	    
sbit RES=P3^2 ;
*/


#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0








 //LCD相关操作
 
















//将数据输出到LCD的数据端口
//#define OutPut_Data(LCD_DATA) GPIO_Write(GPIOC,((GPIOC->ODR & 0xff00) | LCD_DATA ))






void LCD_GPIO_Init(void);
void write_data(int8_t i,int8_t j);
void write_data8(int16_t b);
void write_cmd(int16_t m);
void Delayms(uint32_t t);
void Initial_ILI9163(void);
void display_black(void);
void display_white(void);
void display_red(void);
void display_green(void);
void display_blue(void);
void LCD_Init(void);


void display_shuzi(uchar s_column,uchar s_row,uchar *wr_sp,uint w_color,uint  bk_color);

#endif




