#ifndef __GPIO_H
#define __GPIO_H

#include "sys.h"
#include "stm32f10x.h" 

//GPIO״̬
typedef enum{
	No_Ready = 0,
	Ready
}GPIO_State;

void GPIO_simple_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);

#endif

