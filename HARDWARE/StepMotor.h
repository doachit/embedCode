#ifndef __STEP_MOTOR_H
#define __STEP_MOTOR_H	 
#include "sys.h"
#include "delay.h"
 
extern u8 STEP; //定义单步计数 全局变量
 
 
#define STEP_MOTOR_PORT			GPIOB	//定义IO接口所在组
#define STEP_MOTOR_A				GPIO_Pin_12	//定义IO接口
#define STEP_MOTOR_B				GPIO_Pin_13	//定义IO接口
#define STEP_MOTOR_C				GPIO_Pin_14	//定义IO接口
#define STEP_MOTOR_D				GPIO_Pin_15	//定义IO接口
#define STEP_MOTOR_CLK      RCC_APB2Periph_GPIOB
 
 
 
void STEP_MOTOR_Init(void);//初始化
void STEP_MOTOR_OFF (void);//断电状态
void STEP_MOTOR_8A (u8 a,u16 speed);
void STEP_MOTOR_NUM (u8 RL,u16 num,u8 speed);//电机按步数运行

// 函数第一个参数：控制正反转 1→正转   0→反转
//    第二个参数：控制电机转的圈数
//    第三个参数：延时

void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u8 speed);//电机按圈数运行
 
#endif