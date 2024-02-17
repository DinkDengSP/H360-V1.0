/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:33:29
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 16:29:10
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplBufferInject.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_BUFFER_INJECT_VERSION_0_H_
#define __APP_IMPL_BUFFER_INJECT_VERSION_0_H_
#include "AppImplBufferInjectConfigVersion0.h"

//缓冲液泵阀复位
LH_ERR AppImplBufferSolutionInjectResetVersion0(void);

//缓冲液灌注
LH_ERR AppImplBufferSolutionInjectPrimeVersion0(BOTTLE_BUFFER_SOLUTION bottltSelect);

//缓冲液注液
LH_ERR AppImplBufferSolutionInjectInjectVersion0(BOTTLE_BUFFER_SOLUTION bottltSelect);

//缓冲液清洗维护
LH_ERR AppImplBufferSolutionInjectMaintainPrimeVersion0(BOTTLE_BUFFER_SOLUTION bottltSelect);

//指定步进电机复位
LH_ERR AppImplBufferSolutionInjectStepMotorResetVersion0(INDEX_MOTOR_BUFFER_SOLUTION index);

//指定步进电机走指定步数
LH_ERR AppImplBufferSolutionInjectStepMotorRunStepsVersion0(INDEX_MOTOR_BUFFER_SOLUTION index,int32_t steps,int32_t* posAfterRun);

#endif




