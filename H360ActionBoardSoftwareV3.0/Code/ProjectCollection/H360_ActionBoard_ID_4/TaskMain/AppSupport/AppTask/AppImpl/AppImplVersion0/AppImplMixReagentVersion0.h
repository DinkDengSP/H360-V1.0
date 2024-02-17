/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:57:52
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-20 11:48:12
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppTask\AppImpl\AppImplMixReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_MIX_REAGENT_VERSION0_H_
#define __APP_IMPL_MIX_REAGENT_VERSION0_H_
#include "AppImplMixReagentConfigVersion0.h"

//试剂混匀模块复位,带零位偏移
LH_ERR AppImplMixReagentResetVersion0(void);

//试剂混匀升起来
LH_ERR AppImplMixReagentUpVersion0(MIX_MODE mixMode);

//试剂混匀升降降下去
LH_ERR AppImplMixReagentDownVersion0(void);

//试剂混匀旋转混匀一次,不带自动升降
LH_ERR AppImplMixReagentStartRotateVersion0(MIX_MODE mixMode);

//试剂混匀集成混匀一次
LH_ERR AppImplMixReagentRotateIntegratedVersion0(MIX_MODE mixMode);

//指定步进电机复位
LH_ERR AppImplMixReagentStepMotorResetVersion0(INDEX_MOTOR_MIX_REAGENT index);

//指定步进电机走指定步数
LH_ERR AppImplMixReagentStepMotorRunStepsVersion0(INDEX_MOTOR_MIX_REAGENT index,int32_t steps,int32_t* posAfterRun);



#endif


