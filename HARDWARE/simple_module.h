#ifndef __SHW_H
#define __SHW_H 			   
#include "sys.h"  


//////////////////////////////////////////////////////
//蜂鸣器
#define BEEP_PORT   GPIOC
#define BEEP_PIN    GPIO_Pin_13
//蜂鸣器响
#define BEEP_ON     GPIO_ResetBits(BEEP_PORT,BEEP_PIN)
//蜂鸣器关闭
#define BEEP_OFF    GPIO_SetBits(BEEP_PORT,BEEP_PIN);

void beep_Init(void);

//////////////////////////////////////////////////////
//LED
#define LED_PORT    GPIOB
#define LED_PIN     GPIO_Pin_6
#define WIN_PIN     GPIO_Pin_6
#define LED         PinOut(LED_PORT,LED_PIN)
#define LED_ON      GPIO_SetBits(LED_PORT,LED_PIN)
#define LED_OFF     GPIO_ResetBits(LED_PORT,LED_PIN)
#define FAN_OFF     LED_OFF
#define FAN_ON      LED_ON

#define WIN_PIN_ON   GPIO_SetBits(LED_PORT,WIN_PIN)
#define WIN_PIN_OFF  GPIO_ResetBits(LED_PORT,WIN_PIN)

#define MOTOR_ON      LED_ON
#define MOTOR_OFF     LED_OFF
void LED_Init(void);//LED初始化

//////////////////////////////////////////////////////
//Key   低电平触发
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
//人体传感器    高电平触发
#define HC_PORT     GPIOB
#define HC_PIN      GPIO_Pin_7
#define HC_READ     GPIO_ReadInputDataBit(HC_PORT,HC_PIN)
#define Find_person (HC_READ == 0)
void HC_Init(void);

//////////////////////////////////////////////////////
//红外传感器 Infrared sensor    低电平触发
#define INS_PORT     GPIOB
#define INS_PIN      GPIO_Pin_8
#define INS_READ     GPIO_ReadInputDataBit(INS_PORT,INS_PIN)
#define Find_Sth    (INS_READ == 0)
void INS_Init(void);

//////////////////////////////////////////////////////
//水滴传感器 water sensor 低电平有水
#define WATER_PORT     GPIOB
#define WATER_PIN      GPIO_Pin_9
#define WATER_READ     GPIO_ReadInputDataBit(WATER_PORT,WATER_PIN)
#define Find_Water      (WATER_READ == 0)
void WATER_Init(void);

//////////////////////////////////////////////////////
//继电器
#define SWITCH_PORT      GPIOC
#define SWITCH_PIN1      GPIO_Pin_14
#define SWITCH_PIN2      GPIO_Pin_15

#define SWITCH1_ON      GPIO_SetBits(SWITCH_PORT,SWITCH_PIN1)
#define SWITCH1_OFF     GPIO_ResetBits(SWITCH_PORT,SWITCH_PIN1)

#define SWITCH2_ON      GPIO_SetBits(SWITCH_PORT,SWITCH_PIN2)
#define SWITCH2_OFF     GPIO_ResetBits(SWITCH_PORT,SWITCH_PIN2)
#define Spraying_ON     SWITCH2_ON
#define Spraying_OFF    SWITCH2_OFF
void SWITCH_Init(void);
#endif
