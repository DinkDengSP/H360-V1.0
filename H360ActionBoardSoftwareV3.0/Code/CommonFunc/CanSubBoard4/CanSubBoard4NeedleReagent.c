/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 16:00:11
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-29 18:11:52
**FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard4\CanSubBoard4NeedleReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard4NeedleReagent.h"

//指令板号
#define BOARD_ID_NEEDLE_REAGENT     CAN_MASTER_SUB_BOARD4

//通道指令
typedef enum CMD_NEEDLE_REAGENT
{
    CMD_NEEDLE_REAGENT_RESET                        = 0X00000000,//试剂针复位
    CMD_NEEDLE_REAGENT_CLEAN                        = 0X00000001,//试剂针清洗
    CMD_NEEDLE_REAGENT_PRIME                        = 0X00000002,//试剂针灌注
    CMD_NEEDLE_REAGENT_MAINTAIN_CLEAN               = 0X00000003,//试剂针清洗维护
    CMD_NEEDLE_REAGENT_ROTATE                       = 0X00000004,//试剂针旋转指定位置
    CMD_NEEDLE_REAGENT_LIQUID_ABSORB                = 0X00000005,//试剂针吸液
    CMD_NEEDLE_REAGENT_LIQUID_INJECT                = 0X00000006,//试剂针注液
    CMD_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST         = 0X00000007,//试剂针探液校准
    CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE     = 0X00000008,//读试剂针探液板状态
    CMD_NEEDLE_REAGENT_STEP_MOTOR_RESET             = 0X00000009,//指定步进电机复位
    CMD_NEEDLE_REAGENT_STEP_MOTOR_RUN_STEPS         = 0X0000000A,//指定步进电机走步数
    CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_RES_VOL   = 0X0000000B,//读取针尖电阻和针尖电压
    CMD_NEEDLE_REAGENT_LIQUID_DETECT_WRITE_RES      = 0X0000000C,//写入探液板电阻值
    CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_BASE_VOL  = 0X0000000D,//读取针尖基准电压
    CMD_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK         = 0X0000000E,//试剂针试剂余量检查
}CMD_NEEDLE_REAGENT;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigNeedleReagentArray[] = {
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL                 },//试剂针复位
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,2  ,60000   ,0 ,CAN_BASE_DATA_NULL                 },//试剂针清洗
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL                 },//试剂针灌注
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,2  ,60000   ,0 ,CAN_BASE_DATA_NULL                 },//试剂针清洗维护
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL                 },//试剂针旋转指定位置
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,4  ,60000   ,8 ,CAN_SUB_DATA_MODULE_NEEDLE_REAGENT },//试剂针吸液
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,2  ,60000   ,0 ,CAN_BASE_DATA_NULL                 },//试剂针注液
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL                 },//试剂针探液校准
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,1  ,60000   ,8 ,CAN_SUB_DATA_MODULE_NEEDLE_REAGENT },//读试剂针探液板状态
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL                 },//指定步进电机复位
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,5  ,60000   ,7 ,CAN_BASE_DATA_MODULE_MOTOR         },//指定步进电机走步数
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,6 ,CAN_SUB_DATA_MODULE_NEEDLE_REAGENT },//读取针尖电阻和针尖电压
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,2  ,60000   ,0 ,CAN_BASE_DATA_NULL                 },//写入探液板电阻值
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,4 ,CAN_SUB_DATA_MODULE_NEEDLE_REAGENT },//读取针尖基准电压
    {BOARD_ID_NEEDLE_REAGENT    ,CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,6 ,CAN_SUB_DATA_MODULE_NEEDLE_REAGENT },//试剂针试剂余量检查
};

//试剂针复位,带零位偏移
LH_ERR CanSubBoard4NeedleReagent_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_RESET,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = ;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//试剂针清洗
LH_ERR CanSubBoard4NeedleReagent_Clean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_CLEAN,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);
    canMasterSendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//试剂针灌注
LH_ERR CanSubBoard4NeedleReagent_Prime(NEEDLE_PRIME_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_PRIME,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//试剂针清洗维护
LH_ERR CanSubBoard4NeedleReagent_MaintainClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_MAINTAIN_CLEAN,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);
    canMasterSendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//试剂针旋转指定位置
LH_ERR CanSubBoard4NeedleReagent_Rotate(NEEDLE_REAGENT_POS targetPos)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_ROTATE,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(targetPos);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//试剂针吸液
LH_ERR CanSubBoard4NeedleReagent_LiquidAbsorb(uint16_t CountMinUl,uint16_t lastLiquidDetectPos,uint16_t* currentLiquidDetectPos,uint16_t* pumpMinUlCount,
                                                LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_LIQUID_ABSORB,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    CanBaseWriteUint16DataToBuffer(canMasterSendCmd.paramBuffer,0,CountMinUl);
    CanBaseWriteUint16DataToBuffer(canMasterSendCmd.paramBuffer,2,lastLiquidDetectPos);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentLiquidDetectPos = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,2);
    *pumpMinUlCount = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,4);
    *liquidResultRerurn = (LIQUID_FUNC_RESULT)(canMasterReadData.recvDataPtr[6]);
    *needleLiquidDetectResult = (LIQUID_NEEDLE_LIQUID_DETECT_RESULT)(canMasterReadData.recvDataPtr[7]);
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//试剂针注液
LH_ERR CanSubBoard4NeedleReagent_LiquidInject(NEEDLE_INJECT_LEVEL injectLevel,uint8_t mixCount)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_LIQUID_INJECT,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(injectLevel);
    canMasterSendCmd.paramBuffer[1] = (uint8_t)(mixCount);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//试剂针探液校准
LH_ERR CanSubBoard4NeedleReagent_LiquidDetectAdjust(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//读试剂针探液板状态
LH_ERR CanSubBoard4NeedleReagent_LiquidDetectReadState(LIQUID_DETECT_CONFIG config,uint16_t* needleVol,uint16_t* thresholdValue,uint16_t* slopeValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(config);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *needleVol = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,2);
    *thresholdValue = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,4);
    *slopeValue = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,6);
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR CanSubBoard4NeedleReagent_StepMotorReset(INDEX_MOTOR_NEEDLE_REAGENT index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_STEP_MOTOR_RESET,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//指定步进电机走步数
LH_ERR CanSubBoard4NeedleReagent_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_REAGENT index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_STEP_MOTOR_RUN_STEPS,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//读取电阻和针尖电压
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectReadResVol(uint16_t* resValue,uint16_t* volValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_RES_VOL,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *resValue = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,2);
    *volValue = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,4);

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//写入电阻值
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectWriteRes(uint16_t resValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_WRITE_RES,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    CanBaseWriteUint16DataToBuffer(canMasterSendCmd.paramBuffer,0,resValue);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
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

//读取针尖基准电压
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectReadNeedleBaseVol(uint16_t* needleBaseVol)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_BASE_VOL,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *needleBaseVol = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,2);

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠针试剂余量探测
LH_ERR CanSubBoard4NeedleReagent_LiquidResidueCheck(uint16_t* currentLiquidDetectPos,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK,cmdCanConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    * currentLiquidDetectPos = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,2);
    * liquidResultRerurn = (LIQUID_FUNC_RESULT)canMasterReadData.recvDataPtr[4];
    * needleLiquidDetectResult = (LIQUID_NEEDLE_LIQUID_DETECT_RESULT)(canMasterReadData.recvDataPtr[5]);

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}











