/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-14 17:09:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 14:04:30 +0800
************************************************************************************************/ 
#ifndef __SYSTEM_TASK_CONFIG_H_
#define __SYSTEM_TASK_CONFIG_H_
#include "DriverSystemHeader.h"
#include "SrvTaskConfig.h"
#include "ModuleTaskConfig.h"
#include "AppTaskConfig.h"

/*********************************启动任务**************************************/
//启动任务
#define NAME_SYSTEM_TASK_START                     "SystemTaskStart"
//任务优先级
#define PRIO_SYSTEM_TASK_START                     1
//任务堆栈大小
#define STK_SIZE_SYSTEM_TASK_START                 16 * 128
//任务时间片
#define TIME_SLICE_TASK_START                      TX_NO_TIME_SLICE
//任务堆栈
extern uint64_t stackBufferSystemTaskStart[STK_SIZE_SYSTEM_TASK_START/8];
//任务控制块
extern TX_THREAD tcbSystemTaskStart;
//任务函数
void FuncTaskStartEntry(ULONG threadInput);
/****************************************************************************/




#endif


