#include "key.h"
#include "delay.h"
#include "sys.h"
#include "gy302.h"



/****************************************************************************
**	�������ƣ�BH1750_WriteByte()
**	�������ܣ�BH1750 д����
**	����˵����
**			uint8_t addr: �Ĵ�����ַ
**
**	����ֵ����
**	IIC����˵����
**				I2C_GetFlagStatus��IIC_x, IIC_Flag_x��-- ����־λ�Ƿ��� 1
**
*****************************************************************************/
void BH1750_WriteByte(uint8_t addr)
{	
	IIC_Start();								// 1.������ʼ�ź�
	
	IIC_Send_Byte(BH1750_Addr_GND_REG << 1|0);	// 2.����7λ��ַ���дλ��0��д��
	while(!IIC_Wait_Ack());						// 3.�ȴ��ӻ�Ӧ���ź�
	
	IIC_Send_Byte(addr);						// 4.����8λ����ָ��
	while(!IIC_Wait_Ack());						// 5.�ȴ��ӻ�Ӧ���ź�
	
	IIC_Stop();									// 6.����ֹͣ�ź�
}



/****************************************************************************
**	�������ƣ�BH1750_Read_Data()
**	�������ܣ�BH1750 ��ȡ����
**	����˵����
**			uint8_t addr ���Ĵ�����ַ
**
**	����ֵ��RX_Data
**	IIC����˵����
**				I2C_GetFlagStatus��IIC_x, IIC_Flag_x��-- ����־λ�Ƿ��� 1
**
*****************************************************************************/
uint16_t BH1750_Read_Data(void)
{
    uint8_t 	H_Data;							//�� 8 λ����
	uint8_t 	L_Data;							//�� 8 λ ����
	uint16_t 	Rx_Data;						//���� 16 λ����
	
	IIC_Start();								// 1.������ʼ�ź�
	IIC_Send_Byte(BH1750_Addr_GND_REG << 1|1);	// 2.����7λ��ַ���дλ��1������
	while(!IIC_Wait_Ack());						// 3.�ȴ��ӻ�Ӧ���ź�
	
	H_Data = IIC_Read_Byte(1);					// 4.��ȡ�߰�λ������ACK
	L_Data = IIC_Read_Byte(0);					// 5.��ȡ�Ͱ�λ������NACK
	
	IIC_Stop();									// 6.����ֹͣ�ź�
    Rx_Data = ((uint16_t)H_Data << 8) + L_Data;	// 7.16λ���ݺϳ�
	
    return Rx_Data;								// 8.���ض�ȡ����
}


/**	BH1750�ϵ� **/
void BH1750_Power_ON(void)
{
	BH1750_WriteByte(BH1750_Power_ON_REG);
}

/**	BH1750�ϵ� **/
void BH1750_Power_OFF(void)
{
	BH1750_WriteByte(BH1750_Power_OFF_REG);
}

/**	BH1750���ݼĴ�����λ **/
void BH1750_RESET(void)
{
	BH1750_WriteByte(BH1750_MODULE_RESET_REG);
}

/**	BH1750��ʼ�� **/
void BH1750_Init(void)
{
	IIC_Init();
    delay_ms(100);
	BH1750_Power_OFF();
	BH1750_Power_ON();
	BH1750_RESET();
	BH1750_WriteByte(BH1750_CONTINUE_H_MODE2);
}

/**	
**	��ȡ����ǿ��
**	�ֱ���	����ǿ�ȣ���λlx��=��High Byte + Low Byte��/ 1.2  
**/
float Light_Intensity(void)
{
	return (float)(BH1750_Read_Data() / 1.2f );
}

/*************************************END***********************************************/



