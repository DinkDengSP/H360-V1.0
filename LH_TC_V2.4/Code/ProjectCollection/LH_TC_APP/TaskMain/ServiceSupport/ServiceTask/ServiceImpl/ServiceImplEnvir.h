/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-17 09:09:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-03 15:09:22
 *FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplEnvir.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_ENVIR_H_
#define __SERVICE_IMPL_ENVIR_H_
#include "ServiceImplBase.h"


//自动化运行程序初始化
void ServiceImplEnvir_StateMachineInit(void);

//自动化运行程序循环运转
void ServiceImplEnvir_StateMachineLoop(void);

//读取环境温度传感器状态
LH_ERR ServiceImplEnvir_ReadState(float* tempValue);

#endif




