#ifndef __GENERAL_H
#define __GENERAL_H


#include "stdint.h"

#define  ST_IDLE          0XFF
#define  ST_RTC           0XFE
#define  ST_USB	          0XFD
#define  ST_KEY	          0XFC
#define  ST_DEBUG         0xFB
#define  ST_LCDMENU       0XFA
#define  uchar            unsigned char
#define  uint             unsigned int
#define  ulong            unsigned long
#define  TIMER_TIMEOUT	  0x01


#define u8		uint8_t
#define u16		uint16_t
#define u32		uint32_t




// typedef unsigned short u16;
// typedef unsigned char  u8;

// typedef enum {FALSE = 0, TRUE = !FALSE} bool;

/*
*
 
 EEPROM内部的数据存储区的分布
 存储芯片采用 AT25512
 512Kb=64KB=2^16KB=0xFFFF
 地址范围 0X0000--0XFFFF
*
*/

#define   LOCAL_NUM	       0X01	 /*01 02 03 04*区位码*/
#define   KEY_CODE		   0X05	 /*05*/
#define   KEY_NUM		   0X06	 /* 06 07 08*/
#define   KEY_NAME_MIN     0x09    /*09 0a 0b 0c*/
#define   KEY_NAME_MAX	   0x0d    /*0D 0E 0F 10*/

#define   KEY_NAME_START   0x11    /************/
#define   KEY_NAME_END	   0x7E1   /***********/

//#define   KEY_RECORD_START 0X10d7   /***********锁操作信息的存储记录地址*/
//#define   KEY_RECORD_END   0x5e9d  /***********/
/**
#define   KEY_RECORD_ROW_H  0X10d3
#define   KEY_RECORD_ROW_L	0X10d4
#define   KEY_RECORD_INDEX_H  0X10d5
#define   KEY_RECORD_INDEX_L  0X10d6
***/
#define   KEY_NAME_ZERO    0X107a
#define   KEY_NAME_FIRST   0X107e  /* 0 1 2 3*/ 
#define   KEY_NAME_SECOND  0X1082  /* 4 5 6 7*/
#define   KEY_NAME_THIRD   0x1086  /* 8 9 A B*/
#define   KEY_NAME_FORTH   0x108a  /* C D E F*/
#define   KEY_NAME_FIVE	   0x108e  /* 0 1 2 3*/
#define   KEY_NAME_SIX	   0x1092	   /* 4 5 6 7*/
#define   KEY_NAME_SEVEN   0x1096	   /* 8 9 A B*/
#define   KEY_NAME_EIGHT   0x109a	   /* C D E F*/
#define   KEY_NAME_NINA	   0x109e	   /* 0 1 2 3*/
#define   KEY_NAME_TEN	   0X10a2	   /* 4 5 6 7*/
#define   KEY_NAME_ZERO_TOW    0X10a6
#define   KEY_NAME_FIRST_TOW   0X10aa  /* 0 1 2 3*/ 
#define   KEY_NAME_SECOND_TOW  0X10ae  /* 4 5 6 7*/
#define   KEY_NAME_THIRD_TOW   0x10b2  /* 8 9 A B*/
#define   KEY_NAME_FORTH_TOW   0x10b6  /* C D E F*/
#define   KEY_NAME_FIVE_TOW	   0x10ba  /* 0 1 2 3*/
#define   KEY_NAME_SIX_TOW	   0x10be	   /* 4 5 6 7*/
#define   KEY_NAME_SEVEN_TOW   0x10c2	   /* 8 9 A B*/
#define   KEY_NAME_EIGHT_TOW   0x10c6	   /* C D E F*/
#define   KEY_NAME_NINA_TOW	   0x10ca	   /* 0 1 2 3*/
#define   KEY_NAME_TEN_TOW	   0X10ce	   /* 4 5 6 7*/

#define   KEY_NAME_INDEX_ZERO	     0X11	   /*锁的编号的存储地址，以50X4=200户为一组的格式*/
#define   KEY_NAME_INDEX_FIRST	     0XD9
#define   KEY_NAME_INDEX_SECOND	     0X1A1
#define   KEY_NAME_INDEX_THIRD	     0X269
#define   KEY_NAME_INDEX_FORTH	     0X331
#define   KEY_NAME_INDEX_FIVE	     0X3F9
#define   KEY_NAME_INDEX_SIX	     0X4C1
#define   KEY_NAME_INDEX_SEVEN	     0X589
#define   KEY_NAME_INDEX_EIGHT	     0X651
#define   KEY_NAME_INDEX_NINA	     0X719
#define   KEY_NAME_INDEX_TEN	     0X7E1
#define   KEY_NAME_INDEX_ZERO_TOW	 0X8a9	   /*锁的编号的存储地址，以50户为一组的格式*/
#define   KEY_NAME_INDEX_FIRST_TOW	 0X971
#define   KEY_NAME_INDEX_SECOND_TOW	 0Xa39
#define   KEY_NAME_INDEX_THIRD_TOW	 0Xb01
#define   KEY_NAME_INDEX_FORTH_TOW	 0Xbc9
#define   KEY_NAME_INDEX_FIVE_TOW	 0Xc91
#define   KEY_NAME_INDEX_SIX_TOW	 0Xd59
#define   KEY_NAME_INDEX_SEVEN_TOW	 0Xe21
#define   KEY_NAME_INDEX_EIGHT_TOW	 0Xee9
#define   KEY_NAME_INDEX_NINA_TOW	 0Xfb1
#define   KEY_NAME_INDEX_TEN_TOW	 0X1079

#define   KEY_YONGHU_START	     0x11
#define   KEY_YONGHU_END		 0x1781	    //用户存储信息为最大100用户

#define   KEY_RECORD_START		 0x1782		//操作记录的最大存储值为2000条
#define	  KEY_RECORD_END	     0x65a2

#define   SANBIAO_START			 0x65a3
#define   SANBIAO_END			 0x66fd

#define   KEY_RECORD_ROW_H               0X66fe
#define   KEY_RECORD_ROW_L	             0X66ff
#define   KEY_RECORD_INDEX_H             0X6700
#define   KEY_RECORD_INDEX_L             0X6701

#define   KEY_YONGHU_INDEX_H             0X6702
#define   KEY_YONGHU_INDEX_L		     		 0X6703
#define   HEAD_BIT                       0x6705     //头指针初始化标志位
/***
 存储注销掉的锁的编号
 最大可以注销50个锁的编号
 2X50=100个字节
****/
#define   DETELE_ROW		             0x6706
#define   DETELE_START			         0x6707
#define   DETELE_END			           0x676b
#define   DETELE_INDEX_H	           0x676c
#define   DETELE_INDEX_L		         0x676d

//#define   KEY_YONGHU_ROW_H	         0X676E
//#define   KEY_YONGHU_ROW_L	         0X676F

#define   LOCK_LOCAL			           0x6770  //70 71 72   锁的区号
#define   LOCK_TYPE			             0x6773   //          锁的类型

#define   KEY_YONGHU_ROW_H	         0X677a
#define   KEY_YONGHU_ROW_L	         0X677b
/***
存储操作员的相关信息
一个汉字的字模为32个字节，预留16个汉字内码的空间
2 *16 =32 +1 =33;
***/
#define   Operate_name_Len          0x7000
#define   Operate_name_Start        0x7001
#define   Operate_name_End          0x7021
/***
存储单位信息的地址
一个汉字的字模为32个字节，预留16个汉字字模值的空间
32 *16 =512+1 =513；
***/
#define   Operate_danwei_Len          0x7022
#define   Operate_danwei_Start        0x7023
#define   Operate_danwei_End          0x7223

#define   HANZI_FIRST_START           Operate_name_Start
#define   HANZI_SECOND_START          Operate_danwei_Start

#define   TASK_BIT                    0x7225    //任务标志位，当为 '1' 的时候表示有一个任务未完成，当为 '0' 的时候标志任务已结束
/*****					   
 存储10个中间数据的分布点，减少索引的压力

******/

#define  GREEN_ON          (GPIOA->BSRR = GPIO_Pin_4)
#define  GREEN_OFF           (GPIOA->BRR  = GPIO_Pin_4) 

#define  RED_ON          (GPIOA->BSRR = GPIO_Pin_3)
#define  RED_OFF           (GPIOA->BRR  = GPIO_Pin_3) 

#define  SPX2941_HIGH          (GPIOA->BSRR = GPIO_Pin_12)
#define  SPX2941_LOW           (GPIOA->BRR  = GPIO_Pin_12) 

#define  MOS_ON	         (GPIOA->BSRR = GPIO_Pin_11)		//M_POWER即PA11，MAX_VDD,MAX3232
#define  MOS_OFF		 (GPIOA->BRR  = GPIO_Pin_11)

//#define  SPEAKER_ON          (GPIOA->BSRR = GPIO_Pin_11)
//#define  SPEAKER_OFF           (GPIOA->BRR  = GPIO_Pin_11) 


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



/*

新建存储空间分配
4KB=4096B
地址0x0000~0x0fff

存100组授权，每组12字节，共1200字节
*/


#define KEY_ENCODE	0x00		//钥匙编码，0x00~0x01(0x0001)（地址放低 高地址放高）,暂时只用0x00一个字节

#define INIT_FLAG   0x03	//授权指针的初始化


#define AUTHOR_ROW_H    0x05  //授权用户信息条数,最大25条
#define AUTHOR_ROW_L    0x06  

#define AUTHOR_POINT_H  0x07  //当前授权用户信息的指针
#define AUTHOR_POINT_L  0x08


#define CAIMA_ROW_H			0x09	//采码的条数最大30条
#define CAIMA_ROW_L			0x0a

#define CAIMA_POINT_H		0x0b
#define CAIMA_POINT_L		0x0c


#define RECORD_ROW_H		0x0d  //操作记录最大100条
#define RECORD_ROW_L		0x0e

#define RECORD_POINT_H  0x0f
#define RECORD_POINT_L  0x10


#define XUNJIAN_ROW_H		0x11  //大巡检任务最大10个
#define XUNJIAN_ROW_L		0x12

#define XUNJIAN_POINT_H 0x13
#define XUNJIAN_POINT_L 0x14 


#define CAIMA_SUO_POINT_H 0x15	//采码时锁码的存储指针
#define CAIMA_SUO_POINT_L 0x16


#define AUTHOR_START    0x400   //授权文件,25*24B=600B
#define AUTHOR_END      0x658

#define CAIMA_START			0x659   //采码,30*8B=240B
#define CAIMA_END				0x749

#define RECORD_START		0x800		//操作记录,100*10B=1000B
#define RECORD_END			0xbe8


#define XUNJIAN_START		0xc00		//巡检任务和结果
#define XUNJIAN_END			0xfff	


#endif
