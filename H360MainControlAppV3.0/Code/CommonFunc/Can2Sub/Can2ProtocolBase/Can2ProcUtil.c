/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:44:51
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-11 14:39:54
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\Can2Protocol\Can2ProcUtil.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2ProcUtil.h"

//创建ID
uint32_t Can2ProcUtilCreatePackID(uint8_t targetBoardAddr,uint8_t channelNo,uint8_t frameCode,
                        CAN2_PACK_TYPE packType,CAN2_PACK_LEN_FLAG lenFlag,CAN2_SINGLE_CODE singleCode)
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
    resultID += DEVICE_MAIN_BOARD_ID;
    resultID <<= 6;
    resultID += targetBoardAddr;
    return resultID;
}

//将无符号32位数转换为buffer
void Can2ProcUtilConvertUint32ToArray(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat)
{
    uint32_t srcUint32 = srcDat;
    bufferPtr[0 + startPos] = (uint8_t)(srcUint32 >> 24);
    bufferPtr[1 + startPos] = (uint8_t)(srcUint32 >> 16);
    bufferPtr[2 + startPos] = (uint8_t)(srcUint32 >> 8);
    bufferPtr[3 + startPos] = (uint8_t)(srcUint32);
}

//将有符号32位数转换为buffer
void Can2ProcUtilConvertInt32ToArray(uint8_t* bufferPtr,uint16_t startPos,int32_t srcDat)
{
    uint32_t srcUint32 = *((uint32_t*)(&srcDat));
    bufferPtr[0 + startPos] = (uint8_t)(srcUint32 >> 24);
    bufferPtr[1 + startPos] = (uint8_t)(srcUint32 >> 16);
    bufferPtr[2 + startPos] = (uint8_t)(srcUint32 >> 8);
    bufferPtr[3 + startPos] = (uint8_t)(srcUint32);
}

//将无符号16位数转换为buffer
void Can2ProcUtilConvertUint16ToArray(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat)
{
    uint16_t srcUint16 = srcDat;
    bufferPtr[0 + startPos] = (uint8_t)(srcUint16 >> 8);
    bufferPtr[1 + startPos] = (uint8_t)(srcUint16);
}

//将有符号16位数转换为buffer
void Can2ProcUtilConvertInt16ToArray(uint8_t* bufferPtr,uint16_t startPos,int16_t srcDat)
{
    uint16_t srcUint16 = *((uint16_t*)(&srcDat));
    bufferPtr[0 + startPos] = (uint8_t)(srcUint16 >> 8);
    bufferPtr[1 + startPos] = (uint8_t)(srcUint16);
}


//从数据缓存区中写入一个浮点数
void Can2ProcUtilConvertFloatDataToArray(uint8_t* bufferPtr,uint16_t startPos,float srcDat)
{
    uint32_t writeTemp = 0;
    writeTemp = *((uint32_t*)(&srcDat));

    bufferPtr[startPos + 3] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)writeTemp;
    writeTemp >>= 8;
    bufferPtr[startPos + 0] = (uint8_t)writeTemp;
}

//将buffer转换为无符号32位数
uint32_t Can2ProcUtilConvertArrayToUint32(uint8_t* bufferPtr,uint16_t startPos)
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

//将buffer转换为有符号32位数
int32_t Can2ProcUtilConvertArrayToInt32(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t destUint32 = 0;
    int32_t destInt32 = 0;
    destUint32 += bufferPtr[0 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[1 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[2 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[3 + startPos];

    destInt32 = *((int32_t*)(&destUint32));
    return destInt32;
}

//将buffer转换为无符号16位数
uint16_t Can2ProcUtilConvertArrayToUint16(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destUint16 = 0;
    destUint16 += bufferPtr[0 + startPos];
    destUint16 <<= 8;
    destUint16 += bufferPtr[1 + startPos];
    
    return destUint16;
}

//将buffer转换为有符号16位数
int16_t Can2ProcUtilConvertArrayToInt16(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destUint16 = 0;
    int16_t destInt16 = 0;
    destUint16 += bufferPtr[0 + startPos];
    destUint16 <<= 8;
    destUint16 += bufferPtr[1 + startPos];

    destInt16 = *((int16_t*)(&destUint16));
    return destInt16;
}

//从数据缓存区读取一个浮点数
float Can2ProcUtilConvertArrayToFloat(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t resultTemp = 0;
    float resultData = 0;
    resultTemp += bufferPtr[startPos + 0];
    resultTemp <<= 8;
    resultTemp += bufferPtr[startPos + 1];
    resultTemp <<= 8;
    resultTemp += bufferPtr[startPos + 2];
    resultTemp <<= 8;
    resultTemp += bufferPtr[startPos + 3];

    resultData = *((float*)(&resultTemp));

    return resultData;
}

















