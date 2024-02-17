/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:02:16
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 16:13:03
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplBase.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplBase.h"

//RailPushIn1任务
const uint8_t* RailPushIn1DebugStartMsg = "$$Action 1 Start : ";
const uint8_t* RailPushIn1DebugEndMsg = "$$Action 1 End ";

//RailPushIn2任务
const uint8_t* RailPushIn2DebugStartMsg = "$$Action 2 Start : ";
const uint8_t* RailPushIn2DebugEndMsg = "$$Action 2 End ";

//RailPushOut1任务
const uint8_t* RailPushOut1DebugStartMsg = "$$Action 3 Start : ";
const uint8_t* RailPushOut1DebugEndMsg = "$$Action 3 End ";

//RailPushOut2任务
const uint8_t* RailPushOut2DebugStartMsg = "$$Action 4 Start : ";
const uint8_t* RailPushOut2DebugEndMsg = "$$Action 4 End ";

//RailTrans任务
const uint8_t* RailTransDebugStartMsg = "$$Action 5 Start : ";
const uint8_t* RailTransDebugEndMsg = "$$Action 5 End ";

//RailHand任务
const uint8_t* RailHandDebugStartMsg = "$$Action 6 Start : ";
const uint8_t* RailHandDebugEndMsg = "$$Action 6 End ";

//RailComplex任务
const uint8_t* RailComplexDebugStartMsg = "$$Action 7 Start : ";
const uint8_t* RailComplexDebugEndMsg = "$$Action 7 End ";

//BarScan任务
const uint8_t* BarScanDebugStartMsg = "$$Action 8 Start : ";
const uint8_t* BarScanDebugEndMsg = "$$Action 8 End ";

//Heart任务
const uint8_t* HeartDebugStartMsg = "$$Action 9 Start : ";
const uint8_t* HeartDebugEndMsg = "$$Action 9 End ";

//Debug任务
const uint8_t* DebugDebugStartMsg = "$$Action 10 Start : ";
const uint8_t* DebugDebugEndMsg = "$$Action 10 End ";


//辅助通用功能实现
LH_ERR ServiceImplBase_UtilCommon(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    return LH_ERR_NONE;
}


//自动运行任务的周期,单位毫秒
#define HEART_EVENT_PERIOD_TIME_MS          500
//自动运行任务的触发次数,决定是否执行任务
static uint32_t countHeartEventTrigCount = 0;

//自动循环心跳
void ServiceImplHeart_EventInit(void)
{
    countHeartEventTrigCount = 0; 
}

//只需要执行的事件回调函数,不需要计算时间
static void ServiceImplHeart_EventJustRun(void)
{
    BoardLedToogle(BOARD_LED_GREEN);
}


//自动循环心跳
void ServiceImplHeart_EventRun(uint32_t periodTimeMs)
{
    if(periodTimeMs == 0)
    {
        return;
    }
    //心跳时间计时器增加
    countHeartEventTrigCount++;
    //如果执行周期大于事件周期,立即运行,如果计数器事件次数大于事件次数,也执行
    if((periodTimeMs >= HEART_EVENT_PERIOD_TIME_MS)||(countHeartEventTrigCount >= (uint32_t)(HEART_EVENT_PERIOD_TIME_MS/periodTimeMs)))
    {
        countHeartEventTrigCount = 0;
        //执行一次自动循环任务
        ServiceImplHeart_EventJustRun();
    }
}

//模块读取指定输入
LH_ERR ServiceImplModuleInputRead(uint8_t index,uint8_t indexMax,MODULE_INPUT_MAP_UNIT* arrayPtr,RAIL_SENSOR_STATE* state)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction inputState;
    //序号错误
    if(index >= indexMax)
    {
        return LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX;
    }
    //读取输入
    errorCode = SystemInputRead(arrayPtr[index].inputIndex,&inputState);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //根据读取到的结果转换成有效无效变量
    (inputState == arrayPtr[index].validState)?(*state = RAIL_SENSOR_VALID):(*state = RAIL_SENSOR_INVALID);
    return LH_ERR_NONE;
}

//模块写入指定输出
LH_ERR ServiceImplModuleOutputWrite(uint8_t index,RAIL_OUTPUT_STATE state,uint8_t indexMax,MODULE_OUTPUT_MAP_UNIT* arrayPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction outputState;
    //序号错误
    if(index >= indexMax)
    {
        return LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX;
    }
    //根据配置文件转换出真正要写入的状态
    (state == RAIL_OUTPUT_STATE_VALID)?(outputState = arrayPtr[index].validState):(outputState = (BitAction)(!(arrayPtr[index].validState)));
    //写入
    errorCode = SystemOutputWrite(arrayPtr[index].outputIndex,outputState);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    return LH_ERR_NONE;
}

//模块指定电机复位
LH_ERR ServiceImplModuleMotorReset(uint8_t index,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,uint8_t indexMax,MODULE_STEP_MOTOR_MAP_UNIT* arrayPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //序号错误
    if(index >= indexMax)
    {
        return LH_ERR_BOARD_MAIN_COMM_SM_INDEX;
    }
    //调用复位,必须有零位偏移
    errorCode = arrayPtr[index].resetFuncPtr(resetCorrectFlag);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    return LH_ERR_NONE;
}

//模块指定电机走位
LH_ERR ServiceImplModuleMotorRunSteps(uint8_t index,uint8_t dir,uint32_t steps,uint8_t indexMax,int32_t* posAfterRun,MODULE_STEP_MOTOR_MAP_UNIT* arrayPtr)
{   
    LH_ERR errCode = LH_ERR_NONE;
    
    //走位变量
    SM_RUN_STEP_CMD smRunStepCmd;
    Can2SubSM_RunStepsCmdDataStructInit(&smRunStepCmd);
    //序号错误
    if(index >= indexMax)
    {
        return LH_ERR_BOARD_MAIN_COMM_SM_INDEX;
    }
    //电机序号
    smRunStepCmd.stepMotorIndex = arrayPtr[index].motorIndex;
    //0为负向,1为正向
    smRunStepCmd.steps = steps;
    if(dir == 0)
    {
        smRunStepCmd.steps = 0 - smRunStepCmd.steps;
    }
    //运行指定步数
    errCode = Can2SubSM_RunSpecialStepsWhileReturn(&smRunStepCmd);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //读取当前步数
    errCode = Can2SubSM_ReadPositionWhileReturn(smRunStepCmd.stepMotorIndex,posAfterRun);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //完成返回
    return LH_ERR_NONE;
}
