#include "HC_SR04.h"
#include "usart.h"
#include "delay.h"


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
unsigned overcount=0;//记录定时器溢出次数
void HC_SR04config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    overcount = 0;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	Init_GPIO_Clock(HC_PORT);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=TRIG_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(HC_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=ECHO_PIN;
	GPIO_Init(HC_PORT,&GPIO_InitStruct);
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 999; //设置重装载值为999
	TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIM3时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, DISABLE);  //使能TIMx					 
}
//测距函数
#define MAX_CNT (5)
float get_distance(void)//单精度数据类型
{
    float length=0,sum=0;
	u16 tim;
	unsigned int i=0;
	/*测五次数据计算一次平均值*/
	while(i!=MAX_CNT)
	{
		PBout(8)=1;//拉高信号，作为触发信号
		delay_us(20);//高电平信号超过10us
		PBout(8)=0;//等待回响信号
		while(GPIO_ReadInputDataBit(HC_PORT,ECHO_PIN)==RESET);
		TIM_Cmd(TIM3,ENABLE);//回响信号到来，开启定时器计数
		
		while(GPIO_ReadInputDataBit(HC_PORT,ECHO_PIN)==SET);//回响信号消失
		TIM_Cmd(TIM3,DISABLE);//关闭定时器
		tim=TIM_GetCounter(TIM3);//获取计TIM2数寄存器中的计数值
		length=(tim+overcount*1000)/58.0;//通过回响信号计算距离
		sum=length+sum;
        TIM3->CNT=0;//将TIM2计数寄存器的计数值清零
		overcount=0;//中断溢出次数清零
		delay_ms(100);
        
        i++;//每收到一次回响信号+1,收到5次就计算均值
	}
	length=sum/MAX_CNT;//距离作为函数返回值
	return length;
}
		
	

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)  //检查TIM3更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
        overcount++;
    }
}












