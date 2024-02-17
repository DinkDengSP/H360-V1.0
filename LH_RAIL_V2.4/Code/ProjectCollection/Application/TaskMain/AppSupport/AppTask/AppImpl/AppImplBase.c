/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:51:45
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-20 14:45:59
**FilePath: \App\TaskMain\AppSupport\AppTask\AppImpl\AppImplBase.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplBase.h"

//SampleRackIn任务
const uint8_t* SampleRackInDebugStartMsg = "$$Action 15 Start : ";
const uint8_t* SampleRackInDebugEndMsg = "$$Action 15 End ";

//SampleRackOut任务
const uint8_t* SampleRackOutDebugStartMsg = "$$Action 16 Start : ";
const uint8_t* SampleRackOutDebugEndMsg = "$$Action 16 End ";

//SampleRackMove任务
const uint8_t* SampleRackMoveDebugStartMsg = "$$Action 17 Start : ";
const uint8_t* SampleRackMoveDebugEndMsg = "$$Action 17 End ";

//StateParam任务
const uint8_t* StateParamDebugStartMsg = "$$Action 18 Start : ";
const uint8_t* StateParamDebugEndMsg = "$$Action 18 End ";

//辅助通用功能实现
LH_ERR AppImplBase_UtilCommon(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    return LH_ERR_NONE;
}
