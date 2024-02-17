/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:51:05
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 14:44:31
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_EXIT_H_
#define __APP_IMPL_TUBE_EXIT_H_
#include "AppImplTubeExitConfig.h"

//初始化输出检测状态机
void AppImplTubeExitFSM_Init(void);

//运行输出检测状态机
void AppImplTubeExitFSM_Run(void);

//电机复位
LH_ERR AppImplTubeExitStepMotorReset(RAIL_ONLINE_TUBE_EXIT_SM_INDEX motorIndex,int32_t *posAfterResetPtr);

//电机走步数
LH_ERR AppImplTubeExitStepMotorRunSteps(RAIL_ONLINE_TUBE_EXIT_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr);

//输出节点复位
LH_ERR AppImplTubeExitReset(APP_STATE_TUBE_EXIT** tubeExistStatePtrPtr);

//读传感器节点信息
LH_ERR AppImplTubeExitReadState(APP_STATE_TUBE_EXIT** tubeExistStatePtrPtr);

//清除输出节点状态
LH_ERR AppImplTubeExitClearState(void);



#endif

