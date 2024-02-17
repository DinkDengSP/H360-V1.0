/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:08:19
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:42:39
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplPlateReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PLATE_REAGENT_H_
#define __SERVICE_IMPL_PLATE_REAGENT_H_
#include "ServiceImplPlateReagentConfig.h"

//自动运行状态机初始化
LH_ERR ServiceImplPlateReagent_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplPlateReagent_StateMachineLoop(void);

//试剂盘复位
LH_ERR ServiceImplPlateReagent_ResetAll(uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,CAN1_REPORT_ERR_CODE* commErrorCode,
                                                CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂盘运行指定个杯位
LH_ERR ServiceImplPlateReagent_RunAnyCups(uint8_t cupsCount,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,CAN1_REPORT_ERR_CODE* commErrorCode,
                                                CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂盘运行到指定杯位带偏移
LH_ERR ServiceImplPlateReagent_Run2IndexWithOffset(uint8_t targetIndex,PLATE_REAGENT_OFFSET offset,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,
                                                    CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂盘运行到指定杯位的扫码位并扫码
LH_ERR ServiceImplPlateReagent_Run2IndexScanAndQRScan(uint8_t targetIndex,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,uint8_t** barCodeBufferPtr,
                                                        uint16_t* barCodeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂盘扫码一次
LH_ERR ServiceImplPlateReagent_BarScanOnce(uint8_t** barCodeBufferPtr,uint16_t* barCodeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂盘清除探液数据
LH_ERR ServiceImplPlateReagent_ClearLiquidDetectData(uint8_t targetIndex,uint8_t clearConfig,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//设置盘设置状态空闲
LH_ERR ServiceImplPlateReagent_SetIdle(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplPlateReagent_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplPlateReagent_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplPlateReagent_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplPlateReagent_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);




#endif


