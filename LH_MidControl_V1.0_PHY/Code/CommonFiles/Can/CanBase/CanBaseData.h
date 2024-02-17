/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-30 13:56:03
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-11 11:59:39
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_BASE_DATA_H_
#define __CAN_BASE_DATA_H_
#include "stm32h7xx_hal.h"
#include "CanBaseConfig.h"
#include "CanBaseCmd.h"

//当前接收缓存错误标识
typedef enum CAN_RECV_ERR
{
    CAN_RECV_ERR_NONE                      = 0X0000,//无错误
    CAN_RECV_ERR_START_LOSS                = 0X0001,//数据接收缺失起始帧
    CAN_RECV_ERR_DATA_LOSS                 = 0X0002,//数据接收丢失数据帧
    CAN_RECV_ERR_DATA_OVERFLOW             = 0X0003,//数据接收数据帧过多
    CAN_RECV_ERR_CRC_CHECK                 = 0X0004,//数据接收结果帧校验不通过
    CAN_RECV_ERR_LONG_PACK_FIRST_NOT_FULL  = 0X0005,//长包的第一包不是满包
    CAN_RECV_ERR_LONG_PACK_LOSS_FIRST_PACK = 0X0006,//长包的第一包数据包序号不是0
    CAN_RECV_ERR_WAIT_END_RECV_LONG_PACK   = 0X0007,//等待结果包的时候收到了长包数据包
    CAN_RECV_ERR_END_RECV_LONG_PACK        = 0X0008,//接收完成的时候收到了长包数据包
    CAN_RECV_ERR_RECV_LONG_PACK            = 0X0009,//数据错误的时候收到了长包数据包
    CAN_RECV_IDLE_RECV_LONG_PACK           = 0X000A,//空闲状态的时候收到了长包数据包
    CAN_RECV_CMD_MATCH_LONG_END            = 0X000B,//长包结果包的指令匹配异常
    CAN_RECV_CMD_STAGE_MATCH_END           = 0X000C,//状态不对的时候收到了一帧结束包
}CAN_RECV_ERR;

//当前接收缓存状态
typedef enum CAN_RECV_STATE
{
    CAN_RECV_STATE_IDLE        = 0X00,//接收缓存状态空闲
    CAN_RECV_STATE_START       = 0X01,//接收到开始帧,等待数据帧
    CAN_RECV_STATE_DATA        = 0X02,//接收数据帧,等待下一帧数据帧
    CAN_RECV_STATE_WAIT_END    = 0X03,//接收完成数据帧,等待完成帧
    CAN_RECV_STATE_END         = 0X04,//接收完成
    CAN_RECV_STATE_ERR         = 0X05,//接收过程出现异常
}CAN_RECV_STATE;

//CAN总线回传ACK状态
typedef enum CAN_ACK_STATE
{
    CAN_ACK_STATE_ERR = 0,//CAN接收数据正常
    CAN_ACK_STATE_OK  = 1,//CAN接收ACK异常
}CAN_ACK_STATE;

//接收到最终组包结果
typedef struct CAN_RECV_PACK
{
    uint8_t srcModuleNo;//发送数据的源模块号
    uint8_t srcBoardAddr;//发送数据的源板号
    uint8_t targetModuleNo;//目标数据模块号
    uint8_t targetBoardAddr;//目标数据板号
    uint16_t command;//指令
    uint16_t recvDataLength;//已接收数据长度
    uint8_t* recvDataBuffer;//接收数据缓存
}CAN_RECV_PACK;
#define LENGTH_CAN_RECV_PACK    ((sizeof(CAN_RECV_PACK))/(sizeof(uint8_t)))

//接收状态缓存
typedef struct CAN_RECV_CACHE
{
    //接收状态
    CAN_RECV_STATE receiveState;
    //接收错误代码
    CAN_RECV_ERR errorState;
    //接收缓存
    CAN_RECV_PACK receivePack;
    //预期接收的数据总量
    uint16_t exceptDataByteCount;
    //预期接收的数据包总数
    uint16_t exceptDataPackCount;
    //当前接收的数据总数
    uint16_t currentDataByteCount;
    //当前接收的数据包
    uint16_t currentDataPackCount;
}CAN_RECV_CACHE;

//CAN的ID切片
typedef struct CAN_ID_SECTION
{
    //原始ID
    uint32_t canSrcId;
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
}CAN_ID_SECTION;

//CAN发送数据包的时候的指令结构体
typedef struct CAN_CMD_SEND
{
    uint8_t srcModuleNo;
    uint8_t srcBoardAddr;
    uint8_t targetModuleNo;
    uint8_t targetBoardAddr;
    uint16_t command;
    uint16_t sendDataLength;
    uint8_t* sendDataBuffer;
}CAN_CMD_SEND;

//CAN接收指令处理程序
typedef void (*CanMsgProcFuncPtr)(CAN_RECV_PACK* recvPackPtr);
//CAN信息处理的单元配置
typedef struct CAN_MSG_PROC_UNIT
{
    //指令
    CAN_CMD cmd;     
    //回调处理函数      
    CanMsgProcFuncPtr canMsgProcFuncPtr;
}CAN_MSG_PROC_UNIT;


#endif




