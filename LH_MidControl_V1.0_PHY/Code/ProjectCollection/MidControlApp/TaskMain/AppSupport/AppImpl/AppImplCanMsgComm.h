/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-18 18:44:38
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-26 18:04:08
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_CAN_MSG_COMM_H_
#define __APP_IMPL_CAN_MSG_COMM_H_
#include "AppImplBase.h"

//can层通用信息处理
//准备升级
void AppImplCanMsgCommIapParpare(uint8_t* dataBuf,uint16_t dataLength);

//升级开始
void AppImplCanMsgCommIapStart(uint8_t* dataBuf,uint16_t dataLength);

//升级数据传输
void AppImplCanMsgCommIapData(uint8_t* dataBuf,uint16_t dataLength);

//升级结束
void AppImplCanMsgCommIapEnd(uint8_t* dataBuf,uint16_t dataLength);

//读取Boot版本号
void AppImplCanMsgCommIapReadBootVersion(uint8_t moduleNo,uint8_t* dataBuf,uint16_t dataLength);

//读取App版本号
void AppImplCanMsgCommIapReadAppVersion(uint8_t moduleNo,uint8_t* dataBuf,uint16_t dataLength);

//显示设备转发来的日志信息
void AppImplCanMsgCommShowLog(uint8_t* logPrefix,uint8_t* logDataBuffer);

//处理调试指令返回讯息
void AppImplCanMsgCommDebugResult(uint8_t* dataBuf,uint16_t dataLength);

//处理报警讯息
void AppImplCanMsgCommWarningMsg(uint8_t moduleNo,uint8_t* dataBuf,uint16_t dataLength);

#endif




