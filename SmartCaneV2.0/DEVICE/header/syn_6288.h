#ifndef __SYN6288_H
#define __SYN6288_H

#define GB2312 0
#define GBK 1
#define BIG5 2
#define UNICODE 3
#define SYN_CODE GBK
#define SYN_Handler USART2_IRQHandler

#include "usart.h"
#include "string.h"

//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
		//m[0~16]:0��������Ϊ������16���������������
		//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
		//t[0~5]:0�ʶ�����������5�ʶ��������
		//���������ù�����ο������ֲ�
typedef struct{
	uint8_t Music;
	uint8_t *data;
	void (*Init)(void);
	void (*Play)(uint8_t Music, uint8_t *HZdata);
}SYN_DEVICE;

void Syn_Init(void);
void Syn_Voice(uint8_t Music, uint8_t *HZdata);
//void Syn_Stop(void);
//void Syn_Suspend(void);
//void Syn_Resume(void);
//void Syn_Query(void);
void Syn_PowerDown(void);

#endif

