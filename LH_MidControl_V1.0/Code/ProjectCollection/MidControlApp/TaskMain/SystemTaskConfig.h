/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:07:12
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-04 13:30:13
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_TASK_CONFIG_H_
#define __SYSTEM_TASK_CONFIG_H_
#include "SrvTaskConfig.h"
#include "AppTaskConfig.h"

/*********************************启动任务**************************************/
//启动任务
#define NAME_SYSTEM_TASK_START                     "SystemTask Start"
//任务优先级
#define PRIO_SYSTEM_TASK_START                     2
//任务堆栈大小
#define STK_SIZE_SYSTEM_TASK_START                 8 * 128
//任务堆栈
extern uint64_t stackBufferSystemTaskStart[STK_SIZE_SYSTEM_TASK_START/8];
//任务控制块
extern TX_THREAD tcbSystemTaskStart;
//任务函数
void TaskFuncSystemStart(unsigned long threadInput);
/****************************************************************************/


//任务函数定义
typedef void (*SystemTaskFuncPtr)(unsigned long threadInput);
//操作系统任务配置
typedef struct OS_TASK_CREATE_CONFIG
{
    TX_THREAD* tcbTaskPtr;//任务TCB控制块的指针
    CHAR* taskNamePtr;//任务的名称
    SystemTaskFuncPtr taskFuncPtr;//任务函数地址
    unsigned long threadInput;//任务输入参数
    void* taskStkBasePtr;//任务堆栈指针
    unsigned long taskStkSize;//任务堆栈长度
    uint32_t priority;//任务优先级
    uint32_t preempt_threshold;//抢占阈值优先级,当抢占阈值为任务优先级的时候禁止任何任务抢占
    unsigned long  time_slice;//任务时间片长度,只有开启了任务时间片才能使用,否则定义为TX_NO_TIME_SLICE
    uint32_t auto_start;//任务创建后是否自动启动,启动使用TX_AUTO_START,不启动使用TX_DONT_START
}OS_TASK_CREATE_CONFIG;

#endif
