#ifndef __LIGHT_H
#define __LIGHT_H

#include "gpio.h"
#include "delay.h"

#define Light_GPIO GPIOB
#define Light_Pin GPIO_Pin_4

typedef struct{
	void (*Init)(void);
	void (*Off)(void);
	void (*On)(void);
	void (*Alarm)(void);
}LIGHT_DEVICE;

void Light_Init(void);
void Light_Off(void);
void Light_On(void);
void Light_alarm(void);

#endif

