/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 15:49:27
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-07 10:32:28
 *FilePath: \DeviceMainApplicationc:\LiHe\Local\02-Development\Software\H360_STM32\H360MainControlAppV2.2\Code\CommonFunc\SystemCommUtil\SystemCmd.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_CMD_H_
#define __SYSTEM_CMD_H_
#include "MCU_Inc.h"
#include "Can1CmdDef.h"

//CAN1一包数据的最大长度
#define CAN1_SINGLE_PACK_MAX_LENGTH     2048

//系统指令源
typedef enum SYSTEM_CMD_SRC
{
    SYSTEM_CMD_SRC_CAN1,
    SYSTEM_CMD_SRC_SELF,
}SYSTEM_CMD_SRC;

//系统指令数据
typedef struct SYSTEM_CMD
{
    //系统指令指针
    void* systemCommandDataPtr;
    //系统指令源表示
    SYSTEM_CMD_SRC commandSrc;
}SYSTEM_CMD;

//自身指令,使用CCRAM
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

typedef LH_ERR (*ServiceTaskSelfCommandProcFuncPtr)(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);

typedef struct SERVICE_TASK_SELF_PROC_UNIT
{
    ServiceTaskSelfCommandProcFuncPtr funcPtr;//回调函数指针
    uint8_t* debugDetailMsg;//调试详细讯息
}SERVICE_TASK_SELF_PROC_UNIT;

//CAN1口指令
typedef struct SYSTEM_CMD_CAN1
{
    uint8_t srcModuleNo;
    uint8_t srcBoardAddr;
    uint8_t targetModuleNo;
    uint8_t targetBoardAddr;
    uint16_t command;
    uint16_t recvDataLength;
    uint8_t can1RecvDataBuffer[CAN1_SINGLE_PACK_MAX_LENGTH];
}SYSTEM_CMD_CAN1;

//CAN1处理函数
typedef LH_ERR (*ServiceTaskCan1CommandProcFuncPtr)(SYSTEM_CMD_CAN1* can1CmdPtr,uint8_t* detailDebugMsgPtr);

//CAN1信息处理的单元配置
typedef struct SERVICE_TASK_CAN1_PROC_UNIT
{
    CAN1_DEBUG_CMD command;//CAN1指令
    uint8_t* debugDetailMsg;//调试详细讯息
    ServiceTaskCan1CommandProcFuncPtr funcPtr;//回调处理函数
}SERVICE_TASK_CAN1_PROC_UNIT;



#endif





