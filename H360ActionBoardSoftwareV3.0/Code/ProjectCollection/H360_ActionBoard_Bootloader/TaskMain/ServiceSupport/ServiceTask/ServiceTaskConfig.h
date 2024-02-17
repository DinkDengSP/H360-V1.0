/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:17
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-11 20:16:33
 *FilePath: \H360_ActionBoard_Bootloader\TaskMain\ServiceSupport\ServiceTask\ServiceTaskConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_TASK_CONFIG_H_
#define __SERVICE_TASK_CONFIG_H_
#include "os.h"
#include "BoardInc.h"
#include "SystemCmd.h"
#include "ServiceImplInc.h"

/***********************************心跳任务************************************/
//任务定义
#define NAME_SERVICE_TASK_HEART                     "Heart ServiceTask"
//任务优先级
#define PRIO_SERVICE_TASK_HEART                     0
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_HEART                 8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskHeart[STK_SIZE_SERVICE_TASK_HEART];
//任务控制块
extern OS_TCB tcbServiceTaskHeart;
//任务函数
void ServiceTaskFuncHeart(void *p_arg);
/*************************************************************************************/

/***********************************CAN1协议解析任务************************************/
//CAN总线中断处理回调函数
//CAN总线接收回调函数
void OS_Can1ReceiveDataCallFunc(CanRxMsg* rxMsg);
//任务定义
#define NAME_SERVICE_TASK_CAN1_RECEIVE         "Can1Recv ServiceTask"
//任务优先级
#define PRIO_SERVICE_TASK_CAN1_RECEIVE          1
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_CAN1_RECEIVE      8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskCan1Receive[STK_SIZE_SERVICE_TASK_CAN1_RECEIVE];
//任务控制块
extern OS_TCB tcbServiceTaskCan1Receive;
//任务函数
void ServiceTaskFuncCan1Receive(void *p_arg);
/*************************************************************************************/

/***************************************IAP任务*****************************/
//任务定义
#define NAME_SERVICE_TASK_DEBUG_IAP                      "DebugIAP ServiceTask"
//任务优先级
#define PRIO_SERVICE_TASK_DEBUG_IAP                      2
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_DEBUG_IAP                  8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskDebugIAP[STK_SIZE_SERVICE_TASK_DEBUG_IAP];
//任务控制块
extern OS_TCB tcbServiceTaskDebugIAP;
//任务函数
void ServiceTaskFuncDebugIAP(void *p_arg);
/*************************************************************************************/





#endif





