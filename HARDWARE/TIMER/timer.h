#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


void TIM3_Int_Init(u16 arr,u16 psc);	
void TIM1_PWM_Init(u16 arr,u16 psc);

void TIM1_CH1_PWM_Init(u16 arr, u16 psc);
void TIM1_CH2_PWM_Init(u16 arr, u16 psc);
#endif
