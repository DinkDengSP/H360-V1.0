/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-04 15:02:10
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-05 13:09:40
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __TCP_BASE_PROTOCOL_H_
#define __TCP_BASE_PROTOCOL_H_
#include "TcpBaseUtil.h"
#include "ErrorCode.h"

//TCP协议层数据发送回调函数,创建好的数据通过这个API发送出去
typedef LH_ERR (*TcpSendDataFunc)(TCP_SEND_CMD_DATA* sendCmdDataPtr);
//协议层错误输出
typedef void (*TcpLogOutFunc)(const char *format, ...);

//网口发送指令回调方法注册
void TcpBaseRegisterFunc(TcpSendDataFunc sendFunc,TcpLogOutFunc logoutFunc);

//基础API,网口按照协议发送一帧数据出去
LH_ERR TcpBaseSendCommandWithData(uint16_t sendCommand,uint8_t* sendDataPtr,uint16_t sendDataLength);

//回传ACK
LH_ERR TcpBaseSendAck(TCP_NET_ACK ackFlag);

//回传系统状态
LH_ERR TcpBaseSendModuleState(SYSTEM_MODULE_STATE* module1State,SYSTEM_MODULE_STATE* module2State,SYSTEM_MODULE_STATE* module3State,
                                    SYSTEM_MODULE_STATE* module4State,SYSTEM_MODULE_STATE* module5State);

//回传联机结果
LH_ERR TcpBaseSendOnLineResult(TCP_CMD_RESULT result);

//回传系统各个模块类型,脱机指令回传,返回脱机是否成功
LH_ERR TcpBaseSendOffLineResult(TCP_CMD_RESULT result);

//回传测试参数确认
LH_ERR TcpBaseSendTestParamConfirm(void);

//回传Boot版本号
LH_ERR TcpBaseSendBootSoftwareVersion(uint8_t moduleNo,uint8_t boardNo,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion);

//回传App版本号
LH_ERR TcpBaseSendAppSoftwareVersion(uint8_t moduleNo,uint8_t boardNo,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion);

//回传升级准备结果
LH_ERR TcpBaseSendIapParpareResult(uint8_t moduleNo,uint8_t boardNo,TCP_CMD_RESULT result);

//回传升级开始结果
LH_ERR TcpBaseSendIapStartResult(uint8_t moduleNo,uint8_t boardNo,TCP_CMD_RESULT result);

//回传升级数据结果
LH_ERR TcpBaseSendIapDataResult(uint8_t moduleNo,uint8_t boardNo,TCP_CMD_RESULT result);

//回传升级结束结果
LH_ERR TcpBaseSendIapEndResult(uint8_t moduleNo,uint8_t boardNo,TCP_CMD_RESULT result);

//向上位机发送指令
LH_ERR TcpNetSendDataToPc(uint16_t cmd,uint8_t *u8Body,uint16_t bodyLen);

//回传程序跳转完成
LH_ERR TcpBaseSendProgramJumpResult(uint8_t moduleNo,uint8_t boardNo,TCP_CMD_RESULT result,uint8_t state);

#endif




