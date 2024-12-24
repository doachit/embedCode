#ifndef __lcd12864_H_
#define __lcd12864_H_

#include "sys.h"
//引脚连接  （只需2根数据线即可完成通讯）
//（1）VSS——GND
//（2）VDD——VCC(5V)
//（3）V0 亮度调节（也可接滑动变阻器调压或者接5v）
//（4）CS/RS ——接VCC，持续高电平，一直选通。
//（5）R/W(SID) ——接PB9
//（6）E (SCLK)——接PB8
// (15) PSB----接GND
//（19）BLA——VCC(5V)
//（20）BLK——接GND  

#define WRITE_CMD	0xF8//写命令  
#define WRITE_DAT	0xFA//写数据

//接口(SID: PB9  SCLK: PB8)
//修改LCD接口在这里修改即可
#define LCD_SID_port   	GPIOB
#define LCD_SID_pin   	GPIO_Pin_9
#define LCD_SCLK_port   GPIOB
#define LCD_SCLK_pin   	GPIO_Pin_8
#define SID PinOut(LCD_SID_port,LCD_SID_pin)//PBout(9)
#define SCLK PinOut(LCD_SCLK_port,LCD_SCLK_pin)//PBout(8)


void Lcd_Init(void);  //主函数调用这一个函数进行初始化
void int2str(int n, uint8_t *str); //整形转字符

void lcd_GPIO_init(void);
void SendByte(u8 Dbyte);
void LCD_Clear(void);
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str); //显示汉字 直接写汉字 例如 LCD_Display_Words(0,0,"哇哇哇哇哇");
void LCD_Display_Picture(uint8_t *img);

void Lcd_WriteData(u8 Dat );
void Lcd_WriteCmd(u8 Cmd );

#endif
