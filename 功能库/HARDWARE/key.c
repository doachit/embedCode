#include "key.h"
#include "delay.h"
#include "sys.h"
 	    
//按键初始化函数 
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	Init_GPIO_Clock(KEY_PORT);
    //Init_GPIO_Clock(GPIOC);
	//__HAL_RCC_GPIOA_CLK_ENABLE();
	//__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = KEY_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入

 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Pin  = KEY_PIN2;
 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin  = KEY_PIN3;
 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = KEY_PIN4;
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
	if(key_up&&(KEY0==0||KEY1==0 || KEY2 == 0 || KEY3 == 0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
        else if(KEY2 == 0) return KEY2_PRES;
        else if(KEY3 == 0) return KEY3_PRES;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1)key_up=1; 	     
	return 0;// 无按键按下
}


