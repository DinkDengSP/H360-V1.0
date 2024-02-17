/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:56:47
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-29 18:12:06
**FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard2\CanSubBoard2NeedleBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard2NeedleBead.h"

//指令板号
#define BOARD_ID_NEEDLE_BEAD     CAN_MASTER_SUB_BOARD2

//通道指令
typedef enum CMD_NEEDLE_BEAD
{
    CMD_NEEDLE_BEAD_RESET                       = 0X00000000,//磁珠针复位
    CMD_NEEDLE_BEAD_CLEAN                       = 0X00000001,//磁珠针清洗
    CMD_NEEDLE_BEAD_PRIME                       = 0X00000002,//磁珠针灌注
    CMD_NEEDLE_BEAD_MAINTAIN_CLEAN              = 0X00000003,//磁珠针清洗维护
    CMD_NEEDLE_BEAD_ROTATE                      = 0X00000004,//磁珠针旋转指定位置
    CMD_NEEDLE_BEAD_LIQUID_ABSORB               = 0X00000005,//磁珠针吸液
    CMD_NEEDLE_BEAD_LIQUID_INJECT               = 0X00000006,//磁珠针注液
    CMD_NEEDLE_BEAD_LIQUID_DETECT_ADJUST        = 0X00000007,//磁珠针探液校准
    CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE    = 0X00000008,//磁珠针探液板状态
    CMD_NEEDLE_BEAD_STEP_MOTOR_RESET            = 0X00000009,//磁珠针模块电机复位
    CMD_NEEDLE_BEAD_STEP_MOTOR_RUN_STEPS        = 0X0000000A,//磁珠针模块电机走步数
    CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_RES_VOL  = 0X0000000B,//读取针尖电阻和针尖电压
    CMD_NEEDLE_BEAD_LIQUID_DETECT_WRITE_RES     = 0X0000000C,//写入探液板电阻值
    CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_BASE_VOL = 0X0000000D,//读取针尖基准电压
    CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK        = 0X0000000E,//磁珠针试剂余量检查
}CMD_NEEDLE_BEAD;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigNeedleBeadArray[] = {
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//磁珠针复位
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,2  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//磁珠针清洗
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//磁珠针灌注
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,2  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//磁珠针清洗维护
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//磁珠针旋转指定位置
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,4  ,60000   ,8 ,CAN_SUB_DATA_MODULE_NEEDLE_BEAD  },//磁珠针吸液
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,2  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//磁珠针注液
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//磁珠针探液校准
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,1  ,60000   ,8 ,CAN_SUB_DATA_MODULE_NEEDLE_BEAD  },//磁珠针探液板状态
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//磁珠针模块电机复位
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,5  ,60000   ,7 ,CAN_BASE_DATA_MODULE_MOTOR       },//磁珠针模块电机走步数
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,6 ,CAN_SUB_DATA_MODULE_NEEDLE_BEAD  },//读取针尖电阻和针尖电压
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,2  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//写入探液板电阻值
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,4 ,CAN_SUB_DATA_MODULE_NEEDLE_BEAD  },//读取针尖基准电压
    {BOARD_ID_NEEDLE_BEAD    ,CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,6 ,CAN_SUB_DATA_MODULE_NEEDLE_BEAD  },//磁珠针试剂余量检查
};

//磁珠针复位,带零位偏移
LH_ERR CanSubBoard2NeedleBead_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_RESET,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//磁珠针清洗
LH_ERR CanSubBoard2NeedleBead_Clean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_CLEAN,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);
    canMasterSendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//磁珠针灌注
LH_ERR CanSubBoard2NeedleBead_Prime(NEEDLE_PRIME_OPT opt)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_PRIME,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//磁珠针清洗维护
LH_ERR CanSubBoard2NeedleBead_MaintainClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_MAINTAIN_CLEAN,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(opt);
    canMasterSendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//磁珠针旋转指定位置
LH_ERR CanSubBoard2NeedleBead_Rotate(NEEDLE_BEAD_POS targetPos)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_ROTATE,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(targetPos);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//磁珠针吸液
LH_ERR CanSubBoard2NeedleBead_LiquidAbsorb(uint16_t CountMinUl,uint16_t lastLiquidDetectPos,uint16_t* currentLiquidDetectPos,uint16_t* pumpMinUlCount,
                                            LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_LIQUID_ABSORB,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    CanBaseWriteUint16DataToBuffer(canMasterSendCmd.paramBuffer,0,CountMinUl);
    CanBaseWriteUint16DataToBuffer(canMasterSendCmd.paramBuffer,2,lastLiquidDetectPos);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//磁珠针注液
LH_ERR CanSubBoard2NeedleBead_LiquidInject(NEEDLE_INJECT_LEVEL injectLevel,uint8_t mixCount)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_LIQUID_INJECT,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(injectLevel);
    canMasterSendCmd.paramBuffer[1] = (uint8_t)(mixCount);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//磁珠针探液校准
LH_ERR CanSubBoard2NeedleBead_LiquidDetectAdjust(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_ADJUST,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//读磁珠针探液板状态
LH_ERR CanSubBoard2NeedleBead_LiquidDetectReadState(LIQUID_DETECT_CONFIG config,uint16_t* needleVol,uint16_t* thresholdValue,uint16_t* slopeValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(config);
    
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//磁珠针试剂余量探测
LH_ERR CanSubBoard2NeedleBead_LiquidResidueCheck(uint16_t* currentLiquidDetectPos,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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

//指定步进电机复位
LH_ERR CanSubBoard2NeedleBead_StepMotorReset(INDEX_MOTOR_NEEDLE_BEAD index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_STEP_MOTOR_RESET,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);
    
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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
LH_ERR CanSubBoard2NeedleBead_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_BEAD index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_STEP_MOTOR_RUN_STEPS,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);
    
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectReadResVol(uint16_t* resValue,uint16_t* volValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_RES_VOL,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectWriteRes(uint16_t resValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_WRITE_RES,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    CanBaseWriteUint16DataToBuffer(canMasterSendCmd.paramBuffer,0,resValue);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectReadNeedleBaseVol(uint16_t* needleBaseVol)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_BASE_VOL,cmdCanConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigNeedleBeadArray);
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















