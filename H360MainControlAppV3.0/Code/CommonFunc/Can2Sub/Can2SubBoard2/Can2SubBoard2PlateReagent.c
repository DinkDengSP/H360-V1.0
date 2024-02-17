/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:57:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-17 12:14:52
**FilePath: \Code\CommonFunc\Can2SubBoard2\Can2SubBoard2PlateReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard2PlateReagent.h"


//指令板号
#define BOARD_ID_PLATE_REAGENT     CAN2_SUB_BOARD2

//通道指令
typedef enum CMD_PLATE_REAGENT
{
    CMD_PLATE_REAGENT_RESET                             = 0X00000000,//试剂盘复位
    CMD_PLATE_REAGENT_RUN_2_CUP_WITH_OFFSET             = 0X00000001,//试剂盘运行到指定序号带偏移杯位
    CMD_PLATE_REAGENT_RUN_ANY_CUPS                      = 0X00000002,//试剂盘运行指定数量个杯位
    CMD_PLATE_REAGENT_STEP_MOTOR_RESET                  = 0X00000003,//试剂盘模块电机复位
    CMD_PLATE_REAGENT_STEP_MOTOR_RUN_STEPS              = 0X00000004,//试剂盘模块电机走步数
    CMD_PLATE_REAGENT_RUN_2_CUP_WITH_OFFSET_FOR_SCAN    = 0X00000005,//试剂盘运行到指定序号带偏移杯位
    CMD_PLATE_REAGENT_RUN_2_CUP_WITH_OFFSET_FOR_RFID    = 0X00000006,//试剂盘运行到指定序号带偏移杯位RFID
}CMD_PLATE_REAGENT;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigPlateReagentArray[] = {
    {BOARD_ID_PLATE_REAGENT    ,CAN2_CHANNEL_BOARD2_PLATE_REAGENT ,0  ,60000   ,4 ,CAN2_SUB_DATA_MODULE_PLATE_REAGENT   },//试剂盘复位
    {BOARD_ID_PLATE_REAGENT    ,CAN2_CHANNEL_BOARD2_PLATE_REAGENT ,2  ,10000   ,4 ,CAN2_SUB_DATA_MODULE_PLATE_REAGENT   },//试剂盘运行到指定序号带偏移杯位
    {BOARD_ID_PLATE_REAGENT    ,CAN2_CHANNEL_BOARD2_PLATE_REAGENT ,1  ,10000   ,4 ,CAN2_SUB_DATA_MODULE_PLATE_REAGENT   },//试剂盘运行指定数量个杯位
    {BOARD_ID_PLATE_REAGENT    ,CAN2_CHANNEL_BOARD2_PLATE_REAGENT ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL                   },//试剂盘模块电机复位
    {BOARD_ID_PLATE_REAGENT    ,CAN2_CHANNEL_BOARD2_PLATE_REAGENT ,5  ,60000   ,7 ,CAN2_SUB_DATA_MODULE_MOTOR           },//试剂盘模块电机走步数
    {BOARD_ID_PLATE_REAGENT    ,CAN2_CHANNEL_BOARD2_PLATE_REAGENT ,2  ,10000   ,4 ,CAN2_SUB_DATA_MODULE_PLATE_REAGENT   },//试剂盘运行到指定序号带扫码偏移杯位
    {BOARD_ID_PLATE_REAGENT    ,CAN2_CHANNEL_BOARD2_PLATE_REAGENT ,2  ,10000   ,4 ,CAN2_SUB_DATA_MODULE_PLATE_REAGENT   },//试剂盘运行到指定序号带扫码偏移杯位
};

//试剂盘复位
LH_ERR Can2SubBoard2PlateReagent_Reset(uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_REAGENT_RESET,cmdCan2ConfigPlateReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = ;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentCupIndex = (uint8_t)(can2ReadData.recvDataPtr[2]);
    *currentOffset = (PLATE_REAGENT_OFFSET)(can2ReadData.recvDataPtr[3]);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//试剂盘运行到指定序号杯位,带偏移
LH_ERR Can2SubBoard2PlateReagent_Run2CupWithOffset(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_REAGENT_RUN_2_CUP_WITH_OFFSET,cmdCan2ConfigPlateReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(targetCupIndex);
    can2SendCmd.paramBuffer[1] = (uint8_t)(targetOffset);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentCupIndex = (uint8_t)(can2ReadData.recvDataPtr[2]);
    *currentOffset = (PLATE_REAGENT_OFFSET)(can2ReadData.recvDataPtr[3]);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//试剂盘运行到指定序号杯位,带偏移
LH_ERR Can2SubBoard2PlateReagent_Run2CupWithOffsetForScan(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_REAGENT_RUN_2_CUP_WITH_OFFSET_FOR_SCAN,cmdCan2ConfigPlateReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(targetCupIndex);
    can2SendCmd.paramBuffer[1] = (uint8_t)(scanOffset);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentCupIndex = (uint8_t)(can2ReadData.recvDataPtr[2]);
    *currentOffset = (PLATE_REAGENT_OFFSET)(can2ReadData.recvDataPtr[3]);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//试剂盘运行到指定序号杯位,带偏移RFID
LH_ERR Can2SubBoard2PlateReagent_Run2CupWithOffsetForRFID(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_REAGENT_RUN_2_CUP_WITH_OFFSET_FOR_RFID,cmdCan2ConfigPlateReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(targetCupIndex);
    can2SendCmd.paramBuffer[1] = (uint8_t)(scanOffset);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentCupIndex = (uint8_t)(can2ReadData.recvDataPtr[2]);
    *currentOffset = (PLATE_REAGENT_OFFSET)(can2ReadData.recvDataPtr[3]);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//试剂盘运行指定数量个杯位,基于当前位置
LH_ERR Can2SubBoard2PlateReagent_RunAnyCups(uint16_t cups,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_REAGENT_RUN_ANY_CUPS,cmdCan2ConfigPlateReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(cups);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentCupIndex = (uint8_t)(can2ReadData.recvDataPtr[2]);
    *currentOffset = (PLATE_REAGENT_OFFSET)(can2ReadData.recvDataPtr[3]);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR Can2SubBoard2PlateReagent_StepMotorReset(INDEX_MOTOR_PLATE_REAGENT index)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_REAGENT_STEP_MOTOR_RESET,cmdCan2ConfigPlateReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateReagentArray);
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

//指定步进电机走步数
LH_ERR Can2SubBoard2PlateReagent_StepMotorRunSteps(INDEX_MOTOR_PLATE_REAGENT index,int32_t steps,int32_t* posAfterRun)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_PLATE_REAGENT_STEP_MOTOR_RUN_STEPS,cmdCan2ConfigPlateReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigPlateReagentArray);
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















