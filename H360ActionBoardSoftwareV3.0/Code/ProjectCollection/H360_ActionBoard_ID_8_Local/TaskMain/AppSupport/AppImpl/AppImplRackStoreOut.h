/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:14:58
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-12-28 11:41:14
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_STORE_OUT_H_
#define __APP_IMPL_RACK_STORE_OUT_H_
#include "AppImplRackStoreOutConfig.h"

//初始化功能状态机
void AppImplRackStoreOutFSM_Init(void);

//运行功能状态机
void AppImplRackStoreOutFSM_Run(void);

//读取指定输入状态
LH_ERR AppImplRackStoreOutReadInput(RACK_STORE_OUT_INPUT inputIndex,SENSOR_STATE* sensorStatePtr);

//写入指定输出状态
LH_ERR AppImplRackStoreOutWriteOutput(RACK_STORE_OUT_OUTPUT outputIndex,OUT_STATE setState);

//指定步进电机复位
LH_ERR AppImplRackStoreOutStepMotorReset(RACK_STORE_OUT_SM smIndex,int32_t* currentPos);

//指定步进电机走位
LH_ERR AppImplRackStoreOutStepMotorRunSteps(RACK_STORE_OUT_SM smIndex,int32_t runSteps,int32_t* currentPos);

//出样仓复位
LH_ERR AppImplRackStoreOutModuleReset(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr);

//出样仓回零
LH_ERR AppImplRackStoreOutReturnZero(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr);

//出样旋转到工作位置
LH_ERR AppImplRackStoreOutRotateSwitchToAction(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr);

//出样旋转到空闲位置
LH_ERR AppImplRackStoreOutRotateSwitchToIdle(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr);

//出样仓运行到推进起点
LH_ERR AppImplRackStoreOutRunToPrepare(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr);

//出样仓推进一次
LH_ERR AppImplRackStoreOutPushOnce(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr);

//出样仓回退一次
LH_ERR AppImplRackStoreOutBackOnce(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr);

//将试管架从出样换轨出口推到出样仓入口
LH_ERR AppImplRackStoreOutOutSwitchToRackStore(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr,APP_STATE_RACK_OUT_SWITCH** rackOutSwitchStatePtrPtr);

//获取出样仓缓存讯息
LH_ERR AppImplRackStoreOutReadState(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr,APP_STATE_RACK_OUT_SWITCH** rackOutSwitchStatePtrPtr);

//清除出样仓缓存讯息
LH_ERR AppImplRackStoreOutClearState(void);

//获取最大推进次数
LH_ERR AppImplRackStoreOutGetPushMax(uint8_t* pushCountMaxTaryNearKey,uint8_t* pushCountMaxTaryNearRail);

#endif
