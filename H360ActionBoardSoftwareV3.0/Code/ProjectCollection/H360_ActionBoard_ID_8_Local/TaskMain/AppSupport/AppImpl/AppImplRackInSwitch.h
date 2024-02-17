/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:14:27
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-12 16:15:59
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_IN_SWITCH_H_
#define __APP_IMPL_RACK_IN_SWITCH_H_
#include "AppImplRackInSwitchConfig.h"

//初始化功能状态机
void AppImplRackInSwitchFSM_Init(void);

//运行功能状态机
void AppImplRackInSwitchFSM_Run(void);

//读取指定输入状态
LH_ERR AppImplRackInSwitchReadInput(RACK_IN_SWITCH_INPUT inputIndex,SENSOR_STATE* sensorStatePtr);

//写入指定输出状态
LH_ERR AppImplRackInSwitchWriteOutput(RACK_IN_SWITCH_OUTPUT outputIndex,OUT_STATE setState);

//指定步进电机复位
LH_ERR AppImplRackInSwitchStepMotorReset(RACK_IN_SWITCH_SM smIndex,int32_t* currentPos);

//指定步进电机走位
LH_ERR AppImplRackInSwitchStepMotorRunSteps(RACK_IN_SWITCH_SM smIndex,int32_t runSteps,int32_t* currentPos);

//节点复位
LH_ERR AppImplRackInSwitchModuleReset(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr);

//节点切换到试管架进入位置
LH_ERR AppImplRackInSwitchToRackComeInPos(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr);

//节点切换到试管架离开位置
LH_ERR AppImplRackInSwitchToRackMoveOutPos(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr);

//节点执行一次试管架换轨
LH_ERR AppImplRackInSwitchTrigOnce(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr);

//获取节点全部讯息
LH_ERR AppImplRackInSwitchReadState(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr);

//清除节点全部讯息
LH_ERR AppImplRackInSwitchClearState(void);






#endif
