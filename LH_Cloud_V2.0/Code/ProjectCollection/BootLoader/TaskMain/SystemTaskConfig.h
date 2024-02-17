/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-10-26 10:48:00
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-26 13:16:13
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_TASK_CONFIG_H_
#define __SYSTEM_TASK_CONFIG_H_
#include "ServiceTaskConfig.h"
#include "AppTaskConfig.h"
#include "SystemUtilInc.h"

/*********************************启动任务**************************************/
//启动任务
#define NAME_TASK_START                     "SystemStart"
//任务优先级
#define PRIO_TASK_START                     0
//任务堆栈大小
#define STK_SIZE_TASK_START                 8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START];
//任务控制块
extern OS_TCB tcbTaskStart;
//任务函数
void TaskFuncStart(void *p_arg);
/****************************************************************************/


#endif
