/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-21 08:55:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 10:20:27
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
void Can1RecvProc(MCU_CAN_RECV_DAT* rxMsg);

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
void Can1SendDebugResponseMsg(uint16_t cmdValue,CAN1_REPORT_ERR_CODE midError,CAN1_REPORT_ERR_LEVEL level,LH_ERR systemError,uint16_t paramLength,uint8_t* paramPtr);

//向中位机发送从测试区取架完成
void Can1RailSendCatchTestingRackStateOK(CAN1_CMD_RECV* can1CommandPtr,uint8_t moduleID);

//在非指令响应中发送从测试区取架完成
void Can1RailSendCatchTestingRackStateOK_Self(uint8_t moduleID);

//在指令响应中发送机械手状态忙
void Can1RailSendHandStateBusyWithCommand(CAN1_CMD_RECV* can1CommandPtr);

//在非指令响应中发送机械手状态忙
void Can1RailSendHandStateBusyWithSelf(void);

//在指令响应中发送机械手状态空闲
void Can1RailSendHandStateIdleWithCommand(CAN1_CMD_RECV* can1CommandPtr);

//在非指令响应中发送机械手状态空闲
void Can1RailSendHandStateIdleWithSelf(void);

//在指令响应中发送样本架到位
void Can1RailSendRackAtPositionWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint16_t rackID,uint8_t moduleNo,uint8_t regionPos,uint8_t subRegion);

//在指令响应中回传系统版本
void Can1RailSendAppVersionWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion);

//在指令响应中回传Boot版本
void Can1RailSendBootVersionWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion);

//在指令响应中回传起始信号是否成功,成功为1 失败为0
void Can1RailSendIapStartResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result);

//在指令响应中回传发送的数据是否成功,成功为1 失败为0
void Can1RailSendIapDataTransResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result);

//在指令响应中回传发送的升级完成是否成功,成功为1 失败为0
void Can1RailSendIapEndResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result);

//在指令响应中回传发送的程序跳转结果
void Can1RailSendIapProgramJumpResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result,uint8_t state);

//在非指令响应中发送程序跳转结果
void Can1RailSendProgramJumpResult_Self(uint8_t result,uint8_t state);

//在非指令响应中发送样本架到位
void Can1RailSendRackAtPositionWithSelf(uint16_t rackID,uint8_t moduleNo,uint8_t regionPos,uint8_t subRegion);

//在非指令响应中发送进样1状态
void Can1RailSendRackRackIn1StateWithSelf(uint8_t state);

//在非指令响应中发送进样2状态
void Can1RailSendRackRackIn2StateWithSelf(uint8_t state);

//在非指令响应中发送出样1状态
void Can1RailSendRackRackOut1StateWithSelf(uint8_t state);

//在非指令响应中发送出样2状态
void Can1RailSendRackRackOut2StateWithSelf(uint8_t state);

//发送已经准备好开始升级 1为准备好 0为未准备好
void Can1RailSendSoftwareUpdatePrepareWithSelf(uint8_t boardID,uint8_t state);

//发送急诊试管架存在
void Can1RailSendEmergencyRackExist(void);

//发送系统状态
void Can1RailSendSendSystemState(uint8_t state);



#endif





