/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:01:59
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-24 16:11:35
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceTaskConfig.h
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_TASK_CONFIG_H_
#define __SERVICE_TASK_CONFIG_H_
#include "ServiceImplInc.h"
#include "ServiceIpcInc.h"

//服务层占用0-15优先级

/********************************************ServiceHeart*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_HEART                                 "ServiceHeart"
//任务优先级                
#define PRIO_TASK_SERVICE_HEART                                 0
//任务堆栈大小              
#define STK_SIZE_TASK_SERVICE_HEART                             8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceHeart[STK_SIZE_TASK_SERVICE_HEART];
//任务控制块
extern OS_TCB tcbTaskServiceHeart;
//任务函数
void TaskFuncServiceHeart(void *p_arg);

/********************************************ServiceCan2Recv*****************************************************/
//CAN2总线接收回调函数
void OS_Can2ReceiveDataCallFunc(MCU_CAN_RECV_DAT* rxMsg);
//任务定义
#define NAME_TASK_SERVICE_CAN2_RECV                             "ServiceCan2Recv"
//任务优先级            
#define PRIO_TASK_SERVICE_CAN2_RECV                             1
//任务堆栈大小          
#define STK_SIZE_TASK_SERVICE_CAN2_RECV                         8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceCan2Recv[STK_SIZE_TASK_SERVICE_CAN2_RECV];
//任务控制块
extern OS_TCB tcbTaskServiceCan2Recv;
//任务函数
void TaskFuncServiceCan2Recv(void *p_arg);

/********************************************ServiceCan1Recv*****************************************************/
//CAN总线接收回调函数
void OS_Can1ReceiveDataCallFunc(MCU_CAN_RECV_DAT* rxMsg);
//任务定义
#define NAME_TASK_SERVICE_CAN1_RECV                             "ServiceCan1Recv"
//任务优先级            
#define PRIO_TASK_SERVICE_CAN1_RECV                             2
//任务堆栈大小          
#define STK_SIZE_TASK_SERVICE_CAN1_RECV                         8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceCan1Recv[STK_SIZE_TASK_SERVICE_CAN1_RECV];
//任务控制块
extern OS_TCB tcbTaskServiceCan1Recv;
//任务函数
void TaskFuncServiceCan1Recv(void *p_arg);

/********************************************ServiceRailPushIn1*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_RAIL_PUSH_IN1                         "ServiceRailPushIn1"
//任务优先级        
#define PRIO_TASK_SERVICE_RAIL_PUSH_IN1                         4
//任务堆栈大小      
#define STK_SIZE_TASK_SERVICE_RAIL_PUSH_IN1                     8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceRailPushIn1[STK_SIZE_TASK_SERVICE_RAIL_PUSH_IN1];
//任务控制块
extern OS_TCB tcbTaskServiceRailPushIn1;
//任务函数
void TaskFuncServiceRailPushIn1(void *p_arg);

/********************************************ServiceRailPushIn2*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_RAIL_PUSH_IN2                         "ServiceRailPushIn2"
//任务优先级        
#define PRIO_TASK_SERVICE_RAIL_PUSH_IN2                         5
//任务堆栈大小      
#define STK_SIZE_TASK_SERVICE_RAIL_PUSH_IN2                     8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceRailPushIn2[STK_SIZE_TASK_SERVICE_RAIL_PUSH_IN2];
//任务控制块
extern OS_TCB tcbTaskServiceRailPushIn2;
//任务函数
void TaskFuncServiceRailPushIn2(void *p_arg);

/********************************************ServiceRailPushOut1*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_RAIL_PUSH_OUT1                        "ServiceRailPushOut1"
//任务优先级
#define PRIO_TASK_SERVICE_RAIL_PUSH_OUT1                        6
//任务堆栈大小
#define STK_SIZE_TASK_SERVICE_RAIL_PUSH_OUT1                    8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceRailPushOut1[STK_SIZE_TASK_SERVICE_RAIL_PUSH_OUT1];
//任务控制块
extern OS_TCB tcbTaskServiceRailPushOut1;
//任务函数
void TaskFuncServiceRailPushOut1(void *p_arg);

/********************************************ServiceRailPushOut2*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_RAIL_PUSH_OUT2                        "ServiceRailPushOut2"
//任务优先级        
#define PRIO_TASK_SERVICE_RAIL_PUSH_OUT2                        7
//任务堆栈大小      
#define STK_SIZE_TASK_SERVICE_RAIL_PUSH_OUT2                    8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceRailPushOut2[STK_SIZE_TASK_SERVICE_RAIL_PUSH_OUT2];
//任务控制块
extern OS_TCB tcbTaskServiceRailPushOut2;
//任务函数
void TaskFuncServiceRailPushOut2(void *p_arg);

/********************************************ServiceRailHand*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_RAIL_HAND                             "ServiceRailHand"
//任务优先级            
#define PRIO_TASK_SERVICE_RAIL_HAND                             8
//任务堆栈大小          
#define STK_SIZE_TASK_SERVICE_RAIL_HAND                         8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceRailHand[STK_SIZE_TASK_SERVICE_RAIL_HAND];
//任务控制块
extern OS_TCB tcbTaskServiceRailHand;
//任务函数
void TaskFuncServiceRailHand(void *p_arg);

/********************************************ServiceRailTrans*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_RAIL_TRANS                            "ServiceRailTrans"
//任务优先级            
#define PRIO_TASK_SERVICE_RAIL_TRANS                            9
//任务堆栈大小          
#define STK_SIZE_TASK_SERVICE_RAIL_TRANS                        8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceRailTrans[STK_SIZE_TASK_SERVICE_RAIL_TRANS];
//任务控制块
extern OS_TCB tcbTaskServiceRailTrans;
//任务函数
void TaskFuncServiceRailTrans(void *p_arg);

/********************************************ServiceRailComplex*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_RAIL_COMPLEX                          "ServiceRailComplex"
//任务优先级        
#define PRIO_TASK_SERVICE_RAIL_COMPLEX                          10
//任务堆栈大小      
#define STK_SIZE_TASK_SERVICE_RAIL_COMPLEX                      8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceRailComplex[STK_SIZE_TASK_SERVICE_RAIL_COMPLEX];
//任务控制块
extern OS_TCB tcbTaskServiceRailComplex;
//任务函数
void TaskFuncServiceRailComplex(void *p_arg);

/********************************************ServiceBarScan*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_BAR_SCAN                              "ServiceBarScan"
//任务优先级            
#define PRIO_TASK_SERVICE_BAR_SCAN                              11
//任务堆栈大小          
#define STK_SIZE_TASK_SERVICE_BAR_SCAN                          8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceBarScan[STK_SIZE_TASK_SERVICE_BAR_SCAN];
//任务控制块
extern OS_TCB tcbTaskServiceBarScan;
//任务函数
void TaskFuncServiceBarScan(void *p_arg);

/********************************************ServiceDebug*****************************************************/
//任务定义
#define NAME_TASK_SERVICE_DEBUG                                 "ServiceDebug"
//任务优先级                
#define PRIO_TASK_SERVICE_DEBUG                                 12
//任务堆栈大小              
#define STK_SIZE_TASK_SERVICE_DEBUG                             8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskServiceDebug[STK_SIZE_TASK_SERVICE_DEBUG];
//任务控制块
extern OS_TCB tcbTaskServiceDebug;
//任务函数
void TaskFuncServiceDebug(void *p_arg);







#endif




