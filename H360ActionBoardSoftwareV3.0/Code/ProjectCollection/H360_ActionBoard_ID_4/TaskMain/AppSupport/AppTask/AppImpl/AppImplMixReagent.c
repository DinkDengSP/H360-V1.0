/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:45:40
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-30 14:27:15
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_4\TaskMain\AppSupport\AppTask\AppImpl\AppImplMixReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplMixReagent.h"

//试剂混匀模块复位,带零位偏移
LH_ERR AppImplMixReagentReset(void)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_REAGENT,APP_PARAM_SUB_INDEX_MIX_REAGENT_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixReagentResetVersion0();
        }
        else
        {
            return AppImplMixReagentResetVersion1();
        }
}

//试剂混匀升起来
LH_ERR AppImplMixReagentUp(MIX_MODE mixMode)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_REAGENT,APP_PARAM_SUB_INDEX_MIX_REAGENT_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixReagentUpVersion0(mixMode);
        }
        else
        {
            return AppImplMixReagentUpVersion1(mixMode);
        }
}

//试剂混匀升降降下去
LH_ERR AppImplMixReagentDown(void)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_REAGENT,APP_PARAM_SUB_INDEX_MIX_REAGENT_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixReagentDownVersion0();
        }
        else
        {
            return AppImplMixReagentDownVersion1();
        }
}

//试剂混匀旋转混匀一次,不带自动升降
LH_ERR AppImplMixReagentStartRotate(MIX_MODE mixMode)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_REAGENT,APP_PARAM_SUB_INDEX_MIX_REAGENT_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixReagentStartRotateVersion0(mixMode);
        }
        else
        {
            return AppImplMixReagentStartRotateVersion1(mixMode);
        }
}

//试剂混匀集成混匀一次
LH_ERR AppImplMixReagentRotateIntegrated(MIX_MODE mixMode)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_REAGENT,APP_PARAM_SUB_INDEX_MIX_REAGENT_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixReagentRotateIntegratedVersion0(mixMode);
        }
        else
        {
            return AppImplMixReagentRotateIntegratedVersion1(mixMode);
        }
}

//指定步进电机复位
LH_ERR AppImplMixReagentStepMotorReset(INDEX_MOTOR_MIX_REAGENT index)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_REAGENT,APP_PARAM_SUB_INDEX_MIX_REAGENT_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixReagentStepMotorResetVersion0(index);
        }
        else
        {
            return AppImplMixReagentStepMotorResetVersion1(index);
        }
}

//指定步进电机走指定步数
LH_ERR AppImplMixReagentStepMotorRunSteps(INDEX_MOTOR_MIX_REAGENT index,int32_t steps,int32_t* posAfterRun)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_REAGENT,APP_PARAM_SUB_INDEX_MIX_REAGENT_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixReagentStepMotorRunStepsVersion0(index,steps,posAfterRun);
        }
        else
        {
            return AppImplMixReagentStepMotorRunStepsVersion1(index,steps,posAfterRun);
        }
}




