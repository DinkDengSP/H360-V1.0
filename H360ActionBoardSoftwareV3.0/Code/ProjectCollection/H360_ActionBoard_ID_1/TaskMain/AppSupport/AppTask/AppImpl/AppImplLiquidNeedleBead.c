/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-16 17:16:57
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 20:25:36
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidNeedleBead.h"

//磁珠针泵阀复位
LH_ERR AppImplLiquidNeedleBeadReset(void)
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
        return AppImplLiquidNeedleBeadResetVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleBeadResetVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//磁珠针清洗泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenClean(NEEDLE_CLEAN_OPT opt)
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
        return AppImplLiquidNeedleBeadOpenCleanVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleBeadOpenCleanVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//磁珠针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseClean(NEEDLE_CLEAN_OPT opt)
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
        return AppImplLiquidNeedleBeadCloseCleanVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleBeadCloseCleanVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//磁珠针灌注泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenPrime(NEEDLE_PRIME_OPT opt)
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
        return AppImplLiquidNeedleBeadOpenPrimeVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleBeadOpenPrimeVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//磁珠针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleBeadClosePrime(NEEDLE_PRIME_OPT opt)
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
        return AppImplLiquidNeedleBeadClosePrimeVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleBeadClosePrimeVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//磁珠针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenMaintain(NEEDLE_MAINTAIN_OPT opt)
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
        return AppImplLiquidNeedleBeadOpenMaintainVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleBeadOpenMaintainVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//磁珠针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseMaintain(NEEDLE_MAINTAIN_OPT opt)
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
        return AppImplLiquidNeedleBeadCloseMaintainVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleBeadCloseMaintainVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}





