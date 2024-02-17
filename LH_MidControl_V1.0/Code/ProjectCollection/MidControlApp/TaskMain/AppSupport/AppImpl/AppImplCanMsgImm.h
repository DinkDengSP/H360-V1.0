/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-18 18:46:03
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-18 18:47:46
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_CAN_MSG_IMM_H_
#define __APP_IMPL_CAN_MSG_IMM_H_
#include "AppImplBase.h"
#include "AppImplCanMsgComm.h"

//化学发光消息处理
void AppImplCanMsgImmProcess(uint8_t srcModule,uint8_t srcBoard,CAN_CMD cmd,uint8_t* dataBuf,uint16_t dataLength, APP_STATE* pstStatus);


#endif



