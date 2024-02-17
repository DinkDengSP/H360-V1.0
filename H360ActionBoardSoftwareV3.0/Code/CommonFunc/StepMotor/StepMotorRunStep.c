/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 09:00:30
**FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorRunStep.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "UserMemManager.h"
#include "StepMotorAlarm.h"

/************************************电机走步数函数申明*******************************************/
static void StepMotorProcessRunStepsSub_NORMAL(STEP_MOTOR_INDEX motorIndex);
/***********************************************************************************************/

//走步数
void CpldStepMotorRunStepsProc(STEP_MOTOR_INDEX motorIndex)
{
    switch(stepMotorVar[motorIndex]->var.motorSubState)
    {
        case STEP_MOTOR_RUN_STEPS_SUB_NORMAL:
            //步进电机正常走步数
            StepMotorProcessRunStepsSub_NORMAL(motorIndex);
            break;
        default:
            //不识别的状态,直接停掉
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorIndex]->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}


/********************************************************步进电机正常走步数的状态机实现*******************************************/
static void StepMotorProcessRunStepsSub_NORMAL(STEP_MOTOR_INDEX motorIndex)
{
    uint32_t currentDataIndex = stepMotorVar[motorIndex]->var.currentTableIndex;
    //检查步数表中是否还有未发送的数据
    if(currentDataIndex >= stepMotorVar[motorIndex]->var.currentTableMaxLength)
    {
        //当前坐标已经到达最大位置,全部的数据都发送完成了
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        stepMotorVar[motorIndex]->var.motorSubState =  STEP_MOTOR_STOP_SUB_NORMAL;
    }
    else
    {
        //数据没有发送完成,继续发送数据
        if(stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[currentDataIndex] > stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax)
        {
            //还在正常运行过程
            //等待发送的数据大于电机配置的最大单次发送脉冲数,发送数据
            BoardCPLD_WriteStepMotorSingle(motorIndex,//电机序号
                                            CPLD_STEP_MOTOR_EN_ENABLE,//使能
                                            stepMotorVar[motorIndex]->var.currentDir,//方向
                                            CPLD_STEP_MOTOR_STOP_OFF,//不急停
                                            stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[currentDataIndex],//速度
                                            stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax//脉冲数
                                            );
            //内部的步数表存储的步数减少
            stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[currentDataIndex] -= stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax;
        }
        else
        {
            //还在正常运行过程
            //等待发送的数据小于电机配置的最大单次发送脉冲数,那么可以一次发送完成
            BoardCPLD_WriteStepMotorSingle(motorIndex,//电机序号
                                            CPLD_STEP_MOTOR_EN_ENABLE,//使能
                                            stepMotorVar[motorIndex]->var.currentDir,//方向
                                            CPLD_STEP_MOTOR_STOP_OFF,//不急停
                                            stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[currentDataIndex],//速度
                                            stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[currentDataIndex]//脉冲数
                                            );
            //步数表减少
            stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[currentDataIndex] = 0;
            //定位往前加1
            stepMotorVar[motorIndex]->var.currentTableIndex += 1;
        }
    }
}

/*********************************************************************************************************************************/
//指定步进电机运行指定步数
LH_ERR StepMotorRunSpecialSteps(StepMotorRunStepsCmd* runStepConfigPtr)
{
    OS_ERR err;
    LH_ERR lhErrCode = LH_ERR_NONE;
    STEP_MOTOR_INDEX motorIndex = runStepConfigPtr->motorIndex;
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    lhErrCode = StepMotorUtilCheckEmergencyCmdState(stepMotorVar[motorIndex]);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //如果步数为0,不需要运行,直接返回
    if(runStepConfigPtr->specialSteps == 0)
    {
        //电机停下来,等待状态切换
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        //写入一次急停
        BoardCPLD_StepMotorEmergencyStop(motorIndex);
        return LH_ERR_NONE;
    }
    //检查步进电目标坐标是否在限制坐标范围内
    lhErrCode = StepMotorUtilCheckTargetCoordinateLimit(motorIndex,stepMotorVar[motorIndex],stepMotorVar[motorIndex]->var.currentCoordinate + runStepConfigPtr->specialSteps);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //相关状态变量初始化到准备走位
    StepMotorStateInitForRunSteps(&(stepMotorVar[motorIndex]->var));
    //检测急停引脚状态和初始化急停相关状态
    lhErrCode = StepMotorUtilCheckAndSetEmergencyPinState(motorIndex,stepMotorVar[motorIndex]);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //检测辅助急停1引脚状态和初始化相关变量标志
    lhErrCode = StepMotorUtilCheckAndSetUtilStop1PinState(motorIndex,stepMotorVar[motorIndex],runStepConfigPtr->utilStop1Enable);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //检测辅助急停2引脚状态和初始化相关变量标志
    lhErrCode = StepMotorUtilCheckAndSetUtilStop2PinState(motorIndex,stepMotorVar[motorIndex],runStepConfigPtr->utilStop2Enable);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //开始计算步进电机运行曲线
    StepMotorTableCalcWithStep(runStepConfigPtr->specialSteps,motorIndex,runStepConfigPtr->selectConfig,runStepConfigPtr->speedRatioMode,runStepConfigPtr->speedRatio);
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVar[motorIndex]->var.stepMotorStartRunningTimeMs = OSTimeGet(&err);
    //设置走位状态
    stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_STEPS;
    stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RUN_STEPS_SUB_NORMAL;
    //计算电机方向
    StepMotorUtilCalcDirWithTargetCoordinate(motorIndex,stepMotorVar[motorIndex],stepMotorVar[motorIndex]->var.currentCoordinate + runStepConfigPtr->specialSteps);
    //发出正式的走位脉冲
    StepMotorUtilSendStartPluse(motorIndex,stepMotorVar[motorIndex]);
    //等待电机停止运行
    StepMotorUtilWaitMotorStop(motorIndex,stepMotorVar[motorIndex],runStepConfigPtr->timeOutSet);
    //运行停止,更新步进电机当前的坐标
    StepMotorUtilUpdateCurrentCoordinate(motorIndex,stepMotorVar[motorIndex],runStepConfigPtr->specialSteps);
    //要根据是否打开了相关的中断和辅助中断来关闭响应的辅助中断和中断
    StepMotorUtilUnregisterIntPort(motorIndex,stepMotorVar[motorIndex]);
    //将数据转换为结果代码之后发送出去
    lhErrCode = StepMotorConvert2LH_ErrCode(stepMotorVar[motorIndex]->var.motorSubState);
    return lhErrCode;
}











