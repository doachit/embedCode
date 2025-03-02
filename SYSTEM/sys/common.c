#include "common.h"

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
