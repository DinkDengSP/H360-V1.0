/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:50:58
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-17 12:12:10
**FilePath: \Code\CommonFunc\Can2SubBoard1\Can2SubBoard1PlateWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard1PlateWash.h"

//指令板号
#define BOARD_ID_PLATE_WASH     CAN2_SUB_BOARD1

//通道指令
typedef enum CMD_PLATE_WASH
{
    CMD_PLATE_WASH_RESET                = 0X00000000,//清洗盘复位
    CMD_PLATE_WASH_RUN_ANY_CUPS         = 0X00000001,//清洗盘运行指定数量个杯位
    CMD_PLATE_WASH_RUN_2_CUP_WITH_INDEX = 0X00000002,//清洗盘运行到指定序号杯位
    CMD_PLATE_WASH_STEP_MOTOR_RESET     = 0X00000003,//清洗盘模块电机复位
    CMD_PLATE_WASH_STEP_MOTOR_RUN_STEPS = 0X00000004,//清洗盘模块电机走步数
}CMD_PLATE_WASH;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigPlateWashArray[] = {
    {BOARD_ID_PLATE_WASH    ,CAN2_CHANNEL_BOARD1_PLATE_WASH ,0  ,60000   ,4 ,CAN2_SUB_DATA_MODULE_PLATE_WASH},//清洗盘复位
    {BOARD_ID_PLATE_WASH    ,CAN2_CHANNEL_BOARD1_PLATE_WASH ,1  ,10000   ,4 ,CAN2_SUB_DATA_MODULE_PLATE_WASH},//清洗盘运行指定数量个杯位
    {BOARD_ID_PLATE_WASH    ,CAN2_CHANNEL_BOARD1_PLATE_WASH ,1  ,10000   ,4 ,CAN2_SUB_DATA_MODULE_PLATE_WASH},//清洗盘运行到指定序号杯位
    {BOARD_ID_PLATE_WASH    ,CAN2_CHANNEL_BOARD1_PLATE_WASH ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL             },//清洗盘模块电机复位
    {BOARD_ID_PLATE_WASH    ,CAN2_CHANNEL_BOARD1_PLATE_WASH ,5  ,60000   ,7 ,CAN2_SUB_DATA_MODULE_MOTOR     },//清洗盘模块电机走步数
};

//清洗盘旋转复位,带零位修正
LH_ERR Can2SubBoard1PlateWash_Reset(SENSOR_STATE* cupExistState,uint8_t* cupIndex)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_WASH_RESET,cmdCan2ConfigPlateWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupIndex = (uint8_t)(can2ReadData.recvDataPtr[2]);
    *cupExistState = (SENSOR_STATE)(can2ReadData.recvDataPtr[3]);
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//清洗盘移动指定个杯位
LH_ERR Can2SubBoard1PlateWash_RunAnyCups(uint8_t cupsCount,SENSOR_STATE* cupExistState,uint8_t* cupIndex)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_WASH_RUN_ANY_CUPS,cmdCan2ConfigPlateWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = cupsCount;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupIndex = (uint8_t)(can2ReadData.recvDataPtr[2]);
    *cupExistState = (SENSOR_STATE)(can2ReadData.recvDataPtr[3]);
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//清洗盘移动到指定序号杯位
LH_ERR Can2SubBoard1PlateWash_Run2CupWithIndex(uint8_t targetIndex,SENSOR_STATE* cupExistState,uint8_t* cupIndex)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_WASH_RUN_2_CUP_WITH_INDEX,cmdCan2ConfigPlateWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = targetIndex;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupIndex = (uint8_t)(can2ReadData.recvDataPtr[2]);
    *cupExistState = (SENSOR_STATE)(can2ReadData.recvDataPtr[3]);
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位,复位带零位修正
LH_ERR Can2SubBoard1PlateWash_StepMotorReset(INDEX_MOTOR_PLATE_WASH index)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_WASH_STEP_MOTOR_RESET,cmdCan2ConfigPlateWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateWashArray);
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

//指定步进电机走指定步数
LH_ERR Can2SubBoard1PlateWash_StepMotorRunSteps(INDEX_MOTOR_PLATE_WASH index,int32_t steps,int32_t* posAfterRun)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_WASH_STEP_MOTOR_RUN_STEPS,cmdCan2ConfigPlateWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *posAfterRun = Can2ProcUtilConvertArrayToInt32(can2ReadData.recvDataPtr,3);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}
















