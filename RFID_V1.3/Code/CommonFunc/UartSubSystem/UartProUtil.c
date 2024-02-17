#include "UartProUtil.h"
//将无符号32位数转换为buffer
void ConvertUint32ToArrayLowFirst(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat)
{
    //低位在前,高位在后
    uint32_t useData = srcDat;
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 3] = (uint8_t)useData;
}

//将无符号32位数转换为buffer
void ConvertUInt32ToArrayHighFirst(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat)
{
    //高位在前,低位在后
    uint32_t useData = srcDat;
    bufferPtr[startPos + 3] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 0] = (uint8_t)useData;
}

//将无符号16位数转换为buffer,低位在前
void ConvertUint16ToArrayLowFist(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat)
{
    //低位在前,高位在后
    uint16_t useData = srcDat;
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
}

//将无符号16位数转换为buffer，高位在前
void ConvertUint16ToArrayHighFist(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat)
{
    //高位在前,低位在后
    uint16_t useData = srcDat;
    bufferPtr[startPos + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 0] = (uint8_t)useData;
}


//将buffer转换为无符号32位数,低位在前
uint32_t ConvertArrayToUint32LowFirst(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t destUint32 = 0;
    destUint32 += bufferPtr[3 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[2 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[1 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[0 + startPos];

    return destUint32;
}

//将buffer转换为无符号32位数,低位在前
uint32_t ConvertArrayToUint32HighFirst(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t destUint32 = 0;
    destUint32 += bufferPtr[0 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[1 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[2 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[3 + startPos];

    return destUint32;
}


//将buffer转换为无符号16位数,高位在前
uint16_t ConvertArrayToUint16HighFirst(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destUint16 = 0;
    destUint16 += bufferPtr[0 + startPos];
    destUint16 <<= 8;
    destUint16 += bufferPtr[1 + startPos];
    
    return destUint16;
}

//将buffer转换为无符号16位数,低位在前
uint16_t ConvertArrayToUint16LowFirst(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destUint16 = 0;
    destUint16 += bufferPtr[1 + startPos];
    destUint16 <<= 8;
    destUint16 += bufferPtr[0 + startPos];
    
    return destUint16;
}

//将buffer转换为有符号16位数,高位在前
int16_t ConvertArrayToInt16HighFirst(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destUint16 = 0;
    int16_t destInt16 = 0;
    destUint16 += bufferPtr[0 + startPos];
    destUint16 <<= 8;
    destUint16 += bufferPtr[1 + startPos];

    destInt16 = *((int16_t*)(&destUint16));
    return destInt16;
}

//将buffer转换为有符号16位数,低位在前
int16_t ConvertArrayToInt16LowFirst(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destUint16 = 0;
    int16_t destInt16 = 0;
    destUint16 += bufferPtr[1 + startPos];
    destUint16 <<= 8;
    destUint16 += bufferPtr[0 + startPos];

    destInt16 = *((int16_t*)(&destUint16));
    return destInt16;
}

