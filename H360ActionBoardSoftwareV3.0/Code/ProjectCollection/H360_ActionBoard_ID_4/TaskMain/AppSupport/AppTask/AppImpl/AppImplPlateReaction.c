/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:47:17
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-29 09:25:15
**FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateReaction.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplPlateReaction.h"

//转换反应盘错误代码的标识
typedef enum FLAG_PLATE_REACTION_ERR_CONVERT
{
    FLAG_PLATE_REACTION_ERR_ROTATE_RESET            = 0X01,//反应盘旋转复位时候产生的错误代码
    FLAG_PLATE_REACTION_ERR_ROTATE_RUN              = 0X02,//反应盘旋转运行时候产生的错误代码
    FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_NEW   = 0X03,//反应盘读取新杯光纤产生的错误信号
    FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_OLD   = 0X04,//反应盘读取旧杯光纤产生的错误信号
}FLAG_PLATE_REACTION_ERR_CONVERT;

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_PLATE_REACTION;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD4_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}

//计算反应盘运行曲线
static uint8_t AppImplPlateReactionCalcStepMotorCurve(int32_t steps)
{
    //先把负数步数修正为正数步数
        if(steps < 0)
        {
            steps = 0 - steps;
        }
    //首先获取一格的脉冲
        int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
        //参数为0,报错
        if(oneCupOffset == 0)return S_CURVE_PLATE_REACTION_ROTATE_DEBUG;
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
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_1_CUP;
    }
    else if(cupCount <= 9)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_9_CUP;
    }
    else if(cupCount <= 13)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_13_CUP;
    }
    else if(cupCount <= 40)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_40_CUP;
    }
    else if(cupCount <= 59)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_59_CUP;
    }
    else if(cupCount <= 120)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_120_CUP;
    }
    else
    {
        return S_CURVE_PLATE_REACTION_ROTATE_DEBUG;
    }
}


//当前反应盘新杯位的杯位序号
static uint8_t currentPlateReactionNewCupHoleIndex = 0;

//反应盘复位
LH_ERR AppImplPlateReactionReset(PLATE_REACTION_RESULT* plateReactionResult)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数配置
        //参数辅助,用于系统参数读取
        int32_t paramUtil;
        //复位指令结构体
        StepMotorResetCmd resetCmd;
    //读取复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_PLATE_REACTION,APP_PARAM_SUB_INDEX_PLATE_REACTION_ROTATE_RESET_CORRECT,&paramUtil);
    //配置升降复制指令结构体
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = PLATE_REACTION_STEP_MOTOR_ROTATE_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_ROTATE_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //复位完成,等待指定光纤稳定时间
        CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_SENSOR_STABLE);
    //读取新杯光纤
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_NEW,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_INVALID;
        }
    //读取旧杯光纤
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_OLD,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_INVALID;
        }
    //设置当前序号为复位位置
        currentPlateReactionNewCupHoleIndex = PLATE_REACTION_INIT_CUP_INDEX;
        plateReactionResult->currentNewCupIndex = currentPlateReactionNewCupHoleIndex;
    //返回完成
        return LH_ERR_NONE;
}

//反应盘正向运转到指定序号杯位
LH_ERR AppImplPlateReactionRunToCupWithIndexPositive(uint8_t targetCup,PLATE_REACTION_RESULT* plateReactionResult)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //修正参数
        if(targetCup > PLATE_REACTION_MAX_CUP_INDEX)
        {
            targetCup = PLATE_REACTION_MAX_CUP_INDEX;
        }
    //获取单个杯位偏移
        int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
    /*计算移动到指定杯位需要的步数 */
        int32_t stepNeedRun = 0;
        //计算坐标
        if(targetCup > currentPlateReactionNewCupHoleIndex)
        {
            stepNeedRun = (targetCup - currentPlateReactionNewCupHoleIndex)*oneCupOffset;
        }
        else if(targetCup < currentPlateReactionNewCupHoleIndex)
        {
            stepNeedRun = (PLATE_REACTION_MAX_CUP_INDEX - currentPlateReactionNewCupHoleIndex + targetCup)*oneCupOffset;
        }
        else
        {
            stepNeedRun = 0;
        }
    //根据步数选择相应的速度曲线
        uint8_t selectCurve = AppImplPlateReactionCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = PLATE_REACTION_STEP_MOTOR_ROTATE_ID;//电机序号
        runStepCmd.specialSteps = stepNeedRun;//步数
        runStepCmd.selectConfig = selectCurve;//曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //更新杯位
        currentPlateReactionNewCupHoleIndex = targetCup;
        plateReactionResult->currentNewCupIndex = currentPlateReactionNewCupHoleIndex;
    //等待指定光纤稳定时间
        CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_SENSOR_STABLE);
    //读取新杯光纤
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_NEW,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_INVALID;
        }
    //读取旧杯光纤
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_OLD,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_INVALID;
        }
    //返回完成
        return LH_ERR_NONE;
}

//反应盘正向旋转指定数量杯位
LH_ERR AppImplPlateReactionRunAnyCupsPositive(uint8_t cups,PLATE_REACTION_RESULT* plateReactionResult)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //修正参数
        if(cups > PLATE_REACTION_MAX_CUP_INDEX)
        {
            cups = PLATE_REACTION_MAX_CUP_INDEX;
        }
    //获取单个杯位偏移
        int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
    /*计算移动到指定杯位需要的步数 */
        int32_t stepNeedRun = 0;
        stepNeedRun = oneCupOffset*cups;
    //根据步数选择相应的速度曲线
        uint8_t selectCurve = AppImplPlateReactionCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = PLATE_REACTION_STEP_MOTOR_ROTATE_ID;//电机序号
        runStepCmd.specialSteps = stepNeedRun;//步数
        runStepCmd.selectConfig = selectCurve;//曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //更新杯位
        currentPlateReactionNewCupHoleIndex += cups;
        if(currentPlateReactionNewCupHoleIndex > PLATE_REACTION_MAX_CUP_INDEX)
        {
            currentPlateReactionNewCupHoleIndex -= PLATE_REACTION_MAX_CUP_INDEX;
        }
        plateReactionResult->currentNewCupIndex = currentPlateReactionNewCupHoleIndex;
    //等待指定光纤稳定时间
        CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_SENSOR_STABLE);
    //读取新杯光纤
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_NEW,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_INVALID;
        }
    //读取旧杯光纤
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_OLD,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_INVALID;
        }
    //返回完成
        return LH_ERR_NONE;
}

//反应盘反向运转到指定序号杯位
LH_ERR AppImplPlateReactionRunToCupWithIndexNegative(uint8_t targetCup,PLATE_REACTION_RESULT* plateReactionResult)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //修正参数
        if(targetCup > PLATE_REACTION_MAX_CUP_INDEX)
        {
            targetCup = PLATE_REACTION_MAX_CUP_INDEX;
        }
    //获取单个杯位偏移
        int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
    //一整圈的步数,这里有个BUG,就是一格为负数的时候需要做正负号的修正处理,这里我们不管
    //但是严格来说代码是不安全的
        int32_t holeCircleSteps = oneCupOffset*PLATE_REACTION_MAX_CUP_INDEX;
    /*计算移动到指定杯位需要的步数 */
        int32_t stepNeedRun = 0;
        //计算坐标
        if(targetCup > currentPlateReactionNewCupHoleIndex)
        {
            stepNeedRun = (targetCup - currentPlateReactionNewCupHoleIndex)*oneCupOffset;
            //计算出反转需要的步数
            stepNeedRun = holeCircleSteps - stepNeedRun;
        }
        else if(targetCup < currentPlateReactionNewCupHoleIndex)
        {
            stepNeedRun = (PLATE_REACTION_MAX_CUP_INDEX - currentPlateReactionNewCupHoleIndex + targetCup)*oneCupOffset;
            //计算出反转需要的步数
            stepNeedRun = holeCircleSteps - stepNeedRun;
        }
        else
        {
            stepNeedRun = 0;
        }
    //根据步数选择相应的速度曲线
        uint8_t selectCurve = AppImplPlateReactionCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = PLATE_REACTION_STEP_MOTOR_ROTATE_ID;//电机序号
        //反向运转,杯位为负
        stepNeedRun = 0 - stepNeedRun;
        runStepCmd.specialSteps = stepNeedRun;//步数
        runStepCmd.selectConfig = selectCurve;//曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //更新杯位
        currentPlateReactionNewCupHoleIndex = targetCup;
        plateReactionResult->currentNewCupIndex = currentPlateReactionNewCupHoleIndex;
    //等待指定光纤稳定时间
        CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_SENSOR_STABLE);
    //读取新杯光纤
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_NEW,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_INVALID;
        }
    //读取旧杯光纤
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_OLD,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_INVALID;
        }
    //返回完成
        return LH_ERR_NONE;
}

//反应盘反向旋转指定数量杯位
LH_ERR AppImplPlateReactionRunAnyCupsNegative(uint8_t cups,PLATE_REACTION_RESULT* plateReactionResult)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //修正参数
        if(cups > PLATE_REACTION_MAX_CUP_INDEX)
        {
            cups = PLATE_REACTION_MAX_CUP_INDEX;
        }
    //获取单个杯位偏移
        int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
    /*计算移动到指定杯位需要的步数 */
        int32_t stepNeedRun = 0;
        stepNeedRun = oneCupOffset*cups;
    //根据步数选择相应的速度曲线
        uint8_t selectCurve = AppImplPlateReactionCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
        StepMotorRunStepsCmd runStepCmd;
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = PLATE_REACTION_STEP_MOTOR_ROTATE_ID;//电机序号
        stepNeedRun = 0 - stepNeedRun;
        runStepCmd.specialSteps = stepNeedRun;//步数
        runStepCmd.selectConfig = selectCurve;//曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //更新杯位
        if(currentPlateReactionNewCupHoleIndex > cups)
        {
            currentPlateReactionNewCupHoleIndex -= cups;
        }
        else
        {
            currentPlateReactionNewCupHoleIndex = PLATE_REACTION_MAX_CUP_INDEX + currentPlateReactionNewCupHoleIndex - cups;
        }
        plateReactionResult->currentNewCupIndex = currentPlateReactionNewCupHoleIndex;
    //等待指定光纤稳定时间
        CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_SENSOR_STABLE);
    //读取新杯光纤
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_NEW,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistNewPos = SENSOR_STATE_INVALID;
        }
    //读取旧杯光纤
        errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPlateReactionConvertErrorCode(FLAG_PLATE_REACTION_ERR_READ_LIGHT_SENSOR_OLD,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换光纤状态
        if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR)
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_VALID;
        }
        else
        {
            plateReactionResult->cupExistOldPos = SENSOR_STATE_INVALID;
        }
    //返回完成
        return LH_ERR_NONE;
}


//步进电机调试配置表
static const MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[] = {
    {PLATE_REACTION_STEP_MOTOR_ROTATE_ID,INDEX_CAN_SUB_BOARD4_PARAM_PLATE_REACTION,APP_PARAM_SUB_INDEX_PLATE_REACTION_ROTATE_RESET_CORRECT},//反应盘旋转
};

//指定步进电机复位
LH_ERR AppImplPlateReactionStepMotorReset(INDEX_MOTOR_PLATE_REACTION index)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
        StepMotorResetCmd resetCmd;
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //读取修正
        AppParamRead((INDEX_CAN_SUB_BOARD4_PARAM_MAIN)(moduleStepMotorResetRunConfigArray[index].resetCorrectMainIndex),
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

//指定步进电机走步数
LH_ERR AppImplPlateReactionStepMotorRunSteps(INDEX_MOTOR_PLATE_REACTION index,int32_t steps,int32_t* posAfterRun)
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








