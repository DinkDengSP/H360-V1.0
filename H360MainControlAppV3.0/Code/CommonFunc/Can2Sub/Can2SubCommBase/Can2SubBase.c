/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-04 10:59:31
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-17 20:11:31
**FilePath: \Code\CommonFunc\Can2SubCommBase\Can2SubBase.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBase.h"

//初始化一个发送接收结构体
void Can2SubBoard_CreateSendBuffer(CAN2_SEND_CMD* can2SendCmdPtr,CAN2_READ_DATA* can2ReadDataPtr,uint32_t cmd,const CMD_CAN2_CONFIG_UNIT* configArrayPtr)
{
    //初始化接收器
    can2ReadDataPtr->recvDataPtr = NULL;
    can2ReadDataPtr->recvDatLength = 0;
    //指令码与超时时间设定
    can2SendCmdPtr->commandCode = cmd;
    can2SendCmdPtr->timeoutMs = configArrayPtr[can2SendCmdPtr->commandCode].sendDataTimeOutMs;
    //计算板卡号码和单元号码
    can2SendCmdPtr->targetBoardID = configArrayPtr[can2SendCmdPtr->commandCode].targetBoardID;
    can2SendCmdPtr->channelNo = configArrayPtr[can2SendCmdPtr->commandCode].channelNo;
    //申请内存
    can2SendCmdPtr->paramBuffer = NULL;//先初始化为空
    can2SendCmdPtr->paramLength = configArrayPtr[can2SendCmdPtr->commandCode].sendDataLength;
    if(can2SendCmdPtr->paramLength != 0)
    {
        do
        {
            //申请内存
            can2SendCmdPtr->paramBuffer = UserMemMalloc(MEM_AXI_SRAM,can2SendCmdPtr->paramLength);
            //必须申请到
            if(can2SendCmdPtr->paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmdPtr->paramBuffer == NULL);
    }
}

//发送指令并初步解析结果
LH_ERR Can2SubBoard_SendAndProcRespBase(CAN2_SEND_CMD* can2SendCmdPtr,CAN2_READ_DATA* can2ReadDataPtr,const CMD_CAN2_CONFIG_UNIT* configArrayPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //数据发送
    errCode = Can2SendCommandWhileReturn(can2SendCmdPtr,can2ReadDataPtr);
    //发送完成,释放发送内存
    if(can2SendCmdPtr->paramBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2SendCmdPtr->paramBuffer);
    }
    //首先看指令是否出错,也就是发送是否完成
    if(errCode != LH_ERR_NONE)
    {
        //错误产生
        if(can2ReadDataPtr->recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadDataPtr->recvDataPtr);
        }
        return errCode;
    }
    //查看是否需要返回数据包,如果需要返回却没收到,报错,否则就可以直接返回结果了
    if(configArrayPtr[can2SendCmdPtr->commandCode].needRespDataMinLength == 0)
    {
        if(can2ReadDataPtr->recvDataPtr != NULL)
        {
            UserMemFree(MEM_AXI_SRAM,can2ReadDataPtr->recvDataPtr);//释放内存
        }
        return LH_ERR_NONE;
    }
    //到这里,就是需要返回数据的,如果没收到返回数据,报错
    if(can2ReadDataPtr->recvDataPtr == NULL)
    {
        return LH_ERR_BOARD_MAIN_COMM_CAN2_DATA_NULL;
    }
    //检查返回数据的长度,必须与配置对的上
    if(can2ReadDataPtr->recvDatLength < configArrayPtr[can2SendCmdPtr->commandCode].needRespDataMinLength)
    {
        //释放接收内存
        UserMemFree(MEM_AXI_SRAM,can2ReadDataPtr->recvDataPtr);
        //释放内存
        return LH_ERR_BOARD_MAIN_COMM_CAN2_DATA_LENGTH;
    }
    //检查返回数据的数据标识,必须和配置对的上
    if(configArrayPtr[can2SendCmdPtr->commandCode].respDataFlag != Can2ProcUtilConvertArrayToUint16(can2ReadDataPtr->recvDataPtr,0))
    {
        //释放接收内存
        UserMemFree(MEM_AXI_SRAM,can2ReadDataPtr->recvDataPtr);
        //数据标志位错误
        return LH_ERR_BOARD_MAIN_COMM_CAN2_DATA_FLAG;
    }
    return LH_ERR_NONE;
}



