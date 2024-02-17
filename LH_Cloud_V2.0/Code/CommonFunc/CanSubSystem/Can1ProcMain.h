/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-21 08:55:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 16:15:11
 *FilePath: \Appd:\SVN_Local\02-Development\Software\H360_STM32\LH_RAIL_V2.3\Code\CommonFunc\Can1Protocol\Can1ProcMain.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN1_PROC_MAIN_H_
#define __CAN1_PROC_MAIN_H_
#include "Can1ProcUtil.h"

//CAN1解析使用CCM内存

//CAN1数据初始化
void Can1ProcDataInit(void);

//CAN1接收处理一个数据单元
void Can1RecvProc(CanRxMsg* rxMsg);

//CAN1接收完成的数据的转发
void Can1DispatchReceive(CAN1_DISPATCH_UNIT* unitPtr,uint16_t unitElementCount);

//CAN1发送指令的数据单元的初始化
void Can1SendCommandDataStructInit(CAN1_CMD_SEND* can1CmdPtr);

//发送数据包
void Can1SendCommandPack(CAN1_CMD_SEND* can1CmdPtr);

//发送数据包,在接收到的数据包的基础上
void Can1SendCommandPackWithRecvCmd(CAN1_CMD_RECV* can1CommandPtr,uint16_t commandValue,uint16_t dataBufferLength,uint8_t* dataBufferPtr);

//发生错误报警
void Can1ReportErrorCode(CAN1_REPORT_ERR_LEVEL level,CAN1_REPORT_ERR_CODE errCode,LH_ERR systemError);

//回传调试讯息的结果讯息
void Can1SendCommandResponseMsg(uint16_t cmdValue,CAN1_REPORT_ERR_CODE midError,CAN1_REPORT_ERR_LEVEL level,LH_ERR systemError,uint16_t paramLength,uint8_t* paramPtr);

//在指令响应中回传结果讯息
void Can1SendCommandResult(CAN1_CMD_RECV* can1CommandPtr,CAN1_REPORT_ERR_CODE reportCode,CAN1_REPORT_ERR_LEVEL reportLevel,LH_ERR errorParam,uint16_t paramLength,uint8_t* paramPtr);

//在指令响应中回传指定长度数据,该API会将参数长度也上传
void Can1SendCommandResultWithLength(CAN1_CMD_RECV* can1CommandPtr,CAN1_REPORT_ERR_CODE reportCode,CAN1_REPORT_ERR_LEVEL reportLevel,LH_ERR errorParam,uint16_t paramLength,uint8_t* paramPtr);

//在指令响应中回传系统版本
void Can1ProtocolSendAppVersionWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion);

//在指令响应中回传Boot版本
void Can1ProtocolSendBootVersionWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion);

//在指令响应中回传起始信号是否成功,成功为1 失败为0
void Can1ProtocolSendIapStartResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result);

//在指令响应中回传发送的数据是否成功,成功为1 失败为0
void Can1ProtocolSendIapDataTransResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result);

//在指令响应中回传发送的升级完成是否成功,成功为1 失败为0
void Can1ProtocolSendIapEndResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result);

//发送已经准备好开始升级 1为准备好 0为未准备好
void Can1ProtocolSendSoftwareUpdatePrepareWithSelf(uint8_t boardID,uint8_t state);

//在指令响应中回传程序跳转结果
void Can1ProtocolSendProgramJumpResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result,uint8_t state);

//在非指令响应中发送程序跳转结果
void Can1ProtocolSendProgramJumpWithSelf(uint8_t boardID,uint8_t result,uint8_t state);

#endif





