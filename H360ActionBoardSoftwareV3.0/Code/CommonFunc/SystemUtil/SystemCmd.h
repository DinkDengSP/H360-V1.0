/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-03 09:38:35
 *FilePath: \H360_ActionBoard_ID_1c:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\SystemCmd.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_COMMAND_H_
#define __SYSTEM_COMMAND_H_
#include "stm32f4xx.h"
#include "ErrorCode.h"
#include "SystemVersion.h"

//指令来源
typedef enum TASK_CMD_SRC
{
    TASK_CMD_SRC_CAN,
    TASK_CMD_SRC_SELF,
}TASK_CMD_SRC;

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

//系统指令集,包括指令来源和指令的数据结构体
typedef struct TASK_CMD
{
    TASK_CMD_SRC cmdSrc;
    //指令携带的数据,根据来源,任务自己做自主判定
    void* cmdData;
}TASK_CMD;



//任务CAN总线接收数据协议处理函数原型
//参数分别是接收参数 接收参数的长度 响应指令的返回数据 返回数据长度
typedef LH_ERR (*SystemTaskCanMsgProcFuncPtr)(void* cachePtr,uint8_t* detailDebugMsgPtr);
//任务处理结构体
typedef struct SYSTEM_TASK_CAN_MSG_PROC_UNIT
{
    //任务处理函数指针
    SystemTaskCanMsgProcFuncPtr funcPtr;
    //任务处理函数需求的参数个数,只能大于等于这个设定值,不能小于这个设定值
    //注意,接收到的前四个字节的数据为指令,要排除掉,所以应该是接收到的缓存区数据量-1
    uint16_t paramDataMinLength;
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



