/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:14:51
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-12-21 09:19:51
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_STORE_IN_H_
#define __APP_IMPL_RACK_STORE_IN_H_
#include "AppImplRackStoreInConfig.h"

//初始化功能状态机
void AppImplRackStoreInFSM_Init(void);

//运行功能状态机
void AppImplRackStoreInFSM_Run(void);

//读取指定输入状态
static LH_ERR AppImplRackStoreInReadInput(RACK_STORE_IN_INPUT inputIndex,SENSOR_STATE* sensorStatePtr);

//写入指定输出状态
LH_ERR AppImplRackStoteInWriteOutput(RACK_STORE_IN_OUTPUT outputIndex,OUT_STATE setState);

//指定步进电机复位
LH_ERR AppImplRackStoreInStepMotorReset(RACK_STORE_IN_SM smIndex,int32_t* currentPos);

//指定步进电机走位
LH_ERR AppImplRackStoreInStepMotorRunSteps(RACK_STORE_IN_SM smIndex,int32_t runSteps,int32_t* currentPos);

//模块复位
LH_ERR AppImplRackStoreInModuleReset(APP_STATE_RACK_STORE_IN** rackStoreInStateSetPtrPtr);

//回零
LH_ERR AppImplRackStoreInModuleReturnZero(APP_STATE_RACK_STORE_IN** rackStoreInStateSetPtrPtr);

//模块推进一次,到进样仓尾端
LH_ERR AppImplRackStoreInModulPushRackToEnd(APP_STATE_RACK_STORE_IN** rackStoreInStateSetPtrPtr);

//模块推进一次,到入口
LH_ERR AppImplRackStoreInModulPushRackToEntry(APP_STATE_RACK_STORE_IN** rackStoreInStateSetPtrPtr);

//读取传感器
LH_ERR AppImplRackStoreInModuleReadState(APP_STATE_RACK_STORE_IN** rackStoreInStateSetPtrPtr);

//清除传感器与事件
LH_ERR AppImplRackStoreInModuleClearState(void);

#endif
