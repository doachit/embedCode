#ifndef __ITO_USER_H
#define __ITO_USER_H
#include "iot_common.h"


void Timed_Data_UpLoad(void);//定时上传任务
u32 Extract_digit(u8 *str);
void Switch_Sta(void *str);//发布开关的状态给服务器 


/******主函数中调用***********/
extern void IOT_Connect(void);
extern void IOT_init(void);

#endif







