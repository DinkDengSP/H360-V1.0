/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:50:50
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 10:36:44
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_ENTRY1_H_
#define __APP_IMPL_TUBE_ENTRY1_H_
#include "AppImplTubeEntryConfig1.h"

//初始化输入检测状态机
void AppImplTubeEntryFSM_Init1(void);

//运行输入检测状态机
void AppImplTubeEntryFSM_Run1(void);

//电机复位
LH_ERR AppImplTubeEntryStepMotorReset1(RAIL_ONLINE_TUBE_ENTRY1_SM_INDEX motorIndex,int32_t *posAfterResetPtr);

//电机走步数
LH_ERR AppImplTubeEntryStepMotorRunSteps1(RAIL_ONLINE_TUBE_ENTRY1_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr);

//输入节点复位
LH_ERR AppImplTubeEntryReset1(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr);

//轨道启动运转
LH_ERR AppImplTubeEntryRailStart1(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr);

//轨道停止运转
LH_ERR AppImplTubeEntryRailStop1(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr);

//读传感器节点信息
LH_ERR AppImplTubeEntryReadState1(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr);

//清除节点状态
LH_ERR AppImplTubeEntryClearState1(void);

//读取转盘入口信息,是否存在底座
LH_ERR AppImplTubeEntryReadRotateInput1(APP_STATE_TUBE_ROTATE** tubeRotateStatePtrPtr);


#endif

