#include "led.h"	    
//LED IO初始化
void LED_Init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure; 
    Init_GPIO_Clock(LED_PORT);
   
    GPIO_InitStructure.GPIO_Pin = LED_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LED_PORT, &GPIO_InitStructure);		
}
 
