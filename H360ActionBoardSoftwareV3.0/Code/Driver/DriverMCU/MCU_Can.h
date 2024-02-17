/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-23 16:23:55
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\Driver\DriverMCU\MCU_Can.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_CAN_H_
#define __MCU_CAN_H_
#include "CoreInc.h"
#include "os.h"
#include "ADT_CirclePtrQueue.h"

/**************************************CAN总线相关定义****************************************/

//波特率
typedef enum MCU_CAN_BAUD
{
    MCU_CAN_BAUD_1000K,
    MCU_CAN_BAUD_500K,
    MCU_CAN_BAUD_250K,
    MCU_CAN_BAUD_125K,
    MCU_CAN_BAUD_100K,
    MCU_CAN_BAUD_75K,
    MCU_CAN_BAUD_50K,
    MCU_CAN_BAUD_COUNT,
}MCU_CAN_BAUD;

#define SELF_CAN_BPS        MCU_CAN_BAUD_1000K
/**************************************CAN总线相关定义****************************************/
//CAN1空闲队列
extern ADT_CIRCLE_PTR_QUEUE can1CirclePtrIdleQueue;

//总线接收缓存区长度
#define MCU_CAN_RECEIVE_BUFFER_LENGTH               60

//CAN口中断回调函数类型
typedef void (*MCU_CAN_RecvIntProcFuncPtr)(CanRxMsg* rxMsg);

//初始化带主板ID过滤器配置,板卡的ID为低六位,必须和设置ID保持一致
//CAN总线中断回调函数
void MCU_CAN1_Init(MCU_CAN_BAUD baud,uint8_t boardID,MCU_CAN_RecvIntProcFuncPtr callBack);

//CAN总线传输讯息
void MCU_CAN1_TransMessage(CanTxMsg* txMsg);

#endif




