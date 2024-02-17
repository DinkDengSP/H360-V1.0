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
#include "AppImplSampleRackTrackVersion0.h"

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

//试管架传送带编码器事件计数
static uint32_t sampleRackTrackGearEncoderEventCount = 0;
//上一个脉冲的系统时钟
static OS_TICK lastEncoderTrigTimeMs = 0;
//试管架传送带码盘中断回调函数
static void SampleRackTrackExitEventCallBack()
{
    OS_ERR err;
    //当前系统时钟
    OS_TICK currentEncoderTrigTimeMs = 0;
    //获取当前脉冲时间
    currentEncoderTrigTimeMs = OSTimeGet(&err);
    if(sampleRackTrackGearEncoderEventCount == 0)
    {
        lastEncoderTrigTimeMs = currentEncoderTrigTimeMs;
        sampleRackTrackGearEncoderEventCount++;
    }
    else
    {
        //本次脉冲时间大于上次的脉冲时间,区分时间大于分辨时间
        if(currentEncoderTrigTimeMs - lastEncoderTrigTimeMs >= TIME_MS_INTERVAL_SAMPLE_RACK_TRACK_TRIG_ENCODER)
        {
            sampleRackTrackGearEncoderEventCount++;
            lastEncoderTrigTimeMs = currentEncoderTrigTimeMs;
        }
        else
        {
            //脉冲无法分辨,忽略掉
        }
    }
}

//当前试管序号
static RACK_TUBE_INDEX currentTubeIndex = RACK_TUBE_INDEX_1;


//正向运行到下一个码盘有效齿位
static LH_ERR AppImplSampleRackTrackRunNextGearPositive(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint32_t correntCount = 0;
    //确定有效脉冲
        BitAction validEncoder = VALID_LEVEL_INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN;
        int32_t paramUtil = 0;
    //获取当前码盘状态,确定接下来要收到多少脉冲
        if(validEncoder == BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
        {
            //此时处于触发状态,需要两次信号变化
            paramUtil = 2;
        }
        else
        {
            //非触发状态,只需要一次信号变化
            paramUtil = 1;
        }
    //清空计数,注册中断,正向中断注册
        sampleRackTrackGearEncoderEventCount = 0;
        BoardExtiRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN,EXTI_Trigger_Rising_Falling,SampleRackTrackExitEventCallBack);
    //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //正向微步运转,直到触发一次中断
        correntCount = 0;
        while(sampleRackTrackGearEncoderEventCount < paramUtil)
        {
            //填充参数
                runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
                runStepCmd.specialSteps = MICRO_STEP_SAMPLE_RACK_TRACK;
                runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_MICRO_STEP;//微步曲线
            //微步运转
                errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
                if(errorCode != LH_ERR_NONE)
                {
                    //解除中断
                    BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                    errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
                    AppShowErrMsg(errorCode);
                    return errorCode;      
                }
            //判定状态,超过修正次数还是没能获取到指定脉冲
                correntCount++;
                if(correntCount > MAX_TIMES_CORRECT_SAMPLE_RACK_TRACK)
                {
                    //解除中断
                    BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                    //修正超行程
                    errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_CORRECT_MAX,errorCode);
                    AppShowErrMsg(errorCode);
                    return errorCode;
                }
        }
    //解除中断
        BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
        correntCount = 0;
        do
        {
        //填充参数
            runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
            runStepCmd.specialSteps = SUPPLEMENT_STEP_SAMPLE_RACK_TRACK;
            runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_MICRO_STEP;//微步曲线
        //触发之后多走一步补充,防止信号在边缘检测异常
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        //到这里,说明已经触发到了边缘,检测电平讯号是否匹配
            if(validEncoder != BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
            {
                //如果信号无效,延时后再次修正
                CoreDelayMs(50);
                correntCount++;
            }
            else
            {
                break;
            }
        }while(correntCount < 3);
        if(correntCount >= 3)
        {
            errorCode |= (paramUtil<<8);
            errorCode |= sampleRackTrackGearEncoderEventCount;
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_STOP_ENCODER_LEVEL,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//正向运行指定个码盘齿数
static LH_ERR AppImplSampleRackTrackRunGearCountPositive(uint32_t encoderCountPositive)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint32_t correntCount = 0;
    int32_t stepsSampleRackTrackNeedRun = 0;
    uint8_t readEncoderPinCount = 0;
    uint8_t correctCount = 0;
    //解除中断
    BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
    //读取参数确定有效脉冲
        BitAction validEncoder = VALID_LEVEL_INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN;
    //首先要检查,码盘信号必须有效
        do
        {
            if(validEncoder != BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
            {
                //无效累加
                readEncoderPinCount++;
            }
            else
            {
                //有效跳出
                break;
            }
            if(readEncoderPinCount >= 3)
            {
                //启动的时候码盘信号就不对
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_START_ENCODER_LEVEL,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            } 
        }while(readEncoderPinCount < 3);
        
    //运行齿数为0,直接返回
        if(encoderCountPositive == 0)
        {
            return LH_ERR_NONE;
        }
    //注册中断
        sampleRackTrackGearEncoderEventCount = 0;
        BoardExtiRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN,EXTI_Trigger_Rising_Falling,SampleRackTrackExitEventCallBack);
    //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //先运行指定齿数,不能一次走到,否则无法进行进一步修正
        stepsSampleRackTrackNeedRun = ((encoderCountPositive*STEP_SAMPLE_RACK_TRACK_ENCODER_ONE_GEAR) - (STEP_SAMPLE_RACK_TRACK_ENCODER_ONE_GEAR));
        runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
        runStepCmd.specialSteps = stepsSampleRackTrackNeedRun;
        runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_NORMAL;//正常曲线
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            //解除中断
            BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //脉冲超过限制,细分不多或者码盘脏了
        if(sampleRackTrackGearEncoderEventCount >= (encoderCountPositive*2))
        {
            //解除中断
            BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_TRIG_MORE_THAN_EXCEPT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //理论脉冲不匹配
        if((sampleRackTrackGearEncoderEventCount+2) != (encoderCountPositive*2))
        {
            //查看脉冲是否超范围+-1
            if(sampleRackTrackGearEncoderEventCount < ((encoderCountPositive*2)-3) || sampleRackTrackGearEncoderEventCount > ((encoderCountPositive*2)-1))
            {
                //解除中断
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                errorCode |= ((encoderCountPositive*2-2)<<8);
                errorCode |= sampleRackTrackGearEncoderEventCount;
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_ENCODER_NOT_MATCH_POSITIVE,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //解除中断
            BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
            //读码盘信号
            if(BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN) == VALID_LEVEL_INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN)
            {
                if(sampleRackTrackGearEncoderEventCount%2 != 0)
                {
                    sampleRackTrackGearEncoderEventCount--;
                }
            }
            else
            {
                if(sampleRackTrackGearEncoderEventCount%2 == 0)
                {
                    sampleRackTrackGearEncoderEventCount++;
                }
            }
            //注册中断
            BoardExtiRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN,EXTI_Trigger_Rising_Falling,SampleRackTrackExitEventCallBack);
        }
    //修正并检测中断信号
        while(sampleRackTrackGearEncoderEventCount < (encoderCountPositive*2))
        {
            //微步运转
            runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
            runStepCmd.specialSteps = MICRO_STEP_SAMPLE_RACK_TRACK;
            runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_MICRO_STEP;//微步曲线
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            correntCount++;
            if(correntCount > MAX_TIMES_CORRECT_SAMPLE_RACK_TRACK)
            {
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                //修正超行程
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_CORRECT_MAX,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
    //解除中断
        BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
        correctCount = 0;

        do
        {
            //触发之后多走一步补充,防止信号在边缘检测异常
            runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
            runStepCmd.specialSteps = SUPPLEMENT_STEP_SAMPLE_RACK_TRACK;
            runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_MICRO_STEP;//微步曲线
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                //解除中断
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            if(validEncoder != BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
            {
                //如果信号无效,延时后再次修正
                CoreDelayMs(50);
                correctCount++;
            }
            else
            {
                break;
            }
        }while(correctCount < 3);
    
    //到这里,说明已经触发到了边缘,检测电平讯号是否匹配
        readEncoderPinCount = 0;
        do
        {
            if(validEncoder != BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
            {
                readEncoderPinCount++;
            }
            else
            {
                break;
            }
            if(readEncoderPinCount >=3)
            {
                //修正后电平讯号不匹配,出现异常
                errorCode |= ((encoderCountPositive*2)<<8);
                errorCode |= sampleRackTrackGearEncoderEventCount;
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_STOP_ENCODER_INVALID_POSITTVE,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            } 
        }while(readEncoderPinCount <3);
        
    //完成
        return LH_ERR_NONE;
}

//反向运行指定个码盘齿数
static LH_ERR AppImplSampleRackTrackRunGearCountNegative(uint32_t encoderCountNegative)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint32_t correntCount = 0;
    int32_t stepsSampleRackTrackNeedRun = 0;
    uint8_t readEncoderPinCount = 0;
    uint8_t correctCount = 0;
    //解除中断
        BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
    //读取参数确定有效脉冲
        BitAction validEncoder = VALID_LEVEL_INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN;
    //首先要检查,码盘信号必须有效    
        do
        {
            if(validEncoder != BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
            {
                //无效累加
                readEncoderPinCount++;
            }
            else
            {
                //有效跳出
                break;
            }
            //启动的时候码盘信号就不对
            if(readEncoderPinCount >= 3)
            {
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_START_ENCODER_LEVEL,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }    
        }while(readEncoderPinCount < 3);
    //运行齿数为0,直接返回
        if(encoderCountNegative == 0)
        {
            return LH_ERR_NONE;
        }
    //注册中断
        sampleRackTrackGearEncoderEventCount = 0;
        BoardExtiRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN,EXTI_Trigger_Rising_Falling,SampleRackTrackExitEventCallBack);
    //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //先运行指定齿数,不能一次走到,否则无法进行进一步修正
        stepsSampleRackTrackNeedRun = 0 - (((encoderCountNegative*STEP_SAMPLE_RACK_TRACK_ENCODER_ONE_GEAR) - (STEP_SAMPLE_RACK_TRACK_ENCODER_ONE_GEAR)));
        runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
        runStepCmd.specialSteps = stepsSampleRackTrackNeedRun;
        runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_NORMAL;//正常曲线
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            //解除中断
            BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //脉冲超过限制,细分不对或者码盘脏了
        if(sampleRackTrackGearEncoderEventCount >= ((encoderCountNegative*2)+1))
        {
            //解除中断
            BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_TRIG_MORE_THAN_EXCEPT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //理论脉冲不匹配
        if((sampleRackTrackGearEncoderEventCount+2) != (encoderCountNegative*2))
        {
            //查看脉冲是否超范围+-1
            if(sampleRackTrackGearEncoderEventCount < ((encoderCountNegative*2)-3) || sampleRackTrackGearEncoderEventCount > ((encoderCountNegative*2)-1))
            {
                //解除中断
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                errorCode |= ((encoderCountNegative*2-2)<<8);
                errorCode |= sampleRackTrackGearEncoderEventCount;
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_ENCODER_NOT_MATCH_NEGATIVE,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            
            //解除中断
            BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
            //读码盘信号
            if(BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN) == VALID_LEVEL_INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN)
            {
                if(sampleRackTrackGearEncoderEventCount%2 != 0)
                {
                    sampleRackTrackGearEncoderEventCount--;
                }
            }
            else
            {
                if(sampleRackTrackGearEncoderEventCount%2 == 0)
                {
                    sampleRackTrackGearEncoderEventCount++;
                }
            }
            //注册中断
            BoardExtiRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN,EXTI_Trigger_Rising_Falling,SampleRackTrackExitEventCallBack);
        }
    //修正并检测中断讯号
        while(sampleRackTrackGearEncoderEventCount < ((encoderCountNegative*2)+1))
        {
            //微步运转
            runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
            runStepCmd.specialSteps = (0 - MICRO_STEP_SAMPLE_RACK_TRACK);
            runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_MICRO_STEP;//微步曲线
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            correntCount++;
            if(correntCount > MAX_TIMES_CORRECT_SAMPLE_RACK_TRACK)
            {
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                //修正超行程
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_CORRECT_MAX,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
    //解除中断
        BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
        correctCount = 0;
        do
        {
            //触发之后多走一步补充,防止信号在边缘检测异常
            runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
            runStepCmd.specialSteps = (0 - SUPPLEMENT_STEP_SAMPLE_RACK_TRACK);
            runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_MICRO_STEP;//微步曲线
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                //解除中断
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            if(validEncoder == BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
            {
                //如果信号有效,延时后再次修正
                CoreDelayMs(50);
                correctCount++;
            }
            else
            {
                break;
            }
        }while(correctCount < 3);
    
    //这个时候检查码盘光电信号,信号必须是无效,否则码盘信号异常
        readEncoderPinCount = 0;
        do
        {
            if(validEncoder == BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
            {
                //有效累加
                readEncoderPinCount++;
            }
            else
            {
                //无效跳出
                break;
            }
            if(readEncoderPinCount >= 3)
            {
                //解除中断
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                errorCode |= ((encoderCountNegative*2+1)<<8);
                errorCode |= sampleRackTrackGearEncoderEventCount;
                //修正后电平讯号不匹配,出现异常
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_ENCODER_FIND_INVALID_EDGE,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }while(readEncoderPinCount <3);
        
    //反向中断到位了,切换成正向
        sampleRackTrackGearEncoderEventCount = 0;
        BoardExtiRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN,EXTI_Trigger_Rising_Falling,SampleRackTrackExitEventCallBack);
    //微步运转直到获取一次脉冲
        correntCount = 0;
        while(sampleRackTrackGearEncoderEventCount < 1)
        {
            //微步运转
            runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
            runStepCmd.specialSteps = MICRO_STEP_SAMPLE_RACK_TRACK;
            runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_MICRO_STEP;//微步曲线
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            correntCount++;
            if(correntCount > MAX_TIMES_CORRECT_SAMPLE_RACK_TRACK)
            {
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                //修正超行程
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_CORRECT_MAX,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
        BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
        correctCount = 0;
        do
        {
            //触发之后多走一步补充,防止信号在边缘检测异常
            runStepCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;
            runStepCmd.specialSteps = SUPPLEMENT_STEP_SAMPLE_RACK_TRACK;
            runStepCmd.selectConfig = S_CURVE_SAMPLE_RACK_TRACK_MICRO_STEP;//微步 曲线
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                //解除中断
                BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            if(validEncoder != BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
            {
                //如果信号无效,延时后再次修正
                CoreDelayMs(50);
                correctCount++;
            }
            else
            {
                break;
            }
        }while(correctCount < 3);
    
    //到这里,说明已经触发到了边缘,检测电平讯号是否匹配,码盘必须是有效
        readEncoderPinCount = 0;
        do
        {
            if(validEncoder != BoardInReadValue(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN))
            {
                //无效累加
                readEncoderPinCount++;
            }
            else
            {
                //有效跳出
                break;
            }
            if(readEncoderPinCount >= 3)
            {
                //修正后电平讯号不匹配,出现异常
                errorCode |= (1<<8);
                errorCode |= sampleRackTrackGearEncoderEventCount;
                errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_STOP_ENCODER_INVALID_NEGATIVE,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }while(readEncoderPinCount <3);
        
    return LH_ERR_NONE;
}



//加样传送带复位
LH_ERR AppImplSampleRackTrackResetVersion0(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //齿数修正
        int resetEncoderIndex = 0;
        // AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,APP_PARAM_SUB_INDEX_SAMPLE_RACK_TRACK_GEAR_COUNT_RESET_CORRECT,&resetEncoderIndex);
        resetEncoderIndex = SAMPLE_RACK_TRACK_GEAR_COUNT_RESET_CORRECT;
    //复位指令结构体
        StepMotorResetCmd resetCmd;
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = SAMPLE_RACK_TRACK_STEP_MOTOR_ID;//电机序号
        resetCmd.correctionCoordinate = 0;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //解除中断
        BoardExtiUnRegister(INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN);
    //第一步是试管架传送带复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            errorCode = AppImplSampleRackTrackConvertErrorCode(FLAG_SAMPLE_RACK_TRACK_ERR_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //等待稳定
        CoreDelayMs(500);
    //第二步是运行到下一个有效齿位
        errorCode = AppImplSampleRackTrackRunNextGearPositive();
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //第三步根据偏移决定走走正向还是反向偏移
        if(resetEncoderIndex > 0)
        {
            errorCode = AppImplSampleRackTrackRunGearCountPositive(resetEncoderIndex);
        }
        else if(resetEncoderIndex < 0)
        {
            resetEncoderIndex = 0 - resetEncoderIndex;
            errorCode = AppImplSampleRackTrackRunGearCountNegative((uint32_t)resetEncoderIndex);
        }
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //初始化架号为RACK_TUBE_INDEX_1
        currentTubeIndex = RACK_TUBE_INDEX_1;
    return LH_ERR_NONE;
}

//加样传送带移动到指定试管位
LH_ERR AppImplSampleRackTrackMove2TubeVersion0(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex,RACK_TUBE_INDEX* currentTube)
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
    //识别目标位置,
    if(rackPos == SAMPLE_RACK_TRACK_POS_CURRENT)
    {
        //同一个试管架内部
        if(tubeIndex > currentTubeIndex)
        {
            //正向走
            errorCode = AppImplSampleRackTrackRunGearCountPositive((COUNT_SAMPLE_RACK_TRACK_ENCODER_ONE_TUBE*(tubeIndex - currentTubeIndex)));
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
        else
        {
            //反向走
            errorCode = AppImplSampleRackTrackRunGearCountNegative((COUNT_SAMPLE_RACK_TRACK_ENCODER_ONE_TUBE*(currentTubeIndex - tubeIndex)));
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
        currentTubeIndex = tubeIndex;
    }
    else
    {
        //下一个试管架,首先要把当前还剩下多少走完
        int32_t gearCount = 0;
        gearCount = COUNT_SAMPLE_RACK_TRACK_ENCODER_ONE_RACK - (currentTubeIndex*COUNT_SAMPLE_RACK_TRACK_ENCODER_ONE_TUBE);
        gearCount += (tubeIndex*COUNT_SAMPLE_RACK_TRACK_ENCODER_ONE_TUBE);
        //正向走位
        errorCode = AppImplSampleRackTrackRunGearCountPositive(gearCount);
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
LH_ERR AppImplSampleRackTrackBackFullSampleRackVersion0(RACK_TUBE_INDEX* currentTube)
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
        errorCode = AppImplSampleRackTrackRunGearCountNegative(COUNT_SAMPLE_RACK_TRACK_ENCODER_ONE_RACK);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前坐标
        *currentTube = currentTubeIndex;
    return errorCode;
}

//加样传送带进行一次码盘齿差修正
LH_ERR AppImplSampleRackTrackEncoderCorrectVersion0(uint8_t encoderCorrectCount)
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
        errorCode = AppImplSampleRackTrackRunGearCountNegative(encoderCorrectCount);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        CoreDelayMs(100);
    //正向将齿数走回来
        errorCode = AppImplSampleRackTrackRunGearCountPositive(encoderCorrectCount);
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
LH_ERR AppImplSampleRackTrackStepMotorResetVersion0(INDEX_MOTOR_SAMPLE_RACK_TRACK index)
{
    return AppImplSampleRackTrackResetVersion0();
}

//指定步进电机走指定步数
LH_ERR AppImplSampleRackTrackStepMotorRunStepsVersion0(INDEX_MOTOR_SAMPLE_RACK_TRACK index,int32_t steps,int32_t* posAfterRun)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = moduleStepMotorResetRunConfigArray[index].motorIndex;
        runStepCmd.specialSteps = steps;
        runStepCmd.selectConfig = SM_CURVE_0;//默认曲线0作为调试曲线
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
