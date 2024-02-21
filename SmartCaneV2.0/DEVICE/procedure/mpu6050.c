#include "mpu6050.h"

uint8_t MPU6050_Init(void){
	IIC_Init();
	MPU6050_WriteReg(MPU_PWR_MGMT1_REG,0x01);
	MPU6050_WriteReg(MPU_PWR_MGMT2_REG,0x00);
	MPU6050_WriteReg(MPU_SAMPLE_RATE_REG,0x09);
	MPU6050_WriteReg(MPU_CFG_REG,0x06);
	MPU6050_WriteReg(MPU_GYRO_CFG_REG,0x18);
	MPU6050_WriteReg(MPU_ACCEL_CFG_REG,0x18);
	if(MPU6050_ReadReg(MPU_DEVICE_ID_REG)==MPU_ADDR)
		return 0;
	else
		return 1;
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress){
	uint8_t Data;
	
	IIC_StartBit();
	IIC_SendByte(MPU_ADDR<<1);
	IIC_SendByte(RegAddress);
	IIC_StartBit();
	IIC_SendByte((MPU_ADDR<<1)|0x01);
	Data = IIC_ReceiveByte(NACK);
	IIC_StopBit();
	
	return Data;
}

uint8_t MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data){
	IIC_StartBit();
	if(IIC_SendByte(MPU_ADDR<<1)){
		IIC_StopBit();
		return 1;
	}
	if(IIC_SendByte(RegAddress)){
		IIC_StopBit();
		return 1;
	}
	if(IIC_SendByte(Data)){
		IIC_StopBit();
		return 1;
	}
	IIC_StopBit();
	return 0;
}

MPU_DATA MPU6050_GetAccelerometer(void){
	MPU_DATA Data;
	uint8_t buf[6];
	
	IIC_ReadReg_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	Data.a_x = (buf[0]<<8)|buf[1];
	Data.a_y = (buf[2]<<8)|buf[3];
	Data.a_z = (buf[4]<<8)|buf[5];
	
	return Data;
}

MPU_DATA MPU6050_GetGyroscope(void){
	MPU_DATA Data;
	uint8_t buf[6];
	
	IIC_ReadReg_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	Data.g_x = (buf[0]<<8)|buf[1];
	Data.g_y = (buf[2]<<8)|buf[3];
	Data.g_z = (buf[4]<<8)|buf[5];
	
	return Data;
}

MPU_DATA MPU6050_GetData(void){
	MPU_DATA Data,re;
	re = MPU6050_GetAccelerometer();
	Data.a_x = re.a_x;
	Data.a_y = re.a_y;
	Data.a_z = re.a_z;
	re = MPU6050_GetGyroscope();
	Data.g_x = re.g_x;
	Data.g_y = re.g_y;
	Data.g_z = re.g_z;
	Data.roll = atan2(Data.a_z,-1*Data.a_x)*180/3.1415926;
	Data.pitch = atan2(Data.a_y,-1*Data.a_x)*180/3.1415926;
	return Data;
}

MPU_DATA MPU6050_GetAngle(void){
	MPU_DATA Data,re;
	re = MPU6050_GetAccelerometer();
	Data.roll = atan2(re.a_z,-1*re.a_x)*180/3.1415926;
	Data.pitch = atan2(re.a_y,-1*re.a_x)*180/3.1415926;
	return Data;
}

double MPU6050_GetCV(void){
	int i;
	MPU_DATA Data;
	double SVM[5],X=0,SD;
	for(i=0;i<5;i++){
		Data = MPU6050_GetAccelerometer();
		SVM[i] = sqrt(Data.a_x*Data.a_x + Data.a_y*Data.a_y + Data.a_z*Data.a_z);
		X+=SVM[i];
	}
	X/=5.f;
	for(i=0;i<5;i++){
		SD += (SVM[i]-X)*(SVM[i]-X);
	}
	SD = sqrt(SD/5.f);
	return X/SD;
}

uint8_t MPU6050_FallDetection(void){
	MPU_DATA data;
	if(MPU6050_GetCV()<10){
		delay_ms(500);
		data = MPU6050_GetAngle();
		if(data.pitch>75||data.pitch<-75||data.roll>75||data.roll<-75){
			return 0;
		}
	}
	return 1;
}

