/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 09:00:13
**FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorReturnZero.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "UserMemManager.h"
#include "StepMotorAlarm.h"

//启动时走到坐标零位
LH_ERR StepMotorReturnZeroWithStart2CoordinateZero(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    OS_ERR err;
    STEP_MOTOR_INDEX motorIndex = returnZeroConfigPtr->motorIndex;
    //此时原点未触发,要计算步数,走向原点
    StepMotorTableCalcWithReturnZero(motorIndex,returnZeroConfigPtr->selectConfig,returnZeroConfigPtr->speedRatioMode,returnZeroConfigPtr->speedRatio);
    //设置状态,回零走到坐标零位
    stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_COORDINATE;
    stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RUN_COORDINATE_SUB_NORMAL;
    //计算方向
    StepMotorUtilCalcDirWithTargetCoordinate(motorIndex,stepMotorVar[motorIndex],0);
    //发出第一步
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVar[motorIndex]->var.stepMotorStartRunningTimeMs = OSTimeGet(&err);
    //以计算速度发送一次脉冲
    StepMotorUtilSendStartPluse(motorIndex,stepMotorVar[motorIndex]);
    //等待运动完成
    while(stepMotorVar[motorIndex]->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //等待步进电机状态机停止
        CoreDelayMinTick();
        //检测原点状态,原点触发时,立即切换减速
        if(stepMotorVar[motorIndex]->attri.baseConfig.resetPinValidLevel == BoardInReadValue(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex))
        {
            //设置状态
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            //写入一次切换下一步
            BoardCPLD_StepMotorEmergencyStop(motorIndex);
        }
        //检测运行时间
        stepMotorVar[motorIndex]->var.stepMotorRunningTimeMsCount = OSTimeGet(&err) - stepMotorVar[motorIndex]->var.stepMotorStartRunningTimeMs;
        //检测超时,自动停止
        if(stepMotorVar[motorIndex]->var.stepMotorRunningTimeMsCount > returnZeroConfigPtr->timeOutSet)
        {
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
        }
    }
    if(stepMotorVar[motorIndex]->var.motorSubState == STEP_MOTOR_STOP_SUB_TIMEOUT)
    {
        //运行超时
        return LH_ERR_SERVICE_SM_TIMEOUT;
    }
    else
    {
        //运行过程中遇到零点立刻停止,正常脉冲发完也立刻停止
        return LH_ERR_NONE;
    }
}

//回零的复位过程
LH_ERR StepMotorReturnZeroStageReset(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    StepMotorResetCmd resetCmd;
    resetCmd.correctionCoordinate = 0;
    resetCmd.motorIndex = returnZeroConfigPtr->motorIndex;
    resetCmd.timeOutSet = returnZeroConfigPtr->timeOutSet;
    return StepMotorReset(&resetCmd);
}

//步进电机找零点
LH_ERR StepMotorReturnZeroStageFindZeroSensor(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    OS_ERR err;
    STEP_MOTOR_INDEX motorIndex = returnZeroConfigPtr->motorIndex;
    //已经找到原点
    if(stepMotorVar[motorIndex]->attri.baseConfig.resetPinValidLevel == BoardInReadValue(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex))
    {
        return LH_ERR_NONE;
    }
    else
    {
        //找原点,设置状态是找原点
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST;
        //电机当前方向应该是运行时反方向
        stepMotorVar[motorIndex]->var.currentDir = (BOARD_CPLD_MOTOR_DIR)(!(stepMotorVar[motorIndex]->attri.baseConfig.dir));
        //增加1步的复位脉冲数
        stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse = 1;
        //记录启动时间
        stepMotorVar[motorIndex]->var.stepMotorStartRunningTimeMs = OSTimeGet(&err);
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,
                                        CPLD_STEP_MOTOR_STOP_OFF,stepMotorVar[motorIndex]->attri.baseConfig.resetSpeed,1);
        //等待找到原点或者报错切换状态
        while(stepMotorVar[motorIndex]->var.motorSubState == STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST)
        {
            CoreDelayMinTick();
            //检测运行时间
            stepMotorVar[motorIndex]->var.stepMotorRunningTimeMsCount = OSTimeGet(&err) - stepMotorVar[motorIndex]->var.stepMotorStartRunningTimeMs;
            //检测超时,自动停止
            if(stepMotorVar[motorIndex]->var.stepMotorRunningTimeMsCount > returnZeroConfigPtr->timeOutSet)
            {
                stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
                stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
            }
        }
        //记录状态
        STEP_MOTOR_SUB_STATE localSubState = stepMotorVar[motorIndex]->var.motorSubState;
        //设置状态
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        //停止
        BoardCPLD_StepMotorEmergencyStop(motorIndex);
        //赶紧停下来
        if(localSubState == STEP_MOTOR_STOP_SUB_TIMEOUT)
        {
            //行程超限制
            return LH_ERR_SERVICE_SM_TIMEOUT;
        }
        else if(localSubState == STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT)
        {
            return LH_ERR_SERVICE_SM_RESET_STEP_LIMIT;
        }
        return LH_ERR_NONE;
    }
}

//走坐标
LH_ERR StepMotorReturnZeroCorrection(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    StepMotorRun2CoordationCmd run2CoordinateCmdPtr;
    run2CoordinateCmdPtr.motorIndex = returnZeroConfigPtr->motorIndex;
    run2CoordinateCmdPtr.targetCoordinate = returnZeroConfigPtr->correctionCoordinate;
    run2CoordinateCmdPtr.selectConfig = returnZeroConfigPtr->selectConfig;
    run2CoordinateCmdPtr.speedRatioMode = returnZeroConfigPtr->speedRatioMode;
    run2CoordinateCmdPtr.speedRatio = returnZeroConfigPtr->speedRatio;
    run2CoordinateCmdPtr.utilStop1Enable = returnZeroConfigPtr->utilStop1Enable;
    run2CoordinateCmdPtr.utilStop2Enable = returnZeroConfigPtr->utilStop2Enable;
    run2CoordinateCmdPtr.timeOutSet = returnZeroConfigPtr->timeOutSet;
    return StepMotorRun2SpecialCoordinates(&run2CoordinateCmdPtr);
}


//指定步进电机回零
LH_ERR StepMotorReturnZeroWithSensor(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    LH_ERR lhErrCode = LH_ERR_NONE;
    STEP_MOTOR_INDEX motorIndex = returnZeroConfigPtr->motorIndex;
    //状态检测
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    lhErrCode = StepMotorUtilCheckEmergencyCmdState(stepMotorVar[motorIndex]);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //检查步进电目标坐标是否在限制坐标范围内
    lhErrCode = StepMotorUtilCheckTargetCoordinateLimit(motorIndex,stepMotorVar[motorIndex],returnZeroConfigPtr->correctionCoordinate);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //初始化电机状态,为回零
    StepMotorStateInitForReturnZero(&(stepMotorVar[motorIndex]->var));
    //是否有原点,没有原点,直接开始修正过程
    if(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex < BOARD_IN_PIN_COUNT)
    {
        if(stepMotorVar[motorIndex]->attri.baseConfig.resetPinValidLevel == BoardInReadValue(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex))
        {
            //调用复位指令
            lhErrCode = StepMotorReturnZeroStageReset(returnZeroConfigPtr);
            //结果未报错,可以开始修正
            if(lhErrCode != LH_ERR_NONE)
            {
                return lhErrCode;
            }
        }
        else
        {
            //运行到零点
            lhErrCode = StepMotorReturnZeroWithStart2CoordinateZero(returnZeroConfigPtr);
            if(lhErrCode != LH_ERR_NONE)
            {
                return lhErrCode;
            }
            CoreDelayMinTick();
            //复位过程
            lhErrCode = StepMotorReturnZeroStageReset(returnZeroConfigPtr);
            //结果未报错,可以开始修正
            if(lhErrCode != LH_ERR_NONE)
            {
                return lhErrCode;
            }
        }
    }
    //设置当前坐标为0
    stepMotorVar[motorIndex]->var.currentCoordinate = 0;
    //修正
    return StepMotorReturnZeroCorrection(returnZeroConfigPtr);
}


//指令步进电机回零,但是不进行复位动作,该指令只要用于泵回零,先找到零点,然后关阀然后复位的过程
//防止产生真空负压
LH_ERR StepMotorReturnZeroWithSensorRemoveResetStage(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    LH_ERR lhErrCode = LH_ERR_NONE;
    STEP_MOTOR_INDEX motorIndex = returnZeroConfigPtr->motorIndex;
    //状态检测
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    lhErrCode = StepMotorUtilCheckEmergencyCmdState(stepMotorVar[motorIndex]);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //检查步进电目标坐标是否在限制坐标范围内
    lhErrCode = StepMotorUtilCheckTargetCoordinateLimit(motorIndex,stepMotorVar[motorIndex],returnZeroConfigPtr->correctionCoordinate);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //初始化电机状态,为回零
    StepMotorStateInitForReturnZero(&(stepMotorVar[motorIndex]->var));
    //是否有原点,没有原点,直接开始修正过程
    if(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex < BOARD_IN_PIN_COUNT)
    {
        if(stepMotorVar[motorIndex]->attri.baseConfig.resetPinValidLevel == BoardInReadValue(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex))
        {
            //当前已经在零点,直接开始修正
            asm("nop");
        }
        else
        {
            //不在零点,运行到零点
            lhErrCode = StepMotorReturnZeroWithStart2CoordinateZero(returnZeroConfigPtr);
            if(lhErrCode != LH_ERR_NONE)
            {
                return lhErrCode;
            }
            //此时不一定回到零点了,有可能回到了坐标零点但是还是没找到传感器,所以需要找传感器
            lhErrCode = StepMotorReturnZeroStageFindZeroSensor(returnZeroConfigPtr);
            if(lhErrCode != LH_ERR_NONE)
            {
                return lhErrCode;
            }
        }
    }
    //设置当前坐标为0
    stepMotorVar[motorIndex]->var.currentCoordinate = 0;
    //无修正
    return LH_ERR_NONE;
}


