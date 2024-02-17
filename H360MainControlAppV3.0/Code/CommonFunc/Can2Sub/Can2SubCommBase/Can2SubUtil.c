/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-02 01:21:49
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-15 13:19:41
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\Can2SubCommBase\Can2SubUtil.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubUtil.h"

//辅助动作的指令
typedef enum CAN2_SUB_UTIL_CMD_CODE
{
    //APP可以接受的指令
    CAN2_SUB_UTIL_CMD_READ_MODE     = 0x00000000,
    CAN2_SUB_UTIL_CMD_READ_VER      = 0x00000001,
    CAN2_SUB_UTIL_CMD_JMP_BOOT      = 0x00000002,
    CAN2_SUB_UTIL_CMD_JMP_APP       = 0x00000002,
    CAN2_SUB_UTIL_CMD_SET_MODE      = 0X00000003,
    CAN2_SUB_UTIL_CMD_PROG_LENGTH   = 0x00000003,
    CAN2_SUB_UTIL_CMD_PROG_DATA     = 0x00000004,
    CAN2_SUB_UTIL_CMD_PROG_CRC_CHECK= 0x00000005,
    CAN2_SUB_UTIL_CMD_PROG_START    = 0x00000006,
}CAN2_SUB_UTIL_CMD_CODE;

//握手
LH_ERR Can2SubUtil_HandShake(CAN2_SUB_BOARD boardID,MODE_RUNNING mode)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //板号通道号
    can2SendCmd.targetBoardID = boardID;
    can2SendCmd.channelNo = CAN2_CHANNEL_IAP;
    do
    {
        //申请内存
        can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,1);
        //必须申请到
        if(can2SendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(can2SendCmd.paramBuffer == NULL);
    //不需要申请内存
    can2SendCmd.paramBuffer[0] = (uint8_t)(mode);
    can2SendCmd.paramLength = 1;
    //设置信道和超时时间
    can2SendCmd.commandCode = CAN2_SUB_UTIL_CMD_SET_MODE;
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
    if(can2ReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return LH_ERR_NONE;
}

//获取软件版本
LH_ERR Can2SubUtil_ReadSoftwareVersion(CAN2_SUB_BOARD boardID,READ_SOFTWARE_VERSION readSoftwareVersion,uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    uint8_t txBuffer[1] = {0};
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //板号通道号
    can2SendCmd.targetBoardID = boardID;
    can2SendCmd.channelNo = CAN2_CHANNEL_IAP;
    //参数
    txBuffer[0] = (uint8_t)readSoftwareVersion;
    can2SendCmd.paramBuffer = txBuffer;
    can2SendCmd.paramLength = 1;
    //设置信道和超时时间
    can2SendCmd.commandCode = CAN2_SUB_UTIL_CMD_READ_VER;
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
        return LH_ERR_BOARD_MAIN_COMM_IAP_DATA_NULL;
    }
    //接收到的数据不需要
    if(can2ReadData.recvDatLength != 9)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return LH_ERR_BOARD_MAIN_COMM_IAP_DATA_LENGTH;
    }
    //读取数据
    if(CAN2_SUB_DATA_SOFTVERSION != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
    {
        //数据标志位错误
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return LH_ERR_BOARD_MAIN_COMM_IAP_DATA_FLAG;
    }
    *mainVersion = can2ReadData.recvDataPtr[2];
    *subVersion = can2ReadData.recvDataPtr[3];
    *debugVersion = can2ReadData.recvDataPtr[4];
    //读取板本
    *svnVersion = Can2ProcUtilConvertArrayToUint32(can2ReadData.recvDataPtr,5);
    //释放内存
    UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    return LH_ERR_NONE;
}

//查询当前动作板处于何种模式
LH_ERR Can2SubActionBoardReadCurrentState(CAN2_SUB_BOARD boardID,ACTION_BOARD_CURRENT_STATE* state)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //板号通道号
    can2SendCmd.targetBoardID = boardID;
    can2SendCmd.channelNo = CAN2_CHANNEL_IAP;
    //不需要申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 0;
    //设置信道和超时时间
    can2SendCmd.commandCode = CAN2_SUB_UTIL_CMD_READ_MODE;
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
        return LH_ERR_BOARD_MAIN_COMM_IAP_DATA_NULL;
    }
    //接收到的数据不需要
    if(can2ReadData.recvDatLength != 3)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return LH_ERR_BOARD_MAIN_COMM_IAP_DATA_LENGTH;
    }
    //读取数据
    if(CAN2_SUB_DATA_IAP_FLAG != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
    {
        //数据标志位错误
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return LH_ERR_BOARD_MAIN_COMM_IAP_DATA_FLAG;
    }
    //读取状态
    *state = (ACTION_BOARD_CURRENT_STATE)(can2ReadData.recvDataPtr[2]);
    //释放内存
    UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    return LH_ERR_NONE;
}

//动作板跳转进入BOOT
LH_ERR Can2SubActionBoardJumpBoot(CAN2_SUB_BOARD boardID)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //板号通道号
    can2SendCmd.targetBoardID = boardID;
    can2SendCmd.channelNo = CAN2_CHANNEL_IAP;
    //不需要申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 0;
    //设置信道和超时时间
    can2SendCmd.commandCode = CAN2_SUB_UTIL_CMD_JMP_BOOT;
    can2SendCmd.timeoutMs = 1000;
    //数据发送
    errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);

    //发送完成,释放发送内存
    if(can2SendCmd.paramBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
    }

    if(can2ReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    //首先看指令是否出错
    if(errCode != LH_ERR_NONE)
    {
        if(errCode == LH_ERR_BOARD_MAIN_COMM_CAN2_RETURN_TIME_OUT)
        {
            //返回超时,运行成功
            return LH_ERR_NONE;
        }
        return errCode;
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//动作板跳转进入APP
LH_ERR Can2SubActionBoardJumpApp(CAN2_SUB_BOARD boardID)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //板号通道号
    can2SendCmd.targetBoardID = boardID;
    can2SendCmd.channelNo = CAN2_CHANNEL_IAP;
    //不需要申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 0;
    //设置信道和超时时间
    can2SendCmd.commandCode = CAN2_SUB_UTIL_CMD_JMP_APP;
    can2SendCmd.timeoutMs = 1000;
    //数据发送
    errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);

    //发送完成,释放发送内存
    if(can2SendCmd.paramBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
    }
    
    if(can2ReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    //首先看指令是否出错
    if(errCode != LH_ERR_NONE)
    {
        if(errCode == LH_ERR_BOARD_MAIN_COMM_CAN2_RETURN_TIME_OUT)
        {
            //返回超时,运行成功
            return LH_ERR_NONE;
        }
        return errCode;
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//传输开始编程的信息
LH_ERR Can2SubActionBoardSetProgDataLength(CAN2_SUB_BOARD boardID,uint32_t pragDataLengthBytes)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //板号通道号
    can2SendCmd.targetBoardID = boardID;
    can2SendCmd.channelNo = CAN2_CHANNEL_IAP;
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
    //设置信道和超时时间
    can2SendCmd.commandCode = CAN2_SUB_UTIL_CMD_PROG_LENGTH;
    can2SendCmd.timeoutMs = 60000;
    //写入参数
    Can2ProcUtilConvertUint32ToArray(can2SendCmd.paramBuffer,0,pragDataLengthBytes);
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
    //释放内存
    UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    return LH_ERR_NONE;
}

//传输编程数据
LH_ERR Can2SubActionBoardSetProgData(CAN2_SUB_BOARD boardID,uint16_t dataLength,uint8_t* dataBuffer)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //板号通道号
    can2SendCmd.targetBoardID = boardID;
    can2SendCmd.channelNo = CAN2_CHANNEL_IAP;
    //申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = dataLength;
    do
    {
        can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,can2SendCmd.paramLength);
        if(can2SendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(can2SendCmd.paramBuffer == NULL);
    //设置信道和超时时间
    can2SendCmd.commandCode = CAN2_SUB_UTIL_CMD_PROG_DATA;
    can2SendCmd.timeoutMs = 60000;
    //写入参数
    UserMemCopy(can2SendCmd.paramBuffer,dataBuffer,dataLength);
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
    //释放内存
    UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    return LH_ERR_NONE;
}

//CRC校验
LH_ERR Can2SubActionBoardSetProgCRC(CAN2_SUB_BOARD boardID,uint32_t crcValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //板号通道号
    can2SendCmd.targetBoardID = boardID;
    can2SendCmd.channelNo = CAN2_CHANNEL_IAP;
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
    //设置信道和超时时间
    can2SendCmd.commandCode = CAN2_SUB_UTIL_CMD_PROG_CRC_CHECK;
    can2SendCmd.timeoutMs = 60000;
    //写入参数
    Can2ProcUtilConvertUint32ToArray(can2SendCmd.paramBuffer,0,crcValue);
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
    //释放内存
    UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    return LH_ERR_NONE;
}

//开始升级
LH_ERR Can2SubActionBoardStartPorg(CAN2_SUB_BOARD boardID)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    can2ReadData.recvDataPtr = NULL;
    can2ReadData.recvDatLength = 0;
    //板号通道号
    can2SendCmd.targetBoardID = boardID;
    can2SendCmd.channelNo = CAN2_CHANNEL_IAP;
    //申请内存
    can2SendCmd.paramBuffer = NULL;
    can2SendCmd.paramLength = 0;
    //设置信道和超时时间
    can2SendCmd.commandCode = CAN2_SUB_UTIL_CMD_PROG_START;
    can2SendCmd.timeoutMs = 60000;
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
    //释放内存
    UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    return LH_ERR_NONE;
}
