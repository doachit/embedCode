#ifndef __SOUND_H
#define __SOUND_H	 
#include "delay.h"
#include "sys.h"
#include "usart.h"
 	    
extern uint8_t soundsbuf[];
extern uint8_t soundsplay[7];
extern u8 soundNumber[];
extern int sounds[][2];
enum 
{
    EMPTY = 0,
    TEMP_IS=11,
    POINT,
    TEMP_UNIT,
    TEMP_OK,
    TEMP_ABNORMAL
};
void soundsContrl(USART_TypeDef * pUSARTx,uint16_t number);
#endif
