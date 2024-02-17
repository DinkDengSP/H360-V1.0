/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:15:02
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-12 14:41:13
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppTaskConfig.h
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

/*******************************************************CupTrayManage任务*****************************************************/
//任务定义
#define NAME_APP_TASK_CUP_TRAY_MANAGE                                                 "CupTrayManage"
//任务优先级                    
#define PRIO_APP_TASK_CUP_TRAY_MANAGE                                                 21
//任务堆栈大小                  
#define STK_SIZE_APP_TASK_CUP_TRAY_MANAGE                                             16 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskCupTrayManage[STK_SIZE_APP_TASK_CUP_TRAY_MANAGE];
//任务控制块
extern OS_TCB tcbAppTaskCupTrayManage;
//任务函数
void AppTaskFuncCupTrayManage(void *p_arg);



/*******************************************************NeedleBead任务*****************************************************/
//任务定义
#define NAME_APP_TASK_NEEDLE_BEAD                                                       "NeedleBead"
//任务优先级                            
#define PRIO_APP_TASK_NEEDLE_BEAD                                                       22
//任务堆栈大小                          
#define STK_SIZE_APP_TASK_NEEDLE_BEAD                                                   8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskNeedleBead[STK_SIZE_APP_TASK_NEEDLE_BEAD];
//任务控制块
extern OS_TCB tcbAppTaskNeedleBead;
//任务函数
void AppTaskFuncNeedleBead(void *p_arg);



/*******************************************************Param任务*****************************************************/
//任务定义
#define NAME_APP_TASK_PARAM                                                             "Param"
//任务优先级                                
#define PRIO_APP_TASK_PARAM                                                             23
//任务堆栈大小                              
#define STK_SIZE_APP_TASK_PARAM                                                         8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskParam[STK_SIZE_APP_TASK_PARAM];
//任务控制块
extern OS_TCB tcbAppTaskParam;
//任务函数
void AppTaskFuncParam(void *p_arg);



/*******************************************************PlateReagent任务*****************************************************/
//任务定义
#define NAME_APP_TASK_PLATE_REAGENT                                                     "PlateReagent"
//任务优先级                        
#define PRIO_APP_TASK_PLATE_REAGENT                                                     24
//任务堆栈大                        
#define STK_SIZE_APP_TASK_PLATE_REAGENT                                                 8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskPlateReagent[STK_SIZE_APP_TASK_PLATE_REAGENT];
//任务控制块
extern OS_TCB tcbAppTaskPlateReagent;
//任务函数
void AppTaskFuncPlateReagent(void *p_arg);







#endif


