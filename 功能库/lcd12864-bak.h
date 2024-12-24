#ifndef __LCD12864_H
#define __LCD12864_H

#include "stm32f10x.h"


// 串口通讯需要使用的引脚
// CS片选信号 （如果只有一款屏，可以将CS直接接到VCC上，实现两线通讯。）
// SID数据传输引脚
// CLK时钟
// PSB（低电平表示串行通讯，串口通讯时可以直接接地。）
// VSS、VDD电源脚
#define CS 	GPIO_Pin_0	//片选信号
#define SID GPIO_Pin_9	//数据传输
#define CLK GPIO_Pin_8	//时钟
#define PSB GPIO_Pin_3	//串并选择 低电平为串行
#define CMD_PROT GPIOB	//GPIO分组

#define CS_HIGH GPIO_SetBits(CMD_PROT,CS)
#define CS_LOW GPIO_ResetBits(CMD_PROT,CS)
#define SID_HIGH GPIO_SetBits(CMD_PROT,SID)
#define SID_LOW GPIO_ResetBits(CMD_PROT,SID)
#define CLK_HIGH GPIO_SetBits(CMD_PROT,CLK)
#define CLK_LOW GPIO_ResetBits(CMD_PROT,CLK)
#define PSB_HIGH GPIO_SetBits(CMD_PROT,PSB)
#define PSB_LOW GPIO_ResetBits(CMD_PROT,PSB)

#define CMD 0xF8	//写指令
#define DAT 0xFA	//写数据

typedef struct{
	uint8_t com;	//地址
	uint8_t num;	//长度
	uint8_t *dat;	//数据
}LCD_InitTypeDef;	//定义写文字用的结构体

void LCD12864_UserConfig(void);/*用户配置*/
void LCD12864_Write_Init(void);/*LCD12864上电初始化流程*/
void LCD12864_Write_CMD_Data(uint8_t cmd,uint8_t data);/*LCD12864写函数 传入指令写指令，传入数据写数据*/
void LCD12864_Write_Word(LCD_InitTypeDef LCD_InitStructure);/*LCD12864写文字*/
void LCD12864_Write_Number(uint8_t com,uint8_t num);/*LCD12864写数字*/
void LCD12864_Open(void);/*屏幕开显示*/
void LCD12864_Shut(void);/*屏幕关显示，进入睡眠*/

#endif

