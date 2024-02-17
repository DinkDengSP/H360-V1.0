/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:09:49
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 16:27:30
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_PRESSURE_SENSOR_H_
#define __BOARD_PRESSURE_SENSOR_H_
#include "MCU_Inc.h"

typedef enum BOARD_PRESSURE_CHANNEL
{
    BOARD_PRESSURE_CHANNEL1 = ADC_CHANNEL_10,
    BOARD_PRESSURE_CHANNEL2 = ADC_CHANNEL_11,
    BOARD_PRESSURE_CHANNEL3 = ADC_CHANNEL_12,
    BOARD_PRESSURE_CHANNEL4 = ADC_CHANNEL_13,
}BOARD_PRESSURE_CHANNEL;

//压力传感器检测接口初始化
void BoardPressureSensorInit(void);

//压力传感器读取当前值
uint16_t BoardPressureSensorReadValueSingle(BOARD_PRESSURE_CHANNEL channel);

//压力传感器读取多次值并平均
uint16_t BoardPressureSensorReadValueAverage(BOARD_PRESSURE_CHANNEL channel,uint16_t count);

#endif

