#include "syn_6288.h"

void Syn_Init(void){
	Usart_Init(USART2,9600,8);
}

//Music:ѡ�񱳾����֡�0:�ޱ������֣�1~15��ѡ�񱳾�����
void Syn_Voice(uint8_t Music, uint8_t* HZdata){
/****************��Ҫ���͵��ı�**********************************/
	uint8_t Frame_Info[50];
	uint8_t HZ_Length;
	uint8_t ecc  = 0;  			//����У���ֽ�
	uint32_t i = 0;
	HZ_Length = strlen((char*)HZdata); 			//��Ҫ�����ı��ĳ���

/*****************֡�̶�������Ϣ**************************************/
	Frame_Info[0] = 0xFD ; 			//����֡ͷFD
	Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
	Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
	Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������
	Frame_Info[4] = SYN_CODE | Music << 4 ; //����������������������趨

/*******************У�������***************************************/
	for(i = 0; i < 5; i++){   				//���η��͹���õ�5��֡ͷ�ֽ�
		ecc = ecc ^ (Frame_Info[i]);		//�Է��͵��ֽڽ������У��
	}

	for(i = 0; i < HZ_Length; i++){   		//���η��ʹ��ϳɵ��ı�����
		ecc = ecc ^ (HZdata[i]); 				//�Է��͵��ֽڽ������У��
	}
/*******************����֡��Ϣ***************************************/
	memcpy(&Frame_Info[5], HZdata, HZ_Length);
	Frame_Info[5 + HZ_Length] = ecc;
	for(i = 0; i < (5 + HZ_Length + 1); i++){
		Usart_SendData(USART2,Frame_Info[i]);
	}
}

//оƬ��������ģʽ
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

