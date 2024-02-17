/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-19 11:58:40
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-19 12:59:14
 *FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceUtil\ServiceUtilPID.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_UTIL_PID_H_
#define __SERVICE_UTIL_PID_H_
#include "ServiceUtilMsg.h"

//清除PID系统状态
void ServicePidClearState(SERVICE_MSG_PID* pidState);

//PID计算
void ServicePidCalcOutput(SERVICE_MSG_PID* pidState,float currentValue,float* output);

#endif





