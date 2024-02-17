/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-30 11:48:02
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-18 14:10:19
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanBaseUtil.h"
#include "CanBaseConfig.h"

//计算CRC16
uint16_t CanBaseUtilCalcCRC16(uint8_t* bufferPtr,uint16_t bufferLength)
{
    uint16_t jg   = 0xFFFF;
    uint16_t Poly = 0xA001;
    uint16_t temp;
    uint16_t flag;
    uint32_t i,n;

    if ((NULL == bufferPtr)||(0 == bufferLength))
    {
        return 0xFFFF;
    }
		
    for(i=0;i<bufferLength;i++)
    {
        jg ^= bufferPtr[i];
		
        for(n=0;n<8;n++)
        {
            temp = jg;
            jg = jg>>1;
            flag = 0x0001&temp;
            if(flag)
            {
				jg^=Poly;
            }
        }
    }
    return jg;
}

//接收指令快速切片
void CanBaseUtilFastSectionID(uint32_t id,CAN_ID_SECTION* sectionPtr)
{
    //记录原始ID
    sectionPtr->canSrcId = id;
    //对数据的ID进行解析
    sectionPtr->command = (id&0x7FF);
    id >>= 11;
    //目的板号
    sectionPtr->targetBoardAddr = (id&0x3f);
    id >>= 6;
    //本地板号
    sectionPtr->srcBoardAddr = (id&0x3f);
    id >>= 6;
    //目标模块号
    sectionPtr->targetModuleNo = (id&0x07);
    id >>= 3;
    //本地模块号
    sectionPtr->srcModuleNo = (id&0x07);
    //如果目标板号是广播号,将其转换为本地板号
    if(sectionPtr->targetBoardAddr == CAN_BOARDCAST_BOARD_ID)
    {
        sectionPtr->targetBoardAddr = CAN_SELF_BOARD_ID;
    }
}

/*******************************************通讯数据包ID创建****************************************************/
//创建短包数据ID
uint32_t CanBaseUtilShortID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr,uint16_t command)
{
    uint32_t idValue = 0;
    //源模块号
    srcModuleNo &= 0x07;
    //目标模块号
    targetModuleNo &= 0x07;
    //本地板号
    srcBoardAddr &= 0x3f;
    //目的板号
    targetBoardAddr &= 0x3f;
    //指令码
    command &= 0x07ff;
    //计算ID代码
    idValue += srcModuleNo;
    idValue <<= 3;
    idValue += targetModuleNo;
    idValue <<= 6;
    idValue += srcBoardAddr;
    idValue <<= 6;
    idValue += targetBoardAddr;
    idValue <<= 11;
    idValue += command;

    return idValue;
}

//创建长包起始包ID
uint32_t CanBaseUtilLongStartID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr)
{
    uint32_t idValue = 0;
    //源模块号
    srcModuleNo &= 0x07;
    //目标模块号
    targetModuleNo &= 0x07;
    //本地板号
    srcBoardAddr &= 0x3f;
    //目的板号
    targetBoardAddr &= 0x3f;
    //计算ID代码
    idValue += srcModuleNo;
    idValue <<= 3;
    idValue += targetModuleNo;
    idValue <<= 6;
    idValue += srcBoardAddr;
    idValue <<= 6;
    idValue += targetBoardAddr;
    idValue <<= 11;
    idValue += CAN_COMMAND_LONG_PACK_START;

    return idValue;
}

//创建长包数据包ID
uint32_t CanBaseUtilLongDataID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr)
{
    uint32_t idValue = 0;
    //源模块号
    srcModuleNo &= 0x07;
    //目标模块号
    targetModuleNo &= 0x07;
    //本地板号
    srcBoardAddr &= 0x3f;
    //目的板号
    targetBoardAddr &= 0x3f;
    //计算ID代码
    idValue += srcModuleNo;
    idValue <<= 3;
    idValue += targetModuleNo;
    idValue <<= 6;
    idValue += srcBoardAddr;
    idValue <<= 6;
    idValue += targetBoardAddr;
    idValue <<= 11;
    idValue += CAN_COMMAND_LONG_PACK_DATA;

    return idValue;
}

//创建长包结束包ID
uint32_t CanBaseUtilLongEndID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr)
{
    uint32_t idValue = 0;
    //源模块号
    srcModuleNo &= 0x07;
    //目标模块号
    targetModuleNo &= 0x07;
    //本地板号
    srcBoardAddr &= 0x3f;
    //目的板号
    targetBoardAddr &= 0x3f;
    //计算ID代码
    idValue += srcModuleNo;
    idValue <<= 3;
    idValue += targetModuleNo;
    idValue <<= 6;
    idValue += srcBoardAddr;
    idValue <<= 6;
    idValue += targetBoardAddr;
    idValue <<= 11;
    idValue += CAN_COMMAND_LONG_PACK_END;

    return idValue;
}


/********************************************将数据按照协议转换到数组中*************************************************/
//将无符号64位数转换为buffer
void CanBaseUtilConvertUint64ToArray(uint8_t* bufferPtr,uint16_t startPos,uint64_t srcDat)
{
    //低位在前,高位在后
    bufferPtr[startPos + 0] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 3] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 4] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 5] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 6] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 7] = (uint8_t)srcDat;
}

//将无符号32位数转换为buffer
void CanBaseUtilConvertUint32ToArray(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat)
{
    //低位在前,高位在后
    bufferPtr[startPos + 0] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 3] = (uint8_t)srcDat;
}

//将无符号16位数转换为buffer
void CanBaseUtilConvertUint16ToArray(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat)
{
    //低位在前,高位在后
    bufferPtr[startPos + 0] = (uint8_t)srcDat;
    srcDat >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)srcDat;
}

//将有符号64位数转换为buffer
void CanBaseUtilConvertInt64ToArray(uint8_t* bufferPtr,uint16_t startPos,int64_t srcDat)
{
    //低位在前,高位在后
    uint64_t useData = *((uint64_t*)(&srcDat));
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 3] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 4] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 5] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 6] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 7] = (uint8_t)useData;
}

//将有符号32位数转换为buffer
void CanBaseUtilConvertInt32ToArray(uint8_t* bufferPtr,uint16_t startPos,int32_t srcDat)
{
    //低位在前,高位在后
    uint32_t useData = *((uint32_t*)(&srcDat));
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 3] = (uint8_t)useData;
}

//将有符号16位数转换为buffer
void CanBaseUtilConvertInt16ToArray(uint8_t* bufferPtr,uint16_t startPos,int16_t srcDat)
{
    //低位在前,高位在后
    uint32_t useData = *((uint32_t*)(&srcDat));
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
}

//将浮点数数转换为buffer
void CanBaseUtilConvertFloatToArray(uint8_t* bufferPtr,uint16_t startPos,float srcDat)
{
    //低位在前,高位在后
    uint32_t useData = *((uint32_t*)(&srcDat));
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 3] = (uint8_t)useData;
}


/********************************************从数组中按照协议转换出数据***********************************************/
//将buffer转换为无符号64位数
uint64_t CanBaseUtilConvertArrayToUint64(uint8_t* bufferPtr,uint16_t startPos)
{
    uint64_t destDatUnsigned = 0;
    destDatUnsigned += bufferPtr[7 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[6 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[5 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[4 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[3 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[2 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[1 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[0 + startPos];
    return destDatUnsigned;
}

//将buffer转换为无符号32位数
uint32_t CanBaseUtilConvertArrayToUint32(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t destDatUnsigned = 0;
    destDatUnsigned += bufferPtr[3 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[2 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[1 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[0 + startPos];
    return destDatUnsigned;
}

//将buffer转换为无符号16位数
uint16_t CanBaseUtilConvertArrayToUint16(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destDatUnsigned = 0;
    destDatUnsigned += bufferPtr[1 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[0 + startPos];
    return destDatUnsigned;
}

//将buffer转换为无符号64位数
int64_t CanBaseUtilConvertArrayToInt64(uint8_t* bufferPtr,uint16_t startPos)
{
    uint64_t destDatUnsigned = 0;
    int64_t destReturn = 0;
    destDatUnsigned += bufferPtr[7 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[6 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[5 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[4 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[3 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[2 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[1 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[0 + startPos];
    //转换为有符号
    destReturn = *((int64_t*)(&destDatUnsigned));
    return destReturn;
}

//将buffer转换为有符号32位数
int32_t CanBaseUtilConvertArrayToInt32(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t destDatUnsigned = 0;
    int32_t destReturn = 0;
    destDatUnsigned += bufferPtr[3 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[2 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[1 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[0 + startPos];
    //转换为有符号
    destReturn = *((int32_t*)(&destDatUnsigned));
    return destReturn;
}

//将buffer转换为有符号16位数
int16_t CanBaseUtilConvertArrayToInt16(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destDatUnsigned = 0;
    int16_t destReturn = 0;
    destDatUnsigned += bufferPtr[1 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[0 + startPos];
    //转换为有符号
    destReturn = *((int32_t*)(&destDatUnsigned));
    return destReturn;
}

//将buffer转换为浮点数
float CanBaseUtilConvertArrayToFloat(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t destDatUnsigned = 0;
    float destReturn = 0;
    destDatUnsigned += bufferPtr[3 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[2 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[1 + startPos];
    destDatUnsigned <<= 8;
    destDatUnsigned += bufferPtr[0 + startPos];
    //转换为有符号
    destReturn = *((float*)(&destDatUnsigned));
    return destReturn;
}

