/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 09:00:19
**FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorRunAlways.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "UserMemManager.h"

/**************************************电机持续运行函数申明****************************************/
//持续运行的加速段处理
static void StepMotorProcessRunAlwaysSub_ACCELERATE(STEP_MOTOR_INDEX motorIndex);
//持续运行的持续段处理
static void StepMotorProcessRunAlwaysSub_HOLDING(STEP_MOTOR_INDEX motorIndex);
//持续运行的减速段处理
static void StepMotorProcessRunAlwaysSub_DECELERATE(STEP_MOTOR_INDEX motorIndex);
/***********************************************************************************************/


//持续运行
void CpldStepMotorRunAlways(STEP_MOTOR_INDEX motorIndex)
{
    switch(stepMotorVar[motorIndex]->var.motorSubState)
    {
        case STEP_MOTOR_RUN_ALWAYS_SUB_ACCELERATE:
            //步进电机持续运转的加速过程
            StepMotorProcessRunAlwaysSub_ACCELERATE(motorIndex);
            break;
        case STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING:
            //已经运转到最高速,开始保持运转
            StepMotorProcessRunAlwaysSub_HOLDING(motorIndex);
            break;
        case STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE:
            //切换到减速运转
            StepMotorProcessRunAlwaysSub_DECELERATE(motorIndex);
            break;
        default:
            //不识别的状态,直接停掉
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorIndex]->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}


/*****************************************************系统持续运转相关函数申明******************************************************/
//持续运行的加速段处理
static void StepMotorProcessRunAlwaysSub_ACCELERATE(STEP_MOTOR_INDEX motorIndex)
{
    uint32_t currentDataIndex = stepMotorVar[motorIndex]->var.currentTableIndex;
    //只有一段说明是匀速过程
    if(stepMotorVar[motorIndex]->var.currentTableMaxLength == 1)
    {
        //匀速运动,直接切换到匀速段
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_ALWAYS;
        stepMotorVar[motorIndex]->var.motorSubState =  STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING;
        //发送一次单次最大脉冲数
        BoardCPLD_WriteStepMotorSingle(motorIndex,//电机序号
                                            CPLD_STEP_MOTOR_EN_ENABLE,//使能
                                            stepMotorVar[motorIndex]->var.currentDir,//方向
                                            CPLD_STEP_MOTOR_STOP_OFF,//不急停
                                            stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[0],//速度
                                            stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax//脉冲数
                                            );
    }
    else
    {
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
            //当定位表进入最大行程的一半的时候,说明已经到了最大速度位置,此时,进入保持段
            if(stepMotorVar[motorIndex]->var.currentTableIndex == ((stepMotorVar[motorIndex]->var.currentTableMaxLength)/2))
            {
                stepMotorVar[motorIndex]->var.currentTableIndex -= 1;
                //设置走位状态
                stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_ALWAYS;
                stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING;
            }
        }
    }
}

//持续运行的持续段处理
static void StepMotorProcessRunAlwaysSub_HOLDING(STEP_MOTOR_INDEX motorIndex)
{
    uint32_t currentDataIndex = stepMotorVar[motorIndex]->var.currentTableIndex;
    //只有一段说明是匀速过程
    if(stepMotorVar[motorIndex]->var.currentTableMaxLength == 1)
    {
        //发送一次单次最大脉冲数
        BoardCPLD_WriteStepMotorSingle(motorIndex,//电机序号
                                        CPLD_STEP_MOTOR_EN_ENABLE,//使能
                                        stepMotorVar[motorIndex]->var.currentDir,//方向
                                        CPLD_STEP_MOTOR_STOP_OFF,//不急停
                                        stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[0],//速度
                                        stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax//脉冲数
                                        );
    }
    else
    {
        //保持运转,每次发最大脉冲数,且速度都是最大速度
        BoardCPLD_WriteStepMotorSingle(motorIndex,//电机序号
                                        CPLD_STEP_MOTOR_EN_ENABLE,//使能
                                        stepMotorVar[motorIndex]->var.currentDir,//方向
                                        CPLD_STEP_MOTOR_STOP_OFF,//不急停
                                        stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[currentDataIndex],//速度
                                        stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax//脉冲数
                                        );
    }
}

//持续运行的减速段处理
static void StepMotorProcessRunAlwaysSub_DECELERATE(STEP_MOTOR_INDEX motorIndex)
{
    //减速运转
    uint32_t currentDataIndex = stepMotorVar[motorIndex]->var.currentTableIndex;
    //只有一段说明是匀速过程
    if(stepMotorVar[motorIndex]->var.currentTableMaxLength == 1)
    {
        //直接停止
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    }
    else
    {
        //检查当前序号,必须要切换到转折点之后
        if(stepMotorVar[motorIndex]->var.currentTableIndex < (stepMotorVar[motorIndex]->var.currentTableMaxLength/2))
        {
            stepMotorVar[motorIndex]->var.currentTableIndex = 
                stepMotorVar[motorIndex]->var.currentTableMaxLength - stepMotorVar[motorIndex]->var.currentTableIndex -1;
        }
        else
        {
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
    }
}

/*********************************************************************************************************************************/

//指定步进电机持续运转
LH_ERR StepMotorRunAlways(StepMotorRunAlwaysCmd* runAlwaysConfigPtr)
{
    OS_ERR err;
    LH_ERR lhErrCode = LH_ERR_NONE;
    STEP_MOTOR_INDEX motorIndex = runAlwaysConfigPtr->motorIndex;
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    lhErrCode = StepMotorUtilCheckEmergencyCmdState(stepMotorVar[motorIndex]);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //相关状态变量初始化到准备走位
    StepMotorStateInitForRunAlways(&(stepMotorVar[motorIndex]->var));
    //开始计算步进电机运行曲线
    StepMotorTableCalcWithRunAlways(motorIndex,runAlwaysConfigPtr->selectConfig,
                                    runAlwaysConfigPtr->speedRatioMode,runAlwaysConfigPtr->speedRatio);
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVar[motorIndex]->var.stepMotorStartRunningTimeMs = OSTimeGet(&err);
    //设置走位状态
    stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_ALWAYS;
    stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_ACCELERATE;
    //持续运转的方向是电机正方向
    stepMotorVar[motorIndex]->var.currentDir = runAlwaysConfigPtr->dir;
    //发出正式的走位脉冲
    StepMotorUtilSendStartPluse(motorIndex,stepMotorVar[motorIndex]);
    return LH_ERR_NONE;
}



