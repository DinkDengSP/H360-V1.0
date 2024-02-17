/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 09:00:07
**FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorReset.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "UserMemManager.h"
#include "StepMotorAlarm.h"

/**************************************电机复位函数申明*****************************************/
//复位第一步找零点
static void StepMotorProcessResetSub_FIND_ZERO_FIRST(STEP_MOTOR_INDEX motorIndex);
//复位第二步离开零点
static void StepMotorProcessResetSub_LEAVE_ZERO(STEP_MOTOR_INDEX motorIndex);
//复位第三步,再次回原点
static void StepMotorProcessResetSub_FIND_ZERO_SECOND(STEP_MOTOR_INDEX motorIndex);
//复位第四步 复位修正
static void StepMotorProcessResetSub_CORRECTION(STEP_MOTOR_INDEX motorIndex);
/**********************************************************************************************/

//步进电机复位切换方向的速度比例
#define STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO             5.0

//复位
void CpldStepMotorResetProc(STEP_MOTOR_INDEX motorIndex)
{
    switch(stepMotorVar[motorIndex]->var.motorSubState)
    {
        case STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST:
            //第一步找零点
            StepMotorProcessResetSub_FIND_ZERO_FIRST(motorIndex);
            break;
        case STEP_MOTOR_RESET_SUB_LEAVE_ZERO:
            //第二步离开零点
            StepMotorProcessResetSub_LEAVE_ZERO(motorIndex);
            break;
        case STEP_MOTOR_RESET_SUB_FIND_ZERO_SECOND:
            //第三步找零点
            StepMotorProcessResetSub_FIND_ZERO_SECOND(motorIndex);
            break;
        case STEP_MOTOR_RESET_SUB_CORRECTION:
            //零位修正
            StepMotorProcessResetSub_CORRECTION(motorIndex);
            break;
        default:
            //不识别的状态,直接停掉
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorIndex]->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}


/********************************************************系统复位相关状态机处理函数**************************************************/
//复位第一步找零点
static void StepMotorProcessResetSub_FIND_ZERO_FIRST(STEP_MOTOR_INDEX motorIndex)
{
    //设置了最大复位步数的情况下
    if(stepMotorVar[motorIndex]->attri.baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse > stepMotorVar[motorIndex]->attri.baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //步数超限制状态
            stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    //读取零点状态
    BitAction orginSensorValue = BoardInReadValue(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex);
    //如果此时原点被触发,那么离开原点,否则找原点
    if(orginSensorValue == stepMotorVar[motorIndex]->attri.baseConfig.resetPinValidLevel)
    {
        //找到了,切换离开原点
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
        //离开原点
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RESET_SUB_LEAVE_ZERO;
        stepMotorVar[motorIndex]->var.currentDir = (stepMotorVar[motorIndex]->attri.baseConfig.dir);
        stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse += 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,CPLD_STEP_MOTOR_STOP_OFF,
                                        ((stepMotorVar[motorIndex]->attri.baseConfig.resetSpeed)/STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO),1);
    }
    else
    {
        //继续找
        stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse += 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,
                                        CPLD_STEP_MOTOR_STOP_OFF,stepMotorVar[motorIndex]->attri.baseConfig.resetSpeed,1);
    }
}






//复位第二步离开零点
static void StepMotorProcessResetSub_LEAVE_ZERO(STEP_MOTOR_INDEX motorIndex)
{
    //设置了最大复位步数的情况下
    if(stepMotorVar[motorIndex]->attri.baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse > stepMotorVar[motorIndex]->attri.baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //步数超限制状态
            stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    //读取零点状态
    BitAction orginSensorValue = BoardInReadValue(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex);
    //如果此时原点被触发,那么离开原点,否则,进入下一步回原点
    if(orginSensorValue == stepMotorVar[motorIndex]->attri.baseConfig.resetPinValidLevel)
    {
        //继续离开原点
        stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse += 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,CPLD_STEP_MOTOR_STOP_OFF,
                                        ((stepMotorVar[motorIndex]->attri.baseConfig.resetSpeed)/STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO),1);
    }
    else
    {
        //已经离开,现在再次找原点
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
        //切换第三步继续找零点
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_SECOND;
        //切换方向
        stepMotorVar[motorIndex]->var.currentDir = (BOARD_CPLD_MOTOR_DIR)(!(stepMotorVar[motorIndex]->attri.baseConfig.dir));
        //步数计数
        stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse += 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,CPLD_STEP_MOTOR_STOP_OFF,
                                        ((stepMotorVar[motorIndex]->attri.baseConfig.resetSpeed)/STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO),1);
    }
}


//复位第三步,再次回原点
static void StepMotorProcessResetSub_FIND_ZERO_SECOND(STEP_MOTOR_INDEX motorIndex)
{
    //设置了最大复位步数的情况下
    if(stepMotorVar[motorIndex]->attri.baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse > stepMotorVar[motorIndex]->attri.baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //步数超限制状态
            stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    //读取零点状态
    BitAction orginSensorValue = BoardInReadValue(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex);
    //如果此时原点被触发,那么开始复位修正,否则找原点
    if(orginSensorValue == stepMotorVar[motorIndex]->attri.baseConfig.resetPinValidLevel)
    {
        //复位完成,切换停止
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //正常运行完成
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        
    }
    else
    {
        //继续找
        stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse += 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,CPLD_STEP_MOTOR_STOP_OFF,
                                        ((stepMotorVar[motorIndex]->attri.baseConfig.resetSpeed)/STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO),1);
    }
}






//复位第四步 复位修正
static void StepMotorProcessResetSub_CORRECTION(STEP_MOTOR_INDEX motorIndex)
{
    if(stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0] == 0)
    {
        //运行完成,切换停止
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //正常运行完成
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    }
    else
    {
        //继续运转
        if(stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0] > stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax)
        {
            BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,CPLD_STEP_MOTOR_STOP_OFF,
                                            stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[0],stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax);
            //减小发出的步数
            stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0] -= stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax;
        }
        else
        {
            BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,CPLD_STEP_MOTOR_STOP_OFF,
                                            stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[0],stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0]);
            //发完剩余步数为0
            stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0] = 0;
        }
        //继续修正
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RESET_SUB_CORRECTION;
    }
}
/*********************************************************************************************************************************/



/**************************************************************************************************/
//检测步进电机的复位的起始运行条件
static LH_ERR StepMotorResetCheckStartCondition(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_INDEX motorIndex)
{
    //检测电机行程限制,如果行程限制为0,代表不限制行程,否则,补偿有行程限制
    if(stepMotorVar[motorIndex]->attri.baseConfig.positiveMaxSteps != 0)
    {
        //检查复位修正是否在运行行程里面
        if(resetCmdPtr->correctionCoordinate > (stepMotorVar[motorIndex]->attri.baseConfig.positiveMaxSteps))
        {
            return LH_ERR_SERVICE_SM_RESET_CORRECT_MAX;
        }
    }
    if(stepMotorVar[motorIndex]->attri.baseConfig.reverseMaxSteps != 0)
    {
        //限制行程
        if(resetCmdPtr->correctionCoordinate < (stepMotorVar[motorIndex]->attri.baseConfig.reverseMaxSteps))
        {
            return LH_ERR_SERVICE_SM_RESET_CORRECT_MIN;
        }
    }
    if(resetCmdPtr->timeOutSet == 0)
    {
        return LH_ERR_SERVICE_SM_TIME_OUT_ZERO;
    }
    return LH_ERR_NONE;
}



//步进电机发送复位开始
static void StepMotorResetSendFindZeroStart(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_INDEX motorIndex)
{
    //配置了原点
    //第一步,找原点,先读取当前原点状态
    BitAction orginSensorValue = BoardInReadValue(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex);
    //如果此时原点被触发,那么离开原点,否则找原点
    if(orginSensorValue == stepMotorVar[motorIndex]->attri.baseConfig.resetPinValidLevel)
    {
        //离开原点
        stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
        stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RESET_SUB_LEAVE_ZERO;
        //电机当前方向应该是运行时正方向
        stepMotorVar[motorIndex]->var.currentDir = (stepMotorVar[motorIndex]->attri.baseConfig.dir);
        //增加1步的复位脉冲数
        stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse = 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,CPLD_STEP_MOTOR_STOP_OFF,
                                        ((stepMotorVar[motorIndex]->attri.baseConfig.resetSpeed)/STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO),1);
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
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,
                                        CPLD_STEP_MOTOR_STOP_OFF,stepMotorVar[motorIndex]->attri.baseConfig.resetSpeed,1);
    }
}



//等待步进电机找零点结束
static LH_ERR StepMotorResetWaitFindZeroOver(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_INDEX motorIndex)
{
    LH_ERR lhErrCode = LH_ERR_NONE;
    OS_ERR err;
    //等待电机运行停下来
    while(stepMotorVar[motorIndex]->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //等待步进电机状态机停止
        CoreDelayMinTick();
        //更新运行时间
        stepMotorVar[motorIndex]->var.stepMotorRunningTimeMsCount = OSTimeGet(&err) - stepMotorVar[motorIndex]->var.stepMotorStartRunningTimeMs;
        //检测超时,自动停止
        if(stepMotorVar[motorIndex]->var.stepMotorRunningTimeMsCount > resetCmdPtr->timeOutSet)
        {
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
        }
        //设置了最大复位步数的情况下
        if(stepMotorVar[motorIndex]->attri.baseConfig.resetMaxSteps != 0)
        {
            //检测步数超了,自动停止
            if(stepMotorVar[motorIndex]->var.resetCurrentSetOutPluse > stepMotorVar[motorIndex]->attri.baseConfig.resetMaxSteps)
            {
                stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
                stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            }
        }
    }
    //将数据转换为结果代码之后发送出去
    lhErrCode = StepMotorConvert2LH_ErrCode(stepMotorVar[motorIndex]->var.motorSubState);
    return lhErrCode;
}




//发送步进电机复位修正
static void StepMotorResetSendCorrectStart(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_INDEX motorIndex)
{
    if(resetCmdPtr->correctionCoordinate > 0)
    {
        //正向修正
        //电机当前方向应该是运行时正方向
        stepMotorVar[motorIndex]->var.currentDir = (stepMotorVar[motorIndex]->attri.baseConfig.dir);
        //步数符号转换
        stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0] = (uint32_t)(resetCmdPtr->correctionCoordinate);
    }
    else
    {
        //反向修正
        //电机当前方向应该是运行时反方向
        stepMotorVar[motorIndex]->var.currentDir = (BOARD_CPLD_MOTOR_DIR)(!(stepMotorVar[motorIndex]->attri.baseConfig.dir));
        //步数符号转换
        stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0] = (uint32_t)(0 - resetCmdPtr->correctionCoordinate);
    }
    //设定速度,复位修正的速度是复位速度
    stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[0] = stepMotorVar[motorIndex]->attri.baseConfig.resetSpeed;
    //修正模式
    stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
    stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_RESET_SUB_CORRECTION;
    //开始运转
    if(stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0] > stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax)
    {
        //大于单次发送最大步数,一次发满
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,CPLD_STEP_MOTOR_STOP_OFF,
                                        stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[0],stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax);
        stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0] -= stepMotorVar[motorIndex]->attri.baseConfig.singlePluseMax;
    }
    else
    {
        //小于255,一次发完
        BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,stepMotorVar[motorIndex]->var.currentDir,CPLD_STEP_MOTOR_STOP_OFF,
                                        stepMotorVar[motorIndex]->var.stepMotorSpeedTableDirect[0],stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0]);
        stepMotorVar[motorIndex]->var.stepMotorStepTableDirect[0] = 0;
    }
}





//等待步进电机复位修正结束
static LH_ERR StepMotorResetWaitCorrectOver(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_INDEX motorIndex)
{
    LH_ERR lhErrCode = LH_ERR_NONE;
    OS_ERR err;
    //等待完成
    while(stepMotorVar[motorIndex]->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //等待步进电机状态机停止
        CoreDelayMinTick();
        //更新运行时间
        stepMotorVar[motorIndex]->var.stepMotorRunningTimeMsCount = OSTimeGet(&err) - stepMotorVar[motorIndex]->var.stepMotorStartRunningTimeMs;
        //检测超时,自动停止
        if(stepMotorVar[motorIndex]->var.stepMotorRunningTimeMsCount > resetCmdPtr->timeOutSet)
        {
            stepMotorVar[motorIndex]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorIndex]->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
        }
    }
    //将数据转换为结果代码之后发送出去
    lhErrCode = StepMotorConvert2LH_ErrCode(stepMotorVar[motorIndex]->var.motorSubState);
    return lhErrCode;
}




//指定步进电机复位
LH_ERR StepMotorReset(StepMotorResetCmd* resetCmdPtr)
{
    OS_ERR err;
    LH_ERR lhErrCode = LH_ERR_NONE;
    STEP_MOTOR_INDEX motorIndex = resetCmdPtr->motorIndex;
    //复位运行起始条件检测
    lhErrCode = StepMotorResetCheckStartCondition(resetCmdPtr,motorIndex);
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    //相关状态变量初始化到准备复位状态
    StepMotorStateInitForReset(&(stepMotorVar[motorIndex]->var));
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVar[motorIndex]->var.stepMotorStartRunningTimeMs = OSTimeGet(&err);
    //首先查看是否配置零位传感器,配置了原点,就进入找原点流程
    if(stepMotorVar[motorIndex]->attri.baseConfig.resetPinIndex < BOARD_IN_PIN_COUNT)
    {
        //发送一个找原点讯息
        StepMotorResetSendFindZeroStart(resetCmdPtr,motorIndex);
        //等待找原点结束
        lhErrCode = StepMotorResetWaitFindZeroOver(resetCmdPtr,motorIndex);
        //结果未报错,开始运行修正
        if(lhErrCode != LH_ERR_NONE)
        {
            return lhErrCode;
        }
    }
    //无修正,运行完成了
    if(resetCmdPtr->correctionCoordinate == 0)
    {
        return LH_ERR_NONE; 
    }
    //发送修正信息
    StepMotorResetSendCorrectStart(resetCmdPtr,motorIndex);
    //等待修正结束
    lhErrCode = StepMotorResetWaitCorrectOver(resetCmdPtr,motorIndex);
    //结果未报错,修正完成
    if(lhErrCode != LH_ERR_NONE)
    {
        return lhErrCode;
    }
    else
    {
        //记录当前坐标
        stepMotorVar[motorIndex]->var.currentCoordinate = resetCmdPtr->correctionCoordinate;
        return LH_ERR_NONE;
    }
}
























