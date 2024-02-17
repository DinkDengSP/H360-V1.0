/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-16 17:17:30
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 20:27:22
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplMixWash.h"

//清洗混匀模块复位,带零位偏移
LH_ERR AppImplMixWashReset(void)
{
    //参数辅助
    int32_t paramUtil;
    //读清洗混匀配置
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_MOTOR_CONFIG_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplMixWashResetVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplMixWashResetVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//清洗混匀升起来
LH_ERR AppImplMixWashUp(MIX_MODE mixMode)
{
    //参数辅助
    int32_t paramUtil;
    //读清洗混匀配置
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_MOTOR_CONFIG_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplMixWashUpVersion0(mixMode);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplMixWashUpVersion1(mixMode);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//清洗混匀升降降下去
LH_ERR AppImplMixWashDown(void)
{
    //参数辅助
    int32_t paramUtil;
    //读清洗混匀配置
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_MOTOR_CONFIG_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplMixWashDownVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplMixWashDownVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//清洗混匀旋转混匀一次,不带自动升降
LH_ERR AppImplMixWashStartRotate(MIX_MODE mixMode)
{
    //参数辅助
    int32_t paramUtil;
    //读清洗混匀配置
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_MOTOR_CONFIG_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplMixWashStartRotateVersion0(mixMode);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplMixWashStartRotateVersion1(mixMode);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//清洗混匀集成混匀一次
LH_ERR AppImplMixWashRotateIntegrated(MIX_MODE mixMode)
{
    //参数辅助
    int32_t paramUtil;
    //读清洗混匀配置
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_MOTOR_CONFIG_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplMixWashRotateIntegratedVersion0(mixMode);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplMixWashRotateIntegratedVersion1(mixMode);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//指定步进电机复位
LH_ERR AppImplMixWashStepMotorReset(INDEX_MOTOR_MIX_WASH index)
{
    //参数辅助
    int32_t paramUtil;
    //读清洗混匀配置
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_MOTOR_CONFIG_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplMixWashStepMotorResetVersion0(index);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplMixWashStepMotorResetVersion1(index);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//指定步进电机走指定步数
LH_ERR AppImplMixWashStepMotorRunSteps(INDEX_MOTOR_MIX_WASH index,int32_t steps,int32_t* posAfterRun)
{
    //参数辅助
    int32_t paramUtil;
    //读清洗混匀配置
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_MOTOR_CONFIG_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        return AppImplMixWashStepMotorRunStepsVersion0(index,steps,posAfterRun);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplMixWashStepMotorRunStepsVersion1(index,steps,posAfterRun);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}



