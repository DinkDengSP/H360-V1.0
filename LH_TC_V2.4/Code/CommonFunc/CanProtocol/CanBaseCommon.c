#include "CanBaseCommon.h"

//从CAN接收到的数据创建快速ID解析器
void CanBaseCreateFastID(uint32_t canID,CAN_RECV_FAST_ID* fastID)
{
    fastID->targetBoardAddr = (uint8_t)(canID & 0x003f);
    fastID->senderBoardAddr = (uint8_t)((canID >> 6) & 0x003f);
    fastID->channelNumber = (uint8_t)((canID >> 12) & 0x007f);
    fastID->packType = (CAN_BASE_PACK_TYPE)((canID >> 19) & 0x0007);
    fastID->packLenFlag = (CAN_BASE_PACK_LEN_FLAG)((canID >> 22) & 0x0001);
    fastID->singleCode = (CAN_BASE_SINGLE_CODE)((canID >> 23) & 0x0003);
    fastID->frameCode = (uint8_t)((canID >> 25) & 0x000f);
}

//创建一个运行时心跳包的ID
uint32_t CanBaseCreateHeartPackID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t frameCode)
{
    uint32_t resultID = 0;
    resultID += frameCode;
    resultID<<= 2;
    resultID += CAN_BASE_SINGLE_CODE_SHORT;
    resultID <<= 1;
    resultID += CAN_BASE_PACK_LEN_SHORT;
    resultID <<= 3;
    resultID += CAN_BASE_PACK_HEART;
    resultID <<= 7;
    resultID += 0;//通道号0
    resultID <<= 6;
    resultID += selfAddr;
    resultID <<= 6;
    resultID += targetAddr;
    return resultID;
}

//创建一个主动上报数据包的ID
uint32_t CanBaseCreateAutoUpdateID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t frameCode)
{
    uint32_t resultID = 0;
    resultID += frameCode;
    resultID<<= 2;
    resultID += CAN_BASE_SINGLE_CODE_SHORT;
    resultID <<= 1;
    resultID += CAN_BASE_PACK_LEN_SHORT;
    resultID <<= 3;
    resultID += CAN_BASE_PACK_AUTO_UPDATE;
    resultID <<= 7;
    resultID += 0;//通道号0
    resultID <<= 6;
    resultID += selfAddr;
    resultID <<= 6;
    resultID += targetAddr;
    return resultID;
}

//从数据缓存中读取一个协议的有符号32位数
int32_t CanBaseReadDataConvertToInt32(uint8_t* bufferPtr,uint16_t posOffset)
{
    uint32_t resultTemp = 0;
    int32_t resultData = 0;
    resultTemp += bufferPtr[posOffset + 0];
    resultTemp <<= 8;
    resultTemp += bufferPtr[posOffset + 1];
    resultTemp <<= 8;
    resultTemp += bufferPtr[posOffset + 2];
    resultTemp <<= 8;
    resultTemp += bufferPtr[posOffset + 3];

    resultData = *((int32_t*)(&resultTemp));

    return resultData;
}

//从数据缓存中读取一个协议的无符号32位数
uint32_t CanBaseReadDataConvertToUint32(uint8_t* bufferPtr,uint16_t posOffset)
{
    uint32_t resultData = 0;
    resultData += bufferPtr[posOffset + 0];
    resultData <<= 8;
    resultData += bufferPtr[posOffset + 1];
    resultData <<= 8;
    resultData += bufferPtr[posOffset + 2];
    resultData <<= 8;
    resultData += bufferPtr[posOffset + 3];

    return resultData;
}

//从数据缓存中读取一个协议的有符号16位数
int16_t CanBaseReadDataConvertToInt16(uint8_t* bufferPtr,uint16_t posOffset)
{
    uint16_t resultTemp = 0;
    int16_t resultData = 0;
    resultTemp += bufferPtr[posOffset + 0];
    resultTemp <<= 8;
    resultTemp += bufferPtr[posOffset + 1];

    resultData = *((int16_t*)(&resultTemp));
    return resultData;
}

//从数据缓存中读取一个协议的无符号16位数
uint16_t CanBaseReadDataConvertToUint16(uint8_t* bufferPtr,uint16_t posOffset)
{
    uint16_t resultData = 0;
    resultData += bufferPtr[posOffset + 0];
    resultData <<= 8;
    resultData += bufferPtr[posOffset + 1];

    return resultData;
}

//从数据缓存区读取一个浮点数
float CanBaseReadDataConvertToFloat(uint8_t* bufferPtr,uint16_t posOffset)
{
    uint32_t resultTemp = 0;
    float resultData = 0;
    resultTemp += bufferPtr[posOffset + 0];
    resultTemp <<= 8;
    resultTemp += bufferPtr[posOffset + 1];
    resultTemp <<= 8;
    resultTemp += bufferPtr[posOffset + 2];
    resultTemp <<= 8;
    resultTemp += bufferPtr[posOffset + 3];

    resultData = *((float*)(&resultTemp));

    return resultData;
}

//往数据缓存中写入一个协议的有符号32位数
void CanBaseWriteInt32DataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,int32_t writeDat)
{
    uint32_t writeTemp = 0;
    writeTemp = *((uint32_t*)(&writeDat));

    bufferPtr[posOffset + 3] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 2] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 1] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 0] = (uint8_t)writeTemp;
}

//往数据缓存中写入一个协议的无符号32位数
void CanBaseWriteUint32DataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,uint32_t writeDat)
{
    uint32_t writeTemp = writeDat;

    bufferPtr[posOffset + 3] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 2] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 1] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 0] = (uint8_t)writeTemp;
}

//往数据缓存中写入一个协议的有符号16位数
void CanBaseWriteInt16DataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,int16_t writeDat)
{
    uint16_t writeTemp = 0;
    writeTemp = *((uint16_t*)(&writeDat));

    bufferPtr[posOffset + 1] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 0] = (uint8_t)writeTemp;
}

//往数据缓存中写入一个协议的无符号16位数
void CanBaseWriteUint16DataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,uint16_t writeDat)
{
    uint16_t writeTemp = writeDat;

    bufferPtr[posOffset + 1] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 0] = (uint8_t)writeTemp;
}

//从数据缓存区中写入一个浮点数
void CanBaseWriteFloatDataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,float writeDat)
{
    uint32_t writeTemp = 0;
    writeTemp = *((uint32_t*)(&writeDat));

    bufferPtr[posOffset + 3] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 2] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 1] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[posOffset + 0] = (uint8_t)writeTemp;
}





















