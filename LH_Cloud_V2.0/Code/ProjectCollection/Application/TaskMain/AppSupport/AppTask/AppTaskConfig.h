/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-10-26 13:16:33
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-26 13:17:03
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "SystemUtilInc.h"
#include "BoardInc.h"

/*******************************************************心跳任务********************************************************/
//任务定义
#define NAME_APP_TASK_DEMO                                  "Demo"
//任务优先级
#define PRIO_APP_TASK_DEMO                                  15
//任务堆栈大小
#define STK_SIZE_APP_TASK_DEMO                              8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskDemo[STK_SIZE_APP_TASK_DEMO];
//任务控制块
extern OS_TCB tcbAppTaskDemo;
//任务函数
void AppTaskFuncDemo(void *p_arg);


#endif

