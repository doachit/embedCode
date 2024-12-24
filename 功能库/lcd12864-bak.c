#include "lcd12864.h"
#include "sys.h"

/*用户配置*/
void LCD12864_UserConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin	= CS|SID|CLK|PSB;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(CMD_PROT,&GPIO_InitStructure);
}

/*LCD12864写一个字节*/
void LCD12864_Write_Byte(uint8_t data)
{
	uint8_t WBi;
	CS_HIGH;	//写之前拉高片选信号
	for(WBi=0;WBi<8;WBi++)
	{
		(data<<WBi&0x80)?SID_HIGH:SID_LOW;	//移位，判断高位
	
		CLK_LOW;	//拉低时钟
		CLK_HIGH;	//拉高时钟
	}
	CS_LOW;		//释放片选，不再占据总线
}
	
/*LCD12864写函数 传入指令写指令，传入数据写数据*/
void LCD12864_Write_CMD_Data(uint8_t cmd,uint8_t data)
{
	LCD12864_Write_Byte(cmd);
	LCD12864_Write_Byte(data&0xf0);			//根据时序图，传入数据的高4位变成发送数据的高4位，发送的数据低4位为0。例0x38 0011 1000 & 1111 0000
	LCD12864_Write_Byte((data<<4)&0xf0);	//再取传入数据的低4位，变成发送数据的高4位，发送的数据低4位为0。例0x38<<4 1000 & 1111 0000
	delay_ms(2);	//设置传输延时
}

/*LCD12864上电初始化流程*/
void LCD12864_Write_Init(void)
{
	PSB_LOW;								//设置串行通讯
	LCD12864_Write_CMD_Data(CMD,0x30);		//功能设定，8位MCU接口，基本指令集
	LCD12864_Write_CMD_Data(CMD,0x06);		//显示数据的读写时，设定游标方向右移，DDRAM地址计数器加1
	LCD12864_Write_CMD_Data(CMD,0x0C);		//开显示，关闭光标
	LCD12864_Write_CMD_Data(CMD,0x01);		//清除显示
	LCD12864_Write_CMD_Data(CMD,0x80);		//设置启动位置
}
	
/*屏幕开显示*/
void LCD12864_Open(void)
{
	LCD12864_Write_CMD_Data(CMD,0x0C);	
}

/*屏幕关显示，进入睡眠*/
void LCD12864_Shut(void)
{
	LCD12864_Write_CMD_Data(CMD,0x08);		//进入睡眠模式
}

/*LCD12864写文字*/
void LCD12864_Write_Word(LCD_InitTypeDef LCD_InitStructure)
{
	uint8_t LWWi;
	LCD12864_Write_CMD_Data(CMD,LCD_InitStructure.com);		//写指令:写入地址
	
	for(LWWi=0;LWWi<LCD_InitStructure.num;LWWi++)
	{
		LCD12864_Write_CMD_Data(DAT,*LCD_InitStructure.dat++);	//根据数据长度写入字节
		delay_ms(200);
	}
}
	
/*LCD12864写数字*/
void LCD12864_Write_Number(uint8_t com,uint8_t num)
{
	const uint8_t number[] = {"0123456789"};
	LCD12864_Write_CMD_Data(CMD,com);					//写地址
	
	LCD12864_Write_CMD_Data(DAT,number[num/1000]);		//1234 1
	LCD12864_Write_CMD_Data(DAT,number[num%1000/100]);	//1234 2
	LCD12864_Write_CMD_Data(DAT,number[num%100/10]);	//1234 3
	LCD12864_Write_CMD_Data(DAT,number[num%100%10]);	//1234 4
}

