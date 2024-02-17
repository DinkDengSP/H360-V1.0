/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:31:42
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 19:24:24
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidNeedleSample.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidNeedleSampleVersion1.h"
#include "AppLiquidUtilVersion1.h"

//样本针泵阀复位
LH_ERR AppImplLiquidNeedleSampleResetVersion1(void)
{
    //关闭内针清洗泵
    AppLiquidUtilClosePumpDP305Version1();
    //关闭外针清洗泵
    AppLiquidUtilClosePumpDP306Version1();
    //等待操作
    AppLiquidUtilWaitValveStableVersion1();
    //关闭内针三通阀
    AppLiquidUtilCloseValveV309Version1();
    //关闭外针三通阀
    AppLiquidUtilCloseValveV232Version1();
    //关闭内针清洗阀
    AppLiquidUtilCloseValveV218Version1();
    //关闭外针清洗阀
    AppLiquidUtilCloseValveV221Version1();
    //等待操作
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//样本针清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenCleanVersion1(NEEDLE_CLEAN_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV221Version1();
        //V309切换清洗液
        AppLiquidUtilValveV309SwitchLiquidCleanVersion1();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV218Version1();
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


//样本针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseCleanVersion1(NEEDLE_CLEAN_OPT opt)
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
        AppLiquidUtilCloseValveV221Version1();
        //关闭内针三通
        AppLiquidUtilCloseValveV309Version1();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV218Version1();
        //关闭外针三通
        AppLiquidUtilCloseValveV232Version1();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//样本针强力清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPowerfulCleanVersion1(void)
{
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    return LH_ERR_NONE;
}


//样本针强力清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePowerfulCleanVersion1(void)
{
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//样本针灌注泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPrimeVersion1(NEEDLE_PRIME_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV221Version1();
        //V309切换清洗液
        AppLiquidUtilValveV309SwitchLiquidCleanVersion1();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV218Version1();
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


//样本针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePrimeVersion1(NEEDLE_PRIME_OPT opt)
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
        AppLiquidUtilCloseValveV221Version1();
        //关闭内针三通
        AppLiquidUtilCloseValveV309Version1();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV218Version1();
        //关闭外针三通
        AppLiquidUtilCloseValveV232Version1();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//样本针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenMaintainVersion1(NEEDLE_MAINTAIN_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV221Version1();
        //V309切换纯水
        AppLiquidUtilValveV309SwitchLiquidPureWaterVersion1();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV218Version1();
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


//样本针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseMaintainVersion1(NEEDLE_MAINTAIN_OPT opt)
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
        AppLiquidUtilCloseValveV221Version1();
        //关闭内针三通
        AppLiquidUtilCloseValveV309Version1();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV218Version1();
        //关闭外针三通
        AppLiquidUtilCloseValveV232Version1();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}























