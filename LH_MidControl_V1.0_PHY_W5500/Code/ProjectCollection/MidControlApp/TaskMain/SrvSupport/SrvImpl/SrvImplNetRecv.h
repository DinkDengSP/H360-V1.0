/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-04 19:39:53
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-05 09:12:17
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SRV_IMPL_NET_RECV_H_
#define __SRV_IMPL_NET_RECV_H_
#include "BoardInc.h"

//处理通讯协议接收到的数据
LH_ERR SrvImplNetRecvWithProtocol(uint8_t* recvDataPtr, uint16_t size);

//接收到完整的一包数据之后,进行数据打包和数据转发
LH_ERR SrvImplNetRecvWithCompletePack(NET_CMD_RECV_STATE* recvCompletePack);

//对处理完成的TCIIP数据包进行转发处理
LH_ERR SrvImplNetRecvDispatchCommand(NET_COMMAND_PACK* commandPackPtr);

#endif



