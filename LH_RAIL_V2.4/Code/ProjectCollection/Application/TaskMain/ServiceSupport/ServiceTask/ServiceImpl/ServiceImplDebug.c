/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:24:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-15 10:51:23
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplDebug.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplDebug.h"

//自动运行任务的周期,单位毫秒
#define DEBUG_EVENT_PERIOD_TIME_MS          200
//自动运行任务的触发次数,决定是否执行任务
static uint32_t countDebugEventTrigCount = 0;

//只需要执行的事件回调函数,不需要计算时间
static void ServiceImplDebug_EventJustRun(void)
{
    asm("nop");
}

//自动循环事件初始化
void ServiceImplDebug_EventInit(void)
{
    countDebugEventTrigCount = 0;
}
//自动循环事件
void ServiceImplDebug_EventRun(uint32_t periodTimeMs)
{
    if(periodTimeMs == 0)
    {
        return;
    }
    //心跳时间计时器增加
    countDebugEventTrigCount++;
    //如果执行周期大于事件周期,立即运行,如果计数器事件次数大于事件次数,也执行
    if((periodTimeMs >= DEBUG_EVENT_PERIOD_TIME_MS)||(countDebugEventTrigCount >= (uint32_t)(DEBUG_EVENT_PERIOD_TIME_MS/periodTimeMs)))
    {
        countDebugEventTrigCount = 0;
        //执行一次自动循环任务
        ServiceImplDebug_EventJustRun();
    }
}

/*************************************轨道主控自身使用的指令*****************************************/

//通用调试电机复位
LH_ERR ServiceImplDebugMotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //电机序号检测
        if(motorIndex >= 12)
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_SM_INDEX;
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BASE_MOTOR_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //复位变量
        SM_RESET_CMD resetCommand;
        Can2SubSM_ResetCmdDataStructInit(&resetCommand);
        //设置复位系数
        resetCommand.stepMotorIndex = (CAN2_SUB_SM_INDEX)(motorIndex+CAN2_SUB_SM_BOARD8_SM1);
        //不修正
        resetCommand.correctionPosition = 0;
    //调用复位函数
        errorCode = Can2SubSM_ResetWhileReturn(&resetCommand);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_DEBUG_MOTOR_RESET;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}

//通用调试电机走步数
LH_ERR ServiceImplDebugMotorRunSteps(uint8_t motorIndex,uint8_t dir,uint32_t steps,uint8_t curveS,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //电机序号检测
        if(motorIndex >= 12)
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_SM_INDEX;
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BASE_MOTOR_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //走位变量
        SM_RUN_STEP_CMD smRunStepCmd;
        Can2SubSM_RunStepsCmdDataStructInit(&smRunStepCmd);
    //设置
        smRunStepCmd.stepMotorIndex = (CAN2_SUB_SM_INDEX)(motorIndex + CAN2_SUB_SM_BOARD8_SM1);
        //0为负向,1为正向
        smRunStepCmd.steps = steps;
        if(dir == 0)
        {
            smRunStepCmd.steps = 0 - smRunStepCmd.steps;
        }
        smRunStepCmd.curveSelect = (SM_CURVE_SELECT)(curveS);
    //运行指定步数
        errorCode = Can2SubSM_RunSpecialStepsWhileReturn(&smRunStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_DEBUG_MOTOR_RUN_STEPS;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //读取步数
        errorCode = Can2SubSM_ReadPositionWhileReturn(smRunStepCmd.stepMotorIndex,posAfterRun);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_DEBUG_MOTOR_RUN_STEPS;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}

//读取指定调试序号的IO口输入
LH_ERR ServiceImplDebugInputRead(uint16_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查序号
        if(pinIndex > (uint16_t)(SYSTEM_INPUT_RAIL_BOARD_ARM_SEN32_PE4))
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX;
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BASE_INPUT_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //读取值
        errorCode = SystemInputRead((SYSTEM_INPUT_INDEX)pinIndex,(BitAction*)pinValue);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_DEBUG_INPUT_READ;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}

//写入指定调试序号的IO口输出
LH_ERR ServiceImplDebugOutputWrite(uint16_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查序号
        if(pinIndex > (uint16_t)(SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT20_PD3))
        {
            errorCode = LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX;
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BASE_OUTPUT_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //写入值
        errorCode = SystemOutputWrite((SYSTEM_OUTPUT_INDEX)pinIndex,(BitAction)pinValue);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_DEBUG_OUTPUT_WRITE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}

//读取指定系统参数
LH_ERR ServiceImplDebugReadSystemParam(uint8_t mainIndex,uint8_t subIndex,int32_t* paramResult,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读取下位机存储的参数值
        errorCode = Can2SubRailParamRead((CAN2_SUB_RAIL_PARAM_MAIN_INDEX)mainIndex,subIndex,paramResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BASE_PARAM_READ;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}

//写入指定系统参数
LH_ERR ServiceImplDebugWriteSystemParam(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读取下位机存储的参数值
        errorCode = Can2SubRailParamWrite((CAN2_SUB_RAIL_PARAM_MAIN_INDEX)mainIndex,subIndex,paramWrite);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BASE_PARAM_WRITE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}

//读取版本号
LH_ERR ServiceImplDebugReadBoardVersion(uint8_t boardNo,uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //版本号讯息
    BOARD_APP_VERSION  appVersion;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //板号识别
        if(boardNo == 0)
        {
            //读取本机APP版本
            BoardVersion_ReadAppVersion(&appVersion);
            *mainVersion = appVersion.mainVersion;
            *subVersion = appVersion.subVersion;
            *debugVersion = appVersion.debugVersion;
            *svnVersion = appVersion.svnVersion;
        }
        else
        {
            errorCode = Can2SubActionBoardReadVersion(CAN2_SUB_BOARD8,READ_SOFTWARE_VERSION_APP,mainVersion,subVersion,debugVersion,svnVersion);
            if(errorCode != LH_ERR_NONE)
            {
                //错误代码打印
                SystemDebugShowMsgError(errorCode);
                *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BASE_READ_VERSION;
                *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
                return errorCode;
            }
        }
    return LH_ERR_NONE;
}

//读取指定电机参数
LH_ERR ServiceImplDebugReadMotorParam(uint8_t mainIndex,uint8_t subIndex,int32_t* paramResult,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //目前未实现
        *paramResult = 0;
    return errorCode;
}

//写入指定电机参数
LH_ERR ServiceImplDebugWriteMotorParam(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //目前未实现

    return errorCode;
}

//通讯握手
LH_ERR ServiceImplDebugHandShake(uint8_t shakeMode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //握手模式记录
        shakeMode = shakeMode;
    return errorCode;
}

//系统急停
LH_ERR ServiceImplDebugEmergencyStop(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //紧急急停
        errorCode = Can2SubPortInAllMotorEmergency(CAN2_SUB_BOARD8);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BASE_EMERGENCY_STOP;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}
