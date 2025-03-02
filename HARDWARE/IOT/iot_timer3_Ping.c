/*
	* @Author       : LaiGuo
	* @Date         : 2024-10-13 13:40:34
	* @LastEditTime : 2024-10-13 15:32:57
	* @CSDN  				: https://blog.csdn.net/m0_58326153/
	* @Bili  				: https://b23.tv/llaF6l2
*/
#include "iot_timer3_Ping.h"     //包含需要的头文件
int SystemTimer =0;                 //用户上传数据的时间间隔（全局变量）
unsigned int Length_of_time = 3000;	//PING报文的时间间隔变量（全局变量）
/*
	* @name   TIM3_ENABLE_10MS
	* @brief  初始化定时器3，定时10ms
	* @param  [None]
	* @return [None]
*/
void TIM3_ENABLE_10MS(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           //使能TIM3时钟	
	TIM_DeInit(TIM3);                                             //定时器3寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 100-1; 	          //设置自动重装载值        60000-1
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;              //设置定时器预分频数    36000-1
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);            //设置TIM3
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                      //使能TIM3溢出中断    
	TIM_Cmd(TIM3,ENABLE);                                         //开TIM3                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;                 //设置TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}

/*
	* @name   TIM3_IRQHandler
	* @brief  初始化定时器3，定时10ms
	* @param  [None]
	* @return [None]
*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{   //如果TIM_IT_Update置位，表示TIM3溢出中断，进入if	
		static unsigned int time_temp= 0;
		time_temp++;
		if(time_temp>Length_of_time) //10*3000 =30S进入一次30SPing一次
		{
			time_temp = 0;
			switch(Ping_flag)
			{                               //判断Ping_flag的状态
				case 0:										 //如果Ping_flag等于0，表示正常状态，发送Ping报文  
						Mqtt_PingMessage_To_Buff(); 					 //添加Ping报文到发送缓冲区  
						break;
				case 1:										 //如果Ping_flag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
				
						Mqtt_PingMessage_To_Buff();  					 //添加Ping报文到发送缓冲区  
						break;
				case 2:			Mqtt_PingMessage_To_Buff();								break;	 //如果Ping_flag等于2，说明还没有收到服务器回复
				case 3:			Mqtt_PingMessage_To_Buff();		            	break;             //如果Ping_flag等于3，说明还没有收到服务器回复
				case 4:				Length_of_time = 200;					 //我们将定时器6设置为2s定时,快速发送Ping报文        //如果Ping_flag等于4，说明还没有收到服务器回复	
						Mqtt_PingMessage_To_Buff();  					 //添加Ping报文到发送缓冲区 
						break;
				case 5:										 //如果Ping_flag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
						ConnectALi_flag = 0;                    //连接状态置0，表示断开，没连上服务器
						TIM_Cmd(TIM3,DISABLE);               //关TIM3 				
						break;			
			}
			Ping_flag++;           		             		 //Ping_flag自增1，表示又发送了一次ping，期待服务器的回复
		}	
		SystemTimer++; //发送数据的定时器
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //清除TIM3溢出中断标志 	
	}
}
