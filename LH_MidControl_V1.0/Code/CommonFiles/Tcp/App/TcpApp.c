/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-12 20:20:08
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-20 10:40:44
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "TcpApp.h"
#include "UserMemManage.h"

//报警上传
LH_ERR TcpAppReportWarning(uint8_t moduleNo,uint8_t boardID,uint8_t errorLevel,uint16_t errorCode,uint32_t errorParam,uint8_t* paramBuffer,uint8_t paramLength)
{
    //报警上传固定二十个字节
    uint16_t sendBufferDataLength = 20;
    //申请内存并清空
    uint8_t* sendBufferPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,sendBufferDataLength);;
    UserMemSet(sendBufferPtr,0,sendBufferDataLength);
    //报警填充
    sendBufferPtr[0] = moduleNo;
    sendBufferPtr[1] = boardID;
    sendBufferPtr[2] = errorLevel;
    TcpBaseUtilConvertUint16ToBuffer(errorCode,sendBufferPtr,3);
    TcpBaseUtilConvertUint32ToBuffer(errorParam,sendBufferPtr,5);
    if(paramBuffer != 0)
    {
        paramLength = (paramLength > (sendBufferDataLength-9))?(sendBufferDataLength-9):paramLength;
        //剩余数据拷贝
        UserMemCopy(sendBufferPtr+9,paramBuffer,paramLength);
    }
    //拷贝完成,数据传输
    LH_ERR errorCodeLocal = LH_ERR_NONE;
    errorCodeLocal = TcpBaseSendCommandWithData(TCM_CMD_WARNING_REPORT,sendBufferPtr,sendBufferDataLength);
    //发送完成,释放内存
    UserMemFree(MEM_SDRAM1,sendBufferPtr);
    //回传错误代码
    return errorCodeLocal;
}

//回传急停结果
LH_ERR TcpAppReportEmergencyStopResult(TCP_CMD_RESULT result)
{
    uint8_t bufferSend[1] = {0};
    //返回结果
    bufferSend[0] = (uint8_t)(result);
    return TcpBaseSendCommandWithData(TCP_CMD_EMSTOP,bufferSend,1);
}


//回传调试结果
LH_ERR TcpAppSendDebugControlResultComm(uint8_t moduleNo,uint8_t boardID,uint16_t debugCmd,uint8_t errorLevel,uint32_t errorCode,uint32_t errorParam,uint8_t* paramBuffer,uint16_t paramLength)
{
    uint16_t sendBufferDataLength = 13+paramLength;
    uint8_t* sendBufferPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,sendBufferDataLength);
    sendBufferPtr[0] = moduleNo;//模块号
    sendBufferPtr[1] = boardID;//板号
    TcpBaseUtilConvertUint16ToBuffer(debugCmd,sendBufferPtr,2);
    TcpBaseUtilConvertUint32ToBuffer((uint32_t)errorCode,sendBufferPtr,4);
    sendBufferPtr[8] = errorLevel;
    TcpBaseUtilConvertUint32ToBuffer((uint32_t)errorParam,sendBufferPtr,9);
    if(paramLength != 0)
    {
        UserMemCopy(sendBufferPtr+13,paramBuffer,paramLength);
    }
    LH_ERR errorCodeLocal = LH_ERR_NONE;
    errorCodeLocal = TcpBaseSendCommandWithData(TCP_CMD_DEBUG_CONTROL,sendBufferPtr,sendBufferDataLength);
    //释放内存
    UserMemFree(MEM_SDRAM1,sendBufferPtr);
    //回传结果
    return errorCodeLocal;
}

//立禾云调试结果回传
LH_ERR TcpAppSendDebugControlResultCloud(uint8_t moduleNo,uint8_t boardID,uint16_t debugCmd,uint8_t errorLevel,uint16_t errorCode,uint32_t errorParam,uint8_t* paramBuffer,uint16_t paramLength)
{
    uint16_t sendBufferDataLength = 11+paramLength;
    uint8_t* sendBufferPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,sendBufferDataLength);
    sendBufferPtr[0] = moduleNo;//模块号
    sendBufferPtr[1] = boardID;//板号
    TcpBaseUtilConvertUint16ToBuffer(debugCmd,sendBufferPtr,2);
    TcpBaseUtilConvertUint16ToBuffer((uint16_t)errorCode,sendBufferPtr,4);
    sendBufferPtr[6] = errorLevel;
    TcpBaseUtilConvertUint32ToBuffer((uint32_t)errorParam,sendBufferPtr,7);
    if(paramLength != 0)
    {
        UserMemCopy(sendBufferPtr+11,paramBuffer,paramLength);
    }
    LH_ERR errorCodeLocal = LH_ERR_NONE;
    errorCodeLocal = TcpBaseSendCommandWithData(TCP_CMD_DEBUG_CONTROL,sendBufferPtr,sendBufferDataLength);
    //释放内存
    UserMemFree(MEM_SDRAM1,sendBufferPtr);
    //回传结果
    return errorCodeLocal;
}

LH_ERR TcpAppSendDebugControlResultBio(uint8_t* dataBuffer,uint16_t dataLength)
{
    if(dataLength == 0)
    {
        return LH_ERR_NONE;
    }
    else
    {
        uint8_t* sendBufferPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,dataLength);
        UserMemCopy(sendBufferPtr,dataBuffer,dataLength);
        LH_ERR errorCodeLocal = LH_ERR_NONE;
        errorCodeLocal = TcpBaseSendCommandWithData(TCP_CMD_DEBUG_CONTROL,sendBufferPtr,dataLength);
        //释放内存
        UserMemFree(MEM_SDRAM1,sendBufferPtr);
        //回传结果
        return errorCodeLocal;
    }
}
