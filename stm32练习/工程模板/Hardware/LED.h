#ifndef __LED_H
#define __LED_H

void LED_Init(void);
void LED_Set_On(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_Set_Off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
