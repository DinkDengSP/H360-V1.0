/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:51:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-18 09:21:12
**FilePath: \Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplAssist.c
******************************************************************/

#include "ServiceImplAssist.h"
#include "ServiceStatePrivate.h"
#include "TestFlow_ActionAPI.h"

/*******************************************************************
**description: 自动运行状态机初始化 
******************************************************************/
LH_ERR ServiceImplAssist_StateMachineInit(void)
{
    return LH_ERR_NONE;
}

//循环因子
uint8_t loopTimes = 0;
//读急停按钮输入
static LH_ERR ServiceImplAssist_CheckEmergencyInputKey(void);
/*******************************************************************
**description: 自动运行状态机轮转,心跳闪烁绿灯 
******************************************************************/
LH_ERR ServiceImplAssist_StateMachineLoop(void)
{
    loopTimes++;
    if(loopTimes >= 5)
    {
        loopTimes = 0;
        BoardLedToogle(BOARD_LED_GREEN);
    }
    //急停输入监测
    ServiceImplAssist_CheckEmergencyInputKey();
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 电机复位
**param: 电机序号
**return: 
**Author: DengXiaoJun
**LastEditors: DengXiaoJun
**LastEditTime: 
**LastModifyRecord: 
******************************************************************/
LH_ERR ServiceImplAssist_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

    //电机序号检测
    if(motorIndex >= (uint8_t)(CAN2_SUB_SM_BOARD6_SM1))
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_SM_INDEX;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_MOTOR_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //复位变量
    SM_RESET_CMD resetCommand;
    Can2SubSM_ResetCmdDataStructInit(&resetCommand);

    //设置复位系数
    resetCommand.stepMotorIndex = (CAN2_SUB_SM_INDEX)(motorIndex);
    //不修正
    resetCommand.correctionPosition = 0;

    //调用复位函数
    errorCode = Can2SubSM_ResetWhileReturn(&resetCommand);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_MOTOR_RESET;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }

    return LH_ERR_NONE;
}

/*******************************************************************
**description: 电机走步数
**param: 电机序号
**return: 
**Author: DengXiaoJun
**LastEditors: DengXiaoJun
**LastEditTime: 
**LastModifyRecord: 
******************************************************************/
LH_ERR ServiceImplAssist_MotorRunSteps(uint8_t motorIndex,int32_t steps,uint8_t curveSelect,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

    //电机序号检测
    if(motorIndex >= (uint8_t)(CAN2_SUB_SM_BOARD6_SM1))
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_SM_INDEX;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_MOTOR_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //走位变量
    SM_RUN_STEP_CMD smRunStepCmd;
    Can2SubSM_RunStepsCmdDataStructInit(&smRunStepCmd);

    //设置
    smRunStepCmd.stepMotorIndex = (CAN2_SUB_SM_INDEX)(motorIndex);
    smRunStepCmd.steps = steps;
    //曲线设置检查
    if(curveSelect > SM_CURVE_20)
    {
        errorCode = LH_ERR_BOARD_MAIN_ASSIST_SM_CURVE_SELECT;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_MOTOR_RUN_CURVE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    smRunStepCmd.curveSelect = (SM_CURVE_SELECT)(curveSelect);

    //运行指定步数
    errorCode = Can2SubSM_RunSpecialStepsWhileReturn(&smRunStepCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_MOTOR_RUN_STEPS;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }

    //读取步数
    errorCode = Can2SubSM_ReadPositionWhileReturn(smRunStepCmd.stepMotorIndex,posAfterRun);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_MOTOR_READ_POS;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }

    return LH_ERR_NONE;
}

/*******************************************************************
**description: 输入读取 
******************************************************************/
LH_ERR ServiceImplAssist_InputRead(uint16_t inputIndex,BitAction* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

    //输入信号序号检测
    if(inputIndex > SYSTEM_INPUT_MAIN_BOARD_ARM_SEN32_PE4)
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_SYSTEM_IN_RANGE;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_INPUT_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }

    //读取值
    errorCode = SystemInputRead((SYSTEM_INPUT_INDEX)inputIndex,(BitAction*)inputState);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_INPUT_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 输出写入 
******************************************************************/
LH_ERR ServiceImplAssist_OutputWrite(uint16_t outputIndex,BitAction setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

    //输出信号序号检测
    if(outputIndex > SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT20_PD3)
    {
        errorCode = LH_ERR_BOARD_MAIN_COMM_SYSTEM_OUT_RANGE;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_OUTPUT_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }

    //写入值
    errorCode = SystemOutputWrite((SYSTEM_OUTPUT_INDEX)outputIndex,setState);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_OUTPUT_WRITE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 读取软件版本 
******************************************************************/
LH_ERR ServiceImplAssist_ReadSoftwareVersion(VERSION_BOARD_ID boardID,uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BOARD_APP_VERSION appVersion;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(boardID == VERSION_BOARD_MAIN)
    {
        //主控板
        BoardVersion_ReadAppVersion(&appVersion);
        *mainVersion = appVersion.mainVersion;
        *subVersion = appVersion.subVersion;
        *debugVersion = appVersion.debugVersion;
        *svnVersion = appVersion.svnVersion;
    }
    else if(boardID == VERSION_BOARD_ACTION_BOARD1)
    {
        //下位机1
        errorCode = Can2SubUtil_ReadSoftwareVersion(CAN2_SUB_BOARD1,READ_SOFTWARE_VERSION_APP,mainVersion,subVersion,debugVersion,svnVersion);
    }
    else if(boardID == VERSION_BOARD_ACTION_BOARD2)
    {
        //下位机2
        errorCode = Can2SubUtil_ReadSoftwareVersion(CAN2_SUB_BOARD2,READ_SOFTWARE_VERSION_APP,mainVersion,subVersion,debugVersion,svnVersion);
    }
    else if(boardID == VERSION_BOARD_ACTION_BOARD3)
    {
        //下位机3
        errorCode = Can2SubUtil_ReadSoftwareVersion(CAN2_SUB_BOARD3,READ_SOFTWARE_VERSION_APP,mainVersion,subVersion,debugVersion,svnVersion);
    }
    else if(boardID == VERSION_BOARD_ACTION_BOARD4)
    {
        //下位机4
        errorCode = Can2SubUtil_ReadSoftwareVersion(CAN2_SUB_BOARD4,READ_SOFTWARE_VERSION_APP,mainVersion,subVersion,debugVersion,svnVersion);
    }
    else if(boardID == VERSION_BOARD_ACTION_BOARD5)
    {
        //下位机5
        errorCode = Can2SubUtil_ReadSoftwareVersion(CAN2_SUB_BOARD5,READ_SOFTWARE_VERSION_APP,mainVersion,subVersion,debugVersion,svnVersion);
    }
    else if(boardID == VERSION_BOARD_TC_BOARD)
    {
        //温控板
        errorCode = Can2SubBoardTcGetSoftwareVersion(READ_SOFTWARE_VERSION_APP,mainVersion,subVersion,debugVersion,svnVersion);
    }
    else
    {
        errorCode = LH_ERR_BOARD_MAIN_ASSIST_VERSION_INDEX;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_VERSION_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_VERSION_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    ServiceStateWriteAssistSoftwareVersion(*mainVersion,*subVersion,*debugVersion,*svnVersion);
    return errorCode;
}

/*******************************************************************
**description: 读取动作参数 
******************************************************************/
LH_ERR ServiceImplAssist_ReadActionParam(uint8_t mainIndex,uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return ServiceImplAssistUtilActionParam_Read(mainIndex,subIndex,paramRead,commErrorCode,commErrorLevel);
}

/*******************************************************************
**description: 写入动作参数 
******************************************************************/
LH_ERR ServiceImplAssist_WriteActionParam(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return ServiceImplAssistUtilActionParam_Write(mainIndex,subIndex,paramWrite,commErrorCode,commErrorLevel);
}

/*******************************************************************
**description: 读取系统状态 
******************************************************************/
LH_ERR ServiceImplAssist_ReadSystemState(uint8_t mainIndex,uint8_t subIndex,int32_t* stateRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 写入系统状态 
******************************************************************/
LH_ERR ServiceImplAssist_WriteSystemState(uint8_t mainIndex,uint8_t subIndex,int32_t stateWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 读取电机参数 
******************************************************************/
LH_ERR ServiceImplAssist_ReadMotorParam(uint8_t mainIndex,uint8_t subIndex,int32_t* paramRead,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 写入电机参数 
******************************************************************/
LH_ERR ServiceImplAssist_WriteMotorParam(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 握手 
******************************************************************/
LH_ERR ServiceImplAssist_HandShake(MODE_RUNNING modeRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //下位机1握手
    errorCode = Can2SubUtil_HandShake(CAN2_SUB_BOARD1,modeRun);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_HAND_SHAKE_ACTION_BOARD1;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    else
    {
        //正常模式,下位机1握手无错误,打开自动抽废液
        if(modeRun == MODE_RELEASE)
        {
            errorCode = Can2SubBoard1LiquidAuto_EnableWasteLiquidAutomatic();
            if(errorCode != LH_ERR_NONE)
            {
                //错误代码打印
                ServicePrintfShowError(errorCode);
            }
        }
    }
    //下位机2握手
    errorCode = Can2SubUtil_HandShake(CAN2_SUB_BOARD2,modeRun);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_HAND_SHAKE_ACTION_BOARD2;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    //下位机3握手
    errorCode = Can2SubUtil_HandShake(CAN2_SUB_BOARD3,modeRun);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_HAND_SHAKE_ACTION_BOARD3;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    //下位机4握手
    errorCode = Can2SubUtil_HandShake(CAN2_SUB_BOARD4,modeRun);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_HAND_SHAKE_ACTION_BOARD4;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    //下位机5握手
    errorCode = Can2SubUtil_HandShake(CAN2_SUB_BOARD5,modeRun);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_HAND_SHAKE_ACTION_BOARD5;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    //温控握手
    errorCode = Can2SubBoardTcHandShake(modeRun);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_HAND_SHAKE_TC;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    //主控握手
    ServiceStateWriteAssistRunningMode(modeRun);
    //清除试剂盘液位高度讯息
    ServiceStateClearPlateReagentLiquidDetectPos();
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 进入IAP模式 
******************************************************************/
LH_ERR ServiceImplAssist_EnterIapMode(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 传输IAP数据 
******************************************************************/
LH_ERR ServiceImplAssist_TransIapData(uint32_t startAddr,uint16_t dataLength,uint8_t* dataBuffer,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 开始IAP升级 
******************************************************************/
LH_ERR ServiceImplAssist_IapUpdate(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 查看IAP结果 
******************************************************************/
LH_ERR ServiceImplAssist_CheckIapResult(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 系统紧急急停 
******************************************************************/
LH_ERR ServiceImplAssist_SystemEmergencyStop(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //板卡1急停
    errorCode = Can2SubPortInAllMotorEmergency(CAN2_SUB_BOARD1);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_EMERGENCY_STOP;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    else
    {
        //关闭自动抽废液
        errorCode = Can2SubBoard1LiquidAuto_DisableWasteLiquidAutomatic();
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
        }
    }
    //板卡2急停
    errorCode = Can2SubPortInAllMotorEmergency(CAN2_SUB_BOARD2);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_EMERGENCY_STOP;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    //板卡3急停
    errorCode = Can2SubPortInAllMotorEmergency(CAN2_SUB_BOARD3);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_EMERGENCY_STOP;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    //板卡4急停
    errorCode = Can2SubPortInAllMotorEmergency(CAN2_SUB_BOARD4);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_EMERGENCY_STOP;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    //板卡5急停
    errorCode = Can2SubPortInAllMotorEmergency(CAN2_SUB_BOARD5);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        *commErrorCode = CAN1_REPORT_ERR_ASSIST_EMERGENCY_STOP;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
    }
    return errorCode;
}


/*******************************************************************
**description: 设备信息写入与读取
******************************************************************/
LH_ERR ServiceImplAssist_MachineMsgReadWrite(uint8_t *msgBuf,uint8_t readWriteFlag,uint16_t readWriteOffset,uint16_t bufLength, CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    if(msgBuf == NULL)
    {
        return errorCode;
    }
    if(bufLength > 1024)bufLength = 1024;
    if(readWriteFlag == 0)
    {
        AppParamReadMachineMsg(msgBuf,readWriteOffset,bufLength);
    }
    else if(readWriteFlag == 1)
    {
        AppParamWriteMachineMsg(msgBuf,readWriteOffset,bufLength);
    }
    return errorCode;
}


LH_ERR ServiceImplAssist_SetActionBoardModelVersion(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置下位机模块版本

    return errorCode;

}

//急停输入的按钮引脚
#define INPUT_ASSIST_EMERGENCY_KEY_SENSOR                       SYSTEM_INPUT_MAIN_BOARD_ARM_SEN30_PE2
//急停输入的有效电平
#define VALID_LEVEL_INPUT_ASSIST_EMERGENCY_KEY_SENSOR           Bit_SET
/*******************************************************************
**description: 急停按钮输入读取
******************************************************************/
LH_ERR ServiceImplAssist_CheckEmergencyInputKey(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //按钮有效次数
    static uint8_t keyVaildTimes = 0;
    //输入电平
    BitAction keyValue = Bit_RESET;
    //读输入信号
    errorCode = ServiceImplAssist_InputRead((uint16_t)INPUT_ASSIST_EMERGENCY_KEY_SENSOR,&keyValue,&commErrorCode,&commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //判判断输入信号有效值
    if(keyValue == VALID_LEVEL_INPUT_ASSIST_EMERGENCY_KEY_SENSOR)
    {
        keyVaildTimes++;
    }
    else
    {
        keyVaildTimes = 0;
    }
    //判断有效次数
    if(keyVaildTimes == 3)
    {
        //写入急停指令
        errorCode = ServiceImplAssist_SystemEmergencyStop(&commErrorCode,&commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
        //上报给上位机
        Service_Api_RespondWarning(CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP,CAN1_REPORT_ERR_BUTTON_EMERGENCY_STOP,LH_ERR_NONE);
        //上报给流程
        ServiceReportMsgToAppTask(TESTFLOW_MSG_CMD_EVENT_BUTTON_EMERGENCY_STOP,NULL,0);
    }
    else if(keyVaildTimes >= 4)
    {
        //防止多次写入急停
        keyVaildTimes = 4;
    }
    return errorCode;
}










