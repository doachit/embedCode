#include "bsp_usart.h"

// 串口对象实例
USART_Object USART1_Obj = {0};
USART_Object USART2_Obj = {0};
USART_Object USART3_Obj = {0};

static const USART_HWConfig USART_Configs[] = {
    {   // USART1: 最高优先级
        USART1, GPIOA, GPIO_Pin_9, GPIO_Pin_10,
        RCC_APB2Periph_GPIOA, RCC_APB2Periph_USART1,
        USART1_IRQn, 0  // 抢占优先级0（最高）
    },
    {   // USART2: 中等优先级
        USART2, GPIOA, GPIO_Pin_2, GPIO_Pin_3,
        RCC_APB2Periph_GPIOA, RCC_APB1Periph_USART2,
        USART2_IRQn, 1  // 抢占优先级1
    },
    {   // USART3: 最低优先级
        USART3, GPIOB, GPIO_Pin_10, GPIO_Pin_11,
        RCC_APB2Periph_GPIOB, RCC_APB1Periph_USART3,
        USART3_IRQn, 2  // 抢占优先级2
    }
};

static USART_Object* GetUSARTObj(USART_TypeDef* USARTx) {
    int i;
    for(i=0; i<sizeof(USART_Configs)/sizeof(USART_Configs[0]); i++) {
        if(USART_Configs[i].USARTx == USARTx) {
            switch((uint32_t)USARTx) {
                case USART1_BASE: return &USART1_Obj;
                case USART2_BASE: return &USART2_Obj;
                case USART3_BASE: return &USART3_Obj;
            }
        }
    }
    return NULL;
}

static void NVIC_Configuration(const USART_HWConfig* config) {
	NVIC_InitTypeDef NVIC_InitStructure;
  
	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = config->IRQn;
	/* 抢断优先级*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = config->PreemptPriority;
	/* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void GPIO_Configuration(const USART_HWConfig* config) {
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_APB2PeriphClockCmd(config->GPIO_CLK, ENABLE);

    // TX引脚配置
    GPIO_InitStruct.GPIO_Pin = config->TxPin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(config->GPIOx, &GPIO_InitStruct);

    // RX引脚配置
    GPIO_InitStruct.GPIO_Pin = config->RxPin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(config->GPIOx, &GPIO_InitStruct);
}

void USART_Module_Init(USART_TypeDef* USARTx, uint32_t baudrate, FunctionalState interrupt)
{
    const USART_HWConfig* hw_config = NULL;
    USART_Object* obj = NULL;
        // 串口参数配置
    USART_InitTypeDef USART_InitStruct;
    int i;
    // 查找硬件配置
    for(i=0; i<sizeof(USART_Configs)/sizeof(USART_Configs[0]); i++) {
        if(USART_Configs[i].USARTx == USARTx) {
            hw_config = &USART_Configs[i];
            obj = GetUSARTObj(USARTx);
            break;
        }
    }
    
    if(!hw_config || !obj) return;

    // 初始化对象参数
    obj->USARTx = USARTx;
    obj->baudrate = baudrate;
    obj->interrupt = interrupt;
    obj->ReCount = 0;
    obj->Flag = 0;

    // 时钟使能
    if(hw_config->USART_CLK == RCC_APB2Periph_USART1) {
        RCC_APB2PeriphClockCmd(hw_config->USART_CLK | hw_config->GPIO_CLK, ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(hw_config->USART_CLK, ENABLE);
        RCC_APB2PeriphClockCmd(hw_config->GPIO_CLK, ENABLE);
    }

    // GPIO初始化
    GPIO_Configuration(hw_config);

	// 配置串口的工作参数
	// 配置波特率
	USART_InitStruct.USART_BaudRate = baudrate;
	// 配置 针数据字长
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStruct.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStruct.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
    USART_Init(USARTx, &USART_InitStruct);
    if(interrupt) {
        NVIC_Configuration(hw_config); // 应用优先级配置
    }

	USART_ClearFlag(USARTx,USART_FLAG_RXNE);
	// 使能串口接收中断
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(USARTx, ENABLE);
    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);    
}
void USART_IRQ_Handler(USART_Object* obj)
{
	u8 clear=clear;//这个变量用于清除IDLE中断，下面会做解释

	USART_ClearFlag(obj->USARTx,USART_FLAG_TC);

	if(USART_GetITStatus(obj->USARTx,USART_IT_RXNE)!=Bit_RESET)        
	{
		obj->Rebuff[obj->ReCount++]=obj->USARTx->DR;
		USART_ClearITPendingBit(obj->USARTx, USART_IT_RXNE);
	//通过接受中断，将发送的数据一个一个得存放在数组里		   
	}
	else if(USART_GetFlagStatus(obj->USARTx,USART_FLAG_IDLE) != Bit_RESET)
	{//当我们发完了一帧数据后，该中断就会产生
		clear=obj->USARTx->SR;
		clear=obj->USARTx->DR;//这两行代码在作用上完全类似于   USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//查看手册，对应寄存器可以知道，当我们依次读SR,DR寄存器之后，这个标志位就会自动清除。	
		obj->Rebuff[obj->ReCount++] = '\0';
		obj->Flag = 1;
		if(obj->Process) obj->Process(obj->Rebuff, obj->ReCount);
		Usart_SendArray(obj->USARTx,obj->Rebuff, obj->ReCount);
		obj->ReCount=0;//这个为接收数据数组的计数数据，这里清零后，方便下一次接收数据
		//如果没有这个清零，下次接收就会出问题，
		//比如第一次我们接收了5个数据，分别对   rece_buffer的0  1  2  3  4  放了数据，当我们第二次接收数据，因为RxCounter
		//并未清零，会继续给RxCounter的 5  6  7  .........赋值，如此会对我们的数据处理造成很大影响    
		//printf ("---%s\r\n",Usart1_Receive.Rebuf);//此代码只是为了验证与转换后的结果相同          
	}
}   		

void USART1_IRQHandler(void) { USART_IRQ_Handler(&USART1_Obj); }
void USART2_IRQHandler(void) { USART_IRQ_Handler(&USART2_Obj); }
void USART3_IRQHandler(void) { USART_IRQ_Handler(&USART3_Obj); }

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}
void USART_RegisterCallback(USART_TypeDef* USARTx, USART_Callback callback)
{
    USART_Object* obj = GetUSARTObj(USARTx);
    obj->Process = callback;
}
