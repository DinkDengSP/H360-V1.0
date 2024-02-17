/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-02 15:48:54
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-03 15:16:41
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_CMD_H_
#define __SYSTEM_CMD_H_
#include "stm32f4xx.h"
#include "os.h"
#include "Can1ProcMain.h"

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

//指令来源
typedef enum SYSTEM_CMD_SRC
{
    SYSTEM_CMD_SRC_CAN,
    SYSTEM_CMD_SRC_SELF,
    SYSTEM_CMD_SRC_UART,
}SYSTEM_CMD_SRC;

//系统指令集,包括指令来源和指令的数据结构体
typedef struct SYSTEM_CMD
{
    //指令源
    SYSTEM_CMD_SRC cmdSrc;
    //指令携带的数据,根据来源,任务自己做自主判定
    void* cmdDataPtr;
}SYSTEM_CMD;

//自身指令
typedef struct SYSTEM_CMD_SELF
{
    uint16_t commandIndex;//需要调用的指令的序号
    int32_t commandParam1;//调用的指令的参数1,若没有,则不用管
    int32_t commandParam2;//调用的指令的参数2
    int32_t commandParam3;//调用的指令的参数3
    int32_t commandParam4;//调用的指令的参数4
    int32_t commandParam5;//调用的指令的参数5
    void* commandDataSpecialChannel;//指令传递的时候附带传递的数据,各个任务通道是不同的
}SYSTEM_CMD_SELF;

//任务CAN总线接收数据协议处理函数原型
//CAN1接收指令处理程序
//can1CmdPtr CAN1数据指针
//reportLevelResultPtr 指令运行的错误报警级别
//can1ErrorPtr 返回给中位机的错误代码
//detailDebugMsgPtr 输出调试信息
typedef void (*SystemTaskCanMsgProcFuncPtr)(CAN1_CMD_RECV* can1CmdPtr,uint8_t* detailDebugMsgPtr);
//CAN1信息处理的单元配置
typedef struct SYSTEM_TASK_CAN_MSG_PROC_UNIT
{
    //指令
    CAN1_CMD_DEF cmd;     
    //回调处理函数      
    SystemTaskCanMsgProcFuncPtr can1CmdProcFuncPtr;
    //调试详细讯息
    uint8_t* debugDetailMsg;
}SYSTEM_TASK_CAN_MSG_PROC_UNIT;

//自身指令处理
typedef LH_ERR (*SystemTaskSelfMsgProcFuncPtr)(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//任务处理结构体
typedef struct SYSTEM_TASK_SELF_MSG_PROC_UNIT
{
    SystemTaskSelfMsgProcFuncPtr funcPtr;//回调函数指针
//    uint16_t paramDataMinLength;
    uint8_t* debugDetailMsg;//调试详细讯息
}SYSTEM_TASK_SELF_MSG_PROC_UNIT;




#endif


