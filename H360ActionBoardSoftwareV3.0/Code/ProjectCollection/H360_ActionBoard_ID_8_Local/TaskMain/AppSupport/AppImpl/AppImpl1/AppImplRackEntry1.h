/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:14:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-12-13 10:28:26
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_ENTRY1_H_
#define __APP_IMPL_RACK_ENTRY1_H_
#include "AppImplRackEntry1Config.h"

//初始化功能状态机
void AppImplRackEntry1FSM_Init(void);

//运行功能状态机
void AppImplRackEntry1FSM_Run(void);

//读取指定输入状态
LH_ERR AppImplRackEntry1ReadInput(RACK_ENTRY1_INPUT inputIndex,SENSOR_STATE* sensorStatePtr);

//设置指定输出状态
LH_ERR AppImplRackEntry1WriteOutput(RACK_ENTRY1_OUTPUT outputIndex,OUT_STATE setState);

//指定步进电机复位
LH_ERR AppImplRackEntry1StepMotorReset(RACK_ENTRY1_SM smIndex,int32_t* currentPos);

//指定步进电机走位
LH_ERR AppImplRackEntry1StepMotorRunSteps(RACK_ENTRY1_SM smIndex,int32_t runSteps,int32_t* currentPos);

//节点复位
LH_ERR AppImplRackEntry1ModuleReset(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr);

//节点卡住试管架
LH_ERR AppImplRackEntry1SwitchStateBlock(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr);

//节点释放试管架
LH_ERR AppImplRackEntry1SwitchStateRelease(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr);

//节点放行
LH_ERR AppImplRackEntry1ReleaseRack(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr);

//轨道启动
LH_ERR AppImplRackEntry1RailStart(uint8_t railInDir,uint8_t railOutDir,APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr);

//轨道停止
LH_ERR AppImplRackEntry1RailStop(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr);

//获取节点全部状态
LH_ERR AppImplRackEntry1ReadState(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr);

//清除节点记录状态
LH_ERR AppImplRackEntry1ClearState(void);



#endif
