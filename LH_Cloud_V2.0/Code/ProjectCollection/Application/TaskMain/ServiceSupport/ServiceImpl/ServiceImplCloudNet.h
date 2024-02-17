/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-08 11:05:25
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-08 13:49:18
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_CLOUD_NET_H_
#define __SERVICE_IMPL_CLOUD_NET_H_
#include "ServiceImplBase.h"

#define APP_DEFAULT_SOCKET_NUM          0

//设备连接网络
LH_ERR ServiceImplCloudNet_Connect(uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort);

//设备断开网络连接
LH_ERR ServiceImplCloudNet_Disconnect(void);

//设备发送一包数据
LH_ERR ServiceImplCloudNet_SendWithoutConnect(uint16_t sendLength,uint8_t* sendBufferPtr);

//设备读取一次数据
LH_ERR ServiceImplCloudNet_ReadData(uint16_t exceptLength,uint16_t timeOutMs,uint8_t** recvBuffer,uint16_t* realReadLength);

//设备集成发送并接收数据
LH_ERR ServiceImplCloudNet_SendRecvWithConnect(uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort,
                                            uint16_t exceptLength,uint16_t timeOutMs,uint16_t sendLength,uint8_t* sendBufferPtr,
                                            uint8_t** recvBuffer,uint16_t* realReadLength);

#endif


