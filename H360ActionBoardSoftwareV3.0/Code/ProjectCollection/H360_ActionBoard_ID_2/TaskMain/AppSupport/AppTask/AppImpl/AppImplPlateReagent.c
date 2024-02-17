/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:12:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-17 10:28:38
**FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplPlateReagent.h"


//试剂盘复位
LH_ERR AppImplPlateReagentReset(uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    int32_t paramUtil;
    //版本参数
    AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT,APP_PARAM_SUB_INDEX_PLATE_REAGENT_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据版本号读取不同的API
    if(paramUtil == 0)
    {
        //旧版试剂盘
        return AppImplPlateReagentResetVersion0(currentCupIndex,currentOffset);
    }
    else if(paramUtil == 1)
    {
        //新版试剂盘
        return AppImplPlateReagentResetVersion1(currentCupIndex,currentOffset);
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//试剂盘运行到指定序号杯位,带偏移
LH_ERR AppImplPlateReagentRun2CupWithOffset(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    int32_t paramUtil;
    //版本参数
    AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT,APP_PARAM_SUB_INDEX_PLATE_REAGENT_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据版本号读取不同的API
    if(paramUtil == 0)
    {
        //旧版试剂盘
        return AppImplPlateReagentRun2CupWithOffsetVersion0(targetCupIndex,targetOffset,currentCupIndex,currentOffset);
    }
    else if(paramUtil == 1)
    {
        //新版试剂盘
        return AppImplPlateReagentRun2CupWithOffsetVersion1(targetCupIndex,targetOffset,currentCupIndex,currentOffset);
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//试剂盘运行到指定序号杯位,带扫码偏移
LH_ERR AppImplPlateReagentRun2CupWithOffsetForScan(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    int32_t paramUtil;
    //版本参数
    AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT,APP_PARAM_SUB_INDEX_PLATE_REAGENT_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据版本号读取不同的API
    if(paramUtil == 0)
    {
        //旧版试剂盘
        return AppImplPlateReagentRun2CupWithOffsetForScanVersion0(targetCupIndex,scanOffset,currentCupIndex,currentOffset);
    }
    else if(paramUtil == 1)
    {
        //新版试剂盘
        return AppImplPlateReagentRun2CupWithOffsetForScanVersion1(targetCupIndex,scanOffset,currentCupIndex,currentOffset);
    }
    else
    {
        return LH_ERR_NONE;
    }
}
//试剂盘运行到指定序号杯位,带RFID偏移
LH_ERR AppImplPlateReagentRun2CupWithOffsetForRFID(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    int32_t paramUtil;
    //版本参数
    AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT,APP_PARAM_SUB_INDEX_PLATE_REAGENT_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据版本号读取不同的API
    if(paramUtil == 0)
    {
        //旧版试剂盘
        return AppImplPlateReagentRun2CupWithOffsetForRFIDVersion0(targetCupIndex,scanOffset,currentCupIndex,currentOffset);
    }
    else if(paramUtil == 1)
    {
        //新版试剂盘
        return AppImplPlateReagentRun2CupWithOffsetForRFIDVersion1(targetCupIndex,scanOffset,currentCupIndex,currentOffset);
    }
    else
    {
        return LH_ERR_NONE;
    }
}
//试剂盘运行指定数量个杯位,基于当前位置
LH_ERR AppImplPlateReagentRunAnyCups(uint8_t cups,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset)
{
    int32_t paramUtil;
    //版本参数
    AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT,APP_PARAM_SUB_INDEX_PLATE_REAGENT_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据版本号读取不同的API
    if(paramUtil == 0)
    {
        //旧版试剂盘
        return AppImplPlateReagentRunAnyCupsVersion0(cups,currentCupIndex,currentOffset);
    }
    else if(paramUtil == 1)
    {
        //新版试剂盘
        return AppImplPlateReagentRunAnyCupsVersion1(cups,currentCupIndex,currentOffset);
    }
    else
    {
        return LH_ERR_NONE;
    }
}


//指定步进电机复位
LH_ERR AppImplPlateReagentStepMotorReset(INDEX_MOTOR_PLATE_REAGENT index)
{
    int32_t paramUtil;
    //版本参数
    AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT,APP_PARAM_SUB_INDEX_PLATE_REAGENT_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据版本号读取不同的API
    if(paramUtil == 0)
    {
        //旧版试剂盘
        return AppImplPlateReagentStepMotorResetVersion0(index);
    }
    else if(paramUtil == 1)
    {
        //新版试剂盘
        return AppImplPlateReagentStepMotorResetVersion1(index);
    }
    else
    {
        return LH_ERR_NONE;
    }
}


//指定步进电机走步数
LH_ERR AppImplPlateReagentStepMotorRunSteps(INDEX_MOTOR_PLATE_REAGENT index,int32_t steps,int32_t* posAfterRun)
{
    int32_t paramUtil;
    //版本参数
    AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT,APP_PARAM_SUB_INDEX_PLATE_REAGENT_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据版本号读取不同的API
    if(paramUtil == 0)
    {
        //旧版试剂盘
        return AppImplPlateReagentStepMotorRunStepsVersion0(index,steps,posAfterRun);
    }
    else if(paramUtil == 1)
    {
        //新版试剂盘
        return AppImplPlateReagentStepMotorRunStepsVersion1(index,steps,posAfterRun);
    }
    else
    {
        return LH_ERR_NONE;
    }
}












