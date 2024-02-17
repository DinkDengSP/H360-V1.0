/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-29 18:30:18
 *FilePath: \H360_ActionBoard_ID_4d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanProtocol\CanSlaveDataTypeConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SLAVE_DATA_TYPE_CONFIG_H_
#define __CAN_SLAVE_DATA_TYPE_CONFIG_H_
#include "CanBaseCommon.h"

//缓存通道状态
typedef enum CAN_SLAVE_CHANNEL_STATE
{
    CAN_SLAVE_CHANNEL_IDLE      = 0X00,//空闲状态
    CAN_SLAVE_CHANNEL_WAIT_DATA = 0x01,//接收到起始,等待数据
    CAN_SLAVE_CHANNEL_WAIT_END  = 0X02,//接收数据包够了,等待结束
    CAN_SLAVE_CHANNEL_ERR       = 0X03,//接收出错
    CAN_SLAVE_CHANNEL_OK        = 0x04,//接收完成,等待处理
    CAN_SLAVE_CHANNEL_BUSY      = 0X05,//正在处理,忙
}CAN_SLAVE_CHANNEL_STATE;

//从机CAN每个信道的接收缓存
typedef struct CAN_SLAVE_CHANNEL_CACHE
{
    uint8_t recvCacheArray[CAN_PROTOCOL_SIGNAL_PACK_MAX_LEN];//接收缓存
    uint16_t exceptLength;//期望的数据长度
    uint8_t exceptPackCount;//期望包数量
    uint16_t currentLength;//当前接收长度
    uint8_t currentPackCount;//当前接收包数量
    CAN_SLAVE_CHANNEL_STATE currentState;//当前通道接收状态
    uint8_t lastFrameCode;//上一次接收到的帧号
    uint8_t senderBoardAddr;//数据发送方的地址
    CAN_BASE_ACK ackCode;//返回的ACK错误代码
    OS_TCB* dispatchTaskPtrTCB;//与缓存关联的任务处理通道配置
    uint8_t selfChannelNumber;//标识这个缓存的信道号码
}CAN_SLAVE_CHANNEL_CACHE;

#define CAN_SLAVE_CHANNEL_CHAHE_SIZE    (sizeof(CAN_SLAVE_CHANNEL_CACHE)/sizeof(uint8_t))

//转发配置
typedef struct CAN_SLAVE_DISPATCH_CONFIG
{
    uint8_t channelNumber;
    OS_TCB* dispatchTaskPtrTCB;
}CAN_SLAVE_DISPATCH_CONFIG;


#endif





