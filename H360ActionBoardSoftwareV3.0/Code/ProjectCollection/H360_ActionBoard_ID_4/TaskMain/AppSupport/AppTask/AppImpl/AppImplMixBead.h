/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:57:39
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-11 19:35:04
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppTask\AppImpl\AppImplMixBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_MIX_BEAD_H_
#define __APP_IMPL_MIX_BEAD_H_
#include "AppImplMixBeadVersion0.h"
#include "AppImplMixBeadVersion1.h"
//磁珠混匀模块复位,带零位偏移
LH_ERR AppImplMixBeadReset(void);

//磁珠混匀升起来
LH_ERR AppImplMixBeadUp(MIX_MODE mixMode);

//磁珠混匀升降降下去
LH_ERR AppImplMixBeadDown(void);

//磁珠混匀旋转混匀一次,不带自动升降
LH_ERR AppImplMixBeadStartRotate(MIX_MODE mixMode);

//磁珠混匀集成混匀一次
LH_ERR AppImplMixBeadRotateIntegrated(MIX_MODE mixMode);

//指定步进电机复位
LH_ERR AppImplMixBeadStepMotorReset(INDEX_MOTOR_MIX_BEAD index);

//指定步进电机走指定步数
LH_ERR AppImplMixBeadStepMotorRunSteps(INDEX_MOTOR_MIX_BEAD index,int32_t steps,int32_t* posAfterRun);





#endif




