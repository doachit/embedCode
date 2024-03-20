#ifndef _HC_SR04_H
#define _HC_SR04_H

#include "sys.h"
#define HC_PORT GPIOB
#define TRIG_PIN    GPIO_Pin_8
#define ECHO_PIN    GPIO_Pin_9

#define TRIG_Send  PBout(8)
#define ECHO_Reci  PBin(9)

float get_distance(void);
void  HC_SR04config(void);
 




#endif


