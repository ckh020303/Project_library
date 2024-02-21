#include "adc.h"
#include "delay.h"
#include "gpio.h"

Adc_State Adc1_State = Not_Init;

/**
  * @brief   初始化ADC1.
  * @param   None.
  * @retval  None.
  */															   
void Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(ADC1_PERIPHERAL_EN, ENABLE );	  //使能ADC1通道时钟

	RCC_ADCCLKConfig(ADC1_RCC_PCLK);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	ADC_DeInit(ADC1_PERIPHERAL);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1_PERIPHERAL, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_Cmd(ADC1_PERIPHERAL, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1_PERIPHERAL);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1_PERIPHERAL));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1_PERIPHERAL);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1_PERIPHERAL));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

	Adc1_State = Finish_Intit;
}		

/**
  * @brief  初始化ADC1相应GPIO口.
  * @param  GPIOx：GPIOA或GPIOB.
  * @param  GPIO_PIN：相应GPIO口，GPIOA的GPIO_Pin_0~7、GPIOB的GPIO_Pin_0~1.
  * @retval None.
  */										
void Adc_In_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	if(Adc1_State == Not_Init){
		Adc_Init();
	}
	
	GPIO_simple_Init(GPIOx, GPIO_Pin, GPIO_Mode_AIN);
}

/**
  * @brief  获得一次ADC转换值.
  * @param  ch：ADC1输入通道，ADC_Channel_0~9.
  * @retval 返回一次ADC转换值.
  */
u16 Get_Adc(u8 ch){
  //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1_PERIPHERAL, ch, 1, ADC1_SampleTime );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1_PERIPHERAL, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1_PERIPHERAL, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1_PERIPHERAL);	//返回最近一次ADC1规则组的转换结果
}

/**
  * @brief  求times次AD的平均值.
  * @param  ch：ADC1输入通道，ADC_Channel_0~9.
  * @param  times：获取ADC的次数.
  * @retval 返回获取times次ADC的平均值.
  */
u16 Get_Adc_Average(u8 ch,u8 times){
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}  


