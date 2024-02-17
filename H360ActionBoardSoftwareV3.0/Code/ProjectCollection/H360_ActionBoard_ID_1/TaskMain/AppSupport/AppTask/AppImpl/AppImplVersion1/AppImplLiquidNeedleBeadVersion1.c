/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:32:04
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 19:24:17
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidNeedleBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidNeedleBeadVersion1.h"
#include "AppLiquidUtilVersion1.h"

//磁珠针泵阀复位
LH_ERR AppImplLiquidNeedleBeadResetVersion1(void)
{
    //关闭内针清洗泵
    AppLiquidUtilClosePumpDP305Version1();
    //关闭外针清洗泵
    AppLiquidUtilClosePumpDP306Version1();
    //等待操作
    AppLiquidUtilWaitPumpStableVersion1();
    //关闭内针三通阀
    AppLiquidUtilCloseValveV309Version1();
    //关闭外针三通阀
    AppLiquidUtilCloseValveV232Version1();
    //关闭内针清洗阀
    AppLiquidUtilCloseValveV220Version1();
    //关闭外针清洗阀
    AppLiquidUtilCloseValveV217Version1();
    //等待操作
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//磁珠针清洗泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenCleanVersion1(NEEDLE_CLEAN_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV220Version1();
        //V309切换清洗液
        AppLiquidUtilValveV309SwitchLiquidCleanVersion1();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV217Version1();
        //V232切换纯水
        AppLiquidUtilValveV232SwitchPureWaterVersion1();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗泵
        AppLiquidUtilOpenPumpDP306Version1();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //打开外针清洗泵
        AppLiquidUtilOpenPumpDP305Version1();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    return LH_ERR_NONE;
}


//磁珠针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseCleanVersion1(NEEDLE_CLEAN_OPT opt)
{
    //先关泵后关阀
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        AppLiquidUtilClosePumpDP306Version1();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //关闭外针清洗泵
        AppLiquidUtilClosePumpDP305Version1();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //关闭内针清洗阀
        AppLiquidUtilCloseValveV220Version1();
        //关闭内针三通
        AppLiquidUtilCloseValveV309Version1();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV217Version1();
        //关闭外针三通
        AppLiquidUtilCloseValveV232Version1();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//磁珠针灌注泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenPrimeVersion1(NEEDLE_PRIME_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV220Version1();
        //V309切换清洗液
        AppLiquidUtilValveV309SwitchLiquidCleanVersion1();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV217Version1();
        //V232切换纯水
        AppLiquidUtilValveV232SwitchPureWaterVersion1();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗泵
        AppLiquidUtilOpenPumpDP306Version1();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //打开外针清洗泵
        AppLiquidUtilOpenPumpDP305Version1();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    return LH_ERR_NONE;
}


//磁珠针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleBeadClosePrimeVersion1(NEEDLE_PRIME_OPT opt)
{
    //先关泵后关阀
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        AppLiquidUtilClosePumpDP306Version1();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //关闭外针清洗泵
        AppLiquidUtilClosePumpDP305Version1();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //关闭内针清洗阀
        AppLiquidUtilCloseValveV220Version1();
        //关闭内针三通
        AppLiquidUtilCloseValveV309Version1();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV217Version1();
        //关闭外针三通
        AppLiquidUtilCloseValveV232Version1();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//磁珠针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenMaintainVersion1(NEEDLE_MAINTAIN_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV220Version1();
        //V309切换纯水
        AppLiquidUtilValveV309SwitchLiquidPureWaterVersion1();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV217Version1();
        //V232切换纯水
        AppLiquidUtilValveV232SwitchPureWaterVersion1();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗泵
        AppLiquidUtilOpenPumpDP305Version1();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //打开外针清洗泵
        AppLiquidUtilOpenPumpDP305Version1();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    return LH_ERR_NONE;
}


//磁珠针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseMaintainVersion1(NEEDLE_MAINTAIN_OPT opt)
{
    //先关泵后关阀
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        AppLiquidUtilClosePumpDP305Version1();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //关闭外针清洗泵
        AppLiquidUtilClosePumpDP305Version1();
    }
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //关闭内针清洗阀
        AppLiquidUtilCloseValveV220Version1();
        //关闭内针三通
        AppLiquidUtilCloseValveV309Version1();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV217Version1();
        //关闭外针三通
        AppLiquidUtilCloseValveV232Version1();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}
























