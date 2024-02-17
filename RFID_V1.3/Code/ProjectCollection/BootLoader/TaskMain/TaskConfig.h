/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-31 13:28:40
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-04 15:30:46
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __TASK_CONFIG_H_
#define __TASK_CONFIG_H_
#include "AppTaskConfig.h"

/*********************************启动任务**************************************/
//启动任务
#define NAME_TASK_START                     "start"
//任务优先级
#define PRIO_TASK_START                     0
//任务堆栈大小
#define STK_SIZE_TASK_START                 4 * 32
//任务堆栈
extern CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START];
//任务控制块
extern OS_TCB tcbTaskStart;
//任务函数
void TaskFuncStart(void *p_arg);
/****************************************************************************/

//操作系统任务配置
typedef struct OS_TASK_CREATE_CONFIG
{
    OS_TCB* tcbTaskPtr;//任务TCB控制块的指针
    CPU_CHAR* taskNamePtr;//任务的名称
    OS_TASK_PTR taskFuncPtr;//任务函数地址
    OS_PRIO taskPrio;//任务优先级
    CPU_STK* taskStkBasePtr;//任务堆栈基地址
    CPU_STK_SIZE taskStkSizeLimit;//任务堆栈报警尺寸
    CPU_STK_SIZE taskStkSize;//任务堆栈尺寸
    OS_MSG_QTY msgQueenSize;//任务自带消息队列深度
}OS_TASK_CREATE_CONFIG;



#endif