/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 13:53:06
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 13:23:03
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplAssist.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_ASSIST_H_
#define __SERVICE_IMPL_ASSIST_H_
#include "ServiceImplAssistUtilActionParam.h"

//自动运行状态机初始化
LH_ERR ServiceImplAssist_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplAssist_StateMachineLoop(void);

//电机复位
LH_ERR ServiceImplAssist_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//电机走步数
LH_ERR ServiceImplAssist_MotorRunSteps(uint8_t motorIndex,int32_t steps,uint8_t curveSelect,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//输入读取
LH_ERR ServiceImplAssist_InputRead(uint16_t inputIndex,BitAction* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//输出写入
LH_ERR ServiceImplAssist_OutputWrite(uint16_t outputIndex,BitAction setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取软件版本
LH_ERR ServiceImplAssist_ReadSoftwareVersion(VERSION_BOARD_ID boardID,uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取动作参数
LH_ERR ServiceImplAssist_ReadActionParam(uint8_t mainIndex,uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入动作参数
LH_ERR ServiceImplAssist_WriteActionParam(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取系统状态
LH_ERR ServiceImplAssist_ReadSystemState(uint8_t mainIndex,uint8_t subIndex,int32_t* stateRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入系统状态
LH_ERR ServiceImplAssist_WriteSystemState(uint8_t mainIndex,uint8_t subIndex,int32_t stateWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取电机参数
LH_ERR ServiceImplAssist_ReadMotorParam(uint8_t mainIndex,uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入电机参数
LH_ERR ServiceImplAssist_WriteMotorParam(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//握手
LH_ERR ServiceImplAssist_HandShake(MODE_RUNNING modeRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//进入IAP模式
LH_ERR ServiceImplAssist_EnterIapMode(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//传输IAP数据
LH_ERR ServiceImplAssist_TransIapData(uint32_t startAddr,uint16_t dataLength,uint8_t* dataBuffer,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//开始IAP升级
LH_ERR ServiceImplAssist_IapUpdate(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//查看IAP结果
LH_ERR ServiceImplAssist_CheckIapResult(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//系统紧急急停
LH_ERR ServiceImplAssist_SystemEmergencyStop(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//设备信息写入与读取
LH_ERR ServiceImplAssist_MachineMsgReadWrite(uint8_t *msgBuf,uint8_t readWriteFlag,uint16_t readWriteOffset,uint16_t bufLength, CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

LH_ERR ServiceImplAssist_SetActionBoardModelVersion(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);
#endif


