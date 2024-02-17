#ifndef __CAN2_SUB_RAIL_PUSHIN2_H_
#define __CAN2_SUB_RAIL_PUSHIN2_H_
#include "Can2SubRailBase.h"

//进样2复位
LH_ERR Can2SubRailPushIn2ResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetFlag);

//进样2推进一次
LH_ERR Can2SubRailPushIn2PushOnceWhileReturn(SENSOR_STATE_PUSHIN_EXIST* rackExistStatePtr);

//进样2推进一次直到触发
LH_ERR Can2SubRailPushIn2PushMultiWhileTrigWhileReturn(uint16_t retryMax,SENSOR_STATE_PUSHIN_EXIST* rackExistStatePtr);










#endif





