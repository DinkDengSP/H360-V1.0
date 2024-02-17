/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:23
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-15 10:55:20
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_StepMotor_Input_Output_Test\TaskMain\AppSupport\AppIpc\AppIpcBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_BASE_H_
#define __APP_IPC_BASE_H_
#include "AppImplBase.h"
#include "ServiceIpcBase.h"

//OtherUtil
typedef struct DATA_IPC_OTHER
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_OTHER;

#endif





