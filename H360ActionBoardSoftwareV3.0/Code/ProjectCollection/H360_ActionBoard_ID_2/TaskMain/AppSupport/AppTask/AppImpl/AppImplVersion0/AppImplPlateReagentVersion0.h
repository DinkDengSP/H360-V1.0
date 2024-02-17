/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:13:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-17 10:06:54
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PLATE_REAGENT_VERSION_0_H_
#define __APP_IMPL_PLATE_REAGENT_VERSION_0_H_
#include "AppImplPlateReagentConfigVersion0.h"


//试剂盘复位
LH_ERR AppImplPlateReagentResetVersion0(uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行到指定序号杯位,带偏移
LH_ERR AppImplPlateReagentRun2CupWithOffsetVersion0(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行到指定序号杯位,带扫码偏移
LH_ERR AppImplPlateReagentRun2CupWithOffsetForScanVersion0(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行到指定序号杯位,带RFID偏移
LH_ERR AppImplPlateReagentRun2CupWithOffsetForRFIDVersion0(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行指定数量个杯位,基于当前位置
LH_ERR AppImplPlateReagentRunAnyCupsVersion0(uint8_t cups,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//指定步进电机复位
LH_ERR AppImplPlateReagentStepMotorResetVersion0(INDEX_MOTOR_PLATE_REAGENT index);

//指定步进电机走步数
LH_ERR AppImplPlateReagentStepMotorRunStepsVersion0(INDEX_MOTOR_PLATE_REAGENT index,int32_t steps,int32_t* posAfterRun);





#endif


