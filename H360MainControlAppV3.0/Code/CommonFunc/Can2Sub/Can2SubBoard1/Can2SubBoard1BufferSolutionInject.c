/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:51:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-17 12:08:04
**FilePath: \Code\CommonFunc\Can2SubBoard1\Can2SubBoard1BufferSolutionInject.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard1BufferSolutionInject.h"

//指令板号
#define BOARD_ID_BUFFER_INJECT     CAN2_SUB_BOARD1

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
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigBufferInjectArray[] = {
    {BOARD_ID_BUFFER_INJECT    ,CAN2_CHANNEL_BOARD1_BUFFER_INJECT ,0  ,60000   ,0 ,CAN2_SUB_DATA_NULL         },//缓冲液注液复位
    {BOARD_ID_BUFFER_INJECT    ,CAN2_CHANNEL_BOARD1_BUFFER_INJECT ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL         },//缓冲液注液灌注
    {BOARD_ID_BUFFER_INJECT    ,CAN2_CHANNEL_BOARD1_BUFFER_INJECT ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL         },//缓冲液注液注液一次
    {BOARD_ID_BUFFER_INJECT    ,CAN2_CHANNEL_BOARD1_BUFFER_INJECT ,1  ,20000   ,0 ,CAN2_SUB_DATA_NULL         },//缓冲液注液灌注一次
    {BOARD_ID_BUFFER_INJECT    ,CAN2_CHANNEL_BOARD1_BUFFER_INJECT ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL         },//缓冲液注液模块电机复位
    {BOARD_ID_BUFFER_INJECT    ,CAN2_CHANNEL_BOARD1_BUFFER_INJECT ,5  ,60000   ,7 ,CAN2_SUB_DATA_MODULE_MOTOR },//缓冲液注液模块电机走步数
};

//缓冲液泵阀复位
LH_ERR Can2SubBoard1BufferSolutionInject_Reset(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_BUFFER_INJECT_RESET,cmdCan2ConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigBufferInjectArray);
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

//缓冲液灌注
LH_ERR Can2SubBoard1BufferSolutionInject_Prime(BOTTLE_BUFFER_SOLUTION bottleSelect)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_BUFFER_INJECT_PRIME,cmdCan2ConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(bottleSelect);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigBufferInjectArray);
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

//缓冲液注液
LH_ERR Can2SubBoard1BufferSolutionInject_Inject(BOTTLE_BUFFER_SOLUTION bottleSelect)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_BUFFER_INJECT_INJECT,cmdCan2ConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(bottleSelect);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigBufferInjectArray);
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

//缓冲液清洗维护
LH_ERR Can2SubBoard1BufferSolutionInject_MaintainPrime(BOTTLE_BUFFER_SOLUTION bottleSelect)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_BUFFER_INJECT_MAINTAIN_PRIME,cmdCan2ConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(bottleSelect);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigBufferInjectArray);
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
LH_ERR Can2SubBoard1BufferSolutionInject_StepMotorReset(INDEX_MOTOR_BUFFER_SOLUTION index)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_BUFFER_INJECT_STEP_MOTOR_RESET,cmdCan2ConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigBufferInjectArray);
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
LH_ERR Can2SubBoard1BufferSolutionInject_StepMotorRunSteps(INDEX_MOTOR_BUFFER_SOLUTION index,int32_t steps,int32_t* posAfterRun)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_BUFFER_INJECT_STEP_MOTOR_RUN_STEPS,cmdCan2ConfigBufferInjectArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigBufferInjectArray);
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









