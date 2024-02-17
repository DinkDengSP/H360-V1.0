/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:34:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-14 16:40:10
 *FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppIpc\AppIpcBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_BASE_H_
#define __APP_IPC_BASE_H_
#include "AppImplBase.h"

//HAND_NEW_CUP
typedef struct DATA_IPC_HAND_NEW_CUP
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_HAND_NEW_CUP;

//NEEDLE_SAMPLE
typedef struct DATA_IPC_NEEDLE_SAMPLE
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_NEEDLE_SAMPLE;

//SAMPLE_RACK_TRACK
typedef struct DATA_IPC_SAMPLE_RACK_TRACK
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_SAMPLE_RACK_TRACK;

//APP_PARAM
typedef struct DATA_IPC_APP_PARAM
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_APP_PARAM;


#endif





