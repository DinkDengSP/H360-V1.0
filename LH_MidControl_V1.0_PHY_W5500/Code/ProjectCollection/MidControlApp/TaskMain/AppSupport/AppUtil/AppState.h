/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-10 11:17:39
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-20 11:12:23
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_STATE_H_
#define __APP_STATE_H_
#include "AppStateDefault.h"

//初始化系统状态
void AppStateInit(APP_STATE* appStatePtr);

//获取指定模块的模块状态指针
SYSTEM_MODULE_STATE* AppStateGetModuleStatePtr(STATE_MODULE_NO moduleNo);

//获取指定模块上一次的接收到心跳的时间
uint32_t AppStateGetLastRecvDeviceModuleTime(STATE_MODULE_NO moduleNo);

//设置指定模块接收到心跳的时间为当前时间
uint32_t AppStateSetLastRecvDeviceModuleTime(STATE_MODULE_NO moduleNo);

STATE_CMP_RESULT AppStateCompareModuleState(SYSTEM_MODULE_STATE* moduleState1,SYSTEM_MODULE_STATE* moduleState2);

//将主流程状态设置为默认值
void AppStateSetTestFlowWorkingStateDefault(APP_STATE* pstStatus);

//获取主流程状态指针
APP_STATE_TEST_FLOW* AppStateGetTestFlowWorkingStatePtr(void);

//更新主流程工作状态
extern void AppStateUpdateTestFlowWorkingState(APP_STATE* pstStatus, uint8_t u8ModuleNo, APP_MODULE_STATE wokingState);


#endif



