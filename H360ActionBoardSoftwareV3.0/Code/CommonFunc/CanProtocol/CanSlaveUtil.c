#include "CanSlaveUtil.h"

//创建一个ACK的协议ID
uint32_t CanSlaveCreateAckPackID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode)
{
    uint32_t resultID = 0;
    resultID += frameCode;
    resultID<<= 2;
    resultID += CAN_BASE_SINGLE_CODE_SHORT;
    resultID <<= 1;
    resultID += CAN_BASE_PACK_LEN_SHORT;
    resultID <<= 3;
    resultID += CAN_BASE_PACK_ACK;
    resultID <<= 7;
    resultID += channel;
    resultID <<= 6;
    resultID += selfAddr;
    resultID <<= 6;
    resultID += targetAddr;
    return resultID;
}

//创建一个结果帧的协议ID
uint32_t CanSlaveCreateResultPackID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode)
{
    uint32_t resultID = 0;
    resultID += frameCode;
    resultID<<= 2;
    resultID += CAN_BASE_SINGLE_CODE_SHORT;
    resultID <<= 1;
    resultID += CAN_BASE_PACK_LEN_SHORT;
    resultID <<= 3;
    resultID += CAN_BASE_PACK_RESULT;
    resultID <<= 7;
    resultID += channel;
    resultID <<= 6;
    resultID += selfAddr;
    resultID <<= 6;
    resultID += targetAddr;
    return resultID;
}

//创建一个短包数据协议ID
uint32_t CanSlaveCreateShortDataPackID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode)
{
    uint32_t resultID = 0;
    resultID += frameCode;
    resultID<<= 2;
    resultID += CAN_BASE_SINGLE_CODE_SHORT;
    resultID <<= 1;
    resultID += CAN_BASE_PACK_LEN_SHORT;
    resultID <<= 3;
    resultID += CAN_BASE_PACK_DATA;
    resultID <<= 7;
    resultID += channel;
    resultID <<= 6;
    resultID += selfAddr;
    resultID <<= 6;
    resultID += targetAddr;
    return resultID;
}

//创建一个数据长包 起始帧ID
uint32_t CanSlaveCreateLongDataPackStartID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode)
{
    uint32_t resultID = 0;
    resultID += frameCode;
    resultID<<= 2;
    resultID += CAN_BASE_SINGLE_CODE_LONG_START;
    resultID <<= 1;
    resultID += CAN_BASE_PACK_LEN_LONG;
    resultID <<= 3;
    resultID += CAN_BASE_PACK_DATA;
    resultID <<= 7;
    resultID += channel;
    resultID <<= 6;
    resultID += selfAddr;
    resultID <<= 6;
    resultID += targetAddr;
    return resultID;
}

//创建一个数据长包 过程帧ID
uint32_t CanSlaveCreateLongDataPackProcedureID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode)
{
    uint32_t resultID = 0;
    resultID += frameCode;
    resultID<<= 2;
    resultID += CAN_BASE_SINGLE_CODE_LONG_DATA;
    resultID <<= 1;
    resultID += CAN_BASE_PACK_LEN_LONG;
    resultID <<= 3;
    resultID += CAN_BASE_PACK_DATA;
    resultID <<= 7;
    resultID += channel;
    resultID <<= 6;
    resultID += selfAddr;
    resultID <<= 6;
    resultID += targetAddr;
    return resultID;
}

//创建一个数据长包 结束帧ID
uint32_t CanSlaveCreateLongDataPackEndID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode)
{
    uint32_t resultID = 0;
    resultID += frameCode;
    resultID<<= 2;
    resultID += CAN_BASE_SINGLE_CODE_LONG_END;
    resultID <<= 1;
    resultID += CAN_BASE_PACK_LEN_LONG;
    resultID <<= 3;
    resultID += CAN_BASE_PACK_DATA;
    resultID <<= 7;
    resultID += channel;
    resultID <<= 6;
    resultID += selfAddr;
    resultID <<= 6;
    resultID += targetAddr;
    return resultID;
}
