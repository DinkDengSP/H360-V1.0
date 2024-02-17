/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-27 17:53:31
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplPushIn1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PUSH_IN1_H_
#define __APP_IMPL_PUSH_IN1_H_
#include "AppImplPushIn1Config.h"

//进样1复位,带是否零位偏移
LH_ERR AppImplPushIn1Reset(RAIL_RESET_CORRECT_FLAG correctFlag);

//进样1推进一次
LH_ERR AppImplPushIn1PushOnce(SENSOR_STATE_PUSHIN_EXIST* rackExistSensorStatePtr);

//进样1推进多次直到传感器触发
LH_ERR AppImplPushIn1PushMultiWhileSensorTrig(uint16_t retryMax,SENSOR_STATE_PUSHIN_EXIST* rackExistSensorStatePtr);

#endif







