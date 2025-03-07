#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
#include "stm32f10x.h"


#define EXTI_GPIO_PORT          GPIOB
#define EXIT_PIN_NUM                 15

// 宏的间接展开技巧（解决##直接拼接不展开参数的问题）
#define _CONCAT(a, b)    a##b
#define CONCAT(a, b)     _CONCAT(a, b)   

#define EXTI_GPIO_PIN           CONCAT(GPIO_Pin_, EXIT_PIN_NUM)

     
// 自动推导相关参数
#define EXTI_PIN_NO         EXIT_PIN_NUM
#define EXTI_PORT_SOURCE       ((EXTI_GPIO_PORT == GPIOA) ? GPIO_PortSourceGPIOA : \
                            (EXTI_GPIO_PORT == GPIOB) ? GPIO_PortSourceGPIOB : \
                            GPIO_PortSourceGPIOC)
#define EXTI_PIN_SOURCE        ((uint8_t)EXTI_PIN_NO)
#define EXTI_LINE           (1 << EXTI_PIN_NO)
#define EXTI_IRQ_CHANNEL    ((EXTI_PIN_NO <= 4) ? (EXTI0_IRQn + EXTI_PIN_NO) : \
                            (EXTI_PIN_NO <= 9) ? EXTI9_5_IRQn : EXTI15_10_IRQn)

typedef void (*ExtIntCallback)(void);

// 函数声明
void ExtInt_Init(ExtIntCallback callback, EXTITrigger_TypeDef mode);
void ExtInt_EnableIT(void);
void ExtInt_DisableIT(void);
                                 
//extern int gTurnCnt;
//void EXTIX_Init(void);//IO初始化
		 					    
#endif

