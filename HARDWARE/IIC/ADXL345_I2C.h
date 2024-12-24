#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

#define ADXL345_PORT    GPIOB
#define ADXL345_SCL     GPIO_Pin_10
#define ADXL345_SDA     GPIO_Pin_12
//IO方向设置
#define ADXL345_SDA_IN()  Set_Pin_Input_Mode(ADXL345_PORT,ADXL345_SDA)//{GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=8<<16;}
#define ADXL345_SDA_OUT() Set_Pin_Output_Mode(ADXL345_PORT,ADXL345_SDA)//{GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=3<<16;}

//IO操作函数	 
#define ADXL345_IIC_SCL    PinOut(ADXL345_PORT,ADXL345_SCL) //SCL
#define ADXL345_IIC_SDA    PinOut(ADXL345_PORT,ADXL345_SDA) //SDA	 
#define ADXL345_READ_SDA   PinIn(ADXL345_PORT,ADXL345_SDA)  //输入SDA 

//IIC所有操作函数
void ADXL345_IIC_Init(void);                //初始化IIC的IO口				 
void ADXL345_IIC_Start(void);				//发送IIC开始信号
void ADXL345_IIC_Stop(void);	  			//发送IIC停止信号
void ADXL345_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 ADXL345_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 ADXL345_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void ADXL345_IIC_Ack(void);					//IIC发送ACK信号
void ADXL345_IIC_NAck(void);				//IIC不发送ACK信号

void ADXL345_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 ADXL345_IIC_Read_One_Byte(u8 daddr,u8 addr);
#endif
















