/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:08:12
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:42:31
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplPlateReaction.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PLATE_REACTION_H_
#define __SERVICE_IMPL_PLATE_REACTION_H_
#include "ServiceImplPlateReactionConfig.h"

//自动运行状态机初始化
LH_ERR ServiceImplPlateReaction_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplPlateReaction_StateMachineLoop(void);

//反应盘复位
LH_ERR ServiceImplPlateReaction_ResetAll(uint8_t* plateReactionIndex,SENSOR_STATE* newCupExist,SENSOR_STATE* washCupPlateReactionExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//反应盘正向走指定数量个杯位
LH_ERR ServiceImplPlateReaction_RunAnyCupsPositive(uint8_t cupsCount,uint8_t* plateReactionIndex,SENSOR_STATE* newCupExist,SENSOR_STATE* washCupPlateReactionExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//反应盘正向走到指定杯位
LH_ERR ServiceImplPlateReaction_Run2IndexPositive(uint8_t cupIndex,uint8_t* plateReactionIndex,SENSOR_STATE* newCupExist,SENSOR_STATE* washCupPlateReactionExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//反应盘反向走指定数量个杯位
LH_ERR ServiceImplPlateReaction_RunAnyCupsNegative(uint8_t cupsCount,uint8_t* plateReactionIndex,SENSOR_STATE* newCupExist,SENSOR_STATE* washCupPlateReactionExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//反应盘反向走到指定杯位
LH_ERR ServiceImplPlateReaction_Run2IndexNegative(uint8_t cupIndex,uint8_t* plateReactionIndex,SENSOR_STATE* newCupExist,SENSOR_STATE* washCupPlateReactionExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplPlateReaction_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplPlateReaction_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplPlateReaction_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplPlateReaction_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);





#endif


