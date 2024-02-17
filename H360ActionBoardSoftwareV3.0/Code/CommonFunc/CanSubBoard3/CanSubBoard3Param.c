/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:59:55
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-04 19:36:39
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\CanSubBoard3\CanSubBoard3Param.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard3Param.h"

//指令板号
#define BOARD_ID_BOARD3_PARAM     CAN_MASTER_SUB_BOARD3

//通道指令
typedef enum CMD_BOARD3_PARAM
{
    CMD_BOARD3_PARAM_READ        = 0X00000000,//读取指定序号参数
    CMD_BOARD3_PARAM_WRITE       = 0X00000001,//写入指定序号参数
    CMD_BOARD3_PARAM_SET_DEFAULT = 0X00000002,//初始化板上参数为默认值
}CMD_BOARD3_PARAM;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigBoard3ParamArray[] = {
    {BOARD_ID_BOARD3_PARAM    ,CAN_MASTER_CHANNEL_BOARD3_PARAM ,2  ,60000   ,6 ,CAN_BASE_DATA_PARAM   },//读取指定序号参数
    {BOARD_ID_BOARD3_PARAM    ,CAN_MASTER_CHANNEL_BOARD3_PARAM ,6  ,60000   ,0 ,CAN_BASE_DATA_NULL    },//写入指定序号参数
    {BOARD_ID_BOARD3_PARAM    ,CAN_MASTER_CHANNEL_BOARD3_PARAM ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL    },//初始化板上参数为默认值
};

//读取指定序号参数
LH_ERR CanSubBoard3Param_Read(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_BOARD3_PARAM_READ,cmdCanConfigBoard3ParamArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(mainIndex);
    canMasterSendCmd.paramBuffer[1] = (uint8_t)(subIndex);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigBoard3ParamArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *paramReadPtr = CanBaseReadDataConvertToInt32(canMasterReadData.recvDataPtr,2);
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//写入指定序号参数
LH_ERR CanSubBoard3Param_Write(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_BOARD3_PARAM_WRITE,cmdCanConfigBoard3ParamArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(mainIndex);
    canMasterSendCmd.paramBuffer[1] = (uint8_t)(subIndex);
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,2,paramWrite);
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigBoard3ParamArray);
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

//初始化板上参数为默认值
LH_ERR CanSubBoard3Param_SetDefault(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_BOARD3_PARAM_SET_DEFAULT,cmdCanConfigBoard3ParamArray);
    //如果存在数据需要传送,在这里填充数据

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigBoard3ParamArray);
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










