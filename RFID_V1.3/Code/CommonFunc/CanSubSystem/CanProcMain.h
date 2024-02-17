/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-21 08:55:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-04 16:48:07
 *FilePath: \Appd:\SVN_Local\02-Development\Software\H360_STM32\LH_RAIL_V2.3\Code\CommonFunc\CanProtocol\CanProcMain.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_PROC_MAIN_H_
#define __CAN_PROC_MAIN_H_
#include "CanProcUtil.h"

//CAN数据初始化
void CanProcDataInit(void);

//CAN接收处理一个数据单元
void CanRecvProc(uint8_t selfBoardID,CanRxMsg* rxMsg);

//转发数据
void CanDispatchReceive(CAN_DISPATCH_UNIT* unitPtr,uint16_t unitElementCount);

//CAN发送指令的数据单元的初始化
void CanSendCommandDataStructInit(uint8_t selfBoardID,CAN_CMD_SEND* canCmdPtr);

//发送数据包
void CanSendCommandPack(CAN_CMD_SEND* canCmdPtr);

//发送数据包,在接收到的数据包的基础上
void CanSendCommandPackWithRecvCmd(CAN_CMD_RECV* canCommandPtr,uint16_t commandValue,uint16_t dataBufferLength,uint8_t* dataBufferPtr);

//发生错误报警
void CanReportErrorCode(uint8_t selfBoardID,CAN_REPORT_ERR_LEVEL level,CAN_REPORT_ERR_CODE errCode,LH_ERR systemError);

//回传调试讯息的结果讯息
void CanSendDebugResponseMsg(uint8_t selfBoardID,uint16_t cmdValue,CAN_REPORT_ERR_CODE midError,CAN_REPORT_ERR_LEVEL level,LH_ERR systemError,uint16_t paramLength,uint8_t* paramPtr);

//在指令响应中回传系统版本
void CanRFID_SendAppVersionWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion);

//在指令响应中回传Boot版本
void CanRFID_SendBootVersionWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion);

//在指令响应中回传起始信号是否成功,成功为1 失败为0
void CanRFID_SendIapStartResultWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t result);

//在指令响应中回传发送的数据是否成功,成功为1 失败为0
void CanRFID_SendIapDataTransResultWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t result);

//在指令响应中回传发送的升级完成是否成功,成功为1 失败为0
void CanRFID_SendIapEndResultWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t result);

//发送已经准备好开始升级 1为准备好 0为未准备好
void CanRFID_SendSoftwareUpdatePrepareWithSelf(uint8_t boardID,uint8_t state);

//发送RFID信息
void CanRFID_SendCardMsg(uint8_t selfBoardID,uint8_t* cardMsg,uint16_t length);



#endif





