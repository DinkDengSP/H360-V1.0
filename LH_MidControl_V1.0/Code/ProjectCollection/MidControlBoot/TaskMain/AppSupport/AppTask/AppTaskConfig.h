/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reser
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:08:41
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-23 12:01:36
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "BoardInc.h"
#include "SrvTaskConfig.h"

/*********************************TCP消息处理任务**************************************/
//应用TcpMsg任务
#define NAME_APP_TASK_TCP_MSG                     "AppTask TcpMsg"
//任务优先级
#define PRIO_APP_TASK_TCP_MSG                     20
//任务堆栈大小
#define STK_SIZE_APP_TASK_TCP_MSG                 8 * 512
//任务堆栈
extern uint64_t stackBufferAppTaskTcpMsg[STK_SIZE_APP_TASK_TCP_MSG/8];
//任务控制块
extern TX_THREAD tcbAppTaskTcpMsg;
//任务函数
void TaskFuncAppTcpMsg(unsigned long threadInput);
//任务专用的消息队列
extern TX_QUEUE queueAppTaskTcpMsg;
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_TCP_MSG         1024
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskTcpMsg[LENGTH_QUEUE_APP_TASK_TCP_MSG];
/****************************************************************************/


#endif



