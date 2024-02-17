/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:59:55
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-17 14:06:16
**FilePath: \Code\CommonFunc\Can2SubBoard3\Can2SubBoard3Param.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard3Param.h"

//指令板号
#define BOARD_ID_BOARD3_PARAM     CAN2_SUB_BOARD3

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
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigBoard3ParamArray[] = {
    {BOARD_ID_BOARD3_PARAM    ,CAN2_CHANNEL_BOARD3_PARAM ,2  ,4000   ,6 ,CAN2_SUB_DATA_PARAM   },//读取指定序号参数
    {BOARD_ID_BOARD3_PARAM    ,CAN2_CHANNEL_BOARD3_PARAM ,6  ,4000   ,0 ,CAN2_SUB_DATA_NULL    },//写入指定序号参数
    {BOARD_ID_BOARD3_PARAM    ,CAN2_CHANNEL_BOARD3_PARAM ,0  ,8000   ,0 ,CAN2_SUB_DATA_NULL    },//初始化板上参数为默认值
};

//读取指定序号参数
LH_ERR Can2SubBoard3Param_Read(INDEX_CAN2_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_BOARD3_PARAM_READ,cmdCan2ConfigBoard3ParamArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(mainIndex);
    can2SendCmd.paramBuffer[1] = (uint8_t)(subIndex);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigBoard3ParamArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *paramReadPtr = Can2ProcUtilConvertArrayToInt32(can2ReadData.recvDataPtr,2);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//写入指定序号参数
LH_ERR Can2SubBoard3Param_Write(INDEX_CAN2_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_BOARD3_PARAM_WRITE,cmdCan2ConfigBoard3ParamArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(mainIndex);
    can2SendCmd.paramBuffer[1] = (uint8_t)(subIndex);
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,2,paramWrite);
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigBoard3ParamArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//初始化板上参数为默认值
LH_ERR Can2SubBoard3Param_SetDefault(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_BOARD3_PARAM_SET_DEFAULT,cmdCan2ConfigBoard3ParamArray);
    //如果存在数据需要传送,在这里填充数据

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigBoard3ParamArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}










