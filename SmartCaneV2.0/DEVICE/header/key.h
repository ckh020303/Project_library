#ifndef __KEY_H
#define __KEY_H

#include "gpio.h"

#define KEY_GPIO GPIOB
#define KEY_BIT0 GPIO_Pin_13
#define KEY_BIT1 GPIO_Pin_14
#define KEY_BIT2 GPIO_Pin_15
#define KEY_EN GPIO_Pin_12
#define KEY_EXIT_GPIO GPIO_PortSourceGPIOB
#define KEY_EXIT_PIN GPIO_PinSource12
#define KEY_Handler EXTI15_10_IRQHandler
#define KEY8 0
#define KEY7 1
#define KEY6 2
#define KEY5 3
#define KEY4 4
#define KEY3 5
#define KEY2 6
#define KEY1 7

void Key_Init(void);
uint8_t Key_Read(void);

#endif

