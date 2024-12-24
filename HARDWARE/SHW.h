#ifndef __SHW_H
#define __SHW_H 			   
#include "sys.h"  


//////////////////////////////////////////////////////
//蜂鸣器
#define BEEP_PORT   GPIOB
#define BEEP_PIN    GPIO_Pin_10
//蜂鸣器响
#define BEEP_ON     GPIO_ResetBits(BEEP_PORT,BEEP_PIN)
//蜂鸣器关闭
#define BEEP_OFF    GPIO_SetBits(BEEP_PORT,BEEP_PIN);

void beep_Init(void);

//////////////////////////////////////////////////////
//LED
#define LED_PORT    GPIOC
#define LED_PIN     GPIO_Pin_13
#define LED         PinOut(LED_PORT,LED_PIN)
#define MOTOR_ON      GPIO_SetBits(LED_PORT,LED_PIN)
#define MOTOR_OFF     GPIO_ResetBits(LED_PORT,LED_PIN)

void LED_Init(void);//LED初始化

//////////////////////////////////////////////////////
//Key
#define KEY_PORT    GPIOB
#define KEY_PIN1    GPIO_Pin_12
#define KEY_PIN2    GPIO_Pin_13
#define KEY_PIN3    GPIO_Pin_12
#define KEY_PIN4    GPIO_Pin_12
#define KEY1    GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN1)//读取按键1
#define KEY2    GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN2)//读取按键2
#define KEY3    GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN3)//读取按键3 
#define KEY4    GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN4)//读取按键4 

#define KEY1_PRES	1		//KEY1 
#define KEY2_PRES	2		//KEY2 
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

void KEY_Init(void);    //IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数		
//////////////////////////////////////////////////////
//人体传感器
#define HC_PORT     GPIOB
#define HC_PIN      GPIO_Pin_11
#define HC_READ     GPIO_ReadInputDataBit(HC_PORT,HC_PIN)
void HC_Init(void);

//////////////////////////////////////////////////////
//红外传感器 Infrared sensor
#define INS_PORT     GPIOB
#define INS_PIN      GPIO_Pin_11
#define INS_READ     GPIO_ReadInputDataBit(INS_PORT,INS_PIN)
void INS_Init(void);

//////////////////////////////////////////////////////
//水滴传感器 water sensor
#define WATER_PORT     GPIOB
#define WATER_PIN      GPIO_Pin_11
#define WATER_READ     GPIO_ReadInputDataBit(WATER_PORT,WATER_PIN)
void WATER_Init(void);

//////////////////////////////////////////////////////
//继电器
#define SWITCH_PORT     GPIOB
#define SWITCH_PIN1      GPIO_Pin_11
#define SWITCH_PIN2      GPIO_Pin_11

#define SWITCH1_ON     GPIO_SetBits(SWITCH_PORT,SWITCH_PIN1)
#define SWITCH1_OFF     GPIO_ResetBits(SWITCH_PORT,SWITCH_PIN1)
#define SWITCH2_ON     GPIO_SetBits(SWITCH_PORT,SWITCH_PIN2)
#define SWITCH2_OFF     GPIO_ResetBits(SWITCH_PORT,SWITCH_PIN2)

void SWITCH_Init(void);
#endif
