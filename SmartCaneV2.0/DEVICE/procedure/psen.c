#include "psen.h"

//光敏传感器初始化
void Psen_Init(void){
	Adc_In_Init(PSEN_GPIO,PSEN_Pin);
}

//光敏传感器取值
u16 Psen_Get_Value(void){
	return Get_Adc_Average(PSEN_Ch,10);
}

//光敏传感器一次取值
u16 Psen_Get_One_Value(void){
	return Get_Adc(PSEN_Ch);
}

//电压求光照强度取值
float Psen_Get_Light_Intensity(void);
