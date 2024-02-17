/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:15:04
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-12-07 10:26:05
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_TEST_H_
#define __APP_IMPL_RACK_TEST_H_
#include "AppImplRackTestConfig.h"

//初始化功能状态机
void AppImplRackTestFSM_Init(void);

//运行功能状态机
void AppImplRackTestFSM_Run(void);

//读取指定输入状态
LH_ERR AppImplRackTestReadInput(RACK_TEST_INPUT inputIndex,SENSOR_STATE* sensorStatePtr);

//设置指定输出状态
LH_ERR AppImplRackTestWriteOutput(RACK_TEST_OUTPUT outputIndex,OUT_STATE setState);

//指定步进电机复位
LH_ERR AppImplRackTestStepMotorReset(RACK_TEST_SM smIndex,int32_t* currentPos);

//指定步进电机走位
LH_ERR AppImplRackTestStepMotorRunSteps(RACK_TEST_SM smIndex,int32_t runSteps,int32_t* currentPos);

//节点复位
LH_ERR AppImplRackTestModuleReset(APP_STATE_RACK_TEST** rackTestStatePtrPtr);

//节点切换下一试管
LH_ERR AppImplRackTestSwitchNextTube(uint8_t nextTubeNo,APP_STATE_RACK_TEST** rackTestStatePtrPtr);

//节点切换到释放状态
LH_ERR AppImplRackTestSwitchSwitchRelease(APP_STATE_RACK_TEST** rackTestStatePtrPtr);

//获取全部的缓存信息
LH_ERR AppImplRackTestReadState(APP_STATE_RACK_TEST** rackTestStatePtrPtr);

//清除缓存讯息
LH_ERR AppImplRackTestClearState(void);



#endif
