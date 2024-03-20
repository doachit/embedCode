#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "HC_SR04.h"
#include "beep.h"
#include "key.h"
#include "DS1302.h"


//设定距离阈值
#define DIS_THRESHOLD   (30.0) 

#define TIME_TO_VALUE(hour,minute)  (hour*60+minute)

enum STR_TYPE
{
    INVALID_TIME = 0,
    HAS_PEOPLE = 8,
    OFFLINE = 16,
    NO_PEOPLE = 24
};
void showMsg(int type)
{
    int i;
    for(i=0;i<8;i++)
    {
        OLED_ShowChinese(i*16,0,type+i,16,1);
    }
}
 int main(void)
 {		
    char str[40];
    float fDistance=0.0;
    u8 startHour=0,startMinute=12,endHour=22,endMinute=30; //起始结束时间
    u8 key;
	delay_init();	    	 //延时函数初始化
    delay_ms(500);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
     printf("Receive Data\n");
     while(1);
    OLED_Init();
    OLED_ColorTurn(0);
    OLED_DisplayTurn(0);
    beep_Init();
    KEY_Init();
    DS1302_Init();
    HC_SR04config();

     //校准时间
     while(SET_TIME_ENABLE)
     {
         //按键处理
        key = KEY_Scan(1);
        switch(key)
        {				 
            case KEY0_PRES:	//mon
            {
                time_buf[1] += 1;
                time_buf[1] %=12;
            }
            break;

            case KEY1_PRES:	//day
            {
                time_buf[2] += 1;
                time_buf[2] %=31;
            }
            break;
            case KEY2_PRES: //hour
            {
                time_buf[3] +=1;
                time_buf[3] %=24;
            }
            break;
            case KEY3_PRES://min
            {
                time_buf[4] +=1;
                time_buf[4] %=60;

            }
            break;
            default:break;
        }
        
         sprintf(str,"%d-%02d-%02d %02d:%02d", time_buf[0],time_buf[1],time_buf[2],time_buf[3],time_buf[4]);
         OLED_ShowString(0,48,(u8 *)str,16,1);
         DS1302_WriteTime();
         OLED_ShowString(0,20,"Set time",16,1);
         OLED_Refresh();
         delay_ms(100);
     }
    while(1) printf("Distance: \n");
     while(1)
    {
        
        DS1302_GetTime(); //获取当前时间
        sprintf(str,"%d-%02d-%02d %02d:%02d", TimeData.year,TimeData.month,TimeData.day,TimeData.hour,TimeData.minute);
        OLED_ShowString(0,48,(u8 *)str,16,1);

        printf("time : %s\n",str);	
        
        //按键处理
        key = KEY_Scan(1);
        switch(key)
        {				 
            case KEY0_PRES:	
            {
                startHour += 1;
                startHour %=24;
            }
            break;

            case KEY1_PRES:	
            {
                endHour += 1;
                endHour %=24;
            }
            break;
            case KEY2_PRES:
            {
                endMinute +=1;
                endMinute %=60;
            }
            break;
            case KEY3_PRES:
            {
                startMinute +=1;
                startMinute %=60;

            }
            break;
            default:break;
        }
        
        sprintf(str,"  %02d:%02d-%02d:%02d", startHour,startMinute,endHour,endMinute);
        OLED_ShowString(0,20,(u8 *)str,16,1);
      
        if(TIME_TO_VALUE(startHour,startMinute) >= TIME_TO_VALUE(endHour,endMinute))//无效时间：起始时间大于结束时间
        {
            //设置时间无效
            showMsg(INVALID_TIME);
        }
        else 
        {
            if(TIME_TO_VALUE(startHour,startMinute)> TIME_TO_VALUE(TimeData.hour,TimeData.minute) || 
                TIME_TO_VALUE(TimeData.hour,TimeData.minute) > TIME_TO_VALUE(endHour,endMinute)) //不在设定的时间段内，报警系统不工作
            {
                //非工作时间
                showMsg(OFFLINE);
            }
            else
            {
                fDistance =  get_distance();//超声波距离测量
                printf("Distance: %f\n",fDistance);
                if(floatCompare(DIS_THRESHOLD,fDistance) > 0)
                {
                    //人已离开
                    showMsg(NO_PEOPLE);
                    BEEP_ON;
                }
                else
                {
                    //人存在
                    BEEP_OFF;
                    showMsg(HAS_PEOPLE);
                }
            }
        }
        delay_ms(50);
        OLED_Refresh();
    }   
 }



