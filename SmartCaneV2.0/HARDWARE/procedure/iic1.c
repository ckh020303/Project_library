#include "iic1.h"

void IIC1_Init(void){
	GPIO_simple_Init(IIC1_PORT, IIC1_SCL|IIC1_SDA, GPIO_Mode_Out_OD);
	IIC1_W_SDA(1);
	IIC1_W_SCL(1);
}

void IIC1_StartBit(void){
	IIC1_W_SDA(1);
	IIC1_DELAY();
	IIC1_W_SCL(1);
	IIC1_DELAY();
	IIC1_W_SDA(0);
	IIC1_DELAY();
	IIC1_W_SCL(0);
	IIC1_DELAY();
}
	
void IIC1_StopBit(void){
	IIC1_W_SDA(0);
	IIC1_DELAY();
	IIC1_W_SCL(1);
	IIC1_DELAY();
	IIC1_W_SDA(1);
	IIC1_DELAY();
}
void IIC1_SendBit(uint8_t bit){
	if(bit){
		IIC1_W_SDA(1);
	}else{
		IIC1_W_SDA(0);
	}
	IIC1_DELAY();
	IIC1_W_SCL(1);
	IIC1_DELAY();
	IIC1_W_SCL(0);
	IIC1_DELAY();
}

uint8_t IIC1_SendByte(uint8_t byte){
	uint8_t i,act_bit;
	for(i=0; i<8; i++){
		if(byte&0x80){
			IIC1_SendBit(1);
		}else{
			IIC1_SendBit(0);
		}
		byte<<=1;
	}
	act_bit = IIC1_ReceiveBit();
	return act_bit;
}

uint8_t IIC1_ReceiveBit(void){
	uint8_t bit;

	IIC1_W_SDA(1);
	IIC1_DELAY();
	IIC1_W_SCL(1);
	IIC1_DELAY();
	if(IIC1_R_SDA()){
		bit = 1;
	}else{
		bit = 0;
	}
	IIC1_W_SCL(0);
	IIC1_DELAY();
	
	return bit;
}

uint8_t IIC1_ReceiveByte(uint8_t ack_bit){
	uint8_t byte,i;
	for(i=0; i<8; i++){
		byte <<= 1;
		if(IIC1_ReceiveBit()){
			byte |= 0x01;
		}else{
			byte &= 0xfe;
		}
	}
	IIC1_SendBit(ack_bit);
	return byte;
}

uint8_t IIC1_ReadReg_Len(uint8_t addr,uint8_t RegAddress,uint8_t len,uint8_t *Data){
	IIC1_StartBit(); 
	if(IIC1_SendByte((addr<<1)|0)){	//����������ַ+д����,�ȴ�Ӧ��
		IIC1_StopBit();		 
		return 1;		
	}
    IIC1_SendByte(RegAddress);	//д�Ĵ�����ַ
    IIC1_StartBit();
	IIC1_SendByte((addr<<1)|1);//����������ַ+������	
	while(len)
	{
		if(len==1)*Data=IIC1_ReceiveByte(NACK);//������,����nACK 
		else *Data=IIC1_ReceiveByte(ACK);		//������,����ACK  
		len--;
		Data++; 
	}    
    IIC1_StopBit();	//����һ��ֹͣ���� 
	return 0;
}

uint8_t IIC1_WriteReg_Len(uint8_t addr,uint8_t RegAddress,uint8_t len,uint8_t *Data){
	uint8_t i; 
    IIC1_StartBit(); 
	if(IIC1_SendByte((addr<<1)|0)){	//����������ַ+д����,�ȴ�Ӧ��
		IIC1_StopBit();		 
		return 1;		
	}
    IIC1_SendByte(RegAddress);	//д�Ĵ�����ַ
	for(i=0;i<len;i++){
		if(IIC1_SendByte(Data[i])){	//��������,�ȴ�ACK
			IIC1_StopBit();	 
			return 1;		 
		}		
	}    
    IIC1_StopBit();	 
	return 0;
}

