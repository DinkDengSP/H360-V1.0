/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-11 12:43:46
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanProtocol\CanSlaveMain.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SLAVE_MAIN_H_
#define __CAN_SLAVE_MAIN_H_
#include "CanSlaveDataTypeConfig.h"
#include "CanSlaveUtil.h"

//CAN总线接收缓存初始化
void CanSlaveInit(CAN_SLAVE_DISPATCH_CONFIG* dispatchConfigPtr);

//接收数据转发到指定信道处理
void CanSlaveDispatchData2SpecialTask(void);

//发送ACK
void CanSlaveSendAckWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,CAN_BASE_ACK ackCode);

//发送结果代码
void CanSlaveSendResultWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,LH_ERR errCode);

//发送数据代码
void CanSlaveSendDataPackWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,CAN_DATA_FLAG dataFlag,uint8_t* bufferPtr,uint16_t bufferLength);

//CAN从机处理数据单元
void CanSlaveProcRecvData(CAN_RECV_FAST_ID* fastCanIDPtr,CanRxMsg* rxMsg);


#endif






