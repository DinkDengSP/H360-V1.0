/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-16 19:59:13
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-06 11:08:48
 *FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceTaskConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/

#ifndef __SERVICE_TASK_CONFIG_H_
#define __SERVICE_TASK_CONFIG_H_
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

/*********************************************心跳**************************************************/
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

/*********************************************传感器信息更新**************************************************/
//启动任务
#define NAME_SERVICE_TASK_SENSOR                           "ServiceTaskSensor"
//任务优先级
#define PRIO_SERVICE_TASK_SENSOR                           2
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_SENSOR                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskSensor[STK_SIZE_SERVICE_TASK_SENSOR];
//任务控制块
extern OS_TCB tcbServiceTaskSensor;
//任务函数
void ServiceTaskFuncSensor(void *p_arg);
/****************************************************************************************************************/

/*********************************************环境温度相关控制**************************************************/
//启动任务
#define NAME_SERVICE_TASK_ENVIR                           "ServiceTaskEnvir"
//任务优先级
#define PRIO_SERVICE_TASK_ENVIR                           3
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_ENVIR                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskEnvir[STK_SIZE_SERVICE_TASK_ENVIR];
//任务控制块
extern OS_TCB tcbServiceTaskEnvir;
//任务函数
void ServiceTaskFuncEnvir(void *p_arg);
/****************************************************************************************************************/

/*********************************************参数读写**************************************************/
//启动任务
#define NAME_SERVICE_TASK_PARAM                           "ServiceTaskParam"
//任务优先级
#define PRIO_SERVICE_TASK_PARAM                           4
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_PARAM                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskParam[STK_SIZE_SERVICE_TASK_PARAM];
//任务控制块
extern OS_TCB tcbServiceTaskParam;
//任务函数
void ServiceTaskFuncParam(void *p_arg);
/****************************************************************************************************************/

/*********************************************反应盘**************************************************/
//启动任务
#define NAME_SERVICE_TASK_PLATE_REACTION                           "ServiceTaskPlateReaction"
//任务优先级
#define PRIO_SERVICE_TASK_PLATE_REACTION                           5
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_PLATE_REACTION                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskPlateReaction[STK_SIZE_SERVICE_TASK_PLATE_REACTION];
//任务控制块
extern OS_TCB tcbServiceTaskPlateReaction;
//任务函数
void ServiceTaskFuncPlateReaction(void *p_arg);
/****************************************************************************************************************/

/*********************************************试剂盘**************************************************/
//启动任务
#define NAME_SERVICE_TASK_PLATE_REAGENT                           "ServiceTaskPlateReagent"
//任务优先级
#define PRIO_SERVICE_TASK_PLATE_REAGENT                           6
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_PLATE_REAGENT                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskPlateReagent[STK_SIZE_SERVICE_TASK_PLATE_REAGENT];
//任务控制块
extern OS_TCB tcbServiceTaskPlateReagent;
//任务函数
void ServiceTaskFuncPlateReagent(void *p_arg);
/****************************************************************************************************************/

/*********************************************清洗盘**************************************************/
//启动任务
#define NAME_SERVICE_TASK_PLATE_WASH                           "ServiceTaskPlateWash"
//任务优先级
#define PRIO_SERVICE_TASK_PLATE_WASH                           7
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_PLATE_WASH                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskPlateWash[STK_SIZE_SERVICE_TASK_PLATE_WASH];
//任务控制块
extern OS_TCB tcbServiceTaskPlateWash;
//任务函数
void ServiceTaskFuncPlateWash(void *p_arg);
/****************************************************************************************************************/

/*******************************************************DebugIAP任务*****************************************************/
//任务定义
#define NAME_SERVICE_TASK_DEBUG_IAP                              "DebugIAP"
//任务优先级
#define PRIO_SERVICE_TASK_DEBUG_IAP                              8
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_DEBUG_IAP                          8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskDebugIAP[STK_SIZE_SERVICE_TASK_DEBUG_IAP];
//任务控制块
extern OS_TCB tcbServiceTaskDebugIAP;
//任务函数
void ServiceTaskFuncDebugIAP(void *p_arg);



#endif



