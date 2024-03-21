#ifndef __BH1750_H
#define __BH1750_H


#include "stm32f10x.h"
#include "delay.h"





/*	������ַ(7λ-������1λ-���1λΪ��дλ--1Ϊ��-0Ϊд) */
#define				BH1750_Addr_GND_REG				0X23	//ADDR���Žӵ͵�ƽ
#define				BH1750_Addr_VCC_REG				0X5C	//ADDR���ŽӸߵ�ƽ


				/******	ָ��궨��*****/
#define				BH1750_Power_OFF_REG			0x00	//�ϵ�ָ��
#define				BH1750_Power_ON_REG				0x01	//ͨ��,�ȴ���������
#define				BH1750_MODULE_RESET_REG			0x07	//�������ݼĴ���,�ػ�ģʽ�£����ܽ�����������


/*************************************************	
	��ͬģʽ�·ֱ��ʲ�ͬ��Ҳ�����Ȳ�ͬ��
	�߷ֱ���ģʽ2���ֱ�����0.5lx
	�߷ֱ���ģʽ���ֱ���1lx
	�ͷֱ���ģʽ���ֱ���4lx
	��ͬģʽֻ�Ǿ��Ȳ�һ�������ڼ���û������
***************************************************/
	/**	����ģʽָ�	**/
#define				BH1750_CONTINUE_H_MODE			0x10	//����H�ֱ���ģʽ
#define				BH1750_CONTINUE_H_MODE2			0x11	//����H�ֱ���ģʽ2
#define				BH1750_CONTINUE_L_MODE			0x13	//����L�ֱ���ģʽ
#define				BH1750_ONE_TIME_H_MODE			0x20	//һ��H�ֱ���ģʽ
#define				BH1750_ONE_TIME_H_MODE2			0x21	//һ��H�ֱ���ģʽ2
#define				BH1750_ONE_TIME_L_MODE			0x23	//һ����L�ֱ���ģʽ



/**		����������	**/
void 		I2C_Configuration				(void);
void 		BH1750_WriteByte				(uint8_t addr);
uint16_t 	BH1750_Read_Data				(void);
void 		BH1750_Power_ON					(void);
void 		BH1750_Power_OFF				(void);
void 		BH1750_RESET					(void);
void 		BH1750_Init						(void);
float 		Light_Intensity					(void);
		

	
#endif

