/*******************************************************************************
** �ļ���: 		mian.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.14
** ����: 		wuguoyana
** ��������: 	2011-04-29
** ����:		LED���Գ���
				������д�������ϣ�LED2-LED5�����ε�������˸
** ����ļ�:	stm32f10x.h
** �޸���־��	2011-04-29 �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
#include "stm32f10x.h"
/* �������� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define LED_D0   GPIO_Pin_8		//PA8
#define LED_D1   GPIO_Pin_2		//PD2

/* ���� ----------------------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;
/* �������� ------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount);
void LED_Configuration(void);
void RCC_HSI_Configuration(void);
/* �������� ------------------------------------------------------------------*/

/*******************************************************************************
  * @��������	main
  * @����˵��   ��������Ӳ����ʼ����ʵ��LED1-LED4��˸
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
int main(void)
{
    LED_Configuration();

    //�����ж��������λ���� 0x3000
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x5000);

    while (1)
    {
        //����ͨ��3�ַ�ʽ������GPIO��ƽʵ��LED��˸
        GPIO_ResetBits(GPIOA,LED_D0);//����LED1
        Delay(100);
        GPIO_SetBits(GPIOA,LED_D0);  //Ϩ��LED1
        Delay(100);
        GPIO_ResetBits(GPIOD,LED_D1);//����LED2
        Delay(100);
        GPIO_SetBits(GPIOD,LED_D1);  //Ϩ��LED2
        Delay(100);

//         GPIO_WriteBit(GPIOA,LED_D0,Bit_RESET); //����LED3
//         Delay(100);
//         GPIO_WriteBit(GPIOD,LED_D1,Bit_SET);   //Ϩ��LED3
//         Delay(100);


    }
}

/*******************************************************************************
  * @��������	LED_Configuration
  * @����˵��   ����ʹ��LED
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void LED_Configuration(void)
{
    //ʹ��LED����GPIO��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);
    //��ʼ��LED��GPIO
    GPIO_InitStructure.GPIO_Pin = LED_D0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,LED_D0);  //Ϩ��LED_D0
	
    GPIO_InitStructure.GPIO_Pin = LED_D1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_SetBits(GPIOD,LED_D1);  //Ϩ��LED_D1
	
	
}

/*******************************************************************************
  * @��������	Delay
  * @����˵��   ����һ����ʱʱ��
  * @�������   nCount: ָ����ʱʱ�䳤��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
    int i,j;
    for (i=0; i<nCount; i++)
        for (j=0; j<10000; j++)
            ;
}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
  * @��������	assert_failed
  * @����˵��   �����ڼ�������������ʱ��Դ�ļ����ʹ�������
  * @�������   file: Դ�ļ���
  				line: ������������
  * @���ز���   ��
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* �û����������Լ��Ĵ������ڱ��������ļ�������������,
       ���磺printf("�������ֵ: �ļ��� %s �� %d��\r\n", file, line) */

    /* ����ѭ�� */
    while (1)
    {
    }
}
#endif

/***************************END OF FILE****************************************/
