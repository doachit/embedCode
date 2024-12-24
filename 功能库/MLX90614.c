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
	delay_init();	    	 //��ʱ������ʼ��
    delay_ms(500);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600

    SMBus_Init();

     
   while (1)
    {
        fTemp = SMBus_ReadTemp(); //�������¶�
        printf("��ǰ�¶�:%.2f\n",fTemp);
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


