/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:08:26
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:42:46
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplPlateWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PLATE_WASH_H_
#define __SERVICE_IMPL_PLATE_WASH_H_
#include "ServiceImplPlateWashConfig.h"

//自动运行状态机初始化
LH_ERR ServiceImplPlateWash_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplPlateWash_StateMachineLoop(void);

//清洗盘复位
LH_ERR ServiceImplPlateWash_ResetAll(uint8_t* plateWashIndex,SENSOR_STATE* cupPlateWashExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗盘移动指定数量个杯位
LH_ERR ServiceImplPlateWash_MoveAnyCups(uint8_t cups,uint8_t* plateWashIndex,SENSOR_STATE* cupPlateWashExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//清洗盘移动到指定序号的位置
LH_ERR ServiceImplPlateWash_Move2Index(uint8_t targetCupIndex,uint8_t* plateWashIndex,SENSOR_STATE* cupPlateWashExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplPlateWash_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplPlateWash_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplPlateWash_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplPlateWash_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);




#endif


