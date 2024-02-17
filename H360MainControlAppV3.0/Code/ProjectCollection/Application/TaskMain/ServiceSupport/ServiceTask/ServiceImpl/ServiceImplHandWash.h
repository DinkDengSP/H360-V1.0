/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:03:55
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:31:55
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplHandWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_HAND_WASH_H_
#define __SERVICE_IMPL_HAND_WASH_H_
#include "ServiceImplHandWashConfig.h"

//自动运行状态机初始化
LH_ERR ServiceImplHandWash_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplHandWash_StateMachineLoop(void);

//清洗机械手复位
LH_ERR ServiceImplHandWash_ResetAll(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗机械手旋转到清洗盘
LH_ERR ServiceImplHandWash_RotatePlateWash(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗机械手旋转到反应盘
LH_ERR ServiceImplHandWash_RotatePlateReaction(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗机械手旋转到测量模块
LH_ERR ServiceImplHandWash_RotateMeasureModule(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗机械手从反应盘抓杯
LH_ERR ServiceImplHandWash_CatchPlateReaction(SENSOR_STATE* plateReactionCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗机械手往反应盘放杯
LH_ERR ServiceImplHandWash_PutPlateReaction(SENSOR_STATE* plateReactionCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗机械手从清洗盘抓杯
LH_ERR ServiceImplHandWash_CatchPlateWash(SENSOR_STATE* plateWashCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗机械手往清洗盘放杯
LH_ERR ServiceImplHandWash_PutPlateWash(SENSOR_STATE* plateWashCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗机械手穷测量模块抓杯
LH_ERR ServiceImplHandWash_CatchMeasureModule(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗机械手往测量模块放杯
LH_ERR ServiceImplHandWash_PutMeasureModule(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplHandWash_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplHandWash_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplHandWash_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplHandWash_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);




#endif


