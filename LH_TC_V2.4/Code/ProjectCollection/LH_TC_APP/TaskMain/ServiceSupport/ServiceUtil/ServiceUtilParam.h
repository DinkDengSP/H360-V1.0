/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-17 10:02:27
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-03 14:02:13
 *FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceUtil\ServiceUtilParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_UTIL_PARAM_H_
#define __SERVICE_UTIL_PARAM_H_
#include "ServiceUtilParamDefault.h"
#include "ServiceUtilParamIndex.h"

//导出的配置文件
extern SERVICE_PARAM* serviceParam;

//参数初始化
void ServiceUtilParamInit(void);

//读取参数
LH_ERR ServiceUtilParam_Read(SERVICE_PARAM_PARAM_MAIN mainIndex,uint8_t subIndex,float* paramRead);

//写入参数
LH_ERR ServiceUtilParam_Write(SERVICE_PARAM_PARAM_MAIN mainIndex,uint8_t subIndex,float paramWrite);

#endif




