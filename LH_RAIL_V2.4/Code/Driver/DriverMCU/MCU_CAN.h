/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:03:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-30 10:57:22
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_CAN_H_
#define __MCU_CAN_H_
#include "MCU_Common.h"
#include "ADT_Inc.h"


//CAN1在网络中的设备地址
#define CAN1_BOARDID_MAIN_CONTROL        0X00000001//设备主控
#define CAN1_BOARDID_RAIL_CONTROL        0X00000023//轨道主控
//CAN1网络广播编号
#define CAN1_BOARDCAST_BOAID_ID          0X0000003F//板号
//CAN数据缓存数量
#define MCU_CAN_MSG_FIFO_LENGTH          512

//CAN模块接收加发送总共的FIFO数量
#define CAN_FIFO_SEND_RECV_MAX              64
//CAN1总线接收缓存区长度
#define MCU_CAN1_RECEIVE_FIFO_LENGTH        60
//CAN1总线发送缓存区长度
#define MCU_CAN1_SEND_FIFO_LENGTH           (CAN_FIFO_SEND_RECV_MAX-MCU_CAN1_RECEIVE_FIFO_LENGTH)

//CAN2总线接收缓存区长度
#define MCU_CAN2_RECEIVE_FIFO_LENGTH        48
//CAN2总线发送缓存区长度
#define MCU_CAN2_SEND_FIFO_LENGTH           (CAN_FIFO_SEND_RECV_MAX-MCU_CAN2_RECEIVE_FIFO_LENGTH)

//CAN模块波特率
typedef enum MCU_CAN_BPS
{
    MCU_CAN_BPS_1000K   = 0,
    MCU_CAN_BPS_500K    = 1,
    MCU_CAN_BPS_250K    = 2,
    MCU_CAN_BPS_125K    = 3,
    MCU_CAN_BPS_50K     = 4,
}MCU_CAN_BPS;

//CAN1空闲队列
extern ADT_CIRCLE_CAN_RX_PTR_QUEUE can1CirclePtrIdleQueue;
//CAN2空闲队列
extern ADT_CIRCLE_CAN_RX_PTR_QUEUE can2CirclePtrIdleQueue;

//CAN1初始化
void MCU_CAN1_Init(MCU_CAN_BPS bps,uint32_t moduleNo,uint32_t boardID,MCU_CAN_RecvDataProcFunc recvCallBack);

//CAN1发送消息
void MCU_CAN1_SendMsg(MCU_CAN_SEND_DAT* sendMsg);


//CAN2初始化
void MCU_CAN2_Init(MCU_CAN_BPS bps,uint32_t boardID,MCU_CAN_RecvDataProcFunc recvCallBack);

//CAN2发送消息
void MCU_CAN2_SendMsg(MCU_CAN_SEND_DAT* sendMsg);



#endif
