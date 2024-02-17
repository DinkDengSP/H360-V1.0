/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-17 09:32:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-06 11:08:32
 *FilePath: \LH_TC_APP_V2.3\TaskMain\AppSupport\AppTask\AppTaskConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "AppImplInc.h"


/*********************************************系统状态检测**************************************************/
//启动任务
#define NAME_APP_TASK_SYSTEM_MSG                           "AppTaskSystemMsg"
//任务优先级
#define PRIO_APP_TASK_SYSTEM_MSG                           10
//任务堆栈大小
#define STK_SIZE_APP_TASK_SYSTEM_MSG                       8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskSystemMsg[STK_SIZE_APP_TASK_SYSTEM_MSG];
//任务控制块
extern OS_TCB tcbAppTaskSystemMsg;
//任务函数
void AppTaskFuncSystemMsg(void *p_arg);
/****************************************************************************************************************/




#endif




