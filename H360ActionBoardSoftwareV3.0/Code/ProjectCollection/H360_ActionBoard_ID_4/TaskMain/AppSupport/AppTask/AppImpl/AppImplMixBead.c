/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:45:04
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-30 14:28:54
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_4\TaskMain\AppSupport\AppTask\AppImpl\AppImplMixBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplMixBead.h"

//磁珠混匀模块复位,带零位偏移
LH_ERR AppImplMixBeadReset(void)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_BEAD,APP_PARAM_SUB_INDEX_MIX_BEAD_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixBeadResetVersion0();
        }
        else
        {
            return AppImplMixBeadResetVersion1();
        }
}

//磁珠混匀升起来
LH_ERR AppImplMixBeadUp(MIX_MODE mixMode)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_BEAD,APP_PARAM_SUB_INDEX_MIX_BEAD_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixBeadUpVersion0(mixMode);
        }
        else
        {
            return AppImplMixBeadUpVersion1(mixMode);
        }
}

//磁珠混匀升降降下去
LH_ERR AppImplMixBeadDown(void)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_BEAD,APP_PARAM_SUB_INDEX_MIX_BEAD_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixBeadDownVersion0();
        }
        else
        {
            return AppImplMixBeadDownVersion1();
        }
}

//磁珠混匀旋转混匀一次,不带自动升降
LH_ERR AppImplMixBeadStartRotate(MIX_MODE mixMode)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_BEAD,APP_PARAM_SUB_INDEX_MIX_BEAD_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixBeadStartRotateVersion0(mixMode);
        }
        else
        {
            return AppImplMixBeadStartRotateVersion1(mixMode);
        }
}

//磁珠混匀集成混匀一次
LH_ERR AppImplMixBeadRotateIntegrated(MIX_MODE mixMode)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_BEAD,APP_PARAM_SUB_INDEX_MIX_BEAD_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixBeadRotateIntegratedVersion0(mixMode);
        }
        else
        {
            return AppImplMixBeadRotateIntegratedVersion1(mixMode);
        }
}


//指定步进电机复位
LH_ERR AppImplMixBeadStepMotorReset(INDEX_MOTOR_MIX_BEAD index)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_BEAD,APP_PARAM_SUB_INDEX_MIX_BEAD_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixBeadStepMotorResetVersion0(index);
        }
        else
        {
            return AppImplMixBeadStepMotorResetVersion1(index);
        }
}

//指定步进电机走指定步数
LH_ERR AppImplMixBeadStepMotorRunSteps(INDEX_MOTOR_MIX_BEAD index,int32_t steps,int32_t* posAfterRun)
{
    //参数辅助,用于系统参数读取
        int32_t paramUtil;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MIX_BEAD,APP_PARAM_SUB_INDEX_MIX_BEAD_MOTOR_CONFIG_VERSION,&paramUtil);
        if(paramUtil >1)
            paramUtil = 1;
        else if(paramUtil <0)
            paramUtil = 0;
    //根据配置分别调用
        if(paramUtil == 0)
        {
            return AppImplMixBeadStepMotorRunStepsVersion0(index,steps,posAfterRun);
        }
        else
        {
            return AppImplMixBeadStepMotorRunStepsVersion1(index,steps,posAfterRun);
        }
}




