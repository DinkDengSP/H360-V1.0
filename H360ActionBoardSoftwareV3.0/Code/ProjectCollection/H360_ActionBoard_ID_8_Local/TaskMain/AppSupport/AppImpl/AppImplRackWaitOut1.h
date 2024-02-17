/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:15:10
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-01 09:29:07
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_WAIT_OUT1_H_
#define __APP_IMPL_RACK_WAIT_OUT1_H_
#include "AppImplRackWaitOut1Config.h"

//初始化功能状态机
void AppImplRackWaitOut1FSM_Init(void);

//运行功能状态机
void AppImplRackWaitOut1FSM_Run(void);

//读取指定输入状态
LH_ERR AppImplRackWaitOut1ReadInput(RACK_WAIT_OUT1_INPUT inputIndex,SENSOR_STATE* sensorStatePtr);

//设置指定输出状态
LH_ERR AppImplRackWaitOut1WriteOutput(RACK_WAIT_OUT1_OUTPUT outputIndex,OUT_STATE setState);

//指定步进电机复位
LH_ERR AppImplRackWaitOut1StepMotorReset(RACK_WAIT_OUT1_SM smIndex,int32_t* currentPos);

//指定步进电机走位
LH_ERR AppImplRackWaitOut1StepMotorRunSteps(RACK_WAIT_OUT1_SM smIndex,int32_t runSteps,int32_t* currentPos);

//模块节点复位
LH_ERR AppImplRackWaitOut1ModuleReset(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr);

//切换到卡住状态
LH_ERR AppImplRackWaitOut1SwitchStateBrake(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr);

//切换到释放状态
LH_ERR AppImplRackWaitOut1SwitchStateRelease(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr);

//执行一次释放
LH_ERR AppImplRackWaitOut1ReleaseRackOnce(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr);

//获取节点缓存讯息
LH_ERR AppImplRackWaitOut1ReadState(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr);

//清除节点缓存讯息
LH_ERR AppImplRackWaitOut1ClearState(void);



#endif
