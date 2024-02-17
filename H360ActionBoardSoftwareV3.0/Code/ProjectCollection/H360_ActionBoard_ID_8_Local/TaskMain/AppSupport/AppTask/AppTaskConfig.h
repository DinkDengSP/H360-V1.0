/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:06:29
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-08 16:51:18
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "os.h"
#include "AppImplInc.h"


/****************************************************进样仓任务***************************************************/
//任务定义
#define NAME_APP_TASK_RACK_STORE_IN                                     "AppRackStoreIn"
//任务优先级            
#define PRIO_APP_TASK_RACK_STORE_IN                                     20
//任务堆栈大小          
#define STK_SIZE_APP_TASK_RACK_STORE_IN                                 8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRackStoreIn[STK_SIZE_APP_TASK_RACK_STORE_IN];
//任务控制块
extern OS_TCB tcbAppTaskRackStoreIn;
//任务函数
void AppTaskFuncRackStoreIn(void *p_arg);

/****************************************************试管架入口任务***************************************************/
//任务定义
#define NAME_APP_TASK_RACK_ENTRY                                        "AppRackEntry"
//任务优先级            
#define PRIO_APP_TASK_RACK_ENTRY                                         21
//任务堆栈大小          
#define STK_SIZE_APP_TASK_RACK_ENTRY                                     8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRackEntry[STK_SIZE_APP_TASK_RACK_ENTRY];
//任务控制块
extern OS_TCB tcbAppTaskRackEntry;
//任务函数
void AppTaskFuncRackEntry(void *p_arg);

/****************************************************试管架扫码任务***************************************************/
//任务定义
#define NAME_APP_TASK_RACK_BAR_SCAN                                     "AppRackBarScan"
//任务优先级            
#define PRIO_APP_TASK_RACK_BAR_SCAN                                     22
//任务堆栈大小          
#define STK_SIZE_APP_TASK_RACK_BAR_SCAN                                 8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRackBarScan[STK_SIZE_APP_TASK_RACK_BAR_SCAN];
//任务控制块
extern OS_TCB tcbAppTaskRackBarScan;
//任务函数
void AppTaskFuncRackBarScan(void *p_arg);

/****************************************************试管架测试任务***************************************************/
//任务定义
#define NAME_APP_TASK_RACK_TEST                                         "AppRackTest"
//任务优先级            
#define PRIO_APP_TASK_RACK_TEST                                         23
//任务堆栈大小          
#define STK_SIZE_APP_TASK_RACK_TEST                                     8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRackTest[STK_SIZE_APP_TASK_RACK_TEST];
//任务控制块
extern OS_TCB tcbAppTaskRackTest;
//任务函数
void AppTaskFuncRackTest(void *p_arg);

/****************************************************试管架进架换轨任务***************************************************/
//任务定义
#define NAME_APP_TASK_RACK_IN_SWITCH                                    "AppRackInSwitch"
//任务优先级            
#define PRIO_APP_TASK_RACK_IN_SWITCH                                    24
//任务堆栈大小          
#define STK_SIZE_APP_TASK_RACK_IN_SWITCH                                8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRackInSwitch[STK_SIZE_APP_TASK_RACK_IN_SWITCH];
//任务控制块
extern OS_TCB tcbAppTaskRackInSwitch;
//任务函数
void AppTaskFuncRackInSwitch(void *p_arg);

/****************************************************试管架出架等待1任务***************************************************/
//任务定义
#define NAME_APP_TASK_RACK_WAIT_OUT1                                    "AppRackWaitOut1"
//任务优先级            
#define PRIO_APP_TASK_RACK_WAIT_OUT1                                    25
//任务堆栈大小          
#define STK_SIZE_APP_TASK_RACK_WAIT_OUT1                                8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRackWaitOut1[STK_SIZE_APP_TASK_RACK_WAIT_OUT1];
//任务控制块
extern OS_TCB tcbAppTaskRackWaitOut1;
//任务函数
void AppTaskFuncRackWaitOut1(void *p_arg);

/****************************************************试管架出架等待2任务***************************************************/
//任务定义
#define NAME_APP_TASK_RACK_WAIT_OUT2                                    "AppRackWaitOut2"
//任务优先级            
#define PRIO_APP_TASK_RACK_WAIT_OUT2                                    26
//任务堆栈大小          
#define STK_SIZE_APP_TASK_RACK_WAIT_OUT2                                8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRackWaitOut2[STK_SIZE_APP_TASK_RACK_WAIT_OUT2];
//任务控制块
extern OS_TCB tcbAppTaskRackWaitOut2;
//任务函数
void AppTaskFuncRackWaitOut2(void *p_arg);

/****************************************************试管架出架换轨任务***************************************************/
//任务定义
#define NAME_APP_TASK_RACK_OUT_SWITCH                                   "AppRackOutSwitch"
//任务优先级            
#define PRIO_APP_TASK_RACK_OUT_SWITCH                                   27
//任务堆栈大小          
#define STK_SIZE_APP_TASK_RACK_OUT_SWITCH                               8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRackOutSwitch[STK_SIZE_APP_TASK_RACK_OUT_SWITCH];
//任务控制块
extern OS_TCB tcbAppTaskRackOutSwitch;
//任务函数
void AppTaskFuncRackOutSwitch(void *p_arg);

/****************************************************试管架出样仓任务***************************************************/
//任务定义
#define NAME_APP_TASK_RACK_STORE_OUT                                    "AppRackStoreOut"
//任务优先级            
#define PRIO_APP_TASK_RACK_STORE_OUT                                    28
//任务堆栈大小          
#define STK_SIZE_APP_TASK_RACK_STORE_OUT                                8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskRackStoreOut[STK_SIZE_APP_TASK_RACK_STORE_OUT];
//任务控制块
extern OS_TCB tcbAppTaskRackStoreOut;
//任务函数
void AppTaskFuncRackStoreOut(void *p_arg);

/****************************************************试管架参数与辅助功能任务***************************************************/
//任务定义
#define NAME_APP_TASK_PARAM_UTIL                                        "AppParamUtil"
//任务优先级            
#define PRIO_APP_TASK_PARAM_UTIL                                        29
//任务堆栈大小          
#define STK_SIZE_APP_TASK_PARAM_UTIL                                    8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskParamUtil[STK_SIZE_APP_TASK_PARAM_UTIL];
//任务控制块
extern OS_TCB tcbAppTaskParamUtil;
//任务函数
void AppTaskFuncParamUtil(void *p_arg);







#endif




