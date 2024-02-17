/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-18 18:45:56
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-18 18:47:39
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_CAN_MSG_CLOUD_H_
#define __APP_IMPL_CAN_MSG_CLOUD_H_
#include "AppImplBase.h"
#include "AppImplCanMsgComm.h"

//立禾云CAN消息处理
void AppImplCanMsgCloudProcess(uint8_t srcModule,uint8_t srcBoard,uint16_t cmd,uint8_t* dataBuf,uint16_t dataLength);


#endif



