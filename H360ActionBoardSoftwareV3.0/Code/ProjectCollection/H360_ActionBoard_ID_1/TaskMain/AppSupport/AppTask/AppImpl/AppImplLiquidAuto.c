/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-16 17:16:19
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 20:24:40
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidAuto.h"

//液路自动化状态机初始化
void AppImplLiquidAutoStateMachineInit(void)
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
        AppImplLiquidAutoStateMachineInitVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        AppImplLiquidAutoStateMachineInitVersion1();
    }
    else
    {
        //未定义的其他版本
        asm("nop");
    }
}

//液路自动化状态机循环
void AppImplLiquidAutoStateMachineLoop(void)
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
        AppImplLiquidAutoStateMachineLoopVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        AppImplLiquidAutoStateMachineLoopVersion1();
    }
    else
    {
        //未定义的其他版本
        asm("nop");
    }
}

//液路自动化单元初始化
LH_ERR AppImplLiquidAutoInit(void)
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
        return AppImplLiquidAutoInitVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidAutoInitVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//强制执行一次抽废液
LH_ERR AppImplLiquidAutoForceTrigWasteLiquid(void)
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
        return AppImplLiquidAutoForceTrigWasteLiquidVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidAutoForceTrigWasteLiquidVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//打开自动抽废液功能
LH_ERR AppImplLiquidAutoEnableWasteLiquidAutomatic(void)
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
        return AppImplLiquidAutoEnableWasteLiquidAutomaticVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidAutoEnableWasteLiquidAutomaticVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//关闭自动抽废液功能
LH_ERR AppImplLiquidAutoDisableWasteLiquidAutomatic(void)
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
        return AppImplLiquidAutoDisableWasteLiquidAutomaticVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidAutoDisableWasteLiquidAutomaticVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//查看自动抽废液状态
LH_ERR AppImplLiquidAutoCheckStateWasteLiquid(LIQUID_AUTOMATIC_STATE* statePtr)
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
        return AppImplLiquidAutoCheckStateWasteLiquidVersion0(statePtr);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidAutoCheckStateWasteLiquidVersion1(statePtr);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//关闭三针清洗液稀释液路
LH_ERR AppImplLiquidAutoCloseLiquidDilutionThreeNeedleClean(void)
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
        return AppImplLiquidAutoCloseLiquidDilutionThreeNeedleCleanVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidAutoCloseLiquidDilutionThreeNeedleCleanVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//打开三针清洗液稀释液路
LH_ERR AppImplLiquidAutoOpenLiquidDilutionThreeNeedleClean(DILUTION_CHANNEL channel)
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
        return AppImplLiquidAutoOpenLiquidDilutionThreeNeedleCleanVersion0(channel);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidAutoOpenLiquidDilutionThreeNeedleCleanVersion1(channel);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}




