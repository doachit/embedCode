#ifndef __GPS_H
#define __GPS_H	 
#include "sys.h"
 
typedef struct {
    float latitude;      // 十进制纬度
    float longitude;     // 十进制经度
    char time[10];       // UTC时间 hhmmss.sss
    char status;         // 定位状态 A=有效 V=无效
    char mode;           // 定位模式 
    uint8_t checksum_ok;// 校验结果 1=有效 0=无效
} GPS_Data;

extern void uart_parse_gps_info(uint8_t *data, uint16_t len);

extern GPS_Data gpsInfo;
extern float minX;
extern float minY;
extern float maxX;
extern float maxY;
#endif
