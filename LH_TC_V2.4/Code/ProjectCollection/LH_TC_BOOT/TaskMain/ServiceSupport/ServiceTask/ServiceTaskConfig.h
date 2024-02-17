/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-16 16:30:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-12 11:55:34
 *FilePath: \LH_TC_BOOT_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceTaskConfig.h
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
#include "CommonFuncInc.h"
#include "CanSlaveMain.h"
#include "ServiceImplInc.h"


/*********************************************CAN总线接收分发处理任务**************************************************/
//CAN总线接收回调函数
void OS_CanReceiveDataCallFunc(CanRxMsg* rxMsg);
//启动任务
#define NAME_SERVICE_TASK_CAN_RECV                             "ServiceTaskCanRecv"
//任务优先级
#define PRIO_SERVICE_TASK_CAN_RECV                             0
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_CAN_RECV                         8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskCanRecv[STK_SIZE_SERVICE_TASK_CAN_RECV];
//任务控制块
extern OS_TCB tcbServiceTaskCanRecv;
//任务函数
void ServiceTaskFuncCanRecv(void *p_arg);
/****************************************************************************************************************/

/*********************************************心跳任务**************************************************/
//启动任务
#define NAME_SERVICE_TASK_HEART                           "ServiceTaskHeart"
//任务优先级
#define PRIO_SERVICE_TASK_HEART                           1
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_HEART                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskHeart[STK_SIZE_SERVICE_TASK_HEART];
//任务控制块
extern OS_TCB tcbServiceTaskHeart;
//任务函数
void ServiceTaskFuncHeart(void *p_arg);
/****************************************************************************************************************/


/*********************************************传感器检测任务**************************************************/
//启动任务
#define NAME_SERVICE_TASK_SENSOR                                  "ServiceTaskSensor"
//任务优先级
#define PRIO_SERVICE_TASK_SENSOR                                  2
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_SENSOR                              8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskSensor[STK_SIZE_SERVICE_TASK_SENSOR];
//任务控制块
extern OS_TCB tcbServiceTaskSensor;
//任务函数
void ServiceTaskFuncSensor(void *p_arg);
/****************************************************************************************************************/


/*********************************************IAP任务*************************************************/
//启动任务
#define NAME_SERVICE_TASK_IAP                                     "ServiceTaskSystemIap"
//任务优先级
#define PRIO_SERVICE_TASK_IAP                                     3
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_IAP                                 8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskIap[STK_SIZE_SERVICE_TASK_IAP];
//任务控制块
extern OS_TCB tcbServiceTaskIap;
//任务函数
void ServiceTaskFuncIap(void *p_arg);
/****************************************************************************************************************/


#endif



