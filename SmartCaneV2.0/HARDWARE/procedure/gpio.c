#include "gpio.h"

void GPIO_simple_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode){
	GPIO_InitTypeDef GPIO_InitStructure;
  
	if(GPIOx == GPIOA){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	}else if(GPIOx == GPIOB){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	}else if(GPIOx == GPIOC){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	}
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_Init(GPIOx, &GPIO_InitStructure);
}


