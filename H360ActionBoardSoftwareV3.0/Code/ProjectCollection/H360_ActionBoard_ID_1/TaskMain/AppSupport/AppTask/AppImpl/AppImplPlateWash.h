/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-16 17:16:08
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 19:26:14
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PLATE_WASH_H_
#define __APP_IMPL_PLATE_WASH_H_
#include "AppImplPlateWashVersion0.h"
#include "AppImplPlateWashVersion1.h"


//清洗盘旋转复位,带零位修正
LH_ERR AppImplPlateWashReset(SENSOR_STATE* cupExistState,uint8_t* cupIndex);

//清洗盘移动指定个杯位
LH_ERR AppImplPlateWashRunAnyCups(uint8_t cupsCount,SENSOR_STATE* cupExistState,uint8_t* cupIndex);

//清洗盘移动到指定序号杯位
LH_ERR AppImplPlateWashRun2CupWithIndex(uint8_t targetIndex,SENSOR_STATE* cupExistState,uint8_t* cupIndex);

//指定步进电机复位,复位带零位修正
LH_ERR AppImplPlateWashStepMotorReset(INDEX_MOTOR_PLATE_WASH index);

//指定步进电机走指定步数
LH_ERR AppImplPlateWashStepMotorRunSteps(INDEX_MOTOR_PLATE_WASH index,int32_t steps,int32_t* posAfterRun);




#endif

