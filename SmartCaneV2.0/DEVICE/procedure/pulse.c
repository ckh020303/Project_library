#include "pulse.h"

u16 pulse_readData, pulse_preReadData;		   		// ��ȡ���� AD ֵ
u16 pulse_timeCount=0;       // �������ڼ�������
u16 pulse_firstTimeCount=0;   // ��һ������ʱ��
u16 pulse_secondTimeCount=0;   // �ڶ�������ʱ��
u16 pulse_data[BUFF_SIZE]={0};   // �������ݻ���
u16 PRE_PULSE=0;		// ��Ч����
u16 PULSE=0;        // ��ǰ��Ч����
u8 pulse_index=0; 				// ���������±�
u16 pulse_max, pulse_min, pulse_mid;			// �����С���м�ֵ
u16 pulse_filter=4096;				// �˲�ֵ
u16 pulseCount;	// ��������  

//���ʴ�������ʼ��
void Pulse_Init(void){
	Adc_In_Init(PULSE_GPIO,PULSE_Pin);
	while(1){
		pulse_preReadData = pulse_readData;	        // ����ǰһ��ֵ
		pulse_readData = Pulse_Get_One_Value();		// ��ȡADת��ֵ
		
		if ((pulse_readData - pulse_preReadData) < pulse_filter)    // �˳�ͻ�������źŸ���
			pulse_data[pulse_index++] = pulse_readData;	// ��仺������

		if (pulse_index >= BUFF_SIZE)
		{	
			pulse_index = 0;	// ������������ͷ����
		
			// ͨ�����������ȡ�����źŵĲ��塢����ֵ���������м�ֵ��Ϊ�ж��ο���ֵ
			pulse_max = get_Array_Max(pulse_data, BUFF_SIZE);
			pulse_min = get_Array_Min(pulse_data, BUFF_SIZE);
			pulse_mid = (pulse_max + pulse_min)/2;
			pulse_filter = (pulse_max - pulse_min) / 2;
			break;
		}
	}
}

//���ʴ�����ȡֵ
u16 Pulse_Get_One_Value(void){
	return Get_Adc(PULSE_Ch);
}

//���ʴ�����ȡֵ
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

//��ȡ�������ֵ
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

//��ȡ������Сֵ
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

//��ȡ������ֵ
void pulse_Get_Heart_Rate(u16 *BPM){
	u16 IBI;
	while(1){
		pulse_preReadData = pulse_readData;	        // ����ǰһ��ֵ
		pulse_readData = Pulse_Get_Value();		// ��ȡADת��ֵ
		
		if ((pulse_readData - pulse_preReadData) < pulse_filter)    // �˳�ͻ�������źŸ���
			pulse_data[pulse_index++] = pulse_readData;	// ��仺������

		if (pulse_index >= BUFF_SIZE){	
			pulse_index = 0;	// ������������ͷ����
		
			// ͨ�����������ȡ�����źŵĲ��塢����ֵ���������м�ֵ��Ϊ�ж��ο���ֵ
			pulse_max = get_Array_Max(pulse_data, BUFF_SIZE);
			pulse_min = get_Array_Min(pulse_data, BUFF_SIZE);
			pulse_mid = (pulse_max + pulse_min)/2;
			pulse_filter = (pulse_max - pulse_min) / 2;
		}
		
		PRE_PULSE = PULSE;	// ���浱ǰ����״̬
		PULSE = (pulse_readData > pulse_mid) ? 1 : 0;  // ����ֵ�����м�ֵΪ��Ч����
		
		if (PRE_PULSE == 1 && PULSE == 0){  // Ѱ�ҵ����ź�����������м�λ�á��������㣬��⵽һ����Ч����	
			pulseCount++;
			pulseCount %= 2;	 
			
			if(pulseCount == 1){ // ���������ĵ�һ��                         	
				pulse_firstTimeCount = pulse_timeCount;   // ��¼��һ������ʱ��
			}
			if(pulseCount == 0){  // ���������ĵڶ���                             			
				pulse_secondTimeCount = pulse_timeCount;  // ��¼�ڶ�������ʱ��
				pulse_timeCount = 0;	

				if ( (pulse_secondTimeCount > pulse_firstTimeCount)){
					IBI = (pulse_secondTimeCount - pulse_firstTimeCount) * SAMPLE_PERIOD;	// ������������������ʱ�䣬�õ� IBI
					*BPM = 60000 / IBI;  // ͨ�� IBI �õ�����ֵ BPM
					
					if (*BPM > 200)    //����BPM�����ʾֵ
						*BPM = 200;	 				
					if (*BPM < 30)    //����BPM�����ʾֵ
						*BPM=30;
					
				}	
			}
			break;
		}
		pulse_timeCount++;  // ʱ������ۼ�
		delay_ms(15);
	}
}

