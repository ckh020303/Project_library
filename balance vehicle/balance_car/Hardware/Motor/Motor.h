#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h" 

#define Ain1  PBout(13)
#define Ain2  PBout(12)

#define Bin1  PBout(14)
#define Bin2  PBout(15)

void Motor_Init(void);
int abs(int p);
void Limit(int *motoA,int *motoB);
void Load(int moto1, int moto2);

#endif
