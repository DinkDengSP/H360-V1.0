/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:31:03
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 19:23:05
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplPlateWashVersion1.h"

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplPlateWashConvertErrorCode(FLAG_PLATE_WASH_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_PLATE_WASH;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD1_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}


//当前清洗盘新杯位的杯位序号
static uint8_t currentPlateWashNewCupHoleIndex = 0;

//计算清洗盘运行曲线
static uint8_t AppImplPlateWashCalcStepMotorCurve(int32_t steps)
{
    //先把负数步数修正为正数步数
        if(steps < 0)
        {
            steps = 0 - steps;
        }
    //首先获取一格的脉冲
        int32_t oneCupOffset = PLATE_WASH_ONE_HOLE_STEPS;
        //参数为0,报错
        if(oneCupOffset == 0)return S_CURVE_PLATE_WASH_ROTATE_DEBUG;
        //一格对应步数修正
        if(oneCupOffset < 0)
        {
            oneCupOffset = 0 - oneCupOffset;
        }
    //计算目前走的坐标对应几格
        int32_t cupCount =  (steps/oneCupOffset);
        if((steps%oneCupOffset) != 0)
        {
            cupCount += 1;
        }
    //计算曲线
    if(cupCount <= 1)
    {
        return S_CURVE_PLATE_WASH_ROTATE_MOVE_1_CUP;
        /* #if(PLATE_WASH_INIT_CUP_INDEX == 1)
            if(currentPlateWashNewCupHoleIndex == PLATE_WASH_MAX_CUP_INDEX)
            {
                return S_CURVE_PLATE_WASH_ROTATE_MOVE_1_CUP_RESET;
            }
            else
            {
                return S_CURVE_PLATE_WASH_ROTATE_MOVE_1_CUP;
            }
        #else
            if(currentPlateWashNewCupHoleIndex == (PLATE_WASH_INIT_CUP_INDEX -1))
            {
                return S_CURVE_PLATE_WASH_ROTATE_MOVE_1_CUP_RESET;
            }
            else
            {
                return S_CURVE_PLATE_WASH_ROTATE_MOVE_1_CUP;
            }
        #endif */
    }
    else if(cupCount <= 4)
    {
        return S_CURVE_PLATE_WASH_ROTATE_MOVE_4_CUP;
    }
    else if(cupCount <= 8)
    {
        return S_CURVE_PLATE_WASH_ROTATE_MOVE_8_CUP;
    }
    else if(cupCount <= 12)
    {
        return S_CURVE_PLATE_WASH_ROTATE_MOVE_12_CUP;
    }
    else if(cupCount <= 16)
    {
        return S_CURVE_PLATE_WASH_ROTATE_MOVE_16_CUP;
    }
    else if(cupCount <= 20)
    {
        return S_CURVE_PLATE_WASH_ROTATE_MOVE_20_CUP;
    }
    else if(cupCount <= 24)
    {
        return S_CURVE_PLATE_WASH_ROTATE_MOVE_24_CUP;
    }
    else
    {
        return S_CURVE_PLATE_WASH_ROTATE_DEBUG;
    }
}

//清洗盘旋转复位,带零位修正
LH_ERR AppImplPlateWashResetVersion1(SENSOR_STATE* cupExistState,uint8_t* cupIndex)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数配置
        //参数辅助,用于系统参数读取
        int32_t paramUtil;
        //复位指令结构体
        StepMotorResetCmd resetCmd;
    //读取复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_PLATE_WASH,APP_PARAM_SUB_INDEX_PLATE_WASH_RESET_CORRECT,&paramUtil);
    //配置复位指令结构体
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = PLATE_WASH_STEP_MOTOR_ROTATE_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateWashConvertErrorCode(FLAG_PLATE_WASH_ERR_ROTATE_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //复位完成,等待指定光纤稳定时间
        CoreDelayMsSensitivity(TIME_MS_PLATE_WASH_WAIT_LIGHT_SENSOR_STABLE);
    //读取新杯光纤
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_PLATE_WASH_NEW_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateWashConvertErrorCode(FLAG_PLATE_WASH_ERR_READ_LIGHT_SENSOR_NEW,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_WASH_NEW_CUP_LIGHT_SENSOR)
        {
            *cupExistState = SENSOR_STATE_VALID;
        }
        else
        {
            *cupExistState = SENSOR_STATE_INVALID;
        }
    //设置当前序号为复位位置
        currentPlateWashNewCupHoleIndex = PLATE_WASH_INIT_CUP_INDEX;
        *cupIndex = currentPlateWashNewCupHoleIndex;
    //返回完成
        return LH_ERR_NONE;
}


//清洗盘移动指定个杯位
LH_ERR AppImplPlateWashRunAnyCupsVersion1(uint8_t cupsCount,SENSOR_STATE* cupExistState,uint8_t* cupIndex)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //修正参数
        if(cupsCount > PLATE_WASH_MAX_CUP_INDEX)
        {
            cupsCount = PLATE_WASH_MAX_CUP_INDEX;
        }
    //获取单个杯位偏移
        int32_t oneCupOffset = PLATE_WASH_ONE_HOLE_STEPS;
    /*计算移动到指定杯位需要的步数 */
        int32_t stepNeedRun = 0;
        stepNeedRun = oneCupOffset*cupsCount;
    //根据步数选择相应的速度曲线
        uint8_t selectCurve = AppImplPlateWashCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = PLATE_WASH_STEP_MOTOR_ROTATE_ID;//电机序号
        runStepCmd.specialSteps = stepNeedRun;//步数
        runStepCmd.selectConfig = selectCurve;//曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateWashConvertErrorCode(FLAG_PLATE_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //更新杯位
        currentPlateWashNewCupHoleIndex += cupsCount;
        if(currentPlateWashNewCupHoleIndex > PLATE_WASH_MAX_CUP_INDEX)
        {
            currentPlateWashNewCupHoleIndex -= PLATE_WASH_MAX_CUP_INDEX;
        }
        *cupIndex = currentPlateWashNewCupHoleIndex;
    //运动完成,等待指定光纤稳定时间
        CoreDelayMsSensitivity(TIME_MS_PLATE_WASH_WAIT_LIGHT_SENSOR_STABLE);
    //读取新杯光纤
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_PLATE_WASH_NEW_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateWashConvertErrorCode(FLAG_PLATE_WASH_ERR_READ_LIGHT_SENSOR_NEW,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_WASH_NEW_CUP_LIGHT_SENSOR)
        {
            *cupExistState = SENSOR_STATE_VALID;
        }
        else
        {
            *cupExistState = SENSOR_STATE_INVALID;
        }
    //返回完成
        return LH_ERR_NONE;
}


//清洗盘移动到指定序号杯位
LH_ERR AppImplPlateWashRun2CupWithIndexVersion1(uint8_t targetIndex,SENSOR_STATE* cupExistState,uint8_t* cupIndex)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //修正参数
        if(targetIndex > PLATE_WASH_MAX_CUP_INDEX)
        {
            targetIndex = PLATE_WASH_MAX_CUP_INDEX;
        }
    //获取单个杯位偏移
        int32_t oneCupOffset = PLATE_WASH_ONE_HOLE_STEPS;
    /*计算移动到指定杯位需要的步数 */
        int32_t stepNeedRun = 0;
        //计算坐标
        if(targetIndex > currentPlateWashNewCupHoleIndex)
        {
            stepNeedRun = (targetIndex - currentPlateWashNewCupHoleIndex)*oneCupOffset;
        }
        else if(targetIndex < currentPlateWashNewCupHoleIndex)
        {
            stepNeedRun = (PLATE_WASH_MAX_CUP_INDEX - currentPlateWashNewCupHoleIndex + targetIndex)*oneCupOffset;
        }
        else
        {
            stepNeedRun = 0;
        }
    //根据步数选择相应的速度曲线
        uint8_t selectCurve = AppImplPlateWashCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = PLATE_WASH_STEP_MOTOR_ROTATE_ID;//电机序号
        //反向运转,杯位为负
        stepNeedRun = stepNeedRun;
        runStepCmd.specialSteps = stepNeedRun;//步数
        runStepCmd.selectConfig = selectCurve;//曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateWashConvertErrorCode(FLAG_PLATE_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //更新杯位
        currentPlateWashNewCupHoleIndex = targetIndex;
        *cupIndex = currentPlateWashNewCupHoleIndex;
    //运动完成,等待指定光纤稳定时间
        CoreDelayMsSensitivity(TIME_MS_PLATE_WASH_WAIT_LIGHT_SENSOR_STABLE);
    //读取新杯光纤
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_PLATE_WASH_NEW_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateWashConvertErrorCode(FLAG_PLATE_WASH_ERR_READ_LIGHT_SENSOR_NEW,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_WASH_NEW_CUP_LIGHT_SENSOR)
        {
            *cupExistState = SENSOR_STATE_VALID;
        }
        else
        {
            *cupExistState = SENSOR_STATE_INVALID;
        }
    //返回完成
        return LH_ERR_NONE;
}


//步进电机调试配置表
static const MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[] = {
    {PLATE_WASH_STEP_MOTOR_ROTATE_ID,INDEX_CAN_SUB_BOARD1_PARAM_MAIN_PLATE_WASH,APP_PARAM_SUB_INDEX_PLATE_WASH_RESET_CORRECT},//清洗盘旋转
};

//指定步进电机复位,复位带零位修正
LH_ERR AppImplPlateWashStepMotorResetVersion1(INDEX_MOTOR_PLATE_WASH index)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
        StepMotorResetCmd resetCmd;
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //读取修正
        AppParamRead((INDEX_CAN_SUB_BOARD1_PARAM_MAIN)(moduleStepMotorResetRunConfigArray[index].resetCorrectMainIndex),
                        moduleStepMotorResetRunConfigArray[index].resetCorrectSubIndex,
                        &paramUtil);
    //填充参数
        resetCmd.motorIndex = moduleStepMotorResetRunConfigArray[index].motorIndex;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    return LH_ERR_NONE;
}


//指定步进电机走指定步数
LH_ERR AppImplPlateWashStepMotorRunStepsVersion1(INDEX_MOTOR_PLATE_WASH index,int32_t steps,int32_t* posAfterRun)
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
























