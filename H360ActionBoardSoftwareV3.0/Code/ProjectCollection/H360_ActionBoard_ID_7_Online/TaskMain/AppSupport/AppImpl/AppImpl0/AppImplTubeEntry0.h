/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-12-20 09:10:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-12-20 09:32:04 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_TUBE_ENTRY0_H_
#define __APP_IMPL_TUBE_ENTRY0_H_
#include "AppImplTubeEntryConfig0.h"

//初始化输入检测状态机
void AppImplTubeEntryFSM_Init0(void);

//运行输入检测状态机
void AppImplTubeEntryFSM_Run0(void);

//电机复位
LH_ERR AppImplTubeEntryStepMotorReset0(RAIL_ONLINE_TUBE_ENTRY0_SM_INDEX motorIndex,int32_t *posAfterResetPtr);

//电机走步数
LH_ERR AppImplTubeEntryStepMotorRunSteps0(RAIL_ONLINE_TUBE_ENTRY0_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr);

//输入节点复位
LH_ERR AppImplTubeEntryReset0(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr);

//轨道启动运转
LH_ERR AppImplTubeEntryRailStart0(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr);

//轨道停止运转
LH_ERR AppImplTubeEntryRailStop0(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr);

//读传感器节点信息
LH_ERR AppImplTubeEntryReadState0(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr);

//清除节点状态
LH_ERR AppImplTubeEntryClearState0(void);

//读取转盘入口信息,是否存在底座
LH_ERR AppImplTubeEntryReadRotateInput0(APP_STATE_TUBE_ROTATE** tubeRotateStatePtrPtr);


#endif