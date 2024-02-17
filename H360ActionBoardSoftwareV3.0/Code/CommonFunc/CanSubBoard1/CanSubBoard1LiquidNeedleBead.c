/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-01 19:59:26
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-04 14:01:25
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\CanSubBoard1\CanSubBoard1LiquidNeedleBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard1LiquidNeedleBead.h"

//指令板号
#define BOARD_ID_LIQUID_NEEDLE_BEAD     CAN_MASTER_SUB_BOARD1

//通道指令
typedef enum CMD_LIQUID_NEEDLE_BEAD
{
    CMD_LIQUID_NEEDLE_BEAD_RESET             = 0X00000000,//磁珠针液路复位
    CMD_LIQUID_NEEDLE_BEAD_OPEN_CLEAN        = 0X00000001,//磁珠针液路清洗泵阀打开
    CMD_LIQUID_NEEDLE_BEAD_CLOSE_CLEAN       = 0X00000002,//磁珠针液路清洗泵阀关闭
    CMD_LIQUID_NEEDLE_BEAD_OPEN_PRIME        = 0X00000003,//磁珠针液路灌注泵阀打开
    CMD_LIQUID_NEEDLE_BEAD_CLOSE_PRIME       = 0X00000004,//磁珠针液路灌注清洗泵阀关闭
    CMD_LIQUID_NEEDLE_BEAD_OPEN_MAINTAIN     = 0X00000005,//磁珠针液路清洗维护泵阀打开
    CMD_LIQUID_NEEDLE_BEAD_CLOSE_MAINTAIN    = 0X00000006,//磁珠针液路清洗维护泵阀关闭
}CMD_LIQUID_NEEDLE_BEAD;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigLiquidNeedleBeadArray[] = {
    {BOARD_ID_LIQUID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_BEAD ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL},//磁珠针液路复位
    {BOARD_ID_LIQUID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//磁珠针液路清洗泵阀打开
    {BOARD_ID_LIQUID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//磁珠针液路清洗泵阀关闭
    {BOARD_ID_LIQUID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//磁珠针液路灌注泵阀打开
    {BOARD_ID_LIQUID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//磁珠针液路灌注清洗泵阀关闭
    {BOARD_ID_LIQUID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//磁珠针液路清洗维护泵阀打开
    {BOARD_ID_LIQUID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL},//磁珠针液路清洗维护泵阀关闭
};

//磁珠针泵阀复位
LH_ERR CanSubBoard1LiquidNeedleBead_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_BEAD_RESET,cmdCanConfigLiquidNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleBeadArray);
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

//磁珠针清洗泵阀打开
LH_ERR CanSubBoard1LiquidNeedleBead_OpenClean(NEEDLE_CLEAN_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_BEAD_OPEN_CLEAN,cmdCanConfigLiquidNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleBeadArray);
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

//磁珠针清洗泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleBead_CloseClean(NEEDLE_CLEAN_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_BEAD_CLOSE_CLEAN,cmdCanConfigLiquidNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleBeadArray);
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

//磁珠针灌注泵阀打开
LH_ERR CanSubBoard1LiquidNeedleBead_OpenPrime(NEEDLE_PRIME_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_BEAD_OPEN_PRIME,cmdCanConfigLiquidNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleBeadArray);
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

//磁珠针灌注泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleBead_ClosePrime(NEEDLE_PRIME_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_BEAD_CLOSE_PRIME,cmdCanConfigLiquidNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleBeadArray);
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

//磁珠针清洗维护泵阀打开
LH_ERR CanSubBoard1LiquidNeedleBead_OpenMaintain(NEEDLE_MAINTAIN_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_BEAD_OPEN_MAINTAIN,cmdCanConfigLiquidNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleBeadArray);
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

//磁珠针清洗维护泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleBead_CloseMaintain(NEEDLE_MAINTAIN_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_LIQUID_NEEDLE_BEAD_CLOSE_MAINTAIN,cmdCanConfigLiquidNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigLiquidNeedleBeadArray);
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










