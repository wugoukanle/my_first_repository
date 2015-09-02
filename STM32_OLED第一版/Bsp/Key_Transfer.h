#ifndef __KEY_TRANSFER_H
#define __KEY_TRANSFER_H

#include "general.h"

#define  TXD_S_HIGH          (GPIOB->BSRR = GPIO_Pin_13)		//PB13置位
#define  TXD_S_LOW           (GPIOB->BRR  = GPIO_Pin_13) 		//PB13复位
#define  RXD_S_DATA          (GPIOB->IDR  & GPIO_Pin_13)		//PB13的输入数据寄存器
#define  KEY_CHEAK          (GPIOB->IDR  & GPIO_Pin_13)			

//spx2941控制脚
#define  KEY_POWER_ON	    	(GPIOA->BSRR = GPIO_Pin_8)   //PA8即SP-EN，置位按钮
#define  KEY_POWER_OFF		  (GPIOA->BRR  = GPIO_Pin_8)  // (实际的原理图上为TPS79633)


#define  SPKEAR_ON	    (GPIOB->BSRR = GPIO_Pin_12)
#define  SPKEAR_OFF		 (GPIOB->BRR  = GPIO_Pin_12)


 void Key_IO_Init_In(void);
 void Key_IO_Init_Out(void);
 uchar Read_KeyInfo(void);
 void WByte(uchar input);
 void Key_Sendata(uchar dir[],uchar len);
 uchar RByte(void);
 uchar RByte1(void);
 uchar Re_leveldata(uchar data_bit);
 void Read_LevelData(void);
 void Key_Power_Init(void);
 void Key_OE_Init(void);
 
 void Read_LeadCode(void);
 
 void Order_Transmit(void);
//5ms
 #define  FIVE_MS  0x0580 
 //2.5ms 
 #define  HALF_MS  0x02bf
 //7.5ms
 #define  OVER_MS  0x0850
 //6ms
 #define  SIX_MS   0X695
 //4ms 
 #define  FOUR_MS  0X467
 //10ms
 #define  TEN_MS   0X0AF0

 #define  ONE_MS    0x118
 #define  TOW_MS    0X22a
 #define  THREE_MS	0x345


 //test
 // #define  FIVE_MS  0x0550 

 uchar Cheak_lock_Num(void);
 uchar compare_Lock_num(uint i2caddr ,uchar str[]);
 uchar Float_subtract(uchar str1[],uchar str2[],uchar term);
 void OperateKey(void);  
 void  Save_Lock_doing(void);
 uchar  Read_FirstCode(void);
 
 void Key_Trismit(void);
 
 uchar Test_RByte(void);
 void Test_Head(void);
 void Init_KeyRecord(void);
 void  Test_record(void);
 uchar Read_New_One(void);
 uchar Read_New_Code(void);
 void Second_WByte(uchar input);
 uchar Up_Midle(void);
 uchar Down_Midle(void);
 uchar Up_FiveMidle(void);
 uchar Down_FiveMidle(void);
 uchar Up_six_tenMidle(void);
 uchar Down_six_tenMidle(void);
 void  Write_order(void);
 void  Write_code(void);
 void  Write_LockNum(void);
 void Cheak_I2cHead(void);
uchar Cheak_New_LockNum(void);
uchar  XunHuan(void);
void Test_SaveSuo(void);
uchar Cheak_Lock_Local(void);

void mykey_init(void);

uchar check_suoma(void);



#endif
