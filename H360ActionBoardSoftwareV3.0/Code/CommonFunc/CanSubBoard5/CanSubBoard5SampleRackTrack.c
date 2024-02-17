/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 16:02:28
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-29 13:09:58
**FilePath: \H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard5\CanSubBoard5SampleRackTrack.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard5SampleRackTrack.h"

//指令板号
#define BOARD_ID_SAMPLE_RACK_TRACK  CAN_MASTER_SUB_BOARD5

//通道指令
typedef enum CMD_SAMPLE_RACK_TRACK
{
    CMD_SAMPLE_RACK_TRACK_RESET                     = 0x00000000,//试管架传送带复位
    CMD_SAMPLE_RACK_TRACK_MOVE_TO_TUBE              = 0x00000001,//试管架传送带移动到指定试管位
    CMD_SAMPLE_RACK_TRACK_BACK_FULL_SAMPLE_RACK     = 0X00000002,//试管架传送带回退一整个试管架
    CMD_SAMPLE_RACK_TRACK_STEP_MOTOR_RESET          = 0X00000003,//试管架传送带指定步进电机复位
    CMD_SAMPLE_RACK_TRACK_STEP_MOTOR_RUN_STEPS      = 0X00000004,//试管架传送带指定步进电机走指定步数
    CMD_SAMPLE_RACK_TRACK_ENCODER_CORRECT           = 0X00000005,//试管架传送带齿差修正
}CMD_MEARURE_ROOM;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigSampleRackTrackArray[] = {
    {BOARD_ID_SAMPLE_RACK_TRACK    ,CAN_MASTER_CHANNEL_BOARD5_SAMPLE_RACK_TRACK ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                       },//试管架传送带复位
    {BOARD_ID_SAMPLE_RACK_TRACK    ,CAN_MASTER_CHANNEL_BOARD5_SAMPLE_RACK_TRACK ,2  ,60000   ,3   ,CAN_SUB_DATA_MODULE_SAMPLE_RACK_TRACK    },//试管架传送带移动到指定试管位  
    {BOARD_ID_SAMPLE_RACK_TRACK    ,CAN_MASTER_CHANNEL_BOARD5_SAMPLE_RACK_TRACK ,0  ,60000   ,3   ,CAN_SUB_DATA_MODULE_SAMPLE_RACK_TRACK    },//试管架传送带回退一整个试管架          
    {BOARD_ID_SAMPLE_RACK_TRACK    ,CAN_MASTER_CHANNEL_BOARD5_SAMPLE_RACK_TRACK ,1  ,60000   ,0   ,CAN_BASE_DATA_NULL                       },//试管架传送带指定步进电机复位
    {BOARD_ID_SAMPLE_RACK_TRACK    ,CAN_MASTER_CHANNEL_BOARD5_SAMPLE_RACK_TRACK ,5  ,60000   ,7   ,CAN_BASE_DATA_MODULE_MOTOR               },//试管架传送带指定步进电机走指定步数
    {BOARD_ID_SAMPLE_RACK_TRACK    ,CAN_MASTER_CHANNEL_BOARD5_SAMPLE_RACK_TRACK ,1  ,60000   ,0   ,CAN_BASE_DATA_NULL                       },//试管架传送带齿差修正
};    


//加样传送带复位
LH_ERR CanSubBoard5SampleRackTrack_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_SAMPLE_RACK_TRACK_RESET,cmdCanConfigSampleRackTrackArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigSampleRackTrackArray);
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

//加样传送带移动到指定试管位
LH_ERR CanSubBoard5SampleRackTrack_Move2Tube(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex,RACK_TUBE_INDEX* currentTube)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_SAMPLE_RACK_TRACK_MOVE_TO_TUBE,cmdCanConfigSampleRackTrackArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)rackPos;
    canMasterSendCmd.paramBuffer[0] = (uint8_t)tubeIndex;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigSampleRackTrackArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentTube = (RACK_TUBE_INDEX)(canMasterReadData.recvDataPtr[2]);

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//加样传送带回退一整个试管架
LH_ERR CanSubBoard5SampleRackTrack_BackFullSampleRack(RACK_TUBE_INDEX* currentTube)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_SAMPLE_RACK_TRACK_BACK_FULL_SAMPLE_RACK,cmdCanConfigSampleRackTrackArray);
    //如果存在数据需要传送,在这里填充数据
    

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigSampleRackTrackArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentTube = (RACK_TUBE_INDEX)(canMasterReadData.recvDataPtr[2]);

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR CanSubBoard5SampleRackTrack_StepMotorReset(INDEX_MOTOR_SAMPLE_RACK_TRACK index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_SAMPLE_RACK_TRACK_STEP_MOTOR_RESET,cmdCanConfigSampleRackTrackArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index); 

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigSampleRackTrackArray);
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
LH_ERR CanSubBoard5SampleRackTrack_StepMotorRunSteps(INDEX_MOTOR_SAMPLE_RACK_TRACK index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_SAMPLE_RACK_TRACK_STEP_MOTOR_RESET,cmdCanConfigSampleRackTrackArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps); 

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigSampleRackTrackArray);
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

//加样传送带齿差修正
LH_ERR CanSubBoard5SampleRackTrack_EncoderCorrect(uint8_t correctCount)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_SAMPLE_RACK_TRACK_ENCODER_CORRECT,cmdCanConfigSampleRackTrackArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(correctCount); 

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigSampleRackTrackArray);
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





















