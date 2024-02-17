/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-17 11:02:14
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

/****************************************************输入节点任务***************************************************/
//任务定义
#define NAME_APP_TASK_TUBE_ENTRY                                              "AppTubeEntry"
//任务优先级            
#define PRIO_APP_TASK_TUBE_ENTRY                                              20
//任务堆栈大小          
#define STK_SIZE_APP_TASK_TUBE_ENTRY                                          8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskTubeEntry[STK_SIZE_APP_TASK_TUBE_ENTRY];
//任务控制块
extern OS_TCB tcbAppTaskTubeEntry;
//任务函数
void AppTaskFuncTubeEntry(void *p_arg);

/****************************************************测试转盘任务***************************************************/
//任务定义
#define NAME_APP_TASK_TUBE_TUBE_ROTATE                                             "AppTubeRotate"
//任务优先级            
#define PRIO_APP_TASK_TUBE_TUBE_ROTATE                                             21
//任务堆栈大小          
#define STK_SIZE_APP_TASK_TUBE_TUBE_ROTATE                                         8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskTubeRotate[STK_SIZE_APP_TASK_TUBE_TUBE_ROTATE];
//任务控制块
extern OS_TCB tcbAppTaskTubeRotate;
//任务函数
void AppTaskFuncTubeRotate(void *p_arg);

/****************************************************扫码任务***************************************************/
//任务定义
#define NAME_APP_TASK_TUBE_BAR_SCAN                                           "AppTubeBarScan"
//任务优先级            
#define PRIO_APP_TASK_TUBE_BAR_SCAN                                           22
//任务堆栈大小          
#define STK_SIZE_APP_TASK_TUBE_BAR_SCAN                                       8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskTubeBarScan[STK_SIZE_APP_TASK_TUBE_BAR_SCAN];
//任务控制块
extern OS_TCB tcbAppTaskTubeBarScan;
//任务函数
void AppTaskFuncTubeBarScan(void *p_arg);

/****************************************************测试位夹手任务***************************************************/
//任务定义
#define NAME_APP_TASK_TUBE_CLAMP                                         "AppTubeClamp"
//任务优先级            
#define PRIO_APP_TASK_TUBE_CLAMP                                         23
//任务堆栈大小          
#define STK_SIZE_APP_TASK_TUBE_CLAMP                                     8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskTubeClamp[STK_SIZE_APP_TASK_TUBE_CLAMP];
//任务控制块
extern OS_TCB tcbAppTaskTubeClamp;
//任务函数
void AppTaskFuncTubeClamp(void *p_arg);

/*****************************************************输出任务*****************************************************/
//任务定义  
#define NAME_APP_TASK_TUBE_EXIT                                             "AppTubeExit"
//任务优先级            
#define PRIO_APP_TASK_TUBE_EXIT                                             24
//任务堆栈大小          
#define STK_SIZE_APP_TASK_TUBE_EXIT                                         8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskTubeExit[STK_SIZE_APP_TASK_TUBE_EXIT];
//任务控制块
extern OS_TCB tcbAppTaskTubeExit;
//任务函数
void AppTaskFuncTubeExit(void *p_arg);

/****************************************************参数管理与其他辅助任务***************************************************/
//任务定义
#define NAME_APP_TASK_PARAM_UTIL                                                "AppParamUtil"
//任务优先级            
#define PRIO_APP_TASK_PARAM_UTIL                                                25
//任务堆栈大小          
#define STK_SIZE_APP_TASK_PARAM_UTIL                                            8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskParamUtil[STK_SIZE_APP_TASK_PARAM_UTIL];
//任务控制块
extern OS_TCB tcbAppTaskParamUtil;
//任务函数
void AppTaskFuncParamUtil(void *p_arg);

#endif




