/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-01 19:59:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-04 13:57:36
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\CanSubBoard1\CanSubBoard1LiquidNeedleReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard1LiquidNeedleReagent.h"

//指令板号
#define BOARD_ID_LIQUID_NEEDLE_REAGENT     CAN_MASTER_SUB_BOARD1

//通道指令
typedef enum CMD_LIQUID_NEEDLE_REAGENT
{
    CMD_LIQUID_NEEDLE_REAGENT_RESET             = 0X00000000,//试剂针液路复位
    CMD_LIQUID_NEEDLE_REAGENT_OPEN_CLEAN        = 0X00000001,//试剂针液路清洗泵阀打开
    CMD_LIQUID_NEEDLE_REAGENT_CLOSE_CLEAN       = 0X00000002,//试剂针液路清洗泵阀关闭
    CMD_LIQUID_NEEDLE_REAGENT_OPEN_PRIME        = 0X00000003,//试剂针液路灌注泵阀打开
    CMD_LIQUID_NEEDLE_REAGENT_CLOSE_PRIME       = 0X00000004,//试剂针液路灌注清洗泵阀关闭
    CMD_LIQUID_NEEDLE_REAGENT_OPEN_MAINTAIN     = 0X00000005,//试剂针液路清洗维护泵阀打开
    CMD_LIQUID_NEEDLE_REAGENT_CLOSE_MAINTAIN    = 0X00000006,//试剂针液路清洗维护泵阀关闭
}CMD_LIQUID_NEEDLE_REAGENT;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigLiquidNeedleReagentArray[] = {
    {BOARD_ID_LIQUID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_REAGENT ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL},//试剂针液路复位
    {BOARD_ID_LIQUID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//试剂针液路清洗泵阀打开
    {BOARD_ID_LIQUID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//试剂针液路清洗泵阀关闭
    {BOARD_ID_LIQUID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//试剂针液路灌注泵阀打开
    {BOARD_ID_LIQUID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//试剂针液路灌注清洗泵阀关闭
    {BOARD_ID_LIQUID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//试剂针液路清洗维护泵阀打开
    {BOARD_ID_LIQUID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//试剂针液路清洗维护泵阀关闭
};

//试剂针泵阀复位
LH_ERR CanSubBoard1LiquidNeedleReagent_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_REAGENT_RESET,cmdCanConfigLiquidNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleReagentArray);
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

//试剂针清洗泵阀打开
LH_ERR CanSubBoard1LiquidNeedleReagent_OpenClean(NEEDLE_CLEAN_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_REAGENT_OPEN_CLEAN,cmdCanConfigLiquidNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleReagentArray);
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

//试剂针清洗泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleReagent_CloseClean(NEEDLE_CLEAN_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_REAGENT_CLOSE_CLEAN,cmdCanConfigLiquidNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleReagentArray);
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

//试剂针灌注泵阀打开
LH_ERR CanSubBoard1LiquidNeedleReagent_OpenPrime(NEEDLE_PRIME_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_REAGENT_OPEN_PRIME,cmdCanConfigLiquidNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleReagentArray);
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

//试剂针灌注泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleReagent_ClosePrime(NEEDLE_PRIME_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_REAGENT_CLOSE_PRIME,cmdCanConfigLiquidNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleReagentArray);
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

//试剂针清洗维护泵阀打开
LH_ERR CanSubBoard1LiquidNeedleReagent_OpenMaintain(NEEDLE_MAINTAIN_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_REAGENT_OPEN_MAINTAIN,cmdCanConfigLiquidNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleReagentArray);
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

//试剂针清洗维护泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleReagent_CloseMaintain(NEEDLE_MAINTAIN_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_REAGENT_CLOSE_MAINTAIN,cmdCanConfigLiquidNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleReagentArray);
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
















