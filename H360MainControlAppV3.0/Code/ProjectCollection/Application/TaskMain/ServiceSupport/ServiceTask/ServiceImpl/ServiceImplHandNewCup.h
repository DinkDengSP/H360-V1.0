/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:02:54
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:31:47
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplHandNewCup.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_HAND_NEW_CUP_H_
#define __SERVICE_IMPL_HAND_NEW_CUP_H_
#include "ServiceImplHandNewCupConfig.h"

//自动运行状态机初始化
LH_ERR ServiceImplHandNewCup_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplHandNewCup_StateMachineLoop(void);

//新杯机械手复位
LH_ERR ServiceImplHandNewCup_ResetAll(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新杯机械手移动到新杯盘
LH_ERR ServiceImplHandNewCup_Move2NewStack(uint8_t leftRight,uint8_t backFront,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新杯机械手移动到反应盘
LH_ERR ServiceImplHandNewCup_Move2PlateReaction(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新杯机械手移动到垃圾桶
LH_ERR ServiceImplHandNewCup_Move2Garbage(GARBAGE_SELECT garbageNo,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新杯机械手从新杯盘取杯
LH_ERR ServiceImplHandNewCup_CatchCupFromNewStack(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新杯机械手从反应盘取杯
LH_ERR ServiceImplHandNewCup_CatchCupFromPlateReaction(SENSOR_STATE* newCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新杯机械手往反应盘放杯
LH_ERR ServiceImplHandNewCup_PutCupToPlateReaction(SENSOR_STATE* newCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新杯机械手往垃圾桶放杯
LH_ERR ServiceImplHandNewCup_PutCupToGarbage(GARBAGE_SELECT garbageNo,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplHandNewCup_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplHandNewCup_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplHandNewCup_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplHandNewCup_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);




#endif


