/*
	* @Author       : LaiGuo
	* @Date         : 2024-10-13 13:40:34
	* @LastEditTime : 2024-10-13 17:09:57
	* @CSDN  				: https://blog.csdn.net/m0_58326153/
	* @Bili  				: https://b23.tv/llaF6l2
*/

#include "iot_wifi_mqtt.h"
#include "sys.h"
/*-----------------------------���Եı���----------------------------------*/

/*-------------------------------------------------------mqtt.c�ı���----------------------------------*/
//MQTT�������ݻ�����
unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];           //���ݵĽ��ջ�����,����----------------���������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
unsigned char *MQTT_RxDataInPtr;                            //ָ����ջ�����������ݵ�λ��
unsigned char *MQTT_RxDataOutPtr;                          //ָ����ջ�������ȡ���ݵ�λ��
unsigned char *MQTT_RxDataEndPtr;                           //ָ����ջ�����������λ��
//MQTT�������ݻ�����
unsigned char  MQTT_TxDataBuf[T_NUM][TBUFF_UNIT];           //���ݵķ��ͻ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
unsigned char *MQTT_TxDataInPtr;                            //ָ���ͻ�����������ݵ�λ��
unsigned char *MQTT_TxDataOutPtr;                           //ָ���ͻ�������ȡ���ݵ�λ��
unsigned char *MQTT_TxDataEndPtr;                           //ָ���ͻ�����������λ��
//MQTT�����������
unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];              //�������ݵĽ��ջ�����
unsigned char *MQTT_CMDInPtr;                               //ָ���������������ݵ�λ��
unsigned char *MQTT_CMDOutPtr;                              //ָ�����������ȡ���ݵ�λ��
unsigned char *MQTT_CMDEndPtr;                              //ָ���������������λ��

char ClientID[128];        																	//��ſͻ���ID�Ļ�����
int  ClientID_len;                                          //��ſͻ���ID�ĳ���

char Username[128];                                         //����û����Ļ�����
int  Username_len;											                		//����û����ĳ���

char Passward[128];                                         //�������Ļ�����
int  Passward_len;								                          //�������ĳ���

char ServerIP[128];	                                        //��ŷ�����IP��������
int  ServerPort;                                            //��ŷ������Ķ˿ں�

int   Fixed_len;                       					     //�̶���ͷ����
int   Variable_len;                     					 //�ɱ䱨ͷ����
int   Payload_len;                       					 //��Ч���ɳ���
unsigned char  mqtt_buff[TBUFF_UNIT];						 //��ʱ������������������

//ping����״̬      1��Ping�����ѷ��ͣ����յ� �������ظ����ĵĺ� ��1��Ϊ0
char Ping_flag = 0;           //ping����״̬      0������״̬���ȴ���ʱʱ�䵽������Ping����
char ConnectALi_flag = 0;        //ͬ����������״̬  0����û�����ӷ�����  1�������Ϸ�������
char ConnectPack_flag = 0;    //CONNECT����״̬   1��CONNECT���ĳɹ�
char SubcribePack_flag = 0;   //���ı���״̬      1�����ı��ĳɹ�


/*-------------------------------------------------------mqtt.c�ı���----------------------------------*/
/*----------------------------wifi�ı���--------------------------------------------*/
char wifi_mode = 0;     //����ģʽ 0��SSID������д�ڳ�����   1��Smartconfig��ʽ��APP����
/*----------------------------wifi�ı���--------------------------------------------*/
void  ITO_GetCmdData_Deal(void);//�·���Ϣ

/*-------------------------------------------------------------*/
/*                     ���ӷ������ĺ���                    */
/*-------------------------------------------------------------*/
void Connect_server(void)
{
    /*--------------------------------------------------------------------*/
    /*   ConnectALi_flag=1ͬ����������������,���ǿ��Է������ݺͽ���������    */
    /*--------------------------------------------------------------------*/
    if(ConnectALi_flag == 1)
    {
        /*-------------------------------------------------------------*/
        /*                     �����ͻ���������                      */
        /*-------------------------------------------------------------*/
        if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)                     //if�����Ļ���˵�����ͻ�������������
        {
            //3������ɽ���if//�������ӵ�����£�������Ϣ0x30����������0x82����������0xC0
            //��1�֣�0x10 ���ӱ���
            //��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
            //��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
            if((DMA_flag == 0) && ((MQTT_TxDataOutPtr[2] == 0x10) || ((MQTT_TxDataOutPtr[2] == 0x82) && (ConnectPack_flag == 1)) || (SubcribePack_flag == 1)))
            {
                MQTT_TxData(MQTT_TxDataOutPtr);                       //��������
                MQTT_TxDataOutPtr += TBUFF_UNIT;                      //ָ������
                if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)            //���ָ�뵽������β����
                    MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
            }
        }//�����ͻ��������ݵ�else if��֧��β
        /*-------------------------------------------------------------*/
        /*                     ������ջ���������                      */
        /*-------------------------------------------------------------*/
        if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr)                 //if�����Ļ���˵�����ջ�������������
        {
						#if mqtt_debug
							IotDebug_prinf("���յ�����:");                             //������ʾ��Ϣ
						#endif
            /*-----------------------------------------------------*/
            /*                    ����CONNACK����                  */
            /*-----------------------------------------------------*/
            //if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
            //��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
            if(MQTT_RxDataOutPtr[2] == 0x20)
            {
                switch(MQTT_RxDataOutPtr[5])
                {
                case 0x00 :
									
										#if mqtt_debug
											IotDebug_prinf("CONNECT���ĳɹ�\r\n");                            //���������Ϣ
										#endif
                    ConnectPack_flag = 1;                                        //CONNECT���ĳɹ�
                    Mqtt_SubscribeMessage_To_Buff(S_TOPIC_NAME, 0);	                            //���ͻ�������Ӷ���topic���ȼ�0
                    break;                                                       //������֧case 0x00
                case 0x01 :
										#if mqtt_debug
											IotDebug_prinf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");     //���������Ϣ
										#endif
                    ConnectALi_flag = 0;                                            //ConnectALi_flag���㣬��������
                    break;                                                       //������֧case 0x01
                case 0x02 :
										#if mqtt_debug
											IotDebug_prinf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
										#endif
                    ConnectALi_flag = 0;                                            //ConnectALi_flag���㣬��������
                    break;                                                       //������֧case 0x02
                case 0x03 :
										#if mqtt_debug
											IotDebug_prinf("�����Ѿܾ�������˲����ã�׼������\r\n");         //���������Ϣ
										#endif
                    ConnectALi_flag = 0;                                            //ConnectALi_flag���㣬��������
                    break;                                                       //������֧case 0x03
                case 0x04 :
										#if mqtt_debug
											IotDebug_prinf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");   //���������Ϣ
										#endif
                    ConnectALi_flag = 0;                                            //ConnectALi_flag���㣬��������
                    break;                                                       //������֧case 0x04
                case 0x05 :
										#if mqtt_debug
											IotDebug_prinf("�����Ѿܾ���δ��Ȩ��׼������\r\n");               //���������Ϣ
										#endif
                    ConnectALi_flag = 0;                                            //ConnectALi_flag���㣬��������
                    break;                                                       //������֧case 0x05
                default   :
										#if mqtt_debug
											IotDebug_prinf("�����Ѿܾ���δ֪״̬��׼������\r\n");             //���������Ϣ
										#endif
                    ConnectALi_flag = 0;                                            //ConnectALi_flag���㣬��������
                    break;                                                       //������֧case default
                }
            }
            /*-----------------------------------------------------*/
            /*                    ����SUBACK����                   */
            /*-----------------------------------------------------*/
            //if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
            //��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
            else if(MQTT_RxDataOutPtr[2] == 0x90)
            {
                switch(MQTT_RxDataOutPtr[6])
                {
                case 0x00 :
                case 0x01 :
										#if mqtt_debug
											IotDebug_prinf("���ĳɹ�\r\n");             //���������Ϣ
										#endif
                    SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ���
										TIM3_ENABLE_10MS();										// ������ʱ�� �ζ�ʱ���������÷������ݵļ�ʱ��PTNG���ĵļ�ʱ

                    break;                                //������֧
                default   :
										#if mqtt_debug
											IotDebug_prinf("����ʧ�ܣ�׼������\r\n");   //���������Ϣ
										#endif
                    ConnectALi_flag = 0;                     //ConnectALi_flag���㣬��������
                    break;                                //������֧
                }
            }
            /*-----------------------------------------------------*/
            /*                  ����PINGRESP����                   */
            /*-----------------------------------------------------*/
            //if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
            else if(MQTT_RxDataOutPtr[2] == 0xD0)
            {
								#if mqtt_debug
									IotDebug_prinf("PING���Ļظ�\r\n"); 		  //���������Ϣ
								#endif
                if(Ping_flag == 1)                    //���Ping_flag=1����ʾ��һ�η���
                {
                    Ping_flag = 0;    				  //Ҫ���Ping_flag��־
                }
                else if(Ping_flag > 1)  				 //���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
                {
                    Ping_flag = 0;     				  //Ҫ���Ping_flag��־
										Length_of_time = 3000;      //��������PING���ĵĸ���ʱ��Ϊ30S
                }
            }
            /*-----------------------------------------------------*/
            /*                  �����������ͱ���                   */
            /*-----------------------------------------------------*/
            //if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
            //����Ҫ��ȡ��������
            else if(MQTT_RxDataOutPtr[2] == 0x30)
            {
								#if mqtt_debug
									IotDebug_prinf("�������ȼ�0����\r\n"); 		   //���������Ϣ
								#endif
                Mqtt_RxBuffData_To_CmdBuffData(MQTT_RxDataOutPtr);  //����ȼ�0��������
            }

            MQTT_RxDataOutPtr += RBUFF_UNIT;                    //����ָ������
            if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)          //�������ָ�뵽���ջ�����β����
                MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //����ָ���λ�����ջ�������ͷ
        }

        /*-------------------------------------------------------------*/
        /*                     ���������������                      */
        /*-------------------------------------------------------------*/
        if(MQTT_CMDOutPtr != MQTT_CMDInPtr)                              //if�����Ļ���˵�����������������
        {
            ITO_GetCmdData_Deal();
        }//��������������ݵ�else if��֧��β

        /*-------------------------------------------------------------*/
        /*                     ����ʱ�ϴ�����!!                        */
        /*-------------------------------------------------------------*/
				Timed_Data_UpLoad();


    }
    /*--------------------------------------------------------------------*/
    /*         ConnectALi_flag=0ͬ�������Ͽ�������,����Ҫ���ӷ�����          */
    /*--------------------------------------------------------------------*/
    else
    {
        #if mqtt_debug
            IotDebug_prinf("׼�����ӷ�����\r\n");                  //���������Ϣ
        #endif
        Usart2_Init(115200);                             //����2���ܳ�ʼ����������115200
        TIM_Cmd(TIM3, DISABLE);                          //�ر�TIM3
        TIM_Cmd(TIM2, DISABLE);                          //�ر�TIM2
				SystemTimer=0;
        WiFi_RxCounter = 0;                              //WiFi������������������
        memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);        //���WiFi���ջ�����
        if(WiFi_Connect_IoTServer() == 0)    			   //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
        {
						#if mqtt_debug
							IotDebug_prinf("���ӷ������ɹ�\r\n");              //���������Ϣ
						#endif
            Usart2_IDELInit(115200);                     //����2 ����DMA �� �����ж�
            ConnectALi_flag = 1;                            //ConnectALi_flag��1����ʾ���ӳɹ�
            WiFi_RxCounter = 0;                          //WiFi������������������
            memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);    //���WiFi���ջ�����
            MQTT_Buff_Init();                            //��ʼ�����ͽ����������
        }
    }
}

/*----------------------------------------------------------*/
/*����������ʼ������,����,�������ݵ� ������ �Լ���״̬����  */
/*��  ������                                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void MQTT_Buff_Init(void)
{
    MQTT_RxDataInPtr = MQTT_RxDataBuf[0];             //ָ���ͻ�����������ݵ�ָ���λ
    MQTT_RxDataOutPtr = MQTT_RxDataInPtr;             //ָ���ͻ�������ȡ���ݵ�ָ���λ
    MQTT_RxDataEndPtr = MQTT_RxDataBuf[R_NUM - 1];    //ָ���ͻ�����������ָ���λ

    MQTT_TxDataInPtr = MQTT_TxDataBuf[0];             //ָ���ͻ�����������ݵ�ָ���λ
    MQTT_TxDataOutPtr = MQTT_TxDataInPtr;             //ָ���ͻ�������ȡ���ݵ�ָ���λ
    MQTT_TxDataEndPtr = MQTT_TxDataBuf[T_NUM - 1];    //ָ���ͻ�����������ָ���λ

    MQTT_CMDInPtr = MQTT_CMDBuf[0];                   //ָ���������������ݵ�ָ���λ
    MQTT_CMDOutPtr = MQTT_CMDInPtr;                   //ָ�����������ȡ���ݵ�ָ���λ
    MQTT_CMDEndPtr = MQTT_CMDBuf[C_NUM - 1];          //ָ���������������ָ���λ
		
    Mqtt_ConnectMessege_To_Buff();                                //���ͻ�����������ӱ���
    Ping_flag = ConnectPack_flag = SubcribePack_flag = 0;  //������������
}
/*----------------------------------------------------------*/
/*�������������Ƴ�ʼ���������õ��ͻ���ID���û���������      */
/*��  ������                                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void AliIoT_Parameter_Init(void)
{
    char temp[128];                                                       //������ܵ�ʱ����ʱʹ�õĻ�����

    memset(ClientID, 0, 128);                                             //�ͻ���ID�Ļ�����ȫ������
    sprintf(ClientID, "%s|securemode=3,signmethod=hmacsha1|", DEVICENAME); //�����ͻ���ID�������뻺����
    ClientID_len = strlen(ClientID);                                      //����ͻ���ID�ĳ���

    memset(Username, 0, 128);                                             //�û����Ļ�����ȫ������
    sprintf(Username, "%s&%s", DEVICENAME, PRODUCTKEY);                   //�����û����������뻺����
    Username_len = strlen(Username);                                      //�����û����ĳ���

    memset(temp, 0, 128);                                                                    //��ʱ������ȫ������
    sprintf(temp, "clientId%sdeviceName%sproductKey%s", DEVICENAME, DEVICENAME, PRODUCTKEY); //��������ʱ������
    utils_hmac_sha1(temp, strlen(temp), Passward, DEVICESECRE, DEVICESECRE_LEN);             //��DeviceSecretΪ��Կ��temp�е����ģ�����hmacsha1���ܣ�����������룬�����浽��������
    Passward_len = strlen(Passward);                                                         //�����û����ĳ���

    memset(ServerIP, 0, 128);
    sprintf(ServerIP, "%s.iot-as-mqtt.cn-shanghai.aliyuncs.com", PRODUCTKEY);                //��������������
    ServerPort = 1883;                                                                       //�������˿ں�1883
#if mqtt_debug
    IotDebug_prinf("�� �� ����%s:%d\r\n", ServerIP, ServerPort); //�������������Ϣ
    IotDebug_prinf("�ͻ���ID��%s\r\n", ClientID);              //�������������Ϣ
    IotDebug_prinf("�� �� ����%s\r\n", Username);              //�������������Ϣ
    IotDebug_prinf("��    �룺%s\r\n", Passward);              //�������������Ϣ
#endif
}

/*----------------------------------------------------------*/
/*�����������ӷ���������                                    */
/*��  ������                                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void Mqtt_ConnectMessege_To_Buff(void)
{
    int temp, Remaining_len;

    Fixed_len = 1;                                                        //���ӱ����У��̶���ͷ������ʱ��=1
    Variable_len = 10;                                                    //���ӱ����У��ɱ䱨ͷ����=10
    Payload_len = 2 + ClientID_len + 2 + Username_len + 2 + Passward_len; //���ӱ����У����س���
    Remaining_len = Variable_len + Payload_len;                           //ʣ�೤��=�ɱ䱨ͷ����+���س���

    mqtt_buff[0] = 0x10;                     //�̶���ͷ��1���ֽ� ���̶�0x01
    do                                       //ѭ������̶���ͷ�е�ʣ�೤���ֽڣ��ֽ�������ʣ���ֽڵ���ʵ���ȱ仯
    {
        temp = Remaining_len % 128;          //ʣ�೤��ȡ��128
        Remaining_len = Remaining_len / 128; //ʣ�೤��ȡ��128
        if(Remaining_len > 0)
            temp |= 0x80;                    //��Э��Ҫ��λ7��λ
        mqtt_buff[Fixed_len] = temp;         //ʣ�೤���ֽڼ�¼һ������
        Fixed_len++;	                     //�̶���ͷ�ܳ���+1
    }
    while(Remaining_len > 0);                //���Remaining_len>0�Ļ����ٴν���ѭ��

    mqtt_buff[Fixed_len + 0] = 0x00; //�ɱ䱨ͷ��1���ֽ� ���̶�0x00
    mqtt_buff[Fixed_len + 1] = 0x04; //�ɱ䱨ͷ��2���ֽ� ���̶�0x04
    mqtt_buff[Fixed_len + 2] = 0x4D;	//�ɱ䱨ͷ��3���ֽ� ���̶�0x4D
    mqtt_buff[Fixed_len + 3] = 0x51;	//�ɱ䱨ͷ��4���ֽ� ���̶�0x51
    mqtt_buff[Fixed_len + 4] = 0x54;	//�ɱ䱨ͷ��5���ֽ� ���̶�0x54
    mqtt_buff[Fixed_len + 5] = 0x54;	//�ɱ䱨ͷ��6���ֽ� ���̶�0x54
    mqtt_buff[Fixed_len + 6] = 0x04;	//�ɱ䱨ͷ��7���ֽ� ���̶�0x04
    mqtt_buff[Fixed_len + 7] = 0xC2;	//�ɱ䱨ͷ��8���ֽ� ��ʹ���û���������У�飬��ʹ���������������Ự
    mqtt_buff[Fixed_len + 8] = 0x00; 	//�ɱ䱨ͷ��9���ֽ� ������ʱ����ֽ� 0x00
    mqtt_buff[Fixed_len + 9] = 0x64;	//�ɱ䱨ͷ��10���ֽڣ�����ʱ����ֽ� 0x64   100s

    /*     CLIENT_ID      */
    mqtt_buff[Fixed_len + 10] = ClientID_len / 256;                			  			 //�ͻ���ID���ȸ��ֽ�
    mqtt_buff[Fixed_len + 11] = ClientID_len % 256;               			  			 //�ͻ���ID���ȵ��ֽ�
    memcpy(&mqtt_buff[Fixed_len + 12], ClientID, ClientID_len);                 			//���ƹ����ͻ���ID�ִ�
    /*     �û���        */
    mqtt_buff[Fixed_len + 12 + ClientID_len] = Username_len / 256; 				  		 //�û������ȸ��ֽ�
    mqtt_buff[Fixed_len + 13 + ClientID_len] = Username_len % 256; 				 		 //�û������ȵ��ֽ�
    memcpy(&mqtt_buff[Fixed_len + 14 + ClientID_len], Username, Username_len);          //���ƹ����û����ִ�
    /*      ����        */
    mqtt_buff[Fixed_len + 14 + ClientID_len + Username_len] = Passward_len / 256;			 //���볤�ȸ��ֽ�
    mqtt_buff[Fixed_len + 15 + ClientID_len + Username_len] = Passward_len % 256;			 //���볤�ȵ��ֽ�
    memcpy(&mqtt_buff[Fixed_len + 16 + ClientID_len + Username_len], Passward, Passward_len); //���ƹ��������ִ�

    Mqtt_TxData_LoadBuff_Executive(mqtt_buff, Fixed_len + Variable_len + Payload_len);                  //���뷢�����ݻ�����
}
/*----------------------------------------------------------*/
/*��������SUBSCRIBE����topic����                            */
/*��  ����QoS�����ĵȼ�                                     */
/*��  ����topic_name������topic��������                     */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void Mqtt_SubscribeMessage_To_Buff(char *topic_name, int QoS)
{
	int remaining_size;//ʣ���ֽڳ���
	int len;
	 Fixed_len=1;
	Variable_len = 2;	//SUBSCRIBE�����У��ɱ䱨ͷ����=2	
	Payload_len = 2 + strlen(topic_name)+1; //������Ч���ɳ��� = 2�ֽ�(topic_name����)+ topic_name�ַ����ĳ��� + 1�ֽڷ���ȼ�
	remaining_size = Variable_len+Payload_len;	
	mqtt_buff[0]=0x82;
	while( remaining_size > 0)
  {
        len= remaining_size%128;
				remaining_size = remaining_size/128;    
        if(remaining_size > 0)
        {
          len =len|0x80;
        }
         mqtt_buff[Fixed_len++]=len;
   }
		mqtt_buff[Fixed_len+0]=0x00;    
    mqtt_buff[Fixed_len+1]=0x0A;
		mqtt_buff[Fixed_len+2] = strlen(topic_name) /256;
		mqtt_buff[Fixed_len+3] = strlen(topic_name) % 256;
    memcpy(&mqtt_buff[Fixed_len+4], topic_name,strlen(topic_name)); 
		mqtt_buff[Fixed_len+4+strlen(topic_name)]=QoS; //����QOS�ȼ�
		 
		Mqtt_TxData_LoadBuff_Executive(mqtt_buff, Fixed_len + Variable_len + Payload_len);  

}
/*----------------------------------------------------------*/
/*��������PING���ģ�������                                  */
/*��  ������                                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void Mqtt_PingMessage_To_Buff(void)
{
    mqtt_buff[0] = 0xC0;            //��1���ֽ� ���̶�0xC0
    mqtt_buff[1] = 0x00;            //��2���ֽ� ���̶�0x00

    Mqtt_TxData_LoadBuff_Executive(mqtt_buff, 2);   //�������ݵ�������
}
/*----------------------------------------------------------*/
/*���������ȼ�0 ������Ϣ����   �����ǽ�Ҫ��������Ϣ��������*/
//ͨ��MQTT_TxData������������
/*��  ����topic_name��topic����                             */
/*��  ����data������                                        */
/*��  ����data_len�����ݳ���                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void Mqtt_PublishQs0Message_To_Buff(char *topic, char *data, int data_len)
{
    int temp, Remaining_len;

    Fixed_len = 1;                              //�̶���ͷ������ʱ�ȵ��ڣ�1�ֽ�
    Variable_len = 2 + strlen(topic);           //�ɱ䱨ͷ���ȣ�2�ֽ�(topic����)+ topic�ַ����ĳ���
    Payload_len = data_len;                     //��Ч���ɳ��ȣ�����data_len
    Remaining_len = Variable_len + Payload_len; //ʣ�೤��=�ɱ䱨ͷ����+���س���

    mqtt_buff[0] = 0x30;                     //�̶���ͷ��1���ֽ� ���̶�0x30
    do                                       //ѭ������̶���ͷ�е�ʣ�೤���ֽڣ��ֽ�������ʣ���ֽڵ���ʵ���ȱ仯
    {
        temp = Remaining_len % 128;          //ʣ�೤��ȡ��128
        Remaining_len = Remaining_len / 128; //ʣ�೤��ȡ��128
        if(Remaining_len > 0)
            temp |= 0x80;                    //��Э��Ҫ��λ7��λ
        mqtt_buff[Fixed_len] = temp;         //ʣ�೤���ֽڼ�¼һ������
        Fixed_len++;	                     //�̶���ͷ�ܳ���+1
    }
    while(Remaining_len > 0);                //���Remaining_len>0�Ļ����ٴν���ѭ��

    mqtt_buff[Fixed_len + 0] = strlen(topic) / 256;                //�ɱ䱨ͷ��1���ֽ�     ��topic���ȸ��ֽ�
    mqtt_buff[Fixed_len + 1] = strlen(topic) % 256;		         //�ɱ䱨ͷ��2���ֽ�     ��topic���ȵ��ֽ�
    memcpy(&mqtt_buff[Fixed_len + 2], topic, strlen(topic));       //�ɱ䱨ͷ��3���ֽڿ�ʼ ������topic�ַ���
    memcpy(&mqtt_buff[Fixed_len + 2 + strlen(topic)], data, data_len); //��Ч���ɣ�����data����

    Mqtt_TxData_LoadBuff_Executive(mqtt_buff, Fixed_len + Variable_len + Payload_len);  //���뷢�����ݻ�����
}
/*----------------------------------------------------------*/
/*����������������������ĵȼ�0������                       */
/*��  ����redata�����յ�����                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void Mqtt_RxBuffData_To_CmdBuffData(unsigned char *redata)
{
    int  re_len;               	           //����һ����������Ž��յ������ܳ���
    int  pack_num;                         //����һ�����������������һ�����ʱ���������͵ĸ���
    int  temp, temp_len;                   //����һ���������ݴ�����
    int  totle_len;                        //����һ������������Ѿ�ͳ�Ƶ����͵���������
    int  topic_len;              	       //����һ���������������������ĳ���
    int  cmd_len;                          //����һ����������������а������������ݵĳ���
    int  cmd_loca;                         //����һ����������������а������������ʼλ��
    int  i;                                //����һ������������forѭ��
    int  local, multiplier;
    unsigned char tempbuff[RBUFF_UNIT];	   //��ʱ������
    unsigned char *data;                   //redata������ʱ�򣬵�һ���ֽ�������������data����ָ��redata�ĵ�2���ֽڣ����������ݿ�ʼ�ĵط�

    re_len = redata[0] * 256 + redata[1];                           //��ȡ���յ������ܳ���
    data = &redata[2];                                              //dataָ��redata�ĵ�2���ֽڣ����������ݿ�ʼ��
    pack_num = temp_len = totle_len = temp = 0;                	    //������������
    local = 1;
    multiplier = 1;
    do
    {
        pack_num++;                                     			//��ʼѭ��ͳ�����͵ĸ�����ÿ��ѭ�����͵ĸ���+1
        do
        {
            temp = data[totle_len + local];
            temp_len += (temp & 127) * multiplier;
            multiplier *= 128;
            local++;
        }
        while ((temp & 128) != 0);
        totle_len += (temp_len + local);                          	//�ۼ�ͳ�Ƶ��ܵ����͵����ݳ���
        re_len -= (temp_len + local) ;                              //���յ������ܳ��� ��ȥ ����ͳ�Ƶ����͵��ܳ���
        local = 1;
        multiplier = 1;
        temp_len = 0;
    }
    while(re_len != 0);                                  			//������յ������ܳ��ȵ���0�ˣ�˵��ͳ�������
		#if mqtt_debug
			IotDebug_prinf("���ν�����%d����������\r\n", pack_num); //���������Ϣ
		#endif
    temp_len = totle_len = 0;                		            	//������������
    local = 1;
    multiplier = 1;
    for(i = 0; i < pack_num; i++)                                   //�Ѿ�ͳ�Ƶ��˽��յ����͸�������ʼforѭ����ȡ��ÿ�����͵�����
    {
        do
        {
            temp = data[totle_len + local];
            temp_len += (temp & 127) * multiplier;
            multiplier *= 128;
            local++;
        }
        while ((temp & 128) != 0);
        topic_len = data[local + totle_len] * 256 + data[local + 1 + totle_len] + 2; //���㱾����������������ռ�õ�������
        cmd_len = temp_len - topic_len;                             //���㱾��������������������ռ�õ�������
        cmd_loca = totle_len + local +  topic_len;                  //���㱾�������������������ݿ�ʼ��λ��
        memcpy(tempbuff, &data[cmd_loca], cmd_len);                 //�������ݿ�������
        Mqtt_CmdData_LoadBuff_Executive(tempbuff, cmd_len);                             //�������������
        totle_len += (temp_len + local);                            //�ۼ��Ѿ�ͳ�Ƶ����͵����ݳ���
        local = 1;
        multiplier = 1;
        temp_len = 0;
    }
}
/*----------------------------------------------------------*/
/*�������������ͻ����� //��Ƭ�����͵����ݼ��ص�������                                   */
/*��  ����data������                                        */
/*��  ����size�����ݳ���                                    */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void Mqtt_TxData_LoadBuff_Executive(unsigned char *data, int size)
{
    memcpy(&MQTT_TxDataInPtr[2], data, size);    //�������ݵ����ͻ�����
    MQTT_TxDataInPtr[0] = size / 256;            //��¼���ݳ���
    MQTT_TxDataInPtr[1] = size % 256;            //��¼���ݳ���
    MQTT_TxDataInPtr += TBUFF_UNIT;               //ָ������
    if(MQTT_TxDataInPtr == MQTT_TxDataEndPtr)    //���ָ�뵽������β����
        MQTT_TxDataInPtr = MQTT_TxDataBuf[0];    //ָ���λ����������ͷ
}
/*----------------------------------------------------------*/
/*�������������������  ////��������뻺��������                                  */
/*��  ����data������                                        */
/*��  ����size�����ݳ���                                    */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void Mqtt_CmdData_LoadBuff_Executive(unsigned char *data, int size)
{
    memcpy(&MQTT_CMDInPtr[2], data, size);    //�������ݵ��������
    MQTT_CMDInPtr[0] = size / 256;            //��¼���ݳ���
    MQTT_CMDInPtr[1] = size % 256;            //��¼���ݳ���
    MQTT_CMDInPtr[size + 2] = '\0';           //�����ַ���������
    MQTT_CMDInPtr += CBUFF_UNIT;               //ָ������
    if(MQTT_CMDInPtr == MQTT_CMDEndPtr)       //���ָ�뵽������β����
        MQTT_CMDInPtr = MQTT_CMDBuf[0];       //ָ���λ����������ͷ
}


//�����ƶ˽��յ����ݼ��ص�����������
void Mqtt_RxData_LoadBuff_Executive(char *data, int size)//�����ƶ˽��յ����ݼ��ص�����������
{
     memcpy(&MQTT_RxDataInPtr[2], Usart2_RxBuff, Usart2_RxCounter);                    //�������ݵ����ջ�����
     MQTT_RxDataInPtr[0] = Usart2_RxCounter / 256;                                     //��¼���ݳ��ȸ��ֽ�
     MQTT_RxDataInPtr[1] = Usart2_RxCounter % 256;                                     //��¼���ݳ��ȵ��ֽ�
     MQTT_RxDataInPtr += RBUFF_UNIT;                                                   //ָ������
     if(MQTT_RxDataInPtr == MQTT_RxDataEndPtr)                                         //���ָ�뵽������β����
         MQTT_RxDataInPtr = MQTT_RxDataBuf[0]; 
}

/*-------------------------------------------------*/
/*���ܣ������ͻ���������//���ʽ��������������� ���ͳ�ȥ     					          */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TxBuff_DataSend(void)
{
    /*-------------------------------------------------------------*/
    /*                     �����ͻ���������                      */
    /*-------------------------------------------------------------*/
    if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)                     //if�����Ļ���˵�����ͻ�������������
    {
        //3������ɽ���if
        //��1�֣�0x10 ���ӱ���
        //��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
        //��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
        if((DMA_flag == 0) && ((MQTT_TxDataOutPtr[2] == 0x10) || ((MQTT_TxDataOutPtr[2] == 0x82) && (ConnectPack_flag == 1)) || (SubcribePack_flag == 1)))
        {
						#if mqtt_debug
							IotDebug_prinf("��������:0x%x\r\n", MQTT_TxDataOutPtr[2]); //������ʾ��Ϣ
						#endif
            MQTT_TxData(MQTT_TxDataOutPtr);                       //��������
            MQTT_TxDataOutPtr += TBUFF_UNIT;                      //ָ������
            if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)            //���ָ�뵽������β����
                MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
        }
    }//�����ͻ��������ݵ�else if��֧��β
}


/////////////////////////ESP8266//////////////////////////////////////////////////////

/*-------------------------------------------------*/
/*����������ʼ��WiFi�ĸ�λIO                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void WiFi_ResetIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                      //����һ������IO�˿ڲ����Ľṹ��
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);    //ʹ��PA�˿�ʱ��
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //׼������PA4
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //����50Mhz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		  //���������ʽ
    GPIO_Init(GPIOA, &GPIO_InitStructure);            		  //����PA4
    RESET_IO(1);                                              //��λIO���ߵ�ƽ
}

/*-------------------------------------------------*/
/*��������WiFi�Զ���λ                                 */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
    RESET_IO(0);                                    //��λIO���͵�ƽ
    delay_ms(500);                                  //��ʱ500ms
    RESET_IO(1);                                    //��λIO���ߵ�ƽ
    while(timeout--)                                //�ȴ���ʱʱ�䵽0
    {
        delay_ms(100);                              //��ʱ100ms
        if(strstr(WiFi_RX_BUF, "ready")||strstr(WiFi_RX_BUF, "invalid"))            //������յ�  ready invalid ��ʾ��λ�ɹ�
            break;       						    //��������whileѭ��
    }
    if(timeout <= 0)return 1;                       //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�ready������1
    else return 0;		         				    //��֮����ʾ��ȷ��˵���յ�ready��ͨ��break��������while
}

//��λWIFIģ��
//�ɹ�����0 ʧ�ܷ���1
char WIFI_ConfighardwareRst(int time)
{
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);
	WiFi_RxCounter = 0;
	while(time--)//��ѭ����ʱ�ȴ�//timeһ��Ϊ100 //100*100 = 10s 10�����Ҳ������˳�while������
	{
		delay_ms(100);//100*time = ����������ʱ ����while �ȴ�̫�� ����while
		if(strstr(WiFi_RX_BUF,"ready"))
			break;//�ҵ� ����while
		#if mqtt_debug
			IotDebug_prinf ("%d ",time); 
		#endif
	}
	if(time>0)
       return 0;
    else
			 return 1;
}


//����ATָ���
//cmd�����͵�����  response�����յ�������
//�ɹ�����0 ʧ�ܷ���1
char WIFI_Config(int time,char *cmd,char *response)
{
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);
	WiFi_RxCounter = 0;
	u2_printf("%s\r\n",cmd); 
	while(time--)
	{
		delay_ms(100);
		if(strstr(WiFi_RX_BUF,response))
			break;
		#if mqtt_debug
			IotDebug_prinf ("%d ",time); 
		#endif
	}
	if(time>0)
       return 0; 
    else
       return 1; 
}
//����WIFI���纯��
char WIFI_Router(int time)
{
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);
	WiFi_RxCounter = 0;
	u2_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",WIFIID,PASSWORD); 
	while(time--)
	{
		delay_ms(1000);//����WIFIʱ�䳤��
		if(strstr(WiFi_RX_BUF,"OK"))
			break;
		#if mqtt_debug
			IotDebug_prinf ("%d ",time); 
		#endif
	}
	if(time>0)
       return 0;
    else
       return 1;
}

char WIFI_ConnectTCP(int time)//���߲�������TCP���ӣ����ֻ��ķ���˽������ӣ�
{
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);
	WiFi_RxCounter = 0;

	u2_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ServerIP,ServerPort); 
	while(time--)
	{
		delay_ms(1000);//����WIFIʱ�䳤��
		if(strstr(WiFi_RX_BUF,"OK"))
			break;
		#if mqtt_debug
			IotDebug_prinf ("%d ",time); 
		#endif
	}
	if(time>0)
       return 0;
    else
       return 1;
}

char WiFi_Connect_IoTServer(void)
{

/*0��������λ*/
	#if mqtt_debug
		IotDebug_prinf ("0��׼��������λ!\r\n");
	#endif
	if(WiFi_Reset(100))
	{
		IotDebug_prinf ("������λʧ��!\r\n");
		return 1;
	}
	else
	{
		#if mqtt_debug
			IotDebug_prinf ("������λ�ɹ�!\r\n");
		#endif
	}
	#if mqtt_debug
		IotDebug_prinf ("\r\n");
	#endif
	
/*1������WIFIģʽ*///stationģʽ
	#if mqtt_debug
		IotDebug_prinf ("1��׼������WIFIģʽ!\r\n");
	#endif
	if(WIFI_Config(50,"AT+CWMODE=1\r\n","OK"))
	{
		#if mqtt_debug
			IotDebug_prinf ("����WIFIģʽʧ��!\r\n");
		#endif
		return 2;
	}
	else
	{
		#if mqtt_debug
			IotDebug_prinf ("����WIFIģʽ�ɹ�!\r\n");
		#endif		
	}
	#if mqtt_debug
		IotDebug_prinf ("\r\n");	
	#endif
/*2������(���ʽ)*/	
	#if mqtt_debug
		IotDebug_prinf ("2��׼����λ!\r\n");
	#endif	
	if(WIFI_Config(50,"AT+RST\r\n","ready"))
	{
		#if mqtt_debug		
			IotDebug_prinf ("��λʧ��!\r\n");
		#endif	
		return 3;
	}
	else
	{
		#if mqtt_debug
			IotDebug_prinf ("��λ�ɹ�!\r\n");
		#endif
	}
		#if mqtt_debug
			IotDebug_prinf ("\r\n");
		#endif
/*3��ȡ���Զ�����*/		
	#if mqtt_debug
		IotDebug_prinf ("3��׼��ȡ���Զ�����\r\n");
	#endif
	if(WIFI_Config(50,"AT+CWAUTOCONN=0\r\n","OK"))
	{
		#if mqtt_debug
			IotDebug_prinf ("ȡ���Զ�����ʧ��!\r\n");
		#endif
		return 4;
	}
	else
	{
		#if mqtt_debug
			IotDebug_prinf ("ȡ���Զ����ӳɹ�!\r\n");
		#endif
	}	
	#if mqtt_debug
		IotDebug_prinf ("\r\n");
	#endif
/*4������·����*/	
	#if mqtt_debug
		IotDebug_prinf ("4��׼������·����\r\n");
	#endif
	if(WIFI_Router(50))
	{
		#if mqtt_debug
			IotDebug_prinf ("����·����ʧ��!\r\n");
		#endif
		return 5;
	}
	else
	{
		#if mqtt_debug
			IotDebug_prinf ("����·�����ɹ�!\r\n");
		#endif
	}
	#if mqtt_debug
		IotDebug_prinf ("\r\n");
	#endif
/*5�����õ�·����ģʽ*/	
	#if mqtt_debug	
		IotDebug_prinf ("5��׼�����õ�·����ģʽ!\r\n");
	#endif
	if(WIFI_Config(50,"AT+CIPMUX=0\r\n","OK"))
	{
		#if mqtt_debug	
			IotDebug_prinf ("���õ�·����ģʽʧ��!\r\n");
		#endif
		return 6;
	}
	else
	{
		#if mqtt_debug
			IotDebug_prinf ("���õ�·����ģʽ�ɹ�!\r\n");
		#endif
	}
	#if mqtt_debug	
	IotDebug_prinf ("\r\n");
	#endif
/*6������͸��ģʽ*/
	#if	mqtt_debug
		IotDebug_prinf ("6��׼������͸��ģʽ\r\n");
	#endif
	if(WIFI_Config(50,"AT+CIPMODE=1\r\n","OK"))
	{
		#if	mqtt_debug
			IotDebug_prinf ("����͸��ģʽʧ��!\r\n");
		#endif
		return 7;
	}
	else
	{
		#if	mqtt_debug
			IotDebug_prinf ("����͸��ģʽ�ɹ�!\r\n");
		#endif
	}
	#if	mqtt_debug
		IotDebug_prinf ("\r\n");
	#endif
/*7������TCP����*/	//���ֻ��ķ���˽������ӣ�
	//��PC/�ֻ���TCP����������
	//Ҳ���ԺͰ��������Ӱ�
	if(WIFI_ConnectTCP(50))
	{
		#if	mqtt_debug
			IotDebug_prinf ("����TCP����ʧ��!\r\n");
		#endif
		return 8;
	}
	else
	{
		#if	mqtt_debug
			IotDebug_prinf ("����TCP���ӳɹ�!\r\n");
		#endif
	}
	#if	mqtt_debug
		IotDebug_prinf ("\r\n");
	#endif
/*8������͸��ģʽ*/	//���Է�������
	#if	mqtt_debug
		IotDebug_prinf ("8��׼������͸��ģʽ\r\n");
	#endif
	if(WIFI_Config(50,"AT+CIPSEND\r\n","\r\nOK\r\n\r\n>"))
	{
		#if	mqtt_debug
			IotDebug_prinf ("����͸��ģʽʧ��!\r\n");
		#endif
		return 9;
	}
	else
	{
		#if	mqtt_debug
			IotDebug_prinf ("����͸��ģʽ�ɹ�!\r\n");
		#endif
	}
	return 0;
}


/******************************************************************************
����esp6266����
	WIFI_ConfigRst(100);//��λ
	WIFI_Config(50,"AT+CWMODE=1\r\n","OK");//��һ��������WIFIģ��ΪSationģʽ
	WIFI_Config(50,"AT+RST\r\n","ready");//�ڶ���������WIFIģ��
	WIFI_Config(50,"AT+CWAUTOCONN=0\r\n","OK");//�����������Զ���������
	WIFI_Router(50);//���Ĳ�����������
	WIFI_Config(50,"AT+CIPMUX=0\r\n","OK");//���岽�����õ�����ģʽ ֻ������һ���豸
	WIFI_Config(50,"AT+CIPMODE=1\r\n","OK");//������������Ϊ͸��ģʽ
	WIFI_ConnectTCP(50);//���߲�������TCP���ӣ����ֻ��ķ���˽������ӣ�
	WIFI_Config(50,"AT+CIPSEND\r\n","\r\nOK\r\n\r\n>");//�ڰ˲�����͸��ģʽ����ʼ��������
***********************************************************************************/


