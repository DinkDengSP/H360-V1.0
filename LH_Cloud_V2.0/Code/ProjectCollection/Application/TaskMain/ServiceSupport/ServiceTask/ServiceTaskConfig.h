/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-10-26 13:13:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-27 14:13:52
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_TASK_CONFIG_H_
#define __SERVICE_TASK_CONFIG_H_
#include "SystemUtilInc.h"
#include "BoardInc.h"
#include "ServiceImplInc.h"

/****************************************************CAN协议解析任务****************************************************/
//CAN总线接收回调函数
void OS_Can1ReceiveDataCallFunc(CanRxMsg* rxMsg);
//任务定义
#define NAME_SERVICE_TASK_CAN_MSG_PROC                              "CanMsgProc"
//任务优先级
#define PRIO_SERVICE_TASK_CAN_MSG_PROC                              0
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_CAN_MSG_PROC                          16 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskCanMsgProc[STK_SIZE_SERVICE_TASK_CAN_MSG_PROC];
//任务控制块
extern OS_TCB tcbServiceTaskCanMsgProc;
//任务函数
void ServiceTaskFuncCanMsgProc(void *p_arg);

/*******************************************************心跳任务********************************************************/
//任务定义
#define NAME_SERVICE_TASK_HEART                                  "Heart"
//任务优先级
#define PRIO_SERVICE_TASK_HEART                                  2
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_HEART                              16 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskHeart[STK_SIZE_SERVICE_TASK_HEART];
//任务控制块
extern OS_TCB tcbServiceTaskHeart;
//任务函数
void ServiceTaskFuncHeart(void *p_arg);

/*******************************************************信号强度检测任务********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STRENGTH_CHECK                                  "StrengthCheck"
//任务优先级
#define PRIO_SERVICE_TASK_STRENGTH_CHECK                                  3
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STRENGTH_CHECK                              16 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStrengthCheck[STK_SIZE_SERVICE_TASK_STRENGTH_CHECK];
//任务控制块
extern OS_TCB tcbServiceTaskStrengthCheck;
//任务函数
void ServiceTaskFuncStrengthCheck(void *p_arg);

/*******************************************************立禾云基本任务********************************************************/
//任务定义
#define NAME_SERVICE_TASK_CLOUD_BASE                                  "CloudBase"
//任务优先级
#define PRIO_SERVICE_TASK_CLOUD_BASE                                  5
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_CLOUD_BASE                              16 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskCloudBase[STK_SIZE_SERVICE_TASK_CLOUD_BASE];
//任务控制块
extern OS_TCB tcbServiceTaskCloudBase;
//任务函数
void ServiceTaskFuncCloudBase(void *p_arg);

/*******************************************************立禾云网络通讯任务********************************************************/
//任务定义
#define NAME_SERVICE_TASK_CLOUD_NET                                  "CloudNet"
//任务优先级
#define PRIO_SERVICE_TASK_CLOUD_NET                                  6
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_CLOUD_NET                              16 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskCloudNet[STK_SIZE_SERVICE_TASK_CLOUD_NET];
//任务控制块
extern OS_TCB tcbServiceTaskCloudNet;
//任务函数
void ServiceTaskFuncCloudNet(void *p_arg);

/**************************************************IAP任务**********************************************************/
//是否需要回复准备好标志
extern uint8_t bootLoaderNeedResp;
//任务定义
#define NAME_TASK_BOARD_IAP                                     "BoardIap"
//任务优先级                
#define PRIO_TASK_BOARD_IAP                                     7
//任务堆栈大小              
#define STK_SIZE_TASK_BOARD_IAP                                 16 * 32
//任务堆栈
extern CPU_STK stackBufferTaskBoardIap[STK_SIZE_TASK_BOARD_IAP];
//任务控制块
extern OS_TCB tcbTaskBoardIap;
//任务函数
void ServiceTaskFuncBoardIap(void *p_arg);



#endif

