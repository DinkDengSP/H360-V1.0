/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:32:58
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-14 19:32:59
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidExcite.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidExciteVersion0.h"


//激发液泵阀复位
LH_ERR AppImplLiquidExciteResetVersion0(void)
{
    //关闭V308
    AppLiquidUtilCloseValveV308Version0();
    //关闭V203
    AppLiquidUtilCloseValveV203Version0();
    //关闭V204
    AppLiquidUtilCloseValveV204Version0();
    //关闭DP2
    AppLiquidUtilClosePumpDP2Version0();
    return LH_ERR_NONE;
}


//激发液选通指定瓶
LH_ERR AppImplLiquidExciteOpenSelectInjectVersion0(BOTTLE_LIQUID_EXCITE bottle)
{
    //根据选瓶决定
    if(bottle == BOTTLE_LIQUID_EXCITE_A)
    {
        //选A瓶
        AppLiquidUtilValveV204SelectBottleAVersion0();
    }
    else
    {
        //选B瓶
        AppLiquidUtilValveV204SelectBottleBVersion0();
    }
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion0();
    //打开注液液路
    AppLiquidUtilOpenValveV203Version0();
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//激发液选通清洗维护
LH_ERR AppImplLiquidExciteOpenSelectMaintainVersion0(BOTTLE_LIQUID_EXCITE bottle)
{
    //选通纯水瓶
    AppLiquidUtilOpenValveV308Version0();
    //根据选瓶决定
    if(bottle == BOTTLE_LIQUID_EXCITE_A)
    {
        //选A瓶
        AppLiquidUtilValveV204SelectBottleAVersion0();
    }
    else
    {
        //选B瓶
        AppLiquidUtilValveV204SelectBottleBVersion0();
    }
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion0();
    //打开注液液路
    AppLiquidUtilOpenValveV203Version0();
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//激发液泵阀关闭
LH_ERR AppImplLiquidExciteCloseVersion0(void)
{
    //关闭纯水选通阀
    AppLiquidUtilCloseValveV308Version0();
    //关闭V204
    AppLiquidUtilCloseValveV204Version0();
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion0();
    //关闭V203
    AppLiquidUtilCloseValveV203Version0();
    //延时等待响应
    AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}


//激发液开关废液排放
LH_ERR AppImplLiquidExciteSetWasteLiquidVersion0(LIQUID_EXCITE_WASTE_STATE state)
{
    //根据设定打开关闭废液泵
    if(state == LIQUID_EXCITE_WASTE_STATE_ON)
    {
        //打开DP2
        AppLiquidUtilOpenPumpDP2Version0();
    }
    else
    {
        //关闭DP2
        AppLiquidUtilClosePumpDP2Version0();
    }
    return LH_ERR_NONE;
}






















