#ifndef __SHOCK_H
#define __SHOCK_H

#include "gpio.h"
#include "delay.h"

#define SHOCK_GPIO GPIOA
#define SHOCK_PIN GPIO_Pin_12
#define SHOCK PAout(12)	   

void Shock_Init(void);
void Shock_Alert(uint16_t times);
void Shock_Remind(void);

#endif

