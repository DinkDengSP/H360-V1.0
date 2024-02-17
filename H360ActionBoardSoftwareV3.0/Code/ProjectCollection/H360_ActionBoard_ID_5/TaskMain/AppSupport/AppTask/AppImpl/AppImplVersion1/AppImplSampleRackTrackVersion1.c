/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:32:38
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-13 16:39:21
**FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppTask\AppImpl\AppImplSampleRackTrack.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplSampleRackTrackVersion1.h"

//电机步数累计误差
static float errStepSum = 0;
//当前试管序号
static RACK_TUBE_INDEX currentTubeIndex = RACK_TUBE_INDEX_1;


//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplSampleRackTrackConvertErrorCode(uint16_t flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_SAMPLE_RACK_TRACK;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD5_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}




//传送带修正指定个脉冲
static LH_ERR AppImplSampleRackTrackCorrectTargetPulse(SAMPLE_RACK_TRACK_CORRECT_DIR correctDir,uint32_t correctPulse)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //辅助参数
    int32_t paramUtil = 0;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    //目标修正脉冲数
    uint32_t targetCorrectPulse = 0;
    //实际修正脉冲数
    uint32_t actualCorrectPulse = 0;
    //从定时器获取到的脉冲数
    uint32_t readPulse = 0;
    //修正次数
    uint8_t correctCount = 0;
    targetCorrectPulse = correctPulse;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    do
    {
        //计算出要修正的步数
        paramUtil = (int32_t)(correctPulse/PULSE_SAMPLE_RACK_TRACK_ONE_STEP);
        if(correctDir == SAMPLE_RACK_TRACK_CORRECT_DIR_NEGATIVE)
        {
            paramUtil = 0-paramUtil;
        }
        //清除当前脉冲数
        BoardEncoderClear(BOARD_ENCODER_1);
        runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
        runStepCmd.specialSteps = paramUtil;
        runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_RUN_TUBE1_2;
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;      
        }
        //获取脉冲数
        readPulse = BoardEncoderReadCurrentValue(BOARD_ENCODER_1);
        actualCorrectPulse += readPulse;
        //实际脉冲数大于目标脉冲数,且超出阈值,报错
        if((actualCorrectPulse>targetCorrectPulse) && ((actualCorrectPulse - targetCorrectPulse) > PULSE_CORRECT_THRESHOLD_SAMPLE_RACK_TRACK))
        {  
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_PULSE_OVER_TARGET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //实际脉冲数小于脉冲数,且超出阈值,需要修正
        if((actualCorrectPulse < targetCorrectPulse) && ((targetCorrectPulse - actualCorrectPulse) > PULSE_CORRECT_THRESHOLD_SAMPLE_RACK_TRACK))
        {
            //稍微延时一会
            CoreDelayMs(50);
            correctCount++;
            //更新需要修正的脉冲数
            correctPulse = targetCorrectPulse - actualCorrectPulse;
        }
        else
        {
            break;
        }
    }while(correctCount <= MAX_CORRECT_COUNT_SAMPLE_RACK_TRACK);
    //修正超次数,报错
    if(correctCount > MAX_CORRECT_COUNT_SAMPLE_RACK_TRACK)
    {
        errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_CORRECT_MAX,errorCode);
        AppShowErrMsg(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//运行指定步数
static LH_ERR AppImplSampleRackTrackRunTargetStep(int32_t stepsToRun)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    //辅助参数
    int32_t paramUtil = 0;
    //目标脉冲数,一定是一个正值
    uint32_t targetPulse = 0;
    //实际读到的脉冲数
    uint32_t actualPulse = 0;
    //计算目标脉冲数
    paramUtil = stepsToRun < 0 ? 0-stepsToRun:stepsToRun;
    targetPulse = (uint32_t)(paramUtil*PULSE_SAMPLE_RACK_TRACK_ONE_STEP);
    //清除当前脉冲数
    BoardEncoderClear(BOARD_ENCODER_1);
    //电机运行指定步数
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
    runStepCmd.specialSteps = stepsToRun;
    runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_RUN_TUBE1_2;
    errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
        AppShowErrMsg(errorCode);
        return errorCode;      
    }
    //获取脉冲数
    actualPulse = BoardEncoderReadCurrentValue(BOARD_ENCODER_1);
    //脉冲数相等,直接过  
    if(actualPulse == targetPulse)
    {
        return LH_ERR_NONE;
    }
    //实际脉冲少一目标脉冲一半,不修正
    if(actualPulse < (targetPulse/2))
    {
        errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_PULSE_LESS_HALF_TAEGET,errorCode);
        AppShowErrMsg(errorCode);
        return errorCode;
    }
    //实际脉冲大于理论脉冲,且超出阈值,报错
    if((actualPulse > targetPulse) && ((actualPulse-targetPulse) > PULSE_CORRECT_THRESHOLD_SAMPLE_RACK_TRACK))  
    {
        errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_PULSE_OVER_TARGET,errorCode);
        AppShowErrMsg(errorCode);
        return errorCode;
    }
    //实际脉冲少于理论脉冲,且超出阈值,需要修正
    if((actualPulse < targetPulse) && ((targetPulse-actualPulse) > PULSE_CORRECT_THRESHOLD_SAMPLE_RACK_TRACK))
    {
        if(stepsToRun<0)
        {
            //反向修正
            return AppImplSampleRackTrackCorrectTargetPulse(SAMPLE_RACK_TRACK_CORRECT_DIR_NEGATIVE,targetPulse-actualPulse);
        }
        else
        {
            //正向修正
            return AppImplSampleRackTrackCorrectTargetPulse(SAMPLE_RACK_TRACK_CORRECT_DIR_POSITIVE,targetPulse-actualPulse);
        }
        
    }
    //脉冲误差在阈值范围内,不修正
    return LH_ERR_NONE;
}




//加样传送带复位
LH_ERR AppImplSampleRackTrackResetVersion1(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //编码器零位修正
    int32_t paramUtil = 0;
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,APP_PARAM_SUB_INDEX_SAMPLE_RACK_TRACK_ENCODER_ZERO_CORRECT,&paramUtil);
    //复位指令结构体
        StepMotorResetCmd resetCmd;
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //第一步是试管架传送带复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //清除当前脉冲数
        BoardEncoderClear(BOARD_ENCODER_1);
    //初始化架号为RACK_TUBE_INDEX_1
        currentTubeIndex = RACK_TUBE_INDEX_1;
    //清除误差
        errStepSum = 0;
    return LH_ERR_NONE;
}

//加样传送带移动到指定试管位
LH_ERR AppImplSampleRackTrackMove2TubeVersion1(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex,RACK_TUBE_INDEX* currentTube)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机理论要运行步数
    float stepToRunTheory = 0;
    //电机实际运行步数
    int32_t stepToRunActual = 0;
    //检查电机是否被急停
        errorCode = IPC_StepMotorCheckEmergencyStop(SAMPLE_RACK_TRACK_STEP_MOTOR_ID);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //识别目标位置,
    if(rackPos == SAMPLE_RACK_TRACK_POS_CURRENT)
    {
        //此次理论要走的步数
        stepToRunTheory = (STEPS_SAMPLE_RACK_TRACK_ENCODER_ONE_TUBE*(tubeIndex - currentTubeIndex));
        //此次实际能走的步数
        stepToRunActual = (int32_t)stepToRunTheory;
        //误差累计
        errStepSum += stepToRunTheory-stepToRunActual;
        //看误差是否大于1
        while(errStepSum >= 1)
        {
            if(stepToRunActual >= 0)
            {
                stepToRunActual +=1;
            }
            else
            {
                stepToRunActual -=1;
            }
            errStepSum -= 1;
        }
        //看误差是否小于-1
        while(errStepSum <= -1)
        {
            if(stepToRunActual >= 0)
            {
                stepToRunActual +=1;
            }
            else
            {
                stepToRunActual -=1;
            }
            errStepSum += 1;
        }
        //
        errorCode = AppImplSampleRackTrackRunTargetStep(stepToRunActual);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        currentTubeIndex = tubeIndex;
    }
    else
    {
        //下一个试管架,首先要把当前还剩下多少走完
        int32_t stepToRun = 0;
        stepToRun = STEPS_SAMPLE_RACK_TRACK_ENCODER_ONE_TRACK;
        //理论步数
        stepToRunTheory = (tubeIndex-currentTubeIndex)*STEPS_SAMPLE_RACK_TRACK_ENCODER_ONE_TUBE;
        //此次实际能走的步数
        stepToRunActual = (int32_t)stepToRunTheory;
        //误差累计
        errStepSum += stepToRunTheory-stepToRunActual;
        //看误差是否大于1
        while(errStepSum >= 1)
        {
            if(stepToRunActual >= 0)
            {
                stepToRunActual +=1;
            }
            else
            {
                stepToRunActual -=1;
            }
            errStepSum -= 1;
        }
        //看误差是否小于-1
        while(errStepSum <= -1)
        {
            if(stepToRunActual >= 0)
            {
                stepToRunActual +=1;
            }
            else
            {
                stepToRunActual -=1;
            }
            errStepSum += 1;
        }
        //最终的步数
        stepToRun +=stepToRunActual;
        //走位
        errorCode = AppImplSampleRackTrackRunTargetStep(stepToRun);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        currentTubeIndex = tubeIndex;
    }
    *currentTube = currentTubeIndex;
    return LH_ERR_NONE;
}

//加样传送带回退一整个试管架
LH_ERR AppImplSampleRackTrackBackFullSampleRackVersion1(RACK_TUBE_INDEX* currentTube)
{
    //反向运转20个齿序号
    LH_ERR errorCode = LH_ERR_NONE;
    //检查电机是否被急停
        errorCode = IPC_StepMotorCheckEmergencyStop(SAMPLE_RACK_TRACK_STEP_MOTOR_ID);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //回退整个试管架
        errorCode = AppImplSampleRackTrackRunTargetStep(0-STEPS_SAMPLE_RACK_TRACK_ENCODER_ONE_TRACK);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前坐标
        *currentTube = currentTubeIndex;
    return errorCode;
}

//加样传送带进行一次码盘齿差修正,正反向走没有误差
LH_ERR AppImplSampleRackTrackEncoderCorrectVersion1(uint8_t encoderCorrectCount)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //检查电机是否被急停
        errorCode = IPC_StepMotorCheckEmergencyStop(SAMPLE_RACK_TRACK_STEP_MOTOR_ID);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //反向走指定修正齿数
        errorCode = AppImplSampleRackTrackRunTargetStep(0-encoderCorrectCount*STEPS_SAMPLE_RACK_TRACK_ENCODER_ONE_GEAR);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        CoreDelayMs(100);
    //正向将齿数走回来
        errorCode = AppImplSampleRackTrackRunTargetStep(encoderCorrectCount*STEPS_SAMPLE_RACK_TRACK_ENCODER_ONE_GEAR);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //执行完成
        return LH_ERR_NONE;
}


//步进电机调试配置表
static const MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[] = {
    {SAMPLE_RACK_TRACK_STEP_MOTOR_ID,0,0},//样本架传送带旋转
};

//指定步进电机复位
LH_ERR AppImplSampleRackTrackStepMotorResetVersion1(INDEX_MOTOR_SAMPLE_RACK_TRACK index)
{
    return AppImplSampleRackTrackResetVersion1();
}

//指定步进电机走指定步数
LH_ERR AppImplSampleRackTrackStepMotorRunStepsVersion1(INDEX_MOTOR_SAMPLE_RACK_TRACK index,int32_t steps,int32_t* posAfterRun)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = moduleStepMotorResetRunConfigArray[index].motorIndex;
        runStepCmd.specialSteps = steps;
        runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_DEBUG_VERSION1;//默认曲线0作为调试曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取结果
        errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,posAfterRun);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
