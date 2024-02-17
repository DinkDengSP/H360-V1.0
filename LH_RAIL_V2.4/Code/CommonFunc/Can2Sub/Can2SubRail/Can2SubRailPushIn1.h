#ifndef __CAN2_SUB_RAIL_PUSHIN1_H_
#define __CAN2_SUB_RAIL_PUSHIN1_H_
#include "Can2SubRailBase.h"

//进样1复位
LH_ERR Can2SubRailPushIn1ResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetFlag);

//进样1推进一次
LH_ERR Can2SubRailPushIn1PushOnceWhileReturn(SENSOR_STATE_PUSHIN_EXIST* rackExistStatePtr);

//进样1推进一次直到触发
LH_ERR Can2SubRailPushIn1PushMultiWhileTrigWhileReturn(uint16_t retryMax,SENSOR_STATE_PUSHIN_EXIST* rackExistStatePtr);



#endif





