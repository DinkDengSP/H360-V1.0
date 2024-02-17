/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:51:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-04 16:05:56
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\CanSubBoard1\CanSubBoard1NeedleWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard1NeedleWash.h"

//指令板号
#define BOARD_ID_NEEDLE_WASH     CAN_MASTER_SUB_BOARD1

//通道指令
typedef enum CMD_NEEDLE_WASH
{
    CMD_NEEDLE_WASH_RESET                   = 0X00000000,//清洗针复位
    CMD_NEEDLE_WASH_PRIME                   = 0X00000001,//清洗针灌注
    CMD_NEEDLE_WASH_MAINTAIN_CLEAN          = 0X00000002,//清洗针清洗维护
    CMD_NEEDLE_WASH_BEAD_CLEAN              = 0X00000003,//清洗针磁珠清洗
    CMD_NEEDLE_WASH_STEP_MOTOR_RESET        = 0X00000004,//清洗针模块电机复位
    CMD_NEEDLE_WASH_STEP_MOTOR_RUN_STEPS    = 0X00000005,//清洗针模块电机走步数
}CMD_NEEDLE_WASH;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigNeedleWashArray[] = {
    {BOARD_ID_NEEDLE_WASH    ,CAN_MASTER_CHANNEL_BOARD1_NEEDLE_WASH ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针复位
    {BOARD_ID_NEEDLE_WASH    ,CAN_MASTER_CHANNEL_BOARD1_NEEDLE_WASH ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针灌注
    {BOARD_ID_NEEDLE_WASH    ,CAN_MASTER_CHANNEL_BOARD1_NEEDLE_WASH ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针清洗维护
    {BOARD_ID_NEEDLE_WASH    ,CAN_MASTER_CHANNEL_BOARD1_NEEDLE_WASH ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针磁珠清洗
    {BOARD_ID_NEEDLE_WASH    ,CAN_MASTER_CHANNEL_BOARD1_NEEDLE_WASH ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针模块电机复位
    {BOARD_ID_NEEDLE_WASH    ,CAN_MASTER_CHANNEL_BOARD1_NEEDLE_WASH ,5  ,60000   ,7 ,CAN_BASE_DATA_MODULE_MOTOR },//清洗针模块电机走步数
};

//清洗针复位,带零位偏移
LH_ERR CanSubBoard1NeedleWash_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_WASH_RESET,cmdCanConfigNeedleWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleWashArray);
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

//清洗针灌注
LH_ERR CanSubBoard1NeedleWash_Prime(uint8_t primeCount)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_WASH_PRIME,cmdCanConfigNeedleWashArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(primeCount);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleWashArray);
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

//清洗针液路维护
LH_ERR CanSubBoard1NeedleWash_MaintainClean(uint8_t maintainCount)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_WASH_MAINTAIN_CLEAN,cmdCanConfigNeedleWashArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(maintainCount);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleWashArray);
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

//清洗针执行一次清洗
LH_ERR CanSubBoard1NeedleWash_BeadClean(NEEDLE_CLEAN_CONFIG cleanConfig)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_WASH_BEAD_CLEAN,cmdCanConfigNeedleWashArray);
    //如果存在数据需要传送,在这里填充数据
    uint8_t cleanConfigByte = 0;
    //数据转换
    cleanConfigByte += ((cleanConfig.needle5InjectEnable != 0)?0x01:0x00);
    cleanConfigByte <<= 1;
    cleanConfigByte += ((cleanConfig.needle4InjectEnable != 0)?0x01:0x00);
    cleanConfigByte <<= 1;
    cleanConfigByte += ((cleanConfig.needle3InjectEnable != 0)?0x01:0x00);
    cleanConfigByte <<= 1;
    cleanConfigByte += ((cleanConfig.needle2InjectEnable != 0)?0x01:0x00);
    cleanConfigByte <<= 1;
    cleanConfigByte += ((cleanConfig.needle1InjectEnable != 0)?0x01:0x00);
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(cleanConfigByte);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleWashArray);
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
LH_ERR CanSubBoard1NeedleWash_StepMotorReset(INDEX_MOTOR_NEEDLE_WASH index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_WASH_STEP_MOTOR_RESET,cmdCanConfigNeedleWashArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleWashArray);
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
LH_ERR CanSubBoard1NeedleWash_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_WASH index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_WASH_STEP_MOTOR_RUN_STEPS,cmdCanConfigNeedleWashArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleWashArray);
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



