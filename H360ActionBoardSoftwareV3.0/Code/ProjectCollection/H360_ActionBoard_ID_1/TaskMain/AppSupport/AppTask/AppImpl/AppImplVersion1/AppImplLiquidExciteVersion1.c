/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:32:58
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-01 13:37:10
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidExcite.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidExciteVersion1.h"
#include "AppLiquidUtilVersion1.h"

//激发液泵阀复位
LH_ERR AppImplLiquidExciteResetVersion1(void)
{
    //关闭V308
    AppLiquidUtilCloseValveV308Version1();
    //关闭V203
    AppLiquidUtilCloseValveV203Version1();
    //关闭V204
    AppLiquidUtilCloseValveV204Version1();
    //关闭DP2
    AppLiquidUtilClosePumpDP2Version1();
    return LH_ERR_NONE;
}


//激发液选通指定瓶
LH_ERR AppImplLiquidExciteOpenSelectInjectVersion1(BOTTLE_LIQUID_EXCITE bottle)
{
    //根据选瓶决定
    if(bottle == BOTTLE_LIQUID_EXCITE_A)
    {
        //选A瓶
        AppLiquidUtilValveV203SelectBottle1Version1();
    }
    else
    {
        //选B瓶
        AppLiquidUtilValveV203SelectBottle2Version1();
    }
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion1();
    //打开注液液路
    AppLiquidUtilOpenValveV204Version1();
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//激发液选通清洗维护
LH_ERR AppImplLiquidExciteOpenSelectMaintainVersion1(BOTTLE_LIQUID_EXCITE bottle)
{
    //选通纯水瓶
    AppLiquidUtilOpenValveV308Version1();
    //根据选瓶决定
    if(bottle == BOTTLE_LIQUID_EXCITE_A)
    {
        //选A瓶
        AppLiquidUtilValveV203SelectBottle1Version1();
    }
    else
    {
        //选B瓶
        AppLiquidUtilValveV203SelectBottle2Version1();
    }
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion1();
    //打开注液液路
    AppLiquidUtilOpenValveV204Version1();
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//激发液泵阀关闭
LH_ERR AppImplLiquidExciteCloseVersion1(void)
{
    //关闭纯水选通阀
    AppLiquidUtilCloseValveV308Version1();
    //关闭V203
    AppLiquidUtilCloseValveV203Version1();
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion1();
    //关闭V204
    AppLiquidUtilCloseValveV204Version1();
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//激发液开关废液排放
LH_ERR AppImplLiquidExciteSetWasteLiquidVersion1(LIQUID_EXCITE_WASTE_STATE state)
{
    //根据设定打开关闭废液泵
    if(state == LIQUID_EXCITE_WASTE_STATE_ON)
    {
        //打开DP2
        AppLiquidUtilOpenPumpDP2Version1();
    }
    else
    {
        //关闭DP2
        AppLiquidUtilClosePumpDP2Version1();
    }
    return LH_ERR_NONE;
}






















