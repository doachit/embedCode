#include "beep.h"
#include "sys.h"
void beep_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    Init_GPIO_Clock(BEEP_PORT);
   
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);		

    BEEP_OFF;
}
 
