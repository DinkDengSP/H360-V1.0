/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:51:46
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 13:17:31
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_CLAMP_H_
#define __APP_IMPL_TUBE_CLAMP_H_
#include "AppImplTubeClampConfig.h"

//初始化试管机构状态机
void AppImplTubeClampFSM_Init(void);

//试管扶正机构运转
void AppImplTubeClampFSM_Run(void);

//电机复位
LH_ERR AppImplTubeClampStepMotorReset(RAIL_ONLINE_TUBE_CLAMP_SM_INDEX motorIndex,int32_t *posAfterResetPtr);

//电机走步数
LH_ERR AppImplTubeClampStepMotorRunSteps(RAIL_ONLINE_TUBE_CLAMP_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr);

//试管扶正机构复位
LH_ERR AppImplTubeClampReset(APP_STATE_TUBE_CLAMP** tubeClampStatePtrPtr);

//试管扶正机构抓住
LH_ERR AppImplTubeClampCatch(APP_STATE_TUBE_CLAMP** tubeClampStatePtrPtr);

//试管扶正机构释放
LH_ERR AppImplTubeClampRelease(APP_STATE_TUBE_CLAMP** tubeClampStatePtrPtr);

//读传感器事件状态
LH_ERR AppImplTubeClampReadState(APP_STATE_TUBE_CLAMP** tubeClampStatePtrPtr);

//读传感器事件状态
LH_ERR AppImplTubeClampClearState(void);


#endif



