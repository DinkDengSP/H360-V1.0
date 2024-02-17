/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:23:35
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-02 15:41:03
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplRailHand.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplRailHand.h"
#include "ServiceIpcRailPushOut1.h"
#include "ServiceIpcRailPushOut2.h"

//自动运行任务的周期,单位毫秒
#define RAIL_HAND_EVENT_PERIOD_TIME_MS          200
//自动运行任务的触发次数,决定是否执行任务
static uint32_t countRailHandEventTrigCount = 0;

//只需要执行的事件回调函数,不需要计算时间
static void ServiceImplRailHand_EventJustRun(void)
{
    //读取状态,如果状态忙,切换为状态空闲
    if(RAIL_HAND_STATE_BUSY == ServiceStateRailHandReadHandModuleState())
    {
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    }
}

//自动循环事件初始化
void ServiceImplRailHand_EventInit(void)
{
    //计数器
        countRailHandEventTrigCount = 0;
    //设置状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
}

//自动循环事件
void ServiceImplRailHand_EventRun(uint32_t periodTimeMs)
{
    //周期检查
        if(periodTimeMs == 0)
        {
            return;
        }
    //心跳时间计时器增加
        countRailHandEventTrigCount++;
    //如果执行周期大于事件周期,立即运行,如果计数器事件次数大于事件次数,也执行
        if((periodTimeMs >= RAIL_HAND_EVENT_PERIOD_TIME_MS)||(countRailHandEventTrigCount >= (uint32_t)(RAIL_HAND_EVENT_PERIOD_TIME_MS/periodTimeMs)))
        {
            countRailHandEventTrigCount = 0;
            //执行一次自动循环任务
            ServiceImplRailHand_EventJustRun();
        }
}

/*************************************机械手主控自身使用的指令*****************************************/
//机械手旋转复位
LH_ERR ServiceImplRailHandRotateReset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //复位
        errorCode = Can2SubRailHandResetRotateWhileReturn(resetFlag);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RESET_ROTATE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手前后复位
LH_ERR ServiceImplRailHandBackFrontReset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //复位
        errorCode = Can2SubRailHandResetBackFrontWhileReturn(resetFlag);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RESET_BACKFRONT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    //设置机械手爪手收回
        ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE_BACK);
    return errorCode;
}

//机械手升降复位
LH_ERR ServiceImplRailHandUpDownReset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //复位
        errorCode = Can2SubRailHandResetUpDownWhileReturn(resetFlag);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RESET_UPDOWN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手三维复位
LH_ERR ServiceImplRailHandModuleReset(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //复位
        errorCode = Can2SubRailHandResetAllWhileReturn();
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RESET;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    //设置机械手爪手收回
        ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE_BACK);
    return errorCode;
}

//机械手指定电机复位
LH_ERR ServiceImplRailHandMotorReset(uint8_t motorIndex,RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
     //复位
        errorCode = ServiceImplRailHandModuleStepMotorReset((MOTOR_HAND_NAME)(motorIndex),resetFlag,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手指定电机走步数
LH_ERR ServiceImplRailHandStepMotorRunSteps(uint8_t motorIndex,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
     //走步数
        errorCode = ServiceImplRailHandModuleStepMotorRunSteps((MOTOR_HAND_NAME)(motorIndex),dir,steps,posAfterRun,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手读取指定端口
LH_ERR ServiceImplRailHandInputRead(uint8_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //读取模块输入
        errorCode = ServiceImplRailHandModuleInputRead((INPUT_HAND_NAME)pinIndex,&inputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //转换结果
        *pinValue = (RAIL_SENSOR_VALID == inputState)?1:0;
    return errorCode;
}

//机械手写入指定端口
LH_ERR ServiceImplRailHandOutPutWrite(uint8_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_OUTPUT_STATE outputState;
    //设置写入状态
        outputState = (pinValue == 0)?RAIL_OUTPUT_STATE_INVALID:RAIL_OUTPUT_STATE_VALID;
    //写入模块输出
        errorCode = ServiceImplRailHandModuleOutputWrite((OUTPUT_HAND_NAME)pinIndex,outputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    return errorCode;
}

//机械手抓取进样1
LH_ERR ServiceImplRailHandCatchPushIn1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandCatchPushIn1WhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_CATCH;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手抓取进样2
LH_ERR ServiceImplRailHandCatchPushIn2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandCatchPushIn2WhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_CATCH;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

// //机械手放下出样1
// LH_ERR ServiceImplRailHandPutPushOut1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
// {
//     LH_ERR errorCode = LH_ERR_NONE;
//     //初始化无错误
//         *can1ErrorPtr = CAN1_REPORT_OK;
//         *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
//     //机械手方向与机械手上架子状态
//         HAND_DIR handCurrentDir;
//         SENSOR_STATE_HAND_RACK_EXIST handRackExist;
//     //设置机械手状态忙
//         ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
//     //函数调度
//         errorCode = Can2SubRailHandPutPushOut1WhileReturn(&handCurrentDir,&handRackExist);
//         if(errorCode != LH_ERR_NONE)
//         {
//             //错误代码打印
//             SystemDebugShowMsgError(errorCode);
//             *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_PUT;
//             *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
//             return errorCode;
//         }
//     //设置机械手方向和机械手上是否有架子
//         ServiceStateRailHandWriteCurrentDir(handCurrentDir);
//         ServiceStateRailHandWriteHandRackExist(handRackExist);
//     //设置机械手状态空闲
//         ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
//     return errorCode;
// }

// //机械手放下出样2
// LH_ERR ServiceImplRailHandPutPushOut2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
// {
//     LH_ERR errorCode = LH_ERR_NONE;
//     //初始化无错误
//         *can1ErrorPtr = CAN1_REPORT_OK;
//         *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
//     //机械手方向与机械手上架子状态
//         HAND_DIR handCurrentDir;
//         SENSOR_STATE_HAND_RACK_EXIST handRackExist;
//     //设置机械手状态忙
//         ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
//     //函数调度
//         errorCode = Can2SubRailHandPutPushOut2WhileReturn(&handCurrentDir,&handRackExist);
//         if(errorCode != LH_ERR_NONE)
//         {
//             //错误代码打印
//             SystemDebugShowMsgError(errorCode);
//             *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_PUT;
//             *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
//             return errorCode;
//         }
//     //设置机械手方向和机械手上是否有架子
//         ServiceStateRailHandWriteCurrentDir(handCurrentDir);
//         ServiceStateRailHandWriteHandRackExist(handRackExist);
//     //设置机械手状态空闲
//         ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
//     return errorCode;
// }

//机械手放下出样1
LH_ERR ServiceImplRailHandPutPushOut1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    LH_ERR errorCode1 = LH_ERR_NONE;
    //初始化无错误R
        CAN1_REPORT_ERR_CODE  can1ErrorCode= CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL  can1ErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
        SENSOR_STATE_HAND_RACK_EXIST handRackExistBefore;
    //获取机械手放架子前状态
        errorCode = ServiceImplRailHandGetState(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
        handRackExistBefore = ServiceStateRailHandReadHandRackExist();
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度，机械手夹爪出样1放架子
        errorCode = Can2SubRailHandClawReachPushOut1WhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_HAND_REACH_PUSHOUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手爪手伸出
        ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE_FRONT);
    //放下前有架子，放下后无架子报错
        if(handRackExistBefore == SENSOR_STATE_HAND_RACK_EXIST_TRIG && handRackExist != SENSOR_STATE_HAND_RACK_EXIST_TRIG)
        {
            //错误代码打印
            errorCode = LH_ERR_BOARD_MAIN_RAIL_HAND_PUSH_OUT_PUT_RACK_DOWN_RACK_LOSS;
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_HAND_REACH_PUSHOUT_RACK_LOSS;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //延时等待一段时间
        CoreDelayMs(500);
    //出样1推动一次,保存错误代码
        errorCode1 = IPC_RailPushOut1PushOnceWithReturnZeroWhileReturn(&can1ErrorCode,&can1ErrorLevel);
    //延时等待一段时间
        CoreDelayMs(100);
    //获取机械手推架子后状态
        errorCode = ServiceImplRailHandGetState(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
        handRackExist = ServiceStateRailHandReadHandRackExist();
    //放下前有架子，推架子后有架子报错
        if(handRackExistBefore == SENSOR_STATE_HAND_RACK_EXIST_TRIG && handRackExist == SENSOR_STATE_HAND_RACK_EXIST_TRIG)
        {
            //错误代码打印
            errorCode = LH_ERR_BOARD_MAIN_RAIL_HAND_RACK_EXIST_AFTER_PUSH_OUT_PUSH_ONCE;
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_EXIST_AFTER_PUT_PUSH_OUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //机械手爪手回收
        errorCode = Can2SubRailHandTakeClawBackWhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_TAKE_HAND_BACK;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手爪手收回
        ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE_BACK);
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    //放架前有架子，放架后还有架子，放架失败
        if(handRackExistBefore == SENSOR_STATE_HAND_RACK_EXIST_TRIG && handRackExist == SENSOR_STATE_HAND_RACK_EXIST_TRIG)
        {
            //错误代码打印
            errorCode = LH_ERR_BOARD_MAIN_RAIL_INTEGRATED_RACK_EXIST_AFTER_PUT;
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_EXIST_AFTER_PUT_PUSH_OUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //到此机械手无错误，上报出样的错误
    if(errorCode1 != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode1);
        *can1ErrorPtr = can1ErrorCode;
        *reportLevelResultPtr = can1ErrorLevel;
        return errorCode1;  
    }
    return errorCode;
}

//机械手放下出样2
LH_ERR ServiceImplRailHandPutPushOut2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    LH_ERR errorCode1 = LH_ERR_NONE;
    //初始化无错误
        CAN1_REPORT_ERR_CODE  can1ErrorCode= CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL  can1ErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
        SENSOR_STATE_HAND_RACK_EXIST handRackExistBefore;
    //获取机械手放架子前状态
        errorCode = ServiceImplRailHandGetState(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
        handRackExistBefore = ServiceStateRailHandReadHandRackExist();
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度，机械手夹爪出样2放架子
        errorCode = Can2SubRailHandClawReachPushOut2WhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_HAND_REACH_PUSHOUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手爪手伸出
        ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE_FRONT);
    //放下前有架子，放下后无架子报错
        if(handRackExistBefore == SENSOR_STATE_HAND_RACK_EXIST_TRIG && handRackExist != SENSOR_STATE_HAND_RACK_EXIST_TRIG)
        {
            //错误代码打印
            errorCode = LH_ERR_BOARD_MAIN_RAIL_HAND_PUSH_OUT_PUT_RACK_DOWN_RACK_LOSS;
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_HAND_REACH_PUSHOUT_RACK_LOSS;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //延时等待一段时间
        CoreDelayMs(500);
    //出样2推动一次
        errorCode1 = IPC_RailPushOut2PushOnceWithReturnZeroWhileReturn(&can1ErrorCode,&can1ErrorLevel);
    //延时等待一段时间
        CoreDelayMs(100);
    //获取机械手推架子后状态
        errorCode = ServiceImplRailHandGetState(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
        handRackExist = ServiceStateRailHandReadHandRackExist();
    //放下前有架子，推架子后有架子报错
        if(handRackExistBefore == SENSOR_STATE_HAND_RACK_EXIST_TRIG && handRackExist == SENSOR_STATE_HAND_RACK_EXIST_TRIG)
        {
            //错误代码打印
            errorCode = LH_ERR_BOARD_MAIN_RAIL_HAND_RACK_EXIST_AFTER_PUSH_OUT_PUSH_ONCE;
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_EXIST_AFTER_PUT_PUSH_OUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //机械手爪手回收
        errorCode = Can2SubRailHandTakeClawBackWhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_TAKE_HAND_BACK;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手爪手收回
        ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE_BACK);
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    //放架前有架子，放架后还有架子，放架失败
        if(handRackExistBefore == SENSOR_STATE_HAND_RACK_EXIST_TRIG && handRackExist == SENSOR_STATE_HAND_RACK_EXIST_TRIG)
        {
            //错误代码打印
            errorCode = LH_ERR_BOARD_MAIN_RAIL_INTEGRATED_RACK_EXIST_AFTER_PUT;
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_EXIST_AFTER_PUT_PUSH_OUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //到此机械手无错误，上报出样的错误
        if(errorCode1 != LH_ERR_NONE)
        {
            SystemDebugShowMsgError(errorCode1);
            *can1ErrorPtr = can1ErrorCode;
            *reportLevelResultPtr = can1ErrorLevel;
            return errorCode1;  
        }
    return errorCode;
}

//机械手旋转进样方向
LH_ERR ServiceImplRailHandRotate2PushIn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandRotatePushInWhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_ROTATE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手旋转到急诊位方向
LH_ERR ServiceImplRailHandRotate2EmergencyCall(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandRotateEmergencyCallWhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_ROTATE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手旋转到指定方向
LH_ERR ServiceImplRailHandRotate2SpecialDir(HAND_DIR targetDir,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandRotateSpecialDirWhileReturn(targetDir,&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_ROTATE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手从急诊位抓取试管架
LH_ERR ServiceImplRailHandCatchEmergencyCall(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandCatchEmergencyCallWhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_CATCH;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手从指定模块待测位抓取试管架
LH_ERR ServiceImplRailHandCatchWaitTest(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandCatchWaitTestWhileReturn(moduleNo,&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_CATCH;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手往指定模块待测位放置试管架
LH_ERR ServiceImplRailHandPutWaitTest(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandPutWaitTestWhileReturn(moduleNo,&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_PUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手从指定模块重测位抓取试管架
LH_ERR ServiceImplRailHandCatchRepeatTest(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandCatchRepeatTestWhileReturn(moduleNo,&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_CATCH;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//机械手往指定模块重测位放置试管架
LH_ERR ServiceImplRailHandPutRepeatTest(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandPutRepeatTestWhileReturn(moduleNo,&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_PUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//获取机械手当前状态
LH_ERR ServiceImplRailHandGetState(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);

    //函数调度
        errorCode = Can2SubRailHandGetStateWhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_STATE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    return errorCode;
}

//轨道机械手夹爪到出样1
LH_ERR ServiceImplRailHandClawReachPushOut1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandClawReachPushOut1WhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_HAND_REACH_PUSHOUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    //设置机械手爪手伸出
        ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE_FRONT);
    return errorCode;
}

//轨道机械手夹爪到出样2
LH_ERR ServiceImplRailHandClawReachPushOut2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandClawReachPushOut2WhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_HAND_REACH_PUSHOUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    //设置机械手爪手伸出
        ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE_FRONT);
    return errorCode;
}

//轨道机械手收回夹爪
LH_ERR ServiceImplRailHandTakeClawBack(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手方向与机械手上架子状态
        HAND_DIR handCurrentDir;
        SENSOR_STATE_HAND_RACK_EXIST handRackExist;
    //设置机械手状态忙
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_BUSY);
    //函数调度
        errorCode = Can2SubRailHandTakeClawBackWhileReturn(&handCurrentDir,&handRackExist);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RACK_TAKE_HAND_BACK;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置机械手方向和机械手上是否有架子
        ServiceStateRailHandWriteCurrentDir(handCurrentDir);
        ServiceStateRailHandWriteHandRackExist(handRackExist);
    //设置机械手状态空闲
        ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE_IDLE);
    //设置机械手爪手收回
        ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE_BACK);
    return errorCode;
}