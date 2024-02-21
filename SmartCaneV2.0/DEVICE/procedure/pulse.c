#include "pulse.h"

u16 pulse_readData, pulse_preReadData;		   		// 读取到的 AD 值
u16 pulse_timeCount=0;       // 采样周期计数变量
u16 pulse_firstTimeCount=0;   // 第一个心跳时间
u16 pulse_secondTimeCount=0;   // 第二个心跳时间
u16 pulse_data[BUFF_SIZE]={0};   // 采样数据缓存
u16 PRE_PULSE=0;		// 有效脉搏
u16 PULSE=0;        // 先前有效脉搏
u8 pulse_index=0; 				// 缓存数组下标
u16 pulse_max, pulse_min, pulse_mid;			// 最大、最小及中间值
u16 pulse_filter=4096;				// 滤波值
u16 pulseCount;	// 脉搏计数  

//心率传感器初始化
void Pulse_Init(void){
	Adc_In_Init(PULSE_GPIO,PULSE_Pin);
	while(1){
		pulse_preReadData = pulse_readData;	        // 保存前一次值
		pulse_readData = Pulse_Get_One_Value();		// 读取AD转换值
		
		if ((pulse_readData - pulse_preReadData) < pulse_filter)    // 滤除突变噪声信号干扰
			pulse_data[pulse_index++] = pulse_readData;	// 填充缓存数组

		if (pulse_index >= BUFF_SIZE)
		{	
			pulse_index = 0;	// 数组填满，从头再填
		
			// 通过缓存数组获取脉冲信号的波峰、波谷值，并计算中间值作为判定参考阈值
			pulse_max = get_Array_Max(pulse_data, BUFF_SIZE);
			pulse_min = get_Array_Min(pulse_data, BUFF_SIZE);
			pulse_mid = (pulse_max + pulse_min)/2;
			pulse_filter = (pulse_max - pulse_min) / 2;
			break;
		}
	}
}

//心率传感器取值
u16 Pulse_Get_One_Value(void){
	return Get_Adc(PULSE_Ch);
}

//心率传感器取值
u16 Pulse_Get_Value(void){
	u8 i,j;
	u16 buf[20] = {0},temp;
	for(i=0;i<20;i++){
		buf[i] = Pulse_Get_One_Value();
	}
	for(i=0;i<19;i++){
		for(j=0;j<19-i;j++){
			if(buf[j] > buf[j+1]){
				temp = buf[j];
				buf[j] = buf[j+1];
				buf[j+1] = temp;
			}
		}
	}
	temp=0;
	for(i=2;i<18;i++){
		temp += buf[i];
	}
	return temp/16;
}

//获取脉搏最大值
u16 get_Array_Max(u16 *array, u8 size){
	u16 max = array[0];
	u8 i;
	
	for (i = 1; i < size; i++)
	{
		if (array[i] > max)
			max = array[i];
	}
	
	return max;
}

//获取脉搏最小值
u16 get_Array_Min(u16 *array, u8 size){
	u16 min = array[0];
	u8 i;
	
	for (i = 1; i < size; i++)
	{
		if (array[i] < min)
			min = array[i];
	}
	
	return min;
}

//获取心率数值
void pulse_Get_Heart_Rate(u16 *BPM){
	u16 IBI;
	while(1){
		pulse_preReadData = pulse_readData;	        // 保存前一次值
		pulse_readData = Pulse_Get_Value();		// 读取AD转换值
		
		if ((pulse_readData - pulse_preReadData) < pulse_filter)    // 滤除突变噪声信号干扰
			pulse_data[pulse_index++] = pulse_readData;	// 填充缓存数组

		if (pulse_index >= BUFF_SIZE){	
			pulse_index = 0;	// 数组填满，从头再填
		
			// 通过缓存数组获取脉冲信号的波峰、波谷值，并计算中间值作为判定参考阈值
			pulse_max = get_Array_Max(pulse_data, BUFF_SIZE);
			pulse_min = get_Array_Min(pulse_data, BUFF_SIZE);
			pulse_mid = (pulse_max + pulse_min)/2;
			pulse_filter = (pulse_max - pulse_min) / 2;
		}
		
		PRE_PULSE = PULSE;	// 保存当前脉冲状态
		PULSE = (pulse_readData > pulse_mid) ? 1 : 0;  // 采样值大于中间值为有效脉冲
		
		if (PRE_PULSE == 1 && PULSE == 0){  // 寻找到“信号上升到振幅中间位置”的特征点，检测到一次有效脉搏	
			pulseCount++;
			pulseCount %= 2;	 
			
			if(pulseCount == 1){ // 两次脉搏的第一次                         	
				pulse_firstTimeCount = pulse_timeCount;   // 记录第一次脉搏时间
			}
			if(pulseCount == 0){  // 两次脉搏的第二次                             			
				pulse_secondTimeCount = pulse_timeCount;  // 记录第二次脉搏时间
				pulse_timeCount = 0;	

				if ( (pulse_secondTimeCount > pulse_firstTimeCount)){
					IBI = (pulse_secondTimeCount - pulse_firstTimeCount) * SAMPLE_PERIOD;	// 计算相邻两次脉搏的时间，得到 IBI
					*BPM = 60000 / IBI;  // 通过 IBI 得到心率值 BPM
					
					if (*BPM > 200)    //限制BPM最高显示值
						*BPM = 200;	 				
					if (*BPM < 30)    //限制BPM最低显示值
						*BPM=30;
					
				}	
			}
			break;
		}
		pulse_timeCount++;  // 时间计数累加
		delay_ms(15);
	}
}

