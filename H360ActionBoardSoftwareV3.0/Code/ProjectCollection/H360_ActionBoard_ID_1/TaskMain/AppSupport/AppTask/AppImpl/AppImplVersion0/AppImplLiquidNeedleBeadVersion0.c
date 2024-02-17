/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:32:04
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 15:36:44
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidNeedleBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidNeedleBeadVersion0.h"

//磁珠针泵阀复位
LH_ERR AppImplLiquidNeedleBeadResetVersion0(void)
{
    //关闭内针清洗泵
    AppLiquidUtilClosePumpDP306Version0();
    //关闭外针清洗泵
    AppLiquidUtilClosePumpDP305Version0();
    //等待操作
    AppLiquidUtilWaitPumpStableVersion0();
    //关闭内针清洗阀
    AppLiquidUtilCloseValveV220Version0();
    //关闭外针清洗阀
    AppLiquidUtilCloseValveV217Version0();
    //关闭清洗维护阀
    AppLiquidUtilCloseValveV309Version0();
    //等待操作
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//磁珠针清洗泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenCleanVersion0(NEEDLE_CLEAN_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV220Version0();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV217Version0();
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


//磁珠针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseCleanVersion0(NEEDLE_CLEAN_OPT opt)
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
        AppLiquidUtilCloseValveV220Version0();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV217Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//磁珠针灌注泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenPrimeVersion0(NEEDLE_PRIME_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV220Version0();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV217Version0();
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


//磁珠针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleBeadClosePrimeVersion0(NEEDLE_PRIME_OPT opt)
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
        AppLiquidUtilCloseValveV220Version0();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV217Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//磁珠针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenMaintainVersion0(NEEDLE_MAINTAIN_OPT opt)
{
    //打开纯水切换到纯水
    AppLiquidUtilValveV309SwitchLiquidPureWaterVersion0();
    //先开阀后开泵
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV220Version0();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV217Version0();
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


//磁珠针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseMaintainVersion0(NEEDLE_MAINTAIN_OPT opt)
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
        AppLiquidUtilCloseValveV220Version0();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV217Version0();
    }
    //三通关闭
    AppLiquidUtilCloseValveV309Version0();
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}
























