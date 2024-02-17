/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-16 14:18:00
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceIPC\CommonServiceIpcBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __COMMON_SERVICE_IPC_BASE_H_
#define __COMMON_SERVICE_IPC_BASE_H_
#include "CommonServiceImplBase.h"
#include "StepMotor.h"
#include "CanSubSystemInc.h"
//这是一批供上机任务调用的API实现,通过此处API转发实现上层应用任务与下层
//动作任务之间的分离

//系统服务调用
//系统内部指令处理的阶段定义
typedef enum STATE_IPC
{
    STATE_IPC_IDLE          = 0,//空闲
    STATE_IPC_WAIT_ACK      = 1,//等待ACK
    STATE_IPC_WAIT_RETURN   = 2,//等待结果
    STATE_IPC_CMD_COMPLETE  = 3,//执行完成
}STATE_IPC;

//系统动作调用API,分为三种 
//1. 发送动作并等待ACK(ACK无错误代表动作开始执行)  
//2. 发送动作并等待结果(无错误代表动作执行完成)  
//3. 读取上一次动作的结果
//如果调用了发送动作并等待ACK,那么同一个模块在下次发送动作之前必须读取上一次的动作结果,否则后一次调用动作报错.
//辅助任务
//通用IPC组件
typedef struct DATA_IPC_COMMON
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_COMMON;

//步进电机
typedef struct DATA_IPC_STEPMOTOR
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //如果有新的交互数据需要加入,那么在此处添加新的数据
    int32_t currentCoordinate;
}DATA_IPC_STEPMOTOR;

//输出
typedef struct DATA_IPC_OUTPUT
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
}DATA_IPC_OUTPUT;

//输入
typedef struct DATA_IPC_INPUT
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //如果有新的交互数据需要加入,那么在此处添加新的数据
    BitAction pinValue;
}DATA_IPC_INPUT;

//步进电机指令表
typedef enum IPC_CMD_STEP_MOTOR
{
    IPC_CMD_STEP_MOTOR_RESET            = 0X0000,//复位
    IPC_CMD_STEP_MOTOR_RUN_STEPS        = 0X0001,//运行指定步数
    IPC_CMD_STEP_MOTOR_RUN_2_COORDINATE = 0X0002,//运行到指定坐标
    IPC_CMD_STEP_MOTOR_RETURN_ZERO      = 0X0003,//运行回零
    IPC_CMD_STEP_MOTOR_RUN_ALWAYS       = 0X0004,//持续运转
    IPC_CMD_STEP_MOTOR_STOP_IMMEDIATELY = 0X0005,//立即停止
    IPC_CMD_STEP_MOTOR_STOP_DECELERATION= 0X0006,//紧急停止
    IPC_CMD_STEP_MOTOR_REFLUSH_STATE    = 0X0007,//更新状态
    IPC_CMD_STEP_MOTOR_RETURN_ZERO_WITHOUT_RESET = 0X0008,//回零但是不复位
    IPC_CMD_STEP_MOTOR_SET_STATE        = 0X000A,//设置电机状态
}IPC_CMD_STEP_MOTOR;



#endif





