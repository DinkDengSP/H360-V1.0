/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-05 11:59:04
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-05 13:30:52
**FilePath: \H360ActionBoardSoftwareV2.3\CommonFunc\CanSubSystem\CanSubBase.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBase.h"


//初始化一个发送接收结构体
void CanSubBoardBase_CreateSendBuffer(CAN_MASTER_SEND_CMD* canSendCmdPtr,CAN_MASTER_READ_DATA* canReadDataPtr,uint32_t cmd,const CMD_CAN_CONFIG_UNIT* configArrayPtr)
{
    //初始化接收器
    canReadDataPtr->recvDataPtr = NULL;
    canReadDataPtr->recvDatLength = 0;
    //指令码与超时时间设定
    canSendCmdPtr->commandCode = cmd;
    canSendCmdPtr->timeoutMs = configArrayPtr[canSendCmdPtr->commandCode].sendDataTimeOutMs;
    //计算板卡号码和单元号码
    canSendCmdPtr->targetBoardID = configArrayPtr[canSendCmdPtr->commandCode].targetBoardID;
    canSendCmdPtr->channelNo = configArrayPtr[canSendCmdPtr->commandCode].channelNo;
    //申请内存
    canSendCmdPtr->paramBuffer = NULL;//先初始化为空
    canSendCmdPtr->paramLength = configArrayPtr[canSendCmdPtr->commandCode].sendDataLength;
    if(canSendCmdPtr->paramLength != 0)
    {
        do
        {
            //申请内存
            canSendCmdPtr->paramBuffer = UserMemMalloc(SRAM_IN,canSendCmdPtr->paramLength);
            //必须申请到
            if(canSendCmdPtr->paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(canSendCmdPtr->paramBuffer == NULL);
    }
}

//发送指令并初步解析结果
LH_ERR CanSubBoardBase_SendAndProcRespBase(CAN_MASTER_SEND_CMD* canSendCmdPtr,CAN_MASTER_READ_DATA* canReadDataPtr,const CMD_CAN_CONFIG_UNIT* configArrayPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //数据发送
    errCode = CanMasterSendCommandWhileReturn(canSendCmdPtr,canReadDataPtr);
    //发送完成,释放发送内存
    if(canSendCmdPtr->paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canSendCmdPtr->paramBuffer);
    }
    //首先看指令是否出错,也就是发送是否完成
    if(errCode != LH_ERR_NONE)
    {
        //错误产生
        if(canReadDataPtr->recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(SRAM_IN,canReadDataPtr->recvDataPtr);
        }
        return errCode;
    }
    //查看是否需要返回数据包,如果需要返回却没收到,报错,否则就可以直接返回结果了
    if(configArrayPtr[canSendCmdPtr->commandCode].needRespDataMinLength == 0)
    {
        if(canReadDataPtr->recvDataPtr != NULL)
        {
            UserMemFree(SRAM_IN,canReadDataPtr->recvDataPtr);//释放内存
        }
        return LH_ERR_NONE;
    }
    //到这里,就是需要返回数据的,如果没收到返回数据,报错
    if(canReadDataPtr->recvDataPtr == NULL)
    {
        return LH_ERR_CAN_MASTER_BASE_DATA_NULL;
    }
    //检查返回数据的长度,必须与配置对的上
    if(canReadDataPtr->recvDatLength < configArrayPtr[canSendCmdPtr->commandCode].needRespDataMinLength)
    {
        //释放接收内存
        UserMemFree(SRAM_IN,canReadDataPtr->recvDataPtr);
        //释放内存
        return LH_ERR_CAN_MASTER_BASE_DATA_LENGTH;
    }
    //检查返回数据的数据标识,必须和配置对的上
    if(configArrayPtr[canSendCmdPtr->commandCode].respDataFlag != CanBaseReadDataConvertToUint16(canReadDataPtr->recvDataPtr,0))
    {
        //释放接收内存
        UserMemFree(SRAM_IN,canReadDataPtr->recvDataPtr);
        //数据标志位错误
        return LH_ERR_CAN_MASTER_BASE_DATA_FLAG;
    }
    return LH_ERR_NONE;
}

