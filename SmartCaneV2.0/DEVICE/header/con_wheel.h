#ifndef __CON_WHEEL_H
#define __CON_WHEEL_H

#define Wheel_GPIO GPIOB
#define Wheel_Pin GPIO_Pin_3

#include "gpio.h"

void Wheel_Init(void);
void Wheel_Off(void);
void Wheel_On(void);

#endif

