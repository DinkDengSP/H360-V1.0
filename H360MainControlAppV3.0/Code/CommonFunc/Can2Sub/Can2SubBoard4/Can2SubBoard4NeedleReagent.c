/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 16:00:11
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-08 14:44:33
**FilePath: \Code\CommonFunc\Can2SubBoard4\Can2SubBoard4NeedleReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard4NeedleReagent.h"

//指令板号
#define BOARD_ID_NEEDLE_REAGENT     CAN2_SUB_BOARD4

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
    CMD_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK         = 0X0000000E,//试剂针余量检查
    CMD_NEEDLE_REAGENT_READ_UPDOWN_EMSTOP_POS       = 0X0000000F,//读试剂针升降急停高度
    CMD_NEEDLE_REAGENT_READ_LIQUID_BOARD_VERSION    = 0X00000010,//读试剂针探液板版本号
}CMD_NEEDLE_REAGENT;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigNeedleReagentArray[] = {
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,0 ,CAN2_SUB_DATA_NULL                 },//试剂针复位
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,2  ,60000   ,0 ,CAN2_SUB_DATA_NULL                 },//试剂针清洗
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL                 },//试剂针灌注
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,2  ,60000   ,0 ,CAN2_SUB_DATA_NULL                 },//试剂针清洗维护
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL                 },//试剂针旋转指定位置
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,5  ,60000   ,13 ,CAN2_SUB_DATA_MODULE_NEEDLE_REAGENT},//试剂针吸液
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,5  ,60000   ,0 ,CAN2_SUB_DATA_NULL                 },//试剂针注液
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,0 ,CAN2_SUB_DATA_NULL                 },//试剂针探液校准
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,1  ,60000   ,8 ,CAN2_SUB_DATA_MODULE_NEEDLE_REAGENT},//读试剂针探液板状态
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL                 },//指定步进电机复位
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,5  ,60000   ,7 ,CAN2_SUB_DATA_MODULE_MOTOR         },//指定步进电机走步数
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,6 ,CAN2_SUB_DATA_MODULE_NEEDLE_REAGENT},//读取针尖电阻和针尖电压
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,2  ,60000   ,0 ,CAN2_SUB_DATA_NULL                 },//写入探液板电阻值
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,4 ,CAN2_SUB_DATA_MODULE_NEEDLE_REAGENT},//读取针尖基准电压
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,6 ,CAN2_SUB_DATA_MODULE_NEEDLE_REAGENT},//试剂针余量检查
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,4 ,CAN2_SUB_DATA_MODULE_NEEDLE_REAGENT},//读试剂针升降急停高度
    {BOARD_ID_NEEDLE_REAGENT    ,CAN2_CHANNEL_BOARD4_NEEDLE_REAGENT ,0  ,60000   ,4 ,CAN2_SUB_DATA_MODULE_NEEDLE_REAGENT},//读试剂针探液板版本号
};

//试剂针复位,带零位偏移
LH_ERR Can2SubBoard4NeedleReagent_Reset(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_RESET,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = ;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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

//试剂针清洗
LH_ERR Can2SubBoard4NeedleReagent_Clean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_CLEAN,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(opt);
    can2SendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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

//试剂针灌注
LH_ERR Can2SubBoard4NeedleReagent_Prime(NEEDLE_PRIME_OPT opt)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_PRIME,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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

//试剂针清洗维护
LH_ERR Can2SubBoard4NeedleReagent_MaintainClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_MAINTAIN_CLEAN,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(opt);
    can2SendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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

//试剂针旋转指定位置
LH_ERR Can2SubBoard4NeedleReagent_Rotate(NEEDLE_REAGENT_POS targetPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_ROTATE,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(targetPos);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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

//试剂针吸液
LH_ERR Can2SubBoard4NeedleReagent_LiquidAbsorb(uint16_t CountMinUl,uint16_t lastLiquidDetectPos,uint8_t liquidDetectStage,uint16_t* currentLiquidDetectPos,uint16_t* pumpMinUlCount
                                                    ,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult,uint8_t *liquidDetectStageReturn,LIQUID_DETECT_NEEDLE_VOL_UNIT *needleVolReturnPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_LIQUID_ABSORB,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,CountMinUl);
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,2,lastLiquidDetectPos);
    can2SendCmd.paramBuffer[4] = liquidDetectStage;
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentLiquidDetectPos = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    *pumpMinUlCount = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,4);
    *liquidResultRerurn = (LIQUID_FUNC_RESULT)(can2ReadData.recvDataPtr[6]);
    *needleLiquidDetectResult = (LIQUID_NEEDLE_LIQUID_DETECT_RESULT)(can2ReadData.recvDataPtr[7]);
    *liquidDetectStageReturn = can2ReadData.recvDataPtr[8];
    needleVolReturnPtr->liquidDetectBaseVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,9);
    needleVolReturnPtr->liquidDetectNeedleVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,11);
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//试剂针注液
LH_ERR Can2SubBoard4NeedleReagent_LiquidInject(uint32_t cupLiquidUl,uint8_t mixCount)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_LIQUID_INJECT,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint32ToArray(can2SendCmd.paramBuffer,0,cupLiquidUl);
    can2SendCmd.paramBuffer[4] = (uint8_t)(mixCount);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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

//试剂针探液校准
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectAdjust(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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

//读试剂针探液板状态
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectReadState(LIQUID_DETECT_CONFIG config,uint16_t* needleVol,uint16_t* thresholdValue,uint16_t* slopeValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(config);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *needleVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    *thresholdValue = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,4);
    *slopeValue = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,6);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR Can2SubBoard4NeedleReagent_StepMotorReset(INDEX_MOTOR_NEEDLE_REAGENT index)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_STEP_MOTOR_RESET,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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
LH_ERR Can2SubBoard4NeedleReagent_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_REAGENT index,int32_t steps,int32_t* posAfterRun)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_STEP_MOTOR_RUN_STEPS,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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

//读取电阻和针尖电压
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectReadResVol(uint16_t* resValue,uint16_t* volValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_RES_VOL,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *resValue = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    *volValue = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,4);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//写入电阻值
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectWriteRes(uint16_t resValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_WRITE_RES,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,resValue);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
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

//读取针尖基准电压
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectReadNeedleBaseVol(uint16_t* needleBaseVol)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_BASE_VOL,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *needleBaseVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//试剂针试剂余量校准
LH_ERR Can2SubBoard4NeedleReagent_LiquidResidueCheck(uint16_t* currentLiquidDetectPos,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    * currentLiquidDetectPos = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    * liquidResultRerurn = (LIQUID_FUNC_RESULT)(can2ReadData.recvDataPtr[4]);
    * needleLiquidDetectResult = (LIQUID_NEEDLE_LIQUID_DETECT_RESULT)(can2ReadData.recvDataPtr[5]);
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//读试剂针升降急停高度
LH_ERR Can2SubBoard4NeedleReagent_ReadNeedleUpDownEmStopPos(uint16_t* needleEmStopPosPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_READ_UPDOWN_EMSTOP_POS,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *needleEmStopPosPtr = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//读试剂针探液板版本号
LH_ERR Can2SubBoard4NeedleReagent_ReadLiquidBoardVersion(uint16_t* boardVersionPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_REAGENT_READ_LIQUID_BOARD_VERSION,cmdCan2ConfigNeedleReagentArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleReagentArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *boardVersionPtr = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}








