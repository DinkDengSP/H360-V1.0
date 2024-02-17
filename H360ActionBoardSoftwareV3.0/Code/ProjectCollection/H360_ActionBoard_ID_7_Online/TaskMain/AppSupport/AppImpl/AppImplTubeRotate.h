/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:51:20
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 14:02:23
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_ROTATE_H_
#define __APP_IMPL_TUBE_ROTATE_H_
#include "AppImplTubeRotateConfig.h"

//初始化转盘状态机
void AppImplTubeRotateFSM_Init(void);

//运行转盘检测状态机
void AppImplTubeRotateFSM_Run(void);

//电机复位
LH_ERR AppImplTubeRotateStepMotorReset(RAIL_ONLINE_TUBE_ROTATE_SM_INDEX motorIndex,int32_t *posAfterResetPtr);

//电机走步数
LH_ERR AppImplTubeRotateStepMotorRunSteps(RAIL_ONLINE_TUBE_ROTATE_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr);

//转盘复位
LH_ERR AppImplTubeRotateReset(APP_STATE_TUBE_ROTATE** tubeRotateStatePtrPtr);

//转盘旋转一格
LH_ERR AppImplTubeRotateOnce(APP_STATE_TUBE_ROTATE** tubeRotateStatePtrPtr);

//读传感器事件信息
LH_ERR AppImplTubeRotateReadState(APP_STATE_TUBE_ROTATE** tubeRotateStatePtrPtr);

//清除事件信息
LH_ERR AppImplTubeRotateClearState(void);




#endif

