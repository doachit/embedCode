#include "stdio.h"//标准输入输出库
#include "string.h"//字符串库
#include "stdlib.h"//常用的系统函数库
#include "sys.h"//系统中断分组库
#include "delay.h"//延时函数库
#include "bsp_usart.h"//串口设置库
#include "timer.h"//定时器驱动库
#include "lcd12864.h"


#include "dht11.h"
unsigned char *empty="               ";
int nPwmPeroid = 1;
int gTempThresold = 30;
int gHumiThresold = 90;

u8 motorWorkFlag;
DHT11_Data_TypeDef dht11Data;
void SYS_Init(void);


int main(void)
{
  SYS_Init();//系统初始化总函数
	Lcd_Init();	
	//KEY_Init();
	
	//若未正确连接指纹模块，将在这一步卡住，可更改这个函数使程序向下进行
 	//Shake_to_AS608();  //用来与指纹模块握手，检测指纹模块和获取一些模块的信息

	//若未正确连接AT24C02模块，将在这一步卡住，可更改这里使程序向下进行
	while(1)//检测不到24c02
	{
		LCD_Display_Words(1,1,"你\xFD好a");
		delay_ms(500);
	}
}

/**
  * @brief  系统初始化总函数
  * @param  无
  * @retval 无
  */
void SYS_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组函数
	delay_init();	    							 //延时函数初始化
    delay_ms(500);	    
	USART_Config();	 	                         //串口初始化为9600
    //LCD1602_INIT();
    //OLED_Init();
	//beep_Init();	
    //DHT11_Init(); //温湿度计控制
    //WATER_Init();   //水滴传感器
    //INS_Init();     //红外传感器
    //SWITCH_Init();  //喷药和关窗控制
	//LED_Init();  //电机控制开关，复用一下LED的

    //TIM1_PWM_Init(999,71);   //控制路灯亮度　不分频。PWM频率=72000/(899+1)=80Khz 
    //TIM3_Int_Init(99,719);
}

