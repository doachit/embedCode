#include "Sound.h"

uint8_t soundsbuf[]={0x7E,0x03,0x13,0x10,0xEF};
uint8_t soundsplay[7]={0x7E,0x05,0x41};
u8 soundNumber[]={0,1,2,3,4,5,6,7,8,9};

int sounds[][2] = {
    {0,0}, //empty
    {1,1}, // 0
    {2,1}, //1
    {3,1}, //2
    {4,1}, //3
    {5,1}, //4
    {6,1}, //5
    {7,1}, //6
    {8,1}, //7 
    {9,1},  //8
    {10,1}, //9
    {11,3}, //体温为 (3个字)
    {12,1}, //点
    {13,3}, //摄氏度
    {14,4}, //体温正常
    {15,4}  //体温异常
};
void soundsContrl(USART_TypeDef * pUSARTx,uint16_t number)
{
	u8 id;
	soundsplay[3] = sounds[number][0] &  0xff00;
	soundsplay[4] = sounds[number][0] &  0x00ff;
	soundsplay[5] = soundsplay[1]^soundsplay[2]^soundsplay[3]^soundsplay[4];
	soundsplay[6] = 0xEF;	
	//HAL_UART_Transmit(&huart1,soundsplay,8,HAL_MAX_DELAY);
    for(id = 0; id < 8;++id)
    {
        Usart_SendByte(pUSARTx,soundsplay[id]);
        delay_ms(10);
    }
	soundsplay[3] = 0;
	soundsplay[4] = 0;
	soundsplay[5] = 0;
	soundsplay[6] = 0;
    for(id = 0; id < sounds[number][1];id++)
    {
        delay_ms(500);
    }
}
