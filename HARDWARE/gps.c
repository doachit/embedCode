#include "gps.h"
#include "delay.h"
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "bsp_usart.h"

GPS_Data gpsInfo;
static int first = 0;
static int setMode = 0;
float minX = 0;
float minY = 0;
float maxX = 0;
float maxY = 0;

#define fminf(a, b) ((a) < (b) ? (a) : (b))
#define fmaxf(a, b) ((a) > (b) ? (a) : (b))

static int is_valid_coordinate(float lat, float lon) {
    return !(isnan(lat) || isnan(lon) || fabs(lat) > 90.0f || fabs(lon) > 180.0f);
}

int check_pos(GPS_Data* data, float minX, float minY, float maxX, float maxY) {
    /* 所有变量提前定义 */
    //float actual_minX = 0.0f, actual_maxX = 0.0f;
    //float actual_minY = 0.0f, actual_maxY = 0.0f;
    int is_valid = 0;

    /* 参数有效性验证（符合图片中的错误数据检查） */
    //if(data == NULL) return 0;
    //if(!is_valid_coordinate(data->longitude, 0)) return 0; // 经度为X轴
    //if(!is_valid_coordinate(data->latitude, 1)) return 0;  // 纬度为Y轴

    /* 状态检查（根据图片status字段说明） */
    //if(data->status != 'A') return 0;

//    /* 规范化坐标范围（对应图片中的坐标系定义） */
//    actual_minX = fminf(minX, maxX); // X轴（经度）
//    actual_maxX = fmaxf(minX, maxX);
//    actual_minY = fminf(minY, maxY); // Y轴（纬度）
//    actual_maxY = fmaxf(minY, maxY);

    /* 区域判断（基于图片中的坐标系统） */
    is_valid = (data->longitude >= minX - 1e-6f) &&  // X轴比较
               (data->longitude <= maxX + 1e-6f) &&
               (data->latitude >= minY - 1e-6f) &&   // Y轴比较
               (data->latitude <= maxY + 1e-6f);

    return is_valid;
}

// 辅助函数：计算十六进制字符值
static uint8_t hex2num(char c) {
    if(isdigit(c)) return c - '0';
    return toupper(c) - 'A' + 10;
}

// 自定义安全长度检查函数（替代strnlen）
static uint8_t safe_strlen(const char* s, uint8_t max_len) {
    uint8_t len = 0;
    while(len < max_len && s[len] != '\0') len++;
    return len;
}

/* 校验和验证函数 */
static uint8_t validate_checksum(const char* data, uint16_t len)
{
    uint8_t calc_sum = 0;
    uint8_t check_mode = 0;
    uint16_t i = 0;
    uint8_t recv_sum = 0;
    char c = 0;
    
    for(i = 1; i < len; i++){ // 跳过$
        c = data[i];
        if(c == '*') {
            check_mode = 1;
            continue;
        }
        if(check_mode) break;
        calc_sum ^= c;
    }
    
    /* 提取校验值 */
    if(len < 3 || data[len-3] != '*') return 0;
    recv_sum = (hex2num(data[len-2]) << 4) | hex2num(data[len-1]);
    return (calc_sum == recv_sum);
}

uint8_t parse_GNGLL(const uint8_t* data, uint16_t len, GPS_Data* result)
{
    /* 所有变量在函数开始处定义 */
    uint16_t i = 0;
    uint8_t field_idx = 0;
    uint16_t field_start = 7; // 跳过$GNGLL,
    const char* fields[8] = {0};
    const char* lat_str = NULL;
    const char* lon_str = NULL;
    float lat_deg = 0.0f, lat_min = 0.0f;
    float lon_deg = 0.0f, lon_min = 0.0f;
    char temp_buf[12] = {0};
    
    /* 初始化输出结构 */
    memset(result, 0, sizeof(GPS_Data));
    
    /* 基础验证 */
    if(len < 20 || data[0] != '$') return 0;
    if(memcmp(data, "$GNGLL", 6) != 0) return 0;
    result->checksum_ok = validate_checksum(data, len);
    //if(!result->checksum_ok) return 0;

    /* 字段分割 */
    for(i = 7; i < len; i++) {
        if(data[i] == ',' || data[i] == '*') {
            fields[field_idx++] = &data[field_start];
            field_start = i + 1;
            if(field_idx >= 7) break; // 只需要前7个字段
        }
    }
    if(field_idx < 7) return 0;

    /* 解析纬度（格式：ddmm.mmmm） */
    lat_str = fields[0];
    if(safe_strlen(lat_str, 10) >= 4) {
        memcpy(temp_buf, lat_str, 2);
        lat_deg = (float)atoi(temp_buf);
        lat_min = atof(lat_str + 2);
        result->latitude = lat_deg + lat_min / 60.0f;
        if(*fields[1] == 'S') result->latitude = -result->latitude;
    }

    /* 解析经度（格式：dddmm.mmmm） */
    lon_str = fields[2];
    if(safe_strlen(lon_str, 11) >= 5) {
        memcpy(temp_buf, lon_str, 3);
        lon_deg = (float)atoi(temp_buf);
        lon_min = atof(lon_str + 3);
        result->longitude = lon_deg + lon_min / 60.0f;
        if(*fields[3] == 'W') result->longitude = -result->longitude;
    }

    /* 时间字段（hhmmss.sss） */
    strncpy(result->time, fields[4], sizeof(result->time)-1);

    /* 定位状态（A/V）和模式 */
    result->status = *fields[5];
    result->mode = *fields[6];

    return 1;
}
void copy_str(char *dst, const uint8_t* src){
    uint8_t idx = 0;
    char c;
    while(1) {
       c = src[idx];
       if (c == ',') break;
       dst[idx++] = c;
    }
    dst[idx] = '\0';
}
/* 主解析函数 */
uint8_t parse_GNRMC(const uint8_t* data, uint16_t len, GPS_Data* result)
{
    uint16_t i = 0;                // 循环变量
    uint8_t field_idx = 0;         // 字段索引
    uint8_t field_start = 0;
    const char* fields[7] = {0};   // 关键字段指针
    char temp_buf[15] = {0};        // 临时转换缓冲区
    float deg_part = 0.0f;         // 度数值
    float min_part = 0.0f;         // 分数值

    /* 初始化输出结构 */
    memset(result, 0, sizeof(GPS_Data));
    result->status = 'V'; // 默认无效
    /* 基础验证 */
    if(len < 40 || data[0] != '$') return 0;
    if(memcmp(data, "$GNRMC", 6) != 0) return 0;


    /* 校验和验证 */
    //if(!validate_checksum(data, len)) return 0;
    
    /* 字段分割（定位关键逗号位置） */
    for(i = 6, field_idx = 0, field_start = 7; i < len && field_idx < 7; i++) {
        if(data[i] == ',' || data[i] == '*') {
            fields[field_idx++] = &data[i+1];
            //field_start = i + 1;
        }
    }

    /* UTC时间（字段1） */
    if(fields[0] && fields[1] && (fields[1] - fields[0] - 1) >= 9) {
        strncpy(result->time, fields[0], 9);
    }

    /* 定位状态（字段2） */
    if(fields[1] && *fields[1] != '\0') {
        result->status = *fields[1];
    }

    /* 纬度解析（字段3+4） */
    if(fields[2] && fields[3]) {
        /* 解析度分格式 ddmm.mmmm */
        memcpy(temp_buf, fields[2], 2);
        temp_buf[2] = '\0';
        deg_part = (float)atoi(temp_buf);
        copy_str(temp_buf,fields[2]+2);
        min_part = atof(temp_buf);
        result->latitude = deg_part + min_part / 60.0f;
        if(*fields[3] == 'S') result->latitude = -result->latitude;
    }

    /* 经度解析（字段5+6） */
    if(fields[4] && fields[5]) {
        /* 解析度分格式 dddmm.mmmm */
        memcpy(temp_buf, fields[4], 3);
        temp_buf[3] = '\0';
        deg_part = (float)atoi(temp_buf);
        copy_str(temp_buf,fields[4]+3);
        min_part = atof(temp_buf);
        //min_part = atof(fields[4]+3);
        result->longitude = deg_part + min_part / 60.0f;
        if(*fields[5] == 'W') result->longitude = -result->longitude;
    }

    return 1;
}

void uart_parse_gps_info(uint8_t *data, uint16_t len) {
    //Usart_SendString(USART1,"Recv:");
    //Usart_SendArray(USART1,data,len);
    if(parse_GNRMC(data, len, &gpsInfo)) {
        //printf("\nLat: %.6f\n", gpsInfo.latitude);
        //////printf("Lon: %.6f\n", gpsInfo.longitude);
        //printf("Time: %s\n", gpsInfo.time);
       // printf("Status: %c\n", gpsInfo.status);
       // printf("Mode: %c\n", gpsInfo.mode);
    }
}

void UART2_GetData(USART_Object* obj)
{
	u8 recContent;   // 存储接收内容
	
	// 如果串口接收到内容
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		recContent = USART_ReceiveData(USART2);   // 存储接收内容
		//Usart_SendByte(USART1,recContent);
		// 如果接收到的是$（$是一帧信息的开始）
		// 保证每接收到新的一帧信息就从缓冲区起始位置开始存储
		if(recContent == '$')
		{
            memset(obj->Rebuff, 0, RBUF_MAX);
			obj->ReCount = 0;   // 清零帧信息计数变量
		}
		
		// 存储接收到的帧信息
		obj->Rebuff[obj->ReCount++] = recContent;
		
		// 确定是否收到"GPRMC/GNRMC"这一帧数据
		if(obj->Rebuff[0] == '$' && obj->Rebuff[4] == 'M' && obj->Rebuff[5] == 'C')
		{
            //Usart_SendByte(USART1,recContent);
            //Usart_SendByte(USART1,obj->ReCount);
			// 接收到换行（接收完了一帧信息）
			if(recContent == '\n')									   
			{
                 Usart_SendArray(USART1,obj->Rebuff, obj->ReCount);
				//memset(receDataFrame.Frame_Buffer, 0, Frame_Buffer_Length);   // 初始化接收帧信息数组
				//memcpy(receDataFrame.Frame_Buffer, gUart1RcecBuf, gReceCunt);   // 保存GPRMC/GNRMC这帧的数据
				obj->Flag = 1;   // 接收成功
                if(obj->Process) obj->Process(obj->Rebuff, obj->ReCount);
				obj->ReCount = 0;   // 清零接收帧信息接收计数变量
				memset(obj->Rebuff, 0, RBUF_MAX);   // 清空串口1接收Buf				
			}		
		}
		
		// 如果接收内容超出最大长度，不再继续接收
		if(obj->ReCount >= RBUF_MAX)
		{
			obj->ReCount = RBUF_MAX;
		}
	}
}