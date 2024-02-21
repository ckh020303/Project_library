#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"                  // Device header

float PID_x(float target_value_x, float temp_val_x, float kp_x, float ki_x, float kd_x);
uint16_t PID_y(uint16_t target_value_y, uint16_t temp_val_y, float kp_y, float ki_y, float kd_y);

#endif
