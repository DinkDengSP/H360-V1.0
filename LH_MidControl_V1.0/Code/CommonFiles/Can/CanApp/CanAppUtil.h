/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-06 11:26:57
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-10-19 13:47:49
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_APP_UTIL_H_
#define __CAN_APP_UTIL_H_
#include "CanBase.h"

//CAN总线回传ACK状态
LH_ERR CanSendAckDefault(uint8_t targetModule,uint8_t targetBoard,CAN_ACK_STATE ackState);

//CAN总线发送IAP准备
LH_ERR CanSendIapPrepare(uint8_t targetModule,uint8_t targetBoard,uint8_t iapBoardID);

//CAN总线发送IAP开始
LH_ERR CanSendIapStart(uint8_t targetModule,uint8_t targetBoard,uint8_t iapBoardID,uint32_t byteCount,uint16_t packCount,uint16_t packackLengt);

//CAN总线发送IAP数据
LH_ERR CanSendIapData(uint8_t targetModule,uint8_t targetBoard,uint8_t iapBoardID,uint16_t packIndex,uint8_t* datBuf,uint16_t dataLength);

//CAN总线发送IAP结果
LH_ERR CanSendIapEnd(uint8_t targetModule,uint8_t targetBoard,uint8_t iapBoardID,uint32_t crcCode);

//CAN总线发送读取Boot版本号
LH_ERR CanSendReadBootVersion(uint8_t targetModule,uint8_t targetBoard);

//CAN发送读取APP版本号
LH_ERR CanSendReadAppVersion(uint8_t targetModule,uint8_t targetBoard);

//CAN发送调试指令,通用版本,免疫,轨道
LH_ERR CanSendDebugControlMsgComm(uint8_t targetModule,uint8_t targetBoard,uint16_t debugCmd,uint8_t* paramBufferPtr,uint16_t paramLength);

//CAN发送调试指令,生化版本
LH_ERR CanSendDebugControlMsgBio(uint8_t targetModule,uint8_t targetBoard,uint16_t debugCmd,uint8_t* paramBufferPtr,uint16_t paramLength);

//CAN向模块发送指令
LH_ERR CanSendDataToModule(uint16_t cmd, uint8_t targetModule,uint8_t targetBoard,uint8_t* paramBufferPtr,uint16_t paramLength);

#endif





