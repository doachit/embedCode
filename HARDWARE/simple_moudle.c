#include "simple_module.h"
#include "sys.h"
#include "delay.h"

void beep_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    INIT_GPIO_CLOCK(BEEP_PORT);
   
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);		
    BEEP_OFF;
}
 
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    INIT_GPIO_CLOCK(LED_PORT);	
    GPIO_InitStructure.GPIO_Pin = LED_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

void SWITCH_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    INIT_GPIO_CLOCK(SWITCH_PORT);	
    GPIO_InitStructure.GPIO_Pin = SWITCH_PIN1|SWITCH_PIN2;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(SWITCH_PORT, &GPIO_InitStructure);
}

//人体传感器
void HC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    INIT_GPIO_CLOCK(HC_PORT);	
    GPIO_InitStructure.GPIO_Pin = HC_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //下拉输入
    GPIO_Init(HC_PORT, &GPIO_InitStructure); 
}

//红外传感器
void INS_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    INIT_GPIO_CLOCK(INS_PORT);	
    GPIO_InitStructure.GPIO_Pin = INS_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //下拉输入
    GPIO_Init(INS_PORT, &GPIO_InitStructure); 
}

//水滴传感器
void WATER_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    INIT_GPIO_CLOCK(WATER_PORT);	
    GPIO_InitStructure.GPIO_Pin = WATER_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //下拉输入
    GPIO_Init(WATER_PORT, &GPIO_InitStructure); 
}

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    INIT_GPIO_CLOCK(KEY_PORT);	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = KEY_PIN1|KEY_PIN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = KEY_PIN3|KEY_PIN4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0 || KEY4==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
		else if(KEY4==0)return KEY4_PRES;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	     
	return 0;// 无按键按下
}
