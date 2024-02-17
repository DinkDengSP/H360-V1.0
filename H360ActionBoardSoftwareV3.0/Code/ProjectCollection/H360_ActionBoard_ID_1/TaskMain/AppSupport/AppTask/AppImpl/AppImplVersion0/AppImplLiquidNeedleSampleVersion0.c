/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:31:42
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 15:36:37
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidNeedleSample.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidNeedleSampleVersion0.h"


//样本针泵阀复位
LH_ERR AppImplLiquidNeedleSampleResetVersion0(void)
{
    //关闭内针清洗泵
    AppLiquidUtilClosePumpDP307Version0();
    //关闭外针清洗泵
    AppLiquidUtilClosePumpDP305Version0();
    //关闭强洗清洗泵
    AppLiquidUtilClosePumpDP304Version0();
    //等待操作
    AppLiquidUtilWaitPumpStableVersion0();
    //关闭内针清洗阀
    AppLiquidUtilCloseValveV221Version0();
    //关闭外针清洗阀
    AppLiquidUtilCloseValveV218Version0();
    //关闭强洗阀
    AppLiquidUtilCloseValveV215Version0();
    //关闭清洗维护阀
    AppLiquidUtilCloseValveV309Version0();
    //等待操作
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//样本针清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenCleanVersion0(NEEDLE_CLEAN_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV221Version0();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV218Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗泵
        AppLiquidUtilOpenPumpDP307Version0();
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


//样本针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseCleanVersion0(NEEDLE_CLEAN_OPT opt)
{
    //先关泵后关阀
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        AppLiquidUtilClosePumpDP307Version0();
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
        AppLiquidUtilCloseValveV221Version0();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV218Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//样本针强力清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPowerfulCleanVersion0(void)
{
    //打开强力清洗阀
    AppLiquidUtilOpenValveV215Version0();
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    //打开强力清洗泵
    AppLiquidUtilOpenPumpDP304Version0();
    //等待响应
    AppLiquidUtilWaitPumpStableVersion0();
    return LH_ERR_NONE;
}


//样本针强力清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePowerfulCleanVersion0(void)
{
    //关闭强力清洗泵
    AppLiquidUtilClosePumpDP304Version0();
    //等待响应
    AppLiquidUtilWaitPumpStableVersion0();
    //关闭强力清洗阀
    AppLiquidUtilCloseValveV215Version0();
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//样本针灌注泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPrimeVersion0(NEEDLE_PRIME_OPT opt)
{
    //先开阀后开泵
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV221Version0();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV218Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗泵
        AppLiquidUtilOpenPumpDP307Version0();
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


//样本针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePrimeVersion0(NEEDLE_PRIME_OPT opt)
{
    //先关泵后关阀
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        AppLiquidUtilClosePumpDP307Version0();
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
        AppLiquidUtilCloseValveV221Version0();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV218Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//样本针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenMaintainVersion0(NEEDLE_MAINTAIN_OPT opt)
{
    //打开存水切换到纯水
    AppLiquidUtilValveV309SwitchLiquidPureWaterVersion0();
    //先开阀后开泵
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        AppLiquidUtilOpenValveV221Version0();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        AppLiquidUtilOpenValveV218Version0();
    }
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗泵
        AppLiquidUtilOpenPumpDP307Version0();
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


//样本针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseMaintainVersion0(NEEDLE_MAINTAIN_OPT opt)
{
    //先关泵后关阀
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        AppLiquidUtilClosePumpDP307Version0();
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
        AppLiquidUtilCloseValveV221Version0();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        AppLiquidUtilCloseValveV218Version0();
    }
    //三通关闭
    AppLiquidUtilCloseValveV309Version0();
    //等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}























