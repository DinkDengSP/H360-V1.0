/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-11 12:44:13
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanProtocol\CanMasterMain.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_MASTER_MAIN_H_
#define __CAN_MASTER_MAIN_H_
#include "CanMasterDataTypeConfig.h"
#include "CanMasterUtil.h"


//CAN Master接收协议初始化
void CanMasterProcDataInit(void);

//CAN Master接收处理一个数据单元
void CanMasterRecvProc(CAN_RECV_FAST_ID* fastCanIDPtr,CanRxMsg* rxMsg);

//CAN Master发送指令,直到收到ACK,注意,此处因为是异步的,所以会有一个数据等待释放的锁
//也就是说,,调用了这个函数,必须调用wairreturn
LH_ERR CanMasterSendCommandWhileAck(CAN_MASTER_SEND_CMD* sendCommand);

//CAN Master等待指定板卡的指定通道的数据返回
LH_ERR CanMasterSendCommandWaitReturn(uint8_t boardID,CAN_MASTER_CHANNEL channelNo,uint32_t timeOutMs,CAN_MASTER_READ_DATA* readData);

//CAN Master发送指令,直到收到Return返回
LH_ERR CanMasterSendCommandWhileReturn(CAN_MASTER_SEND_CMD* sendCommand,CAN_MASTER_READ_DATA* readData);

#endif




