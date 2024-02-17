/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-04 15:02:00
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-18 20:22:48
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "TcpBase.h"
#include "UserMemManage.h"
#include "CoreUtil.h"
#include "AppStateDataType.h"
#include "AppImplTcpMsg.h"

//数据输出
static TcpSendDataFunc tcpSendDataFuncPtr = NULL;
//日志输出
static TcpLogOutFunc tcpLogOutFuncPtr = NULL;

//网口发送指令回调方法注册
void TcpBaseRegisterFunc(TcpSendDataFunc sendFunc,TcpLogOutFunc logoutFunc)
{
    tcpSendDataFuncPtr = sendFunc;
    tcpLogOutFuncPtr = logoutFunc;
}

//基础API,网口按照协议发送一帧数据出去
LH_ERR TcpBaseSendCommandWithData(uint16_t sendCommand,uint8_t* sendDataPtr,uint16_t sendDataLength)
{
    uint16_t indexUtil = 0;
    TCP_SEND_CMD_DATA* tcpSendCmdDataPtr = NULL;
    //回调函数为空,调用这个没有意义
    if((tcpSendDataFuncPtr == NULL)||(tcpLogOutFuncPtr == NULL))
    {
        return LH_ERR_BOARD_MAIN_COMM_NET_CALL_BACK_NULL;
    }
    //申请结构体内存
    tcpSendCmdDataPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,LENGTH_TCP_SEND_CMD_DATA);
    //数据赋值
    tcpSendCmdDataPtr->command = sendCommand;
    //计算需要的数据量,指令头加上需要发送的数据
    tcpSendCmdDataPtr->dataLength = TCP_CMD_HEADER_SIZE+sendDataLength;
    //申请申请数据内存
    tcpSendCmdDataPtr->dataBufferPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,tcpSendCmdDataPtr->dataLength);
    //数据填充
    //指令前导头
    TcpBaseUtilConvertUint16ToBuffer(TCP_CMD_LEADER_CODE,tcpSendCmdDataPtr->dataBufferPtr,0);
    //数据包指令
    TcpBaseUtilConvertUint16ToBuffer(sendCommand,tcpSendCmdDataPtr->dataBufferPtr,TCP_CMD_HEADER_CMD_OFFSET);
    //指令数据包长度
    TcpBaseUtilConvertUint16ToBuffer(sendDataLength,tcpSendCmdDataPtr->dataBufferPtr,TCP_CMD_HEADER_DLEN_OFFSET);
    //计算数据包校验和
    uint32_t dataCheckSum = 0;
    for (indexUtil = 0; indexUtil < sendDataLength; indexUtil++)
    {
        dataCheckSum += sendDataPtr[indexUtil];
    }
    TcpBaseUtilConvertUint32ToBuffer(dataCheckSum,tcpSendCmdDataPtr->dataBufferPtr,TCP_CMD_DATA_SUM_OFFSET);
    //计算指令头校验和
    uint16_t headerCheckSum = 0;
    for (indexUtil=0; indexUtil<(TCP_CMD_HEADER_SIZE-2); indexUtil++)
    {
        headerCheckSum += tcpSendCmdDataPtr->dataBufferPtr[indexUtil];
    }
    TcpBaseUtilConvertUint16ToBuffer(headerCheckSum,tcpSendCmdDataPtr->dataBufferPtr,TCP_CMD_HEADER_SUM_OFFSET);
    //数据拷贝
    if(sendDataLength != 0)
    {
        UserMemCopy(tcpSendCmdDataPtr->dataBufferPtr+TCP_CMD_HEADER_SIZE,sendDataPtr,sendDataLength);
    }
    //数据发送
    LH_ERR errorCode = tcpSendDataFuncPtr(tcpSendCmdDataPtr);
    //检测是否数据发送成功
    if(errorCode != LH_ERR_NONE)
    {
        //数据发送失败,需要释放本次申请的内存
        UserMemFree(MEM_SDRAM1,tcpSendCmdDataPtr->dataBufferPtr);
        tcpSendCmdDataPtr->dataBufferPtr = NULL;
        UserMemFree(MEM_SDRAM1,tcpSendCmdDataPtr);
        //打印错误输出语句
        tcpLogOutFuncPtr("E>TcpBaseSendCommandWithData Failed, ErrorCode : 0X%08x\r\n",errorCode);
    }
    return errorCode;
}

//回传ACK
LH_ERR TcpBaseSendAck(TCP_NET_ACK ackFlag)
{
    uint8_t ackFlagLocal = ackFlag;
    return TcpBaseSendCommandWithData(TCP_CMD_COMMUNICATE_ACK,&ackFlagLocal,1);
}

//回传系统状态
LH_ERR TcpBaseSendModuleState(SYSTEM_MODULE_STATE* module1State,SYSTEM_MODULE_STATE* module2State,SYSTEM_MODULE_STATE* module3State,
                                    SYSTEM_MODULE_STATE* module4State,SYSTEM_MODULE_STATE* module5State)
{
    //申请内存,存放模块状态回传讯息
    uint16_t sendBufferDataLength = (LENGTH_SYSTEM_MODULE_STATE+1)*SYSTEM_MODULE_COUNT;
    uint8_t* sendBufferPtr = NULL;
    //内存申请
    sendBufferPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,sendBufferDataLength);
    //数据拷贝,只有一个模块,但是协议要求五个模块信息,用这一个模块填充五个模块的信息,上位机只解析一个模块的讯息
    sendBufferPtr[0] = 1;
    sendBufferPtr[(LENGTH_SYSTEM_MODULE_STATE+1)*1] = 2;
    sendBufferPtr[(LENGTH_SYSTEM_MODULE_STATE+1)*2] = 3;
    sendBufferPtr[(LENGTH_SYSTEM_MODULE_STATE+1)*3] = 4;
    sendBufferPtr[(LENGTH_SYSTEM_MODULE_STATE+1)*4] = 5;
    UserMemCopy(sendBufferPtr+1,(void*)module1State,LENGTH_SYSTEM_MODULE_STATE);
    UserMemCopy(sendBufferPtr+((LENGTH_SYSTEM_MODULE_STATE+1)*1)+1,(void*)module2State,LENGTH_SYSTEM_MODULE_STATE);
    UserMemCopy(sendBufferPtr+((LENGTH_SYSTEM_MODULE_STATE+1)*2)+1,(void*)module3State,LENGTH_SYSTEM_MODULE_STATE);
    UserMemCopy(sendBufferPtr+((LENGTH_SYSTEM_MODULE_STATE+1)*3)+1,(void*)module4State,LENGTH_SYSTEM_MODULE_STATE);
    UserMemCopy(sendBufferPtr+((LENGTH_SYSTEM_MODULE_STATE+1)*4)+1,(void*)module5State,LENGTH_SYSTEM_MODULE_STATE);
    //拷贝完成,数据传输
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = TcpBaseSendCommandWithData(TCP_CMD_STATUS,sendBufferPtr,sendBufferDataLength);
    //发送完成,释放内存
    UserMemFree(MEM_SDRAM1,sendBufferPtr);
    //回传错误代码
    return errorCode;
}

//回传联机结果
LH_ERR TcpBaseSendOnLineResult(TCP_CMD_RESULT result)
{
    uint8_t bufferSend[6] = {0};
    //数据拷贝
    bufferSend[0] = (uint8_t)(result);
    //H80模块1是主控,其余模块不存在
    bufferSend[1] = (uint8_t)AppImplConfigGetModuleTypeByID(CAN_CELL_MODULE_1);
    bufferSend[2] = (uint8_t)AppImplConfigGetModuleTypeByID(CAN_CELL_MODULE_2);
    bufferSend[3] = (uint8_t)(DEVICE_MODULE_TYPE_INVALID);
    bufferSend[4] = (uint8_t)(DEVICE_MODULE_TYPE_INVALID);
    bufferSend[5] = (uint8_t)(DEVICE_MODULE_TYPE_INVALID);
    return TcpBaseSendCommandWithData(TCP_CMD_ON_LINE,bufferSend,6);
}

//回传系统各个模块类型,脱机指令回传,返回脱机是否成功
LH_ERR TcpBaseSendOffLineResult(TCP_CMD_RESULT result)
{
    uint8_t bufferSend[6] = {0};
    //数据拷贝
    bufferSend[0] = (uint8_t)(result);
    //H80模块1是主控,其余模块不存在
    bufferSend[1] = (uint8_t)AppImplConfigGetModuleTypeByID(CAN_CELL_MODULE_1);
    bufferSend[2] = (uint8_t)AppImplConfigGetModuleTypeByID(CAN_CELL_MODULE_2);
    bufferSend[3] = (uint8_t)(DEVICE_MODULE_TYPE_INVALID);
    bufferSend[4] = (uint8_t)(DEVICE_MODULE_TYPE_INVALID);
    bufferSend[5] = (uint8_t)(DEVICE_MODULE_TYPE_INVALID);
    return TcpBaseSendCommandWithData(TCP_CMD_OFF_LINE,bufferSend,6);
}

//回传测试参数确认
LH_ERR TcpBaseSendTestParamConfirm(void)
{
    return TcpBaseSendCommandWithData(TCP_CMD_TEST_PARAM,NULL,0);
}

//回传Boot版本号
LH_ERR TcpBaseSendBootSoftwareVersion(uint8_t moduleNo,uint8_t boardNo,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion)
{
    uint8_t bufferSend[9] = {0};
    //写入数据
    bufferSend[0] = moduleNo;
    bufferSend[1] = boardNo;
    bufferSend[2] = mainVersion;
    bufferSend[3] = subVersion;
    bufferSend[4] = debugVersion;
    //SVN版本号
    TcpBaseUtilConvertUint32ToBuffer(svnVersion,bufferSend,5);
    return TcpBaseSendCommandWithData(TCP_CMD_READ_BOOT_VERSION,bufferSend,9);
}

//回传App版本号
LH_ERR TcpBaseSendAppSoftwareVersion(uint8_t moduleNo,uint8_t boardNo,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion)
{
    uint8_t bufferSend[9] = {0};
    //写入数据
    bufferSend[0] = moduleNo;
    bufferSend[1] = boardNo;
    bufferSend[2] = mainVersion;
    bufferSend[3] = subVersion;
    bufferSend[4] = debugVersion;
    //SVN版本号
    TcpBaseUtilConvertUint32ToBuffer(svnVersion,bufferSend,5);
    return TcpBaseSendCommandWithData(TCP_CMD_READ_VERSION,bufferSend,9);
}


//回传升级准备结果
LH_ERR TcpBaseSendIapParpareResult(uint8_t moduleNo,uint8_t boardNo,TCP_CMD_RESULT result)
{
    uint8_t bufferSend[3] = {0};
    //数据拷贝
    bufferSend[0] = (uint8_t)(moduleNo);
    bufferSend[1] = (uint8_t)(boardNo);
    bufferSend[2] = (uint8_t)(result);
    return TcpBaseSendCommandWithData(TCP_CMD_IAP_PREPARE,bufferSend,3);
}

//回传升级开始结果
LH_ERR TcpBaseSendIapStartResult(uint8_t moduleNo,uint8_t boardNo,TCP_CMD_RESULT result)
{
    uint8_t bufferSend[3] = {0};
    //数据拷贝
    bufferSend[0] = (uint8_t)(moduleNo);
    bufferSend[1] = (uint8_t)(boardNo);
    bufferSend[2] = (uint8_t)(result);
    return TcpBaseSendCommandWithData(TCP_CMD_IAP_START,bufferSend,3);
}

//回传升级数据结果
LH_ERR TcpBaseSendIapDataResult(uint8_t moduleNo,uint8_t boardNo,TCP_CMD_RESULT result)
{
    uint8_t bufferSend[3] = {0};
    //数据拷贝
    bufferSend[0] = (uint8_t)(moduleNo);
    bufferSend[1] = (uint8_t)(boardNo);
    bufferSend[2] = (uint8_t)(result);
    return TcpBaseSendCommandWithData(TCP_CMD_IAP_DATA,bufferSend,3);
}

//回传升级结束结果
LH_ERR TcpBaseSendIapEndResult(uint8_t moduleNo,uint8_t boardNo,TCP_CMD_RESULT result)
{
    uint8_t bufferSend[3] = {0};
    //数据拷贝
    bufferSend[0] = (uint8_t)(moduleNo);
    bufferSend[1] = (uint8_t)(boardNo);
    bufferSend[2] = (uint8_t)(result);
    return TcpBaseSendCommandWithData(TCP_CMD_IAP_END,bufferSend,3);
}

//向上位机上报数据
LH_ERR TcpNetSendDataToPc(uint16_t cmd,uint8_t *u8Body,uint16_t bodyLen)
{
    return TcpBaseSendCommandWithData(cmd,u8Body,bodyLen);
}

