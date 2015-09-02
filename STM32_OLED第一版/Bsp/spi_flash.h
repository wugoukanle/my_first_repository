/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : spi_flash.h
* Author             : MCD Application Team
* Date First Issued  : 02/05/2007
* Description        : Header for spi_flash.c file.
********************************************************************************
* History:
* 05/21/2007: V0.3
* 04/02/2007: V0.2
* 02/05/2007: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "general.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Select SPI FLASH: ChipSelect pin low  */
#define SPI_FLASH_CS_LOW()     GPIO_ResetBits(GPIOA, GPIO_Pin_4)
/* Deselect SPI FLASH: ChipSelect pin high */
#define SPI_FLASH_CS_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define  MAX_BUFF      520     //255
#define  MAX_TI        246
#define  WRITE_ZHI     40


extern unsigned  char Ti[MAX_TI];
extern unsigned char g_Buff[MAX_BUFF];
extern unsigned char buff_idx;   //缓冲区的指针，来指示缓冲区接收到的数据
extern unsigned char g_State;
extern unsigned char Lock_number[4];  //存储锁的编号作为全局变量
extern unsigned char time_buff[6];
extern unsigned char key_in_code[4];  //存储锁的区位码及类型
extern unsigned int  h;
extern unsigned int l;
extern unsigned char MI_High;
extern unsigned char MI_Low;
extern unsigned char MI_High_Last;
extern unsigned char MI_Low_Last;
extern unsigned char Ti_INDEX;  //是否删除的地址，为连接表中的第一个地址
extern unsigned int  battery[20]; 
extern unsigned char Rfid_Buff[6];  //读取到的RFID信息的存储缓冲区
extern unsigned char       KeyNum;  //按键值的存储
extern unsigned char m_Uart_State;


/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(const u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(const u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);

/*----- Low layer function -----*/
u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

uchar ReadData(uint address);
void WritEDAta(uint address,uchar data);
void eeprom_Init(void);
uchar ReadSatus(void);
#endif /* __SPI_FLASH_H */

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
