/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 16:00:11
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-04 17:37:20
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\CanSubBoard4\CanSubBoard4MixReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard4MixReagent.h"

//指令板号
#define BOARD_ID_MIX_REAGENT     CAN_MASTER_SUB_BOARD4

//通道指令
typedef enum CMD_MIX_REAGENT
{
    CMD_MIX_REAGENT_RESET                 = 0X00000000,//试剂摇匀复位
    CMD_MIX_REAGENT_UP                    = 0X00000001,//试剂摇匀升起来
    CMD_MIX_REAGENT_DOWN                  = 0X00000002,//试剂摇匀降下去
    CMD_MIX_REAGENT_START_ROTATE          = 0X00000003,//试剂摇匀运行一次,不带自动升降
    CMD_MIX_REAGENT_ROTATE_INTEGRATED     = 0X00000004,//试剂摇匀集成摇匀一次
    CMD_MIX_REAGENT_STEP_MOTOR_RESET      = 0X00000005,//试剂摇匀模块电机复位
    CMD_MIX_REAGENT_STEP_MOTOR_RUN_STEPS  = 0X00000006,//试剂摇匀模块电机走步数
}CMD_MIX_REAGENT;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigMixReagentArray[] = {
    {BOARD_ID_MIX_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_MIX_REAGENT ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//试剂摇匀复位
    {BOARD_ID_MIX_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_MIX_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//试剂摇匀升起来
    {BOARD_ID_MIX_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_MIX_REAGENT ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//试剂摇匀降下去
    {BOARD_ID_MIX_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_MIX_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//试剂摇匀运行一次,不带自动升降
    {BOARD_ID_MIX_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_MIX_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//试剂摇匀集成摇匀一次
    {BOARD_ID_MIX_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_MIX_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//试剂摇匀模块电机复位
    {BOARD_ID_MIX_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_MIX_REAGENT ,5  ,60000   ,7 ,CAN_BASE_DATA_MODULE_MOTOR },//试剂摇匀模块电机走步数
};

//试剂混匀模块复位,带零位偏移
LH_ERR CanSubBoard4MixReagent_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_REAGENT_RESET,cmdCanConfigMixReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = ;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixReagentArray);
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

//试剂混匀升起来
LH_ERR CanSubBoard4MixReagent_Up(MIX_MODE mixMode)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_REAGENT_UP,cmdCanConfigMixReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(mixMode);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixReagentArray);
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

//试剂混匀升降降下去
LH_ERR CanSubBoard4MixReagent_Down(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_REAGENT_DOWN,cmdCanConfigMixReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)(mixMode);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixReagentArray);
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

//试剂混匀旋转混匀一次,不带自动升降
LH_ERR CanSubBoard4MixReagent_StartRotate(MIX_MODE mixMode)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_REAGENT_START_ROTATE,cmdCanConfigMixReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(mixMode);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixReagentArray);
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

//试剂混匀集成混匀一次
LH_ERR CanSubBoard4MixReagent_RotateIntegrated(MIX_MODE mixMode)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_REAGENT_ROTATE_INTEGRATED,cmdCanConfigMixReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(mixMode);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixReagentArray);
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
LH_ERR CanSubBoard4MixReagent_StepMotorReset(INDEX_MOTOR_MIX_REAGENT index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_REAGENT_STEP_MOTOR_RESET,cmdCanConfigMixReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixReagentArray);
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
LH_ERR CanSubBoard4MixReagent_StepMotorRunSteps(INDEX_MOTOR_MIX_REAGENT index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MIX_REAGENT_STEP_MOTOR_RUN_STEPS,cmdCanConfigMixReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMixReagentArray);
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













