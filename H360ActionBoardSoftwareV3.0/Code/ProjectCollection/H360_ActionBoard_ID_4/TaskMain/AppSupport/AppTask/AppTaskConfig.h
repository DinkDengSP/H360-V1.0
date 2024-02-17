/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:43:58
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-12 23:01:46
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_4\TaskMain\AppSupport\AppTask\AppTaskConfig.h
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


/*******************************************************NeedleReagent任务*****************************************************/
//任务定义
#define NAME_APP_TASK_NEEDLE_REAGENT                                                 "NeedleReagent"
//任务优先级                    
#define PRIO_APP_TASK_NEEDLE_REAGENT                                                 21
//任务堆栈大小                  
#define STK_SIZE_APP_TASK_NEEDLE_REAGENT                                             8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskNeedleReagent[STK_SIZE_APP_TASK_NEEDLE_REAGENT];
//任务控制块
extern OS_TCB tcbAppTaskNeedleReagent;
//任务函数
void AppTaskFuncNeedleReagent(void *p_arg);

/*******************************************************PlateReaction任务*****************************************************/
//任务定义
#define NAME_APP_TASK_PLATE_REACTION                                                 "PlateReaction"
//任务优先级                    
#define PRIO_APP_TASK_PLATE_REACTION                                                 22
//任务堆栈大小                  
#define STK_SIZE_APP_TASK_PLATE_REACTION                                             8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskPlateReaction[STK_SIZE_APP_TASK_PLATE_REACTION];
//任务控制块
extern OS_TCB tcbAppTaskPlateReaction;
//任务函数
void AppTaskFuncPlateReaction(void *p_arg);

/*******************************************************MixBead任务*****************************************************/
//任务定义
#define NAME_APP_TASK_MIX_BEAD                                                      "MixBead"
//任务优先级                            
#define PRIO_APP_TASK_MIX_BEAD                                                      23
//任务堆栈大小                          
#define STK_SIZE_APP_TASK_MIX_BEAD                                                  8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskMixBead[STK_SIZE_APP_TASK_MIX_BEAD];
//任务控制块
extern OS_TCB tcbAppTaskMixBead;
//任务函数
void AppTaskFuncMixBead(void *p_arg);

/*******************************************************MixReagent任务*****************************************************/
//任务定义
#define NAME_APP_TASK_MIX_REAGENT                                                   "MixReagent"
//任务优先级                        
#define PRIO_APP_TASK_MIX_REAGENT                                                   24
//任务堆栈大小                      
#define STK_SIZE_APP_TASK_MIX_REAGENT                                               8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskMixReagent[STK_SIZE_APP_TASK_MIX_REAGENT];
//任务控制块
extern OS_TCB tcbAppTaskMixReagent;
//任务函数
void AppTaskFuncMixReagent(void *p_arg);

/*******************************************************Param任务*****************************************************/
//任务定义
#define NAME_APP_TASK_PARAM                                                         "Param"
//任务优先级                            
#define PRIO_APP_TASK_PARAM                                                         25
//任务堆栈大小                          
#define STK_SIZE_APP_TASK_PARAM                                                     8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskParam[STK_SIZE_APP_TASK_PARAM];
//任务控制块
extern OS_TCB tcbAppTaskParam;
//任务函数
void AppTaskFuncParam(void *p_arg);





















#endif


