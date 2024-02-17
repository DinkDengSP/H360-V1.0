/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 09:00:50
**FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorUtil.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotorUtil.h"
#include "StepMotorAlarm.h"

//初始化电机状态
void StepMotorStateInit(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //状态设定
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->currentDir = BOARD_CPLD_MOTOR_DIR_CW;//当前方向
    motorStatePtr->currentCoordinate = 0;//当前坐标
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 0;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}

//电机状态初始化,针对复位
void StepMotorStateInitForReset(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->currentDir = BOARD_CPLD_MOTOR_DIR_CW;//当前方向
    motorStatePtr->currentCoordinate = 0;//当前坐标
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}

//电机状态初始化,针对走位
void StepMotorStateInitForRunSteps(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}

//电机状态初始化,针对电机持续运转
void StepMotorStateInitForRunAlways(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}

void StepMotorStateInitForRunCoordinate(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}

void StepMotorStateInitForReturnZero(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}



//检查电机走指定步数的参数是否合格
LH_ERR StepMotorCheckRunStepsVar(StepMotorRunStepsCmd* runStepPtr)
{
    if(runStepPtr->motorIndex > STEP_MOTOR12)
    {
        //电机序号不对
        return LH_ERR_SERVICE_SM_PARAM_STEPS;
    }
    if(runStepPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return LH_ERR_SERVICE_SM_PARAM_STEPS;
    }
    if(runStepPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return LH_ERR_SERVICE_SM_PARAM_STEPS;
    }
    if(runStepPtr->speedRatio > 100)
    {
        //速度比例错误
        return LH_ERR_SERVICE_SM_PARAM_STEPS;
    }
    if(runStepPtr->speedRatio == 0)
    {
        //速度比例错误
        return LH_ERR_SERVICE_SM_PARAM_STEPS;
    }
    if(runStepPtr->utilStop1Enable != 0)
    {
        //纠正一下可能出错的数据
        runStepPtr->utilStop1Enable = 1;
    }
    if(runStepPtr->utilStop2Enable != 0)
    {
        //纠正一下可能出错的数据
        runStepPtr->utilStop2Enable = 1;
    }
    //两个辅助急停不能同时被使能
    if((runStepPtr->utilStop1Enable != 0)&&(runStepPtr->utilStop2Enable != 0))
    {
        return LH_ERR_SERVICE_SM_PARAM_STEPS;
    }
    if(runStepPtr->timeOutSet == 0)
    {
        //超时时间不能为0
        return LH_ERR_SERVICE_SM_PARAM_STEPS;
    }
    //校验通过
    return LH_ERR_NONE;
}

//检查电机走到指定坐标的参数是否合格
LH_ERR StepMotorCheckRun2CoordationVar(StepMotorRun2CoordationCmd* runCoordationPtr)
{
    if(runCoordationPtr->motorIndex > STEP_MOTOR12)
    {
        //电机序号不对
        return LH_ERR_SERVICE_SM_PARAM_COORDATION;
    }
    if(runCoordationPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return LH_ERR_SERVICE_SM_PARAM_COORDATION;
    }
    if(runCoordationPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return LH_ERR_SERVICE_SM_PARAM_COORDATION;
    }
    if(runCoordationPtr->speedRatio > 100)
    {
        //速度比例错误
        return LH_ERR_SERVICE_SM_PARAM_COORDATION;
    }
    if(runCoordationPtr->speedRatio == 0)
    {
        //速度比例错误
        return LH_ERR_SERVICE_SM_PARAM_COORDATION;
    }
    if(runCoordationPtr->utilStop1Enable != 0)
    {
        //纠正一下可能出错的数据
        runCoordationPtr->utilStop1Enable = 1;
    }
    if(runCoordationPtr->utilStop2Enable != 0)
    {
        //纠正一下可能出错的数据
        runCoordationPtr->utilStop2Enable = 1;
    }
    //两个辅助急停不能同时被使能
    if((runCoordationPtr->utilStop1Enable != 0)&&(runCoordationPtr->utilStop2Enable != 0))
    {
        return LH_ERR_SERVICE_SM_PARAM_COORDATION;
    }
    if(runCoordationPtr->timeOutSet == 0)
    {
        //超时时间不能为0
        return LH_ERR_SERVICE_SM_PARAM_COORDATION;
    }
    //校验通过
    return LH_ERR_NONE;
}

//检查电机回零指令的参数是否合格
LH_ERR StepMotorCheckReturnZeroVar(StepMotorReturnZeroCmd* returnZeroPtr)
{
    if(returnZeroPtr->motorIndex > STEP_MOTOR12)
    {
        //电机序号不对
        return LH_ERR_SERVICE_SM_PARAM_RETURN_ZERO;
    }
    if(returnZeroPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return LH_ERR_SERVICE_SM_PARAM_RETURN_ZERO;
    }
    if(returnZeroPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return LH_ERR_SERVICE_SM_PARAM_RETURN_ZERO;
    }
    if(returnZeroPtr->speedRatio > 100)
    {
        //速度比例错误
        return LH_ERR_SERVICE_SM_PARAM_RETURN_ZERO;
    }
    if(returnZeroPtr->speedRatio == 0)
    {
        //速度比例错误
        return LH_ERR_SERVICE_SM_PARAM_RETURN_ZERO;
    }
    if(returnZeroPtr->utilStop1Enable != 0)
    {
        //纠正一下可能出错的数据
        returnZeroPtr->utilStop1Enable = 1;
    }
    if(returnZeroPtr->utilStop2Enable != 0)
    {
        //纠正一下可能出错的数据
        returnZeroPtr->utilStop2Enable = 1;
    }
    //两个辅助急停不能同时被使能
    if((returnZeroPtr->utilStop1Enable != 0)&&(returnZeroPtr->utilStop2Enable != 0))
    {
        return LH_ERR_SERVICE_SM_PARAM_RETURN_ZERO;
    }
    if(returnZeroPtr->timeOutSet == 0)
    {
        //超时时间不能为0
        return LH_ERR_SERVICE_SM_PARAM_RETURN_ZERO;
    }
    //校验通过
    return LH_ERR_NONE;
}

//检查电机持续运转的参数是否合格
LH_ERR StepMotorCheckRunAlways(StepMotorRunAlwaysCmd* runAlwaysPtr)
{
    if(runAlwaysPtr->motorIndex > STEP_MOTOR12)
    {
        //电机序号不对
        return LH_ERR_SERVICE_SM_PARAM_RUN_ALWAYS;
    }
    if(runAlwaysPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return LH_ERR_SERVICE_SM_PARAM_RUN_ALWAYS;
    }
    if(runAlwaysPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return LH_ERR_SERVICE_SM_PARAM_RUN_ALWAYS;
    }
    if(runAlwaysPtr->speedRatio > 100)
    {
        //速度比例错误
        return LH_ERR_SERVICE_SM_PARAM_RUN_ALWAYS;
    }
    if(runAlwaysPtr->speedRatio == 0)
    {
        //速度比例错误
        return LH_ERR_SERVICE_SM_PARAM_RUN_ALWAYS;
    }
    //校验通过
    return LH_ERR_NONE;
}

//检查步进电机急停状态,如果是指令急停,那么报错
LH_ERR StepMotorUtilCheckEmergencyCmdState(STEP_MOTOR_VAR* motorVarPtr)
{
    //检测电机状态,如果电机状态为指令急停,那么报错,指令急停必须复位取消
    if((motorVarPtr->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)&&
        (motorVarPtr->var.motorSubState == STEP_MOTOR_STOP_SUB_EMERGENCY_CMD))
    {
        return LH_ERR_SERVICE_SM_EMERGENCY_CMD;
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//检查步进电机电机的目标坐标是否超过限制
LH_ERR StepMotorUtilCheckTargetCoordinateLimit(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate)
{
    //检测电机行程限制,如果行程限制为0,代表不限制行程,否则,补偿有行程限制
    if(motorVarPtr->attri.baseConfig.positiveMaxSteps != 0)
    {
        //检查目的坐标是否在运行行程里面
        if(targetCoordinate > (motorVarPtr->attri.baseConfig.positiveMaxSteps))
        {
            //电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop(motorIndex);
            return LH_ERR_SERVICE_SM_CORDINATE_MAX;
        }
    }
    //负方向的行程限制
    if(motorVarPtr->attri.baseConfig.reverseMaxSteps != 0)
    {
        //检查目的坐标是否在运行行程里面
        if(targetCoordinate < (motorVarPtr->attri.baseConfig.reverseMaxSteps))
        {
            //电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop(motorIndex);
            return LH_ERR_SERVICE_SM_CORDINATE_MIN;
        }
    }
    return LH_ERR_NONE;
}

//检查和设置步进电机急停引脚状态
LH_ERR StepMotorUtilCheckAndSetEmergencyPinState(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr)
{
    LH_ERR lhErrCode = LH_ERR_NONE;
    //如果系统支持急停,那么查看是否有急停端口,并且尝试将急停设置为中断
    if((motorVarPtr->attri.baseConfig.emergencyStopPinIndex)< BOARD_IN_PIN_COUNT)
    {
        //配置了系统急停,那么检测此时急停IO状态,触发状态,直接报错
        if(motorVarPtr->attri.baseConfig.emergencyStopPinValidLevel ==
                     BoardInReadValue(motorVarPtr->attri.baseConfig.emergencyStopPinIndex))
        {
            //电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop(motorIndex);
            return LH_ERR_SERVICE_SM_EMERGENCY_SIGNAL;
        }
        else
        {
            //系统急停没有触发,那么将尝试将急停设置为中断
            if(motorVarPtr->attri.baseConfig.emergencyStopPinValidLevel == Bit_RESET)
            {
                //低电平有效,设置为下降沿中断
                lhErrCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.emergencyStopPinIndex,
                                          EXTI_Trigger_Falling,
                                          StepMotorEmergencyStopByIntSingalFuncPtrArray[motorIndex]
                                          );
            }
            else
            {
                //高电平有效,设置为上升沿中断
                lhErrCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.emergencyStopPinIndex,
                                          EXTI_Trigger_Rising,
                                          StepMotorEmergencyStopByIntSingalFuncPtrArray[motorIndex]
                                          );
            }
            //查看注册错误代码
            if(lhErrCode == LH_ERR_NONE)
            {
                //如果错误代码为无错误,说明已经设置好了中断,运行时不需要循环检测
                motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_EnableRegister;
            }
            else
            {
                //注册失败,需要运行时循环检测
                motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_EnableUnregister;
            }
        }
    }
    else
    {
        //急停信号没有使能
        motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    return LH_ERR_NONE;
}

//检查和设置步进电机辅助急停1状态
LH_ERR StepMotorUtilCheckAndSetUtilStop1PinState(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable)
{
    LH_ERR lhErrCode = LH_ERR_NONE;
    //如果系统支持辅助急停端口,那么查看是否设置有辅助急停端口,并尝试将急停设置为中断
    //辅助急停1被使能且辅助急停1配置为系统输入
    if((enable == 1)&&(motorVarPtr->attri.baseConfig.utilStopPin1Index) < BOARD_IN_PIN_COUNT)
    {
        //首先查看辅助急停状态
        if(motorVarPtr->attri.baseConfig.utilStopPin1ValidLevel ==
                     BoardInReadValue(motorVarPtr->attri.baseConfig.utilStopPin1Index))
        {
            //辅助急停此时已经被触发,电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC1;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop(motorIndex);
            return LH_ERR_SERVICE_SM_UTIL_STOP1;
        }
        else
        {
            //辅助急停未触发,尝试将辅助急停注册到中断函数中
            if(motorVarPtr->attri.baseConfig.utilStopPin1ValidLevel == Bit_RESET)
            {
                //低电平有效,设置为下降沿中断
                lhErrCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index,
                                          EXTI_Trigger_Falling,
                                          StepMotorDaccStopByUtilInt1SingalFuncPtrArray[motorIndex]
                                          );
            }
            else
            {
                //高电平有效,设置为上升沿中断
                lhErrCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index,
                                          EXTI_Trigger_Rising,
                                          StepMotorDaccStopByUtilInt1SingalFuncPtrArray[motorIndex]
                                          );
            }
            //查看注册错误代码
            if(lhErrCode == LH_ERR_NONE)
            {
                //如果错误代码为无错误,说明已经设置好了中断,运行时不需要循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_EnableRegister;
            }
            else
            {
                //注册失败,需要运行时循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_EnableUnregister;
            }
        }
    }
    else
    {
        //辅助急停信号没使能
        motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    return LH_ERR_NONE;
}

//检查和设置步进电机辅助急停2状态
LH_ERR StepMotorUtilCheckAndSetUtilStop2PinState(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable)
{
    LH_ERR lhErrCode = LH_ERR_NONE;
    //辅助急停2被使能且辅助急停配置为系统输入
    if((enable == 1)&&(motorVarPtr->attri.baseConfig.utilStopPin2Index) < BOARD_IN_PIN_COUNT)
    {
        //先检测辅助急停是否已经触发,如果已经出发,函数直接返回
        if(motorVarPtr->attri.baseConfig.utilStopPin2ValidLevel ==
                     BoardInReadValue(motorVarPtr->attri.baseConfig.utilStopPin2Index))
        {
            //辅助急停此时已经被触发,电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC2;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop(motorIndex);
            return LH_ERR_SERVICE_SM_UTIL_STOP2;
        }
        else
        {
            //辅助急停未触发,尝试将辅助急停注册到中断函数中
            if(motorVarPtr->attri.baseConfig.utilStopPin2ValidLevel == Bit_RESET)
            {
                //低电平有效,设置为下降沿中断
                lhErrCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index,
                                          EXTI_Trigger_Falling,
                                          StepMotorDaccStopByUtilInt2SingalFuncPtrArray[motorIndex]
                                          );
            }
            else
            {
                //高电平有效,设置为上升沿中断
                lhErrCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index,
                                          EXTI_Trigger_Rising,
                                          StepMotorDaccStopByUtilInt2SingalFuncPtrArray[motorIndex]
                                          );
            }
            //查看注册错误代码
            if(lhErrCode == LH_ERR_NONE)
            {
                //如果错误代码为无错误,说明已经设置好了中断,运行时不需要循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_EnableRegister;
            }
            else
            {
                //注册失败,需要运行时循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_EnableUnregister;
            }
        }
    }
    else
    {
        //辅助急停信号没使能
        motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    return LH_ERR_NONE;
}

//根据本次电机目的坐标决定方向
void StepMotorUtilCalcDirWithTargetCoordinate(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate)
{
    //然后设置当前方向
    if(targetCoordinate > motorVarPtr->var.currentCoordinate)
    {
        motorVarPtr->var.currentDir = motorVarPtr->attri.baseConfig.dir;
    }
    else
    {
        if(motorVarPtr->attri.baseConfig.dir == BOARD_CPLD_MOTOR_DIR_CCW)
        {
            motorVarPtr->var.currentDir = BOARD_CPLD_MOTOR_DIR_CW;
        }
        else
        {
            motorVarPtr->var.currentDir = BOARD_CPLD_MOTOR_DIR_CCW;
        }
    }
}

//发送启动脉冲,启动运行
void StepMotorUtilSendStartPluse(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr)
{
    //首先将当前序号设置为0
    motorVarPtr->var.currentTableIndex = 0;
    //发出第一步
    if(motorVarPtr->var.stepMotorStepTableDirect[0] > motorVarPtr->attri.baseConfig.singlePluseMax)
    {
        //大于单次发送最大步数,一次发满
        BoardCPLD_WriteStepMotorSingle(motorIndex,
                                        CPLD_STEP_MOTOR_EN_ENABLE,
                                        motorVarPtr->var.currentDir,
                                        CPLD_STEP_MOTOR_STOP_OFF,
                                        motorVarPtr->var.stepMotorSpeedTableDirect[0],
                                        motorVarPtr->attri.baseConfig.singlePluseMax
                                        );
        //步数表实时减小
        motorVarPtr->var.stepMotorStepTableDirect[0] -= motorVarPtr->attri.baseConfig.singlePluseMax;
    }
    else
    {
        //小于最大脉冲数,一次发完
        BoardCPLD_WriteStepMotorSingle(motorIndex,
                                        CPLD_STEP_MOTOR_EN_ENABLE,
                                        motorVarPtr->var.currentDir,
                                        CPLD_STEP_MOTOR_STOP_OFF,
                                        motorVarPtr->var.stepMotorSpeedTableDirect[0],
                                        motorVarPtr->var.stepMotorStepTableDirect[0]
                                        );
        //一次发完,步数表归零
        motorVarPtr->var.stepMotorStepTableDirect[0] = 0;
        //步数表往前加一步
        motorVarPtr->var.currentTableIndex += 1;
    }
}

//发送减速第一步
void StepMotorUtilSendDaccStartPluse(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr)
{
    //首先将当前序号设置为0
    motorVarPtr->var.currentDaccTableIndex = 0;
    //发出第一步
    if(motorVarPtr->var.stepMotorDaccStepTableDirect[0] > motorVarPtr->attri.baseConfig.singlePluseMax)
    {
        //大于单次发送最大步数,一次发满
        BoardCPLD_WriteStepMotorSingle(motorIndex,
                                        CPLD_STEP_MOTOR_EN_ENABLE,
                                        motorVarPtr->var.currentDir,
                                        CPLD_STEP_MOTOR_STOP_OFF,
                                        motorVarPtr->var.stepMotorDaccSpeedTableDirect[0],
                                        motorVarPtr->attri.baseConfig.singlePluseMax
                                        );
        //步数表实时减小
        motorVarPtr->var.stepMotorDaccStepTableDirect[0] -= motorVarPtr->attri.baseConfig.singlePluseMax;
    }
    else
    {
        //小于最大脉冲数,一次发完
        BoardCPLD_WriteStepMotorSingle(motorIndex,
                                        CPLD_STEP_MOTOR_EN_ENABLE,
                                        motorVarPtr->var.currentDir,
                                        CPLD_STEP_MOTOR_STOP_OFF,
                                        motorVarPtr->var.stepMotorDaccSpeedTableDirect[0],
                                        motorVarPtr->var.stepMotorDaccStepTableDirect[0]
                                        );
        //一次发完,步数表归零
        motorVarPtr->var.stepMotorDaccStepTableDirect[0] = 0;
        //步数表往前加一步
        motorVarPtr->var.currentDaccTableIndex += 1;
    }
}

//步进电机解除全部的中断注册,运行完成后调用
void StepMotorUtilUnregisterIntPort(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr)
{
    //急停
    if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(motorVarPtr->attri.baseConfig.emergencyStopPinIndex);
    }
    motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;

    //辅助急停1
    if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index);
    }
    motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;

    //辅助急停2
    if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index);
    }
    motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
}

//等待步进电机停止运转
void StepMotorUtilWaitMotorStop(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,uint32_t timeOutSet)
{
    OS_ERR err;
    //等待步进电机处于停止状态,在等待过程中,如果前面设置的两个中断不支持,那么就要实时监测IO口状态,并进行处理
    while(motorVarPtr->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //检测超时,自动停止
        if(motorVarPtr->var.stepMotorRunningTimeMsCount > timeOutSet)
        {
            //实时监测的时候应该关闭中断
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop(motorIndex);
            //结束循环
            continue;
        }
        //如果急停处于使能未注册的状态,那么需要检测电平,并进行处理
        if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableUnregister)
        {
            //读取的状态满足急停
            if(motorVarPtr->attri.baseConfig.emergencyStopPinValidLevel == BoardInReadValue(motorVarPtr->attri.baseConfig.emergencyStopPinIndex))
            {
                //电机停下来,等待状态切换,如果辅助急停和急停同时出发,那么急停的优先级更高,减速过程会终止
                motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
                motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
                //写入一次急停
                BoardCPLD_StepMotorEmergencyStop(motorIndex);
                //同时将监控状态去掉
                motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;

                //关闭辅助急停1
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                //关闭辅助急停2
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                //处理完成,本次循环跳出,不进行下次的其余模块检查,防止其他急停触发导致系统状态混乱
                continue;
            }
        }
        //如果辅助急停1处于使能未注册状态,需要实时监测
        if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableUnregister)
        {
            //读取的状态满足辅助急停
            if(motorVarPtr->attri.baseConfig.utilStopPin1ValidLevel == BoardInReadValue(motorVarPtr->attri.baseConfig.utilStopPin1Index))
            {
                //注意,因为两个辅助急停不能同时生效,所以基本不用考虑两个辅助急停同时触发的情况,否则,还要考虑
                //两个辅助急停同时触发,状态变量之间的互斥关系
                //切换到减速过程
                motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
                //切换到减速起始状态,要计算减速曲线的那种
                motorVarPtr->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
                //发送立即进入下一步运动step
                BoardCPLD_WriteStepMotorRunNextStep(motorIndex);
                //去除监控状态
                motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                //处理完成,本次循环跳出,不进行下次的其余模块检查,防止其他急停触发导致系统状态混乱

                //关闭急停
                if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.emergencyStopPinIndex);
                }
                if(motorVarPtr->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                //关闭辅助急停2
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                continue;
            }
        }
        //辅助急停2的状态的处理
        if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableUnregister)
        {
            //读取的状态满足辅助急停
            if(motorVarPtr->attri.baseConfig.utilStopPin2ValidLevel == BoardInReadValue(motorVarPtr->attri.baseConfig.utilStopPin2Index))
            {
                motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
                //切换到减速起始状态,要计算减速曲线的那种
                motorVarPtr->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
                //发送立即进入下一步运动step
                BoardCPLD_WriteStepMotorRunNextStep(motorIndex);
                //去除监控状态
                motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                //关闭急停
                if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.emergencyStopPinIndex);
                }
                if(motorVarPtr->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                //关闭辅助急停1
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }
                //处理完成,本次循环跳出,不进行下次的其余模块检查,防止其他急停触发导致系统状态混乱
                continue;
            }
        }
        //等待步进电机状态机停止,进行延时
        CoreDelayMinTick();
        //更新运行时间
        motorVarPtr->var.stepMotorRunningTimeMsCount = OSTimeGet(&err) - motorVarPtr->var.stepMotorStartRunningTimeMs;
    }
}

//更新步进电机当前的坐标
void StepMotorUtilUpdateCurrentCoordinate(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,int32_t exceptSteps)
{
    //还未发出的步数统计
    uint32_t stepNotSendOut = 0;
    uint32_t indexUtil = 0;
    //统计未发送步数
    for(indexUtil = 0; indexUtil < motorVarPtr->var.currentTableMaxLength;indexUtil++)
    {
        stepNotSendOut += motorVarPtr->var.stepMotorStepTableDirect[indexUtil];
    }
    //急停或者正常停止
    if(exceptSteps > 0)
    {
        //正常坐标变化
        motorVarPtr->var.currentCoordinate += exceptSteps;
        //减去未发出的步数
        motorVarPtr->var.currentCoordinate -= stepNotSendOut;
        if((motorVarPtr->var.motorSubState == STEP_MOTOR_STOP_SUB_DACC1)||
            (motorVarPtr->var.motorSubState == STEP_MOTOR_STOP_SUB_DACC2))
        {
            //要在原来的基础上加上一个停止速度段
            motorVarPtr->var.currentCoordinate += motorVarPtr->attri.baseConfig.dacc2StopSteps;
        }
    }
    else
    {
        //正常坐标变化
        motorVarPtr->var.currentCoordinate += exceptSteps;
        //加上未发出的步数
        motorVarPtr->var.currentCoordinate += stepNotSendOut;
        if((motorVarPtr->var.motorSubState == STEP_MOTOR_STOP_SUB_DACC1)||
            (motorVarPtr->var.motorSubState == STEP_MOTOR_STOP_SUB_DACC2))
        {
            //要在原来的基础上减去一个停止速度段
            motorVarPtr->var.currentCoordinate -= motorVarPtr->attri.baseConfig.dacc2StopSteps;
        }
    }
}
