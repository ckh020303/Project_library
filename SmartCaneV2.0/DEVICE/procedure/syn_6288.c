#include "syn_6288.h"

void Syn_Init(void){
	Usart_Init(USART2,9600,8);
}

//Music:选择背景音乐。0:无背景音乐，1~15：选择背景音乐
void Syn_Voice(uint8_t Music, uint8_t* HZdata){
/****************需要发送的文本**********************************/
	uint8_t Frame_Info[50];
	uint8_t HZ_Length;
	uint8_t ecc  = 0;  			//定义校验字节
	uint32_t i = 0;
	HZ_Length = strlen((char*)HZdata); 			//需要发送文本的长度

/*****************帧固定配置信息**************************************/
	Frame_Info[0] = 0xFD ; 			//构造帧头FD
	Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
	Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
	Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
	Frame_Info[4] = SYN_CODE | Music << 4 ; //构造命令参数：背景音乐设定

/*******************校验码计算***************************************/
	for(i = 0; i < 5; i++){   				//依次发送构造好的5个帧头字节
		ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
	}

	for(i = 0; i < HZ_Length; i++){   		//依次发送待合成的文本数据
		ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
	}
/*******************发送帧信息***************************************/
	memcpy(&Frame_Info[5], HZdata, HZ_Length);
	Frame_Info[5 + HZ_Length] = ecc;
	for(i = 0; i < (5 + HZ_Length + 1); i++){
		Usart_SendData(USART2,Frame_Info[i]);
	}
}

//芯片进入休眠模式
void Syn_PowerDown(void){
	uint8_t i;
	uint8_t Frame_Info[] = {0xfd,0x00,0x01,0x88,0x76};
	for(i = 0; i < 5; i++){
		Usart_SendData(USART2,Frame_Info[i]);
	}
}

void SYN_Handler(void){
	USART_ReceiveData(USART2);
}

