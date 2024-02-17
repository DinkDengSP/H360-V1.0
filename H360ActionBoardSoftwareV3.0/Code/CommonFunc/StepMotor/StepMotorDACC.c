/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 08:59:42
**FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorDACC.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "UserMemManager.h"
#include "StepMotorAlarm.h"

/**********************************************电机辅助急停减速函数声明********************************/
//辅助急停1减速起始计算减速曲线
static void StepMotorProcessDaccSub_START1(STEP_MOTOR_INDEX motorIndex);
//辅助急停2减速起始计算减速曲线
static void StepMotorProcessDaccSub_START2(STEP_MOTOR_INDEX motorIndex);
//辅助急停1正常减速曲线发送
static void StepMotorProcessDaccSub_NORMAL1(STEP_MOTOR_INDEX motorIndex);
//辅助急停2正常减速曲线发送
static void StepMotorProcessDaccSub_NORMAL2(STEP_MOTOR_INDEX motorIndex);
/***********************************************************************************************/

//减速运行
void CpldStepMotorRunDacc(STEP_MOTOR_INDEX motorIndex)
{
    //子状态处理
    switch(stepMotorVar[motorIndex]->var.motorSubState)
    {
        case STEP_MOTOR_DACC1_SUB_START:
            //刚发生辅助急停1的中断,现在开始计算减速曲线
            StepMotorProcessDaccSub_START1(motorIndex);
            break;
        case STEP_MOTOR_DACC2_SUB_START:
            //刚发生辅助急停2的中断,现在开始计算减速曲线
            StepMotorProcessDaccSub_START2(motorIndex);
            break;
        case STEP_MOTOR_DACC1_SUB_NORMAL:
            //减速曲线1运行中
            StepMotorProcessDaccSub_NORMAL1(motorIndex);
            break;
        case STEP_MOTOR_DACC2_SUB_NORMAL:
            //减速曲线2运行中
            StepMotorProcessDaccSub_NORMAL2(motorIndex);
            break;
        default:
            //不识别的状态,直接停掉
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorIndex]->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}



/*****************************************************系统减速急停相关状态机处理函数*************************************************/
//发生辅助急停,现在要在这里计算减速曲线
static void StepMotorProcessDaccSub_START1(STEP_MOTOR_INDEX motorIndex)
{
    if(stepMotorVar[motorIndex]->attri.baseConfig.dacc2StopSteps == 0)
    {
        //减速步数为0,切换到停止状态
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //指示减速停止
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC1;
    }
    else
    {
        //计算步进电机减速表
        StepMotorTableCalcDaccTable(motorIndex);
        //发出减速表第一步
        StepMotorUtilSendDaccStartPluse(motorIndex,stepMotorVar[motorIndex]);
        //切换到减速表发送过程
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
        //指示减速停止
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_DACC1_SUB_NORMAL;
    }
}

//辅助急停2发生,现在在此处计算减速曲线
static void StepMotorProcessDaccSub_START2(STEP_MOTOR_INDEX motorIndex)
{
    if(stepMotorVar[motorIndex]->attri.baseConfig.dacc2StopSteps == 0)
    {
        //减速步数为0,切换到停止状态
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //指示减速停止
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC2;
    }
    else
    {
        //计算步进电机减速表
        StepMotorTableCalcDaccTable(motorIndex);
        //发出减速表第一步
        StepMotorUtilSendDaccStartPluse(motorIndex,stepMotorVar[motorIndex]);
        //切换到减速表发送过程
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
        //指示减速停止
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_DACC2_SUB_NORMAL;
    }
}

//步进电机减速步数发送
static void StepMotorProcessDaccSub_NORMAL1(STEP_MOTOR_INDEX motorIndex)
{
    uint32_t tabDaccIndex = stepMotorVar[motorIndex]->var.currentDaccTableIndex;
    if(tabDaccIndex >= stepMotorVar[motorIndex]->var.currentDaccTableMaxLength)
    {
        //脉冲发送完成,切换到停止状态
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //减速停止
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC1;
    }
    else
    {
        if(stepMotorVar[motorIndex]->var.stepMotorDaccStepTableDirect[tabDaccIndex] > stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax)
        {
            //大于单次发送最大步数,一次发满
            BoardCPLD_WriteStepMotorSingle(motorIndex,
                                            CPLD_STEP_MOTOR_EN_ENABLE,
                                            stepMotorVar[motorIndex]->var.currentDir,
                                            CPLD_STEP_MOTOR_STOP_OFF,
                                            stepMotorVar[motorIndex]->var.stepMotorDaccSpeedTableDirect[tabDaccIndex],
                                            stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax
                                            );
            //步数表实时减小
            stepMotorVar[motorIndex]->var.stepMotorDaccStepTableDirect[tabDaccIndex] -= stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax;
        }
        else
        {
            //小于最大脉冲数,一次发完
            BoardCPLD_WriteStepMotorSingle(motorIndex,
                                            CPLD_STEP_MOTOR_EN_ENABLE,
                                            stepMotorVar[motorIndex]->var.currentDir,
                                            CPLD_STEP_MOTOR_STOP_OFF,
                                            stepMotorVar[motorIndex]->var.stepMotorDaccSpeedTableDirect[tabDaccIndex],
                                            stepMotorVar[motorIndex]->var.stepMotorDaccStepTableDirect[tabDaccIndex]
                                            );
            //一次发完,步数表归零
            stepMotorVar[motorIndex]->var.stepMotorDaccStepTableDirect[tabDaccIndex] = 0;
            //步数表往前加一步
            stepMotorVar[motorIndex]->var.currentDaccTableIndex += 1;
        }
    }
}

static void StepMotorProcessDaccSub_NORMAL2(STEP_MOTOR_INDEX motorIndex)
{
    uint32_t tabDaccIndex = stepMotorVar[motorIndex]->var.currentDaccTableIndex;
    if(tabDaccIndex >= stepMotorVar[motorIndex]->var.currentDaccTableMaxLength)
    {
        //脉冲发送完成,切换到停止状态
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //减速停止
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC2;
    }
    else
    {
        if(stepMotorVar[motorIndex]->var.stepMotorDaccStepTableDirect[tabDaccIndex] > stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax)
        {
            //大于单次发送最大步数,一次发满
            BoardCPLD_WriteStepMotorSingle(motorIndex,
                                            CPLD_STEP_MOTOR_EN_ENABLE,
                                            stepMotorVar[motorIndex]->var.currentDir,
                                            CPLD_STEP_MOTOR_STOP_OFF,
                                            stepMotorVar[motorIndex]->var.stepMotorDaccSpeedTableDirect[tabDaccIndex],
                                            stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax
                                            );
            //步数表实时减小
            stepMotorVar[motorIndex]->var.stepMotorDaccStepTableDirect[tabDaccIndex] -= stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax;
        }
        else
        {
            //小于最大脉冲数,一次发完
            BoardCPLD_WriteStepMotorSingle(motorIndex,
                                            CPLD_STEP_MOTOR_EN_ENABLE,
                                            stepMotorVar[motorIndex]->var.currentDir,
                                            CPLD_STEP_MOTOR_STOP_OFF,
                                            stepMotorVar[motorIndex]->var.stepMotorDaccSpeedTableDirect[tabDaccIndex],
                                            stepMotorVar[motorIndex]->var.stepMotorDaccStepTableDirect[tabDaccIndex]
                                            );
            //一次发完,步数表归零
            stepMotorVar[motorIndex]->var.stepMotorDaccStepTableDirect[tabDaccIndex] = 0;
            //步数表往前加一步
            stepMotorVar[motorIndex]->var.currentDaccTableIndex += 1;
        }
    }
}

/*********************************************************************************************************************************/




