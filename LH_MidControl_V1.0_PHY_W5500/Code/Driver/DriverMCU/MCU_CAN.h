/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-04 13:13:58
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-05 20:13:00
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_CAN_H_
#define __MCU_CAN_H_
#include "MCU_CanConfig.h"

//CAN1空闲队列
extern ADT_CIRCLE_PTR_QUEUE can1CirclePtrIdleQueue;
//CAN1互斥信号量
extern TX_MUTEX mutexMCU_CAN1;	

//CAN1初始化
void MCU_CAN1_Init(MCU_CAN_BPS bps,uint32_t moduleNo,uint32_t boardID,MCU_CAN_RecvDataProcFunc recvCallBack);
//CAN1发送消息
LH_ERR MCU_CAN1_SendMsg(MCU_CAN_SEND_DAT* sendMsg);



//CAN2空闲队列
extern ADT_CIRCLE_PTR_QUEUE can2CirclePtrIdleQueue;
//互斥信号量,外界对于CAN总线调用的独占
extern TX_MUTEX mutexMCU_CAN2;

//CAN2初始化
void MCU_CAN2_Init(MCU_CAN_BPS bps,uint32_t moduleNo,uint32_t boardID,MCU_CAN_RecvDataProcFunc recvCallBack);
//CAN2发送消息
LH_ERR MCU_CAN2_SendMsg(MCU_CAN_SEND_DAT* sendMsg);



#endif
