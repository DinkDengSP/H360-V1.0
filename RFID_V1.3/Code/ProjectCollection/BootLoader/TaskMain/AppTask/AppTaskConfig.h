/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-31 13:28:40
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-13 12:15:55
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "BoardInc.h"
#include "CanProcMain.h"
#include "AppUtilParam.h"
#include "UartCmdProtocol.h"
#include "AppImplInc.h"
//CAN1接收指令处理程序
//can1CmdPtr CAN1数据指针
//reportLevelResultPtr 指令运行的错误报警级别
//can1ErrorPtr 返回给中位机的错误代码
//detailDebugMsgPtr 输出调试信息
typedef void (*SystemTaskCanMsgProcFuncPtr)(CAN_CMD_RECV* canCmdPtr,uint8_t* detailDebugMsgPtr);
//CAN1信息处理的单元配置
typedef struct SYSTEM_TASK_CAN_MSG_PROC_UNIT
{
    //指令
    CAN_CMD_DEF cmd;     
    //回调处理函数      
    SystemTaskCanMsgProcFuncPtr canCmdProcFuncPtr;
    //调试详细讯息
    uint8_t* debugDetailMsg;
}SYSTEM_TASK_CAN_MSG_PROC_UNIT;

//Uart接收指令处理程序
//uartCmdPtr 串口数据指针
//reportLevelResultPtr 指令运行的错误报警级别
//detailDebugMsgPtr 输出调试信息
typedef void (*SystemTaskUartMsgProcFuncPtr)(UART_CMD_UTIL* uartCmdPtr,uint8_t* detailDebugMsgPtr);
//UART信息处理的单元配置
typedef struct SYSTEM_TASK_UART_MSG_PROC_UNIT
{
    //指令
    UART_CMD_DEF cmd;     
    //回调处理函数      
    SystemTaskUartMsgProcFuncPtr uartCmdProcFuncPtr;
    //调试详细讯息
    uint8_t* debugDetailMsg;
}SYSTEM_TASK_UART_MSG_PROC_UNIT;

/**************************************CAN总线接收数据处理*************************************/
//CAN总线接收回调函数
void OS_CanReceiveDataCallFunc(CanRxMsg* rxMsg);
//任务名称
#define NAME_TASK_CAN_RECV                     "CanRecv"
//任务优先级
#define PRIO_TASK_CAN_RECV                     0
//任务堆栈大小
#define STK_SIZE_TASK_CAN_RECV                 4 * 32
//任务堆栈
extern CPU_STK stackBufferTaskCanRecv[STK_SIZE_TASK_CAN_RECV];
//任务控制块
extern OS_TCB tcbTaskCanRecv;
//任务函数
void TaskFuncCanRecv(void *p_arg);

/**************************************Uart接收数据处理*************************************/
//空闲中断处理函数
void Uart1IdleIntRecvFunc(uint8_t *dataBuff,uint16_t dataLen);
//任务名称
#define NAME_TASK_UART1_RECV                     "Uart1Recv"
//任务优先级
#define PRIO_TASK_UART1_RECV                     1
//任务堆栈大小
#define STK_SIZE_TASK_UART1_RECV                 4 * 32
//任务堆栈
extern CPU_STK stackBufferTaskUart1Recv[STK_SIZE_TASK_UART1_RECV];
//任务控制块
extern OS_TCB tcbTaskUart1Recv;
//任务函数
void TaskFuncUart1Recv(void *p_arg);

/******************************************心跳任务*********************************/
//任务名称定义
#define NAME_TASK_HEART                         "Heart"
//任务优先级
#define PRIO_TASK_HEART                         2
//任务堆栈大小
#define STK_SIZE_TASK_HEART                     2 * 32
//任务堆栈
extern CPU_STK stackBufferTaskHeart[STK_SIZE_TASK_HEART];
//任务控制块
extern OS_TCB tcbTaskHeart;
//任务函数
void TaskFuncHeart(void *p_arg);



#endif




