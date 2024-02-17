/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:14:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-12 15:12:21
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_2\TaskMain\AppSupport\AppIpc\AppIpcBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_BASE_H_
#define __APP_IPC_BASE_H_
#include "AppImplBase.h"

//杯栈管理
typedef struct DATA_IPC_CUP_TRAY_MANAGE
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_CUP_TRAY_MANAGE;

//磁珠针
typedef struct DATA_IPC_NEEDLE_BEAD
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_NEEDLE_BEAD;

//参数管理
typedef struct DATA_IPC_APP_PARAM
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_APP_PARAM;

//试剂盘
typedef struct DATA_IPC_PLATE_REAGENT
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_PLATE_REAGENT;

#endif





