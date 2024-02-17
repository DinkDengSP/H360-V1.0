#ifndef __CAN2_SUB_RAIL_COMPLEX_H_
#define __CAN2_SUB_RAIL_COMPLEX_H_
#include "Can2SubRailBase.h"

//轨道整体复位
LH_ERR Can2SubRailComplexResetAllWhileReturn(void);

//轨道设置模块总数
LH_ERR Can2SubRailComplexSetModuleCountWhileReturn(uint8_t moduleCount);

//轨道推进一次直到触发
LH_ERR Can2SubRailComplexPushMultiWhileTrigWhileReturn(uint8_t retryMax,SENSOR_STATE_PUSHIN_EXIST* rackPushIn1ExistStatePtr,SENSOR_STATE_PUSHIN_EXIST* rackPushIn2ExistStatePtr);











#endif



