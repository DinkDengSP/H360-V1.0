/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:14:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-12-01 09:27:33
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_OUT_SWITCH_H_
#define __APP_IMPL_RACK_OUT_SWITCH_H_
#include "AppImplRackOutSwitchConfig.h"


//初始化功能状态机
void AppImplRackOutSwitchFSM_Init(void);

//运行功能状态机
void AppImplRackOutSwitchFSM_Run(void);

//读取指定输入状态
LH_ERR AppImplRackOutSwitchReadInput(RACK_OUT_SWITCH_INPUT inputIndex,SENSOR_STATE* sensorStatePtr);

//写入指定输出状态
LH_ERR AppImplRackOutSwitchWriteOutput(RACK_OUT_SWITCH_OUTPUT outputIndex,OUT_STATE setState);

//指定步进电机复位
LH_ERR AppImplRackOutSwitchStepMotorReset(RACK_OUT_SWITCH_SM smIndex,int32_t* currentPos);

//指定步进电机走位
LH_ERR AppImplRackOutSwitchStepMotorRunSteps(RACK_OUT_SWITCH_SM smIndex,int32_t runSteps,int32_t* currentPos);

//节点复位
LH_ERR AppImplRackOutSwitchModuleReset(APP_STATE_RACK_OUT_SWITCH** rackInSwitchStatePtrPtr);

//节点切换到试管架进入位置
LH_ERR AppImplRackOutSwitchToRackComeInPos(APP_STATE_RACK_OUT_SWITCH** rackInSwitchStatePtrPtr);

//节点切换到试管架离开位置
LH_ERR AppImplRackOutSwitchToRackMoveOutPos(APP_STATE_RACK_OUT_SWITCH** rackInSwitchStatePtrPtr);

//节点执行一次试管架换轨
LH_ERR AppImplRackOutSwitchTrigOnce(APP_STATE_RACK_OUT_SWITCH** rackInSwitchStatePtrPtr);

//获取节点全部讯息
LH_ERR AppImplRackOutSwitchReadState(APP_STATE_RACK_OUT_SWITCH** rackInSwitchStatePtrPtr);

//清除节点全部讯息
LH_ERR AppImplRackOutSwitchClearState(void);




#endif
