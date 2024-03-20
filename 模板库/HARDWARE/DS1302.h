#ifndef DS1302_H
#define DS1302_H
#include "sys.h"

//DS1302地址定义
#define DS1302_SEC_ADDR           0x80		//秒数据地址
#define DS1302_MIN_ADDR           0x82		//分数据地址
#define DS1302_HOUR_ADDR          0x84		//时数据地址
#define DS1302_DAY_ADDR           0x86		//日数据地址
#define DS1302_MONTH_ADDR         0x88		//月数据地址
#define DS1302_WEEK_ADDR          0x8a		//星期数据地址
#define DS1302_YEAR_ADDR          0x8c		//年数据地址
#define DS1302_CONTROL_ADDR       0x8e		//控制数据地址
#define DS1302_CHARGER_ADDR       0x90 		//充电功能地址			 
#define DS1302_CLKBURST_ADDR      0xbe


#define DS1302_IN  PAin(2)
#define DS1302_OUT PAout(2)
#define DS1302_RST PAout(0)
#define DS1302_CLK PAout(1)

#define RST_GPIO	GPIOA
#define RST_Pin		GPIO_Pin_0

#define SCLK_GPIO	GPIOA
#define SCLK_Pin	GPIO_Pin_1

#define DATA_GPIO	GPIOA
#define DATA_Pin	GPIO_Pin_2

struct TIMEData
{
	u16 year;
	u8  month;
	u8  day;
	u8  hour;
	u8  minute;
	u8  second;
	u8  week;
};
extern struct TIMEData TimeData;
extern u8 readtime[15];
extern u16 time_buf[8];
void DS1302_Init(void);
void DS1302_WriteByte(u8 addr,u8 data);
u8   DS1302_ReadByte(u8 addr);
void DS1302_WriteTime(void);
void DS1302_ReadTime(void);
void DS1302_GetTime(void);
 
#endif

