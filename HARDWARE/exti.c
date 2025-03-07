#include "exti.h"


static ExtIntCallback userCallback;
// 初始化函数
void ExtInt_Init(ExtIntCallback callback, EXTITrigger_TypeDef mode)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 使能时钟
    INIT_GPIO_CLOCK(EXTI_GPIO_PORT);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 配置GPIO为上拉输入
    GPIO_InitStruct.GPIO_Pin = EXTI_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(EXTI_GPIO_PORT, &GPIO_InitStruct);

    // 配置EXTI
    GPIO_EXTILineConfig(EXTI_PORT_SOURCE, EXTI_PIN_SOURCE);
    EXTI_InitStruct.EXTI_Line = EXTI_LINE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = mode;//EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    // 配置NVIC
    NVIC_InitStruct.NVIC_IRQChannel = EXTI_IRQ_CHANNEL;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    userCallback = callback;
}

// 中断控制函数
void ExtInt_EnableIT(void) { EXTI->IMR |= EXTI_LINE; }
void ExtInt_DisableIT(void) { EXTI->IMR &= ~EXTI_LINE; }

//void EXTI15_10_IRQHandler(void)
//{
//    if (EXTI_GetITStatus(EXTI_LINE) != RESET) {
//        if(userCallback) userCallback();
//        EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
//    }
//}
// 统一中断处理函数
static void handle_irq(void)
{
    if (EXTI_GetITStatus(EXTI_LINE) != RESET) {
        if(userCallback) userCallback();
        EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
    }
}

// 中断向量映射（根据引脚号自动关联）
#if (EXTI_PIN_NO <= 4)
    void EXTI0_IRQHandler(void) __attribute__((alias("handle_irq")));
    void EXTI1_IRQHandler(void) __attribute__((alias("handle_irq")));
    void EXTI2_IRQHandler(void) __attribute__((alias("handle_irq")));
    void EXTI3_IRQHandler(void) __attribute__((alias("handle_irq")));
    void EXTI4_IRQHandler(void) __attribute__((alias("handle_irq")));
#elif (EXTI_PIN_NO <= 9)
    void EXTI9_5_IRQHandler(void) __attribute__((alias("handle_irq")));
#else
    void EXTI15_10_IRQHandler(void) __attribute__((alias("handle_irq")));
#endif
#if 0
//外部中断初始化函数
void EXTIX_Init(void)
{
 
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟

    // //GPIOC.5 中断线以及中断初始化配置
  	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);

  	// EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	// EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	// EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	// EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	// EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.15	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    // //GPIOA.0	  中断线以及中断初始化配置
  	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

   	// EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	// EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	// EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	// EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	// EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


 
  	// NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
  	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1
  	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	// NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
	// NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	// NVIC_Init(&NVIC_InitStructure); 
 
 
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
}

 
void EXTI0_IRQHandler(void)
{
  delay_ms(10);    //消抖

	EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
}
 void EXTI9_5_IRQHandler(void)
{			
	delay_ms(10);   //消抖			 

 	 EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位  
}


void EXTI15_10_IRQHandler(void)
{
  delay_ms(10);    //消抖			 
  if(KEY3==0) {
    if(userCallback) userCallback();
    }
	 EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
}
#endif
