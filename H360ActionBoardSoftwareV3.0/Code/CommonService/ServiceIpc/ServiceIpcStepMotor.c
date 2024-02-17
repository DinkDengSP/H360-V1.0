/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-19 15:52:23
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-29 09:32:46
**FilePath: \H360_ActionBoard_ID_4d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceIpc\ServiceIpcStepMotor.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcStepMotor.h"
#include "ServiceIpcStepMotor1.h"
#include "ServiceIpcStepMotor2.h"
#include "ServiceIpcStepMotor3.h"
#include "ServiceIpcStepMotor4.h"
#include "ServiceIpcStepMotor5.h"
#include "ServiceIpcStepMotor6.h"
#include "ServiceIpcStepMotor7.h"
#include "ServiceIpcStepMotor8.h"
#include "ServiceIpcStepMotor9.h"
#include "ServiceIpcStepMotor10.h"
#include "ServiceIpcStepMotor11.h"
#include "ServiceIpcStepMotor12.h"

/*****************************************************变量填充默认值*********************************************************/
//复位指令的初始化
void IPC_StepMotorUtilResetCmdDataStructInit(StepMotorResetCmd* resetCmdPtr)
{
    //电机序号
    resetCmdPtr->motorIndex = STEP_MOTOR1;
    //复位修正
    resetCmdPtr->correctionCoordinate = 0;
    //超时时间
    resetCmdPtr->timeOutSet = 60000;
}

//走步数指令的初始化
void IPC_StepMotorUtilRunStepsCmdDataStructInit(StepMotorRunStepsCmd* runStepCmdPtr)
{
    //电机序号
    runStepCmdPtr->motorIndex = STEP_MOTOR1;
    //走步数
    runStepCmdPtr->specialSteps = 0;
    //使用的曲线
    runStepCmdPtr->selectConfig = 0;
    //速度曲线模式
    runStepCmdPtr->speedRatioMode = SPEED_RATIO_MODE_UP_START;
    //速度比例
    runStepCmdPtr->speedRatio = 100;
    //辅助急停1
    runStepCmdPtr->utilStop1Enable = 0;
    //辅助急停2
    runStepCmdPtr->utilStop2Enable = 0;
    //超时时间
    runStepCmdPtr->timeOutSet = 60000;
}

//走坐标指令的初始化
void IPC_StepMotorUtilRunCoordinateDataStructInit(StepMotorRun2CoordationCmd* runToPosCmdPtr)
{
    //电机序号
    runToPosCmdPtr->motorIndex = STEP_MOTOR1;
    //目标坐标
    runToPosCmdPtr->targetCoordinate = 0;
    //使用的曲线
    runToPosCmdPtr->selectConfig = 0;
    //速度模式
    runToPosCmdPtr->speedRatioMode = SPEED_RATIO_MODE_UP_START;
    //速度比例
    runToPosCmdPtr->speedRatio = 100;
    //辅助急停1
    runToPosCmdPtr->utilStop1Enable = 0;
    //辅助急停2
    runToPosCmdPtr->utilStop2Enable = 0;
    //超时时间
    runToPosCmdPtr->timeOutSet = 60000;
}

//回零指令的初始化
void IPC_StepMotorUtilReturnZeroDataStructInit(StepMotorReturnZeroCmd* returnZeroCmdPtr)
{
    //电机序号
    returnZeroCmdPtr->motorIndex = STEP_MOTOR1;
    //复位修正
    returnZeroCmdPtr->correctionCoordinate = 0;
    //使用的速度曲线
    returnZeroCmdPtr->selectConfig = 0;
    //速度模式
    returnZeroCmdPtr->speedRatioMode = SPEED_RATIO_MODE_UP_START;
    //速度比例
    returnZeroCmdPtr->speedRatio = 100;
    //辅助急停1
    returnZeroCmdPtr->utilStop1Enable = 0;
    //辅助急停2
    returnZeroCmdPtr->utilStop2Enable = 0;
    //超时时间
    returnZeroCmdPtr->timeOutSet = 60000;
}

//持续运转指令的初始化
void IPC_StepMotorUtilRunAlwaysDataStructInit(StepMotorRunAlwaysCmd* runAlwaysCmdPtr)
{
    //电机序号
    runAlwaysCmdPtr->motorIndex = STEP_MOTOR1;
    //选择的速度曲线
    runAlwaysCmdPtr->selectConfig = 0;
    //速度模式
    runAlwaysCmdPtr->speedRatioMode = SPEED_RATIO_MODE_UP_START;
    //速度比例
    runAlwaysCmdPtr->speedRatio = 100;
    //运转方向
    runAlwaysCmdPtr->dir = BOARD_CPLD_MOTOR_DIR_CW;
}

//输入任务的上一次执行结果,也就是有没有执行完成
typedef STATE_IPC (*IPC_StepMotorGetLastCommandStatusAndResultFuncPtr)(LH_ERR* resultCode);
static const IPC_StepMotorGetLastCommandStatusAndResultFuncPtr IPC_StepMotorGetLastCommandStatusAndResultFuncPtrArray[] = {
    IPC_StepMotor1GetLastCommandStatusAndResult,
    IPC_StepMotor2GetLastCommandStatusAndResult,
    IPC_StepMotor3GetLastCommandStatusAndResult,
    IPC_StepMotor4GetLastCommandStatusAndResult,
    IPC_StepMotor5GetLastCommandStatusAndResult,
    IPC_StepMotor6GetLastCommandStatusAndResult,
    IPC_StepMotor7GetLastCommandStatusAndResult,
    IPC_StepMotor8GetLastCommandStatusAndResult,
    IPC_StepMotor9GetLastCommandStatusAndResult,
    IPC_StepMotor10GetLastCommandStatusAndResult,
    IPC_StepMotor11GetLastCommandStatusAndResult,
    IPC_StepMotor12GetLastCommandStatusAndResult,
};

//等待执行完成,阻塞
typedef LH_ERR (*IPC_StepMotorWaitLastReturnFuncPtr)(void);
static const IPC_StepMotorWaitLastReturnFuncPtr IPC_StepMotorWaitLastReturnFuncPtrArray[] = {
    IPC_StepMotor1WaitLastReturn,
    IPC_StepMotor2WaitLastReturn,
    IPC_StepMotor3WaitLastReturn,
    IPC_StepMotor4WaitLastReturn,
    IPC_StepMotor5WaitLastReturn,
    IPC_StepMotor6WaitLastReturn,
    IPC_StepMotor7WaitLastReturn,
    IPC_StepMotor8WaitLastReturn,
    IPC_StepMotor9WaitLastReturn,
    IPC_StepMotor10WaitLastReturn,
    IPC_StepMotor11WaitLastReturn,
    IPC_StepMotor12WaitLastReturn,
};

//电机复位,不等待返回
typedef LH_ERR (*IPC_StepMotorResetWhileAckFuncPtr)(int32_t correction,uint32_t timeOut);
static const IPC_StepMotorResetWhileAckFuncPtr IPC_StepMotorResetWhileAckFuncPtrArray[] = {
    IPC_StepMotor1ResetWhileAck,
    IPC_StepMotor2ResetWhileAck,
    IPC_StepMotor3ResetWhileAck,
    IPC_StepMotor4ResetWhileAck,
    IPC_StepMotor5ResetWhileAck,
    IPC_StepMotor6ResetWhileAck,
    IPC_StepMotor7ResetWhileAck,
    IPC_StepMotor8ResetWhileAck,
    IPC_StepMotor9ResetWhileAck,
    IPC_StepMotor10ResetWhileAck,
    IPC_StepMotor11ResetWhileAck,
    IPC_StepMotor12ResetWhileAck,
};

//电机走步数,不等待返回
typedef LH_ERR (*IPC_StepMotorRunStepsWhileAckFuncPtr)(int32_t steps,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);
static const IPC_StepMotorRunStepsWhileAckFuncPtr IPC_StepMotorRunStepsWhileAckFuncPtrArray[] = {
    IPC_StepMotor1RunStepsWhileAck,
    IPC_StepMotor2RunStepsWhileAck,
    IPC_StepMotor3RunStepsWhileAck,
    IPC_StepMotor4RunStepsWhileAck,
    IPC_StepMotor5RunStepsWhileAck,
    IPC_StepMotor6RunStepsWhileAck,
    IPC_StepMotor7RunStepsWhileAck,
    IPC_StepMotor8RunStepsWhileAck,
    IPC_StepMotor9RunStepsWhileAck,
    IPC_StepMotor10RunStepsWhileAck,
    IPC_StepMotor11RunStepsWhileAck,
    IPC_StepMotor12RunStepsWhileAck,
};

//电机走坐标,不等待返回
typedef LH_ERR (*IPC_StepMotorRun2CoordinateWhileAckFuncPtr)(int32_t targetCoordinate,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);
static const IPC_StepMotorRun2CoordinateWhileAckFuncPtr IPC_StepMotorRun2CoordinateWhileAckFuncPtrArray[] = {
    IPC_StepMotor1Run2CoordinateWhileAck,
    IPC_StepMotor2Run2CoordinateWhileAck,
    IPC_StepMotor3Run2CoordinateWhileAck,
    IPC_StepMotor4Run2CoordinateWhileAck,
    IPC_StepMotor5Run2CoordinateWhileAck,
    IPC_StepMotor6Run2CoordinateWhileAck,
    IPC_StepMotor7Run2CoordinateWhileAck,
    IPC_StepMotor8Run2CoordinateWhileAck,
    IPC_StepMotor9Run2CoordinateWhileAck,
    IPC_StepMotor10Run2CoordinateWhileAck,
    IPC_StepMotor11Run2CoordinateWhileAck,
    IPC_StepMotor12Run2CoordinateWhileAck,
};

//电机回零,不等待返回
typedef LH_ERR (*IPC_StepMotorReturnZeroWhileAckFuncPtr)(int32_t correctionTarget,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);
static const IPC_StepMotorReturnZeroWhileAckFuncPtr IPC_StepMotorReturnZeroWhileAckFuncPtrArray[] = {
    IPC_StepMotor1ReturnZeroWhileAck,
    IPC_StepMotor2ReturnZeroWhileAck,
    IPC_StepMotor3ReturnZeroWhileAck,
    IPC_StepMotor4ReturnZeroWhileAck,
    IPC_StepMotor5ReturnZeroWhileAck,
    IPC_StepMotor6ReturnZeroWhileAck,
    IPC_StepMotor7ReturnZeroWhileAck,
    IPC_StepMotor8ReturnZeroWhileAck,
    IPC_StepMotor9ReturnZeroWhileAck,
    IPC_StepMotor10ReturnZeroWhileAck,
    IPC_StepMotor11ReturnZeroWhileAck,
    IPC_StepMotor12ReturnZeroWhileAck,
};

//电机持续运行,不等待返回
typedef LH_ERR (*IPC_StepMotorRunAlwaysWhileAckFuncPtr)(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t dir);
static const IPC_StepMotorRunAlwaysWhileAckFuncPtr IPC_StepMotorRunAlwaysWhileAckFuncPtrArray[] = {
    IPC_StepMotor1RunAlwaysWhileAck,
    IPC_StepMotor2RunAlwaysWhileAck,
    IPC_StepMotor3RunAlwaysWhileAck,
    IPC_StepMotor4RunAlwaysWhileAck,
    IPC_StepMotor5RunAlwaysWhileAck,
    IPC_StepMotor6RunAlwaysWhileAck,
    IPC_StepMotor7RunAlwaysWhileAck,
    IPC_StepMotor8RunAlwaysWhileAck,
    IPC_StepMotor9RunAlwaysWhileAck,
    IPC_StepMotor10RunAlwaysWhileAck,
    IPC_StepMotor11RunAlwaysWhileAck,
    IPC_StepMotor12RunAlwaysWhileAck,
};

//电机立即停止,不等待返回
typedef LH_ERR (*IPC_StepMotorStopImmediatelyWhileAckFuncPtr)(void);
static const IPC_StepMotorStopImmediatelyWhileAckFuncPtr IPC_StepMotorStopImmediatelyWhileAckFuncPtrArray[] = {
    IPC_StepMotor1StopImmediatelyWhileAck,
    IPC_StepMotor2StopImmediatelyWhileAck,
    IPC_StepMotor3StopImmediatelyWhileAck,
    IPC_StepMotor4StopImmediatelyWhileAck,
    IPC_StepMotor5StopImmediatelyWhileAck,
    IPC_StepMotor6StopImmediatelyWhileAck,
    IPC_StepMotor7StopImmediatelyWhileAck,
    IPC_StepMotor8StopImmediatelyWhileAck,
    IPC_StepMotor9StopImmediatelyWhileAck,
    IPC_StepMotor10StopImmediatelyWhileAck,
    IPC_StepMotor11StopImmediatelyWhileAck,
    IPC_StepMotor12StopImmediatelyWhileAck,
};

//电机减速停止,不等待返回
typedef LH_ERR (*IPC_StepMotorStopDecelerationWhileAckFuncPtr)(void);
static const IPC_StepMotorStopDecelerationWhileAckFuncPtr IPC_StepMotorStopDecelerationWhileAckFuncPtrArray[] = {
    IPC_StepMotor1StopDecelerationWhileAck,
    IPC_StepMotor2StopDecelerationWhileAck,
    IPC_StepMotor3StopDecelerationWhileAck,
    IPC_StepMotor4StopDecelerationWhileAck,
    IPC_StepMotor5StopDecelerationWhileAck,
    IPC_StepMotor6StopDecelerationWhileAck,
    IPC_StepMotor7StopDecelerationWhileAck,
    IPC_StepMotor8StopDecelerationWhileAck,
    IPC_StepMotor9StopDecelerationWhileAck,
    IPC_StepMotor10StopDecelerationWhileAck,
    IPC_StepMotor11StopDecelerationWhileAck,
    IPC_StepMotor12StopDecelerationWhileAck,
};

//更新当前电机坐标,不等待返回
typedef LH_ERR (*IPC_StepMotorReflushStateWhileAckFuncPtr)(void);
static const IPC_StepMotorReflushStateWhileAckFuncPtr IPC_StepMotorReflushStateWhileAckFuncPtrArray[] = {
    IPC_StepMotor1ReflushStateWhileAck,
    IPC_StepMotor2ReflushStateWhileAck,
    IPC_StepMotor3ReflushStateWhileAck,
    IPC_StepMotor4ReflushStateWhileAck,
    IPC_StepMotor5ReflushStateWhileAck,
    IPC_StepMotor6ReflushStateWhileAck,
    IPC_StepMotor7ReflushStateWhileAck,
    IPC_StepMotor8ReflushStateWhileAck,
    IPC_StepMotor9ReflushStateWhileAck,
    IPC_StepMotor10ReflushStateWhileAck,
    IPC_StepMotor11ReflushStateWhileAck,
    IPC_StepMotor12ReflushStateWhileAck,
};

//设置电机状态,不等待返回
typedef LH_ERR (*IPC_StepMotorSetStateWhileAckFuncPtr)(STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState);
static const IPC_StepMotorSetStateWhileAckFuncPtr IPC_StepMotorSetStateWhileAckFuncPtrArray[]   = {
    IPC_StepMotor1SetStateWhileAck,
    IPC_StepMotor2SetStateWhileAck,
    IPC_StepMotor3SetStateWhileAck,
    IPC_StepMotor4SetStateWhileAck,
    IPC_StepMotor5SetStateWhileAck,
    IPC_StepMotor6SetStateWhileAck,
    IPC_StepMotor7SetStateWhileAck,
    IPC_StepMotor8SetStateWhileAck,
    IPC_StepMotor9SetStateWhileAck,
    IPC_StepMotor10SetStateWhileAck,
    IPC_StepMotor11SetStateWhileAck,
    IPC_StepMotor12SetStateWhileAck,
};

//电机回零但是不复位,不等待返回
typedef LH_ERR (*IPC_StepMotorReturnZeroWithoutResetWhileAckFuncPtr)(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);
static const IPC_StepMotorReturnZeroWithoutResetWhileAckFuncPtr IPC_StepMotorReturnZeroWithoutResetWhileAckFuncPtrArray[] = {
    IPC_StepMotor1ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor2ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor3ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor4ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor5ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor6ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor7ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor8ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor9ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor10ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor11ReturnZeroWithoutResetWhileAck,
    IPC_StepMotor12ReturnZeroWithoutResetWhileAck,
};



//电机复位,等待返回
typedef LH_ERR (*IPC_StepMotorResetWhileReturnFuncPtr)();
static const IPC_StepMotorResetWhileReturnFuncPtr IPC_StepMotorResetWhileReturnFuncPtrArray[] = {
    IPC_StepMotor1ResetWhileReturn,
    IPC_StepMotor2ResetWhileReturn,
    IPC_StepMotor3ResetWhileReturn,
    IPC_StepMotor4ResetWhileReturn,
    IPC_StepMotor5ResetWhileReturn,
    IPC_StepMotor6ResetWhileReturn,
    IPC_StepMotor7ResetWhileReturn,
    IPC_StepMotor8ResetWhileReturn,
    IPC_StepMotor9ResetWhileReturn,
    IPC_StepMotor10ResetWhileReturn,
    IPC_StepMotor11ResetWhileReturn,
    IPC_StepMotor12ResetWhileReturn,
};

//电机走步数,等待返回
typedef LH_ERR (*IPC_StepMotorRunStepsWhileReturnFuncPtr)(int32_t steps,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);
static const IPC_StepMotorRunStepsWhileReturnFuncPtr IPC_StepMotorRunStepsWhileReturnFuncPtrArray[] = {
    IPC_StepMotor1RunStepsWhileReturn,
    IPC_StepMotor2RunStepsWhileReturn,
    IPC_StepMotor3RunStepsWhileReturn,
    IPC_StepMotor4RunStepsWhileReturn,
    IPC_StepMotor5RunStepsWhileReturn,
    IPC_StepMotor6RunStepsWhileReturn,
    IPC_StepMotor7RunStepsWhileReturn,
    IPC_StepMotor8RunStepsWhileReturn,
    IPC_StepMotor9RunStepsWhileReturn,
    IPC_StepMotor10RunStepsWhileReturn,
    IPC_StepMotor11RunStepsWhileReturn,
    IPC_StepMotor12RunStepsWhileReturn,
};

//电机走坐标,等待返回
typedef LH_ERR (*IPC_StepMotorRun2CoordinateWhileReturnFuncPtr)(int32_t targetCoordinate,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);
static const IPC_StepMotorRun2CoordinateWhileReturnFuncPtr IPC_StepMotorRun2CoordinateWhileReturnFuncPtrArray[] = {
    IPC_StepMotor1Run2CoordinateWhileReturn,
    IPC_StepMotor2Run2CoordinateWhileReturn,
    IPC_StepMotor3Run2CoordinateWhileReturn,
    IPC_StepMotor4Run2CoordinateWhileReturn,
    IPC_StepMotor5Run2CoordinateWhileReturn,
    IPC_StepMotor6Run2CoordinateWhileReturn,
    IPC_StepMotor7Run2CoordinateWhileReturn,
    IPC_StepMotor8Run2CoordinateWhileReturn,
    IPC_StepMotor9Run2CoordinateWhileReturn,
    IPC_StepMotor10Run2CoordinateWhileReturn,
    IPC_StepMotor11Run2CoordinateWhileReturn,
    IPC_StepMotor12Run2CoordinateWhileReturn,
};

//电机回零,等待返回
typedef LH_ERR (*IPC_StepMotorReturnZeroWhileReturnFuncPtr)(int32_t correctionTarget,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);
static const IPC_StepMotorReturnZeroWhileReturnFuncPtr IPC_StepMotorReturnZeroWhileReturnFuncPtrArray[] = {
    IPC_StepMotor1ReturnZeroWhileReturn,
    IPC_StepMotor2ReturnZeroWhileReturn,
    IPC_StepMotor3ReturnZeroWhileReturn,
    IPC_StepMotor4ReturnZeroWhileReturn,
    IPC_StepMotor5ReturnZeroWhileReturn,
    IPC_StepMotor6ReturnZeroWhileReturn,
    IPC_StepMotor7ReturnZeroWhileReturn,
    IPC_StepMotor8ReturnZeroWhileReturn,
    IPC_StepMotor9ReturnZeroWhileReturn,
    IPC_StepMotor10ReturnZeroWhileReturn,
    IPC_StepMotor11ReturnZeroWhileReturn,
    IPC_StepMotor12ReturnZeroWhileReturn,
};

//电机持续运行,等待返回
typedef LH_ERR (*IPC_StepMotorRunAlwaysWhileReturnFuncPtr)(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t dir);
static const IPC_StepMotorRunAlwaysWhileReturnFuncPtr IPC_StepMotorRunAlwaysWhileReturnFuncPtrArray[] = {
    IPC_StepMotor1RunAlwaysWhileReturn,
    IPC_StepMotor2RunAlwaysWhileReturn,
    IPC_StepMotor3RunAlwaysWhileReturn,
    IPC_StepMotor4RunAlwaysWhileReturn,
    IPC_StepMotor5RunAlwaysWhileReturn,
    IPC_StepMotor6RunAlwaysWhileReturn,
    IPC_StepMotor7RunAlwaysWhileReturn,
    IPC_StepMotor8RunAlwaysWhileReturn,
    IPC_StepMotor9RunAlwaysWhileReturn,
    IPC_StepMotor10RunAlwaysWhileReturn,
    IPC_StepMotor11RunAlwaysWhileReturn,
    IPC_StepMotor12RunAlwaysWhileReturn,
};

//电机立即停止,等待返回
typedef LH_ERR (*IPC_StepMotorStopImmediatelyWhileReturnFuncPtr)(void);
static const IPC_StepMotorStopImmediatelyWhileReturnFuncPtr IPC_StepMotorStopImmediatelyWhileReturnFuncPtrArray[] = {
    IPC_StepMotor1StopImmediatelyWhileReturn,
    IPC_StepMotor2StopImmediatelyWhileReturn,
    IPC_StepMotor3StopImmediatelyWhileReturn,
    IPC_StepMotor4StopImmediatelyWhileReturn,
    IPC_StepMotor5StopImmediatelyWhileReturn,
    IPC_StepMotor6StopImmediatelyWhileReturn,
    IPC_StepMotor7StopImmediatelyWhileReturn,
    IPC_StepMotor8StopImmediatelyWhileReturn,
    IPC_StepMotor9StopImmediatelyWhileReturn,
    IPC_StepMotor10StopImmediatelyWhileReturn,
    IPC_StepMotor11StopImmediatelyWhileReturn,
    IPC_StepMotor12StopImmediatelyWhileReturn,
};

//电机减速停止,等待返回
typedef LH_ERR (*IPC_StepMotorStopDecelerationWhileReturnFuncPtr)(void);
static const IPC_StepMotorStopDecelerationWhileReturnFuncPtr IPC_StepMotorStopDecelerationWhileReturnFuncPtrArray[] = {
    IPC_StepMotor1StopDecelerationWhileReturn,
    IPC_StepMotor2StopDecelerationWhileReturn,
    IPC_StepMotor3StopDecelerationWhileReturn,
    IPC_StepMotor4StopDecelerationWhileReturn,
    IPC_StepMotor5StopDecelerationWhileReturn,
    IPC_StepMotor6StopDecelerationWhileReturn,
    IPC_StepMotor7StopDecelerationWhileReturn,
    IPC_StepMotor8StopDecelerationWhileReturn,
    IPC_StepMotor9StopDecelerationWhileReturn,
    IPC_StepMotor10StopDecelerationWhileReturn,
    IPC_StepMotor11StopDecelerationWhileReturn,
    IPC_StepMotor12StopDecelerationWhileReturn,
};

//更新当前电机坐标,等待返回
typedef LH_ERR (*IPC_StepMotorReflushStateWhileReturnFuncPtr)(int32_t* currentCoordinate);
static const IPC_StepMotorReflushStateWhileReturnFuncPtr IPC_StepMotorReflushStateWhileReturnFuncPtrArray[] = {
    IPC_StepMotor1ReflushStateWhileReturn,
    IPC_StepMotor2ReflushStateWhileReturn,
    IPC_StepMotor3ReflushStateWhileReturn,
    IPC_StepMotor4ReflushStateWhileReturn,
    IPC_StepMotor5ReflushStateWhileReturn,
    IPC_StepMotor6ReflushStateWhileReturn,
    IPC_StepMotor7ReflushStateWhileReturn,
    IPC_StepMotor8ReflushStateWhileReturn,
    IPC_StepMotor9ReflushStateWhileReturn,
    IPC_StepMotor10ReflushStateWhileReturn,
    IPC_StepMotor11ReflushStateWhileReturn,
    IPC_StepMotor12ReflushStateWhileReturn,
};

//电机回零不复位,等待返回
typedef LH_ERR (*IPC_StepMotorReturnZeroWithoutResetWhileReturnFuncPtr)(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);
static const IPC_StepMotorReturnZeroWithoutResetWhileReturnFuncPtr IPC_StepMotorReturnZeroWithoutResetWhileReturnFuncPtrArray[] = {
    IPC_StepMotor1ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor2ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor3ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor4ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor5ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor6ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor7ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor8ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor9ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor10ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor11ReturnZeroWithoutResetWhileReturn,
    IPC_StepMotor12ReturnZeroWithoutResetWhileReturn,
};


//读取电机当前坐标,,等待返回
typedef int32_t (*IPC_StepMotorCurrentCoordinateFuncPtr)(void);
static const IPC_StepMotorCurrentCoordinateFuncPtr IPC_StepMotorCurrentCoordinateFuncPtrArray[] = {
    IPC_StepMotor1CurrentCoordinate,
    IPC_StepMotor2CurrentCoordinate,
    IPC_StepMotor3CurrentCoordinate,
    IPC_StepMotor4CurrentCoordinate,
    IPC_StepMotor5CurrentCoordinate,
    IPC_StepMotor6CurrentCoordinate,
    IPC_StepMotor7CurrentCoordinate,
    IPC_StepMotor8CurrentCoordinate,
    IPC_StepMotor9CurrentCoordinate,
    IPC_StepMotor10CurrentCoordinate,
    IPC_StepMotor11CurrentCoordinate,
    IPC_StepMotor12CurrentCoordinate,
};

//设置电机状态,等待返回
typedef LH_ERR (*IPC_StepMotorSetStateWhileReturnFuncPtr)(STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState);
static const IPC_StepMotorSetStateWhileReturnFuncPtr IPC_StepMotorSetStateWhileReturnFuncPtrArray[]   = {
    IPC_StepMotor1SetStateWhileReturn,
    IPC_StepMotor2SetStateWhileReturn,
    IPC_StepMotor3SetStateWhileReturn,
    IPC_StepMotor4SetStateWhileReturn,
    IPC_StepMotor5SetStateWhileReturn,
    IPC_StepMotor6SetStateWhileReturn,
    IPC_StepMotor7SetStateWhileReturn,
    IPC_StepMotor8SetStateWhileReturn,
    IPC_StepMotor9SetStateWhileReturn,
    IPC_StepMotor10SetStateWhileReturn,
    IPC_StepMotor11SetStateWhileReturn,
    IPC_StepMotor12SetStateWhileReturn,
};
//检查指定电机是否被指令急停
typedef LH_ERR (*IPC_StepMotorCheckEmergencyStopFuncPtr)(void);
static const IPC_StepMotorCheckEmergencyStopFuncPtr IPC_StepMotorCheckEmergencyStopFuncPtrArray[] = {
    IPC_StepMotor1CheckEmergencyStop,
    IPC_StepMotor2CheckEmergencyStop,
    IPC_StepMotor3CheckEmergencyStop,
    IPC_StepMotor4CheckEmergencyStop,
    IPC_StepMotor5CheckEmergencyStop,
    IPC_StepMotor6CheckEmergencyStop,
    IPC_StepMotor7CheckEmergencyStop,
    IPC_StepMotor8CheckEmergencyStop,
    IPC_StepMotor9CheckEmergencyStop,
    IPC_StepMotor10CheckEmergencyStop,
    IPC_StepMotor11CheckEmergencyStop,
    IPC_StepMotor12CheckEmergencyStop,
};
//强行设置电机当前坐标
typedef LH_ERR (*IPC_StepMotorModifyCoordinateFuncPtr)(int32_t modifyCoordinate);
static const IPC_StepMotorModifyCoordinateFuncPtr IPC_StepMotorModifyCoordinateFuncPtrArray[] = {
    IPC_StepMotor1ModifyCoordinate,
    IPC_StepMotor2ModifyCoordinate,
    IPC_StepMotor3ModifyCoordinate,
    IPC_StepMotor4ModifyCoordinate,
    IPC_StepMotor5ModifyCoordinate,
    IPC_StepMotor6ModifyCoordinate,
    IPC_StepMotor7ModifyCoordinate,
    IPC_StepMotor8ModifyCoordinate,
    IPC_StepMotor9ModifyCoordinate,
    IPC_StepMotor10ModifyCoordinate,
    IPC_StepMotor11ModifyCoordinate,
    IPC_StepMotor12ModifyCoordinate,
};

/*****************************************************等待任务完成*********************************************************/
//输入任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_StepMotorGetLastCommandStatusAndResult(STEP_MOTOR_INDEX smIndex,LH_ERR* resultCode)
{
    return IPC_StepMotorGetLastCommandStatusAndResultFuncPtrArray[smIndex](resultCode);
}

//等待执行完成,阻塞
LH_ERR IPC_StepMotorWaitLastReturn(STEP_MOTOR_INDEX smIndex)
{
    return IPC_StepMotorWaitLastReturnFuncPtrArray[smIndex]();
}


/******************************************只要求ACK不要求返回的指令**********************************************/
//电机复位
LH_ERR IPC_StepMotorResetWhileAck(StepMotorResetCmd* resetCmdPtr)
{
    return IPC_StepMotorResetWhileAckFuncPtrArray[resetCmdPtr->motorIndex](resetCmdPtr->correctionCoordinate,
                                                                                resetCmdPtr->timeOutSet);
}


//电机走步数
LH_ERR IPC_StepMotorRunStepsWhileAck(StepMotorRunStepsCmd* runStepCmdPtr)
{
    return IPC_StepMotorRunStepsWhileAckFuncPtrArray[runStepCmdPtr->motorIndex](runStepCmdPtr->specialSteps,
                                                                                    runStepCmdPtr->selectConfig,
                                                                                    runStepCmdPtr->speedRatio,
                                                                                    runStepCmdPtr->speedRatioMode,
                                                                                    runStepCmdPtr->utilStop1Enable,
                                                                                    runStepCmdPtr->utilStop2Enable,
                                                                                    runStepCmdPtr->timeOutSet);
}


//电机走坐标
LH_ERR IPC_StepMotorRun2CoordinateWhileAck(StepMotorRun2CoordationCmd* runToPosCmdPtr)
{
    return IPC_StepMotorRun2CoordinateWhileAckFuncPtrArray[runToPosCmdPtr->motorIndex](runToPosCmdPtr->targetCoordinate,
                                                                                            runToPosCmdPtr->selectConfig,
                                                                                            runToPosCmdPtr->speedRatio,
                                                                                            runToPosCmdPtr->speedRatioMode,
                                                                                            runToPosCmdPtr->utilStop1Enable,
                                                                                            runToPosCmdPtr->utilStop2Enable,
                                                                                            runToPosCmdPtr->timeOutSet);
}


//电机回零
LH_ERR IPC_StepMotorReturnZeroWhileAck(StepMotorReturnZeroCmd* returnZeroCmdPtr)
{
    return IPC_StepMotorReturnZeroWhileAckFuncPtrArray[returnZeroCmdPtr->motorIndex](returnZeroCmdPtr->correctionCoordinate,
                                                                                        returnZeroCmdPtr->selectConfig,
                                                                                        returnZeroCmdPtr->speedRatio,
                                                                                        returnZeroCmdPtr->speedRatioMode,
                                                                                        returnZeroCmdPtr->utilStop1Enable,
                                                                                        returnZeroCmdPtr->utilStop2Enable,
                                                                                        returnZeroCmdPtr->timeOutSet);
}


//电机持续运行
LH_ERR IPC_StepMotorRunAlwaysWhileAck(StepMotorRunAlwaysCmd* runAlwaysCmdPtr)
{
    return IPC_StepMotorRunAlwaysWhileAckFuncPtrArray[runAlwaysCmdPtr->motorIndex](runAlwaysCmdPtr->selectConfig,
                                                                                    runAlwaysCmdPtr->speedRatio,
                                                                                    runAlwaysCmdPtr->speedRatioMode,
                                                                                    (uint8_t)(runAlwaysCmdPtr->dir));
}


//电机立即停止
LH_ERR IPC_StepMotorStopImmediatelyWhileAck(STEP_MOTOR_INDEX smIndex)
{
    return IPC_StepMotorStopImmediatelyWhileAckFuncPtrArray[smIndex]();
}


//电机减速停止
LH_ERR IPC_StepMotorStopDecelerationWhileAck(STEP_MOTOR_INDEX smIndex)
{
    return IPC_StepMotorStopDecelerationWhileAckFuncPtrArray[smIndex]();
}


//更新当前电机坐标
LH_ERR IPC_StepMotorReflushStateWhileAck(STEP_MOTOR_INDEX smIndex)
{
    return IPC_StepMotorReflushStateWhileAckFuncPtrArray[smIndex]();
}

//设置电机状态
LH_ERR IPC_StepMotorSetStateWhileAck(STEP_MOTOR_INDEX smIndex,STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState)
{
    return IPC_StepMotorSetStateWhileAckFuncPtrArray[smIndex](motorMainState,motorSubState);
}

//电机回零但是不复位
LH_ERR IPC_StepMotorReturnZeroWithoutResetWhileAck(StepMotorReturnZeroCmd* returnZeroCmdPtr)
{
    return IPC_StepMotorReturnZeroWithoutResetWhileAckFuncPtrArray[returnZeroCmdPtr->motorIndex](returnZeroCmdPtr->selectConfig,
                                                                                                    returnZeroCmdPtr->speedRatio,
                                                                                                    returnZeroCmdPtr->speedRatioMode,
                                                                                                    returnZeroCmdPtr->utilStop1Enable,
                                                                                                    returnZeroCmdPtr->utilStop2Enable,
                                                                                                    returnZeroCmdPtr->timeOutSet);
}





/******************************************发出指令并等待返回的指令***********************************************/
//电机复位
LH_ERR IPC_StepMotorResetWhileReturn(StepMotorResetCmd* resetCmdPtr)
{
    return IPC_StepMotorResetWhileReturnFuncPtrArray[resetCmdPtr->motorIndex](resetCmdPtr->correctionCoordinate,
                                                                                resetCmdPtr->timeOutSet);
}


//电机走步数
LH_ERR IPC_StepMotorRunStepsWhileReturn(StepMotorRunStepsCmd* runStepCmdPtr)
{
    return IPC_StepMotorRunStepsWhileReturnFuncPtrArray[runStepCmdPtr->motorIndex](runStepCmdPtr->specialSteps,
                                                                                    runStepCmdPtr->selectConfig,
                                                                                    runStepCmdPtr->speedRatio,
                                                                                    runStepCmdPtr->speedRatioMode,
                                                                                    runStepCmdPtr->utilStop1Enable,
                                                                                    runStepCmdPtr->utilStop2Enable,
                                                                                    runStepCmdPtr->timeOutSet);
}


//电机走坐标
LH_ERR IPC_StepMotorRun2CoordinateWhileReturn(StepMotorRun2CoordationCmd* runToPosCmdPtr)
{
    return IPC_StepMotorRun2CoordinateWhileReturnFuncPtrArray[runToPosCmdPtr->motorIndex](runToPosCmdPtr->targetCoordinate,
                                                                                            runToPosCmdPtr->selectConfig,
                                                                                            runToPosCmdPtr->speedRatio,
                                                                                            runToPosCmdPtr->speedRatioMode,
                                                                                            runToPosCmdPtr->utilStop1Enable,
                                                                                            runToPosCmdPtr->utilStop2Enable,
                                                                                            runToPosCmdPtr->timeOutSet);
}


//电机回零
LH_ERR IPC_StepMotorReturnZeroWhileReturn(StepMotorReturnZeroCmd* returnZeroCmdPtr)
{
    return IPC_StepMotorReturnZeroWhileReturnFuncPtrArray[returnZeroCmdPtr->motorIndex](returnZeroCmdPtr->correctionCoordinate,
                                                                                        returnZeroCmdPtr->selectConfig,
                                                                                        returnZeroCmdPtr->speedRatio,
                                                                                        returnZeroCmdPtr->speedRatioMode,
                                                                                        returnZeroCmdPtr->utilStop1Enable,
                                                                                        returnZeroCmdPtr->utilStop2Enable,
                                                                                        returnZeroCmdPtr->timeOutSet);
}


//电机持续运行
LH_ERR IPC_StepMotorRunAlwaysWhileReturn(StepMotorRunAlwaysCmd* runAlwaysCmdPtr)
{
    return IPC_StepMotorRunAlwaysWhileReturnFuncPtrArray[runAlwaysCmdPtr->motorIndex](runAlwaysCmdPtr->selectConfig,
                                                                                    runAlwaysCmdPtr->speedRatio,
                                                                                    runAlwaysCmdPtr->speedRatioMode,
                                                                                    (uint8_t)(runAlwaysCmdPtr->dir));
}


//电机立即停止
LH_ERR IPC_StepMotorStopImmediatelyWhileReturn(STEP_MOTOR_INDEX smIndex)
{
    return IPC_StepMotorStopImmediatelyWhileReturnFuncPtrArray[smIndex]();
}


//电机减速停止
LH_ERR IPC_StepMotorStopDecelerationWhileReturn(STEP_MOTOR_INDEX smIndex)
{
    return IPC_StepMotorStopDecelerationWhileReturnFuncPtrArray[smIndex]();
}


//更新当前电机坐标
LH_ERR IPC_StepMotorReflushStateWhileReturn(STEP_MOTOR_INDEX smIndex,int32_t* currentCoordinate)
{
    return IPC_StepMotorReflushStateWhileReturnFuncPtrArray[smIndex](currentCoordinate);
}


//电机回零不复位
LH_ERR IPC_StepMotorReturnZeroWithoutResetWhileReturn(StepMotorReturnZeroCmd* returnZeroCmdPtr)
{
    return IPC_StepMotorReturnZeroWithoutResetWhileReturnFuncPtrArray[returnZeroCmdPtr->motorIndex](returnZeroCmdPtr->selectConfig,
                                                                                                        returnZeroCmdPtr->speedRatio,
                                                                                                        returnZeroCmdPtr->speedRatioMode,
                                                                                                        returnZeroCmdPtr->utilStop1Enable,
                                                                                                        returnZeroCmdPtr->utilStop2Enable,
                                                                                                        returnZeroCmdPtr->timeOutSet);
}

//设置电机状态
LH_ERR IPC_StepMotorSetStateWhileReturn(STEP_MOTOR_INDEX smIndex,STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState)
{
    return IPC_StepMotorSetStateWhileReturnFuncPtrArray[smIndex](motorMainState,motorSubState);
}

/*******************************************结果数据读取**************************************************/
//读取电机当前坐标
int32_t  IPC_StepMotorCurrentCoordinate(STEP_MOTOR_INDEX smIndex)
{
    return IPC_StepMotorCurrentCoordinateFuncPtrArray[smIndex]();
}

//获取当前指定电机是否被指令急停
LH_ERR IPC_StepMotorCheckEmergencyStop(STEP_MOTOR_INDEX smIndex)
{
    return IPC_StepMotorCheckEmergencyStopFuncPtrArray[smIndex]();
}

//强行设置电机当前坐标
int32_t  IPC_StepMotorModifyCoordinate(STEP_MOTOR_INDEX smIndex,int32_t modifyCoordinate)
{
    return IPC_StepMotorModifyCoordinateFuncPtrArray[smIndex](modifyCoordinate);
}