#include "beep.h"
#include "sys.h"
void beep_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    Init_GPIO_Clock(BEEP_PORT);
   
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);		

    BEEP_OFF;
}
 
