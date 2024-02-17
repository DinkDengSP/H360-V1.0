/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-16 17:15:48
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:25:51
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_MIX_WASH_H_
#define __APP_IMPL_MIX_WASH_H_
#include "AppImplMixWashVersion0.h"
#include "AppImplMixWashVersion1.h"

//清洗混匀模块复位,带零位偏移
LH_ERR AppImplMixWashReset(void);

//清洗混匀升起来
LH_ERR AppImplMixWashUp(MIX_MODE mixMode);

//清洗混匀升降降下去
LH_ERR AppImplMixWashDown(void);

//清洗混匀旋转混匀一次,不带自动升降
LH_ERR AppImplMixWashStartRotate(MIX_MODE mixMode);

//清洗混匀集成混匀一次
LH_ERR AppImplMixWashRotateIntegrated(MIX_MODE mixMode);

//指定步进电机复位
LH_ERR AppImplMixWashStepMotorReset(INDEX_MOTOR_MIX_WASH index);

//指定步进电机走指定步数
LH_ERR AppImplMixWashStepMotorRunSteps(INDEX_MOTOR_MIX_WASH index,int32_t steps,int32_t* posAfterRun);


#endif

