#ifndef __ITO_WIFI_MQTT_H
#define __ITO_WIFI_MQTT_H

#include "sys.h"       //包含需要的头文件

/*--------------是否开启串口调试模式---------------------------------*/
#define mqtt_debug    0   //1 开启调试  0关闭调试
#define IotDebug_prinf(...) printf(__VA_ARGS__) //调试的串口函数可更换为自己的

/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
#define  R_NUM               7     //接收缓冲区个数
#define  RBUFF_UNIT          400   //接收缓冲区长度

#define  T_NUM               7     //发送缓冲区个数  
#define  TBUFF_UNIT          400   //发送缓冲区长度

#define  C_NUM               7     //命令缓冲区个数
#define  CBUFF_UNIT          400   //命令缓冲区长度
           
#define  MQTT_TxData(x)       u2_TxData(x)                                         //串口2负责数据发送
#define  WIFIID             "302a"    //初始化的路由器SSID名称
#define  PASSWORD           "1234abcd+-*/"       //初始化的路由器密码



#define PRODUCTKEY           "a1Bgo8sVlSl"  																			//产品ID
#define PRODUCTKEY_LEN       strlen(PRODUCTKEY)                                   //产品ID长度
#define DEVICENAME           "mytest" 																						//设备名 
#define DEVICENAME_LEN       strlen(DEVICENAME)                                   //设备名长度
#define DEVICESECRE          "35c77a1c88b49ddfe07e46ee856a6e0a"  									//设备秘钥
#define DEVICESECRE_LEN      strlen(DEVICESECRE) 																	//设备秘钥长度

// 使用简单的宏连接，但注意这里我们不能直接在宏定义中连接字符串字面量和宏  
#define S_TOPIC_PREFIX       "/sys/"  
#define S_TOPIC_MIDDLE       PRODUCTKEY "/" DEVICENAME  
#define S_TOPIC_SUFFIX       "/thing/service/property/set"  
#define S_TOPIC_NAME         S_TOPIC_PREFIX S_TOPIC_MIDDLE S_TOPIC_SUFFIX  		//需要订阅的主题 
  
// 为P_TOPIC_NAME定义  
#define P_TOPIC_PREFIX       "/sys/"  
#define P_TOPIC_MIDDLE       PRODUCTKEY "/" DEVICENAME  
#define P_TOPIC_SUFFIX       "/thing/event/property/post"  
#define P_TOPIC_NAME         P_TOPIC_PREFIX P_TOPIC_MIDDLE P_TOPIC_SUFFIX							//需要发布的主题  

extern unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];       //外部变量声明，数据的接收缓冲区,所有服务器发来的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
extern unsigned char *MQTT_RxDataInPtr;                        //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_RxDataOutPtr;                       //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_RxDataEndPtr;                       //外部变量声明，指向缓冲区结束的位置
extern unsigned char  MQTT_TxDataBuf[T_NUM][TBUFF_UNIT];       //外部变量声明，数据的发送缓冲区,所有发往服务器的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
extern unsigned char *MQTT_TxDataInPtr;                        //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_TxDataOutPtr;                       //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_TxDataEndPtr;                       //外部变量声明，指向缓冲区结束的位置
extern unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];          //外部变量声明，命令数据的接收缓冲区
extern unsigned char *MQTT_CMDInPtr;                           //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_CMDOutPtr;                          //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_CMDEndPtr;                          //外部变量声明，指向缓冲区结束的位置

extern char ClientID[128];     //外部变量声明，存放客户端ID的缓冲区
extern int  ClientID_len;      //外部变量声明，存放客户端ID的长度
extern char Username[128];     //外部变量声明，存放用户名的缓冲区
extern int  Username_len;	   //外部变量声明，存放用户名的长度
extern char Passward[128];     //外部变量声明，存放密码的缓冲区
extern int  Passward_len;	   //外部变量声明，存放密码的长度
extern char ServerIP[128];     //外部变量声明，存放服务器IP或是域名
extern int  ServerPort;        //外部变量声明，存放服务器的端口号

extern char Ping_flag;         //外部变量声明，ping报文状态      0：正常状态，等待计时时间到，发送Ping报文
                               //外部变量声明，ping报文状态      1：Ping报文已发送，当收到 服务器回复报文的后 将1置为0
extern char ConnectALi_flag;      //外部变量声明，同服务器连接状态  0：还没有连接服务器  1：连接上服务器了
extern char ReConnectALi_flag;    //外部变量声明，重连服务器状态    0：连接还存在  1：连接断开，重连
extern char ConnectPack_flag;  //外部变量声明，CONNECT报文状态   1：CONNECT报文成功
extern char SubcribePack_flag; //外部变量声明，订阅报文状态      1：订阅报文成功



/***********连接服务器函数（主函数中初始化）****************************/
void Connect_server(void);



/***********阿里云联网信息加载（主函数中初始化）****************************/
void AliIoT_Parameter_Init(void);

/***********容器初始化 和 加载连接报文和订阅报文到容器中（成功连接阿里云后执行）****************************/
void MQTT_Buff_Init(void);

/*
				1.发送的数据存入发送容器函数
*/
///将单片机要发送的数据放到发送容器
/***********单片机发送十六进制连接报文到容器（内部函数）****************************/
void Mqtt_ConnectMessege_To_Buff(void);

/***********单片机发送十六进制订阅报文到容器（内部函数）****************************/
void Mqtt_SubscribeMessage_To_Buff(char *topic_name, int QoS);

/***********单片机发送十六进制Pubilsh报文到容器（内部函数）****************************/
void Mqtt_PublishQs0Message_To_Buff(char *topic, char *data, int data_len);

/***********单片机发送十六进制PING报文到容器（内部函数）****************************/
void Mqtt_PingMessage_To_Buff(void);


/*
				2.发送容器中的数据 发送到云平台函数
*/

/***********将容器中数据 通过串口2发送出去****************************/
extern void TxBuff_DataSend(void); 


  
/*
					3.云平台发送的命令放入命令缓存函数
*/

/***********将接收容器中的数据 经处理后 放入Cmd容器中（内部函数）****************************/
void Mqtt_RxBuffData_To_CmdBuffData(unsigned char *redata);//将云端接收的命令放入接收容器



/*
				4.读取命令缓存中的命令 并进行功能执行函数
*/
/******在上面函数收到数据 且这个数据是命令数据后 会进行处理 压入命令缓冲区 并在此处理（需要循环调用）***********/
extern void ITO_GetCmdData_Deal(void);//得到云端发送的 命令 进行处理的函数



  


//内内部函数：具体执行功能

/***********具体执行将要发送的十六进制数据 送入发送数据容器函数（内内部函数）****************************/
void Mqtt_TxData_LoadBuff_Executive (unsigned char *data, int size);//单片机发送十六机制

/***********具体执行将要将从云端接收的数据加载到接收容器中 函数（串口2中部函数）****************************/
extern void Mqtt_RxData_LoadBuff_Executive(char *data, int size);

/***********具体执行将接收到的并处理后的命令 放入缓存区函数函数（内内部函数）****************************/
void Mqtt_CmdData_LoadBuff_Executive(unsigned char *data, int size);


/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/

#define WiFi_printf       u2_printf           //串口2控制 WiFi
#define WiFi_RxCounter    Usart2_RxCounter//缓存队列计数    
#define WiFi_RX_BUF       Usart2_RxBuff    //缓存队列数组   
#define WiFi_RXBUFF_SIZE  USART2_RXBUFF_SIZE  //串口2控制 WiFi


//硬件复位WIFI函数
char WIFI_ConfighardwareRst(int time);

char WIFI_Config(int time,char *cmd,char *response);

char WIFI_Router(int time);
char WIFI_ConnectTCP(int time);
char WiFi_Connect_IoTServer(void);

extern void WiFi_ResetIO_Init(void);
char WiFi_Reset(int timeout);
/*---------------------------wifi的头文件--------------------------------------------------------*/
/*---------------------------wifi的头文件--------------------------------------------------------*/
/*---------------------------wifi的头文件--------------------------------------------------------*/
#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)x)  //PA4控制WiFi的复位

/*---------------------------wifi的头文件--------------------------------------------------------*/
/*---------------------------wifi的头文件--------------------------------------------------------*/
/*---------------------------wifi的头文件--------------------------------------------------------*/
#endif
