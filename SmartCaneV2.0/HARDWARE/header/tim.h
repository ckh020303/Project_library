#ifndef __TIM_H
#define __TIM_H
#include "sys.h"
#include "stm32f10x.h" 

extern u8 times;
extern u8 xx;//��Ϣ�����־λ

void TIM_Init(TIM_TypeDef* TIMx, u16 arr, u16 psc);
 
#endif

