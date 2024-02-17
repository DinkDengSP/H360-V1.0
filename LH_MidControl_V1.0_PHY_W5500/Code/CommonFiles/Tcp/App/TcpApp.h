/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-12 20:20:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2022-01-19 20:34:50
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __TCP_APP_H_
#define __TCP_APP_H_
#include "TcpBase.h"

//报警上传
LH_ERR TcpAppReportWarning(uint8_t moduleNo,uint8_t boardID,uint8_t errorLevel,uint16_t errorCode,uint32_t errorParam,uint8_t* paramBuffer,uint8_t paramLength);

//回传急停结果
LH_ERR TcpAppReportEmergencyStopResult(TCP_CMD_RESULT result);

//回传调试结果,一般指令
LH_ERR TcpAppSendDebugControlResultComm(uint8_t moduleNo,uint8_t boardID,uint16_t debugCmd,uint8_t errorLevel,uint32_t errorCode,uint32_t errorParam,uint8_t* paramBuffer,uint16_t paramLength);

//回传调试结果,生化指令
LH_ERR TcpAppSendDebugControlResultBio(uint8_t* dataBuffer,uint16_t dataLength);

//立禾云调试结果回传
LH_ERR TcpAppSendDebugControlResultCloud(uint8_t moduleNo,uint8_t boardID,uint16_t debugCmd,uint8_t errorLevel,uint16_t errorCode,uint32_t errorParam,uint8_t* paramBuffer,uint16_t paramLength);





#endif





