#include "sys.h"
void Init_GPIO_Clock(GPIO_TypeDef* GPIOx)
{
    if(GPIOx == GPIOA)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    }
    else if(GPIOx == GPIOB)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    }
    else if (GPIOx == GPIOC)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    }
    else if (GPIOx == GPIOD)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
    }
}
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

int floatCompare(float base, float cmp)
{
    float diff = cmp - base;
    if(-PRECISION <= diff && diff <= PRECISION)
    {
        return 0;
    }else if(diff > PRECISION)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
void Set_Pin_Output_Mode(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	INIT_GPIO_CLOCK(GPIOx);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
void Set_Pin_Input_Mode(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	INIT_GPIO_CLOCK(GPIOx);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		  
 	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
