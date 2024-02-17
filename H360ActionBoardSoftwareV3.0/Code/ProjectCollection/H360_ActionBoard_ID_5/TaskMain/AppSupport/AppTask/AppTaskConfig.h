/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:30:52
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-13 11:53:32
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_5\TaskMain\AppSupport\AppTask\AppTaskConfig.h
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "AppImplInc.h"
#include "AppIpcInc.h"


/*******************************************************NeedleSample任务*****************************************************/
//任务定义
#define NAME_APP_TASK_NEEDLE_SAMPLE                                                     "NeedleSample"
//任务优先级                        
#define PRIO_APP_TASK_NEEDLE_SAMPLE                                                     22
//任务堆栈大小                      
#define STK_SIZE_APP_TASK_NEEDLE_SAMPLE                                                 8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskNeedleSample[STK_SIZE_APP_TASK_NEEDLE_SAMPLE];
//任务控制块
extern OS_TCB tcbAppTaskNeedleSample;
//任务函数
void AppTaskFuncNeedleSample(void *p_arg);

/*******************************************************HandNewCup任务*****************************************************/
//任务定义
#define NAME_APP_TASK_HAND_NEW_CUP                                                      "HandNewCup"
//任务优先级                            
#define PRIO_APP_TASK_HAND_NEW_CUP                                                      21
//任务堆栈大小                          
#define STK_SIZE_APP_TASK_HAND_NEW_CUP                                                  8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskHandNewCup[STK_SIZE_APP_TASK_HAND_NEW_CUP];
//任务控制块
extern OS_TCB tcbAppTaskHandNewCup;
//任务函数
void AppTaskFuncHandNewCup(void *p_arg);

/*******************************************************SampleRackTrack任务*****************************************************/
//任务定义
#define NAME_APP_TASK_SAMPLE_RACK_TRACK                                                 "SampleRackTrack"
//任务优先级                    
#define PRIO_APP_TASK_SAMPLE_RACK_TRACK                                                 23
//任务堆栈大小                  
#define STK_SIZE_APP_TASK_SAMPLE_RACK_TRACK                                             8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskSampleRackTrack[STK_SIZE_APP_TASK_SAMPLE_RACK_TRACK];
//任务控制块
extern OS_TCB tcbAppTaskSampleRackTrack;
//任务函数
void AppTaskFuncSampleRackTrack(void *p_arg);

/*******************************************************Param任务*****************************************************/
//任务定义
#define NAME_APP_TASK_PARAM                                                             "Param"
//任务优先级                                
#define PRIO_APP_TASK_PARAM                                                             24
//任务堆栈大小                              
#define STK_SIZE_APP_TASK_PARAM                                                         8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskParam[STK_SIZE_APP_TASK_PARAM];
//任务控制块
extern OS_TCB tcbAppTaskParam;
//任务函数
void AppTaskFuncParam(void *p_arg);























#endif



