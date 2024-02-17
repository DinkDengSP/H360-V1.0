/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-21 08:55:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-02 10:51:33
 *FilePath: \Appc:\LiHe\Local\02-Development\Software\H360_STM32\LH_RAIL_V2.3\Code\CommonFunc\SystemCmd.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/

#ifndef __SYSTEM_CMD_H_
#define __SYSTEM_CMD_H_
#include "BoardInc.h"
#include "Can2SubInc.h"
#include "SystemCmd.h"
#include "Can1ProcMain.h"
#include "SystemVersion.h"

//系统指令源
typedef enum SYSTEM_CMD_SRC
{
    SYSTEM_CMD_SRC_CAN1     = 0,
    SYSTEM_CMD_SRC_SELF     = 1,
}SYSTEM_CMD_SRC;

//系统指令数据,使用CCM
typedef struct SYSTEM_CMD
{
    void*           cmdDataPtr; //系统指令指针
    SYSTEM_CMD_SRC  cmdSrc;     //系统指令源表示
}SYSTEM_CMD;

//自身指令,使用CCM
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

//系统服务的IPC回调处理函数
//selfCmdPtr系统服务数据指针
//reportLevelResultPtr 指令运行的错误报警级别
//can1ErrorPtr 返回给中位机的错误代码
//detailDebugMsgPtr 输出调试信息
typedef LH_ERR (*SystemTaskSelfMsgProcFuncPtr)(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,
                                                    uint8_t* detailDebugMsgPtr);
//任务处理结构体
typedef struct SYSTEM_TASK_SELF_MSG_PROC_UNIT
{
    SystemTaskSelfMsgProcFuncPtr funcPtr;//回调函数指针
    uint8_t* debugDetailMsg;//调试详细讯息
}SYSTEM_TASK_SELF_MSG_PROC_UNIT;

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

#endif





