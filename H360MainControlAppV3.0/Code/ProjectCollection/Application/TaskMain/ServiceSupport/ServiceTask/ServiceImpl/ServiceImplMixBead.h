/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:07:17
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:32:19
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplMixBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_MIX_BEAD_H_
#define __SERVICE_IMPL_MIX_BEAD_H_
#include "ServiceImplMixBeadConfig.h"

//自动运行状态机初始化
LH_ERR ServiceImplMixBead_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplMixBead_StateMachineLoop(void);

//磁珠摇匀复位
LH_ERR ServiceImplMixBead_ResetAll(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠摇匀升起来
LH_ERR ServiceImplMixBead_Up(MIX_MODE mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠摇匀降下去
LH_ERR ServiceImplMixBead_Down(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠摇匀开始摇匀
LH_ERR ServiceImplMixBead_StartRotate(MIX_MODE mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠摇匀集成摇匀
LH_ERR ServiceImplMixBead_RotateIntegrate(MIX_MODE mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplMixBead_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplMixBead_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplMixBead_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplMixBead_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);




#endif


