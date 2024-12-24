#ifndef _ITO_TIMER3_PING_H
#define _ITO_TIMER3_PING_H
#include "sys.h"
void TIM3_ENABLE_10MS(void);
extern int SystemTimer;							//控制上传用户数据的时间
extern unsigned int Length_of_time; //控制发送PING报文的时间
extern char Ping_flag;         //外部变量声明，ping报文状态      0：正常状态，等待计时时间到，发送Ping报文
                               //外部变量声明，ping报文状态      1：Ping报文已发送，当收到 服务器回复报文的后 将1置为0
#endif



