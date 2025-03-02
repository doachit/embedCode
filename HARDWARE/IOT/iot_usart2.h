
#ifndef __ITO_USART2_H
#define __ITO_USART2_H

#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件
#include "iot_common.h"

#define USART2_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节

#if  USART2_RX_ENABLE                          //如果使能接收功能
#define USART2_RXBUFF_SIZE   1024              //定义串口2 接收缓冲区大小 1024字节
extern char Usart2_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart2_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
#endif
extern char DMA_flag;

void Usart2_Init(unsigned int);       
void Usart2_IDELInit(unsigned int bound);
void u2_printf(char*,...) ;          
void u2_TxData(unsigned char *data);
void Usart2DMA_init(void);

#endif


