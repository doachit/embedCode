#include "TM7711.h"

void HX710B_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置ADSK为推挽输出
    GPIO_InitStructure.GPIO_Pin = HX710B_ADSK_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HX710B_ADSK_GPIO_PORT, &GPIO_InitStructure);

    // 配置ADDO为输入带上拉
    GPIO_InitStructure.GPIO_Pin = HX710B_ADDO_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(HX710B_ADDO_GPIO_PORT, &GPIO_InitStructure);

    // 初始状态：ADSK拉低
    HX710B_ADSK_LOW();
}

int32_t HX710B_ReadCount(uint32_t timeout_ms) {
    uint32_t count = 0;
    uint8_t i;
    
    // 等待ADDO变低（数据就绪），带超时检测
    uint32_t timeout = timeout_ms * 1000;  // 转换为微秒级计数
    while(HX710B_ADDO_READ() != 0) {
        //if(timeout-- == 0) return 0x800000; // 返回错误值
        //delay_us(1);  // 需要实现微秒级延时函数
    }

    // 读取24位数据
    for(i=0; i<24; i++) {
        HX710B_ADSK_HIGH();      // 时钟上升沿
        delay_us(1);             // 保持高电平时间
        count <<= 1;
        
        HX710B_ADSK_LOW();       // 时钟下降沿
        delay_us(1);             // 保持低电平时间
        
        if(HX710B_ADDO_READ()) { // 读取数据位
            count++;
        }
    }

    // 第25个脉冲完成转换
    HX710B_ADSK_HIGH();
    delay_us(1);
    HX710B_ADSK_LOW();

    // 转换补码（HX710B输出为二进制补码）
    count ^= 0x800000;
    
    return (int32_t)count;
}