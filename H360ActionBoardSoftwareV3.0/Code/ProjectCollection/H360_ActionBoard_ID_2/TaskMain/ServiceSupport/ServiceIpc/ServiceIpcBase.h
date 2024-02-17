/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:11
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-15 09:17:51
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_General\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_BASE_H_
#define __SERVICE_IPC_BASE_H_
#include "CommonServiceIpcBase.h"
#include "ServiceImplBase.h"

//串口1
typedef struct DATA_IPC_SERIAL_USER_RS1
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //如果有新的交互数据需要加入,那么在此处添加新的数据
    uint16_t sendDataLength;//发送数据长度
    uint8_t sendCommandBuffer[SERIAL_RS1_BUFFER_MAX_LENGTH];//发送数据缓存
    uint16_t recvDataLength;//返回数据长度
    uint8_t recvDataBuffer[SERIAL_RS1_BUFFER_MAX_LENGTH];//返回数据缓存
}DATA_IPC_SERIAL_USER_RS1;



#endif




