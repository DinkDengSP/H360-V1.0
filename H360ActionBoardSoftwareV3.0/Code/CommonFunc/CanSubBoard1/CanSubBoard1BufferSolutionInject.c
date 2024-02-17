/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:51:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-04 13:14:20
**FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\CanSubBoard1\CanSubBoard1BufferSolutionInject.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard1BufferSolutionInject.h"

//指令板号
#define BOARD_ID_BUFFER_INJECT     CAN_MASTER_SUB_BOARD1

//通道指令
typedef enum CMD_BUFFER_INJECT
{
    CMD_BUFFER_INJECT_RESET                = 0X00000000,//缓冲液注液复位
    CMD_BUFFER_INJECT_PRIME                = 0X00000001,//缓冲液注液灌注
    CMD_BUFFER_INJECT_INJECT               = 0X00000002,//缓冲液注液注液一次
    CMD_BUFFER_INJECT_MAINTAIN_PRIME       = 0X00000003,//缓冲液注液灌注一次
    CMD_BUFFER_INJECT_STEP_MOTOR_RESET     = 0X00000004,//缓冲液注液模块电机复位
    CMD_BUFFER_INJECT_STEP_MOTOR_RUN_STEPS = 0X00000005,//缓冲液注液模块电机走步数
}CMD_BUFFER_INJECT;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigBufferInjectArray[] = {
    {BOARD_ID_BUFFER_INJECT    ,CAN_MASTER_CHANNEL_BOARD1_BUFFER_INJECT ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针复位
    {BOARD_ID_BUFFER_INJECT    ,CAN_MASTER_CHANNEL_BOARD1_BUFFER_INJECT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针灌注
    {BOARD_ID_BUFFER_INJECT    ,CAN_MASTER_CHANNEL_BOARD1_BUFFER_INJECT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针清洗维护
    {BOARD_ID_BUFFER_INJECT    ,CAN_MASTER_CHANNEL_BOARD1_BUFFER_INJECT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针磁珠清洗
    {BOARD_ID_BUFFER_INJECT    ,CAN_MASTER_CHANNEL_BOARD1_BUFFER_INJECT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL         },//清洗针模块电机复位
    {BOARD_ID_BUFFER_INJECT    ,CAN_MASTER_CHANNEL_BOARD1_BUFFER_INJECT ,5  ,60000   ,7 ,CAN_BASE_DATA_MODULE_MOTOR },//清洗针模块电机走步数
};

//缓冲液泵阀复位
LH_ERR CanSubBoard1BufferSolutionInject_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_BUFFER_INJECT_RESET,cmdCanConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigBufferInjectArray);
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

//缓冲液灌注
LH_ERR CanSubBoard1BufferSolutionInject_Prime(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_BUFFER_INJECT_PRIME,cmdCanConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(bottltSelect);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigBufferInjectArray);
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

//缓冲液注液
LH_ERR CanSubBoard1BufferSolutionInject_Inject(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_BUFFER_INJECT_INJECT,cmdCanConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(bottltSelect);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigBufferInjectArray);
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

//缓冲液清洗维护
LH_ERR CanSubBoard1BufferSolutionInject_MaintainPrime(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_BUFFER_INJECT_MAINTAIN_PRIME,cmdCanConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(bottltSelect);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigBufferInjectArray);
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
LH_ERR CanSubBoard1BufferSolutionInject_StepMotorReset(INDEX_MOTOR_BUFFER_SOLUTION index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_BUFFER_INJECT_STEP_MOTOR_RESET,cmdCanConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigBufferInjectArray);
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
LH_ERR CanSubBoard1BufferSolutionInject_StepMotorRunSteps(INDEX_MOTOR_BUFFER_SOLUTION index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_BUFFER_INJECT_STEP_MOTOR_RUN_STEPS,cmdCanConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigBufferInjectArray);
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









