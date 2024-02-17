#ifndef __BOARD_MAX31856_H_
#define __BOARD_MAX31856_H_
#include "BoardCommBase.h"

//冷端转换分辨率计算系数,也就是芯片最小的那个计算单位代表的系数
#define COLD_JUNCTION_RESOLUTION        0.015625

//热端转换分辨率计算系数,也就是芯片最小的那个计算单位代表的系数
#define TC_RESOLUTION                   0.0078125 

//上下限分辨率转换
#define TC_LIMIT_RESOLUTION              16
//冷端温度分辨率
#define COOL_TEMP_RESOLUTION             64

//出错
#define MAX31856_CJ_Range_FAULT          (uint8_t)0x80//热电偶冷端温度超范围
#define MAX31856_TC_Range_FAULT          (uint8_t)0x40//热电偶热端温度超范围
#define MAX31856_CJHIGH_FAULT            (uint8_t)0x20//热电偶冷端到达温度上限
#define MAX31856_CJLOW_FAULT             (uint8_t)0x10//热电偶冷端到达温度下限
#define MAX31856_TCHIGH_FAULT            (uint8_t)0x08//热电偶热端到达温度上限
#define MAX31856_TCLOW_FAULT             (uint8_t)0x04//热电偶热端到达温度下限
#define MAX31856_OVUV_FAULT              (uint8_t)0x02//热电偶过压或者欠压,一般是短路
#define MAX31856_OPEN_FAULT              (uint8_t)0x01//热电偶开路,线路不通
#define MAX31856_NO_FAULT                (uint8_t)0x00//无故障 

//板上所有MAX31856初始化
void BoardMax31856AllInit(void);

//设置板上指定MAX31856的初始化配置
void BoardMax31856SetConfigDefault(CHANNEL_MAX31856 channel);

//设置指定通道MAX31856的冷端温度,也就是环境温度,从DS18B20中获取
void BoardMax31856SetCoolTemp(CHANNEL_MAX31856 channel,float environmentTemp);

//读取当前冷端温度与测量温度,返回当前错误代码数据,如果为0,代表无错误
uint8_t BoardMax31856_ReadCurentTemp(CHANNEL_MAX31856 channel,float* sensorTemperature);


#endif




