#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart.h"
#include "mlx90614.h"

#define PRECISION   1.0e-4

// 0--equal
// 1- cmp > base
// -1 cmp < base
int floatCompare(float base, float cmp);

 int main(void)
 {		
    float fTemp;
	delay_init();	    	 //延时函数初始化
    delay_ms(500);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600

    SMBus_Init();

     
   while (1)
    {
        fTemp = SMBus_ReadTemp(); //测量的温度
        printf("当前温度:%.2f\n",fTemp);
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


