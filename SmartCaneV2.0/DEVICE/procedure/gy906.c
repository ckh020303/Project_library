#include "gy906.h"

void Gy906_Init(void){
	IIC1_Init();
}

//功能 : 数据校验
u8 PEC_Calculation(u8 pec[])
{
    u8 	crc[6];//存放多项式
    u8	BitPosition=47;//存放所有数据最高位，6*8=48 最高位就是47位
    u8	shift;
    u8	i;
    u8	j;
    u8	temp;

    do
    {
        /*Load pattern value 0x00 00 00 00 01 07*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;

        /*Set shift position at 0*/
        shift=0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */

        /*Get shift value for pattern value*/
        shift=BitPosition-8;

        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/

        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/

    return pec[0];
}

//读取gy906的RAM/EEPROM的内容
u16 Gy906_ReadMemory(u8 slaveAddress,u8 command){
	u8 DataL=0,DataH=0,Pec,PecReg=1,arr[6],ErrorCounter=0;
	slaveAddress<<=1;
	ErrorCounter--;
	
	do{
		IIC1_StopBit();	
		ErrorCounter--;
		if(!ErrorCounter){
			break;
		}		
		IIC1_StartBit();
		if(IIC1_SendByte(slaveAddress)){
			continue;
		}
		if(IIC1_SendByte(command)){
			continue;
		}
		IIC1_StartBit();
		if(IIC1_SendByte(slaveAddress+1)){
			continue;
		}
		DataL=IIC1_ReceiveByte(ACK);
		DataH=IIC1_ReceiveByte(ACK);
		Pec=IIC1_ReceiveByte(NACK);
		
		arr[5] = slaveAddress;		//
        arr[4] = command;			//
        arr[3] = slaveAddress+1;	//Load array arr
        arr[2] = DataL;				//
        arr[1] = DataH;				//
        arr[0] = 0;					//
        PecReg=PEC_Calculation(arr);//Calculate CRC 数据校验
		
	}while(Pec!=PecReg);
	
	return (DataH<<8)|DataL;
}
	
//获取温度，即读取gy906的RAM0x07
float Gy906_ReadTemp(void){   
	float temp;
	temp = Gy906_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15;
	if(temp == -273.15){
		temp = Gy906_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15;
	}
	return temp;
}


