/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:54:23
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-15 09:32:30
**FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard1\CanSubBoard1LiquidAuto.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard1LiquidAuto.h"

//指令板号
#define BOARD_ID_LIQUID_AUTO     CAN_MASTER_SUB_BOARD1

//通道指令
typedef enum CMD_LIQUID_AUTO
{
    CMD_LIQUID_AUTO_INIT                                                = 0X00000000,//液路自动化单元初始化
    CMD_LIQUID_AUTO_FORCE_TRIG_WASTE_LIQUID                             = 0X00000001,//强制执行一次抽废液
    CMD_LIQUID_AUTO_ENABLE_WASTE_LIQUID_AUTOMATIC                       = 0X00000002,//打开自动抽废液功能
    CMD_LIQUID_AUTO_DISABLE_WASTE_LIQUID_AUTOMATIC                      = 0X00000003,//关闭自动抽废液功能
    CMD_LIQUID_AUTO_CHECK_STATE_WASTE_LIQUID                            = 0X00000004,//查看自动抽废液状态
    CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_THREE_NEEDLE_CLEAN            = 0X00000005,//关闭三针清洗液稀释液路
    CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_THREE_NEEDLE_CLEAN             = 0X00000006,//打开三针清洗液稀释液路
}CMD_LIQUID_AUTO;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigLiquidAutomaticArray[] = {
    {BOARD_ID_LIQUID_AUTO    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_AUTOMATIC ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL},//液路自动化单元初始化
    {BOARD_ID_LIQUID_AUTO    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_AUTOMATIC ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL},//强制执行一次抽废液
    {BOARD_ID_LIQUID_AUTO    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_AUTOMATIC ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL},//打开自动抽废液功能
    {BOARD_ID_LIQUID_AUTO    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_AUTOMATIC ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL},//关闭自动抽废液功能
    {BOARD_ID_LIQUID_AUTO    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_AUTOMATIC ,0  ,60000   ,3 ,CAN_SUB_DATA_MODULE_LIQUID_AUTOMATIC},//查看自动抽废液状态
    {BOARD_ID_LIQUID_AUTO    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_AUTOMATIC ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL},//关闭三针清洗液稀释液路
    {BOARD_ID_LIQUID_AUTO    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_AUTOMATIC ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//打开三针清洗液稀释液路
};

//液路自动化单元初始化
LH_ERR CanSubBoard1LiquidAuto_Init(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_AUTO_INIT,cmdCanConfigLiquidAutomaticArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidAutomaticArray);
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

//强制执行一次抽废液
LH_ERR CanSubBoard1LiquidAuto_ForceTrigWasteLiquid(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_AUTO_FORCE_TRIG_WASTE_LIQUID,cmdCanConfigLiquidAutomaticArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidAutomaticArray);
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

//打开自动抽废液功能
LH_ERR CanSubBoard1LiquidAuto_EnableWasteLiquidAutomatic(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_AUTO_ENABLE_WASTE_LIQUID_AUTOMATIC,cmdCanConfigLiquidAutomaticArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidAutomaticArray);
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

//关闭自动抽废液功能
LH_ERR CanSubBoard1LiquidAuto_DisableWasteLiquidAutomatic(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_AUTO_DISABLE_WASTE_LIQUID_AUTOMATIC,cmdCanConfigLiquidAutomaticArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidAutomaticArray);
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

//查看自动抽废液状态
LH_ERR CanSubBoard1LiquidAuto_CheckStateWasteLiquid(LIQUID_AUTOMATIC_STATE* statePtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_AUTO_CHECK_STATE_WASTE_LIQUID,cmdCanConfigLiquidAutomaticArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidAutomaticArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *statePtr = (LIQUID_AUTOMATIC_STATE)(canMasterReadData.recvDataPtr[2]);
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//关闭三针清洗液稀释液路
LH_ERR CanSubBoard1LiquidAuto_CloseLiquidDilutionThreeNeedleClean(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_THREE_NEEDLE_CLEAN,cmdCanConfigLiquidAutomaticArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidAutomaticArray);
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

//打开三针清洗液稀释液路
LH_ERR CanSubBoard1LiquidAuto_OpenLiquidDilutionThreeNeedleClean(DILUTION_CHANNEL channel)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_THREE_NEEDLE_CLEAN,cmdCanConfigLiquidAutomaticArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(channel);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidAutomaticArray);
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















