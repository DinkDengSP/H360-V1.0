/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:05:18 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-19 14:42:41 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_NET_RECV_H_
#define __SRV_IMPL_NET_RECV_H_
#include "SrvImplBase.h"

//处理通讯协议接收到的数据
LH_ERR SrvImplNetRecvWithProtocol(uint8_t* recvDataPtr, uint16_t size);

//接收到完整的一包数据之后,进行数据打包和数据转发
LH_ERR SrvImplNetRecvWithCompletePack(NET_CMD_RECV_STATE* recvCompletePack);

//对处理完成的TCIIP数据包进行转发处理
LH_ERR SrvImplNetRecvDispatchCommand(NET_COMMAND_PACK* commandPackPtr);

//LAN8700数据接收处理
void SvImplNetLAN8700Recv(NX_DATA_RECV* nxDataRecvPtr);

//W5500数据接收处理
void SvImplNetW5500Recv(NX_DATA_RECV* nxDataRecvPtr);


#endif




