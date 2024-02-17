/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reser
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:08:41
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-26 19:45:35
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "SrvTaskConfig.h"
#include "AppIpcInc.h"
#include "AppParam.h"
#include "AppState.h"

/************************************************免疫CAN消息处理任务*********************************************/
//免疫CAN消息处理任务
#define NAME_APP_TASK_CAN_MSG_IMM                                               "AppTask CanMsgImm"
//任务优先级
#define PRIO_APP_TASK_CAN_MSG_IMM                                               10
//任务堆栈大小
#define STK_SIZE_APP_TASK_CAN_MSG_IMM                                           8 * 256
//任务堆栈
extern uint64_t stackBufferAppTaskCanMsgImm[STK_SIZE_APP_TASK_CAN_MSG_IMM/8];
//任务控制块
extern TX_THREAD tcbAppTaskCanMsgImm;
//任务函数
void TaskFuncAppCanMsgImm(unsigned long threadInput);
//任务专用的消息队列
extern TX_QUEUE queueAppTaskCanMsgImm;
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_CAN_MSG_IMM                                       1024
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskCanMsgImm[LENGTH_QUEUE_APP_TASK_CAN_MSG_IMM];
/**********************************************************************************************************/

/************************************************生化CAN消息处理任务*********************************************/
//生化CAN消息处理任务
#define NAME_APP_TASK_CAN_MSG_BIO                                               "AppTask CanMsgBio"
//任务优先级
#define PRIO_APP_TASK_CAN_MSG_BIO                                               11
//任务堆栈大小
#define STK_SIZE_APP_TASK_CAN_MSG_BIO                                           8 * 256
//任务堆栈
extern uint64_t stackBufferAppTaskCanMsgBio[STK_SIZE_APP_TASK_CAN_MSG_BIO/8];
//任务控制块
extern TX_THREAD tcbAppTaskCanMsgBio;
//任务函数
void TaskFuncAppCanMsgBio(unsigned long threadInput);
//任务专用的消息队列
extern TX_QUEUE queueAppTaskCanMsgBio;
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_CAN_MSG_BIO                                       1024
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskCanMsgBio[LENGTH_QUEUE_APP_TASK_CAN_MSG_BIO];
/**********************************************************************************************************/

/************************************************轨道CAN消息处理任务*********************************************/
//轨道CAN消息处理任务
#define NAME_APP_TASK_CAN_MSG_RAIL                                              "AppTask CanMsgRail"
//任务优先级
#define PRIO_APP_TASK_CAN_MSG_RAIL                                              12
//任务堆栈大小
#define STK_SIZE_APP_TASK_CAN_MSG_RAIL                                          8 * 256
//任务堆栈
extern uint64_t stackBufferAppTaskCanMsgRail[STK_SIZE_APP_TASK_CAN_MSG_RAIL/8];
//任务控制块
extern TX_THREAD tcbAppTaskCanMsgRail;
//任务函数
void TaskFuncAppCanMsgRail(unsigned long threadInput);
//任务专用的消息队列
extern TX_QUEUE queueAppTaskCanMsgRail;
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_CAN_MSG_RAIL                                      1024
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskCanMsgRail[LENGTH_QUEUE_APP_TASK_CAN_MSG_RAIL];
/**********************************************************************************************************/

/************************************************立禾云CAN消息处理任务*********************************************/
//立禾云CAN消息处理任务
#define NAME_APP_TASK_CAN_MSG_CLOUD                                             "AppTask CanMsgCloud"
//任务优先级
#define PRIO_APP_TASK_CAN_MSG_CLOUD                                             13
//任务堆栈大小
#define STK_SIZE_APP_TASK_CAN_MSG_CLOUD                                         8 * 256
//任务堆栈
extern uint64_t stackBufferAppTaskCanMsgCloud[STK_SIZE_APP_TASK_CAN_MSG_CLOUD/8];
//任务控制块
extern TX_THREAD tcbAppTaskCanMsgCloud;
//任务函数
void TaskFuncAppCanMsgCloud(unsigned long threadInput);
//任务专用的消息队列
extern TX_QUEUE queueAppTaskCanMsgCloud;
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_CAN_MSG_CLOUD                                     1024
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskCanMsgCloud[LENGTH_QUEUE_APP_TASK_CAN_MSG_CLOUD];
/**********************************************************************************************************/

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

/************************************************系统总流程处理任务*********************************************/
//系统总流程处理任务
#define NAME_APP_TASK_TEST_FLOW                                                 "AppTask TestFlow"
//任务优先级
#define PRIO_APP_TASK_TEST_FLOW                                                 15
//任务堆栈大小
#define STK_SIZE_APP_TASK_TEST_FLOW                                             8 * 1024
//任务堆栈
extern uint64_t stackBufferAppTaskTestFlow[STK_SIZE_APP_TASK_TEST_FLOW/8];
//任务控制块
extern TX_THREAD tcbAppTaskTestFlow;
//任务函数
void TaskFuncAppTestFlow(unsigned long threadInput);
//任务专用的消息队列
extern TX_QUEUE queueAppTaskTestFlow;
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_TEST_FLOW                                         128
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskTestFlow[LENGTH_QUEUE_APP_TASK_TEST_FLOW];
/**********************************************************************************************************/
//获取主流程状态指针
extern APP_STATE* AppStateGetTestStatusPtr(void);
//TCP命令处理
extern void AppTaskAppCommandSendByTcpCommand(uint16_t cmd, uint8_t* datBuf, uint16_t bodyLen);
//CAN-轨道命令处理
extern void AppTaskAppCommandSendByCanRailCommand(uint8_t u8ModuleNo, uint8_t u8BoardID, uint16_t cmd,  uint8_t* datBuf, uint16_t bodyLen);
//CAN-主控命令处理
extern void AppTaskAppCommandSendByCanMainCtrlCommand(uint8_t u8ModuleNo, uint8_t u8BoardID, uint16_t cmd, uint8_t* datBuf, uint16_t bodyLen);


#endif



