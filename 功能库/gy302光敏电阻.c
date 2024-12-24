#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "GY302.h"
#include "GY302_IIC.h"



 int main(void)
 {		
	delay_init();	    	 //延时函数初始化
    delay_ms(500);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
    BH1750_Init();				// BH1750初始化

	printf("Test\r\n");

    while (1)
    {
		printf("光照强度:%f\r\n",Light_Intensity());
		delay_ms(500);
    }
    return 0;
}
