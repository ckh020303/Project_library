#ifndef __PULSE_H
#define __PULSE_H 

#include "adc.h"
#include "delay.h"

#define SAMPLE_PERIOD	25  	// 波形采样周期(ms)
#define BUFF_SIZE		40		// 数据缓冲区大小

#define PULSE_GPIO GPIOA
#define PULSE_Pin GPIO_Pin_4
#define PULSE_Ch ADC_Channel_4

typedef struct{
	u16 data;
	void (*Init)(void);
	void (*Get_Value)(u16 *BPM);
}PULSE_DEVICE;

void Pulse_Init(void);//心率传感器初始化
u16 Pulse_Get_One_Value(void);//心率传感器取一次值
u16 Pulse_Get_Value(void);//心率传感器取值
u16 get_Array_Max(u16 *array, u8 size);
u16 get_Array_Min(u16 *array, u8 size);
void pulse_Get_Heart_Rate(u16 *BPM);

extern u16 pulse_readData, pulse_preReadData;		   		// 读取到的 AD 值
extern u16 pulse_timeCount;       // 采样周期计数变量
extern u16 pulse_firstTimeCount;   // 第一个心跳时间
extern u16 pulse_secondTimeCount;   // 第二个心跳时间
extern u16 pulse_data[BUFF_SIZE];   // 采样数据缓存
extern u16 PRE_PULSE;		// 有效脉搏
extern u16 PULSE;        // 先前有效脉搏
extern u8 pulse_index; 				// 缓存数组下标
extern u16 pulse_max, pulse_min, pulse_mid;			// 最大、最小及中间值
extern u16 pulse_filter;				// 滤波值
extern u16 pulseCount;	// 脉搏计数

#endif

