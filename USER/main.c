#include "stdio.h"//标准输入输出库
#include "string.h"//字符串库
#include "stdlib.h"//常用的系统函数库
#include "sys.h"//系统中断分组库
#include "delay.h"//延时函数库
#include "bsp_usart.h"//串口设置库
#include "timer.h"//定时器驱动库
#include "oled.h"
#include "SHW.h"

#include "dht11.h"
#include "ds18b20.h"


unsigned char *empty="               ";
int nPwmPeroid = 1;
int gTempThresold = 30;
int gHumiThresold = 90;

u8 motorWorkFlag;
DHT11_Data_TypeDef dht11Data;
void SYS_Init(void);
int main(void) 
{
    char oledBuf[10];
    float fTemp;
    SYS_Init();//系统初始化总函数
    DS18B20_Init();
	while(1)   //主循环
	{
        fTemp= DS18B20_Get_Temp();
        printf("temp is %f \n",fTemp);
        
        //printf("Temperature is %d.%d, humidity is %d.%d.\n",dht11Data.temp_int,dht11Data.temp_deci,dht11Data.humi_int,dht11Data.humi_deci);
    }
    #if 0
        if(WATER_READ == 1) //下雨了
        {
            SWITCH1_OFF;
        }
        else{
            SWITCH1_ON;
        }

        //温湿度检测
        if(fTemp > gTempThresold || fHumi > gHumiThresold)
        {
            BEEP_ON;
            MOTOR_ON;
        }else
        {
            BEEP_OFF;
            MOTOR_OFF;
        }

        //虫子检测
        if(INS_READ == 1)
        {
            SWITCH2_ON; //打开喷药开关
        }
        else{
            SWITCH2_OFF; //关闭喷药开关
        }

        if(fLight < 500.0) //亮度很暗
        {
            nPwmPeroid = 21;
        }else if(fLight > 3000.0) //非常亮
        {
            nPwmPeroid = 2;
        }
        else //一般亮
        {
            nPwmPeroid = 3;
        }
  
        //
        //接收蓝牙发送过来的打开以及关闭指令
        if(Usart1_Receive.UsartFlag==1)
		{
			Usart1_Receive.UsartFlag=0;
            //printf("----%s\n",Usart1_Receive.Rebuf);

            if(Find_str((char*)Usart1_Receive.Rebuf,"On"))
            {
                BEEP_ON;
                
            }
            if(Find_str((char*)Usart1_Receive.Rebuf,"Off"))
            {
                BEEP_OFF;
                
            } 
            if(Find_str((char*)Usart1_Receive.Rebuf,"addTemp"))
            {
                gTempThresold +=1;
                
            }             
            if(Find_str((char*)Usart1_Receive.Rebuf,"subTemp"))
            {
                gTempThresold -=1;
                
            }
            if(Find_str((char*)Usart1_Receive.Rebuf,"addHumi"))
            {
                gHumiThresold +=1;
                
            }             
            if(Find_str((char*)Usart1_Receive.Rebuf,"subHumi"))
            {
               gHumiThresold -=1;
            }
            if(Find_str((char*)Usart1_Receive.Rebuf,"addSpeed"))
            {
                gHumiThresold +=1;
                
            }             
            if(Find_str((char*)Usart1_Receive.Rebuf,"subSpeed"))
            {
                gHumiThresold -=1;
            } 
        }
        if(HC_READ == 1) //检测到人存在
        {
            OLED_ShowString(0,0,"People detected",16,1);
            printf("People detected! \r\n");
        }
        else
        {
            OLED_ShowString(0,0,empty,16,1);
            OLED_ShowString(0,0,"No People!",16,1);
        }


        
        //显示工作状态
        if(motorWorkFlag){
            OLED_ShowString(0,32,"Working ...",16,1);
        }
         else
         {
             OLED_ShowString(0,0,empty,16,1);
             OLED_ShowString(0,16,empty,16,1);
             OLED_ShowString(0,32,"Sleeping...",16,1);
         }
        //OLED_ShowString(0,0,"012345678901234",16,1);    
        //OLED_ShowString(0,0,"               ",16,1);      
    #endif  
}
/**
  * @brief  系统初始化总函数
  * @param  无
  * @retval 无
  */
void SYS_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组函数
	delay_init();	    							 //延时函数初始化
    delay_ms(500);	    
	USART_Config();	 	                         //串口初始化为9600
    //OLED_Init();
	//beep_Init();	
    DHT11_Init(); //温湿度计控制
    //WATER_Init();   //水滴传感器
    //INS_Init();     //红外传感器
    //SWITCH_Init();  //喷药和关窗控制
	//LED_Init();  //电机控制开关，复用一下LED的

    //TIM1_PWM_Init(999,71);   //控制路灯亮度　不分频。PWM频率=72000/(899+1)=80Khz 
    //TIM3_Int_Init(99,719);
}

