#ifndef __CAN1_DATA_TYPE_H_
#define __CAN1_DATA_TYPE_H_
#include "Can1ProcConfig.h"

//CAN1解析使用CCM内存

//当前接收缓存状态
typedef enum CAN1_RECV_STATE
{
    CAN1_RECV_STATE_IDLE,
    CAN1_RECV_STATE_START,
    CAN1_RECV_STATE_DATA,
    CAN1_RECV_STATE_WAIT_END,
    CAN1_RECV_STATE_END,
    CAN1_RECV_STATE_ERR,
}CAN1_RECV_STATE;

//当前接收缓存错误标识
typedef enum CAN1_RECV_ERR
{
    CAN1_RECV_ERR_NONE = 0X0000,
    CAN1_RECV_ERR_START_LOSS = 0X0001,
    CAN1_RECV_ERR_DATA_LOSS = 0X0002,
    CAN1_RECV_ERR_DATA_OVERFLOW = 0X0003,
    CAN1_RECV_ERR_CRC_CHECK = 0X0004,
    CAN1_RECV_ERR_UNKNOW = 0X0005,
}CAN1_RECV_ERR;

//接收到的指令结构体
typedef struct CAN1_CMD_RECV
{
    uint8_t srcModuleNo;
    uint8_t srcBoardAddr;
    uint8_t targetModuleNo;
    uint8_t targetBoardAddr;
    uint16_t command;
    uint16_t recvDataLength;
    uint8_t recvDataBuffer[CAN1_SINGLE_PACK_MAX_LENGTH];
}CAN1_CMD_RECV;

//接收指令缓存
typedef struct CAN1_RECV_CACHE
{
    //接收状态
    CAN1_RECV_STATE cmdRecvState;
    //接收错误代码
    CAN1_RECV_ERR errorState;
    //接收缓存
    CAN1_CMD_RECV cmdRecv;
    //预期接收的数据总量
    uint16_t exceptDataByteCount;
    //预期接收的数据包总数
    uint16_t exceptDataPackCount;
    //当前接收的数据总数
    uint16_t currentDataByteCount;
    //当前接收的数据包
    uint16_t currentDataPackCount;
}CAN1_RECV_CACHE;

//CAN1的ID解析结果
typedef struct CAN1_ID_ANAYSIS_RESULT
{
    //源模块号
    uint8_t srcModuleNo;
    //源板号
    uint8_t srcBoardAddr;
    //目标模块号
    uint8_t targetModuleNo;
    //目标板号
    uint8_t targetBoardAddr;
    //指令
    uint16_t command;
}CAN1_ID_ANAYSIS_RESULT;

//CAN1发送数据包的时候的指令结构体
typedef struct CAN1_CMD_SEND
{
    uint8_t srcModuleNo;
    uint8_t srcBoardAddr;
    uint8_t targetModuleNo;
    uint8_t targetBoardAddr;
    uint16_t command;
    uint16_t sendDataLength;
    uint8_t* sendDataBuffer;
}CAN1_CMD_SEND;

//CAN1数据转发的基础元素
typedef struct CAN1_DISPATCH_UNIT
{
    CAN1_CMD_DEF commandValue;//CAN1指令
    OS_TCB* tcbTaskPtr;//CAN1指令与对应的处理函数
}CAN1_DISPATCH_UNIT;


#endif





