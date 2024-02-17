/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-16 17:17:20
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 20:26:54
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidNeedleSample.h"

//样本针泵阀复位
LH_ERR AppImplLiquidNeedleSampleReset(void)
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
        return AppImplLiquidNeedleSampleResetVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleSampleResetVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//样本针清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenClean(NEEDLE_CLEAN_OPT opt)
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
        return AppImplLiquidNeedleSampleOpenCleanVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleSampleOpenCleanVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//样本针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseClean(NEEDLE_CLEAN_OPT opt)
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
        return AppImplLiquidNeedleSampleCloseCleanVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleSampleCloseCleanVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//样本针强力清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPowerfulClean(void)
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
        return AppImplLiquidNeedleSampleOpenPowerfulCleanVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleSampleOpenPowerfulCleanVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//样本针强力清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePowerfulClean(void)
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
        return AppImplLiquidNeedleSampleClosePowerfulCleanVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleSampleClosePowerfulCleanVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//样本针灌注泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPrime(NEEDLE_PRIME_OPT opt)
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
        return AppImplLiquidNeedleSampleOpenPrimeVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleSampleOpenPrimeVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//样本针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePrime(NEEDLE_PRIME_OPT opt)
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
        return AppImplLiquidNeedleSampleClosePrimeVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleSampleClosePrimeVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//样本针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenMaintain(NEEDLE_MAINTAIN_OPT opt)
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
        return AppImplLiquidNeedleSampleOpenMaintainVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleSampleOpenMaintainVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//样本针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseMaintain(NEEDLE_MAINTAIN_OPT opt)
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
        return AppImplLiquidNeedleSampleCloseMaintainVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleSampleCloseMaintainVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}




