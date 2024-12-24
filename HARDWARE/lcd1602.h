#ifndef __LCD1602_H
#define __LCD1602_H

#include "sys.h"

#define LCD_CONTROL_PORT GPIOB
#define LCD_RS_PIN GPIO_Pin_10
#define LCD_RW_PIN GPIO_Pin_1
#define LCD_EN_PIN GPIO_Pin_0

#define LCD_RS  PinOut(LCD_CONTROL_PORT,LCD_RS_PIN)//PBout(10)
#define LCD_RW  PinOut(LCD_CONTROL_PORT,LCD_RW_PIN)//PBout(1)
#define LCD_EN  PinOut(LCD_CONTROL_PORT,LCD_EN_PIN)//PBout(0)

#define LCD_DATA_PORT   GPIOA
#define LCD_DATA_PIN0 GPIO_Pin_7
#define LCD_DATA_PIN1 GPIO_Pin_6
#define LCD_DATA_PIN2 GPIO_Pin_5
#define LCD_DATA_PIN3 GPIO_Pin_4
#define LCD_DATA_PIN4 GPIO_Pin_3
#define LCD_DATA_PIN5 GPIO_Pin_2
#define LCD_DATA_PIN6 GPIO_Pin_1
#define LCD_DATA_PIN7 GPIO_Pin_0
#define LCD_D0  PinOut(LCD_DATA_PORT,LCD_DATA_PIN0)//PAout(7)
#define LCD_D1  PinOut(LCD_DATA_PORT,LCD_DATA_PIN1)//PAout(6)
#define LCD_D2  PinOut(LCD_DATA_PORT,LCD_DATA_PIN2)//PAout(5)
#define LCD_D3  PinOut(LCD_DATA_PORT,LCD_DATA_PIN3)//PAout(4)
#define LCD_D4  PinOut(LCD_DATA_PORT,LCD_DATA_PIN4)//PAout(3)
#define LCD_D5  PinOut(LCD_DATA_PORT,LCD_DATA_PIN5)//PAout(2)
#define LCD_D6  PinOut(LCD_DATA_PORT,LCD_DATA_PIN6)//PAout(1)
#define LCD_D7  PinOut(LCD_DATA_PORT,LCD_DATA_PIN7)//PAout(0)

void LCD1602_WriteCMD(uint8_t CMD);
void LCD1602_WriteDAT(uint8_t DAT); 
void LCD1602_Clear(void);
void LCD1602_INIT(void);
void LCD1602_SetPoint(uint8_t X,uint8_t Y);
void LCD1602_Write_Char(uint8_t X,uint8_t Y,uint8_t chr);
void LCD1602_Write_Str(uint8_t X,uint8_t Y,uint8_t *str);


#endif

