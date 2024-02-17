/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:16:40
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-02 20:18:35
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_BASE_H_
#define __SERVICE_IPC_BASE_H_
#include "ServiceImplInc.h"

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


//辅助任务
typedef struct IPC_DATA_ASSIST
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //返回数据
    int32_t liquidDetectPos;
}IPC_DATA_ASSIST;

//缓冲液注液
typedef struct IPC_DATA_BUFFER_SOLUTION_INJECT
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_BUFFER_SOLUTION_INJECT;

//杯栈管理
typedef struct IPC_DATA_CUP_TRAY_STACK_MANAGE
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_CUP_TRAY_STACK_MANAGE;

//新杯机械手
typedef struct IPC_DATA_HAND_NEW_CUP
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_HAND_NEW_CUP;

//清洗机械手
typedef struct IPC_DATA_HAND_WASH
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_HAND_WASH;

//液路自动
typedef struct IPC_DATA_LIQUID_AUTO
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_LIQUID_AUTO;

//测量模块
typedef struct IPC_DATA_MEASURE_MODULE
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_MEASURE_MODULE;

//磁珠混匀
typedef struct IPC_DATA_MIX_BEAD
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_MIX_BEAD;

//试剂混匀
typedef struct IPC_DATA_MIX_REAGENT
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_MIX_REAGENT;

//清洗混匀
typedef struct IPC_DATA_MIX_WASH
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_MIX_WASH;

//磁珠针
typedef struct IPC_DATA_NEEDLE_BEAD
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_NEEDLE_BEAD;

//试剂针
typedef struct IPC_DATA_NEEDLE_REAGENT
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_NEEDLE_REAGENT;

//样本针
typedef struct IPC_DATA_NEEDLE_SAMPLE
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_NEEDLE_SAMPLE;

//清洗针
typedef struct IPC_DATA_NEEDLE_WASH
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_NEEDLE_WASH;

//反应盘
typedef struct IPC_DATA_PLATE_REACTION
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_PLATE_REACTION;

//试剂盘
typedef struct IPC_DATA_PLATE_REAGENT
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_PLATE_REAGENT;

//清洗盘
typedef struct IPC_DATA_PLATE_WASH
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //数据元素
}IPC_DATA_PLATE_WASH;

//试管架传送带
typedef struct IPC_DATA_SAMPLE_RACK_TRACK
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_SAMPLE_RACK_TRACK;

//温度控制
typedef struct IPC_DATA_TEMPERATURE_CONTROL
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_TEMPERATURE_CONTROL;

//文件系统日志
typedef struct IPC_DATA_FATFS_LOG
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_FATFS_LOG;

//同步任务
typedef struct IPC_DATA_TASK_SYN
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //返回的CAN1错误结果
    CAN1_REPORT_ERR_CODE can1ReportCode;
    //返回的CAN1错误级别
    CAN1_REPORT_ERR_LEVEL can1ReportLevel;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}IPC_DATA_TASK_SYN;

//服务层报警接口
int32_t IPC_Service_ActionWarningSendToCommon(CAN1_REPORT_ERR_LEVEL errLevel, CAN1_REPORT_ERR_CODE errCode, LH_ERR errParam);

extern int32_t CanSendDataToCell(uint8_t u8CellNmbr, uint8_t u8Dest, uint16_t u16Cmd, const uint8_t *pu8Body, uint16_t u16Size);
#endif





