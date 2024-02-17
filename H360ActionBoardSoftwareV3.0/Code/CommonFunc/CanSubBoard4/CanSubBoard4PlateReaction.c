/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 16:00:11
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-05 13:38:12
**FilePath: \H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard4\CanSubBoard4PlateReaction.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard4PlateReaction.h"

//指令板号
#define BOARD_ID_PLATE_REACTION     CAN_MASTER_SUB_BOARD4

//通道指令
typedef enum CMD_PLATE_REACTION
{
    CMD_PLATE_REACTION_RESET                    = 0X00000000,//反应盘复位
    CMD_PLATE_REACTION_RUN_TO_CUP_POSITIVE      = 0X00000001,//反应盘正向运转到指定序号杯位
    CMD_PLATE_REACTION_RUN_ANY_CUPS_POSITIVE    = 0X00000002,//反应盘正向旋转指定数量杯位
    CMD_PLATE_REACTION_RUN_TO_CUP_NEGATIVE      = 0X00000003,//反应盘反向运转到指定序号杯位
    CMD_PLATE_REACTION_RUN_ANY_CUPS_NEGATIVE    = 0X00000004,//反应盘反向旋转指定数量杯位
    CMD_PLATE_REACTION_STEP_MOTOR_RESET         = 0X00000005,//指定步进电机复位
    CMD_PLATE_REACTION_STEP_MOTOR_RUN_STEPS     = 0X00000006,//指定步进电机走步数
}CMD_PLATE_REACTION;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigPlateReactionArray[] = {
    {BOARD_ID_PLATE_REACTION    ,CAN_MASTER_CHANNEL_BOARD4_PLATE_REACTION ,0  ,60000   ,5 ,CAN_SUB_DATA_MODULE_PLATE_REACTION},//反应盘复位
    {BOARD_ID_PLATE_REACTION    ,CAN_MASTER_CHANNEL_BOARD4_PLATE_REACTION ,1  ,60000   ,5 ,CAN_SUB_DATA_MODULE_PLATE_REACTION},//反应盘正向运转到指定序号杯位
    {BOARD_ID_PLATE_REACTION    ,CAN_MASTER_CHANNEL_BOARD4_PLATE_REACTION ,1  ,60000   ,5 ,CAN_SUB_DATA_MODULE_PLATE_REACTION},//反应盘正向旋转指定数量杯位
    {BOARD_ID_PLATE_REACTION    ,CAN_MASTER_CHANNEL_BOARD4_PLATE_REACTION ,1  ,60000   ,5 ,CAN_SUB_DATA_MODULE_PLATE_REACTION},//反应盘反向运转到指定序号杯位
    {BOARD_ID_PLATE_REACTION    ,CAN_MASTER_CHANNEL_BOARD4_PLATE_REACTION ,1  ,60000   ,5 ,CAN_SUB_DATA_MODULE_PLATE_REACTION},//反应盘反向旋转指定数量杯位
    {BOARD_ID_PLATE_REACTION    ,CAN_MASTER_CHANNEL_BOARD4_PLATE_REACTION ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL                },//指定步进电机复位
    {BOARD_ID_PLATE_REACTION    ,CAN_MASTER_CHANNEL_BOARD4_PLATE_REACTION ,5  ,60000   ,7 ,CAN_BASE_DATA_MODULE_MOTOR        },//指定步进电机走步数
};

//反应盘复位
LH_ERR CanSubBoard4PlateReaction_Reset(PLATE_REACTION_RESULT* plateReactionResult)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_PLATE_REACTION_RESET,cmdCanConfigPlateReactionArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = ;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigPlateReactionArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    plateReactionResult->currentNewCupIndex = (uint8_t)(canMasterReadData.recvDataPtr[2]);
    plateReactionResult->cupExistNewPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[3]);
    plateReactionResult->cupExistOldPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[4]);
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//反应盘正向运转到指定序号杯位
LH_ERR CanSubBoard4PlateReaction_RunToCupWithIndexPositive(uint8_t targetCup,PLATE_REACTION_RESULT* plateReactionResult)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_PLATE_REACTION_RUN_TO_CUP_POSITIVE,cmdCanConfigPlateReactionArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(targetCup);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigPlateReactionArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    plateReactionResult->currentNewCupIndex = (uint8_t)(canMasterReadData.recvDataPtr[2]);
    plateReactionResult->cupExistNewPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[3]);
    plateReactionResult->cupExistOldPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[4]);
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//反应盘正向旋转指定数量杯位
LH_ERR CanSubBoard4PlateReaction_RunAnyCupsPositive(uint8_t cups,PLATE_REACTION_RESULT* plateReactionResult)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_PLATE_REACTION_RUN_ANY_CUPS_POSITIVE,cmdCanConfigPlateReactionArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(cups);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigPlateReactionArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    plateReactionResult->currentNewCupIndex = (uint8_t)(canMasterReadData.recvDataPtr[2]);
    plateReactionResult->cupExistNewPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[3]);
    plateReactionResult->cupExistOldPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[4]);
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//反应盘反向运转到指定序号杯位
LH_ERR CanSubBoard4PlateReaction_RunToCupWithIndexNegative(uint8_t targetCup,PLATE_REACTION_RESULT* plateReactionResult)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_PLATE_REACTION_RUN_TO_CUP_NEGATIVE,cmdCanConfigPlateReactionArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(targetCup);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigPlateReactionArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    plateReactionResult->currentNewCupIndex = (uint8_t)(canMasterReadData.recvDataPtr[2]);
    plateReactionResult->cupExistNewPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[3]);
    plateReactionResult->cupExistOldPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[4]);
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//反应盘反向旋转指定数量杯位
LH_ERR CanSubBoard4PlateReaction_RunAnyCupsNegative(uint8_t cups,PLATE_REACTION_RESULT* plateReactionResult)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_PLATE_REACTION_RUN_ANY_CUPS_NEGATIVE,cmdCanConfigPlateReactionArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(cups);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigPlateReactionArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    plateReactionResult->currentNewCupIndex = (uint8_t)(canMasterReadData.recvDataPtr[2]);
    plateReactionResult->cupExistNewPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[3]);
    plateReactionResult->cupExistOldPos = (SENSOR_STATE)(canMasterReadData.recvDataPtr[4]);
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR CanSubBoard4PlateReaction_StepMotorReset(INDEX_MOTOR_PLATE_REACTION index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_PLATE_REACTION_STEP_MOTOR_RESET,cmdCanConfigPlateReactionArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigPlateReactionArray);
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

//指定步进电机走步数
LH_ERR CanSubBoard4PlateReaction_StepMotorRunSteps(INDEX_MOTOR_PLATE_REACTION index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_PLATE_REACTION_STEP_MOTOR_RUN_STEPS,cmdCanConfigPlateReactionArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigPlateReactionArray);
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
















