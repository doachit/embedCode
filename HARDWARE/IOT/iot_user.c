/*
	* @Author       : LaiGuo
	* @Date         : 2024-10-13 13:40:34
	* @LastEditTime : 2024-10-13 15:37:57
	* @CSDN  				: https://blog.csdn.net/m0_58326153/
	* @Bili  				: https://b23.tv/llaF6l2
*/

#include "iot_user.h"
#include "sys.h"
char *star_sp, *end_sp; 	//������ȡ��ʶ��
char identifier_temp[50];	//���ڴ�ű�ʶ��
int identifier_data;			//���ڴ���·�������


/*
	* @name   IOT_init
	* @brief 	IOT�����ܳ�ʼ�����������е��ã���ESP8266 +MQTT+�����ƣ�
	* @param  [None]
	* @return [None]
*/

void IOT_init(void)
{
	WiFi_ResetIO_Init(); 	//WIFI��λ���ų�ʼ��
	AliIoT_Parameter_Init();
}

/*
	* @name   IOT_Connect
	* @brief 	��������ѭ���е���
	* @param  [None]
	* @return [None]
*/
void IOT_Connect(void)
{
	Connect_server();
}


/*
	* @name   ITO_GetCmdData_Deal
	* @brief 	�õ��������·������� 
						identifier_data�ƶ��·�������
						identifier_temp�ƶ��·��ı�ʶ��
	* @param  [None]
	* @return [None]
*/

void ITO_GetCmdData_Deal(void)
{
		star_sp =strstr((char *)MQTT_CMDOutPtr + 2, (char *)"params");
		//  ���ƶ˻�ȡ���� ����������
    if(star_sp == strstr((char *)MQTT_CMDOutPtr + 2, (char *)"params")) //app�·�����---��λ�õ���ʶ��ǰ����ַ� �����������ʶ���ж��
    {		
				end_sp = strstr(star_sp + 10, "\":"); //��λ�õ���ʶ��������ַ� �����������ʶ���ж��--��ʮ����Ϊ params":{" ��һ���ַ�����Ϊʮ
        memcpy(identifier_temp, star_sp + 10, (end_sp) - (star_sp + 10)); //����ʶ����ȡ
				identifier_data = Extract_digit((unsigned char *)end_sp + 2); //atoi�����ַ��������ֱ�����εĺ������õ��·�������   +2������Ϊ ":  ��һ���ַ�����Ϊʮ	
/******************     �û����Ŀ�ʼ->�õ��ƶ˴�����������      *********************************************************************************/
//									if(!(strcmp(identifier_temp, "value1"))) //�����¶�����
//									{
//											variable_num1 = identifier_data;
//									}
//									else if(!(strcmp(identifier_temp, "value2")))//�����¶�����
//									{
//											variable_num2 = identifier_data;
//									}
/******************* �û����Ľ���->�õ��ƶ˴����������� *********************************************************************************/							
				memset(identifier_temp, 0, 255);
    }
		//  ���ƶ˻�ȡ״̬��ִ�ж�Ӧ�Ĳ���
/******************  �û����Ŀ�ʼ->�õ��ƶ˴�������״̬ *********************************************************************************/		
//		if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"relay\":1}"))
//		{	  
//			Relay_On();
//			 Switch_Sta("relay");
//		}
//		else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"relay\":0}"))
//	  {   
//				Relay_Off();
//				Switch_Sta("relay");
//		}
/******************* �û����Ľ���->�õ��ƶ˴�������״̬ *********************************************************************************/		
    MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //ָ������
    if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)           	           //���ָ�뵽������β����
        MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //ָ���λ����������ͷ
}



/*
	* @name   Timed_Data_UpLoad
	* @brief 	��ʱ�ϴ��������ݵ�����   
	* @param  [None]
	* @return [None]
*/
void Timed_Data_UpLoad(void)  
{  
    char temp[600];  
    if ((SubcribePack_flag == 1) && (SystemTimer > 100)) // ConnectPack_flag �����Ǹ���ע�ͻ������ı�־  
    {  
        SystemTimer = 0;  
        // ��������
/******************  �û����Ŀ�ʼ-> �ϴ����� *********************************************************************************/					
//        sprintf(temp,  
//                "{\"method\":\"thing.event.property.post\","  
//                "\"id\":\"203302322\","  
//                "\"params\":{" 
//						/////////////1.��ʶ����ĸ��Ŀ�ʼ/////////////
//                "\"soildata\":%d,"  
//								"\"zifu\":\"%s\","	
//								"\"value1\":%d,"  
//								"\"value2\":%d," 
//						/////////////1.��ʶ����ĸ��Ľ���/////////////	
//                "},"  								
//                "\"version\":\"1.0.0\"}",  
//						/////////////2.���ݾ���ĸ��Ŀ�ʼ/////////////
//								(int)(AD_Value[0]),
//								"YiJianSanLian", 
//								variable_num1,
//								variable_num2		
//						/////////////2.���ݾ���ĸ��Ľ���/////////////
//								); 

/******************* �û����Ľ���->�ϴ����� *********************************************************************************/	



        // �������ݵ�MQTT������  
        Mqtt_PublishQs0Message_To_Buff(P_TOPIC_NAME, temp, strlen(temp));  
    }  
}


/*
	* @name   Switch_Sta
	* @brief 	��ʾ����״̬�ĺ���
						���¿��غ󣬷��ؿ��ص�״̬���ƶ�
	* @param  [None]
	* @return [None]
*/
void Switch_Sta(void *str)
{
	char temp[400]; 
	if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"%s\":1}"))//��Ƭ�� ״̬�ϱ�������
	{
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":1},\"version\":\"1.0.0\"}",(char *)str); 	
	}
	else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"%s\":0}")) 
	{		
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":0},\"version\":\"1.0.0\"}",(char *)str);  
	}
		Mqtt_PublishQs0Message_To_Buff(P_TOPIC_NAME,temp,strlen(temp)); 
}



/*
	* @name   Extract_digit
	* @brief 	��ȡ�����ַ��������ַ��� 
	* @param  ��Ҫ��ȡ���ַ���
	* @return ��ȡ����������
*/
u32 Extract_digit(u8 *str)
{
	u32 num=0;
	while(*str>='0'&&*str<='9')
	{
		num+=(*str)-48;
		*str++;
		num*=10;
	}
	return num/10;
}

/*--------------------------------------------*/
/*									��ע                     */
/*-------------------------------------------*/

/*//�·����ݵ���ʽ
����:{"method":"thing.service.property.set","id":"1087087436","params":{"TempH":41},"version":"1.0.0"}
star_sp=params":{"TempH":41},"version":"1.0.0"}
star_sp+10=TempH":41},"version":"1.0.0"}  
end_sp=":41},"version":"1.0.0"}
*/









