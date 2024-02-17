/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:31:32
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-17 20:38:10
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidAuto.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplLiquidAutoVersion1.h"
#include "AppLiquidUtilVersion1.h"

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplLiquidAutoConvertErrorCode(FLAG_LIQUID_AUTO_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_LIQUID_AUTO;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD1_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}

//系统抽废液状态
static LIQUID_AUTOMATIC_STATE liquidWasteState = LIQUID_AUTOMATIC_STATE_OFF;
//抽废液计时辅助
static int32_t time100MsLiquidWasteCount = 0;
//当前抽废液的状态
static STAGE_LIQUID_WASTE currenrStageLiquidWaste = STAGE_LIQUID_WASTE_CLOSE;

//抽废液状态机初始化
static void AppImplLiquidAutoStateMachineLiquidWasteInit(void)
{
    //初始化默认抽废液功能关闭
    liquidWasteState = LIQUID_AUTOMATIC_STATE_OFF;
    //抽废液关闭状态
    currenrStageLiquidWaste = STAGE_LIQUID_WASTE_CLOSE;
    //计时状态清零
    time100MsLiquidWasteCount = 0;
    //关闭废液泵,等待响应
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1));
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    //关闭废液阀
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1));
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
}

//抽废液正常打开状态
static void AppImplLiquidAutoStateMachineLiquidWasteLoopStageOpenNormal(void)
{
    BitAction liquidWasteSensorState;
    //读取浮球状态
    CanSubInputReadState(INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_1,&liquidWasteSensorState);
    //上浮球触发,直接切入紧急状态
        if(liquidWasteSensorState == VALID_LEVEL_INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_1)
        {
            //切入紧急状态
            currenrStageLiquidWaste = STAGE_LIQUID_WASTE_OPEN_WARNING;
            //计时状态清零
            time100MsLiquidWasteCount = 0;
            //打开废液阀
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1);
            //等待响应
            AppLiquidUtilWaitValveStableVersion1();
            //打开废液泵,等待响应
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1);
            //退出
            return;
        }
    //超过了打开时间
        if(time100MsLiquidWasteCount >= TIME_100MS_LIQUID_WASTE_OPEN_NORMAL)
        {
            //切入停止状态
            currenrStageLiquidWaste = STAGE_LIQUID_WASTE_CLOSE;
            //计时状态清零
            time100MsLiquidWasteCount = 0;
            //关闭废液泵,等待响应
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1));
            //等待响应
            AppLiquidUtilWaitPumpStableVersion1();
            //关闭废液阀
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1));
            return;
        }
}

//抽废液紧急打开状态
static void AppImplLiquidAutoStateMachineLiquidWasteLoopStageOpenWarning(void)
{
    BitAction liquidWasteSensorState;
    //读取浮球状态
        CanSubInputReadState(INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_1,&liquidWasteSensorState);
    //上浮球不触发,切入抽废液正常状态
        if(liquidWasteSensorState != VALID_LEVEL_INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_1)
        {
            //打开抽废液,正常状态
            currenrStageLiquidWaste = STAGE_LIQUID_WASTE_OPEN_NORMAL;
            //计时状态清零
            time100MsLiquidWasteCount = 0;
            //打开废液泵,等待响应
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1);
            //等待响应
            AppLiquidUtilWaitPumpStableVersion1();
            //打开废液阀
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1);
            return;
        }
}

//抽废液关闭状态
static void AppImplLiquidAutoStateMachineLiquidWasteLoopStageClose(void)
{
    BitAction liquidWasteSensorState;
    //读取浮球状态
        CanSubInputReadState(INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_1,&liquidWasteSensorState);
    //如果上浮球触发,立即切入抽废液紧急状态
        if(liquidWasteSensorState == VALID_LEVEL_INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_1)
        {
            //切入紧急状态
            currenrStageLiquidWaste = STAGE_LIQUID_WASTE_OPEN_WARNING;
            //计时状态清零
            time100MsLiquidWasteCount = 0;
            //打开废液阀
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1);
            //等待响应
            AppLiquidUtilWaitValveStableVersion1();
            //打开废液泵,等待响应
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1);
            //退出
            return;
        }
    //上浮球没触发,则等待关闭时间到位后,再次切入正常抽废液状态
        if(time100MsLiquidWasteCount >= TIME_100MS_LIQUID_WASTE_CLOSE)
        {
            //打开抽废液,正常状态
            currenrStageLiquidWaste = STAGE_LIQUID_WASTE_OPEN_NORMAL;
            //计时状态清零
            time100MsLiquidWasteCount = 0;
            //打开废液阀
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1);
            //等待响应
            AppLiquidUtilWaitValveStableVersion1();
            //打开废液泵,等待响应
            CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1);
            return;
        }
}

//抽废液状态机循环
static void AppImplLiquidAutoStateMachineLiquidWasteLoop(void)
{
    //打开废液液路
    if(liquidWasteState == LIQUID_AUTOMATIC_STATE_ON)
    {
        time100MsLiquidWasteCount++;
        switch(currenrStageLiquidWaste)
        {
            case STAGE_LIQUID_WASTE_OPEN_NORMAL://正常打开状态
                AppImplLiquidAutoStateMachineLiquidWasteLoopStageOpenNormal();
                break;
            case STAGE_LIQUID_WASTE_OPEN_WARNING://紧急打开状态
                AppImplLiquidAutoStateMachineLiquidWasteLoopStageOpenWarning();
                break;
            case STAGE_LIQUID_WASTE_CLOSE://关闭状态
                AppImplLiquidAutoStateMachineLiquidWasteLoopStageClose();
                break;
            default:
                currenrStageLiquidWaste = STAGE_LIQUID_WASTE_OPEN_NORMAL;
                break;
        }
    }
    else
    {
        time100MsLiquidWasteCount = 0;
    }
}

//液路自动化状态机初始化
void AppImplLiquidAutoStateMachineInitVersion1(void)
{
    //抽废液状态机初始化
    AppImplLiquidAutoStateMachineLiquidWasteInit();
}

//液路自动化状态机循环
void AppImplLiquidAutoStateMachineLoopVersion1(void)
{
    //抽废液状态机循环
    AppImplLiquidAutoStateMachineLiquidWasteLoop();
}

//液路自动化单元初始化
LH_ERR AppImplLiquidAutoInitVersion1(void)
{
    return LH_ERR_NONE;
}


//强制执行一次抽废液
LH_ERR AppImplLiquidAutoForceTrigWasteLiquidVersion1(void)
{
    //打开废液泵废液阀
    //打开废液阀
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1);
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    //打开废液泵,等待响应
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1);
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    //延时等待抽废液,50S
    CoreDelayMs(50000);
    //关闭废液泵,等待响应
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1));
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    //关闭废液阀
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1));
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    //废液抽完,计时时间清零
    time100MsLiquidWasteCount = 0;
    //进入抽废液关闭状态
    currenrStageLiquidWaste = STAGE_LIQUID_WASTE_CLOSE;
    return LH_ERR_NONE;
}


//打开自动抽废液功能
LH_ERR AppImplLiquidAutoEnableWasteLiquidAutomaticVersion1(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //打开抽废液
    liquidWasteState = LIQUID_AUTOMATIC_STATE_ON;
    //打开抽废液
    currenrStageLiquidWaste = STAGE_LIQUID_WASTE_OPEN_NORMAL;
    //计时状态清零
    time100MsLiquidWasteCount = 0;
    //打开废液阀
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1);
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    //打开废液泵,等待响应
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1);
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    if(errorCode != LH_ERR_NONE)
    {
        return AppImplLiquidAutoConvertErrorCode(FLAG_LIQUID_AUTO_LIQUID_WASTE_TIME_OUT_FULL,errorCode);
    }
    return LH_ERR_NONE;
}


//关闭自动抽废液功能
LH_ERR AppImplLiquidAutoDisableWasteLiquidAutomaticVersion1(void)
{
    //关闭抽废液
    liquidWasteState = LIQUID_AUTOMATIC_STATE_OFF;
    //状态设置为抽废液关闭
    currenrStageLiquidWaste = STAGE_LIQUID_WASTE_CLOSE;
    //计时状态清零
    time100MsLiquidWasteCount = 0;
    //关闭废液泵,等待响应
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP1_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1));
    //等待响应
    AppLiquidUtilWaitPumpStableVersion1();
    //关闭废液阀
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V307_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1));
    //等待响应
    AppLiquidUtilWaitValveStableVersion1();
    return LH_ERR_NONE;
}


//查看自动抽废液状态
LH_ERR AppImplLiquidAutoCheckStateWasteLiquidVersion1(LIQUID_AUTOMATIC_STATE* statePtr)
{
    *statePtr = liquidWasteState;
    return LH_ERR_NONE;
}

//关闭三针清洗液稀释液路
LH_ERR AppImplLiquidAutoCloseLiquidDilutionThreeNeedleCleanVersion1(void)
{
    //关闭DP101
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP101_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP101_VERSION_1));
    //关闭DP304
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP304_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_1));
    //关闭DP103
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP103_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP103_VERSION_1));
    //等待泵响应
    AppLiquidUtilWaitPumpStableVersion1();
    //关闭V235
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V235_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V235_VERSION_1));
    //关闭V240
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V240_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V240_VERSION_1));
    //V237关闭
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V237_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V237_VERSION_1));
    return LH_ERR_NONE;
}


//打开三针清洗液稀释液路
LH_ERR AppImplLiquidAutoOpenLiquidDilutionThreeNeedleCleanVersion1(DILUTION_CHANNEL channel)
{
    if(channel == DILUTION_CHANNEL_TRANSFER_BOTTLE_INJECT_PURE_WATER)
    {
        //中转瓶注纯水
        //关闭DP103
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP103_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP103_VERSION_1));
        //关闭DP304
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP304_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_1));
        //等待泵响应
        AppLiquidUtilWaitPumpStableVersion1();
        //关闭V235
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V235_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V235_VERSION_1));
        //关闭V240
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V240_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V240_VERSION_1));
        //V235切换到纯水
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V235_VERSION_1,(BitAction)(PURE_WATER_LEVEL_OUTPUT_LIQUID_VALVE_V235_VERSION_1));
        //V237纯水阀导通
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V237_VERSION_1,(BitAction)(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V237_VERSION_1));
        //等待阀响应
        AppLiquidUtilWaitValveStableVersion1();
        //打开DP103
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP103_VERSION_1,(BitAction)(VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP103_VERSION_1));
    }
    else if(channel == DILUTION_CHANNEL_TRANSFER_BOTTLE_INJECT_CONCENTRATE_BOTTLE_A)
    {
        //中转瓶注浓缩液1
        //关闭DP103
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP103_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP103_VERSION_1));
        //关闭DP304
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP304_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_1));
        //等待泵响应
        AppLiquidUtilWaitPumpStableVersion1();
        //V235切换到浓缩液
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V235_VERSION_1,(BitAction)(CONCENTRATE_LEVEL_OUTPUT_LIQUID_VALVE_V235_VERSION_1));
        //V240切换到瓶1
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V240_VERSION_1,(BitAction)(BOTTLE_1_LEVEL_OUTPUT_LIQUID_VALVE_V240_VERSION_1));
        //V237关闭
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V237_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V237_VERSION_1));
        //等待阀响应
        AppLiquidUtilWaitValveStableVersion1();
        //打开DP304
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP304_VERSION_1,(BitAction)(VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_1));
    }
    else if(channel == DILUTION_CHANNEL_TRANSFER_BOTTLE_INJECT_CONCENTRATE_BOTTLE_B)
    {
        //中转瓶注浓缩液2
        //关闭DP103
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP103_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP103_VERSION_1));
        //关闭DP304
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP304_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_1));
        //等待泵响应
        AppLiquidUtilWaitPumpStableVersion1();
        //V235切换到浓缩液
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V235_VERSION_1,(BitAction)(CONCENTRATE_LEVEL_OUTPUT_LIQUID_VALVE_V235_VERSION_1));
        //V240切换到瓶2
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V240_VERSION_1,(BitAction)(BOTTLE_2_LEVEL_OUTPUT_LIQUID_VALVE_V240_VERSION_1));
        //V237关闭
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V237_VERSION_1,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V237_VERSION_1));
        //等待阀响应
        AppLiquidUtilWaitValveStableVersion1();
        //打开DP304
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP304_VERSION_1,(BitAction)(VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_1));
    }
    else if(channel == DILUTION_CHANNEL_TRANSFER_BOTTLE_TO_TARGET_BOTTLE)
    {
        //从中转瓶往目标瓶转移
        //打开DP101
        CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP101_VERSION_1,(BitAction)(VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP101_VERSION_1));
        //等待泵响应
        AppLiquidUtilWaitPumpStableVersion1();
    }
    return LH_ERR_NONE;
}












