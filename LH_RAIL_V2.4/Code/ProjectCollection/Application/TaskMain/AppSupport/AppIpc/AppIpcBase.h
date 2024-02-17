/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:57:30
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-20 15:10:42
 *FilePath: \App\TaskMain\AppSupport\AppIpc\AppIpcBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_BASE_H_
#define __APP_IPC_BASE_H_
#include "AppImplBase.h"

//SAMPLE_RACK_IN
typedef struct IPC_DATA_SAMPLE_RACK_IN
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1ErrorCode;
    //中位机错误代码的级别
    CAN1_REPORT_ERR_LEVEL can1ReportErrorLevel;
}IPC_DATA_SAMPLE_RACK_IN;

//SAMPLE_RACK_OUT
typedef struct IPC_DATA_SAMPLE_RACK_OUT
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1ErrorCode;
    //中位机错误代码的级别
    CAN1_REPORT_ERR_LEVEL can1ReportErrorLevel;
}IPC_DATA_SAMPLE_RACK_OUT;

//SAMPLE_RACK_MOVE
typedef struct IPC_DATA_SAMPLE_RACK_MOVE
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1ErrorCode;
    //中位机错误代码的级别
    CAN1_REPORT_ERR_LEVEL can1ReportErrorLevel;
}IPC_DATA_SAMPLE_RACK_MOVE;

//STATE_PARAM
typedef struct IPC_DATA_STATE_PARAM
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1ErrorCode;
    //中位机错误代码的级别
    CAN1_REPORT_ERR_LEVEL can1ReportErrorLevel;
}IPC_DATA_STATE_PARAM;





#endif





