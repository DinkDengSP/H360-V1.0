/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:13:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-17 10:28:00
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PLATE_REAGENT_H_
#define __APP_IMPL_PLATE_REAGENT_H_
#include "AppImplPlateReagentVersion0.h"
#include "AppImplPlateReagentVersion1.h"

//试剂盘复位
LH_ERR AppImplPlateReagentReset(uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行到指定序号杯位,带偏移
LH_ERR AppImplPlateReagentRun2CupWithOffset(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行到指定序号杯位,带扫码偏移
LH_ERR AppImplPlateReagentRun2CupWithOffsetForScan(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行到指定序号杯位,带扫码偏移
LH_ERR AppImplPlateReagentRun2CupWithOffsetForRFID(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行指定数量个杯位,基于当前位置
LH_ERR AppImplPlateReagentRunAnyCups(uint8_t cups,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//指定步进电机复位
LH_ERR AppImplPlateReagentStepMotorReset(INDEX_MOTOR_PLATE_REAGENT index);

//指定步进电机走步数
LH_ERR AppImplPlateReagentStepMotorRunSteps(INDEX_MOTOR_PLATE_REAGENT index,int32_t steps,int32_t* posAfterRun);





#endif


