#ifndef __LIQUID_DATA_PROC_H_
#define __LIQUID_DATA_PROC_H_
#include "BaseInclude.h"

//传感器数据
//基准传感器数据
extern uint16_t baseLiquidSensorValue;
//跟随传感器数据
extern uint16_t followLiquidSensorValue;
//跟随传感器滤波数据
extern uint16_t filterFollowLiquidSensorValue;

//液面探测标志位
extern uint8_t liquidSignalTrigFlag;
//液面探测触发事件
extern uint32_t liquidSignalTrigStamp;

//液面探测数据处理相关的变量初始化
void LiquidDataProcInit(void);

//更新传感器数据
void LiquidReflushSensorValue(void);

//液面探测相关数据的清除
void LiquidDataProcClear(void);

//计算探液结果
uint8_t LiquidDataProcessADConvertResult(void);

#endif




