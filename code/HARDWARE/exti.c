#include "exti.h"
#include "delay.h"

#include "sys.h"

//�ⲿ�жϳ�ʼ��
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //����GPIOAʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();               //����GPIOCʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_0;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_15; 				//PA15
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_5; 				//PC5
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    //�ж���0-PA0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,2);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //ʹ���ж���0
    
	//�ж���5-PC5
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,1);   	//��ռ���ȼ�Ϊ2�������ȼ�Ϊ1
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); 
	
    //�ж���15-PA15
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0); 	//��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);        	//ʹ���ж���2
}


//�жϷ�����
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);		//�����жϴ����ú���
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);		//�����жϴ����ú���
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);		//�����жϴ����ú���
}


//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(100);      	//����
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
        {printf("GPIO_PIN_0\n");}
            break;
        case GPIO_PIN_5:
            {printf("GPIO_PIN_5\n");}
            break;
        case GPIO_PIN_15:
            {printf("GPIO_PIN_15\n");}
            break;
    }
}
