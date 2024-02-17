/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:34:05
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:52:43
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplMixWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_MIX_WASH_VERSION_1_H_
#define __APP_IMPL_MIX_WASH_VERSION_1_H_
#include "AppImplMixWashConfigVersion1.h"

//清洗混匀模块复位,带零位偏移
LH_ERR AppImplMixWashResetVersion1(void);

//清洗混匀升起来
LH_ERR AppImplMixWashUpVersion1(MIX_MODE mixMode);

//清洗混匀升降降下去
LH_ERR AppImplMixWashDownVersion1(void);

//清洗混匀旋转混匀一次,不带自动升降
LH_ERR AppImplMixWashStartRotateVersion1(MIX_MODE mixMode);

//清洗混匀集成混匀一次
LH_ERR AppImplMixWashRotateIntegratedVersion1(MIX_MODE mixMode);

//指定步进电机复位
LH_ERR AppImplMixWashStepMotorResetVersion1(INDEX_MOTOR_MIX_WASH index);

//指定步进电机走指定步数
LH_ERR AppImplMixWashStepMotorRunStepsVersion1(INDEX_MOTOR_MIX_WASH index,int32_t steps,int32_t* posAfterRun);



#endif




