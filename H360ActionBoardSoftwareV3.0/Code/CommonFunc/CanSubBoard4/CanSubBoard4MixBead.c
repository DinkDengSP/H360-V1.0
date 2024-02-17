/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 16:00:11
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-04 17:44:18
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\CanSubBoard4\CanSubBoard4MixBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard4MixBead.h"

//指令板号
#define BOARD_ID_MIX_BEAD     CAN_MASTER_SUB_BOARD4

//通道指令
typedef enum CMD_MIX_BEAD
{
    CMD_MIX_BEAD_RESET                 = 0X00000000,//磁珠摇匀复位
    CMD_MIX_BEAD_UP                    = 0X00000001,//磁珠摇匀升起来
    CMD_MIX_BEAD_DOWN                  = 0X00000002,//磁珠摇匀降下去
    CMD_MIX_BEAD_START_ROTATE          = 0X00000003,//磁珠摇匀运行一次,不带自动升降
    CMD_MIX_BEAD_ROTATE_INTEGRATED     = 0X00000004,//磁珠摇匀集成摇匀一次
    CMD_MIX_BEAD_STEP_MOTOR_RESET      = 0X00000005,//磁珠摇匀模块电机复位
    CMD_MIX_BEAD_STEP_MOTOR_RUN_STEPS  = 0X00000006,//磁珠摇匀模块电机走步数
}CMD_MIX_BEAD;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigMixBeadArray[] = {
    {BOARD_ID_MIX_BEAD    ,CAN_MASTER_CHANNEL_BOARD4_MIX_BEAD ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//磁珠摇匀复位
    {BOARD_ID_MIX_BEAD    ,CAN_MASTER_CHANNEL_BOARD4_MIX_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//磁珠摇匀升起来
    {BOARD_ID_MIX_BEAD    ,CAN_MASTER_CHANNEL_BOARD4_MIX_BEAD ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//磁珠摇匀降下去
    {BOARD_ID_MIX_BEAD    ,CAN_MASTER_CHANNEL_BOARD4_MIX_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//磁珠摇匀运行一次,不带自动升降
    {BOARD_ID_MIX_BEAD    ,CAN_MASTER_CHANNEL_BOARD4_MIX_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//磁珠摇匀集成摇匀一次
    {BOARD_ID_MIX_BEAD    ,CAN_MASTER_CHANNEL_BOARD4_MIX_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//磁珠摇匀模块电机复位
    {BOARD_ID_MIX_BEAD    ,CAN_MASTER_CHANNEL_BOARD4_MIX_BEAD ,5  ,60000   ,7 ,CAN_BASE_DATA_MODULE_MOTOR },//磁珠摇匀模块电机走步数
};

//磁珠混匀模块复位,带零位偏移
LH_ERR CanSubBoard4MixBead_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_BEAD_RESET,cmdCanConfigMixBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = ;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠混匀升起来
LH_ERR CanSubBoard4MixBead_Up(MIX_MODE mixMode)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_BEAD_UP,cmdCanConfigMixBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(mixMode);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠混匀升降降下去
LH_ERR CanSubBoard4MixBead_Down(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_BEAD_DOWN,cmdCanConfigMixBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)(mixMode);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠混匀旋转混匀一次,不带自动升降
LH_ERR CanSubBoard4MixBead_StartRotate(MIX_MODE mixMode)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_BEAD_START_ROTATE,cmdCanConfigMixBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(mixMode);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠混匀集成混匀一次
LH_ERR CanSubBoard4MixBead_RotateIntegrated(MIX_MODE mixMode)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_BEAD_ROTATE_INTEGRATED,cmdCanConfigMixBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(mixMode);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR CanSubBoard4MixBead_StepMotorReset(INDEX_MOTOR_MIX_BEAD index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_BEAD_STEP_MOTOR_RESET,cmdCanConfigMixBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机走指定步数
LH_ERR CanSubBoard4MixBead_StepMotorRunSteps(INDEX_MOTOR_MIX_BEAD index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_BEAD_STEP_MOTOR_RUN_STEPS,cmdCanConfigMixBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *posAfterRun = CanBaseReadDataConvertToInt32(canMasterReadData.recvDataPtr,3);

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}



















