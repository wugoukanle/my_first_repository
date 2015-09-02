#include "spi_flash.h"
#include "delay.h"
/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH_PageSize 256

#define WRITE      0x02  /* Write to Memory instruction */
#define WRSR       0x01  /* Write Status Register instruction */ 
#define WREN       0x06  /* Write enable instruction */

#define READ       0x03  /* Read from Memory instruction */
#define RDSR       0x05  /* Read Status Register instruction  */
#define RDID       0x9F  /* Read identification */
#define SE         0xD8  /* Sector Erase instruction */
#define BE         0xC7  /* Bulk Erase instruction */

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte 0x00

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// void SPI_FLASH_Init(void)
// {
//   SPI_InitTypeDef spi1;
//   GPIO_InitTypeDef gpio;
//    
//   /* Enable SPI1 GPIOA and GPIOB clocks */
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);	
// 	
//   /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
//   gpio.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;/* CLK and MOSI*/
//   gpio.GPIO_Mode = GPIO_Mode_AF_PP;
//   gpio.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_Init(GPIOA,&gpio);

//   gpio.GPIO_Pin = GPIO_Pin_6;/* MISO */
//   gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  
//   GPIO_Init(GPIOA,&gpio);

//   gpio.GPIO_Pin = GPIO_Pin_4;/* CS */
//   gpio.GPIO_Mode = GPIO_Mode_AF_PP;
//   gpio.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_Init(GPIOA,&gpio);     


//   SPI_I2S_DeInit(SPI1);
//   spi1.SPI_Direction = SPI_Direction_2Lines_FullDuplex;/* ��???1�� */
//   spi1.SPI_Mode = SPI_Mode_Master;
//   spi1.SPI_DataSize = SPI_DataSize_8b;
//   spi1.SPI_CPOL = SPI_CPOL_Low;   /*  ?�꨺?0 */
//   spi1.SPI_CPHA = SPI_CPHA_1Edge;         
//   spi1.SPI_NSS = SPI_NSS_Soft;
//   spi1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;/* SPI?��?�� 72MHz/4 = 18MHz*/
//   spi1.SPI_FirstBit = SPI_FirstBit_MSB;
// 	spi1.SPI_CRCPolynomial = 7;  

// 	SPI_Init(SPI1,&spi1);
// 	SPI_Cmd(SPI1,ENABLE);		
// 	

// }



void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
   
  /* Enable SPI1 GPIOA and GPIOB clocks */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB, ENABLE); //�Ѿ������˳�ʼ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
  /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PB.02 as Output push-pull, used as Flash Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
  SPI_I2S_DeInit(SPI1); //reset
  /* SPI1 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL =  SPI_CPOL_Low;	//SPI_CPOL_High; //SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = 	SPI_CPHA_1Edge;// SPI_CPHA_2Edge;//	SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);   
}




/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_FLASH_SendByte(u8 byte)
{
   /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReceiveByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_FLASH_ReceiveByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

void eeprom_Init(void)         
{
  SPI_FLASH_CS_LOW();                                 //Ƭѡeeprom
   msDelay(500);
  SPI_FLASH_SendByte(0x06);  
   msDelay(500);
  SPI_FLASH_CS_HIGH();                               //ȡ��Ƭѡeeprom
}

uchar ReadData(uint address)		//�ⲿeeprom��ͨ��SPI����ȡ����
{
  uchar buffer;
  uchar addressH;
  uchar addressL;
  addressH =((uchar *)&address)[1];        //ȡ��˫�ֽڵĸ�8λ
  addressL =((uchar *)&address)[0];        //ȡ��˫�ֽڵĵ�8λ
  SPI_FLASH_CS_LOW();                                  //Ƭѡeeprom
  msDelay(500);
  SPI_FLASH_SendByte(0x03);                             //���Ͷ�eepromָ��
  SPI_FLASH_SendByte(addressH);                         //���͵�ַ��8λ
  SPI_FLASH_SendByte(addressL);                         //���͵�ַ��8λ
  buffer=SPI_FLASH_ReceiveByte();                          //����8λ������
  msDelay(500);
  SPI_FLASH_CS_HIGH();                                //ȡ��Ƭѡeeprom
  return  buffer;
}
 /***
 ��״̬�Ĵ�����ֵ
 ***/
uchar ReadSatus(void)
{
    uchar buffer =0;
   SPI_FLASH_CS_LOW();                                  //Ƭѡeeprom
   msDelay(500);
   SPI_FLASH_SendByte(RDSR);                             //���Ͷ�eepromָ��
   buffer=SPI_FLASH_ReceiveByte();
   msDelay(500);
   SPI_FLASH_CS_HIGH();                                //ȡ��Ƭѡeeprom
   return buffer;

}
/*****************************д�ֽں���****************************************/ 
void WritEDAta(uint address,uchar data)		//ͨ��SPI����д���ݵ�eeprom��
{ 
  uchar k;
  uchar addressH;
  uchar addressL;
  /**/
  SPI_FLASH_CS_LOW(); 
  msDelay(500);                                //Ƭѡeeprom
  SPI_FLASH_SendByte(0x06);                             //дʹ��
  msDelay(500);
  SPI_FLASH_CS_HIGH();                               //ȡ��Ƭѡeeprom
  /**/
  //msDelay(500);
   //msDelay(500);
  msDelay(500);
  addressH =((uchar *)&address)[1];        //ȡ��˫�ֽڵĸ�8λ
  addressL =((uchar *)&address)[0];        //ȡ��˫�ֽڵĵ�8λ
  msDelay(50);
  SPI_FLASH_CS_LOW(); 
  msDelay(500);                                 //Ƭѡeeprom
  SPI_FLASH_SendByte(0x02);                             //����дeepromָ��
  SPI_FLASH_SendByte(addressH);                         //���͵�ַ��8λ
  SPI_FLASH_SendByte(addressL);                         //���͵�ַ��8λ
  SPI_FLASH_SendByte(data);  
  for(k =0;k<10;k++)    //39 --ok                         //����8λ������
  msDelay(500);
  /***	 //���Ӷ�״̬�Ĵ����ĳ���
  while(1)
  {
                                //���Ͷ�eepromָ��
   buffer= ReadSatus();
   if((buffer &0x01) ==0x00)
   break;
  }
  ***/
  SPI_FLASH_CS_HIGH();                               //ȡ��Ƭѡeeprom
  msDelay(500);                                     
}
/*


*/
void  Write_HeadData(uchar *buff)
{
  uchar *str;
  uint total,i;
  str =buff;
  total =str[0];
  total =(total<<8)&0xff00;
  total =total+str[1];    
  WritEDAta(0x0ff6,str[0]);	       //�洢�趨�ĵ��������ܿ���Ŀ
  WritEDAta(0x0ff7,str[1]);
  for(i =0;i<total; i++)
  WritEDAta(0x1000+i,str[2+i]);
  WritEDAta(0xfffc,str[total-1]);
  WritEDAta(0xfffd,str[total-2]);
  WritEDAta(0xfffe,str[total-3]);
  WritEDAta(0xffff,str[total-4]);  //�洢���ĵ��������
  WritEDAta(0xfff8,str[2]);	       //�洢��С�ĵ��������   
  WritEDAta(0xfff9,str[3]);
  WritEDAta(0xfffa,str[4]);
  WritEDAta(0xfffb,str[5]);
}
 /*

д���ź��豸���뵽�洢��
 */
 void Write_Local(uchar *buff)
 {
   uchar *str;
   str =buff;
   WritEDAta(0x0000,str[0]);
   WritEDAta(0x0001,str[1]);
 }

/*
�洢����������Ϣ��EEPROM��

*/

void Write_KeyData(uchar *buff)
{
  uchar *str;
  uchar nChar,i;
  uint  addr_head;
  str =buff;
  nChar = ReadData(0x000d);
  nChar++;
  if(nChar >=100)
  nChar =100;
  WritEDAta(0x000d,nChar);
  addr_head = ReadData(0x000e);
  addr_head =(addr_head <<8)&0xff00;
  addr_head =addr_head+	ReadData(0x000f);
  for(i =0;i<10;i++)
  {
   WritEDAta(addr_head,str[i]);
   addr_head++;
   if(addr_head >=0x0650)
   addr_head =0x0010;
  }
   nChar =(addr_head >>8)&0x00ff;
   WritEDAta(0x000e,nChar);
   nChar =(addr_head&0x00ff);
   WritEDAta(0x000f,nChar);
}
/*
 ��ȡEEPROM�д洢��Կ�����ݣ����͸�һ��������
  ���͸���������ڵ�ʱ�򣬶�ȡһ���ֽڣ�����һ���ֽڵ�ģʽ
*/
void Read_KeyData(uchar *buff)
{
 
}

/**** ���ݴ洢���Ĳ��Բ���
 void  test_eeprom(void)
{
  uchar i;
  eeprom_Init();
  for(i =0;i<10;i++)
  msDelay(500);
 //while(1)
  //WritEDAta(0x55,0x55);
  g_Buff[0] =ReadSatus();
   msDelay(500);
  // while(1)
    //g_Buff[0] =ReadData(0x20);
   WritEDAta(0x20,0xaa);
   for(i =0;i<50;i++)
   msDelay(500);
   g_Buff[0] =ReadData(0x20);
   msDelay(500);
   while(1);

}
*******/

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
