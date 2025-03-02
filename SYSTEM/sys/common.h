#ifndef __COMMON_H
#define __COMMON_H	
#include "stm32f10x.h"
 
	 
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����
//-------------------------------------------------------------------------
//Ϊ�˱����������ƥ���������⣬������½ӿڽ���������Ʒ�װ�������һ���µĽӿ�

//��PINת��Ϊ�������֣�����PIN_12ת��Ϊ12
#define PIN2NUMBER(pin) (31 - __builtin_clz(pin))

#define PORT_ODR_Addr(port)    (((uint32_t)port)+12)
#define PORT_IDR_Addr(port)    (((uint32_t)port)+8)

//��ĳ����������Ϊ��������÷���PinOut(GPIOA,GPIO_PIN1)��A1��������Ϊ���
#define PinOut(port,pin) BIT_ADDR(PORT_ODR_Addr(port),PIN2NUMBER(pin))
#define PinIn(port,pin) BIT_ADDR(PORT_IDR_Addr(port),PIN2NUMBER(pin))

#define INIT_GPIO_CLOCK(GPIOx) \
    if (GPIOx == GPIOA) { \
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); \
    } else if (GPIOx == GPIOB) { \
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); \
    } else if (GPIOx == GPIOC) { \
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); \
    } else if (GPIOx == GPIOD) { \
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); \
    }

//-----------------------------------------------------------------------------
 
#define PRECISION   1.0e-4
// 0--equal
// 1- cmp > base
// -1 cmp < base
int floatCompare(float base, float cmp);
void Init_GPIO_Clock(GPIO_TypeDef* GPIOx);
#endif
