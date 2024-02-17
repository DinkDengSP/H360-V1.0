/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-28 11:54:28
**FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplPushOut1.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplPushOut1.h"


//转换出样1错误代码的标识
typedef enum FLAG_PUSH_OUT_1_ERR_CONVERT
{
    FLAG_PUSH_OUT_1_ERR_RESET                    = 0X01,//出样1复位失败
    FLAG_PUSH_OUT_1_ERR_RUNNING                  = 0X02,//出样1运行失败
    FLAG_PUSH_OUT_1_ERR_RETURN_ZERO              = 0X03,//出样1回零失败
}FLAG_PUSH_OUT_1_ERR_CONVERT;

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplPushOut1ConvertErrorCode(FLAG_PUSH_OUT_1_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_PUSH_OUT_1;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD8_RAIL_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}

//出样1当前前推的次数
static uint8_t currentPushOut1PushRackIndex = 0;

//出样1复位
LH_ERR AppImplPushOut1Reset(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //复位结构体
        StepMotorResetCmd resetCmd;
    //首先复位
        if(resetCorrectFlag == RAIL_RESET_CORRECT_OFF)
        {
            paramUtil = 0;
        }
        else
        {
            AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_OUT1,APP_PARAM_INDEX_SUB_PUSH_OUT_RESET_CORRECTION,&paramUtil);
        }
    //开始复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = PUSH_OUT_1_STEP_MOTOR_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 55000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushOut1ConvertErrorCode(FLAG_PUSH_OUT_1_ERR_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //清空记录器
        currentPushOut1PushRackIndex = 0;
    return LH_ERR_NONE;
}

//出样1推进一次并回零
LH_ERR AppImplPushOut1PushOnceWithReturnZero(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        //回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //推进一次
        // AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_OUT1,APP_PARAM_INDEX_SUB_PUSH_OUT_ONE_RACK_OFFSET,&paramUtil);
        paramUtil = PUSH_OUT1_ONE_RACK_OFFSET;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
        runStepCmd.motorIndex = PUSH_OUT_1_STEP_MOTOR_ID;//电机序号
        runStepCmd.specialSteps = paramUtil;//步数
        runStepCmd.selectConfig = CURVE_PUSHOUT1_PUSH_WITH_ZERO;//曲线
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushOut1ConvertErrorCode(FLAG_PUSH_OUT_1_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //回零
        AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_OUT1,APP_PARAM_INDEX_SUB_PUSH_OUT_RESET_CORRECTION,&paramUtil);
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = PUSH_OUT_1_STEP_MOTOR_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = CURVE_PUSHOUT1_RETURN_ZERO;
        returnZeroCmd.timeOutSet = 55000;
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushOut1ConvertErrorCode(FLAG_PUSH_OUT_1_ERR_RETURN_ZERO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //推进次数清零
        currentPushOut1PushRackIndex = 0;
    return LH_ERR_NONE;
}


//出样推进一次并不回零
LH_ERR AppImplPushOut1PushOnceNotReturnZero(uint8_t* pushOutRackIndex)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
    //推进一次
        // AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_OUT1,APP_PARAM_INDEX_SUB_PUSH_OUT_ONE_RACK_OFFSET,&paramUtil);
        paramUtil = PUSH_OUT1_ONE_RACK_OFFSET;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
        runStepCmd.motorIndex = PUSH_OUT_1_STEP_MOTOR_ID;//电机序号
        runStepCmd.specialSteps = paramUtil;//步数
        runStepCmd.selectConfig = CURVE_PUSHOUT1_PUSH;//曲线
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushOut1ConvertErrorCode(FLAG_PUSH_OUT_1_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //推进次数增加
        currentPushOut1PushRackIndex++;
    //不回零,返回数据
        *pushOutRackIndex = currentPushOut1PushRackIndex;
    return LH_ERR_NONE;
}

//出样回零
LH_ERR AppImplPushOut1ReturnZero(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //回零
        AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_OUT1,APP_PARAM_INDEX_SUB_PUSH_OUT_RESET_CORRECTION,&paramUtil);
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = PUSH_OUT_1_STEP_MOTOR_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = CURVE_PUSHOUT1_RETURN_ZERO;
        returnZeroCmd.timeOutSet = 55000;
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushOut1ConvertErrorCode(FLAG_PUSH_OUT_1_ERR_RETURN_ZERO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //清空记录器
        currentPushOut1PushRackIndex = 0;
    return LH_ERR_NONE;
}

//获取当前推到第几次
LH_ERR AppImplPushOut1GetIndex(uint8_t* pushOutRackIndex)
{
    //获取序号
    *pushOutRackIndex = currentPushOut1PushRackIndex;
    //返回结果
    return LH_ERR_NONE;
}

//出样推进一次并不回零,按键专用
LH_ERR AppImplPushOut1ButtonPushOnceNotReturnZero(uint8_t* pushOutRackIndex)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
    //推进一次
        // AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_OUT1,APP_PARAM_INDEX_SUB_PUSH_OUT_ONE_RACK_OFFSET,&paramUtil);
        paramUtil = PUSH_OUT1_ONE_RACK_OFFSET;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
        runStepCmd.motorIndex = PUSH_OUT_1_STEP_MOTOR_ID;//电机序号
        runStepCmd.specialSteps = paramUtil/PUSH_OUT1_PUSH_ONCE_STEPS_SCALE;//步数
        runStepCmd.selectConfig = CURVE_PUSHOUT1_PUSH_NOT_RETURN_ZERO;//曲线
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushOut1ConvertErrorCode(FLAG_PUSH_OUT_1_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //推进次数增加
        currentPushOut1PushRackIndex++;
    //不回零,返回数据
        *pushOutRackIndex = currentPushOut1PushRackIndex;
    return LH_ERR_NONE;
}