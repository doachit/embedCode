#ifndef __DHT11_H
#define __DHT11_H	 
#include "sys.h"
 
typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
											
}DHT11_Data_TypeDef;
 
#define DHT11_PORT	GPIOA
#define DHT11_PIN		GPIO_Pin_8
//IO方向设置
#define DHT11_IO_IN()  Set_Pin_Input_Mode(DHT11_PORT,DHT11_PIN)
#define DHT11_IO_OUT() Set_Pin_Output_Mode(DHT11_PORT,DHT11_PIN)

////IO操作函数											   
#define	DHT11_DQ_OUT PinOut(DHT11_PORT,DHT11_PIN) //数据端口	 
#define	DHT11_DQ_IN  PinIn(DHT11_PORT,DHT11_PIN) //数据端口


u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *humiH,u8 *humiL,u8 *tempH,u8 *tempL);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11  
		 				    
#endif
