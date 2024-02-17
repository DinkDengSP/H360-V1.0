/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:07:01
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-24 09:04:45
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplLiquidAuto.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_LIQUID_AUTO_H_
#define __SERVICE_IMPL_LIQUID_AUTO_H_
#include "ServiceImplLiquidAutoUtil.h"

//自动运行状态机初始化
LH_ERR ServiceImplLiquidAuto_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplLiquidAuto_StateMachineLoop(void);

//强制执行一次抽废液
LH_ERR ServiceImplLiquidAuto_EnforceWasteDischargeOnce(SENSOR_STATE* fullSensorState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);


//强制执行一次三针清洗稀释
LH_ERR ServiceImplLiquidAuto_EnforceThreeNeedleCleanDiluentOnce(SENSOR_STATE* bottleStateA,SENSOR_STATE* bottleStateB,
                                                                    SENSOR_STATE* bottleDiluentTransferFull,SENSOR_STATE* bottleDiluentTransferEmpty,
                                                                    SENSOR_STATE* bottleDiluentTargetFull,SENSOR_STATE* bottleDiluentTargetEmpty,
                                                                    CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);


//强制执行一次纯水补水
LH_ERR ServiceImplLiquidAuto_EnforcePureWaterSupplementOnce(SENSOR_STATE* lowLiquidState,SENSOR_STATE* fullLiquidState,
                                                                CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//使能自动抽废液
LH_ERR ServiceImplLiquidAuto_EnableWasteDischargeAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//使能自动三针清洗液稀释
LH_ERR ServiceImplLiquidAuto_EnableThreeNeedleCleanDiluentAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//使能自动存水补充
LH_ERR ServiceImplLiquidAuto_EnablePureWaterSupplementAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//禁止自动抽废液
LH_ERR ServiceImplLiquidAuto_DisableWasteDischargeAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//禁止自动三针清洗液稀释
LH_ERR ServiceImplLiquidAuto_DisableThreeNeedleCleanDiluentAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//禁止存水自动补充
LH_ERR ServiceImplLiquidAuto_DisablePureWaterSupplementAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//设定三针自动稀释浓缩液瓶号
LH_ERR ServiceImplLiquidAuto_SetThreeNeedleConcentrateBottleNumber(CONCENTRATE_BOTTLE_SELECT concentrateBottleNumber,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplLiquidAuto_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplLiquidAuto_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplLiquidAuto_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplLiquidAuto_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//配置瓶注一次纯水
LH_ERR ServiceImplLiquidAuto_ConfigAddPureWaterOnce(uint32_t pulseMaxCount,uint32_t *sensorPulseCount,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//配置瓶注一次浓缩液
LH_ERR ServiceImplLiquidAuto_ConfigAddConcentrateOnce(uint8_t concentrateBottle,uint16_t injectMaxTime,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//配置瓶向工作瓶抽一次
LH_ERR ServiceImplLiquidAuto_ConfigToTargetOnce(CONFIG_TO_TARGET_TRIG_FLAG *trigFlag,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);
#endif


