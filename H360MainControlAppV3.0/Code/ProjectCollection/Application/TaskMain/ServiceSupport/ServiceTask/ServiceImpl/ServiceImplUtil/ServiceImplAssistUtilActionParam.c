/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-17 14:52:39
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-16 20:47:58
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplAssistUtilActionParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplAssistUtilActionParam.h"

//读参数方法指针
typedef LH_ERR (*ServiceImplAssistUtilActionParam_ReadFuncPtr)(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);
//写参数方法指针
typedef LH_ERR (*ServiceImplAssistUtilActionParam_WriteFuncPtr)(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);


/***********************************************************************************杯栈管理0X00**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_CupTrayStackManage(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard2Param_Read(INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_CupTrayStackManage(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard2Param_Write(INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************新杯机械手0X01**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_HandNewCup(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard5Param_Read(INDEX_CAN2_SUB_BOARD5_PARAM_HAND_NEW_CUP,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_HandNewCup(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard5Param_Write(INDEX_CAN2_SUB_BOARD5_PARAM_HAND_NEW_CUP,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************清洗机械手0X02**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_HandWash(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard3Param_Read(INDEX_CAN2_SUB_BOARD3_PARAM_HAND_WASH,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_HandWash(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard3Param_Write(INDEX_CAN2_SUB_BOARD3_PARAM_HAND_WASH,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************反应盘0X03**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_PlateReaction(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard4Param_Read(INDEX_CAN2_SUB_BOARD2_PARAM_PLATE_REACTION,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_PlateReaction(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard4Param_Write(INDEX_CAN2_SUB_BOARD2_PARAM_PLATE_REACTION,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************试剂盘0X04**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_PlateReagent(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(subIndex < 0x80)
    {
        errorCode = Can2SubBoard2Param_Read(INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT,subIndex,paramRead);
    }
    else
    {
        switch (subIndex)
        {
        case 0X80:
            errorCode = ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_PLATE_REAGENT,INDEX_SUB_SERVICE_PARAM_PLATE_REAGENT_BARCODE_SCAN_VERSION,paramRead);
            break;
        case 0X81:
            errorCode = ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_PLATE_REAGENT,INDEX_SUB_SERVICE_PARAM_PLATE_REAGENT_RFID_LOG_CHANNEL,paramRead);
            break;
        case 0X82:
            errorCode = ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_PLATE_REAGENT,INDEX_SUB_SERVICE_PARAM_PLATE_REAGENT_LOG_CHANNEL_UART_OR_SDCARD,paramRead);
            break;
        default:
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_READ;
            break;
        }
    }
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_PlateReagent(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(subIndex < 0X80)
    {
        errorCode = Can2SubBoard2Param_Write(INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT,subIndex,paramWrite);
    }
    else
    {
        switch (subIndex)
        {
        case 0X80:
            errorCode = ServiceParamWriteSingle(INDEX_MAIN_SERVICE_PARAM_PLATE_REAGENT,INDEX_SUB_SERVICE_PARAM_PLATE_REAGENT_BARCODE_SCAN_VERSION,paramWrite);
            break;
        case 0X81:
            errorCode = ServiceParamWriteSingle(INDEX_MAIN_SERVICE_PARAM_PLATE_REAGENT,INDEX_SUB_SERVICE_PARAM_PLATE_REAGENT_RFID_LOG_CHANNEL,paramWrite);
            break;
        case 0X82:
            errorCode = ServiceParamWriteSingle(INDEX_MAIN_SERVICE_PARAM_PLATE_REAGENT,INDEX_SUB_SERVICE_PARAM_PLATE_REAGENT_LOG_CHANNEL_UART_OR_SDCARD,paramWrite);
            break;
        default:
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
            break;
        }
    }
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************清洗盘0X05**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_PlateWash(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard1Param_Read(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_PLATE_WASH,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_PlateWash(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard1Param_Write(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_PLATE_WASH,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************磁珠摇匀0X06**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_MixBead(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard4Param_Read(INDEX_CAN2_SUB_BOARD2_PARAM_MIX_BEAD,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_MixBead(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard4Param_Write(INDEX_CAN2_SUB_BOARD2_PARAM_MIX_BEAD,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************试剂摇匀0X07**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_MixReagent(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard4Param_Read(INDEX_CAN2_SUB_BOARD2_PARAM_MIX_REAGENT,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_MixReagent(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard4Param_Write(INDEX_CAN2_SUB_BOARD2_PARAM_MIX_REAGENT,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************清洗摇匀0X08**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_MixWash(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard1Param_Read(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_MIX_WASH,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_MixWash(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard1Param_Write(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_MIX_WASH,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/************************************************************************************样本针0X09*********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_NeedleSample(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(subIndex < 0x80)
    {
       errorCode = Can2SubBoard5Param_Read(INDEX_CAN2_SUB_BOARD5_PARAM_NEEDLE_SAMPLE,subIndex,paramRead); 
    }
    else
    {
        switch (subIndex)
        {
        case 0x80:
            errorCode = ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_NEEDLE_SAMPLE,INDEX_SUB_SERVICE_PARAM_NEEDLE_SAMPLE_CLEAN_BLOCK_THRESHOLE,paramRead);//清洗堵针阈值
            break;
        default:
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_READ;
            break;
        }
    }
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_NeedleSample(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(subIndex < 0x80)
    {
        errorCode = Can2SubBoard5Param_Write(INDEX_CAN2_SUB_BOARD5_PARAM_NEEDLE_SAMPLE,subIndex,paramWrite);
    }
    else
    {
        switch (subIndex)
        {
        case 0x80:
            errorCode = ServiceParamWriteSingle(INDEX_MAIN_SERVICE_PARAM_NEEDLE_SAMPLE,INDEX_SUB_SERVICE_PARAM_NEEDLE_SAMPLE_CLEAN_BLOCK_THRESHOLE,paramWrite);//清洗堵针阈值
            break;
        default:
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
            break;
        }
    }
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************样本针探液0X0A**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_NeedleSampleLiquidDetect(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //不能为0
    if(subIndex == 0)
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    if((subIndex == 0x80)||(subIndex == 0x81))
    {
        uint16_t resValue,volValue;
        
        errorCode = Can2SubBoard5NeedleSample_LiquidDetectReadResVol(&resValue,&volValue);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        if(subIndex == 0x80)
        {
            //读针尖电压
            *paramRead = (uint32_t)volValue;
        }
        else
        {
            //读探液板电阻
            *paramRead = (uint32_t)resValue;
        }
    }
    else if(subIndex == 0X83)
    {
        //读取探液板版本
        uint16_t liquidBoardVersion;
        errorCode = Can2SubBoard5NeedleSample_ReadLiquidBoardVersion(&liquidBoardVersion);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        *paramRead = (uint32_t)liquidBoardVersion;
    }
    else if(subIndex == 0x82)
    {
        //读取针尖基准电压
        uint16_t needleBaseVol;
        errorCode = Can2SubBoard5NeedleSample_LiquidDetectReadNeedleBaseVol(&needleBaseVol);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        *paramRead = (uint32_t)needleBaseVol;
    }
    else if(subIndex < 0x80)
    {
        //探液配置,最多可以支持63对配置
        uint8_t localUtilIndex = subIndex;
        //读取各种配置,检查序号
        if((localUtilIndex % 2) == 0)
        {
            localUtilIndex -= 1;
        }
        localUtilIndex = (localUtilIndex/2);
        LIQUID_DETECT_CONFIG liquidConfig = (LIQUID_DETECT_CONFIG)(localUtilIndex);
        if(liquidConfig > LIQUID_DETECT_CONFIG_MAX)
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //读取针尖状态
        uint16_t needleVol,thresholdValue,slopeValue;
        errorCode = Can2SubBoard5NeedleSample_LiquidDetectReadState(liquidConfig,&needleVol,&thresholdValue,&slopeValue);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //根据序号决定返回值,单数返回阈值,双数返回斜率
        if((subIndex%2) != 0)
        {
            *paramRead = (int32_t)(thresholdValue);
        }
        else
        {
            *paramRead = (int32_t)(slopeValue);
        }
    }
    else
    {
        //不支持的参数
        errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_READ;
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_NeedleSampleLiquidDetect(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(subIndex == 0x81)
    {
        //写入探液板电阻
        uint16_t liquidRes = (uint16_t)paramWrite;
        //最大1023写入
        if(liquidRes > 1023)
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //写入电阻值
        errorCode = Can2SubBoard5NeedleSample_LiquidDetectWriteRes(liquidRes);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    }
    else
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************试剂针0X0B**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_NeedleReagent(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard4Param_Read(INDEX_CAN2_SUB_BOARD2_PARAM_NEEDLE_REAGENT,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_NeedleReagent(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard4Param_Write(INDEX_CAN2_SUB_BOARD2_PARAM_NEEDLE_REAGENT,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************试剂针探液0X0C**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_NeedleReagentLiquidDetect(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //不能为0
    if(subIndex == 0)
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    if((subIndex == 0x80)||(subIndex == 0x81))
    {
        uint16_t resValue,volValue;
        
        errorCode = Can2SubBoard4NeedleReagent_LiquidDetectReadResVol(&resValue,&volValue);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        if(subIndex == 0x80)
        {
            //读针尖电压
            *paramRead = (uint32_t)volValue;
        }
        else
        {
            //读探液板电阻
            *paramRead = (uint32_t)resValue;
        }
    }
    else if(subIndex == 0x82)
    {
        //读取针尖基准电压
        uint16_t needleBaseVol;
        errorCode = Can2SubBoard4NeedleReagent_LiquidDetectReadNeedleBaseVol(&needleBaseVol);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        *paramRead = (uint32_t)needleBaseVol;
    }
    else if(subIndex == 0x83)
    {
        //读取探液板版本
        uint16_t liquidBoardVersion;
        errorCode = Can2SubBoard4NeedleReagent_ReadLiquidBoardVersion(&liquidBoardVersion);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        *paramRead = (uint32_t)liquidBoardVersion;
    }
    else if(subIndex < 0x80)
    {
        //探液配置,最多可以支持63对配置
        uint8_t localUtilIndex = subIndex;
        //读取各种配置,检查序号
        if((localUtilIndex % 2) == 0)
        {
            localUtilIndex -= 1;
        }
        localUtilIndex = (localUtilIndex/2);
        LIQUID_DETECT_CONFIG liquidConfig = (LIQUID_DETECT_CONFIG)(localUtilIndex);
        if(liquidConfig > LIQUID_DETECT_CONFIG_MAX)
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //读取针尖状态
        uint16_t needleVol,thresholdValue,slopeValue;
        errorCode = Can2SubBoard4NeedleReagent_LiquidDetectReadState(liquidConfig,&needleVol,&thresholdValue,&slopeValue);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //根据序号决定返回值,单数返回阈值,双数返回斜率
        if((subIndex%2) != 0)
        {
            *paramRead = (int32_t)(thresholdValue);
        }
        else
        {
            *paramRead = (int32_t)(slopeValue);
        }
    }
    else
    {
        //不支持的参数
        errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_READ;
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_NeedleReagentLiquidDetect(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(subIndex == 0x81)
    {
        //写入探液板电阻
        uint16_t liquidRes = (uint16_t)paramWrite;
        //最大1023写入
        if(liquidRes > 1023)
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //写入电阻值
        errorCode = Can2SubBoard4NeedleReagent_LiquidDetectWriteRes(liquidRes);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    }
    else
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}   

/***********************************************************************************磁珠针0X0D**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_NeedleBead(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{   
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard2Param_Read(INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_NEEDLE_BEAD,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_NeedleBead(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard2Param_Write(INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_NEEDLE_BEAD,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************磁珠针探液0X0E**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_NeedleBeadLiquidDetect(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //不能为0
    if(subIndex == 0)
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    if((subIndex == 0x80)||(subIndex == 0x81))
    {
        uint16_t resValue,volValue;
        
        errorCode = Can2SubBoard2NeedleBead_LiquidDetectReadResVol(&resValue,&volValue);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        if(subIndex == 0x80)
        {
            //读针尖电压
            *paramRead = (uint32_t)volValue;
        }
        else
        {
            //读探液板电阻
            *paramRead = (uint32_t)resValue;
        }
    }
    else if(subIndex == 0x82)
    {
        //读取针尖基准电压
        uint16_t needleBaseVol;
        errorCode = Can2SubBoard2NeedleBead_LiquidDetectReadNeedleBaseVol(&needleBaseVol);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        *paramRead = (uint32_t)needleBaseVol;
    }
    else if(subIndex == 0x83)
    {
        //读取探液板版本
        uint16_t liquidBoardVersion;
        errorCode = Can2SubBoard2NeedleBead_ReadLiquidBoardVersion(&liquidBoardVersion);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        *paramRead = (uint32_t)liquidBoardVersion;
    }
    else if(subIndex < 0x80)
    {
        //探液配置,最多可以支持63对配置
        uint8_t localUtilIndex = subIndex;
        //读取各种配置,检查序号
        if((localUtilIndex % 2) == 0)
        {
            localUtilIndex -= 1;
        }
        localUtilIndex = (localUtilIndex/2);
        LIQUID_DETECT_CONFIG liquidConfig = (LIQUID_DETECT_CONFIG)(localUtilIndex);
        if(liquidConfig > LIQUID_DETECT_CONFIG_MAX)
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //读取针尖状态
        uint16_t needleVol,thresholdValue,slopeValue;
        errorCode = Can2SubBoard2NeedleBead_LiquidDetectReadState(liquidConfig,&needleVol,&thresholdValue,&slopeValue);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //根据序号决定返回值,单数返回阈值,双数返回斜率
        if((subIndex%2) != 0)
        {
            *paramRead = (int32_t)(thresholdValue);
        }
        else
        {
            *paramRead = (int32_t)(slopeValue);
        }
    }
    else
    {
        //不支持的参数
        errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_READ;
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_NeedleBeadLiquidDetect(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(subIndex == 0x81)
    {
        //写入探液板电阻
        uint16_t liquidRes = (uint16_t)paramWrite;
        //最大1023写入
        if(liquidRes > 1023)
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //写入电阻值
        errorCode = Can2SubBoard2NeedleBead_LiquidDetectWriteRes(liquidRes);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    }
    else
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_APP_PARAM_WRITE;
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_SUB;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************清洗针0X0F**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_NeedleWash(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard1Param_Read(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_NEEDLE_WASH,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_NeedleWash(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard1Param_Write(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_NEEDLE_WASH,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************样本架传送带0X10**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_SampleRackTrack(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard5Param_Read(INDEX_CAN2_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_SampleRackTrack(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard5Param_Write(INDEX_CAN2_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************缓冲液注液0X11**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_BufferSolutionInject(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard1Param_Read(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_BUFFER_INJECT,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_BufferSolutionInject(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard1Param_Write(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_BUFFER_INJECT,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************测量模块0X12**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_MeasureUnit(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard3Param_Read(INDEX_CAN2_SUB_BOARD3_PARAM_MEASURE_MODULE,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_MeasureUnit(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    errorCode = Can2SubBoard3Param_Write(INDEX_CAN2_SUB_BOARD3_PARAM_MEASURE_MODULE,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

/***********************************************************************************液路自动化0X13**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_LiquidAuto(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    int32_t paramRead1;
    int32_t paramRead2;
    //正常的液路自动化数据
    if(subIndex < 0X80)
    {
        //读取主控
        errorCode = ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_LIQUID_AUTO,subIndex,&paramRead1);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //读取下位机
        errorCode = Can2SubBoard1Param_Read(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO,subIndex,&paramRead2);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //如果主控和下位机数据不一致,将主控参数写入下位机
        if(paramRead1 != paramRead2)
        {
            errorCode = Can2SubBoard1Param_Write(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO,subIndex,paramRead1);
            if(errorCode != LH_ERR_NONE)
            {
                *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
                return errorCode;
            }
        }
    }
    else if(subIndex == 0X80)
    {
        uint32_t count = 0;
        //读取流量计脉冲计数
        errorCode = Can2SubLiquidSensorGetCount(&count);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        paramRead1 = count;
    }
    *paramRead = paramRead1;
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_LiquidAuto(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(subIndex < 0X80)
    {
        //写入下位机
        errorCode = Can2SubBoard1Param_Write(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO,subIndex,paramWrite);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
        //写入主控
        errorCode = ServiceParamWriteSingle(INDEX_MAIN_SERVICE_PARAM_LIQUID_AUTO,subIndex,paramWrite);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
    }
    else if(subIndex == 0X80)
    {
        //清除流量计脉冲计数
        errorCode = Can2SubLiquidSensorClearCount();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
    }
    return errorCode;
}

/***********************************************************************************温控0X14**********************************************************************************/
//读取参数
static LH_ERR ServiceImplAssistUtilActionParam_Read_TC(uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    float paramTempCtrl = 0.0;
    switch(subIndex)
    {
        case 1://PARAM_INDEX_REACTION_TARGET
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_TARGET_TEMP,&paramTempCtrl);
            break;
        case 2://PARAM_INDEX_REACTION_KP
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KP,&paramTempCtrl);
            break;
        case 3://PARAM_INDEX_REACTION_KI
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KI,&paramTempCtrl);
            break;
        case 4://PARAM_INDEX_REACTION_KD
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KD,&paramTempCtrl);
            break;
        case 5://PARAM_INDEX_REACTION_HIGH_THRESHOLD
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_HIGH_THRESHOLD,&paramTempCtrl);
            break;
        case 6://PARAM_INDEX_REACTION_LOW_THRESHOLD
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_LOW_THRESHOLD,&paramTempCtrl);
            break;
        case 7://PARAM_INDEX_REACTION_POWER_MAX
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_MAX_POWER,&paramTempCtrl);
            break;
        case 8://PARAM_INDEX_REACTION_POWER_MIN
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_MIN_POWER,&paramTempCtrl);
            break;
        case 9://PARAM_INDEX_REACTION_HIGH_PROTECT
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_HIGH_PROTECT,&paramTempCtrl);
            break;
        case 10://PARAM_INDEX_REACTION_LOW_PROTECT
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_LOW_PROTECT,&paramTempCtrl);
            break;
        case 11://PARAM_INDEX_REACTION_KI_TIME
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KI_TIME_SECOND,&paramTempCtrl);
            break;
        case 12://PARAM_INDEX_REACTION_KD_TIME
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KD_TIME_SECOND,&paramTempCtrl);
            break;
        case 13://PARAM_INDEX_WASH_TARGET
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_TARGET_TEMP,&paramTempCtrl);
            break;
        case 14://PARAM_INDEX_WASH_KP
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KP,&paramTempCtrl);
            break;
        case 15://PARAM_INDEX_WASH_KI
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KI,&paramTempCtrl);
            break;
        case 16://PARAM_INDEX_WASH_KD
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KD,&paramTempCtrl);
            break;
        case 17://PARAM_INDEX_WASH_HIGH_THRESHOLD
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_HIGH_THRESHOLD,&paramTempCtrl);
            break;
        case 18://PARAM_INDEX_WASH_LOW_THRESHOLD
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_LOW_THRESHOLD,&paramTempCtrl);
            break;
        case 19://PARAM_INDEX_WASH_POWER_MAX
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_MAX_POWER,&paramTempCtrl);
            break;
        case 20://PARAM_INDEX_WASH_POWER_MIN
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_MIN_POWER,&paramTempCtrl);
            break;
        case 21://PARAM_INDEX_WASH_HIGH_PROTECT
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_HIGH_PROTECT,&paramTempCtrl);
            break;
        case 22://PARAM_INDEX_WASH_LOW_PROTECT
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_LOW_PROTECT,&paramTempCtrl);
            break;
        case 23://PARAM_INDEX_WASH_KI_TIME
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KI_TIME_SECOND,&paramTempCtrl);
            break;
        case 24://PARAM_INDEX_WASH_KD_TIME
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KD_TIME_SECOND,&paramTempCtrl);
            break;
        case 25://PARAM_INDEX_REAGENT_TARGET
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_TARGET_TEMP,&paramTempCtrl);
            break;
        case 26://PARAM_INDEX_REAGENT_POWER_MAX
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_MAX_POWER,&paramTempCtrl);
            break;
        case 27://PARAM_INDEX_REAGENT_POWER_MIN
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_MIN_POWER,&paramTempCtrl);
            break;
        case 28://PARAM_INDEX_REAGENT_PROTECT_HIGH
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_HIGH_PROTECT,&paramTempCtrl);
            break;
        case 29://PARAM_INDEX_REAGENT_PROTECT_LOW
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_LOW_PROTECT,&paramTempCtrl);
            break;
        case 30://环境温度
            errorCode = Can2SubBoardTcStateReadEnvirTemp(&paramTempCtrl);
            break;
        case 31://反应盘温度
            errorCode = Can2SubBoardTcStateReadPlateReactionTemp(&paramTempCtrl);
            break;
        case 32://清洗盘温度
            errorCode = Can2SubBoardTcStateReadPlateWashTemp(&paramTempCtrl);
            break;
        case 33://试剂盘温度
            errorCode = Can2SubBoardTcStateReadPlateReagentTemp(&paramTempCtrl);
            break;
        case 34://反应盘监控界面温度修正,放大十倍
            errorCode = ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_TC,INDEX_SUB_SERVICE_PARAM_TC_TEMP_CORRECT_SHOW_PLATE_REACTION,paramRead);
            break;
        case 35://清洗盘监控界面温度修正,放大十倍
            errorCode = ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_TC,INDEX_SUB_SERVICE_PARAM_TC_TEMP_CORRECT_SHOW_PLATE_WASH,paramRead);
            break;
        case 36://试剂盘监控界面温度修正,放大十倍
            errorCode = ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_TC,INDEX_SUB_SERVICE_PARAM_TC_TEMP_CORRECT_SHOW_PLATE_REAGENT,paramRead);
            break;
        case 37://试剂盘制冷装置
            errorCode = Can2SubBoardTcParamRead(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_COOLER_DEVICE,&paramTempCtrl);
            break;
        default://无效参数
            errorCode = LH_ERR_BOARD_MAIN_COMM_TC_PARAM_INDEX;
            break;
    }
    //读取成功,数据转换
    if(errorCode == LH_ERR_NONE)
    {
        if((subIndex == 34)||(subIndex == 35)||(subIndex == 36))
        {
            //不转换
            asm("nop");
        }
        else if(subIndex == 37)
        {
            *paramRead = (uint32_t)(paramTempCtrl);
        }
        else
        {
            //将读取到的浮点数放大100倍数
            *paramRead = (int32_t)(paramTempCtrl *100.0);
        }
    }
    else
    {
        *paramRead = 0;
    }
    //读取失败,错误处理
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}

//写入参数
static LH_ERR ServiceImplAssistUtilActionParam_Write_TC(uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

    float tempCtrlWrite = paramWrite;
    tempCtrlWrite /= 100.0;
    switch(subIndex)
    {
        case 1://PARAM_INDEX_REACTION_TARGET
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_TARGET_TEMP,tempCtrlWrite);
            break;
        case 2://PARAM_INDEX_REACTION_KP
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KP,tempCtrlWrite);
            break;
        case 3://PARAM_INDEX_REACTION_KI
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KI,tempCtrlWrite);
            break;
        case 4://PARAM_INDEX_REACTION_KD
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KD,tempCtrlWrite);
            break;
        case 5://PARAM_INDEX_REACTION_HIGH_THRESHOLD
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_HIGH_THRESHOLD,tempCtrlWrite);
            break;
        case 6://PARAM_INDEX_REACTION_LOW_THRESHOLD
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_LOW_THRESHOLD,tempCtrlWrite);
            break;
        case 7://PARAM_INDEX_REACTION_POWER_MAX
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_MAX_POWER,tempCtrlWrite);
            break;
        case 8://PARAM_INDEX_REACTION_POWER_MIN
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_MIN_POWER,tempCtrlWrite);
            break;
        case 9://PARAM_INDEX_REACTION_HIGH_PROTECT
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_HIGH_PROTECT,tempCtrlWrite);
            break;
        case 10://PARAM_INDEX_REACTION_LOW_PROTECT
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_LOW_PROTECT,tempCtrlWrite);
            break;
        case 11://PARAM_INDEX_REACTION_KI_TIME
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KI_TIME_SECOND,tempCtrlWrite);
            break;
        case 12://PARAM_INDEX_REACTION_KD_TIME
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REACTION,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REACTION_KD_TIME_SECOND,tempCtrlWrite);
            break;
        case 13://PARAM_INDEX_WASH_TARGET
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_TARGET_TEMP,tempCtrlWrite);
            break;
        case 14://PARAM_INDEX_WASH_KP
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KP,tempCtrlWrite);
            break;
        case 15://PARAM_INDEX_WASH_KI
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KI,tempCtrlWrite);
            break;
        case 16://PARAM_INDEX_WASH_KD
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KD,tempCtrlWrite);
            break;
        case 17://PARAM_INDEX_WASH_HIGH_THRESHOLD
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_HIGH_THRESHOLD,tempCtrlWrite);
            break;
        case 18://PARAM_INDEX_WASH_LOW_THRESHOLD
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_LOW_THRESHOLD,tempCtrlWrite);
            break;
        case 19://PARAM_INDEX_WASH_POWER_MAX
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_MAX_POWER,tempCtrlWrite);
            break;
        case 20://PARAM_INDEX_WASH_POWER_MIN
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_MIN_POWER,tempCtrlWrite);
            break;
        case 21://PARAM_INDEX_WASH_HIGH_PROTECT
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_HIGH_PROTECT,tempCtrlWrite);
            break;
        case 22://PARAM_INDEX_WASH_LOW_PROTECT
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_LOW_PROTECT,tempCtrlWrite);
            break;
        case 23://PARAM_INDEX_WASH_KI_TIME
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KI_TIME_SECOND,tempCtrlWrite);
            break;
        case 24://PARAM_INDEX_WASH_KD_TIME
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_WASH,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_WASH_KD_TIME_SECOND,tempCtrlWrite);
            break;
        case 25://PARAM_INDEX_REAGENT_TARGET
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_TARGET_TEMP,tempCtrlWrite);
            break;
        case 26://PARAM_INDEX_REAGENT_POWER_MAX
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_MAX_POWER,tempCtrlWrite);
            break;
        case 27://PARAM_INDEX_REAGENT_POWER_MIN
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_MIN_POWER,tempCtrlWrite);
            break;
        case 28://PARAM_INDEX_REAGENT_PROTECT_HIGH
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_HIGH_PROTECT,tempCtrlWrite);
            break;
        case 29://PARAM_INDEX_REAGENT_PROTECT_LOW
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_LOW_PROTECT,tempCtrlWrite);
            break;
        case 34://反应盘监控界面温度修正,放大十倍
            errorCode = ServiceParamWriteSingle(INDEX_MAIN_SERVICE_PARAM_TC,INDEX_SUB_SERVICE_PARAM_TC_TEMP_CORRECT_SHOW_PLATE_REACTION,paramWrite);
            break;
        case 35://清洗盘监控界面温度修正,放大十倍
            errorCode = ServiceParamWriteSingle(INDEX_MAIN_SERVICE_PARAM_TC,INDEX_SUB_SERVICE_PARAM_TC_TEMP_CORRECT_SHOW_PLATE_WASH,paramWrite);
            break;
        case 36://试剂盘监控界面温度修正,放大十倍
            errorCode = ServiceParamWriteSingle(INDEX_MAIN_SERVICE_PARAM_TC,INDEX_SUB_SERVICE_PARAM_TC_TEMP_CORRECT_SHOW_PLATE_REAGENT,paramWrite);
            break;
        case 37://试剂盘制冷装置
            errorCode = Can2SubBoardTcParamWrite(CAN2_SUB_BOARD_TC_PARAM_MAIN_PLATE_REAGENT,CAN2_SUB_BOARD_TC_PARAM_SUB_PLATE_REAGENT_COOLER_DEVICE,(float)(paramWrite));
            break;        
        default://无效参数
            errorCode = LH_ERR_BOARD_MAIN_COMM_TC_PARAM_INDEX;
            break;
    }
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}


//读取数据的函数列表
static const ServiceImplAssistUtilActionParam_ReadFuncPtr serviceImplAssistUtilActionParam_ReadFuncPtrArray[] = {
    ServiceImplAssistUtilActionParam_Read_CupTrayStackManage,
    ServiceImplAssistUtilActionParam_Read_HandNewCup,
    ServiceImplAssistUtilActionParam_Read_HandWash,
    ServiceImplAssistUtilActionParam_Read_PlateReaction,
    ServiceImplAssistUtilActionParam_Read_PlateReagent,
    ServiceImplAssistUtilActionParam_Read_PlateWash,
    ServiceImplAssistUtilActionParam_Read_MixBead,
    ServiceImplAssistUtilActionParam_Read_MixReagent,
    ServiceImplAssistUtilActionParam_Read_MixWash,
    ServiceImplAssistUtilActionParam_Read_NeedleSample,
    ServiceImplAssistUtilActionParam_Read_NeedleSampleLiquidDetect,
    ServiceImplAssistUtilActionParam_Read_NeedleReagent,
    ServiceImplAssistUtilActionParam_Read_NeedleReagentLiquidDetect,
    ServiceImplAssistUtilActionParam_Read_NeedleBead,
    ServiceImplAssistUtilActionParam_Read_NeedleBeadLiquidDetect,
    ServiceImplAssistUtilActionParam_Read_NeedleWash,
    ServiceImplAssistUtilActionParam_Read_SampleRackTrack,
    ServiceImplAssistUtilActionParam_Read_BufferSolutionInject,
    ServiceImplAssistUtilActionParam_Read_MeasureUnit,
    ServiceImplAssistUtilActionParam_Read_LiquidAuto,
    ServiceImplAssistUtilActionParam_Read_TC,
};
#define LENGTH_ACTION_PARAM_READ_FUNC_PTR_ARRAY     ((sizeof(serviceImplAssistUtilActionParam_ReadFuncPtrArray))/(sizeof(serviceImplAssistUtilActionParam_ReadFuncPtrArray[0])))

//写入数据的函数列表
static const ServiceImplAssistUtilActionParam_WriteFuncPtr serviceImplAssistUtilActionParam_WriteFuncPtrArray[] = {
    ServiceImplAssistUtilActionParam_Write_CupTrayStackManage,
    ServiceImplAssistUtilActionParam_Write_HandNewCup,
    ServiceImplAssistUtilActionParam_Write_HandWash,
    ServiceImplAssistUtilActionParam_Write_PlateReaction,
    ServiceImplAssistUtilActionParam_Write_PlateReagent,
    ServiceImplAssistUtilActionParam_Write_PlateWash,
    ServiceImplAssistUtilActionParam_Write_MixBead,
    ServiceImplAssistUtilActionParam_Write_MixReagent,
    ServiceImplAssistUtilActionParam_Write_MixWash,
    ServiceImplAssistUtilActionParam_Write_NeedleSample,
    ServiceImplAssistUtilActionParam_Write_NeedleSampleLiquidDetect,
    ServiceImplAssistUtilActionParam_Write_NeedleReagent,
    ServiceImplAssistUtilActionParam_Write_NeedleReagentLiquidDetect,
    ServiceImplAssistUtilActionParam_Write_NeedleBead,
    ServiceImplAssistUtilActionParam_Write_NeedleBeadLiquidDetect,
    ServiceImplAssistUtilActionParam_Write_NeedleWash,
    ServiceImplAssistUtilActionParam_Write_SampleRackTrack,
    ServiceImplAssistUtilActionParam_Write_BufferSolutionInject,
    ServiceImplAssistUtilActionParam_Write_MeasureUnit,
    ServiceImplAssistUtilActionParam_Write_LiquidAuto,
    ServiceImplAssistUtilActionParam_Write_TC,
};
#define LENGTH_ACTION_PARAM_WRITE_FUNC_PTR_ARRAY     ((sizeof(serviceImplAssistUtilActionParam_WriteFuncPtrArray))/(sizeof(serviceImplAssistUtilActionParam_WriteFuncPtrArray[0])))

//读取动作参数
LH_ERR ServiceImplAssistUtilActionParam_Read(uint8_t mainIndex,uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检测主序号
    if(mainIndex >= LENGTH_ACTION_PARAM_READ_FUNC_PTR_ARRAY)
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_SERVICE_PARAM_READ_MAIN_INDEX;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_MAIN;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    return serviceImplAssistUtilActionParam_ReadFuncPtrArray[mainIndex](subIndex,paramRead,commErrorCode,commErrorLevel);
}

//写入动作参数
LH_ERR ServiceImplAssistUtilActionParam_Write(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检测主序号
    if(mainIndex >= LENGTH_ACTION_PARAM_WRITE_FUNC_PTR_ARRAY)
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_SERVICE_PARAM_WRITE_MAIN_INDEX;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_ACTION_PARAM_INDEX_MAIN;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    return serviceImplAssistUtilActionParam_WriteFuncPtrArray[mainIndex](subIndex,paramWrite,commErrorCode,commErrorLevel);
}


