/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 08:59:23
**FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotor.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "UserMemManager.h"

//步进电机变量以及配置文件
STEP_MOTOR_VAR* stepMotorVar[SYSTEM_STEP_MOTOR_COUNT] = {NULL};

//步进电机中断回调函数
void StepMotorCPLD_CallBackProcessWithStateMachine(STEP_MOTOR_INDEX motorIndex)
{
    //选择步进电机状态进行处理,状态机处理机制
    switch(stepMotorVar[motorIndex]->var.motorMainState)
    {
        case STEP_MOTOR_MAIN_STATE_STOP:
            break;
        case STEP_MOTOR_MAIN_STATE_RESET:
            //复位状态
            CpldStepMotorResetProc(motorIndex);
            break;
        case STEP_MOTOR_MAIN_STATE_RUN_STEPS:
            //运行指定步数状态
            CpldStepMotorRunStepsProc(motorIndex);
            break;
        case STEP_MOTOR_MAIN_STATE_RUN_COORDINATE:
            //运行到指定坐标状态
            CpldStepMotorRunCoordinate(motorIndex);
            break;
        case STEP_MOTOR_MAIN_STATE_RUN_ALWAYS:
            //持续运转状态
            CpldStepMotorRunAlways(motorIndex);
            break;
        case STEP_MOTOR_MAIN_STATE_DACC:
            //减速运行状态
            CpldStepMotorRunDacc(motorIndex);
            break;
        default:
            //不支持的状态
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //未知原因
            stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}


//步进电机参数变量的初始化
void StepMotorVarInit(void)
{
    uint8_t i = 0;
    //内存申请
    for(i = 0; i < SYSTEM_STEP_MOTOR_COUNT; i++)
    {
        do
        {
            //申请内存
            stepMotorVar[i] = UserMemMalloc(SRAM_IS62,sizeof(STEP_MOTOR_VAR)/sizeof(uint8_t));
            //申请失败,报错
            if(stepMotorVar[i] == NULL)
            {
                CoreDelayMs(1000);
            }
        }while(stepMotorVar[i] == NULL);
    }
    //加载电机配置,12个分别加载
    StepMotorParamLoad(STEP_MOTOR_PARAM1_ADDR,&(stepMotorVar[STEP_MOTOR1]->attri),0);
    StepMotorParamLoad(STEP_MOTOR_PARAM2_ADDR,&(stepMotorVar[STEP_MOTOR2]->attri),1);
    StepMotorParamLoad(STEP_MOTOR_PARAM3_ADDR,&(stepMotorVar[STEP_MOTOR3]->attri),2);
    StepMotorParamLoad(STEP_MOTOR_PARAM4_ADDR,&(stepMotorVar[STEP_MOTOR4]->attri),3);
    StepMotorParamLoad(STEP_MOTOR_PARAM5_ADDR,&(stepMotorVar[STEP_MOTOR5]->attri),4);
    StepMotorParamLoad(STEP_MOTOR_PARAM6_ADDR,&(stepMotorVar[STEP_MOTOR6]->attri),5);
    StepMotorParamLoad(STEP_MOTOR_PARAM7_ADDR,&(stepMotorVar[STEP_MOTOR7]->attri),6);
    StepMotorParamLoad(STEP_MOTOR_PARAM8_ADDR,&(stepMotorVar[STEP_MOTOR8]->attri),7);
    StepMotorParamLoad(STEP_MOTOR_PARAM9_ADDR,&(stepMotorVar[STEP_MOTOR9]->attri),8);
    StepMotorParamLoad(STEP_MOTOR_PARAM10_ADDR,&(stepMotorVar[STEP_MOTOR10]->attri),9);
    StepMotorParamLoad(STEP_MOTOR_PARAM11_ADDR,&(stepMotorVar[STEP_MOTOR11]->attri),10);
    StepMotorParamLoad(STEP_MOTOR_PARAM12_ADDR,&(stepMotorVar[STEP_MOTOR12]->attri),11);
    //初始化12个电机状态
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR1]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR2]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR3]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR4]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR5]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR6]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR7]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR8]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR9]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR10]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR11]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR12]->var));
    return;
}

//将步进电机错误代码转换为系统错误代码
LH_ERR StepMotorConvert2LH_ErrCode(STEP_MOTOR_SUB_STATE state)
{
    switch(state)
    {
        case STEP_MOTOR_STOP_SUB_NORMAL:
            return LH_ERR_NONE;
            
        case STEP_MOTOR_STOP_SUB_EMERGENCY_CMD:
            return LH_ERR_SERVICE_SM_EMERGENCY_CMD;
            
        case STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL:
            return LH_ERR_SERVICE_SM_EMERGENCY_SIGNAL;
            
        case STEP_MOTOR_STOP_SUB_DACC1:
            return LH_ERR_SERVICE_SM_UTIL_STOP1;
            
        case STEP_MOTOR_STOP_SUB_DACC2:
            return LH_ERR_SERVICE_SM_UTIL_STOP2;
            
        case STEP_MOTOR_STOP_SUB_TIMEOUT:
            return LH_ERR_SERVICE_SM_TIMEOUT;
            
        case STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT:
            return LH_ERR_SERVICE_SM_RESET_STEP_LIMIT;
            
        default:
            return LH_ERR_NONE;
    }
}

//指定步进电机立即停止
LH_ERR StepMotorStopImmediately(STEP_MOTOR_INDEX motorIndex)
{
    //步进电机状态切换为急停
    stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(motorIndex);
    return LH_ERR_NONE;
}



//指定步进电机减速停止,这个指令只会响应RunAlways状态
LH_ERR StepMotorStopDeceleration(STEP_MOTOR_INDEX motorIndex)
{
    //只有在持续运行状态,且没到减速状态可以调用这个减速停止
    if(stepMotorVar[motorIndex]->var.motorMainState == STEP_MOTOR_MAIN_STATE_RUN_ALWAYS)
    {
        if(stepMotorVar[motorIndex]->var.motorSubState != STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE)
        {
            if(stepMotorVar[motorIndex]->var.currentTableMaxLength == 1)
            {
                //匀速运动,步进电机状态切换为急停
                stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
                //子状态急停
                stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
                //步进电机写入急停
                BoardCPLD_StepMotorEmergencyStop(motorIndex);
            }
            else
            {
                //步数表索引切入对称点
                stepMotorVar[motorIndex]->var.currentTableIndex = 
                    stepMotorVar[motorIndex]->var.currentTableMaxLength - stepMotorVar[motorIndex]->var.currentTableIndex - 1;
                //状态切入减速段
                stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE;
                //运行到下一步
                BoardCPLD_WriteStepMotorRunNextStep(motorIndex);
            }
            
        }
    }
    else
    {
        //步进电机状态切换为急停
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //子状态急停
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        //步进电机写入急停
        BoardCPLD_StepMotorEmergencyStop(motorIndex);
    }
    //等待运行完成
    while(stepMotorVar[motorIndex]->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //等待步进电机状态机停止,进行延时
        CoreDelayMinTick();
    }
    return LH_ERR_NONE;
}



//指定步进电机急停
LH_ERR StepMotorStopEmergency(STEP_MOTOR_INDEX motorIndex)
{
    //步进电机状态切换为急停
    stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_CMD;
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(motorIndex);
    return LH_ERR_NONE;
}



















