#include "key.h"
#include "delay.h"
#include "sys.h"
 	    
//������ʼ������ 
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	Init_GPIO_Clock(KEY_PORT);
    //Init_GPIO_Clock(GPIOC);
	//__HAL_RCC_GPIOA_CLK_ENABLE();
	//__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = KEY_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������

 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Pin  = KEY_PIN2;
 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin  = KEY_PIN3;
 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = KEY_PIN4;
 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0 || KEY2 == 0 || KEY3 == 0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
        else if(KEY2 == 0) return KEY2_PRES;
        else if(KEY3 == 0) return KEY3_PRES;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1)key_up=1; 	     
	return 0;// �ް�������
}


