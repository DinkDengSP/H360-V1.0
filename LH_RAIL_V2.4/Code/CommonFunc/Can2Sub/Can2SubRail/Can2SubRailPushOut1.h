#ifndef __CAN2_SUB_RAIL_PUSHOUT1_H_
#define __CAN2_SUB_RAIL_PUSHOUT1_H_
#include "Can2SubRailBase.h"

//出样1复位
LH_ERR Can2SubRailPushOut1ResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetFlag);

//出样1推进一次并回零
LH_ERR Can2SubRailPushOut1PushOnceWithReturnZeroWhileReturn(void);

//出样1推进一次不回零
LH_ERR Can2SubRailPushOut1PushOnceNotReturnZeroWhileReturn(uint8_t* pushOutIndexPtr);

//出样1回零
LH_ERR Can2SubRailPushOut1ReturnZeroWhileReturn(void);

//出样1获取当前序号
LH_ERR Can2SubRailPushOut1GetIndexWhileReturn(uint8_t* pushOutIndexPtr);

//出样1推进一次不回零,按键专用
LH_ERR Can2SubRailPushOut1ButtonPushOnceNotReturnZeroWhileReturn(uint8_t* pushOutIndexPtr);















#endif





