#include "ADXL345_I2C.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//初始化IIC
void ADXL345_IIC_Init(void)
{					     
 	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	INIT_GPIO_CLOCK(ADXL345_PORT);
	GPIO_InitStructure.GPIO_Pin = ADXL345_SCL|ADXL345_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADXL345_PORT, &GPIO_InitStructure);
	GPIO_SetBits(ADXL345_PORT,ADXL345_SCL|ADXL345_SDA); 	//PB6,PB7 输出高
}
//产生IIC起始信号
void ADXL345_IIC_Start(void)
{
    ADXL345_SDA_OUT();     //sda线输出
	ADXL345_IIC_SCL=0;
	delay_us(1);
	ADXL345_IIC_SDA=1;
	delay_us(1);	  	  
	ADXL345_IIC_SCL=1;
	delay_us(4);
 	ADXL345_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	ADXL345_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 

}	  
//产生IIC停止信号
void ADXL345_IIC_Stop(void)
{
    ADXL345_SDA_OUT();//sda线输出
	ADXL345_IIC_SCL=0;
	delay_us(1);
	ADXL345_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	ADXL345_IIC_SCL=1; 
	delay_us(4);
	ADXL345_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);					  
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 ADXL345_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	ADXL345_SDA_IN();      //SDA设置为输入  
	ADXL345_IIC_SDA=1;delay_us(1);	   
	ADXL345_IIC_SCL=1;delay_us(1);	 
	while(ADXL345_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			ADXL345_IIC_Stop();
			return 1;
		}
	}
	ADXL345_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void ADXL345_IIC_Ack(void)
{
	ADXL345_IIC_SCL=0;
	ADXL345_SDA_OUT();
	ADXL345_IIC_SDA=0;
	delay_us(2);
	ADXL345_IIC_SCL=1;
	delay_us(2);
	ADXL345_IIC_SCL=0;
}
//不产生ACK应答		    
void ADXL345_IIC_NAck(void)
{
	ADXL345_IIC_SCL=0;
	ADXL345_SDA_OUT();
	ADXL345_IIC_SDA=1;
	delay_us(2);
	ADXL345_IIC_SCL=1;
	delay_us(2);
	ADXL345_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void ADXL345_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	ADXL345_SDA_OUT(); 	    
    ADXL345_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        ADXL345_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		ADXL345_IIC_SCL=1;
		delay_us(2); 
		ADXL345_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 ADXL345_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	ADXL345_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        ADXL345_IIC_SCL=0; 
        delay_us(2);
		ADXL345_IIC_SCL=1;
        receive<<=1;
        if(ADXL345_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        ADXL345_IIC_NAck();//发送nACK
    else
        ADXL345_IIC_Ack(); //发送ACK   
    return receive;
}



























