/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-10 10:20:00
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-10 11:09:50
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplTcpMsg.h"


//读取Boot版本号
void AppImplTcpMsgDataProcessReadBootVersion(NET_COMMAND_PACK *netDataPactPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BOARD_BOOT_VERSION bootVersion;
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //读取TCP_MSG版本号
    BoardVersion_ReadBootVersion(&bootVersion);
    errorCode = TcpBaseSendBootSoftwareVersion(MODULE_NO_CELL_RAIL_MID+1,BOARD_ID_MID_CONTROL,bootVersion.mainVersion,bootVersion.subVersion,
                                                bootVersion.debugVersion,bootVersion.svnVersion);
    if(errorCode != LH_ERR_NONE)
    {
        SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
    }
}

//读取APP版本号
void AppImplTcpMsgDataProcessReadAppVersion(NET_COMMAND_PACK *netDataPactPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BOARD_APP_VERSION appVersion;
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //读取APP版本号
    BoardVersion_ReadAppVersion(&appVersion);
    errorCode = TcpBaseSendAppSoftwareVersion(MODULE_NO_CELL_RAIL_MID+1,BOARD_ID_MID_CONTROL,appVersion.mainVersion,appVersion.subVersion,
                                                appVersion.debugVersion,appVersion.svnVersion);
    if(errorCode != LH_ERR_NONE)
    {
        SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
    }
}

//在线升级准备
void AppImplTcpMsgDataProcessIapPrepare(NET_COMMAND_PACK *netDataPactPtr)
{
    TCP_CMD_RESULT cmdResult = TCP_CMD_RESULT_SUCCESS;
    //处理数据,解析板号模块号上位机发下来的模块号和系统内部模块号是存在一个错位的
    uint8_t moduleNo = netDataPactPtr->bodyData[0] -1;
    uint8_t boardNo = netDataPactPtr->bodyData[1];
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d,moduleNo: %d,boardNo: %d\r\n",__FUNCTION__,__LINE__,moduleNo,boardNo);
    //TCP_MSG只能给中位机自己升级,中位机升级的时候,模块号发的是1
    if((moduleNo == MODULE_NO_CELL_1)&&(boardNo == BOARD_ID_MID_CONTROL))
    {
        //设置升级准备完成
        BoardIAP_Parpare(IAP_MODE_BOOT);
        cmdResult = TCP_CMD_RESULT_SUCCESS;
    }
    else 
    {
        cmdResult = TCP_CMD_RESULT_FAILED;
    }
    //回传结果
    TcpBaseSendIapParpareResult(moduleNo+1,boardNo,cmdResult);
}
  
//在线升级开始  
void AppImplTcpMsgDataProcessIapStart(NET_COMMAND_PACK *netDataPactPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    TCP_CMD_RESULT cmdResult = TCP_CMD_RESULT_SUCCESS;
    uint8_t moduleNo = netDataPactPtr->bodyData[0] - 1;
    uint8_t boardNo = netDataPactPtr->bodyData[1];
    //记录APP总的大小,包数量,包尺寸
    uint32_t appSizeBytes = TcpBaseUtilConvertBufferToUint32(netDataPactPtr->bodyData,2);
    uint16_t packageCountTotal = TcpBaseUtilConvertBufferToUint16(netDataPactPtr->bodyData,6);
    uint16_t packageSizeSingle = TcpBaseUtilConvertBufferToUint16(netDataPactPtr->bodyData,8);
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d,moduleNo: %d,boardNo: %d,appSizeBytes: %d ,packageCountTotal: %d ,,packageSizeSingle: %d\r\n",
                    __FUNCTION__,__LINE__,moduleNo,boardNo,appSizeBytes,packageCountTotal,packageSizeSingle);
    //TCP_MSG只能给中位机自己升级
    if((moduleNo == MODULE_NO_CELL_1)&&(boardNo == BOARD_ID_MID_CONTROL))
    {
        //延迟一段时间,等待ACK回传完成在进行擦除
        CoreDelayMs(1000);
        //升级开始,Flash擦除
        errorCode = BoardIAP_Start(appSizeBytes,packageCountTotal,packageSizeSingle);
        if(errorCode != LH_ERR_NONE)
        {
            SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
            cmdResult = TCP_CMD_RESULT_FAILED;
        }
        else
        {
            cmdResult = TCP_CMD_RESULT_SUCCESS;
        }
    }
    else 
    {
        cmdResult = TCP_CMD_RESULT_FAILED;
    }
    TcpBaseSendIapStartResult(moduleNo+1,boardNo,cmdResult);
}

//在线升级数据传输       
void AppImplTcpMsgDataProcessIapData(NET_COMMAND_PACK *netDataPactPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    TCP_CMD_RESULT cmdResult = TCP_CMD_RESULT_SUCCESS;
    uint8_t moduleNo = netDataPactPtr->bodyData[0] - 1;
    uint8_t boardNo = netDataPactPtr->bodyData[1];
    uint16_t packIndex = TcpBaseUtilConvertBufferToUint16(netDataPactPtr->bodyData,2);
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d,moduleNo: %d,boardNo: %d,packIndex: %d \r\n",__FUNCTION__,__LINE__,moduleNo,boardNo,packIndex);
    //TCP_MSG只能给中位机自己升级
    if((moduleNo == MODULE_NO_CELL_1)&&(boardNo == BOARD_ID_MID_CONTROL))
    {
        errorCode = BoardIAP_Data(packIndex,netDataPactPtr->bodyDataLength-4,netDataPactPtr->bodyData+4);
        if(errorCode != LH_ERR_NONE)
        {
            SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
            cmdResult = TCP_CMD_RESULT_FAILED;
        }
        else
        {
            cmdResult = TCP_CMD_RESULT_SUCCESS;
        }
    }
    else 
    {
        cmdResult = TCP_CMD_RESULT_FAILED;
    }
    TcpBaseSendIapDataResult(moduleNo+1,boardNo,cmdResult);
}
  
//在线升级结束      
void AppImplTcpMsgDataProcessIapEnd(NET_COMMAND_PACK *netDataPactPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    TCP_CMD_RESULT cmdResult = TCP_CMD_RESULT_SUCCESS;
    uint8_t moduleNo = netDataPactPtr->bodyData[0] - 1;
    uint8_t boardNo = netDataPactPtr->bodyData[1];
    uint32_t crcResult = TcpBaseUtilConvertBufferToUint32(netDataPactPtr->bodyData,2);
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d,moduleNo: %d,boardNo: %d,crcResult: 0X%08X \r\n",__FUNCTION__,__LINE__,moduleNo,boardNo,crcResult);
    //TCP_MSG只能给中位机自己升级
    if((moduleNo == MODULE_NO_CELL_1)&&(boardNo == BOARD_ID_MID_CONTROL))
    {
        errorCode = BoardIAP_End(IAP_MODE_BOOT,crcResult);
        if(errorCode != LH_ERR_NONE)
        {
            SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
            cmdResult = TCP_CMD_RESULT_FAILED;
        }
        else
        {
            cmdResult = TCP_CMD_RESULT_SUCCESS;
        }
        TcpBaseSendIapEndResult(moduleNo+1,boardNo,cmdResult);
        //延时等待上传完成
        CoreDelayMs(1000);
        //系统复位,复位后会自动跳转到TCP_MSG
        __set_FAULTMASK(1);
        HAL_NVIC_SystemReset();
    }
    else 
    {
        cmdResult = TCP_CMD_RESULT_FAILED;
        TcpBaseSendIapEndResult(moduleNo+1,boardNo,cmdResult);
    }
}

//BOOT下处于中位机BOOT状态
static const SYSTEM_MODULE_STATE modeStateDefault = {TCP_MODULE_STATE_MID_BOOT,{0,37,0,0},{0,5,0,0,},{0,37,0,0}};
//联机
void AppImplTcpMsgDataProcessCmdOnLine(NET_COMMAND_PACK *netDataPactPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //联机时间
    uint64_t utcStamp = TcpBaseUtilConvertBufferToUint64(netDataPactPtr->bodyData,1);
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d ,UTC: %ld\r\n",__FUNCTION__,__LINE__,utcStamp);
    //上传联机状态
    errorCode = TcpBaseSendOnLineResult(TCP_CMD_RESULT_SUCCESS);
    if(errorCode != LH_ERR_NONE)
    {
        SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
    }
    //立刻回传状态心跳
    SYSTEM_MODULE_STATE* moduleStatePtr = (SYSTEM_MODULE_STATE*)(&modeStateDefault);
    TcpBaseSendModuleState(moduleStatePtr,moduleStatePtr,moduleStatePtr,moduleStatePtr,moduleStatePtr);
}

//脱机
void AppImplTcpMsgDataProcessCmdOffLine(NET_COMMAND_PACK *netDataPactPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    errorCode = TcpBaseSendOffLineResult(TCP_CMD_RESULT_SUCCESS);
    if(errorCode != LH_ERR_NONE)
    {
        SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
    }
}

//测试参数
void AppImplTcpMsgDataProcessCmdTestParam(NET_COMMAND_PACK *netDataPactPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    errorCode = TcpBaseSendTestParamConfirm();
    if(errorCode != LH_ERR_NONE)
    {
        SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
    }
}

//调试指令响应
void AppImplTcpMsgDataProcessCmdDebug(NET_COMMAND_PACK *netDataPactPtr)
{
    //打印日志代码
    SystemPrintf("[MID]Boot Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
}
