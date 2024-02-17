#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "os.h"
#include "AppImplInc.h"
#include "AppIpcBase.h"

//任务定义
#define NAME_APP_TASK_OTHER                              "Other"
//任务优先级
#define PRIO_APP_TASK_OTHER                              22
//任务堆栈大小
#define STK_SIZE_APP_TASK_OTHER                          8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskOther[STK_SIZE_APP_TASK_OTHER];
//任务控制块
extern OS_TCB tcbAppTaskOther;
//任务函数
void AppTaskFuncOther(void *p_arg);


#endif




