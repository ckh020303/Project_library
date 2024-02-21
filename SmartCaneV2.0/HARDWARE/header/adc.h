#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#include "stm32f10x.h" 

#define ADC1_PERIPHERAL_EN RCC_APB2Periph_ADC1  //ADC1ʱ��
#define ADC1_PERIPHERAL ADC1   //ADC1
#define ADC1_RCC_PCLK RCC_PCLK2_Div6   //ADC1��Ƶ����
#define ADC1_SampleTime ADC_SampleTime_239Cycles5  //ADC1����ʱ��

//ADC1״̬
typedef enum{
	Not_Init = 0,
	Finish_Intit
}Adc_State;

void Adc_Init(void);
void Adc_In_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
u16  Get_Adc(u8 ch); 
u16  Get_Adc_Average(u8 ch,u8 times); 
 
#endif 

