/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-16 17:18:17
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 20:28:16
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplPlateWash.h"

//清洗盘旋转复位,带零位修正
LH_ERR AppImplPlateWashReset(SENSOR_STATE* cupExistState,uint8_t* cupIndex)
{
    //参数辅助
    int32_t paramUtil;
    //读取液路板本
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO,APP_PARAM_SUB_INDEX_LIQUID_AUTO_PURE_LIQUID_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplPlateWashResetVersion0(cupExistState,cupIndex);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplPlateWashResetVersion1(cupExistState,cupIndex);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//清洗盘移动指定个杯位
LH_ERR AppImplPlateWashRunAnyCups(uint8_t cupsCount,SENSOR_STATE* cupExistState,uint8_t* cupIndex)
{
    //参数辅助
    int32_t paramUtil;
    //读取液路板本
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO,APP_PARAM_SUB_INDEX_LIQUID_AUTO_PURE_LIQUID_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplPlateWashRunAnyCupsVersion0(cupsCount,cupExistState,cupIndex);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplPlateWashRunAnyCupsVersion1(cupsCount,cupExistState,cupIndex);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//清洗盘移动到指定序号杯位
LH_ERR AppImplPlateWashRun2CupWithIndex(uint8_t targetIndex,SENSOR_STATE* cupExistState,uint8_t* cupIndex)
{
    //参数辅助
    int32_t paramUtil;
    //读取液路板本
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO,APP_PARAM_SUB_INDEX_LIQUID_AUTO_PURE_LIQUID_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplPlateWashRun2CupWithIndexVersion0(targetIndex,cupExistState,cupIndex);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplPlateWashRun2CupWithIndexVersion1(targetIndex,cupExistState,cupIndex);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//指定步进电机复位,复位带零位修正
LH_ERR AppImplPlateWashStepMotorReset(INDEX_MOTOR_PLATE_WASH index)
{
    //参数辅助
    int32_t paramUtil;
    //读取液路板本
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO,APP_PARAM_SUB_INDEX_LIQUID_AUTO_PURE_LIQUID_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplPlateWashStepMotorResetVersion0(index);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplPlateWashStepMotorResetVersion1(index);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//指定步进电机走指定步数
LH_ERR AppImplPlateWashStepMotorRunSteps(INDEX_MOTOR_PLATE_WASH index,int32_t steps,int32_t* posAfterRun)
{
    //参数辅助
    int32_t paramUtil;
    //读取液路板本
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO,APP_PARAM_SUB_INDEX_LIQUID_AUTO_PURE_LIQUID_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplPlateWashStepMotorRunStepsVersion0(index,steps,posAfterRun);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplPlateWashStepMotorRunStepsVersion1(index,steps,posAfterRun);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}





