#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"                  // Device header

void PWM_Init(void);
void PWM_SetCompare_x(uint16_t Compare_x);
void PWM_SetCompare_y(uint16_t Compare_y);
void Servo_SetAngle_y(float Angle);
void Servo_SetAngle_x(float Angle);
void PWM_Limit_x(uint16_t *Compare_x); 
void PWM_Limit_y(uint16_t *Compare_y); 

#endif
