/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-17 09:08:05
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-03 15:37:48
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplPlateReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef _SERVICE_IMPL_PLATEREAGENT_VERSION1_H_
#define _SERVICE_IMPL_PLATEREAGENT_VERSION1_H_
#include "ServiceImplBase.h"

//自动化运行程序初始化
void ServiceImplPlateReagent_StateMachineInit_Version1(void);

//自动化运行程序循环运转
void ServiceImplPlateReagent_StateMachineLoop_Version1(void);

//读取试剂盘温度传感器状态
LH_ERR ServiceImplPlateReagent_ReadState_Version1(float* tempValue);

//读取制冷机当前温度
LH_ERR ServiceImplPlateReagent_ReadCoolerState_Version1(uint8_t *coolerState,int16_t *currentTemper,int16_t * compressorRpm,int16_t *setTemper);

//设置制冷机当前温度
LH_ERR ServiceImplPlateReagent_SetCoolerState_Version1(uint8_t setCoolerState,uint8_t *coolerState);
#endif