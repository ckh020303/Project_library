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

//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
		//m[0~16]:0背景音乐为静音，16背景音乐音量最大
		//v[0~16]:0朗读音量为静音，16朗读音量最大
		//t[0~5]:0朗读语速最慢，5朗读语速最快
		//其他不常用功能请参考数据手册
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

