/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:59:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-19 17:17:56 +0800
************************************************************************************************/ 
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "DriverSystemHeader.h"
#include "SrvTaskConfig.h"
#include "ModuleTaskConfig.h"

//服务层使用的内存空间
#define MEM_REGION_APP                                          MEM_SDRAM2
//服务层时间片
#define TIME_SLICE_APP_TASK                                     TX_NO_TIME_SLICE
//任务等待最小时间片
#define APP_TASK_MIN_TICK                                       5


/************************************************TCP消息处理任务*********************************************/
//TCP消息处理任务
#define NAME_APP_TASK_TCP_MSG                                                   "AppTask TcpMsg"
//任务优先级
#define PRIO_APP_TASK_TCP_MSG                                                   14
//任务堆栈大小
#define STK_SIZE_APP_TASK_TCP_MSG                                               8 * 256
//任务堆栈
extern uint64_t stackBufferAppTaskTcpMsg[STK_SIZE_APP_TASK_TCP_MSG/8];
//任务控制块
extern TX_THREAD tcbAppTaskTcpMsg;
//任务函数
void TaskFuncAppTcpMsg(unsigned long threadInput);
//任务专用的消息队列
extern TX_QUEUE queueAppTaskTcpMsg;
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_TCP_MSG                                           128
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskTcpMsg[LENGTH_QUEUE_APP_TASK_TCP_MSG];
/**********************************************************************************************************/

/************************************************IAP消息处理任务*********************************************/
extern uint8_t bootLoaderNeedResp;
//TCP消息处理任务
#define NAME_APP_TASK_IAP                                                       "AppTask Iap"
//任务优先级
#define PRIO_APP_TASK_IAP                                                       15
//任务堆栈大小
#define STK_SIZE_APP_TASK_IAP                                                   8 * 256
//任务堆栈
extern uint64_t stackBufferAppTaskIap[STK_SIZE_APP_TASK_IAP/8];
//任务控制块
extern TX_THREAD tcbAppTaskIap;
//任务函数
void TaskFuncAppIap(unsigned long threadInput);
//任务专用的消息队列
extern TX_QUEUE queueAppTaskIap;
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_IAP                                               32
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskIap[LENGTH_QUEUE_APP_TASK_IAP];
/**********************************************************************************************************/

/************************************************辅助消息处理任务*********************************************/
//TCP消息处理任务
#define NAME_APP_TASK_UTIL                                                      "AppTask Util"
//任务优先级
#define PRIO_APP_TASK_UTIL                                                      16
//任务堆栈大小
#define STK_SIZE_APP_TASK_UTIL                                                  8 * 256
//任务堆栈
extern uint64_t stackBufferAppTaskUtil[STK_SIZE_APP_TASK_UTIL/8];
//任务控制块
extern TX_THREAD tcbAppTaskUtil;
//任务函数
void TaskFuncAppUtil(unsigned long threadInput);
//任务专用的消息队列
extern TX_QUEUE queueAppTaskUtil;
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_UTIL                                              32
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskUtil[LENGTH_QUEUE_APP_TASK_UTIL];
/**********************************************************************************************************/


#endif



