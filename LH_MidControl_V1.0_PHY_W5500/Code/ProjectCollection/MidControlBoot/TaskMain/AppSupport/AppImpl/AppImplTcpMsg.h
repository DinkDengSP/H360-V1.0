/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-10 10:19:52
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-10 10:23:55
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TCP_MSG_H_
#define __APP_IMPL_TCP_MSG_H_
#include "BoardInc.h"
#include "SrvTaskConfig.h"

//读取Boot版本号
void AppImplTcpMsgDataProcessReadBootVersion(NET_COMMAND_PACK *netDataPactPtr);

//读取APP版本号
void AppImplTcpMsgDataProcessReadAppVersion(NET_COMMAND_PACK *netDataPactPtr);

//在线升级准备
void AppImplTcpMsgDataProcessIapPrepare(NET_COMMAND_PACK *netDataPactPtr);

//在线升级开始  
void AppImplTcpMsgDataProcessIapStart(NET_COMMAND_PACK *netDataPactPtr);

//在线升级数据传输       
void AppImplTcpMsgDataProcessIapData(NET_COMMAND_PACK *netDataPactPtr);

//在线升级结束      
void AppImplTcpMsgDataProcessIapEnd(NET_COMMAND_PACK *netDataPactPtr);

//联机
void AppImplTcpMsgDataProcessCmdOnLine(NET_COMMAND_PACK *netDataPactPtr);

//脱机
void AppImplTcpMsgDataProcessCmdOffLine(NET_COMMAND_PACK *netDataPactPtr);

//测试参数
void AppImplTcpMsgDataProcessCmdTestParam(NET_COMMAND_PACK *netDataPactPtr);

//调试指令响应
void AppImplTcpMsgDataProcessCmdDebug(NET_COMMAND_PACK *netDataPactPtr);


#endif




