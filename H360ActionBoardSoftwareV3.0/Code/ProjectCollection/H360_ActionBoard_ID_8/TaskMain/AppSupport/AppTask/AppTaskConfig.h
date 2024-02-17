/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-13 14:50:26
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppTask\AppTaskConfig.h
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "os.h"
#include "AppImplInc.h"
#include "AppIpcBase.h"

/****************************************************轨道进样1任务****************************************************/
//任务定义
#define NAME_APP_TASK_PUSH_IN1                                      "PushIn1"
//任务优先级        
#define PRIO_APP_TASK_PUSH_IN1                                      20
//任务堆栈大小      
#define STK_SIZE_APP_TASK_PUSH_IN1                                  8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskPushIn1[STK_SIZE_APP_TASK_PUSH_IN1];
//任务控制块
extern OS_TCB tcbAppTaskPushIn1;
//任务函数
void AppTaskFuncPushIn1(void *p_arg);

/****************************************************轨道进样2任务****************************************************/
//任务定义
#define NAME_APP_TASK_PUSH_IN2                                      "PushIn2"
//任务优先级        
#define PRIO_APP_TASK_PUSH_IN2                                      21
//任务堆栈大小      
#define STK_SIZE_APP_TASK_PUSH_IN2                                  8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskPushIn2[STK_SIZE_APP_TASK_PUSH_IN2];
//任务控制块
extern OS_TCB tcbAppTaskPushIn2;
//任务函数
void AppTaskFuncPushIn2(void *p_arg);

/****************************************************轨道出样1任务****************************************************/
//任务定义
#define NAME_APP_TASK_PUSH_OUT1                                     "PushOut1"
//任务优先级        
#define PRIO_APP_TASK_PUSH_OUT1                                     22
//任务堆栈大小      
#define STK_SIZE_APP_TASK_PUSH_OUT1                                 8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskPushOut1[STK_SIZE_APP_TASK_PUSH_OUT1];
//任务控制块
extern OS_TCB tcbAppTaskPushOut1;
//任务函数
void AppTaskFuncPushOut1(void *p_arg);

/****************************************************轨道出样2任务****************************************************/
//任务定义
#define NAME_APP_TASK_PUSH_OUT2                                     "PushOut2"
//任务优先级        
#define PRIO_APP_TASK_PUSH_OUT2                                     23
//任务堆栈大小      
#define STK_SIZE_APP_TASK_PUSH_OUT2                                 8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskPushOut2[STK_SIZE_APP_TASK_PUSH_OUT2];
//任务控制块
extern OS_TCB tcbAppTaskPushOut2;
//任务函数
void AppTaskFuncPushOut2(void *p_arg);


/****************************************************轨道传输任务****************************************************/
//任务定义
#define NAME_APP_TASK_RAIL_TRANS                                    "RailTrans"
//任务优先级        
#define PRIO_APP_TASK_RAIL_TRANS                                    24
//任务堆栈大小      
#define STK_SIZE_APP_TASK_RAIL_TRANS                                8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRailTrans[STK_SIZE_APP_TASK_RAIL_TRANS];
//任务控制块
extern OS_TCB tcbAppTaskRailTrans;
//任务函数
void AppTaskFuncRailTrans(void *p_arg);

/****************************************************机械手运转任务****************************************************/
//任务定义
#define NAME_APP_TASK_RAIL_HAND                                     "RailHand"
//任务优先级        
#define PRIO_APP_TASK_RAIL_HAND                                     25
//任务堆栈大小      
#define STK_SIZE_APP_TASK_RAIL_HAND                                 8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRailHand[STK_SIZE_APP_TASK_RAIL_HAND];
//任务控制块
extern OS_TCB tcbAppTaskRailHand;
//任务函数
void AppTaskFuncRailHand(void *p_arg);

/****************************************************机械手+轨道集成任务************************************************/
//任务定义
#define NAME_APP_TASK_RAIL_COMPLEX                                  "RailComplex"
//任务优先级    
#define PRIO_APP_TASK_RAIL_COMPLEX                                  26
//任务堆栈大小  
#define STK_SIZE_APP_TASK_RAIL_COMPLEX                              8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRailComplex[STK_SIZE_APP_TASK_RAIL_COMPLEX];
//任务控制块
extern OS_TCB tcbAppTaskRailComplex;
//任务函数
void AppTaskFuncRailComplex(void *p_arg);

/****************************************************条码扫描任务****************************************************/
//任务定义
#define NAME_APP_TASK_BAR_SCAN                                      "BarScan"
//任务优先级        
#define PRIO_APP_TASK_BAR_SCAN                                      27
//任务堆栈大小      
#define STK_SIZE_APP_TASK_BAR_SCAN                                  8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskBarScan[STK_SIZE_APP_TASK_BAR_SCAN];
//任务控制块
extern OS_TCB tcbAppTaskBarScan;
//任务函数
void AppTaskFuncBarScan(void *p_arg);

/****************************************************参数管理任务***************************************************/
//任务定义
#define NAME_APP_TASK_PARAM                                         "AppParam"
//任务优先级            
#define PRIO_APP_TASK_PARAM                                         28
//任务堆栈大小          
#define STK_SIZE_APP_TASK_PARAM                                     8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskParam[STK_SIZE_APP_TASK_PARAM];
//任务控制块
extern OS_TCB tcbAppTaskParam;
//任务函数
void AppTaskFuncParam(void *p_arg);

#endif




