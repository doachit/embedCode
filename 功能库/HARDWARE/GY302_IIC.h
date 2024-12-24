#ifndef __IIC_H
#define __IIC_H




#include "stm32f10x.h"
#include "delay.h"
#include "sys.h" 


/*********************************
**	����ѡ��					**
**			SCL ---> PXx		**
**			SDA ---> PXx		**
**********************************/
	
/*******************************����ʱ��***********************************/  
#define  IIC_Delay()		{	delay_us(5);	}	   		   

/*************************** IO��������(GPIOx * 4) ****************************/
#define GY302_PORT	    GPIOB
#define GY302_SCL		GPIO_Pin_6
#define GY302_SDA		GPIO_Pin_7
					/**		PXx ��������ģʽ		**/
#define  SDA_IN()  {GPIOB -> CRL &= 0X0FFFFFFF; GPIOB -> CRL |= (u32)8 << 28;}
					/**		PXx �������ģʽ		**/
#define  SDA_OUT() {GPIOB -> CRL &= 0X0FFFFFFF; GPIOB -> CRL |= (u32)3 << 28;}





/*************************** IO�������� *************************/	 
#define 		IIC_SCL    		PBout(6) 	//SCL ���
#define 		IIC_SDA    		PBout(7) 	//SDA ���	
#define 		Read_SDA   		PBin (7)  	//SDA ����





/***************************** IIC���в������� **********************************/

void 				IIC_Init				(void);             //��ʼ��IIC��IO��
void 				IIC_Start				(void);				//����IIC��ʼ�ź�
void 				IIC_Stop				(void);	  			//����IICֹͣ�ź�
void 				IIC_Send_Byte			(uint8_t Txd);		//IIC����һ���ֽ�
uint8_t 			IIC_Read_Byte			(uint8_t ACK);		//IIC��ȡһ���ֽ�
uint8_t 			IIC_Wait_Ack			(void); 			//IIC�ȴ�ACK�ź�
void 				IIC_Ack					(void);				//IIC����ACK�ź�
void 				IIC_NAck				(void);				//IIC������ACK�ź�


 
#endif
