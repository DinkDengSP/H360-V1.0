/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:34:28
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:55:13
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PLATE_WASH_VERSION_1_H_
#define __APP_IMPL_PLATE_WASH_VERSION_1_H_
#include "AppImplPlateWashConfigVersion1.h"


//清洗盘旋转复位,带零位修正
LH_ERR AppImplPlateWashResetVersion1(SENSOR_STATE* cupExistState,uint8_t* cupIndex);

//清洗盘移动指定个杯位
LH_ERR AppImplPlateWashRunAnyCupsVersion1(uint8_t cupsCount,SENSOR_STATE* cupExistState,uint8_t* cupIndex);

//清洗盘移动到指定序号杯位
LH_ERR AppImplPlateWashRun2CupWithIndexVersion1(uint8_t targetIndex,SENSOR_STATE* cupExistState,uint8_t* cupIndex);

//指定步进电机复位,复位带零位修正
LH_ERR AppImplPlateWashStepMotorResetVersion1(INDEX_MOTOR_PLATE_WASH index);

//指定步进电机走指定步数
LH_ERR AppImplPlateWashStepMotorRunStepsVersion1(INDEX_MOTOR_PLATE_WASH index,int32_t steps,int32_t* posAfterRun);




#endif




