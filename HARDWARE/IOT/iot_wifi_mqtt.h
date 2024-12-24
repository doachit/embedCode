#ifndef __ITO_WIFI_MQTT_H
#define __ITO_WIFI_MQTT_H

#include "sys.h"       //������Ҫ��ͷ�ļ�

/*--------------�Ƿ������ڵ���ģʽ---------------------------------*/
#define mqtt_debug    0   //1 ��������  0�رյ���
#define IotDebug_prinf(...) printf(__VA_ARGS__) //���ԵĴ��ں����ɸ���Ϊ�Լ���

/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
#define  R_NUM               7     //���ջ���������
#define  RBUFF_UNIT          400   //���ջ���������

#define  T_NUM               7     //���ͻ���������  
#define  TBUFF_UNIT          400   //���ͻ���������

#define  C_NUM               7     //�����������
#define  CBUFF_UNIT          400   //�����������
           
#define  MQTT_TxData(x)       u2_TxData(x)                                         //����2�������ݷ���
#define  WIFIID             "302a"    //��ʼ����·����SSID����
#define  PASSWORD           "1234abcd+-*/"       //��ʼ����·��������



#define PRODUCTKEY           "a1Bgo8sVlSl"  																			//��ƷID
#define PRODUCTKEY_LEN       strlen(PRODUCTKEY)                                   //��ƷID����
#define DEVICENAME           "mytest" 																						//�豸�� 
#define DEVICENAME_LEN       strlen(DEVICENAME)                                   //�豸������
#define DEVICESECRE          "35c77a1c88b49ddfe07e46ee856a6e0a"  									//�豸��Կ
#define DEVICESECRE_LEN      strlen(DEVICESECRE) 																	//�豸��Կ����

// ʹ�ü򵥵ĺ����ӣ���ע���������ǲ���ֱ���ں궨���������ַ����������ͺ�  
#define S_TOPIC_PREFIX       "/sys/"  
#define S_TOPIC_MIDDLE       PRODUCTKEY "/" DEVICENAME  
#define S_TOPIC_SUFFIX       "/thing/service/property/set"  
#define S_TOPIC_NAME         S_TOPIC_PREFIX S_TOPIC_MIDDLE S_TOPIC_SUFFIX  		//��Ҫ���ĵ����� 
  
// ΪP_TOPIC_NAME����  
#define P_TOPIC_PREFIX       "/sys/"  
#define P_TOPIC_MIDDLE       PRODUCTKEY "/" DEVICENAME  
#define P_TOPIC_SUFFIX       "/thing/event/property/post"  
#define P_TOPIC_NAME         P_TOPIC_PREFIX P_TOPIC_MIDDLE P_TOPIC_SUFFIX							//��Ҫ����������  

extern unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];       //�ⲿ�������������ݵĽ��ջ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
extern unsigned char *MQTT_RxDataInPtr;                        //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_RxDataOutPtr;                       //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_RxDataEndPtr;                       //�ⲿ����������ָ�򻺳���������λ��
extern unsigned char  MQTT_TxDataBuf[T_NUM][TBUFF_UNIT];       //�ⲿ�������������ݵķ��ͻ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
extern unsigned char *MQTT_TxDataInPtr;                        //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_TxDataOutPtr;                       //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_TxDataEndPtr;                       //�ⲿ����������ָ�򻺳���������λ��
extern unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];          //�ⲿ�����������������ݵĽ��ջ�����
extern unsigned char *MQTT_CMDInPtr;                           //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_CMDOutPtr;                          //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_CMDEndPtr;                          //�ⲿ����������ָ�򻺳���������λ��

extern char ClientID[128];     //�ⲿ������������ſͻ���ID�Ļ�����
extern int  ClientID_len;      //�ⲿ������������ſͻ���ID�ĳ���
extern char Username[128];     //�ⲿ��������������û����Ļ�����
extern int  Username_len;	   //�ⲿ��������������û����ĳ���
extern char Passward[128];     //�ⲿ�����������������Ļ�����
extern int  Passward_len;	   //�ⲿ�����������������ĳ���
extern char ServerIP[128];     //�ⲿ������������ŷ�����IP��������
extern int  ServerPort;        //�ⲿ������������ŷ������Ķ˿ں�

extern char Ping_flag;         //�ⲿ����������ping����״̬      0������״̬���ȴ���ʱʱ�䵽������Ping����
                               //�ⲿ����������ping����״̬      1��Ping�����ѷ��ͣ����յ� �������ظ����ĵĺ� ��1��Ϊ0
extern char ConnectALi_flag;      //�ⲿ����������ͬ����������״̬  0����û�����ӷ�����  1�������Ϸ�������
extern char ReConnectALi_flag;    //�ⲿ��������������������״̬    0�����ӻ�����  1�����ӶϿ�������
extern char ConnectPack_flag;  //�ⲿ����������CONNECT����״̬   1��CONNECT���ĳɹ�
extern char SubcribePack_flag; //�ⲿ�������������ı���״̬      1�����ı��ĳɹ�



/***********���ӷ������������������г�ʼ����****************************/
void Connect_server(void);



/***********������������Ϣ���أ��������г�ʼ����****************************/
void AliIoT_Parameter_Init(void);

/***********������ʼ�� �� �������ӱ��ĺͶ��ı��ĵ������У��ɹ����Ӱ����ƺ�ִ�У�****************************/
void MQTT_Buff_Init(void);

/*
				1.���͵����ݴ��뷢����������
*/
///����Ƭ��Ҫ���͵����ݷŵ���������
/***********��Ƭ������ʮ���������ӱ��ĵ��������ڲ�������****************************/
void Mqtt_ConnectMessege_To_Buff(void);

/***********��Ƭ������ʮ�����ƶ��ı��ĵ��������ڲ�������****************************/
void Mqtt_SubscribeMessage_To_Buff(char *topic_name, int QoS);

/***********��Ƭ������ʮ������Pubilsh���ĵ��������ڲ�������****************************/
void Mqtt_PublishQs0Message_To_Buff(char *topic, char *data, int data_len);

/***********��Ƭ������ʮ������PING���ĵ��������ڲ�������****************************/
void Mqtt_PingMessage_To_Buff(void);


/*
				2.���������е����� ���͵���ƽ̨����
*/

/***********������������ ͨ������2���ͳ�ȥ****************************/
extern void TxBuff_DataSend(void); 


  
/*
					3.��ƽ̨���͵������������溯��
*/

/***********�����������е����� ������� ����Cmd�����У��ڲ�������****************************/
void Mqtt_RxBuffData_To_CmdBuffData(unsigned char *redata);//���ƶ˽��յ���������������



/*
				4.��ȡ������е����� �����й���ִ�к���
*/
/******�����溯���յ����� ������������������ݺ� ����д��� ѹ��������� ���ڴ˴�����Ҫѭ�����ã�***********/
extern void ITO_GetCmdData_Deal(void);//�õ��ƶ˷��͵� ���� ���д���ĺ���



  


//���ڲ�����������ִ�й���

/***********����ִ�н�Ҫ���͵�ʮ���������� ���뷢�������������������ڲ�������****************************/
void Mqtt_TxData_LoadBuff_Executive (unsigned char *data, int size);//��Ƭ������ʮ������

/***********����ִ�н�Ҫ�����ƶ˽��յ����ݼ��ص����������� ����������2�в�������****************************/
extern void Mqtt_RxData_LoadBuff_Executive(char *data, int size);

/***********����ִ�н����յ��Ĳ����������� ���뻺�����������������ڲ�������****************************/
void Mqtt_CmdData_LoadBuff_Executive(unsigned char *data, int size);


/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/
/*------------------------------------------------------------ mqtt��ͷ�ļ�----------------------------------------------------*/

#define WiFi_printf       u2_printf           //����2���� WiFi
#define WiFi_RxCounter    Usart2_RxCounter//������м���    
#define WiFi_RX_BUF       Usart2_RxBuff    //�����������   
#define WiFi_RXBUFF_SIZE  USART2_RXBUFF_SIZE  //����2���� WiFi


//Ӳ����λWIFI����
char WIFI_ConfighardwareRst(int time);

char WIFI_Config(int time,char *cmd,char *response);

char WIFI_Router(int time);
char WIFI_ConnectTCP(int time);
char WiFi_Connect_IoTServer(void);

extern void WiFi_ResetIO_Init(void);
char WiFi_Reset(int timeout);
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)x)  //PA4����WiFi�ĸ�λ

/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
/*---------------------------wifi��ͷ�ļ�--------------------------------------------------------*/
#endif
