/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-30 14:09:07
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-05 21:59:01
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_BASE_H_
#define __CAN_BASE_H_
#include "CanBaseData.h"
#include "CanBaseUtil.h"
#include "CoreUtil.h"

//回调函数原型,用于接收完成事件处理
typedef void(*CanRecvCompleteEventProcFuncPtr)(CAN_RECV_PACK* completePackPtr);

//回调函数原型,用于接收错误的事件处理
typedef void(*CanRecvErrorEventProcFuncPtr)(CAN_RECV_CACHE* recvCache,uint32_t canId,uint8_t* dataBuf,uint8_t datLength);

extern CAN_ID_SECTION canIdFastSection;
//初始化协议栈
void CanProtocolInit(CanRecvCompleteEventProcFuncPtr recvCompleteProc,CanRecvErrorEventProcFuncPtr errOccorProcFuncPtr);

//接收一个单元的数据
void CanRecvPackProc(uint32_t canId,uint8_t* dataBuf,uint8_t datLength);

//发送一帧CAN数据,需要详细设置,不需要ACK确认
LH_ERR CanSendPackDetailWithoutAck(CAN_CMD_SEND* cmdPack);

//发送一帧CAN数据,采用默认设置,不需要ACK确认
LH_ERR CanSendPackDefaultWithoutAck(uint16_t cmd,uint8_t targetModuleNo,uint8_t targetBoardAddr,uint8_t* dataBuf,uint16_t dataLength);

//发送一帧CAN数据,需要详细设置,需要ACK确认
LH_ERR CanSendPackDetailWithAck(CAN_CMD_SEND* cmdPack);

//发送一帧CAN数据,采用默认设置,需要ACK确认
LH_ERR CanSendPackDefaultWithAck(uint16_t cmd,uint8_t targetModuleNo,uint8_t targetBoardAddr,uint8_t* dataBuf,uint16_t dataLength);

//更新信号灯,当接收到ACK之后更新
void CanReflushCommandAck(CAN_ACK_STATE ackState,uint8_t srcModuleNo,uint8_t srcBoardAddr);



#endif



