/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:49:43
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-27 15:36:27
**FilePath: \App\TaskMain\AppSupport\AppTask\AppTaskConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "AppImplInc.h"
#include "AppIpcInc.h"

/********************************************AppStateUpLoad*****************************************************/
//任务定义
#define NAME_TASK_APP_STATE_UPLOAD                             "AppStateUpLoad"
//任务优先级            
#define PRIO_TASK_APP_STATE_UPLOAD                              3
//任务堆栈大小          
#define STK_SIZE_TASK_APP_STATE_UPLOAD                          8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskAppStateUpLoad[STK_SIZE_TASK_APP_STATE_UPLOAD];
//任务控制块
extern OS_TCB tcbTaskAppStateUpLoad;
//任务函数
void TaskFuncAppStateUpLoad(void *p_arg);

/********************************************AppSampleRackIn*****************************************************/
//任务定义
#define NAME_TASK_APP_SAMPLE_RACK_IN                              "AppSampleRackIn"
//任务优先级            
#define PRIO_TASK_APP_SAMPLE_RACK_IN                              15
//任务堆栈大小          
#define STK_SIZE_TASK_APP_SAMPLE_RACK_IN                          8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskAppSampleRackIn[STK_SIZE_TASK_APP_SAMPLE_RACK_IN];
//任务控制块
extern OS_TCB tcbTaskAppSampleRackIn;
//任务函数
void TaskFuncAppSampleRackIn(void *p_arg);

/********************************************AppSampleRackMove*****************************************************/
//任务定义
#define NAME_TASK_APP_SAMPLE_RACK_MOVE                              "AppSampleRackMove"
//任务优先级            
#define PRIO_TASK_APP_SAMPLE_RACK_MOVE                              16
//任务堆栈大小          
#define STK_SIZE_TASK_APP_SAMPLE_RACK_MOVE                          8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskAppSampleRackMove[STK_SIZE_TASK_APP_SAMPLE_RACK_MOVE];
//任务控制块
extern OS_TCB tcbTaskAppSampleRackMove;
//任务函数
void TaskFuncAppSampleRackMove(void *p_arg);

/********************************************AppSampleRackOut*****************************************************/
//任务定义
#define NAME_TASK_APP_SAMPLE_RACK_OUT                              "AppSampleRackOut"
//任务优先级            
#define PRIO_TASK_APP_SAMPLE_RACK_OUT                              17
//任务堆栈大小          
#define STK_SIZE_TASK_APP_SAMPLE_RACK_OUT                          8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskAppSampleRackOut[STK_SIZE_TASK_APP_SAMPLE_RACK_OUT];
//任务控制块
extern OS_TCB tcbTaskAppSampleRackOut;
//任务函数
void TaskFuncAppSampleRackOut(void *p_arg);

/********************************************AppStateParam*****************************************************/
//任务定义
#define NAME_TASK_APP_STATE_PARAM                              "AppStateParam"
//任务优先级            
#define PRIO_TASK_APP_STATE_PARAM                              18
//任务堆栈大小          
#define STK_SIZE_TASK_APP_STATE_PARAM                          8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskAppStateParam[STK_SIZE_TASK_APP_STATE_PARAM];
//任务控制块
extern OS_TCB tcbTaskAppStateParam;
//任务函数
void TaskFuncAppStateParam(void *p_arg);






#endif

