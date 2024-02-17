/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-02 16:59:28
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-04 21:43:44
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_CMD_H_
#define __SYSTEM_CMD_H_
#include "CoreUtil.h"

//系统指令源
typedef enum SYSTEM_MSG_SRC
{
    SYSTEM_CMD_SRC_CAN,
    SYSTEM_CMD_SRC_SELF,
    SYSTEM_CMD_SRC_NET,
}SYSTEM_MSG_SRC;

//系统消息数据
typedef struct SYSTEM_MSG
{
    //系统指令指针
    void* msgDataPtr;
    //系统指令源表示
    SYSTEM_MSG_SRC msgSrc;
}SYSTEM_MSG;
#define LENGTH_SYSTEM_MSG       (sizeof(SYSTEM_MSG)/sizeof(uint8_t))

//自身指令,使用CCRAM
typedef struct SYSTEM_MSG_IPC
{
    uint16_t cmdIndex;//需要调用的指令的序号
    int32_t cmdParam1;//调用的指令的参数1,若没有,则不用管
    int32_t cmdParam2;//调用的指令的参数2
    int32_t cmdParam3;//调用的指令的参数3
    int32_t cmdParam4;//调用的指令的参数4
    int32_t cmdParam5;//调用的指令的参数5
    uint8_t* paramBuffer;//参数不够的,使用这个缓冲区存放
    uint16_t paramLength;//缓冲区的参数长度
    void* ipcBasePtr;//发送方的IPC结构体指针,用于处理线程可以对这个结构进行修改
}SYSTEM_MSG_IPC;
//自身调用指令的处理函数
typedef LH_ERR (*SystemMsgIpcProcFuncPtr)(SYSTEM_MSG_IPC* msgIpcPtr);



#endif




