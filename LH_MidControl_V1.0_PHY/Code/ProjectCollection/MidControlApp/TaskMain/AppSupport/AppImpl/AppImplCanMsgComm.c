/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-09 13:11:20
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-19 20:44:03
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplCanMsgComm.h"
#include "AppStateDataType.h"
#include "AppImplTcpMsg.h"

//准备升级
void AppImplCanMsgCommIapParpare(uint8_t* dataBuf,uint16_t dataLength)
{
    uint8_t moduleNo = dataBuf[0] +1;
    uint8_t boardID = dataBuf[1];
    uint8_t result = dataBuf[2];
    if(result == 0)
    {
        TcpBaseSendIapParpareResult(moduleNo,boardID,TCP_CMD_RESULT_FAILED);
        SystemPrintf("[MID]E>Func:%s,Line:%d ,Failed\r\n",__FUNCTION__,__LINE__);
    }
    else
    {
        SystemPrintf("[MID]Func:%s,Line:%d ,Success\r\n",__FUNCTION__,__LINE__);
        TcpBaseSendIapParpareResult(moduleNo,boardID,TCP_CMD_RESULT_SUCCESS);
    }
}

//升级开始
void AppImplCanMsgCommIapStart(uint8_t* dataBuf,uint16_t dataLength)
{
    uint8_t moduleNo = dataBuf[0] +1;
    uint8_t boardID = dataBuf[1];
    uint8_t result = dataBuf[2];
    if(result == 0)
    {
        TcpBaseSendIapStartResult(moduleNo,boardID,TCP_CMD_RESULT_FAILED);
        SystemPrintf("[MID]E>Func:%s,Line:%d ,Failed\r\n",__FUNCTION__,__LINE__);
    }
    else
    {
        TcpBaseSendIapStartResult(moduleNo,boardID,TCP_CMD_RESULT_SUCCESS);
    }
}

//升级数据传输
void AppImplCanMsgCommIapData(uint8_t* dataBuf,uint16_t dataLength)
{
    uint8_t moduleNo = dataBuf[0] +1;
    uint8_t boardID = dataBuf[1];
    uint8_t result = dataBuf[2];
    if(result == 0)
    {
        TcpBaseSendIapDataResult(moduleNo,boardID,TCP_CMD_RESULT_FAILED);
        SystemPrintf("[MID]E>Func:%s,Line:%d ,Failed\r\n",__FUNCTION__,__LINE__);
    }
    else
    {
        TcpBaseSendIapDataResult(moduleNo,boardID,TCP_CMD_RESULT_SUCCESS);
    }
}

//升级结束
void AppImplCanMsgCommIapEnd(uint8_t* dataBuf,uint16_t dataLength)
{
    uint8_t moduleNo = dataBuf[0] +1;
    uint8_t boardID = dataBuf[1];
    uint8_t result = dataBuf[2];
    if(result == 0)
    {
        TcpBaseSendIapEndResult(moduleNo,boardID,TCP_CMD_RESULT_FAILED);
        SystemPrintf("[MID]E>Func:%s,Line:%d ,Failed\r\n",__FUNCTION__,__LINE__);
    }
    else
    {
        TcpBaseSendIapEndResult(moduleNo,boardID,TCP_CMD_RESULT_SUCCESS);
    }
}

//读取Boot版本号
void AppImplCanMsgCommIapReadBootVersion(uint8_t moduleNo,uint8_t* dataBuf,uint16_t dataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t moduleNoLocal = moduleNo+1;
    uint8_t boardID = dataBuf[0];
    uint8_t bootMainVersion = dataBuf[1];
    uint8_t bootSubVersion = dataBuf[2];
    uint8_t bootDebugVersion = dataBuf[3];
    uint32_t bootSvnVersion = CanBaseUtilConvertArrayToUint32(dataBuf,4);
    //打印讯息
    SystemPrintf("[MID]Func:%s,Line:%d,Module: %d,BoardID: %d,mainVersion: %d,subVersion: %d,debugVersion:%d,SvnVersion:%d\r\n",
                        __FUNCTION__,__LINE__,moduleNo,boardID,bootMainVersion,bootSubVersion,bootDebugVersion,bootSvnVersion);
    //发送给主机
    errorCode = TcpBaseSendBootSoftwareVersion(moduleNoLocal,boardID,bootMainVersion,bootSubVersion,bootDebugVersion,bootSvnVersion);
    if(errorCode != LH_ERR_NONE)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
    }
}

//读取App版本号
void AppImplCanMsgCommIapReadAppVersion(uint8_t moduleNo,uint8_t* dataBuf,uint16_t dataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t moduleNoLocal = moduleNo+1;
    uint8_t boardID = dataBuf[0];
    uint8_t appMainVersion = dataBuf[1];
    uint8_t appSubVersion = dataBuf[2];
    uint8_t appDebugVersion = dataBuf[3];
    uint32_t appSvnVersion = CanBaseUtilConvertArrayToUint32(dataBuf,4);
    //打印讯息
    SystemPrintf("[MID]Func:%s,Line:%d,Module: %d,BoardID: %d,mainVersion: %d,subVersion: %d,debugVersion:%d,SvnVersion:%d\r\n",
                        __FUNCTION__,__LINE__,moduleNo,boardID,appMainVersion,appSubVersion,appDebugVersion,appSvnVersion);
    //发送给主机
    errorCode = TcpBaseSendAppSoftwareVersion(moduleNoLocal,boardID,appMainVersion,appSubVersion,appDebugVersion,appSvnVersion);
    if(errorCode != LH_ERR_NONE)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
    }
}

//处理调试指令返回讯息
void AppImplCanMsgCommDebugResult(uint8_t* dataBuf,uint16_t dataLength)
{
    LH_ERR errorCodeLocal = LH_ERR_NONE;
    uint8_t moduleNo;
    uint8_t boardID;
    uint16_t debugCmd;
    uint8_t errorLevel;
    uint32_t errorParam;
    uint8_t* paramBufferPtr;
    uint16_t paramLength;
    uint8_t u8Module = dataBuf[0] - 1;
    if((dataBuf[0] == CAN_CLOUD_MODULE_NO)&&(dataBuf[1] == CAN_CLOUD_BOARD_ID))
    {
        //立禾云
        moduleNo = dataBuf[0];
        boardID = dataBuf[1];
        debugCmd = CanBaseUtilConvertArrayToUint16(dataBuf,2);
        uint16_t errorCodeCloud = CanBaseUtilConvertArrayToUint16(dataBuf,4);
        errorLevel = dataBuf[6];
        errorParam = CanBaseUtilConvertArrayToUint32(dataBuf,7);
        paramBufferPtr = dataBuf + 11;
        paramLength = dataLength -11;
        SystemPrintf("[MID]Func:%s,Line:%d,Module: %d,BoardID: %d,debugCmd: 0X%04X,errorCodeCloud: 0X%04X,errorParam:0X%08X,paramLength:%d\r\n",
                            __FUNCTION__,__LINE__,moduleNo,boardID,debugCmd,errorCodeCloud,errorParam,paramLength);
        //回传给上位机
        errorCodeLocal = TcpAppSendDebugControlResultCloud(moduleNo+1,boardID,debugCmd,errorLevel,errorCodeCloud,errorParam,paramBufferPtr,paramLength);
    }
    else if(AppImplConfigGetModuleTypeByID(u8Module) == CAN_CFG_CELL_TYPE_CS)
    {
        SystemPrintf("[MID]BioDebugReturn,Func:%s,Line:%d-%d,\r\n",__FUNCTION__,__LINE__,u8Module);
        //生化
        errorCodeLocal = TcpAppSendDebugControlResultBio(dataBuf,dataLength);
    }
    else
    {
        //免疫轨道
        moduleNo = dataBuf[0];
        boardID = dataBuf[1];
        debugCmd = CanBaseUtilConvertArrayToUint16(dataBuf,2);
        uint32_t errorCode = CanBaseUtilConvertArrayToUint32(dataBuf,4);
        errorLevel = dataBuf[8];
        errorParam = CanBaseUtilConvertArrayToUint32(dataBuf,9);
        paramBufferPtr = dataBuf + 13;
        paramLength = dataLength -13;
        SystemPrintf("[MID]Func:%s,Line:%d,Module: %d,BoardID: %d,debugCmd: 0X%04X,errorCode: 0X%04X,errorParam:0X%08X,paramLength:%d\r\n",
                            __FUNCTION__,__LINE__,moduleNo,boardID,debugCmd,errorCode,errorParam,paramLength);
        //回传给上位机
        errorCodeLocal = TcpAppSendDebugControlResultComm(moduleNo+1,boardID,debugCmd,errorLevel,errorCode,errorParam,paramBufferPtr,paramLength);
    }
    if(errorCodeLocal != LH_ERR_NONE)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCodeLocal);
    }
}


//显示设备转发来的日志信息
void AppImplCanMsgCommShowLog(uint8_t* logPrefix,uint8_t* logDataBuffer)
{
    if(logDataBuffer == NULL)return;
    SystemPrintf("%s %s",logPrefix,logDataBuffer);
}

//处理报警讯息
void AppImplCanMsgCommWarningMsg(uint8_t moduleNo,uint8_t* dataBuf,uint16_t dataLength)
{
    LH_ERR errorCodeLocal = LH_ERR_NONE;
    uint8_t boardID = dataBuf[0];
    uint8_t errorLevel = dataBuf[1];
    uint16_t errorCode = CanBaseUtilConvertArrayToUint16(dataBuf,2);
    uint32_t errorParam = CanBaseUtilConvertArrayToUint32(dataBuf,4);
    uint8_t* paramBufferPtr = dataBuf +8;
    uint16_t paramLength = dataLength -8;
    SystemPrintf("[MID]Func:%s,Line:%d,Module: %d,BoardID: %d,errorLevel: 0X%02X,errorCode: 0X%04X,errorParam:0X%08X,paramLength:%d\r\n",
                        __FUNCTION__,__LINE__,moduleNo,boardID,errorLevel,errorCode,errorParam,paramLength);
    //上报错误
    errorCodeLocal = TcpAppReportWarning(moduleNo+1,boardID,errorLevel,errorCode,errorParam,paramBufferPtr,paramLength);
    if(errorCodeLocal != LH_ERR_NONE)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCodeLocal);
    }
}
