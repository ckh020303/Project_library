#include "adc.h"
#include "delay.h"
#include "gpio.h"

Adc_State Adc1_State = Not_Init;

/**
  * @brief   ��ʼ��ADC1.
  * @param   None.
  * @retval  None.
  */															   
void Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(ADC1_PERIPHERAL_EN, ENABLE );	  //ʹ��ADC1ͨ��ʱ��

	RCC_ADCCLKConfig(ADC1_RCC_PCLK);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	ADC_DeInit(ADC1_PERIPHERAL);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1_PERIPHERAL, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_Cmd(ADC1_PERIPHERAL, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1_PERIPHERAL);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1_PERIPHERAL));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1_PERIPHERAL);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1_PERIPHERAL));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

	Adc1_State = Finish_Intit;
}		

/**
  * @brief  ��ʼ��ADC1��ӦGPIO��.
  * @param  GPIOx��GPIOA��GPIOB.
  * @param  GPIO_PIN����ӦGPIO�ڣ�GPIOA��GPIO_Pin_0~7��GPIOB��GPIO_Pin_0~1.
  * @retval None.
  */										
void Adc_In_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	if(Adc1_State == Not_Init){
		Adc_Init();
	}
	
	GPIO_simple_Init(GPIOx, GPIO_Pin, GPIO_Mode_AIN);
}

/**
  * @brief  ���һ��ADCת��ֵ.
  * @param  ch��ADC1����ͨ����ADC_Channel_0~9.
  * @retval ����һ��ADCת��ֵ.
  */
u16 Get_Adc(u8 ch){
  //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1_PERIPHERAL, ch, 1, ADC1_SampleTime );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1_PERIPHERAL, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1_PERIPHERAL, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1_PERIPHERAL);	//�������һ��ADC1�������ת�����
}

/**
  * @brief  ��times��AD��ƽ��ֵ.
  * @param  ch��ADC1����ͨ����ADC_Channel_0~9.
  * @param  times����ȡADC�Ĵ���.
  * @retval ���ػ�ȡtimes��ADC��ƽ��ֵ.
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


