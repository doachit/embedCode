#ifndef __USART_H
#define	__USART_H

#include "sys.h"
#include "stm32f10x.h"
#include <stdio.h>
#define  DEBUG_USARTx                   USART1

#define RBUF_MAX    50
typedef void (*USART_Callback)(uint8_t *data, uint16_t len);
typedef struct {
    void (*Process)(uint8_t *data, uint16_t len);
    uint8_t Rebuff[RBUF_MAX];
    uint16_t ReCount;
    uint8_t Flag;
    USART_TypeDef* USARTx;
    uint32_t baudrate;
    FunctionalState interrupt;
} USART_Object;

// GPIO配置结构体（新增优先级字段）
typedef struct {
    USART_TypeDef* USARTx;
    GPIO_TypeDef* GPIOx;
    uint16_t TxPin;
    uint16_t RxPin;
    uint32_t GPIO_CLK;
    uint32_t USART_CLK;
    uint8_t IRQn;
    uint8_t PreemptPriority;  // 新增抢占优先级字段
} USART_HWConfig;

void USART_Module_Init(USART_TypeDef* USARTx, uint32_t baudrate, FunctionalState interrupt);
void USART_RegisterCallback(USART_TypeDef* USARTx, USART_Callback callback);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);

#endif /* __USART_H */
