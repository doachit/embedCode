#ifndef __ITO_USER_H
#define __ITO_USER_H
#include "sys.h"  //������Ҫ��ͷ�ļ�

void Timed_Data_UpLoad(void);//��ʱ�ϴ�����
u32 Extract_digit(u8 *str);
void Switch_Sta(void *str);//�������ص�״̬�������� 


/******�������е���***********/
extern void IOT_Connect(void);
extern void IOT_init(void);

#endif







