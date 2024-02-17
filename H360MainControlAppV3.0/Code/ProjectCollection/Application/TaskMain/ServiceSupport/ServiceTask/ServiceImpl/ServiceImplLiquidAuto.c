/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:54:14
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-17 15:25:17
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplLiquidAuto.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplLiquidAuto.h"
#include "ServiceStatePrivate.h"
#include "TestFlow_ActionAPI.h"

//上次上传浓缩瓶状态的时间
static OS_TICK lastReportLiquidAutoTickTime = 0;
#define REPORT_LIQUID_STATE_TIME_INTERVAL       10000
//LED状态初始化
static void ServiceImplLiquidAuto_LedStateCheck(SENSOR_STATE* bottleEmptyStateA,SENSOR_STATE* bottleEmptyStateB,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    OS_ERR err;
    OS_TICK currentReportLiquidAutoTickTime = OSTimeGet(&err);
    //读取A B瓶状态
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LEVEL_EMPTY,bottleEmptyStateA,commErrorCode,commErrorLevel);
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LEVEL_EMPTY,bottleEmptyStateB,commErrorCode,commErrorLevel);
    //根据状态设置LED灯
    //A瓶
        if(*bottleEmptyStateA == SENSOR_STATE_VALID)
        {
            //A瓶指示灯黄灯点亮
            ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LED_EMPTY,OUT_STATE_ENABLE,commErrorCode,commErrorLevel);
        }
        else
        {
            //A瓶指示灯熄灭
            ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LED_EMPTY,OUT_STATE_DISABLE,commErrorCode,commErrorLevel);
        }
    //B瓶
        if(*bottleEmptyStateB == SENSOR_STATE_VALID)
        {
            //B瓶指示灯黄灯点亮
            ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LED_EMPTY,OUT_STATE_ENABLE,commErrorCode,commErrorLevel);
        }
        else
        {
            //B瓶指示灯熄灭
            ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LED_EMPTY,OUT_STATE_DISABLE,commErrorCode,commErrorLevel);
        }
    //非老化模式,才会有状态上报
        if((MODE_RELEASE == ServiceStateReadAssistRunningMode())&&(currentReportLiquidAutoTickTime - lastReportLiquidAutoTickTime >= REPORT_LIQUID_STATE_TIME_INTERVAL))
        {
            //更新计时讯息
            lastReportLiquidAutoTickTime = currentReportLiquidAutoTickTime;
            if((*bottleEmptyStateA == SENSOR_STATE_VALID)&&(*bottleEmptyStateB == SENSOR_STATE_VALID))
            {
                //两瓶浓缩液均不足
                Service_Api_RespondWarning(CAN1_REPORT_ERR_LEVEL_WARNING, CAN1_REPORT_ERR_LIQUID_AUTO_CONCENTRATE_BOTTLE_ALL_EMPTY, LH_ERR_BOARD_MAIN_LIQUID_AUTO_CONCENTRATE_BOTTLE_ALL_EMPTY);
            }
            else if(*bottleEmptyStateA == SENSOR_STATE_VALID)
            {
                //浓缩液A瓶不足
                Service_Api_RespondWarning(CAN1_REPORT_ERR_LEVEL_WARNING, CAN1_REPORT_ERR_LIQUID_AUTO_CONCENTRATE_BOTTLE_A_EMPTY, LH_ERR_BOARD_MAIN_LIQUID_AUTO_CONCENTRATE_BOTTLE_A_EMPTY);
            }
            else if(*bottleEmptyStateB == SENSOR_STATE_VALID)
            {
                //浓缩液B瓶不足
                Service_Api_RespondWarning(CAN1_REPORT_ERR_LEVEL_WARNING, CAN1_REPORT_ERR_LIQUID_AUTO_CONCENTRATE_BOTTLE_B_EMPTY, LH_ERR_BOARD_MAIN_LIQUID_AUTO_CONCENTRATE_BOTTLE_B_EMPTY);
            }
        }
}
/*******************************************************************
**description: 自动运行状态机初始化 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_StateMachineInit(void)
{
    //初始化系统灯光
        OS_ERR err;
        LH_ERR errorCode = LH_ERR_NONE;
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //A B瓶状态
        SENSOR_STATE bottleEmptyStateA;
        SENSOR_STATE bottleEmptyStateB;
        //初始化上报
        lastReportLiquidAutoTickTime = OSTimeGet(&err);
    //初始化灯光状态
        ServiceImplLiquidAuto_LedStateCheck(&bottleEmptyStateA,&bottleEmptyStateB,&commErrorCode,&commErrorLevel);
    //初始化废液,纯水,清洗液稀释状态
        ServiceImplLiquidAutoWasteStateInit();
        ServiceImplLiquidAutoPureWaterSupplementStateInit();
        ServiceImplLiquidAutoCleanDilutionStateInit();
    return errorCode;
}

/*******************************************************************
**description: 自动运行状态机轮转 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_StateMachineLoop(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //A B瓶状态
        SENSOR_STATE bottleEmptyStateA;
        SENSOR_STATE bottleEmptyStateB;
    //更新灯光状态
        ServiceImplLiquidAuto_LedStateCheck(&bottleEmptyStateA,&bottleEmptyStateB,&commErrorCode,&commErrorLevel);
    //非老化模式,才会有状态循环
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //废液,纯水,清洗液状态循环
        ServiceImplLiquidAutoWasteStateLoop();
        ServiceImplLiquidAutoPureWaterSupplementStateLoop();
        ServiceImplLiquidAutoCleanDilutionStateLoop();
    return errorCode;
}

/*******************************************************************
**description: 强制执行一次抽废液 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_EnforceWasteDischargeOnce(SENSOR_STATE* fullSensorState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //如果打开了自动抽废液,则不能强制执行抽废液
        if(LIQUID_AUTO_STATE_ENABLE == ServiceStateReadLiquidAutoWasteState())
        {
            errorCode = LH_ERR_BOARD_MAIN_LIQUID_AUTO_WASTE_BUSY;
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_ENFORCE_WASTE_DISCHARGE_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //强制抽废液
        errorCode = Can2SubBoard1LiquidAuto_ForceTrigWasteLiquid();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_ENFORCE_WASTE_DISCHARGE_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //读取废液桶满传感器
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_LIQUID_WASTE_FULL_SENSOR,fullSensorState,commErrorCode,commErrorLevel);
    return errorCode;
}


/*******************************************************************
**description: 强制执行一次三针清洗稀释 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_EnforceThreeNeedleCleanDiluentOnce(SENSOR_STATE* bottleStateA,SENSOR_STATE* bottleStateB,
                                                                    SENSOR_STATE* bottleDiluentTransferFull,SENSOR_STATE* bottleDiluentTransferEmpty,
                                                                    SENSOR_STATE* bottleDiluentTargetFull,SENSOR_STATE* bottleDiluentTargetEmpty,
                                                                    CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //三针清洗稀释强制执行一次
        errorCode = ServiceImplLiquidAutoCleanDilutionEnforceOnce();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_ENFORCE_CLEAN_DILUTION_ONCE;
            /* 两瓶浓缩液都空则急停 */
            if(errorCode == LH_ERR_BOARD_MAIN_LIQUID_AUTO_CLEAN_DILUTION_SENSOR_CONCENTRATE_BOTTLE)
            {
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            }
            else
            {
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //读取一次传感器讯号
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LEVEL_EMPTY,bottleStateA,commErrorCode,commErrorLevel);
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LEVEL_EMPTY,bottleStateB,commErrorCode,commErrorLevel);
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_FULL,bottleDiluentTransferFull,commErrorCode,commErrorLevel);
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_EMPTY,bottleDiluentTransferEmpty,commErrorCode,commErrorLevel);
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_FULL,bottleDiluentTargetFull,commErrorCode,commErrorLevel);
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_EMPTY,bottleDiluentTargetEmpty,commErrorCode,commErrorLevel);
    return errorCode;
}


/*******************************************************************
**description: 强制执行一次纯水补水 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_EnforcePureWaterSupplementOnce(SENSOR_STATE* lowLiquidState,SENSOR_STATE* fullLiquidState,CAN1_REPORT_ERR_CODE* commErrorCode,
                                                                CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //纯水强制补水一次
        errorCode = ServiceImplLiquidAutoPureWaterSupplementEnforceOnce();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_ENFORCE_PURE_WATER_SUPPLEMENT_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //读取一次传感器信号
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_PURE_WATER_EMPTY_SENSOR,lowLiquidState,commErrorCode,commErrorLevel);
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_PURE_WATER_FULL_SENSOR,fullLiquidState,commErrorCode,commErrorLevel);
    return errorCode;
}

/*******************************************************************
**description: 使能自动抽废液 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_EnableWasteDischargeAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
     LH_ERR errorCode = LH_ERR_NONE;
     //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //打开自动抽废液
        errorCode = Can2SubBoard1LiquidAuto_EnableWasteLiquidAutomatic();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_ENABLE_WASTE_DISCHARGE_AUTO;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
        }
    //清除废液错误计数
        liquidAutoWasteFullSensorValidCount = 0;
    //打开废液桶抽废液状态
        ServiceStateWriteLiquidAutoWasteState(LIQUID_AUTO_STATE_ENABLE);
    return LH_ERR_NONE;
}


/*******************************************************************
**description: 使能自动三针清洗液稀释 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_EnableThreeNeedleCleanDiluentAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
     //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //打开三针自动稀释
        errorCode = ServiceImplLiquidAutoCleanDilutionEnable();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_ENABLE_CLEAN_DILUTION_AUTO;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return errorCode;
}

/*******************************************************************
**description: 使能自动存水补充 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_EnablePureWaterSupplementAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
     //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //打开自动纯水补充
        errorCode = ServiceImplLiquidAutoPureWaterSupplementEnable();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_ENABLE_PURE_WATER_SUPPLEMENT_AUTO;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return errorCode;
}

/*******************************************************************
**description: 禁止自动抽废液 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_DisableWasteDischargeAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
     //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //关闭自动抽废液
        errorCode = Can2SubBoard1LiquidAuto_DisableWasteLiquidAutomatic();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_DISABLE_WASTE_DISCHARGE_AUTO;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清除废液错误计数
        liquidAutoWasteFullSensorValidCount = 0;
    //关闭自动抽废液状态
        ServiceStateWriteLiquidAutoWasteState(LIQUID_AUTO_STATE_DISABLE);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 禁止自动三针清洗液稀释 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_DisableThreeNeedleCleanDiluentAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
     //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //关闭三针自动稀释
        errorCode = ServiceImplLiquidAutoCleanDilutionDisable();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_DISABLE_CLEAN_DILUTION_AUTO;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return errorCode;
}

/*******************************************************************
**description: 禁止存水自动补充 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_DisablePureWaterSupplementAuto(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
     //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //关闭自动纯水补充
        errorCode = ServiceImplLiquidAutoPureWaterSupplementDisable();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_DISABLE_PURE_WATER_SUPPLEMENT_AUTO;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return errorCode;
}

/*******************************************************************
**description: 设定三针自动稀释浓缩液瓶号
******************************************************************/
LH_ERR ServiceImplLiquidAuto_SetThreeNeedleConcentrateBottleNumber(CONCENTRATE_BOTTLE_SELECT concentrateBottleNumber,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(concentrateBottleNumber > CONCENTRATE_BOTTLE_SELECT_BOTTLE_2)
    {
        concentrateBottleNumber =CONCENTRATE_BOTTLE_SELECT_BOTTLE_2;
    }
    ServiceStateWriteConcentrateBottleNumber(concentrateBottleNumber);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 读取模块内部指定输入的状态 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplLiquidAuto_ModuleInputRead((INPUT_LIQUID_AUTO_NAME)inputIndex,inputState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
    }
    return errorCode;
}

/*******************************************************************
**description: 写入模块指定输出的状态 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplLiquidAuto_ModuleOutputWrite((OUTPUT_LIQUID_AUTO_NAME)outputIndex,setState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
    }
    return errorCode;
}

/*******************************************************************
**description: 指定模块特定电机复位 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 指定模块特动电机运行指定步数 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}


/*******************************************************************
**description: 配置瓶注一次纯水
******************************************************************/
LH_ERR ServiceImplLiquidAuto_ConfigAddPureWaterOnce(uint32_t pulseMaxCount,uint32_t *sensorPulseCount,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //自动稀释状态下不能执行
        if(LIQUID_AUTO_STATE_ENABLE == ServiceStateReadLiquidAutoCleanDilutionState())
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_CONFIG_ADD_PURE_WATER_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return LH_ERR_BOARD_MAIN_LIQUID_AUTO_CLEAN_DILUTION_BUSY;
        }
    //配置瓶注一次纯水
        errorCode = ServiceImplLiquidAutoConfigAddPureWater(pulseMaxCount,sensorPulseCount);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_CONFIG_ADD_PURE_WATER_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 配置瓶注一次浓缩液
******************************************************************/
LH_ERR ServiceImplLiquidAuto_ConfigAddConcentrateOnce(uint8_t concentrateBottle,uint16_t injectMaxTime,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //自动稀释状态下不能执行
        if(LIQUID_AUTO_STATE_ENABLE == ServiceStateReadLiquidAutoCleanDilutionState())
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_CONFIG_ADD_CONCENTRATE_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return LH_ERR_BOARD_MAIN_LIQUID_AUTO_CLEAN_DILUTION_BUSY;
        }
    //配置瓶注一次浓缩液
        errorCode = ServiceImplLiquidAutoConfigAddConcentrate(concentrateBottle,injectMaxTime);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_CONFIG_ADD_CONCENTRATE_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}


/*******************************************************************
**description: 配置瓶向工作瓶抽一次
******************************************************************/
LH_ERR ServiceImplLiquidAuto_ConfigToTargetOnce(CONFIG_TO_TARGET_TRIG_FLAG *trigFlag,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //老化模式不执行
        if(MODE_DEBUG == ServiceStateReadAssistRunningMode())
        {
            return errorCode;
        }
    //自动稀释状态下不能执行
        if(LIQUID_AUTO_STATE_ENABLE == ServiceStateReadLiquidAutoCleanDilutionState())
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_CONFIG_TO_TARGET_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return LH_ERR_BOARD_MAIN_LIQUID_AUTO_CLEAN_DILUTION_BUSY;
        }
    //配置瓶向工作瓶抽
        errorCode = ServiceImplLiquidAutoConfigToTarget(trigFlag);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_CONFIG_TO_TARGET_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}
















