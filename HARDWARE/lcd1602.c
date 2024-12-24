#include "lcd1602.h"
#include "delay.h"

/* LCD1602写命令 */
void LCD1602_WriteCMD(uint8_t CMD)
{
	LCD_EN = 0;
	LCD_RS = 0;       /* 输入指令 */
	LCD_RW = 0;       /* 向LCD写入指令或者数据 */
	delay_ms(1);		 
	LCD_D0 = ( (CMD & 0x01) >> 0);
	LCD_D1 = ( (CMD & 0x02) >> 1);
	LCD_D2 = ( (CMD & 0x04) >> 2);
	LCD_D3 = ( (CMD & 0x08) >> 3);
	LCD_D4 = ( (CMD & 0x10) >> 4);
	LCD_D5 = ( (CMD & 0x20) >> 5);
	LCD_D6 = ( (CMD & 0x40) >> 6);
	LCD_D7 = ( (CMD & 0x80) >> 7);
	delay_ms(1);	
	LCD_EN = 1;       /* 使能 */
	delay_ms(1);		
	LCD_EN = 0;
}

/* LCD1602写数据 */
void LCD1602_WriteDAT(uint8_t DAT)
{
	LCD_RS = 1;       /* 输入数据 */
	LCD_RW = 0;       /* 向LCD写入指令或者数据 */
	LCD_EN = 0;

	
	LCD_D0 = ( (DAT & 0x01) >> 0);
	LCD_D1 = ( (DAT & 0x02) >> 1);
	LCD_D2 = ( (DAT & 0x04) >> 2);
	LCD_D3 = ( (DAT & 0x08) >> 3);
	LCD_D4 = ( (DAT & 0x10) >> 4);
	LCD_D5 = ( (DAT & 0x20) >> 5);
	LCD_D6 = ( (DAT & 0x40) >> 6);
	LCD_D7 = ( (DAT & 0x80) >> 7);
	delay_ms(1);	
	LCD_EN = 1;       /* 使能 */
	delay_ms(1);		
	LCD_EN = 0;
}

/* LCD1602清屏 */
void LCD1602_Clear(void)
{
	LCD1602_WriteCMD(0x01);
}

/* LCD1602 设置坐标 */
void LCD1602_SetPoint(uint8_t X,uint8_t Y)
{
	if(X == 0) LCD1602_WriteCMD(0x80+Y);
	if(X == 1) LCD1602_WriteCMD(0x80+0x40+Y);	
}

/* LCD1602 写字符 */
void LCD1602_Write_Char(uint8_t X,uint8_t Y,uint8_t chr)
{
	LCD1602_SetPoint(X,Y);
	LCD1602_WriteDAT(chr);
}

/* LCD1602显示字符串 */
void LCD1602_Write_Str(uint8_t X,uint8_t Y,uint8_t *str)
{
	LCD1602_SetPoint(X,Y);
	while(*str)
	{
		LCD1602_WriteDAT(*str);
		str ++;
	}
}

/* LCD1602初始化函数 */
void LCD1602_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	INIT_GPIO_CLOCK(LCD_DATA_PORT);
	INIT_GPIO_CLOCK(LCD_CONTROL_PORT);
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTB时钟
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN;                 // 设置引脚号
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            // 设置GPIO的模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           // 设置IO口的速度
 	GPIO_Init(LCD_CONTROL_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = LCD_DATA_PIN0 | LCD_DATA_PIN1 | LCD_DATA_PIN2 | LCD_DATA_PIN3 | LCD_DATA_PIN4 | LCD_DATA_PIN5 | LCD_DATA_PIN6 | LCD_DATA_PIN7;                 // 设置引脚号
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            // 设置GPIO的模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           // 设置IO口的速度
 	GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure); 
	delay_ms(20);	
	
	LCD1602_WriteCMD(0x38);	     // 设置16x2显示，5x7点阵，8位数据接口
  	LCD1602_WriteCMD(0x0c);      // 开显示，显示光标，光标不闪烁
	LCD1602_WriteCMD(0x06);      // 写一个字符后指针加一，屏幕不移动
	LCD1602_WriteCMD(0x01);      // 清屏显示
	LCD1602_WriteCMD(0x80);      // 设置数据地址指针起点   
}
