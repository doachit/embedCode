#include "led.h"	    
//LED IO��ʼ��
void LED_Init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure; 
    Init_GPIO_Clock(LED_PORT);
   
    GPIO_InitStructure.GPIO_Pin = LED_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(LED_PORT, &GPIO_InitStructure);		
}
 
