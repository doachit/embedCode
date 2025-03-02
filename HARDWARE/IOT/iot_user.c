/*
	* @Author       : LaiGuo
	* @Date         : 2024-10-13 13:40:34
	* @LastEditTime : 2024-10-13 15:37:57
	* @CSDN  				: https://blog.csdn.net/m0_58326153/
	* @Bili  				: https://b23.tv/llaF6l2
*/

#include "iot_user.h"

char *star_sp, *end_sp; 	//用于提取标识符
char identifier_temp[50];	//用于存放标识符
int identifier_data;			//用于存放下发的数据


/*
	* @name   IOT_init
	* @brief 	IOT程序总初始化（主函数中调用）（ESP8266 +MQTT+阿里云）
	* @param  [None]
	* @return [None]
*/

void IOT_init(void)
{
	WiFi_ResetIO_Init(); 	//WIFI复位引脚初始化
	AliIoT_Parameter_Init();
}

/*
	* @name   IOT_Connect
	* @brief 	在主函数循环中调用
	* @param  [None]
	* @return [None]
*/
void IOT_Connect(void)
{
	Connect_server();
}


/*
	* @name   ITO_GetCmdData_Deal
	* @brief 	得到并处理下发的数据 
						identifier_data云端下发的数据
						identifier_temp云端下发的标识符
	* @param  [None]
	* @return [None]
*/

void ITO_GetCmdData_Deal(void)
{
		star_sp =strstr((char *)MQTT_CMDOutPtr + 2, (char *)"params");
		//  从云端获取数据 并更改数据
    if(star_sp == strstr((char *)MQTT_CMDOutPtr + 2, (char *)"params")) //app下发数据---定位得到标识符前面的字符 ，用于求出标识符有多大
    {		
				end_sp = strstr(star_sp + 10, "\":"); //定位得到标识符后面的字符 ，用于求出标识符有多大--加十是因为 params":{" 这一串字符长度为十
        memcpy(identifier_temp, star_sp + 10, (end_sp) - (star_sp + 10)); //将标识符提取
				identifier_data = Extract_digit((unsigned char *)end_sp + 2); //atoi（将字符串的数字变成整形的函数）得到下发的数据   +2是是因为 ":  这一串字符长度为十	
/******************     用户更改开始->得到云端传下来的数据      *********************************************************************************/
//									if(!(strcmp(identifier_temp, "value1"))) //空气温度上限
//									{
//											variable_num1 = identifier_data;
//									}
//									else if(!(strcmp(identifier_temp, "value2")))//空气温度下限
//									{
//											variable_num2 = identifier_data;
//									}
/******************* 用户更改结束->得到云端传下来的数据 *********************************************************************************/							
				memset(identifier_temp, 0, 255);
    }
		//  从云端获取状态后执行对应的操作
/******************  用户更改开始->得到云端传下来的状态 *********************************************************************************/		
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
/******************* 用户更改结束->得到云端传下来的状态 *********************************************************************************/		
    MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //指针下移
    if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)           	           //如果指针到缓冲区尾部了
        MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头
}



/*
	* @name   Timed_Data_UpLoad
	* @brief 	定时上传各种数据的任务   
	* @param  [None]
	* @return [None]
*/
void Timed_Data_UpLoad(void)  
{  
    char temp[600];  
    if ((SubcribePack_flag == 1) && (SystemTimer > 100)) // ConnectPack_flag 可能是个旧注释或待处理的标志  
    {  
        SystemTimer = 0;  
        // 构建数据
/******************  用户更改开始-> 上传数据 *********************************************************************************/					
//        sprintf(temp,  
//                "{\"method\":\"thing.event.property.post\","  
//                "\"id\":\"203302322\","  
//                "\"params\":{" 
//						/////////////1.标识具体的更改开始/////////////
//                "\"soildata\":%d,"  
//								"\"zifu\":\"%s\","	
//								"\"value1\":%d,"  
//								"\"value2\":%d," 
//						/////////////1.标识具体的更改结束/////////////	
//                "},"  								
//                "\"version\":\"1.0.0\"}",  
//						/////////////2.数据具体的更改开始/////////////
//								(int)(AD_Value[0]),
//								"YiJianSanLian", 
//								variable_num1,
//								variable_num2		
//						/////////////2.数据具体的更改结束/////////////
//								); 

/******************* 用户更改结束->上传数据 *********************************************************************************/	



        // 发布数据到MQTT服务器  
        Mqtt_PublishQs0Message_To_Buff(P_TOPIC_NAME, temp, strlen(temp));  
    }  
}


/*
	* @name   Switch_Sta
	* @brief 	显示开关状态的函数
						按下开关后，返回开关的状态给云端
	* @param  [None]
	* @return [None]
*/
void Switch_Sta(void *str)
{
	char temp[400]; 
	if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"%s\":1}"))//单片机 状态上报阿里云
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
	* @brief 	提取传入字符串的数字返回 
	* @param  需要提取的字符串
	* @return 提取出来的数字
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
/*									备注                     */
/*-------------------------------------------*/

/*//下发数据的样式
命令:{"method":"thing.service.property.set","id":"1087087436","params":{"TempH":41},"version":"1.0.0"}
star_sp=params":{"TempH":41},"version":"1.0.0"}
star_sp+10=TempH":41},"version":"1.0.0"}  
end_sp=":41},"version":"1.0.0"}
*/









