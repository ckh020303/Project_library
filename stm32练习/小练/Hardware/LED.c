#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
}

void LED_Set_On(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOx, GPIO_Pin);
}

void LED_Set_Off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_SetBits(GPIOx, GPIO_Pin);
}

void LED_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)==1){
		GPIO_ResetBits(GPIOx, GPIO_Pin);
	}
	else
	{
		GPIO_SetBits(GPIOx, GPIO_Pin);
	}
}
