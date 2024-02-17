/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-30 08:54:30
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-02 17:42:34
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_TASK_CONFIG_H_
#define __SYSTEM_TASK_CONFIG_H_
#include "BoardInc.h"
#include "os.h"
#include "SystemCmd.h"

//使能调试信息输出
#define SYSTEM_DEBUG_MSG_SHOW_ENABLE           1


//这个打印信息是动作任务在响应应用任务调用时候的输出信息
#if(SYSTEM_DEBUG_MSG_SHOW_ENABLE == 1)
//输出任务调试讯息
#define SystemDebugShowMsgStart(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)       MCU_Uart3Printf("%s%s - %d - %d - %d - %d - %d\r\n",(uint8_t*)msgStr, (uint8_t*)paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)                                                                                                           
//输出任务结束讯息
#define SystemDebugShowMsgEnd(endStr,userErrorCode)                                                      MCU_Uart3Printf("%s - 0X%08X\r\n",endStr,userErrorCode)
//输出错误信息
#define SystemDebugShowMsgError(errorCode)                                                               MCU_Uart3Printf("!!Action Error Func : %s , Line : %d , ErrorCode : 0X%08X\r\n", __FUNCTION__,__LINE__,errorCode)
#else
//不输出信息调试
#define SystemDebugShowMsgStart(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)       asm("nop")
#define SystemDebugShowMsgEnd(endStr,userErrorCode)                                                      asm("nop")
#define SystemDebugShowMsgError(errorCode)                                                               asm("nop")
#endif


/********************************************启动任务************************************************************/
//启动任务
#define NAME_TASK_START                     "SystemStart Task"
//任务优先级
#define PRIO_TASK_START                     0
//任务堆栈大小
#define STK_SIZE_TASK_START                 8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START];
//任务控制块
extern OS_TCB tcbTaskStart;
//任务函数
void TaskFuncStart(void *p_arg);

/********************************************ServiceHeart*****************************************************/
//任务定义
#define NAME_TASK_HEART                                 "ServiceHeart"
//任务优先级                
#define PRIO_TASK_HEART                                 0
//任务堆栈大小              
#define STK_SIZE_TASK_HEART                             8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskHeart[STK_SIZE_TASK_HEART];
//任务控制块
extern OS_TCB tcbTaskHeart;
//任务函数
void TaskFuncHeart(void *p_arg);

/********************************************ServiceCan2Recv*****************************************************/
//CAN2总线接收回调函数
void OS_Can2ReceiveDataCallFunc(MCU_CAN_RECV_DAT* rxMsg);
//任务定义
#define NAME_TASK_CAN2_RECV                             "ServiceCan2Recv"
//任务优先级            
#define PRIO_TASK_CAN2_RECV                             1
//任务堆栈大小          
#define STK_SIZE_TASK_CAN2_RECV                         8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskCan2Recv[STK_SIZE_TASK_CAN2_RECV];
//任务控制块
extern OS_TCB tcbTaskCan2Recv;
//任务函数
void TaskFuncCan2Recv(void *p_arg);

/********************************************ServiceCan1Recv*****************************************************/
//CAN总线接收回调函数
void OS_Can1ReceiveDataCallFunc(MCU_CAN_RECV_DAT* rxMsg);
//任务定义
#define NAME_TASK_CAN1_RECV                             "ServiceCan1Recv"
//任务优先级            
#define PRIO_TASK_CAN1_RECV                             2
//任务堆栈大小          
#define STK_SIZE_TASK_CAN1_RECV                         8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskCan1Recv[STK_SIZE_TASK_CAN1_RECV];
//任务控制块
extern OS_TCB tcbTaskCan1Recv;
//任务函数
void TaskFuncCan1Recv(void *p_arg);

/********************************************ServiceIap*****************************************************/
extern uint8_t bootLoaderNeedResp;
//任务定义
#define NAME_TASK_BOARD_IAP                                 "ServiceBoardIap"
//任务优先级                
#define PRIO_TASK_BOARD_IAP                                 3
//任务堆栈大小              
#define STK_SIZE_TASK_BOARD_IAP                             8 * 32
//任务堆栈
extern CPU_STK stackBufferTaskBoardIap[STK_SIZE_TASK_BOARD_IAP];
//任务控制块
extern OS_TCB tcbTaskBoardIap;
//任务函数
void TaskFuncIap(void *p_arg);



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



