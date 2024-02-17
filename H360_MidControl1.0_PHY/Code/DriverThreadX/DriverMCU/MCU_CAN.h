/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-20 14:44:08 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-21 10:35:02 +0800
************************************************************************************************/ 
#ifndef __MCU_CAN_H_
#define __MCU_CAN_H_
#include "CoreUtil.h"

#define CAN_COMM_MODULE_NO                  0x00000005//中位机模块号
//CAN在网络中的设备地址 
#define CAN_COMM_BOARD_ID                   0x00000020//中位机板号
//CAN网络广播编号   
#define CAN_BOARDCAST_BOAID_ID              0X0000003F//板号
//CAN总线接收信息缓存   
#define CAN_MSG_BUFFER_NUMBER               512
//CAN协议支持最大一包数据数量   
#define PROTOCOL_CAN_DATA_MAX               (2048u)

//CAN模块接收加发送总共的FIFO数量
#define CAN_FIFO_SEND_RECV_MAX              64
//CAN1总线接收缓存区长度
#define MCU_CAN1_RECEIVE_FIFO_LENGTH        61
//CAN1总线发送缓存区长度
#define MCU_CAN1_SEND_FIFO_LENGTH           (CAN_FIFO_SEND_RECV_MAX-MCU_CAN1_RECEIVE_FIFO_LENGTH)
//数据发送最长等待时间
#define MCU_CAN1_SEND_WAIT_TIME_MS_MAX      (10*MCU_CAN1_SEND_FIFO_LENGTH)

//CAN2总线接收缓存区长度
#define MCU_CAN2_RECEIVE_FIFO_LENGTH        61
//CAN2总线发送缓存区长度
#define MCU_CAN2_SEND_FIFO_LENGTH           (CAN_FIFO_SEND_RECV_MAX-MCU_CAN2_RECEIVE_FIFO_LENGTH)
//数据发送最长等待时间
#define MCU_CAN2_SEND_WAIT_TIME_MS_MAX      (10*MCU_CAN2_SEND_FIFO_LENGTH)



/***************************************CAN*****************************************/
//CAN模块初始化配置信息
typedef struct MCU_CAN_BPS_SETTING
{
    uint8_t tqWithSJW;
    uint8_t tqWithBS1;
    uint8_t tqWithBS2;
    uint16_t brpDiv;
} MCU_CAN_BPS_SETTING;

//CAN ID模式
typedef enum MCU_CAN_ID_MODE
{
    MCU_CAN_ID_MODE_STD = FDCAN_STANDARD_ID,//标准帧
    MCU_CAN_ID_MODE_EXT = FDCAN_EXTENDED_ID,//扩展帧
}MCU_CAN_ID_MODE;

//接收数据数据格式
typedef struct MCU_CAN_RECV_DAT
{
    uint32_t id;
    MCU_CAN_ID_MODE idMode;
    uint8_t dataLength;
    uint8_t dataRecvBuffer[8];
}MCU_CAN_RECV_DAT;

//发送数据数据格式
typedef struct MCU_CAN_SEND_DAT
{
    uint32_t id;
    MCU_CAN_ID_MODE idMode;
    uint8_t dataLength;
    uint8_t dataSendBuffer[8];
}MCU_CAN_SEND_DAT;

//发送数据处理的函数
typedef void (*MCU_CAN_RecvDataProcFunc)(MCU_CAN_RECV_DAT* recvDat);

//CAN模块波特率
typedef enum MCU_CAN_BPS
{
    MCU_CAN_BPS_1000K   = 0,
    MCU_CAN_BPS_500K    = 1,
    MCU_CAN_BPS_250K    = 2,
    MCU_CAN_BPS_125K    = 3,
    MCU_CAN_BPS_50K     = 4,
}MCU_CAN_BPS;

//CAN1初始化
void MCU_CAN1_Init(MCU_CAN_BPS bps,uint32_t moduleNo,uint32_t boardID,MCU_CAN_RecvDataProcFunc recvCallBack);
//CAN1发送消息
int32_t MCU_CAN1_SendMsg(uint32_t u32ExtID, uint8_t *pBody, uint8_t u8Size);
//CAN1释放消息
void MCU_CAN1_ReleaseMsg(MCU_CAN_RECV_DAT* recvDat);
//CAN1获取互斥锁
void MCU_CAN1_GetLock(void);
//CAN1释放互斥锁
void MCU_CAN1_ReleaseLock(void);

//CAN2初始化
void MCU_CAN2_Init(MCU_CAN_BPS bps,uint32_t moduleNo,uint32_t boardID,MCU_CAN_RecvDataProcFunc recvCallBack);
//CAN2发送消息
int32_t MCU_CAN2_SendMsg(uint32_t u32ExtID, uint8_t *pBody, uint8_t u8Size);
//CAN2释放消息
void MCU_CAN2_ReleaseMsg(MCU_CAN_RECV_DAT* recvDat);
//CAN2获取互斥锁
void MCU_CAN2_GetLock(void);
//CAN2释放互斥锁
void MCU_CAN2_ReleaseLock(void);

#endif
