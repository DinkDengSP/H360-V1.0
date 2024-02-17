/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:29:23
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-20 15:19:13
 *FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_BASE_H_
#define __SERVICE_IPC_BASE_H_
#include "ServiceImplBase.h"

//系统内部指令处理的阶段定义
//系统动作调用API,分为三种 
//1. 发送动作并等待ACK(ACK无错误代表动作开始执行)  
//2. 发送动作并等待结果(无错误代表动作执行完成)  
//3. 读取上一次动作的结果
//如果调用了发送动作并等待ACK,那么同一个模块在下次发送动作之前必须读取上一次的动作结果,否则后一次调用动作报错.
typedef enum STATE_IPC
{
    STATE_IPC_IDLE = 0,        //空闲
    STATE_IPC_WAIT_ACK = 1,    //等待ACK
    STATE_IPC_WAIT_RETURN = 2, //等待结果
    STATE_IPC_CMD_COMPLETE = 3,//执行完成
}STATE_IPC;

//RAIL_PUSH_IN1
typedef struct IPC_DATA_RAIL_PUSH_IN1
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
}IPC_DATA_RAIL_PUSH_IN1;

//RAIL_PUSH_IN2
typedef struct IPC_DATA_RAIL_PUSH_IN2
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
}IPC_DATA_RAIL_PUSH_IN2;

//RAIL_PUSH_OUT1
typedef struct IPC_DATA_RAIL_PUSH_OUT1
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
}IPC_DATA_RAIL_PUSH_OUT1;

//RAIL_PUSH_OUT2
typedef struct IPC_DATA_RAIL_PUSH_OUT2
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
}IPC_DATA_RAIL_PUSH_OUT2;

//RAIL_HAND
typedef struct IPC_DATA_RAIL_HAND
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
}IPC_DATA_RAIL_HAND;

//RAIL_TRANS
typedef struct IPC_DATA_RAIL_TRANS
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
}IPC_DATA_RAIL_TRANS;

//RAIL_COMPLEX
typedef struct IPC_DATA_RAIL_COMPLEX
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
}IPC_DATA_RAIL_COMPLEX;

//BAR_SCAN
typedef struct IPC_DATA_BAR_SCAN
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
}IPC_DATA_BAR_SCAN;

//DEBUG
typedef struct IPC_DATA_DEBUG
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
}IPC_DATA_DEBUG;

//HEART
typedef struct IPC_DATA_HEART
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
}IPC_DATA_HEART;




#endif

