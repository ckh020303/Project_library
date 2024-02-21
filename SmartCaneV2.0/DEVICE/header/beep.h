#ifndef __BEEP_H
#define __BEEP_H

#include "gpio.h"
#include "delay.h"

#define BEEP_GPIO GPIOB
#define BEEP_PIN GPIO_Pin_5
#define BEEP PBout(5)	   

typedef struct{
	void (*Init)(void);
	void (*Alert)(uint16_t times);
	void (*Remind)();
}BEEP_DEVICE;

void Beep_Init(void);	
void Beep_Alert(uint16_t times);
void Beep_Remind(void);

#endif

