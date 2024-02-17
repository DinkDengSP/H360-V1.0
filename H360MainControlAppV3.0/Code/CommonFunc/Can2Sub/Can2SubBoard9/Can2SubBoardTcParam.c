/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-02 10:47:07
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-02 10:57:25
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\Can2SubBoardTC\Can2SubBoardTcParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoardTcParam.h"

//读参数指令
#define TC_BOARD_PARAM_READ     0X00000000
//写参数指令
#define TC_BOARD_PARAM_WRITE    0X00000001

//温控读取参数
LH_ERR Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN mainIndex,uint8_t subIndex,float* paramResult)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //计算板卡号码和单元号码
    can2SendCmd.targetBoardID = BOARD_ID_TC;
    can2SendCmd.channelNo = CAN2_CHANNEL_BOARD9_TC_PARAM;
    //申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 2;
    do
    {
        can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,2);
        if(can2SendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(can2SendCmd.paramBuffer == NULL);
    //指令码设定
    can2SendCmd.commandCode = TC_BOARD_PARAM_READ;
    can2SendCmd.timeoutMs = 1000;
    //写入参数
    can2SendCmd.paramBuffer[0] = (uint8_t)mainIndex;
    can2SendCmd.paramBuffer[1] = (uint8_t)subIndex;
    //数据发送
    errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
    if(can2SendCmd.paramBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
    }

    //数据为空
    if(can2ReadData.recvDataPtr == NULL)
    {
        return LH_ERR_BOARD_MAIN_COMM_CAN2_DATA_NULL;
    }
    //接收到的数据不需要
    if(can2ReadData.recvDatLength != 6)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return LH_ERR_BOARD_MAIN_COMM_CAN2_DATA_LENGTH;
    }
    //读取数据
    if(CAN_SUB_DATA_TC_PARAM_READ != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
    {
        //数据标志位错误
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return LH_ERR_BOARD_MAIN_COMM_CAN2_DATA_FLAG;
    }
    //读取通道错误状态
    *paramResult = Can2ProcUtilConvertArrayToFloat(can2ReadData.recvDataPtr,2);
    //释放内存
    UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    return errCode;
}

//温控写入参数
LH_ERR Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN mainIndex,uint8_t subIndex,float paramWrite)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //计算板卡号码和单元号码
    can2SendCmd.targetBoardID = BOARD_ID_TC;
    can2SendCmd.channelNo = CAN2_CHANNEL_BOARD9_TC_PARAM;
    //申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 6;
    do
    {
        can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,6);
        if(can2SendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(can2SendCmd.paramBuffer == NULL);
    //指令码设定
    can2SendCmd.commandCode = TC_BOARD_PARAM_WRITE;
    can2SendCmd.timeoutMs = 1000;
    //写入参数
    can2SendCmd.paramBuffer[0] = mainIndex;
    can2SendCmd.paramBuffer[1] = subIndex;
    //写入浮点数
    Can2ProcUtilConvertFloatDataToArray(can2SendCmd.paramBuffer,2,paramWrite);
    //数据发送
    errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
    if(can2SendCmd.paramBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
    }
    //接收到的数据不需要
    if(can2ReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}














