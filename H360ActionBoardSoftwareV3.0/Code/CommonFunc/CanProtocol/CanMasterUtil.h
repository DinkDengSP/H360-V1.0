#ifndef __CAN_MASTER_UTIL_H_
#define __CAN_MASTER_UTIL_H_
#include "CanBaseCommon.h"

//创建一个Master ID
uint32_t CanMasterUtilCreatePackID(uint8_t targetBoardAddr,uint8_t channelNo,uint8_t frameCode,
                        CAN_BASE_PACK_TYPE packType,CAN_BASE_PACK_LEN_FLAG lenFlag,CAN_BASE_SINGLE_CODE singleCode);

#endif





