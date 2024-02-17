/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-20 17:03:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-11-12 17:20:44
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplBaseConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_BASE_CONFIG_H_
#define __SERVICE_IMPL_BASE_CONFIG_H_
#include "SystemInput.h"
#include "SystemOutput.h"
#include "Can2SubRailBase.h"

/***************************LED点亮熄灭的电平*****************/
#define VALID_LEVEL_OUTPUT_RAIL_LED_ON                   Bit_RESET
#define VALID_LEVEL_OUTPUT_RAIL_LED_OFF                  Bit_SET

/***************************急诊电子锁电平*****************/
#define VALID_LEVEL_OUTPUT_RAIL_EMERGENCY_LOCK           Bit_RESET
#define VALID_LEVEL_OUTPUT_RAIL_EMERGENCY_UNLOCK         Bit_SET

//轨道传感器信号有效
typedef enum RAIL_SENSOR_STATE
{
    RAIL_SENSOR_INVALID = 0,//传感器无效
    RAIL_SENSOR_VALID   = 1,//传感器状态有效
}RAIL_SENSOR_STATE;

//出样的状态
typedef enum RAIL_PUSH_OUT_RESULT
{
    PUSH_OUT_NORMAL     = 0,//出样正常
    PUSH_OUT_FULL       = 1,//出样满
    PUSH_OUT_BLOCK      = 2,//出样堵住
}RAIL_PUSH_OUT_RESULT;

//输出状态
typedef enum RAIL_OUTPUT_STATE
{
    RAIL_OUTPUT_STATE_INVALID = 0,//输出无效
    RAIL_OUTPUT_STATE_VALID   = 1,//输出有效
}RAIL_OUTPUT_STATE;

//出样盘将架子从末端推到顶端的最大推进次数
#define COUNT_PUSH_OUT_RACK_TOP_MAX                      30


#endif




