/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-17 14:52:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-17 14:55:22
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplAssistUtilActionParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_ASSIST_UTIL_ACTION_PARAM_H_
#define __SERVICE_IMPL_ASSIST_UTIL_ACTION_PARAM_H_
#include "ServiceImplBase.h"


//读取动作参数
LH_ERR ServiceImplAssistUtilActionParam_Read(uint8_t mainIndex,uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入动作参数
LH_ERR ServiceImplAssistUtilActionParam_Write(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);



#endif





