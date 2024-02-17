/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 14:12:06
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceImpl\ServiceImplInput.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_INPUT_H_
#define __SERVICE_IMPL_INPUT_H_
#include "CommonServiceImplBase.h"

//读取板上指定IO口讯息
BitAction ServiceImplInputReadValue(BOARD_IN_PIN inPin);

//板上所有电机急停
LH_ERR  ServiceImplInputStopEmergencyWithCanMsg(void* canMsgCachePtr);

//板上所有电机急停
LH_ERR  ServiceImplInputStopEmergencyWithSelfMsg(void);




#endif

