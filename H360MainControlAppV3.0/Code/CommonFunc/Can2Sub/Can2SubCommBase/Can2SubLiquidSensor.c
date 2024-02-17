/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-04-15 10:25:40
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-15 10:34:17
**FilePath: \Code\CommonFunc\Can2SubCommBase\Can2SubLiquidSensor.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubLiquidSensor.h"
#include "Can2SubOutputIndex.h"

//读取传感器脉冲数
LH_ERR Can2SubLiquidSensorGetCount(uint32_t* liquidCount)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;

    can2SendCmd.targetBoardID = CAN2_SUB_BOARD1;
    can2SendCmd.channelNo = CAN2_CHANNEL_SERIAL1;

    //申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 0;

    //指令码设定
    can2SendCmd.commandCode = 0x00000003;
    can2SendCmd.timeoutMs = 1000;
    
    //数据发送
    errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
    if(can2SendCmd.paramBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
    }
    //首先看指令是否出错
    if(errCode != LH_ERR_NONE)
    {
        //错误产生
        if(can2ReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        }
        return errCode;
    }
    //数据为空
    if(can2ReadData.recvDataPtr == NULL)
    {
        return LH_ERR_BOARD_MAIN_COMM_SERIAL_DATA_NULL;
    }
    //接收到的数据不需要
    if(can2ReadData.recvDatLength != 6)//2 + 4 
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return LH_ERR_BOARD_MAIN_COMM_SERIAL_DATA_LENGTH;
    }
    //读取数据
    if(CAN2_SUB_DATA_SERIAL_RS1 != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
    {
        //数据标志位错误
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return LH_ERR_BOARD_MAIN_COMM_SERIAL_DATA_FLAG;
    }

    //读取结果
    *liquidCount = Can2ProcUtilConvertArrayToUint32(can2ReadData.recvDataPtr,2);
    
    //释放内存
    UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    return errCode;
}

//清除传感器脉冲数
LH_ERR Can2SubLiquidSensorClearCount(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;

    can2SendCmd.targetBoardID = CAN2_SUB_BOARD1;
    can2SendCmd.channelNo = CAN2_CHANNEL_SERIAL1;
    
    //申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 0;
    
    //指令码设定
    can2SendCmd.commandCode = 0x00000004;
    can2SendCmd.timeoutMs = 1000;
    
    //数据发送
    errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
    if(can2SendCmd.paramBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
    }
    //首先看指令是否出错
    if(errCode != LH_ERR_NONE)
    {
        //错误产生
        if(can2ReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        }
        return errCode;
    }
    //不需要返回数据
    if(can2ReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }

    return errCode;
}

//注册指定IO口自动关闭
LH_ERR Can2SubLiquidSensorRegisterPortOutOff(uint32_t trigCount)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;

    can2SendCmd.targetBoardID = CAN2_SUB_BOARD1;
    can2SendCmd.channelNo = CAN2_CHANNEL_SERIAL1;

    //申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 4;
    do
    {
        can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,can2SendCmd.paramLength);
        if(can2SendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(can2SendCmd.paramBuffer == NULL);
    //指令码设定
    can2SendCmd.commandCode = 0x00000005;
    can2SendCmd.timeoutMs = 1000;
    //写入参数
    Can2ProcUtilConvertUint32ToArray(can2SendCmd.paramBuffer,0,trigCount);
    
    //数据发送
    errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
    if(can2SendCmd.paramBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
    }
    //首先看指令是否出错
    if(errCode != LH_ERR_NONE)
    {
        //错误产生
        if(can2ReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        }
        return errCode;
    }
    //接收到的数据不需要
    if(can2ReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//读取触发标志
LH_ERR Can2SubLiquidSensorReadTrigFlag(uint8_t* trigFlag)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;

    can2SendCmd.targetBoardID = CAN2_SUB_BOARD1;
    can2SendCmd.channelNo = CAN2_CHANNEL_SERIAL1;

    //申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 0;

    //指令码设定
    can2SendCmd.commandCode = 0x00000006;
    can2SendCmd.timeoutMs = 1000;
    
    //数据发送
    errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
    if(can2SendCmd.paramBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
    }
    //首先看指令是否出错
    if((errCode != LH_ERR_NONE)&&(errCode != LH_ERR_SERVICE_SERIAL_LIQUID_TRIG_NONE))
    {
        //错误产生
        if(can2ReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        }
        return errCode;
    }
    if(errCode == LH_ERR_SERVICE_SERIAL_LIQUID_TRIG_NONE)
    {
        *trigFlag = 0;
    }
    else
    {
        *trigFlag = 1;
    }
    //释放内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return LH_ERR_NONE;
}
