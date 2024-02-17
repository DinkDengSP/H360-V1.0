#ifndef __CAN2_SUB_RAIL_PUSHOUT2_H_
#define __CAN2_SUB_RAIL_PUSHOUT2_H_
#include "Can2SubRailBase.h"

//出样2复位
LH_ERR Can2SubRailPushOut2ResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetFlag);

//出样2推进一次并回零
LH_ERR Can2SubRailPushOut2PushOnceWithReturnZeroWhileReturn(void);

//出样2推进一次不回零
LH_ERR Can2SubRailPushOut2PushOnceNotReturnZeroWhileReturn(uint8_t* pushOutIndexPtr);

//出样2回零
LH_ERR Can2SubRailPushOut2ReturnZeroWhileReturn(void);

//出样2获取当前序号
LH_ERR Can2SubRailPushOut2GetIndexWhileReturn(uint8_t* pushOutIndexPtr);

//出样2推进一次不回零,按键专用
LH_ERR Can2SubRailPushOut2ButtonPushOnceNotReturnZeroWhileReturn(uint8_t* pushOutIndexPtr);














#endif



