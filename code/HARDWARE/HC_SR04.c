#include "HC_SR04.h"
#include "usart.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
unsigned overcount=0;//��¼��ʱ���������
void HC_SR04config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    overcount = 0;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	Init_GPIO_Clock(HC_PORT);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=TRIG_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(HC_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=ECHO_PIN;
	GPIO_Init(HC_PORT,&GPIO_InitStruct);
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 999; //������װ��ֵΪ999
	TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIM3ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx					 
}
//��ຯ��
#define MAX_CNT (5)
float get_distance(void)//��������������
{
    float length=0,sum=0;
	u16 tim;
	unsigned int i=0;
	/*��������ݼ���һ��ƽ��ֵ*/
	while(i!=MAX_CNT)
	{
		PBout(8)=1;//�����źţ���Ϊ�����ź�
		delay_us(20);//�ߵ�ƽ�źų���10us
		PBout(8)=0;//�ȴ������ź�
		while(GPIO_ReadInputDataBit(HC_PORT,ECHO_PIN)==RESET);
		TIM_Cmd(TIM3,ENABLE);//�����źŵ�����������ʱ������
		
		while(GPIO_ReadInputDataBit(HC_PORT,ECHO_PIN)==SET);//�����ź���ʧ
		TIM_Cmd(TIM3,DISABLE);//�رն�ʱ��
		tim=TIM_GetCounter(TIM3);//��ȡ��TIM2���Ĵ����еļ���ֵ
		length=(tim+overcount*1000)/58.0;//ͨ�������źż������
		sum=length+sum;
        TIM3->CNT=0;//��TIM2�����Ĵ����ļ���ֵ����
		overcount=0;//�ж������������
		delay_ms(100);
        
        i++;//ÿ�յ�һ�λ����ź�+1,�յ�5�ξͼ����ֵ
	}
	length=sum/MAX_CNT;//������Ϊ��������ֵ
	return length;
}
		
	

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)  //���TIM3�����жϷ������
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
        overcount++;
    }
}












