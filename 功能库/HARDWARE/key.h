#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 

#define KEY_PORT    GPIOB
#define KEY_PIN1    GPIO_Pin_12
#define KEY_PIN2    GPIO_Pin_13
#define KEY_PIN3    GPIO_Pin_14
#define KEY_PIN4    GPIO_Pin_15

#define KEY0  GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN1)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN2)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN3)//读取按键2
#define KEY3  GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN4)//读取按键3

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define KEY2_PRES	3		//KEY2  
#define KEY3_PRES	4		//KEY3
void KEY_Init(void);    //IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数					    
#endif
