/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:32:24
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 16:21:09
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplBufferInject.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplBufferInjectVersion0.h"

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplBufferSolutionInjectConvertErrorCode(FLAG_BUFFER_SOLUTION_INJECT_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_BUFFER_INJECT;
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

/*******************************************缓冲液注液老化模式程序**************************************************/
//缓冲液泵阀复位,老化模式
static LH_ERR AppImplBufferSolutionInjectResetModeDebug(void)
{
    //老化模式,什么都不做
    CoreDelayMs(400);
    return LH_ERR_NONE;
}

//缓冲液注液,老化模式
static LH_ERR AppImplBufferSolutionInjectInjectModeDebug(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    //老化模式,什么都不做
    CoreDelayMs(400);
    return LH_ERR_NONE;
}

//缓冲液灌注,老化模式
static LH_ERR AppImplBufferSolutionInjectPrimeModeDebug(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    //老化模式,什么都不做
    CoreDelayMs(400);
    return LH_ERR_NONE;
}

//缓冲液清洗维护,老化模式
static LH_ERR AppImplBufferSolutionInjectMaintainPrimeModeDebug(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    //老化模式,什么都不做
    CoreDelayMs(400);
    return LH_ERR_NONE;
}

/*******************************************缓冲液注液测试模式程序**************************************************/
//缓冲液泵阀复位,测试模式
static LH_ERR AppImplBufferSolutionInjectResetModeRelease(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数配置
        //参数辅助,用于系统参数读取
        int32_t paramUtil;
        //复位指令结构体
        StepMotorResetCmd resetCmd;
    //读取复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_BUFFER_INJECT,APP_PARAM_SUB_INDEX_BUFFER_INJECT_RESET_CORRECT_PUMP,&paramUtil);
    //配置复位指令结构体
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = BUFFER_INJECT_STEP_MOTOR_PUMP_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplBufferSolutionInjectConvertErrorCode(FLAG_BUFFER_SOLUTION_INJECT_ERR_PUMP_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //关闭DP2
        AppLiquidUtilClosePumpDP2Version0();
    //关闭V201
        AppLiquidUtilCloseValveV201Version0();
    //关闭V202
        AppLiquidUtilCloseValveV202Version0();
    //等待阀响应
        AppLiquidUtilWaitValveStableVersion0();
    return LH_ERR_NONE;
}

//正常模式,灌注
static LH_ERR AppImplBufferSolutionInjectPrimeModeRelease(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数配置
        //参数辅助,用于系统参数读取
        int32_t paramUtil;
        //复位指令结构体
        StepMotorResetCmd resetCmd;
        //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
    //打开DP2
        AppLiquidUtilOpenPumpDP2Version0();
    //等待泵响应
        AppLiquidUtilWaitPumpStableVersion0();
    //根据参数选通AB瓶
        if(bottltSelect == BOTTLE_BUFFER_SOLUTION_A)
        {
            AppLiquidUtilValveV202SelectBottleAVersion0();
        }
        else
        {
            AppLiquidUtilValveV202SelectBottleBVersion0();
        }
    //等待阀响应
        AppLiquidUtilWaitValveStableVersion0();
    //打开注液液路
        AppLiquidUtilOpenValveV201Version0();
    //等待阀响应
        AppLiquidUtilWaitValveStableVersion0();
    //注液一次走步数
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
        runStepCmd.motorIndex = BUFFER_INJECT_STEP_MOTOR_PUMP_ID;//电机序号
        runStepCmd.specialSteps = STEPS_FULL_CIRCLE_BUFFER_INJECT_PUMP;//步数
        runStepCmd.selectConfig = S_CURVE_BUFFER_INJECT_PUMP_PRIME;//曲线
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplBufferSolutionInjectConvertErrorCode(FLAG_BUFFER_SOLUTION_INJECT_ERR_PUMP_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //等待灌注稳定
        CoreDelayMsSensitivity(TIME_MS_WAIT_PRIME_STABLE);
    
    //关闭AB瓶选择
        AppLiquidUtilCloseValveV202Version0();
    //等待阀响应
        AppLiquidUtilWaitValveStableVersion0();
    //关闭注液通路
        AppLiquidUtilCloseValveV201Version0();
    //等待阀响应
        AppLiquidUtilWaitValveStableVersion0();
    
    //读取复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_BUFFER_INJECT,APP_PARAM_SUB_INDEX_BUFFER_INJECT_RESET_CORRECT_PUMP,&paramUtil);
    //注射泵复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = BUFFER_INJECT_STEP_MOTOR_PUMP_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplBufferSolutionInjectConvertErrorCode(FLAG_BUFFER_SOLUTION_INJECT_ERR_PUMP_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //关闭DP2
        AppLiquidUtilClosePumpDP2Version0();
    //等待泵稳定
        AppLiquidUtilWaitPumpStableVersion0();
    //返回结果
    return errorCode;
}

//缓冲液注液,正常模式
static LH_ERR AppImplBufferSolutionInjectInjectModeRelease(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数配置
        //参数辅助,用于系统参数读取
        int32_t paramUtil;
        //复位指令结构体
        StepMotorResetCmd resetCmd;
        //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
    //根据参数选通AB瓶
        if(bottltSelect == BOTTLE_BUFFER_SOLUTION_A)
        {
            AppLiquidUtilValveV202SelectBottleAVersion0();
        }
        else
        {
            AppLiquidUtilValveV202SelectBottleBVersion0();
        }
    //等待阀响应
        AppLiquidUtilWaitValveStableVersion0();
    //打开注液液路
        AppLiquidUtilOpenValveV201Version0();
    //等待阀响应
        AppLiquidUtilWaitValveStableVersion0();
    //注液一次走步数
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
        runStepCmd.motorIndex = BUFFER_INJECT_STEP_MOTOR_PUMP_ID;//电机序号
        runStepCmd.specialSteps = STEPS_FULL_CIRCLE_BUFFER_INJECT_PUMP;//步数
        runStepCmd.selectConfig = S_CURVE_BUFFER_INJECT_PUMP_INJECT;//曲线
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplBufferSolutionInjectConvertErrorCode(FLAG_BUFFER_SOLUTION_INJECT_ERR_PUMP_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //等待灌注稳定
        CoreDelayMsSensitivity(TIME_MS_WAIT_INJECT_STABLE);
    
    //关闭AB瓶选择
        AppLiquidUtilCloseValveV202Version0();
    //等待阀响应
        AppLiquidUtilWaitValveStableVersion0();
    //关闭注液通路
        AppLiquidUtilCloseValveV201Version0();
    //等待阀响应
        AppLiquidUtilWaitValveStableVersion0();
    
    //读取复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_BUFFER_INJECT,APP_PARAM_SUB_INDEX_BUFFER_INJECT_RESET_CORRECT_PUMP,&paramUtil);
    //注射泵复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = BUFFER_INJECT_STEP_MOTOR_PUMP_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplBufferSolutionInjectConvertErrorCode(FLAG_BUFFER_SOLUTION_INJECT_ERR_PUMP_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //返回结果
    return errorCode;
}


//缓冲液清洗维护,正常模式
static LH_ERR AppImplBufferSolutionInjectMaintainPrimeModeRelease(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    return LH_ERR_NONE;
}


/*******************************************面向外部的API**********************************************/
//缓冲液泵阀复位
LH_ERR AppImplBufferSolutionInjectResetVersion0(void)
{
    if(MODE_RELEASE == SystemModeGet())
    {
        //测试模式
        return AppImplBufferSolutionInjectResetModeRelease();
    }
    else
    {
        //老化模式
        return AppImplBufferSolutionInjectResetModeDebug();
    }
}

//缓冲液灌注
LH_ERR AppImplBufferSolutionInjectPrimeVersion0(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    if(MODE_RELEASE == SystemModeGet())
    {
        //测试模式
        return AppImplBufferSolutionInjectPrimeModeRelease(bottltSelect);
    }
    else
    {
        //老化模式
        return AppImplBufferSolutionInjectPrimeModeDebug(bottltSelect);
    }
}

//缓冲液注液
LH_ERR AppImplBufferSolutionInjectInjectVersion0(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    if(MODE_RELEASE == SystemModeGet())
    {
        //测试模式
        return AppImplBufferSolutionInjectInjectModeRelease(bottltSelect);
    }
    else
    {
        //老化模式
        return AppImplBufferSolutionInjectInjectModeDebug(bottltSelect);
    }
}

//缓冲液清洗维护
LH_ERR AppImplBufferSolutionInjectMaintainPrimeVersion0(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    if(MODE_RELEASE == SystemModeGet())
    {
        //测试模式
        return AppImplBufferSolutionInjectMaintainPrimeModeRelease(bottltSelect);
    }
    else
    {
        //老化模式
        return AppImplBufferSolutionInjectMaintainPrimeModeDebug(bottltSelect);
    }
}

//步进电机调试配置表
static const MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[] = {
    {BUFFER_INJECT_STEP_MOTOR_PUMP_ID,INDEX_CAN_SUB_BOARD1_PARAM_MAIN_BUFFER_INJECT,APP_PARAM_SUB_INDEX_BUFFER_INJECT_RESET_CORRECT_PUMP},//缓冲液注射泵
};

//指定步进电机复位
LH_ERR AppImplBufferSolutionInjectStepMotorResetVersion0(INDEX_MOTOR_BUFFER_SOLUTION index)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
        StepMotorResetCmd resetCmd;
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //读取修正
        AppParamRead((INDEX_CAN_SUB_BOARD1_PARAM_MAIN)(moduleStepMotorResetRunConfigArray[index].resetCorrectMainIndex),
                        moduleStepMotorResetRunConfigArray[index].resetCorrectSubIndex,
                        &paramUtil);
    //填充参数
        resetCmd.motorIndex = moduleStepMotorResetRunConfigArray[index].motorIndex;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    return LH_ERR_NONE;
}


//指定步进电机走指定步数
LH_ERR AppImplBufferSolutionInjectStepMotorRunStepsVersion0(INDEX_MOTOR_BUFFER_SOLUTION index,int32_t steps,int32_t* posAfterRun)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = moduleStepMotorResetRunConfigArray[index].motorIndex;
        runStepCmd.specialSteps = steps;
        runStepCmd.selectConfig = SM_CURVE_0;//默认曲线0作为调试曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取结果
        errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,posAfterRun);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}























