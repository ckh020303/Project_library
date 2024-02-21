#include "iic.h"

void IIC_Init(void){
	GPIO_simple_Init(IIC_PORT, IIC_SCL|IIC_SDA, GPIO_Mode_Out_OD);
	IIC_W_SDA(1);
	IIC_W_SCL(1);
}

void IIC_StartBit(void){
	IIC_W_SDA(1);
	IIC_DELAY();
	IIC_W_SCL(1);
	IIC_DELAY();
	IIC_W_SDA(0);
	IIC_DELAY();
	IIC_W_SCL(0);
	IIC_DELAY();
}
	
void IIC_StopBit(void){
	IIC_W_SDA(0);
	IIC_DELAY();
	IIC_W_SCL(1);
	IIC_DELAY();
	IIC_W_SDA(1);
	IIC_DELAY();
}
void IIC_SendBit(uint8_t bit){
	if(bit){
		IIC_W_SDA(1);
	}else{
		IIC_W_SDA(0);
	}
	IIC_DELAY();
	IIC_W_SCL(1);
	IIC_DELAY();
	IIC_W_SCL(0);
	IIC_DELAY();
}

uint8_t IIC_SendByte(uint8_t byte){
	uint8_t i,act_bit;
	for(i=0; i<8; i++){
		if(byte&0x80){
			IIC_SendBit(1);
		}else{
			IIC_SendBit(0);
		}
		byte<<=1;
	}
	act_bit = IIC_ReceiveBit();
	return act_bit;
}

uint8_t IIC_ReceiveBit(void){
	uint8_t bit;

	IIC_W_SDA(1);
	IIC_DELAY();
	IIC_W_SCL(1);
	IIC_DELAY();
	if(IIC_R_SDA()){
		bit = 1;
	}else{
		bit = 0;
	}
	IIC_W_SCL(0);
	IIC_DELAY();
	
	return bit;
}

uint8_t IIC_ReceiveByte(uint8_t ack_bit){
	uint8_t byte,i;
	for(i=0; i<8; i++){
		byte <<= 1;
		if(IIC_ReceiveBit()){
			byte |= 0x01;
		}else{
			byte &= 0xfe;
		}
	}
	IIC_SendBit(ack_bit);
	return byte;
}

uint8_t IIC_ReadReg_Len(uint8_t addr,uint8_t RegAddress,uint8_t len,uint8_t *Data){
	IIC_StartBit(); 
	if(IIC_SendByte((addr<<1)|0)){	//发送器件地址+写命令,等待应答
		IIC_StopBit();		 
		return 1;		
	}
    IIC_SendByte(RegAddress);	//写寄存器地址
    IIC_StartBit();
	IIC_SendByte((addr<<1)|1);//发送器件地址+读命令	
	while(len)
	{
		if(len==1)*Data=IIC_ReceiveByte(NACK);//读数据,发送nACK 
		else *Data=IIC_ReceiveByte(ACK);		//读数据,发送ACK  
		len--;
		Data++; 
	}    
    IIC_StopBit();	//产生一个停止条件 
	return 0;
}

uint8_t IIC_WriteReg_Len(uint8_t addr,uint8_t RegAddress,uint8_t len,uint8_t *Data){
	uint8_t i; 
    IIC_StartBit(); 
	if(IIC_SendByte((addr<<1)|0)){	//发送器件地址+写命令,等待应答
		IIC_StopBit();		 
		return 1;		
	}
    IIC_SendByte(RegAddress);	//写寄存器地址
	for(i=0;i<len;i++){
		if(IIC_SendByte(Data[i])){	//发送数据,等待ACK
			IIC_StopBit();	 
			return 1;		 
		}		
	}    
    IIC_StopBit();	 
	return 0;
}

