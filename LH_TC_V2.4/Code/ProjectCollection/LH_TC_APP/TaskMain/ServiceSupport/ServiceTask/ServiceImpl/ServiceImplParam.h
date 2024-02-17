/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-17 11:45:28
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-03 14:20:29
 *FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PARAM_H_
#define __SERVICE_IMPL_PARAM_H_
#include "ServiceImplBase.h"


//自动化运行程序初始化
void ServiceImplParam_StateMachineInit(void);

//自动化运行程序循环运转
void ServiceImplParam_StateMachineLoop(void);

//读取参数
LH_ERR ServiceImplParam_Read(SERVICE_PARAM_PARAM_MAIN mainIndex,uint8_t subIndex,float* paramRead);

//写入参数
LH_ERR ServiceImplParam_Write(SERVICE_PARAM_PARAM_MAIN mainIndex,uint8_t subIndex,float paramWrite);


#endif
