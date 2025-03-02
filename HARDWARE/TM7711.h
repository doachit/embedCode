#ifndef __TM7711_H
#define __TM7711_H	 
#include "sys.h"
 
// 硬件定义
#define TM7711_SCLK_PORT    GPIOB
#define TM7711_SCLK_PIN     GPIO_Pin_6
#define TM7711_DOUT_PORT    GPIOB
#define TM7711_DOUT_PIN     GPIO_Pin_7

// 引脚定义（根据实际接线修改）
#define HX710B_ADDO_GPIO_PORT    GPIOB
#define HX710B_ADDO_GPIO_PIN     GPIO_Pin_7
#define HX710B_ADSK_GPIO_PORT    GPIOB
#define HX710B_ADSK_GPIO_PIN     GPIO_Pin_6

// 快速引脚操作宏（比库函数快5倍）
#define HX710B_ADSK_HIGH()       HX710B_ADSK_GPIO_PORT->BSRR = HX710B_ADSK_GPIO_PIN
#define HX710B_ADSK_LOW()        HX710B_ADSK_GPIO_PORT->BRR  = HX710B_ADSK_GPIO_PIN
#define HX710B_ADDO_READ()       (HX710B_ADDO_GPIO_PORT->IDR & HX710B_ADDO_GPIO_PIN)

int32_t HX710B_ReadCount(uint32_t timeout_ms);

void HX710B_GPIO_Init(void);
#endif
