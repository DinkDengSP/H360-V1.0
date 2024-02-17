/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:51:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-17 12:11:02
**FilePath: \Code\CommonFunc\Can2SubBoard1\Can2SubBoard1MixWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard1MixWash.h"

//指令板号
#define BOARD_ID_MIX_WASH     CAN2_SUB_BOARD1

//通道指令
typedef enum CMD_MIX_WASH
{
    CMD_MIX_WASH_RESET                 = 0X00000000,//清洗摇匀复位
    CMD_MIX_WASH_UP                    = 0X00000001,//清洗摇匀升起来
    CMD_MIX_WASH_DOWN                  = 0X00000002,//清洗摇匀降下去
    CMD_MIX_WASH_START_ROTATE          = 0X00000003,//清洗摇匀运行一次,不带自动升降
    CMD_MIX_WASH_ROTATE_INTEGRATED     = 0X00000004,//清洗摇匀集成摇匀一次
    CMD_MIX_WASH_STEP_MOTOR_RESET      = 0X00000005,//清洗摇匀模块电机复位
    CMD_MIX_WASH_STEP_MOTOR_RUN_STEPS  = 0X00000006,//清洗摇匀模块电机走步数
}CMD_MIX_WASH;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigMixWashArray[] = {
    {BOARD_ID_MIX_WASH    ,CAN2_CHANNEL_BOARD1_MIX_WASH ,0  ,60000   ,0 ,CAN2_SUB_DATA_NULL         },//清洗摇匀复位
    {BOARD_ID_MIX_WASH    ,CAN2_CHANNEL_BOARD1_MIX_WASH ,1  ,5000    ,0 ,CAN2_SUB_DATA_NULL         },//清洗摇匀升起来
    {BOARD_ID_MIX_WASH    ,CAN2_CHANNEL_BOARD1_MIX_WASH ,0  ,5000    ,0 ,CAN2_SUB_DATA_NULL         },//清洗摇匀降下去
    {BOARD_ID_MIX_WASH    ,CAN2_CHANNEL_BOARD1_MIX_WASH ,1  ,15000   ,0 ,CAN2_SUB_DATA_NULL         },//清洗摇匀运行一次,不带自动升降
    {BOARD_ID_MIX_WASH    ,CAN2_CHANNEL_BOARD1_MIX_WASH ,1  ,20000   ,0 ,CAN2_SUB_DATA_NULL         },//清洗摇匀集成摇匀一次
    {BOARD_ID_MIX_WASH    ,CAN2_CHANNEL_BOARD1_MIX_WASH ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL         },//清洗摇匀模块电机复位
    {BOARD_ID_MIX_WASH    ,CAN2_CHANNEL_BOARD1_MIX_WASH ,5  ,60000   ,7 ,CAN2_SUB_DATA_MODULE_MOTOR },//清洗摇匀模块电机走步数
};

//清洗混匀模块复位,带零位偏移
LH_ERR Can2SubBoard1MixWash_Reset(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_MIX_WASH_RESET,cmdCan2ConfigMixWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigMixWashArray);
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

//清洗混匀升起来
LH_ERR Can2SubBoard1MixWash_Up(MIX_MODE mixMode)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_MIX_WASH_UP,cmdCan2ConfigMixWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigMixWashArray);
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

//清洗混匀升降降下去
LH_ERR Can2SubBoard1MixWash_Down(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_MIX_WASH_DOWN,cmdCan2ConfigMixWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigMixWashArray);
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

//清洗混匀旋转混匀一次,不带自动升降
LH_ERR Can2SubBoard1MixWash_StartRotate(MIX_MODE mixMode)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_MIX_WASH_START_ROTATE,cmdCan2ConfigMixWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigMixWashArray);
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

//清洗混匀集成混匀一次
LH_ERR Can2SubBoard1MixWash_RotateIntegrated(MIX_MODE mixMode)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_MIX_WASH_ROTATE_INTEGRATED,cmdCan2ConfigMixWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigMixWashArray);
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

//指定步进电机复位
LH_ERR Can2SubBoard1MixWash_StepMotorReset(INDEX_MOTOR_MIX_WASH index)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_MIX_WASH_STEP_MOTOR_RESET,cmdCan2ConfigMixWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigMixWashArray);
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
LH_ERR Can2SubBoard1MixWash_StepMotorRunSteps(INDEX_MOTOR_MIX_WASH index,int32_t steps,int32_t* posAfterRun)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_MIX_WASH_STEP_MOTOR_RUN_STEPS,cmdCan2ConfigMixWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigMixWashArray);
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














