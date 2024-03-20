#include "ds1302.h"
#include "delay.h"
#include "usart.h"

struct TIMEData TimeData;

 
//初始时间定义
u16 time_buf[8] = {2023,4,24,13,0,0,0};//初始时间2023年4月24号13点0分0秒
u8 time_readbuf[8]={0x20};//读取时间
u8 readtime[15];//当前时间
u8 sec_buf=0;  //秒缓存
u8 sec_flag=0; //秒标志位
 
//DS1302初始化函数
void DS1302_Init() 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = RST_Pin; 				//RST
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_Init(RST_GPIO, &GPIO_InitStructure);			//初始化
	GPIO_ResetBits(RST_GPIO,RST_Pin); 
	 
	GPIO_InitStructure.GPIO_Pin = SCLK_Pin; 			//SCLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_Init(SCLK_GPIO, &GPIO_InitStructure);			//初始化
	GPIO_ResetBits(SCLK_GPIO,SCLK_Pin);  
}

/*
 * 数据端口输出配置
 */
void DS1302_OutPut_Mode()//配置双向I/O端口为输出态
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 
	GPIO_InitStructure.GPIO_Pin = DATA_Pin; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DATA_GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(DATA_GPIO,DATA_Pin);
}
 
 /*
  * 数据端口输入配置
  */
void DS1302_InPut_Mode()//配置双向I/O端口为输入态
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DATA_Pin; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DATA_GPIO, &GPIO_InitStructure);
}

//向DS1302写入一字节数据
void DS1302_WriteByte(u8 addr,u8 data) 
{
	u8 i;
	DS1302_RST=0; //禁止数据传输 ！！！这条很重要
	DS1302_CLK=0; //确保写数据前SCLK为低电平
	DS1302_RST=1;	//启动DS1302总线	
	DS1302_OutPut_Mode();
	addr=addr&0xFE;  //最低位置零，寄存器0位为0时写，为1时读
	for(i=0;i<8;i++) //写入目标地址：addr
	{
		if (addr&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1; //时钟上升沿写入数据
		DS1302_CLK=0;
		addr=addr>>1;
	}	
	for (i=0;i<8;i++) //写入数据：data
	{
		if(data&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1;    //时钟上升沿写入数据
		DS1302_CLK=0;
		data = data >> 1;
	}
	DS1302_CLK=1;    // 将时钟电平置于高电平状态 ，处于已知状态
	DS1302_RST=0;	//停止DS1302总线
}
 
//从DS1302读出一字节数据
u8 DS1302_ReadByte(u8 addr) 
{
	u8 i,temp;	
	DS1302_RST=0; 			//这条很重要
	DS1302_CLK=0; 			//先将SCLK置低电平,确保写数居前SCLK被拉低
	DS1302_RST=1; 			//启动DS1302总线
	DS1302_OutPut_Mode();
	//写入目标地址：addr
	addr=addr|0x01; //最低位置高，寄存器0位为0时写，为1时读
	for(i=0;i<8;i++) 
	{
		if (addr&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1; //写数据
		DS1302_CLK=0;
		addr = addr >> 1;
	}	
	//从DS1302读出数据：temp
	DS1302_InPut_Mode();
	for(i=0;i<8;i++)
	{
		temp=temp>>1;
		if (DS1302_IN) temp|=0x80;
		else temp&=0x7F;
		DS1302_CLK=1;
		DS1302_CLK=0;
	}	
	DS1302_CLK=1;  //将时钟电平置于已知状态
	DS1302_RST=0;	//停止DS1302总线
	return temp;
}
//向DS1302写入时钟数据,用于时间校准修改
void DS1302_WriteTime() 
{
    u8 year,mon,day,hour,min,sec;
    year = time_buf[0];
    mon = time_buf[1];
    day = time_buf[2];
    hour = time_buf[3];
    min = time_buf[4];
    sec = time_buf[5];
    
    year = year - 2000; //年默认2000开始
	if(year > 100) year = 0; 
	
	// 10转为bdc码
	year = ((year /10) << 4) + year %10;
	mon  = ((mon  /10) << 4) + mon  %10;
	day  = ((day  /10) << 4) + day  %10;
	hour = ((hour /10) << 4) + hour %10;
	min  = ((min  /10) << 4) + min  %10;
	sec  = ((sec  /10) << 4) + sec  %10;

    
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x00);       //关闭写保护 
	DS1302_WriteByte(DS1302_SEC_ADDR,0x80);           //暂停时钟 
	//DS1302_WriteByte(DS1302_CHARGER_ADDR,0xa9);     //涓流充电 
	DS1302_WriteByte(DS1302_YEAR_ADDR,year);   //年 
	DS1302_WriteByte(DS1302_MONTH_ADDR, mon);  //月 
	DS1302_WriteByte(DS1302_DAY_ADDR,day);    //日 
	DS1302_WriteByte(DS1302_HOUR_ADDR,hour);   //时 
	DS1302_WriteByte(DS1302_MIN_ADDR,min);    //分
	DS1302_WriteByte(DS1302_SEC_ADDR,sec);    //秒
//	DS1302_WriteByte(DS1302_WEEK_ADDR,time_buf[7]);	  //周 
//	DS1302_WriteByte(DS1302_CHARGER_ADDR,0xA5);			//打开充电功能 选择2K电阻充电方式
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x80);			//打开写保护     
}
//从DS1302读出时钟数据
void DS1302_ReadTime(void)  
{
	time_readbuf[1]=DS1302_ReadByte(DS1302_YEAR_ADDR);          //年 
	time_readbuf[2]=DS1302_ReadByte(DS1302_MONTH_ADDR);         //月 
	time_readbuf[3]=DS1302_ReadByte(DS1302_DAY_ADDR);           //日 
	time_readbuf[4]=DS1302_ReadByte(DS1302_HOUR_ADDR);          //时 
	time_readbuf[5]=DS1302_ReadByte(DS1302_MIN_ADDR);           //分 
	time_readbuf[6]=(DS1302_ReadByte(DS1302_SEC_ADDR))&0x7f;    //秒，屏蔽秒的第7位，避免超出59
	time_readbuf[7]=DS1302_ReadByte(DS1302_WEEK_ADDR);          //周 	
}
 
//DS1302获取完时间后进行数据的计算
void DS1302_GetTime()
{ 
    DS1302_ReadTime(); //读取时间
    TimeData.year	=(time_readbuf[0]>>4)*1000	+(time_readbuf[0]&0x0F)*100+(time_readbuf[1]>>4)*10+(time_readbuf[1]&0x0F); //计算年份
    TimeData.month	=(time_readbuf[2]>>4)*10	+(time_readbuf[2]&0x0F);  	//计算月份
    TimeData.day	=(time_readbuf[3]>>4)*10	+(time_readbuf[3]&0x0F);    //计算日期
    TimeData.hour	=(time_readbuf[4]>>4)*10	+(time_readbuf[4]&0x0F);   	//计算小时
    TimeData.minute	=(time_readbuf[5]>>4)*10	+(time_readbuf[5]&0x0F); 	//计算分钟
    TimeData.second	=(time_readbuf[6]>>4)*10	+(time_readbuf[6]&0x0F); 	//计算秒钟
    TimeData.week	=(time_readbuf[7]&0x0F);                       		//计算星期																										
}
