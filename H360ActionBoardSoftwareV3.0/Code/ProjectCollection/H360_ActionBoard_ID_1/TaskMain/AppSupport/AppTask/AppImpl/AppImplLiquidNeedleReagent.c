/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-16 17:17:13
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 20:26:03
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidNeedleReagent.h"

//试剂针泵阀复位
LH_ERR AppImplLiquidNeedleReagentReset(void)
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
        return AppImplLiquidNeedleReagentResetVersion0();
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleReagentResetVersion1();
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//试剂针清洗泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenClean(NEEDLE_CLEAN_OPT opt)
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
        return AppImplLiquidNeedleReagentOpenCleanVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleReagentOpenCleanVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//试剂针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleReagentCloseClean(NEEDLE_CLEAN_OPT opt)
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
        return AppImplLiquidNeedleReagentCloseCleanVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleReagentCloseCleanVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//试剂针灌注泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenPrime(NEEDLE_PRIME_OPT opt)
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
        return AppImplLiquidNeedleReagentOpenPrimeVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleReagentOpenPrimeVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//试剂针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleReagentClosePrime(NEEDLE_PRIME_OPT opt)
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
        return AppImplLiquidNeedleReagentClosePrimeVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleReagentClosePrimeVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//试剂针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenMaintain(NEEDLE_MAINTAIN_OPT opt)
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
        return AppImplLiquidNeedleReagentOpenMaintainVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleReagentOpenMaintainVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}

//试剂针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleReagentCloseMaintain(NEEDLE_MAINTAIN_OPT opt)
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
        return AppImplLiquidNeedleReagentCloseMaintainVersion0(opt);
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        return AppImplLiquidNeedleReagentCloseMaintainVersion1(opt);
    }
    else
    {
        //未定义的其他版本
        return LH_ERR_NONE;
    }
}



