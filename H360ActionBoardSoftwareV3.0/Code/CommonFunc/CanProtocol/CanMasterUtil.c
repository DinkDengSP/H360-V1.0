#include "CanMasterUtil.h"
#include "SelfBoardID.h"

//创建一个Master ID
uint32_t CanMasterUtilCreatePackID(uint8_t targetBoardAddr,uint8_t channelNo,uint8_t frameCode,
                        CAN_BASE_PACK_TYPE packType,CAN_BASE_PACK_LEN_FLAG lenFlag,CAN_BASE_SINGLE_CODE singleCode)
{
    uint32_t resultID = 0x00000000;
    resultID += (uint32_t)frameCode;
    resultID<<= 2;
    resultID += singleCode;
    resultID <<= 1;
    resultID += lenFlag;
    resultID <<= 3;
    resultID += packType;
    resultID <<= 7;
    resultID += channelNo;
    resultID <<= 6;
    resultID += SELF_BOARD_ID;
    resultID <<= 6;
    resultID += targetBoardAddr;
    return resultID;
}


