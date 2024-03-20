#ifndef __BEEP_H
#define __BEEP_H 			   
#include "sys.h"  

#define BEEP_PORT   GPIOC
#define BEEP_PIN    GPIO_Pin_13

//��������
#define BEEP_ON     GPIO_ResetBits(BEEP_PORT,BEEP_PIN)
//�������ر�
#define BEEP_OFF    GPIO_SetBits(BEEP_PORT,BEEP_PIN);

void beep_Init(void);

#endif
