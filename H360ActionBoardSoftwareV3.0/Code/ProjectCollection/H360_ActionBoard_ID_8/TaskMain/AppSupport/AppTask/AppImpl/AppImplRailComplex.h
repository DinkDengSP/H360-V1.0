/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-02 16:17:49
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplRailComplex.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RAIL_COMPLEX_H_
#define __APP_IMPL_RAIL_COMPLEX_H_
#include "AppImplBase.h"

//轨道整体复位
LH_ERR AppImplRailComplexResetAll(void);

//设置轨道模块数量
LH_ERR AppImplRailComplexSetModuleCount(uint8_t moduleCount);

//双轨道同时进架多次直到触发
LH_ERR  AppImplRailComplexPushInTrigMultiTogether(uint8_t retryMax,SENSOR_STATE_PUSHIN_EXIST* pushin1SensorPtr,SENSOR_STATE_PUSHIN_EXIST* pushin2SensorPtr);



#endif

