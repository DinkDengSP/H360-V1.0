/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-04 15:01:02
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-12 19:34:37
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "TcpBaseUtil.h"
#include "UserMemManage.h"

//网口状态集合
static NET_STATE_SET netStateSet;

//网口指令包数据初始化
void TcpBaseUtilNetCmdPackInit(NET_COMMAND_PACK* netCmdPackPtr)
{
    if(netCmdPackPtr == NULL)
    {
        return;
    }
    netCmdPackPtr->netCommand = 0;
    netCmdPackPtr->bodyDataLength = 0;
    netCmdPackPtr->bodyDataCheckSum = 0;
    netCmdPackPtr->headerDataCheckSum = 0;
    netCmdPackPtr->bodyData = NULL;
}


//初始化TCP通讯辅助状态
void TcpBaseUtilStateInit(void)
{
    netStateSet.tcpProtocolRecvAckFlag = TCP_NET_ACK_OK;
    netStateSet.systemLastHeartTick = HEART_TICK_ERR_UNUSED;
    netStateSet.netCmdRecvState.packLastRecvTimeTick = 0;
    netStateSet.netCmdRecvState.recvDataLength = 0;
    netStateSet.netCmdRecvState.recvBodyLength = 0;
    //创建信号量
    tx_semaphore_create(&(netStateSet.semTcpProtocolAck),"semTcpProtocolAck",0);
}


//获取当前网卡ack信号量指针
TX_SEMAPHORE* TcpBaseUtilGetNetAckSemPtr(void)
{
    return &(netStateSet.semTcpProtocolAck);
}

//获取设置网络通讯ACK标志
TCP_NET_ACK TcpBaseUtilGetNetAckFlag(void)
{
    return netStateSet.tcpProtocolRecvAckFlag;
}
void TcpBaseUtilSetNetAckFlag(TCP_NET_ACK ackFlag)
{
    netStateSet.tcpProtocolRecvAckFlag = ackFlag;
}

//获取设置心跳时间
uint32_t TcpBaseUtilGetLastHeartTick(void)
{
    return netStateSet.systemLastHeartTick;
}
void TcpBaseUtilSetLastHeartTick(uint32_t timeSet)
{
    netStateSet.systemLastHeartTick = timeSet;
}
//设置心跳时间为当前时间
void TcpBaseUtilSetLastHeartTickNow(void)
{
    netStateSet.systemLastHeartTick = tx_time_get();
}

//获取网口接收状态指针
NET_CMD_RECV_STATE* TcpBaseUtilGetRecvStatePtr(void)
{
    return &(netStateSet.netCmdRecvState);
}
//清零网口数据接收状态
void TcpBaseUtilClearRecvState(void)
{
    netStateSet.netCmdRecvState.packLastRecvTimeTick = 0;
    netStateSet.netCmdRecvState.recvDataLength = 0;
    netStateSet.netCmdRecvState.recvBodyLength = 0;
    UserMemSet(netStateSet.netCmdRecvState.recvDat,0,TCP_CMD_SIZE_MAX);
}
/***********************************字节数据转换为整机数据*************************************/
//从缓存中转换出一个无符号64位数
uint64_t TcpBaseUtilConvertBufferToUint64(uint8_t* bufferPtr,uint16_t startPos)
{
    uint64_t resultValue = 0;
    resultValue += bufferPtr[startPos+7];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos+6];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos+5];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos+4];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos+3];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos+2];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos+1];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos];
    return resultValue;
}
//从缓存中转换出一个无符号三十二位数
uint32_t TcpBaseUtilConvertBufferToUint32(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t resultValue = 0;
    resultValue += bufferPtr[startPos+3];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos+2];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos+1];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos];
    return resultValue;
}
//从缓存中转换出一个无符号16位数
uint16_t TcpBaseUtilConvertBufferToUint16(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t resultValue = 0;
    resultValue += bufferPtr[startPos+1];
    resultValue <<= 8;
    resultValue += bufferPtr[startPos];
    return resultValue;
}
//从缓存中转换出一个有符号64位数
int64_t TcpBaseUtilConvertBufferToInt64(uint8_t* bufferPtr,uint16_t startPos)
{
    uint64_t resultTemp = TcpBaseUtilConvertBufferToUint64(bufferPtr,startPos);
    int64_t result = *((int64_t*)(&resultTemp));
    return result;
}
//从缓存中转换出一个有符号32位数
int32_t TcpBaseUtilConvertBufferToInt32(uint8_t* bufferPtr,uint16_t startPos)
{   
    uint32_t resultTemp = TcpBaseUtilConvertBufferToUint32(bufferPtr,startPos);
    int32_t result = *((int32_t*)(&resultTemp));
    return result;
}
//从缓存中转换出一个有符号16位数
int16_t TcpBaseUtilConvertBufferToInt16(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t resultTemp = TcpBaseUtilConvertBufferToUint16(bufferPtr,startPos);
    int16_t result = *((int16_t*)(&resultTemp));
    return result;
}
//从缓存中转换出一个浮点数
float TcpBaseUtilConvertBufferToFloat(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t resultTemp = TcpBaseUtilConvertBufferToUint32(bufferPtr,startPos);
    float result = *((float*)(&resultTemp));
    return result;
}

/***********************************整体数据转换为字节数据******************************************/
//无符号64位数转换成数组
void TcpBaseUtilConvertUint64ToBuffer(uint64_t srcData,uint8_t* bufferPtr,uint16_t startPos)
{
    bufferPtr[startPos] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+1] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+2] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+3] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+4] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+5] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+6] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+7] = ((uint8_t)srcData);
    srcData >>= 8;
}
//无符号三十二位数转换成数组
void TcpBaseUtilConvertUint32ToBuffer(uint32_t srcData,uint8_t* bufferPtr,uint16_t startPos)
{
    bufferPtr[startPos] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+1] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+2] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+3] = ((uint8_t)srcData);
}
//无符号十六位数转换成数组
void TcpBaseUtilConvertUint16ToBuffer(uint16_t srcData,uint8_t* bufferPtr,uint16_t startPos)
{
    bufferPtr[startPos] = ((uint8_t)srcData);
    srcData >>= 8;
    bufferPtr[startPos+1] = ((uint8_t)srcData);
}
//有符号六十四位数转换成数组
void TcpBaseUtilConvertInt64ToBuffer(int64_t srcData,uint8_t* bufferPtr,uint16_t startPos)
{
    uint64_t srcDataTemp = *((uint64_t*)(&srcData));
    TcpBaseUtilConvertUint64ToBuffer(srcDataTemp,bufferPtr,startPos);
}
//有符号三十二位数转换成数组
void TcpBaseUtilConvertInt32ToBuffer(int32_t srcData,uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t srcDataTemp = *((uint32_t*)(&srcData));
    TcpBaseUtilConvertUint32ToBuffer(srcDataTemp,bufferPtr,startPos);
}
//有符号十六位数转换成数组
void TcpBaseUtilConvertInt16ToBuffer(int16_t srcData,uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t srcDataTemp = *((uint16_t*)(&srcData));
    TcpBaseUtilConvertUint16ToBuffer(srcDataTemp,bufferPtr,startPos);
}
//有符号浮点数转换为数组
void TcpBaseUtilConvertFloatToBuffer(float srcData,uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t srcDataTemp = *((uint32_t*)(&srcData));
    TcpBaseUtilConvertUint32ToBuffer(srcDataTemp,bufferPtr,startPos);
}
