/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:13:57
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-22 13:13:28
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_W5500_H_
#define __BOARD_W5500_H_
#include "BoardCommon.h"
#include "socket.h"
#include "w5500.h"
#include "w5500_conf.h"

//W5500单包最大长度
#define NET_W5500_SINGLE_PACK_LENGTH_MAX        2048

//中断事务处理函数
//该函数去处理网卡数据业务,参数为本次接受的数据长度
typedef void(*BoardW5500RecvDataIntProc)(uint8_t* bufferRecvPtr,uint16_t recvDataLength);

//W5500初始化
LH_ERR BoardW5500_Init(BoardW5500RecvDataIntProc callBack);

//网卡状态机
void BoardW5500_DoTcpServer(void);

//网卡发送数据
void BoardW5500_SendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);

//网卡获取连接状态
uint8_t BoardW5500_GetConnectState(void);

//网卡重启
void resetw5500(void);

#endif



