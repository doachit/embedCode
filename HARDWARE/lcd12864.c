#include "lcd12864.h"
#include "delay.h"

uint8_t str[25];//用于整型转字符型的中间数组

// n <2的32次数，所以最大10位
// n 可能为负数，也可能为正数
//整型转字符型
void int2str(int n, uint8_t *str)
{
    char buf[10] = "";
    int i = 0;
    int len = 0;
    // temp为n的绝对值
    int temp = n<0?-n:n;
 
 
    // 如果str为空，直接返回
    if(str == NULL)
    {
     return ;
    }
    //把tmp的每一位的数存入buf
    // 逆序存入，如1234 -> 在buf中为4321
    while(temp)
    {
     buf[i++] = (temp%10) + '0';
     temp = temp / 10;
    }
    //如果是负数多留一位放符串
    len = n<0?++i:i;
    str[i] = 0;
    //反转字符串4321- 转成 -1234
    while(1)
    {
     i--;
     if(buf[len-i-1] == 0)
     {
        break;
     }
     str[i] = buf[len-i-1];
    }
    // 如果是符号，加回去
    if(i==0)
    {
        str[i] = '-';
    }
}


/*! 
*  @brief      GPIO_init
 *  @since      v1.0
 *  @param  None
 *  @author     Z小旋
 *  使用时自行修改这里的初始化即可
 */
void lcd_GPIO_init()
{
 
  GPIO_InitTypeDef GPIO_InitStructure;	
  //RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PC端口时钟
  INIT_GPIO_CLOCK(LCD_SID_port);
  INIT_GPIO_CLOCK(LCD_SCLK_port);
	
  GPIO_InitStructure.GPIO_Pin = LCD_SID_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置GPIO模式,输入上拉       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_SID_port, &GPIO_InitStructure);  //初始化PC端口

  GPIO_InitStructure.GPIO_Pin = LCD_SCLK_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置GPIO模式,输入上拉       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_SCLK_port, &GPIO_InitStructure);  //初始化PC端口		
	
  SID=1;
  SCLK=1;
}

/* 字符显示RAM地址    4行8列 */
u8 LCD_addr[4][8]={
	{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},  		//第一行
	{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},		//第二行
	{0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},		//第三行
	{0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}		//第四行
	};

/*! 
*  @brief      LCD串行发送一个字节
 *  @since      v1.0
 *  @param  byte   写入字节
 *  @author     Z小旋
 */
void SendByte(u8 byte)
{
     u8 i; 
	  for(i = 0;i < 8;i++)
    {
			if((byte << i) & 0x80)  //0x80(1000 0000)  只会保留最高位
			{
				SID = 1;           // 引脚输出高电平，代表发送1
			}
			else
			{
				SID = 0;         // 引脚输出低电平，代表发送0
			}
		/*或		
			SID =	(Dbyte << i) & 0x80;
				
			上面那样为了方便理解
		*/
			SCLK = 0;   //时钟线置低  允许SID变化
			delay_us(5); //延时使数据写入
			SCLK = 1;    //拉高时钟，让从机读SID
		}   
}


/*! 
 *  @brief      LCD写指令
 *  @since      v1.0
 *  @param  Cmd   要写入的指令
 *  @author     Z小旋
 */
void Lcd_WriteCmd(u8 Cmd )
{
     delay_ms(1);     //由于我们没有写LCD正忙的检测，所以直接延时1ms，使每次写入数据或指令间隔大于1ms 便可不用写忙状态检测
     SendByte(WRITE_CMD);            //11111,RW(0),RS(0),0   
     SendByte(0xf0&Cmd);      //高四位
     SendByte(Cmd<<4);   //低四位(先执行<<)
}

/*! 
 *  @brief      LCD写数据
 *  @since      v1.0
 *  @param  Dat   要写入的数据
 *  @author     Z小旋
 */
void Lcd_WriteData(u8 Dat )
{
     delay_ms(1);     
     SendByte(WRITE_DAT);            //11111,RW(0),RS(1),0
     SendByte(0xf0&Dat);      //高四位
     SendByte(Dat<<4);   //低四位(先执行<<)
}
/*! 
 *  @brief      LCD初始化
 *  @since      v1.0
 *  @param  None
 *  @author     Z小旋
 */
void Lcd_Init(void)
{ 
    lcd_GPIO_init();

    delay_ms(50);   	//等待液晶自检（延时>40ms）
    Lcd_WriteCmd(0x30);        //功能设定:选择基本指令集
    delay_ms(1);//延时>100us
    Lcd_WriteCmd(0x30);        //功能设定:选择8bit数据流
    delay_ms(1);	//延时>37us
    Lcd_WriteCmd(0x0c);        //开显示
    delay_ms(1);	//延时>100us
    Lcd_WriteCmd(0x01);        //清除显示，并且设定地址指针为00H
    delay_ms(30);	//延时>10ms
    Lcd_WriteCmd(0x06);        //进入设定点，初始化完成
}


/*! 
 *  @brief      显示字符或汉字
 *  @since      v1.0
 *  @param  x: row(0~3)
 *  @param  y: line(0~7) 
 *  @param 	str: 要显示的字符或汉字
 *  @author     Z小旋
 */
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str)  //空格和数字占半个字符，必须使汉字在一个整的字符格内，不然会乱码
{ 
	Lcd_WriteCmd(LCD_addr[x][y]); //写初始光标位置
	while(*str>0)   //字符串的末尾会有‘/0’标志结束 
    { 
      Lcd_WriteData(*str);    //写数据
      str++;     
    }
}
/*! 
 *  @brief      显示图片
 *  @since      v1.0
 *  @param  *pic   图片地址
 *  @author     
 */
void LCD_Display_Picture(uint8_t *img)
	{
		uint8_t x,y,i;
		Lcd_WriteCmd(0x34);		//切换到扩充指令
		Lcd_WriteCmd(0x34);		//关闭图形显示
		for(i = 0; i < 1; i++)   //上下屏写入
		{
			for(y=0;y<32;y++)   //垂直Y写32次
			{  
				for(x=0;x<8;x++)   //横向X写8次
				{
					Lcd_WriteCmd(0x80 + y);		//行地址
					Lcd_WriteCmd(0x80 + x+i);		//列地址
					Lcd_WriteData(*img ++);		//写高位字节数据 D15－D8   
					Lcd_WriteData(*img ++);		//写低位字节数据 D7－D0
				}
			}
		}
		Lcd_WriteCmd(0x36);//打开图形显示		
		Lcd_WriteCmd(0x30);        //切换回基本指令
	}	
/*! 
 *  @brief      清屏函数
 *  @since      v1.0
 *  @param  None
 *  @author     Z小旋
 */
void LCD_Clear(void)
	{
		Lcd_WriteCmd(0x01);			//清屏指令
		delay_ms(2);				//延时以待液晶稳定【至少1.6ms】
	}
	
