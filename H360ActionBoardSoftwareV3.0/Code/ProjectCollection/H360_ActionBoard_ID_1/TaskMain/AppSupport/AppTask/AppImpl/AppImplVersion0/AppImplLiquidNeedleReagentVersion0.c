/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:31:51
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-14 21:21:04
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidNeedleReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidNeedleReagentVersion0.h"


//试剂针泵阀复位
LH_ERR AppImplLiquidNeedleReagentResetVersion0(void)
{
    //关闭内针清洗泵
    AppLiquidUtilClosePumpDP306Version0();
    //关闭外针清洗泵
    AppLiquidUtilClosePumpDP305Version0();
    //等待操作
    AppLiquidUtilWaitPumpStableVersion0();
    //关闭内针清洗阀
    AppLiquidUtilCloseValveV219Version0();
    //关闭外针清洗阀
    AppLiquidUtilCloseValveV216Version0();
    //关闭清洗维护阀
    AppLiquidUtilCloseValveV309Version0();
    //等待操作
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//试剂针清洗泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenCleanVersion0(NEEDLE_CLEAN_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV219Version0();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV216Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗泵
        AppLiquidUtilOpenPumpDP306Version0();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //打开外针清洗泵
        AppLiquidUtilOpenPumpDP305Version0();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion0();
    return LH_ERR_NONE;
}


//试剂针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleReagentCloseCleanVersion0(NEEDLE_CLEAN_OPT opt)
{
    //先关泵后关阀
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        AppLiquidUtilClosePumpDP306Version0();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //关闭外针清洗泵
        AppLiquidUtilClosePumpDP305Version0();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion0();
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //关闭内针清洗阀
        AppLiquidUtilCloseValveV219Version0();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV216Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//试剂针灌注泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenPrimeVersion0(NEEDLE_PRIME_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV219Version0();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV216Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗泵
        AppLiquidUtilOpenPumpDP306Version0();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //打开外针清洗泵
        AppLiquidUtilOpenPumpDP305Version0();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion0();
    return LH_ERR_NONE;
}


//试剂针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleReagentClosePrimeVersion0(NEEDLE_PRIME_OPT opt)
{
    //先关泵后关阀
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        AppLiquidUtilClosePumpDP306Version0();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //关闭外针清洗泵
        AppLiquidUtilClosePumpDP305Version0();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion0();
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //关闭内针清洗阀
        AppLiquidUtilCloseValveV219Version0();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV216Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//试剂针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenMaintainVersion0(NEEDLE_MAINTAIN_OPT opt)
{
    //打开纯水切换到纯水
    AppLiquidUtilValveV309SwitchLiquidPureWaterVersion0();
    //先开阀后开泵
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV219Version0();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV216Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗泵
        AppLiquidUtilOpenPumpDP306Version0();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //打开外针清洗泵
        AppLiquidUtilOpenPumpDP305Version0();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion0();
    return LH_ERR_NONE;
}


//试剂针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleReagentCloseMaintainVersion0(NEEDLE_MAINTAIN_OPT opt)
{
    //先关泵后关阀
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        AppLiquidUtilClosePumpDP306Version0();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //关闭外针清洗泵
        AppLiquidUtilClosePumpDP305Version0();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion0();
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //关闭内针清洗阀
        AppLiquidUtilCloseValveV219Version0();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV216Version0();
    }
    //三通关闭
    AppLiquidUtilCloseValveV309Version0();
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}























