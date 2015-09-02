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
 
 EEPROM�ڲ������ݴ洢���ķֲ�
 �洢оƬ���� AT25512
 512Kb=64KB=2^16KB=0xFFFF
 ��ַ��Χ 0X0000--0XFFFF
*
*/

#define   LOCAL_NUM	       0X01	 /*01 02 03 04*��λ��*/
#define   KEY_CODE		   0X05	 /*05*/
#define   KEY_NUM		   0X06	 /* 06 07 08*/
#define   KEY_NAME_MIN     0x09    /*09 0a 0b 0c*/
#define   KEY_NAME_MAX	   0x0d    /*0D 0E 0F 10*/

#define   KEY_NAME_START   0x11    /************/
#define   KEY_NAME_END	   0x7E1   /***********/

//#define   KEY_RECORD_START 0X10d7   /***********��������Ϣ�Ĵ洢��¼��ַ*/
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

#define   KEY_NAME_INDEX_ZERO	     0X11	   /*���ı�ŵĴ洢��ַ����50X4=200��Ϊһ��ĸ�ʽ*/
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
#define   KEY_NAME_INDEX_ZERO_TOW	 0X8a9	   /*���ı�ŵĴ洢��ַ����50��Ϊһ��ĸ�ʽ*/
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
#define   KEY_YONGHU_END		 0x1781	    //�û��洢��ϢΪ���100�û�

#define   KEY_RECORD_START		 0x1782		//������¼�����洢ֵΪ2000��
#define	  KEY_RECORD_END	     0x65a2

#define   SANBIAO_START			 0x65a3
#define   SANBIAO_END			 0x66fd

#define   KEY_RECORD_ROW_H               0X66fe
#define   KEY_RECORD_ROW_L	             0X66ff
#define   KEY_RECORD_INDEX_H             0X6700
#define   KEY_RECORD_INDEX_L             0X6701

#define   KEY_YONGHU_INDEX_H             0X6702
#define   KEY_YONGHU_INDEX_L		     		 0X6703
#define   HEAD_BIT                       0x6705     //ͷָ���ʼ����־λ
/***
 �洢ע���������ı��
 ������ע��50�����ı��
 2X50=100���ֽ�
****/
#define   DETELE_ROW		             0x6706
#define   DETELE_START			         0x6707
#define   DETELE_END			           0x676b
#define   DETELE_INDEX_H	           0x676c
#define   DETELE_INDEX_L		         0x676d

//#define   KEY_YONGHU_ROW_H	         0X676E
//#define   KEY_YONGHU_ROW_L	         0X676F

#define   LOCK_LOCAL			           0x6770  //70 71 72   ��������
#define   LOCK_TYPE			             0x6773   //          ��������

#define   KEY_YONGHU_ROW_H	         0X677a
#define   KEY_YONGHU_ROW_L	         0X677b
/***
�洢����Ա�������Ϣ
һ�����ֵ���ģΪ32���ֽڣ�Ԥ��16����������Ŀռ�
2 *16 =32 +1 =33;
***/
#define   Operate_name_Len          0x7000
#define   Operate_name_Start        0x7001
#define   Operate_name_End          0x7021
/***
�洢��λ��Ϣ�ĵ�ַ
һ�����ֵ���ģΪ32���ֽڣ�Ԥ��16��������ģֵ�Ŀռ�
32 *16 =512+1 =513��
***/
#define   Operate_danwei_Len          0x7022
#define   Operate_danwei_Start        0x7023
#define   Operate_danwei_End          0x7223

#define   HANZI_FIRST_START           Operate_name_Start
#define   HANZI_SECOND_START          Operate_danwei_Start

#define   TASK_BIT                    0x7225    //�����־λ����Ϊ '1' ��ʱ���ʾ��һ������δ��ɣ���Ϊ '0' ��ʱ���־�����ѽ���
/*****					   
 �洢10���м����ݵķֲ��㣬����������ѹ��

******/

#define  GREEN_ON          (GPIOA->BSRR = GPIO_Pin_4)
#define  GREEN_OFF           (GPIOA->BRR  = GPIO_Pin_4) 

#define  RED_ON          (GPIOA->BSRR = GPIO_Pin_3)
#define  RED_OFF           (GPIOA->BRR  = GPIO_Pin_3) 

#define  SPX2941_HIGH          (GPIOA->BSRR = GPIO_Pin_12)
#define  SPX2941_LOW           (GPIOA->BRR  = GPIO_Pin_12) 

#define  MOS_ON	         (GPIOA->BSRR = GPIO_Pin_11)		//M_POWER��PA11��MAX_VDD,MAX3232
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

�½��洢�ռ����
4KB=4096B
��ַ0x0000~0x0fff

��100����Ȩ��ÿ��12�ֽڣ���1200�ֽ�
*/


#define KEY_ENCODE	0x00		//Կ�ױ��룬0x00~0x01(0x0001)����ַ�ŵ� �ߵ�ַ�Ÿߣ�,��ʱֻ��0x00һ���ֽ�

#define INIT_FLAG   0x03	//��Ȩָ��ĳ�ʼ��


#define AUTHOR_ROW_H    0x05  //��Ȩ�û���Ϣ����,���25��
#define AUTHOR_ROW_L    0x06  

#define AUTHOR_POINT_H  0x07  //��ǰ��Ȩ�û���Ϣ��ָ��
#define AUTHOR_POINT_L  0x08


#define CAIMA_ROW_H			0x09	//������������30��
#define CAIMA_ROW_L			0x0a

#define CAIMA_POINT_H		0x0b
#define CAIMA_POINT_L		0x0c


#define RECORD_ROW_H		0x0d  //������¼���100��
#define RECORD_ROW_L		0x0e

#define RECORD_POINT_H  0x0f
#define RECORD_POINT_L  0x10


#define XUNJIAN_ROW_H		0x11  //��Ѳ���������10��
#define XUNJIAN_ROW_L		0x12

#define XUNJIAN_POINT_H 0x13
#define XUNJIAN_POINT_L 0x14 


#define CAIMA_SUO_POINT_H 0x15	//����ʱ����Ĵ洢ָ��
#define CAIMA_SUO_POINT_L 0x16


#define AUTHOR_START    0x400   //��Ȩ�ļ�,25*24B=600B
#define AUTHOR_END      0x658

#define CAIMA_START			0x659   //����,30*8B=240B
#define CAIMA_END				0x749

#define RECORD_START		0x800		//������¼,100*10B=1000B
#define RECORD_END			0xbe8


#define XUNJIAN_START		0xc00		//Ѳ������ͽ��
#define XUNJIAN_END			0xfff	


#endif
