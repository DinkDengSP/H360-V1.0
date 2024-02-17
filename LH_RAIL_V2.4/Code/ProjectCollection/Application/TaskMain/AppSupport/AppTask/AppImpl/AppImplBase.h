/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:51:55
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-09 09:55:43
 *FilePath: \App\TaskMain\AppSupport\AppTask\AppImpl\AppImplBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_BASE_H_
#define __APP_IMPL_BASE_H_
#include "ServiceImplBase.h"
#include "ServiceIpcInc.h"
#include "AppParam.h"
#include "AppState.h"

//SampleRackIn任务
extern const uint8_t* SampleRackInDebugStartMsg;
extern const uint8_t* SampleRackInDebugEndMsg;

//SampleRackOut任务
extern const uint8_t* SampleRackOutDebugStartMsg;
extern const uint8_t* SampleRackOutDebugEndMsg;

//SampleRackMove任务
extern const uint8_t* SampleRackMoveDebugStartMsg;
extern const uint8_t* SampleRackMoveDebugEndMsg;

//StateParam任务
extern const uint8_t* StateParamDebugStartMsg;
extern const uint8_t* StateParamDebugEndMsg;

//辅助通用功能实现
LH_ERR AppImplBase_UtilCommon(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


#endif







