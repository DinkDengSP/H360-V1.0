/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:38:11
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-12 16:30:52
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_3\TaskMain\AppSupport\AppTask\AppTaskConfig.h
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

/*******************************************************HandWash任务*****************************************************/
//任务定义
#define NAME_APP_TASK_HAND_WASH                                                 "HandWash"
//任务优先级                    
#define PRIO_APP_TASK_HAND_WASH                                                 21
//任务堆栈大小                  
#define STK_SIZE_APP_TASK_HAND_WASH                                             8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskHandWash[STK_SIZE_APP_TASK_HAND_WASH];
//任务控制块
extern OS_TCB tcbAppTaskHandWash;
//任务函数
void AppTaskFuncHandWash(void *p_arg);

/*******************************************************MeasureUnit任务*****************************************************/
//任务定义
#define NAME_APP_TASK_MEASURE_UNIT                                              "MeasureUnit"
//任务优先级                    
#define PRIO_APP_TASK_MEASURE_UNIT                                              22
//任务堆栈大小                  
#define STK_SIZE_APP_TASK_MEASURE_UNIT                                          8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskMeasureUnit[STK_SIZE_APP_TASK_MEASURE_UNIT];
//任务控制块
extern OS_TCB tcbAppTaskMeasureUnit;
//任务函数
void AppTaskFuncMeasureUnit(void *p_arg);

/*******************************************************Param任务*****************************************************/
//任务定义
#define NAME_APP_TASK_PARAM                                                     "Param"
//任务优先级                        
#define PRIO_APP_TASK_PARAM                                                     23
//任务堆栈大小                      
#define STK_SIZE_APP_TASK_PARAM                                                 8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskParam[STK_SIZE_APP_TASK_PARAM];
//任务控制块
extern OS_TCB tcbAppTaskParam;
//任务函数
void AppTaskFuncParam(void *p_arg);























#endif


