/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-06 11:26:49
**LastEditors: DengXiaoJun
**LastEditTime: 2021-10-19 13:49:39
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanAppUtil.h"
#include "SrvTaskConfig.h"


//CAN总线回传ACK状态
LH_ERR CanSendAckDefault(uint8_t targetModule,uint8_t targetBoard,CAN_ACK_STATE ackState)
{
    //发送ACK不需要确认
    return CanSendPackDefaultWithoutAck(CAN_CMD_COMM_ACK,targetModule,targetBoard,(uint8_t*)(&ackState),1);
}

//CAN总线发送IAP准备
LH_ERR CanSendIapPrepare(uint8_t targetModule,uint8_t targetBoard,uint8_t iapBoardID)
{
    return CanSendPackDefaultWithAck(CAN_CMD_COMM_IAP_PREPARE,targetModule,targetBoard,&iapBoardID,1);
}

//CAN总线发送IAP开始
LH_ERR CanSendIapStart(uint8_t targetModule,uint8_t targetBoard,uint8_t iapBoardID,uint32_t byteCount,uint16_t packCount,uint16_t packackLengt)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t* canSendBuf = NULL;
    canSendBuf = UserMemMallocWhileSuccess(MEM_D2_SRAM2,9);
    canSendBuf[0] = (uint8_t)iapBoardID;
    CanBaseUtilConvertUint32ToArray(canSendBuf,1,byteCount);
    CanBaseUtilConvertUint16ToArray(canSendBuf,5,packCount);
    CanBaseUtilConvertUint16ToArray(canSendBuf,7,packackLengt);
    errorCode = CanSendPackDefaultWithAck(CAN_CMD_COMM_IAP_START,targetModule,targetBoard,canSendBuf,9);
    //释放内存
    UserMemFree(MEM_D2_SRAM2,canSendBuf);
    return errorCode;
}

//CAN总线发送IAP数据
LH_ERR CanSendIapData(uint8_t targetModule,uint8_t targetBoard,uint8_t iapBoardID,uint16_t packIndex,uint8_t* datBuf,uint16_t dataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t* canSendBuf = NULL;
    canSendBuf = UserMemMallocWhileSuccess(MEM_D2_SRAM2,3+dataLength);
    canSendBuf[0] = (uint8_t)iapBoardID;
    CanBaseUtilConvertUint16ToArray(canSendBuf,1,packIndex);
    UserMemCopy(canSendBuf+3,datBuf,dataLength);
    errorCode = CanSendPackDefaultWithAck(CAN_CMD_COMM_IAP_DATA,targetModule,targetBoard,canSendBuf,3+dataLength);
    //释放内存
    UserMemFree(MEM_D2_SRAM2,canSendBuf);
    return errorCode;
}

//CAN总线发送IAP结果
LH_ERR CanSendIapEnd(uint8_t targetModule,uint8_t targetBoard,uint8_t iapBoardID,uint32_t crcCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t* canSendBuf = NULL;
    canSendBuf = UserMemMallocWhileSuccess(MEM_D2_SRAM2,5);
    canSendBuf[0] = (uint8_t)iapBoardID;
    CanBaseUtilConvertUint32ToArray(canSendBuf,1,crcCode);
    errorCode = CanSendPackDefaultWithAck(CAN_CMD_COMM_IAP_END,targetModule,targetBoard,canSendBuf,5);
    //释放内存
    UserMemFree(MEM_D2_SRAM2,canSendBuf);
    return errorCode;
}

//CAN总线发送读取Boot版本号
LH_ERR CanSendReadBootVersion(uint8_t targetModule,uint8_t targetBoard)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t canSendBuf[2] = {0};
    canSendBuf[0] = targetModule;
    canSendBuf[1] = (uint8_t)targetBoard;
    errorCode = CanSendPackDefaultWithAck(CAN_CMD_COMM_BOOT_VERSION,targetModule,targetBoard,canSendBuf,2);
    return errorCode;
}

//CAN发送读取APP版本号
LH_ERR CanSendReadAppVersion(uint8_t targetModule,uint8_t targetBoard)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t canSendBuf[2] = {0};
    canSendBuf[0] = targetModule;
    canSendBuf[1] = (uint8_t)targetBoard;
    errorCode = CanSendPackDefaultWithAck(CAN_CMD_COMM_APP_VERSION,targetModule,targetBoard,canSendBuf,2);
    return errorCode;
}

//CAN发送调试指令.轨道,免疫
LH_ERR CanSendDebugControlMsgComm(uint8_t targetModule,uint8_t targetBoard,uint16_t debugCmd,uint8_t* paramBufferPtr,uint16_t paramLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t* canSendBuf = NULL;
    canSendBuf = UserMemMallocWhileSuccess(MEM_D2_SRAM2,3+paramLength);
    canSendBuf[0] = (uint8_t)targetBoard;
    CanBaseUtilConvertUint16ToArray(canSendBuf,1,debugCmd);
    if(paramLength != 0)
    {
        UserMemCopy(canSendBuf+3,paramBufferPtr,paramLength);
    }
    errorCode = CanSendPackDefaultWithAck(CAN_CMD_COMM_DEBUG_CONTROL,targetModule,targetBoard,canSendBuf,3+paramLength);
    //释放内存
    UserMemFree(MEM_D2_SRAM2,canSendBuf);
    return errorCode;
} 

//CAN发送调试指令,生化版本
LH_ERR CanSendDebugControlMsgBio(uint8_t targetModule,uint8_t targetBoard,uint16_t debugCmd,uint8_t* paramBufferPtr,uint16_t paramLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t* canSendBuf = NULL;
    canSendBuf = UserMemMallocWhileSuccess(MEM_D2_SRAM2,3+paramLength);
    canSendBuf[0] = (uint8_t)targetBoard;
    CanBaseUtilConvertUint16ToArray(canSendBuf,1,debugCmd);
    if(paramLength != 0)
    {
        UserMemCopy(canSendBuf+3,paramBufferPtr,paramLength);
    }
    errorCode = CanSendPackDefaultWithAck(CAN_CMD_COMM_DEBUG_CONTROL,targetModule,CAN_DEVICE_BOARD_ID,canSendBuf,3+paramLength);
    //释放内存
    UserMemFree(MEM_D2_SRAM2,canSendBuf);
    return errorCode;
}

//CAN向模块发送指令
LH_ERR CanSendDataToModule(uint16_t cmd, uint8_t targetModule,uint8_t targetBoard,uint8_t* paramBufferPtr,uint16_t paramLength)
{
    int32_t i;
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = CanSendPackDefaultWithAck(cmd,targetModule,targetBoard,paramBufferPtr,paramLength);
    if(errorCode != LH_ERR_NONE)
    {
        SystemPrintf("E>CanSendDataToModule:%d,%d,%d\r\n", cmd,targetModule,errorCode);
    }

    if(cmd == CAN_CMD_IMM_MULTI_REAGENT)
    {
        for(i = 0; i < paramLength; i++)
        {
            SystemPrintf("%02X,", paramBufferPtr[i]);
        }
        SystemPrintf("\r\n");
    }
    
    return errorCode;
}

    
    
    
    
    
