  #include "Li_Power.h"
  #include "stm32f10x_adc.h"
  //#include "main.h"
 
  u8 batt_inc =0;
	unsigned int  battery[20];

/*****
SPX5205控制电源芯片开关
*******/

 void Spx_En_Out(void) 
 {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);    //Key_IO--通信管脚
 }


  /*****************************************************************************
* Function Name  : ADC_GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   /* Configure PB.00 (ADC Channel8) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
锂电池检测是ADC1
VDD检测是ADC2
两个通道同时打开的话，必须采用DMA模式
*******************************************************************************/
void ADC_Configuration(void)
{
  /* ADC1 Configuration ------------------------------------------------------*/
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);/* ADC1 regular channel1 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_13Cycles5);
  /* Configure high and low analog watchdog thresholds */
  ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0B00, 0x0300);
  /* Configure channel8 as the single analog watchdog guarded channel */
  ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_1);
  /* Enable analog watchdog on one regular channel */
  ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
 /* Enable AWD interupt */
 // ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

u8 Power_Cheak(void)
{
  u8 state=0;
  u8 i =0;
  battery[batt_inc] =ADC_GetConversionValue(ADC1);//
  batt_inc++;
  if(batt_inc >=20)
  {
       Clear_WDT();
	   batt_inc =0;
	   for(batt_inc=0;batt_inc<20;batt_inc++)
	   {
	      if(battery[batt_inc] < BATTERY_VALUE)
	      {
	        i++;
	      }
       }
	   if(i >=15)
	   {
	    Clear_WDT();
			state =1;return state;//低电压

	   }
		 else if(i<15 && i>10)
		 {
			 Clear_WDT();
			 state =2;return state;
     }
		 else if(i<10 && i>5)
		 {
			 Clear_WDT();
		state =3;return state;
     }
	  
   }
    Clear_WDT();
		 return state;  

}
