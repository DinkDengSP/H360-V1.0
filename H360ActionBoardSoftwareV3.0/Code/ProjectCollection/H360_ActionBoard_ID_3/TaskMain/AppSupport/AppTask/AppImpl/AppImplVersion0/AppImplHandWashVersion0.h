/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:39:37
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-24 20:02:17
 *FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppTask\AppImpl\AppImplHandWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_HAND_WASH_VERSION0_H_
#define __APP_IMPL_HAND_WASH_VERSION0_H_
#include "AppImplHandWashConfigVersion0.h"

//清洗机械手复位
LH_ERR AppImplHandWashResetVersion0(void);

//清洗机械手旋转到反应盘
LH_ERR AppImplHandWashRotateToPlateReactionVersion0(void);

//清洗机械手旋转到清洗盘
LH_ERR AppImplHandWashRotateToPlateWashVersion0(void);

//清洗机械手旋转到测量模块
LH_ERR AppImplHandWashRotateToMeasureUnitVersion0(void);

//清洗机械手反应盘取杯
LH_ERR AppImplHandWashCatchCupOnPlateReactionVersion0(SENSOR_STATE* cupExistPlateReactionOld);

//清洗机械手反应盘抛杯
LH_ERR AppImplHandWashPutCupOnPlateReactionVersion0(SENSOR_STATE* cupExistPlateReactionOld);

//清洗机械手清洗盘取杯
LH_ERR AppImplHandWashCatchCupOnPlateWashVersion0(SENSOR_STATE* cupExistPlateWashNew);

//清洗机械手清洗盘抛杯
LH_ERR AppImplHandWashPutCupOnPlateWashVersion0(SENSOR_STATE* cupExistPlateWashNew);

//清洗机械手测量模块取杯
LH_ERR AppImplHandWashCatchCupOnMeasureUnitVersion0(void);

//清洗机械手测量模块放杯
LH_ERR AppImplHandWashPutCupOnMeasureUnitVersion0(void);

//指定电机复位
LH_ERR AppImplHandWashMotorResetVersion0(INDEX_MOTOR_HAND_WASH index);

//指定电机走相应的步数
LH_ERR AppImplHandWashMotorRunStepsVersion0(INDEX_MOTOR_HAND_WASH index,int32_t steps,int32_t* posAfterRun);

#endif

