/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-16 16:26:41
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-16 19:01:18
 *FilePath: \LH_TC_BOOT_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplSensor.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_SENSOR_H_
#define __SERVICE_IMPL_SENSOR_H_
#include "ServiceImplBase.h"



//自动运行任务初始化
void ServiceImplSensor_StateMachineInit(void);


//自动运行任务
void ServiceImplSensor_StateMachineLoop(void);





#endif

