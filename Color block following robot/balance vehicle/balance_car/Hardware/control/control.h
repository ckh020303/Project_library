#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

extern int Encoder_Left,Encoder_Right;
extern int Vertical_out,Velocity_out,Turn_out; // 直立环&速度环&转向环的输出变量

float Vertical(float Med,float Angle,float gyro_Y); // 函数声明
int Velocity(int Target,int encoder_left,int encoder_right);
float Turn(int gyro_Z);
//void TIM3_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

#endif
