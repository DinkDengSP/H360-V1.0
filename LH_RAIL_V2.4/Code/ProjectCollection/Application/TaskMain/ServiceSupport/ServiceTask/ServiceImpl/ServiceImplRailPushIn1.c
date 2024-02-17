/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:22:31
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-23 14:29:33
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplRailPushIn1.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplRailPushIn1.h"

//自动运行任务的周期,单位毫秒
#define RAIL_PUSH_IN1_EVENT_PERIOD_TIME_MS          200
//自动运行任务的触发次数,决定是否执行任务
static uint32_t countRailPushIn1EventTrigCount = 0;

/***********************************************************系统自动化运行的任务*********************************************************/
//进样1上一次按键状态
static RAIL_SENSOR_STATE pushin1KeySensorLastState;
//进样1本次按键状态
static RAIL_SENSOR_STATE pushin1KeySensorCurrentState;

//模块状态初始化
static void ServiceImplRailPushIn1_EventModuleStateInit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputStateDoor;
    RAIL_SENSOR_STATE inputStateTray;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //初始化先设置空闲状态
        ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
    //读取门状态
        errorCode = ServiceImplRailPushIn1ModuleInputRead(PUSHIN1_DOOR,&inputStateDoor,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStatePushIn1WriteDoorClose(inputStateDoor);
    //读取托盘状态
        errorCode = ServiceImplRailPushIn1ModuleInputRead(PUSHIN1_TRAY,&inputStateTray,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStatePushIn1WriteTrayExist(inputStateTray);
    //联合判定状态
        if((inputStateDoor != RAIL_SENSOR_VALID)&&(inputStateTray != RAIL_SENSOR_VALID))
        {
            //状态空闲,门开着,托盘不在
            ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_LOSS);
        }
        else if(inputStateDoor != RAIL_SENSOR_VALID)
        {
            //状态空闲,门开着,托盘在
            ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST);
        }
        else if(inputStateTray != RAIL_SENSOR_VALID)
        {
            //状态空闲,门关着,托盘不在
            ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS);
        }
        else
        {
            //状态空闲,门关着,托盘在位
            ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
        }
    //根据状态进行显示灯光设置
        if(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState())
        {
            //进样1空闲状态,黄灯关闭,绿灯打开
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_GREEN,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_YELLOW,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
        }
        else
        {
            //进样1其他状态,黄灯打开,绿灯关闭
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_GREEN,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_YELLOW,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        }
    //根据状态进行按键灯光设置,只要托盘在位,灯光就可以操作
        if((PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState())||
            (PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState()))
        {
            //按键灯打开
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_KEY_LIGHT,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        }
        else
        {
            //按键灯关闭
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_KEY_LIGHT,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
        }
    //设置上次状态和本次状态相同
        ServiceStatePushIn1WriteLastModuleState(ServiceStatePushIn1ReadCurrentModuleState());    
}

//按键状态初始化
static void ServiceImplRailPushIn1_EventKeyInit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读取按键状态
        errorCode = ServiceImplRailPushIn1ModuleInputRead(PUSHIN1_KEY,&inputState,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
    //初始化按键状态
        if(inputState != RAIL_SENSOR_VALID)
        {
            //当前状态和之前的状态同时设定
            pushin1KeySensorLastState = RAIL_SENSOR_INVALID;
            pushin1KeySensorCurrentState = RAIL_SENSOR_INVALID;
        }
        else
        {
            pushin1KeySensorLastState = RAIL_SENSOR_VALID;
            pushin1KeySensorCurrentState = RAIL_SENSOR_VALID;
        }
}

//模块状态切换事件
static void ServiceImplRailPushIn1_StateEventRun(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputStateDoor;
    RAIL_SENSOR_STATE inputStateTray;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //先设置空闲状态
        ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
    //读取门状态
        errorCode = ServiceImplRailPushIn1ModuleInputRead(PUSHIN1_DOOR,&inputStateDoor,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStatePushIn1WriteDoorClose(inputStateDoor);
    //读取托盘状态
        errorCode = ServiceImplRailPushIn1ModuleInputRead(PUSHIN1_TRAY,&inputStateTray,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStatePushIn1WriteTrayExist(inputStateTray);
    //联合判定状态
        if((inputStateDoor != RAIL_SENSOR_VALID)&&(inputStateTray != RAIL_SENSOR_VALID))
        {
            //状态空闲,门开着,托盘不在
            ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_LOSS);
        }
        else if(inputStateDoor != RAIL_SENSOR_VALID)
        {
            //状态空闲,门开着,托盘在
            ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST);
        }
        else if(inputStateTray != RAIL_SENSOR_VALID)
        {
            //状态空闲,门关着,托盘不在
            ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS);
        }
        else
        {
            //状态空闲,门关着,托盘在位
            ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
        }
    //根据状态设置灯光
        if(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState())
        {
            //进样1空闲状态,黄灯关闭,绿灯打开
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_GREEN,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_YELLOW,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
        }
        else
        {
            //进样1其他状态,黄灯打开,绿灯关闭
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_GREEN,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_YELLOW,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        }
    //根据状态设置按键灯
        //根据状态进行按键灯光设置,只要托盘在位,灯光就可以操作
        if((PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState())||
            (PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState()))
        {
            //按键灯打开
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_KEY_LIGHT,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        }
        else
        {
            //按键灯关闭
            ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_KEY_LIGHT,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
        }
    //上传状态变化
        if(ServiceStatePushIn1ReadCurrentModuleState() != ServiceStatePushIn1ReadLastModuleState())
        {
            if((PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_LOSS == ServiceStatePushIn1ReadCurrentModuleState())||
                (PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS == ServiceStatePushIn1ReadCurrentModuleState()))
            {
                //上传托盘丢失
                Can1RailSendRackRackIn1StateWithSelf(2);
            }
            else if(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState())
            {
                //上传门打开
                Can1RailSendRackRackIn1StateWithSelf(3);
            }
            else
            {
                //上传门空闲
                Can1RailSendRackRackIn1StateWithSelf(0);
            }
            //将上一次状态切换为本次状态
            ServiceStatePushIn1WriteLastModuleState(ServiceStatePushIn1ReadCurrentModuleState());
        }
}

//按键事件
static void ServiceImplRailPushIn1_KeyEventRun(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读取按键状态
        errorCode = ServiceImplRailPushIn1ModuleInputRead(PUSHIN1_KEY,&inputState,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
    //按键状态变化,检查当前状态与之前的状态是否一致
        if(inputState != pushin1KeySensorCurrentState)
        {
            //按键状态变化之后切换按键状态
            pushin1KeySensorLastState = pushin1KeySensorCurrentState;
            pushin1KeySensorCurrentState = inputState;
            //触发有效,当前状态有效,前面状态无效
            if((pushin1KeySensorLastState == RAIL_SENSOR_INVALID)&&(pushin1KeySensorCurrentState == RAIL_SENSOR_VALID))
            {
                //进样推进一次
                errorCode = ServiceImplRailPushIn1PushOnce(&can1Error,&reportLevelResult);
                return;
            }
        }
}


//自动循环事件初始化
void ServiceImplRailPushIn1_EventInit(void)
{
    //循环事件计数器
        countRailPushIn1EventTrigCount = 0;
    //模块状态初始化
        ServiceImplRailPushIn1_EventModuleStateInit();
    //按键状态初始化
        ServiceImplRailPushIn1_EventKeyInit();
}

//自动循环事件
void ServiceImplRailPushIn1_EventRun(uint32_t periodTimeMs)
{
    //周期检查
        if(periodTimeMs == 0)
        {
            return;
        }
    //心跳时间计时器增加
        countRailPushIn1EventTrigCount++;
    //如果执行周期大于事件周期,立即运行,如果计数器事件次数大于事件次数,也执行
        if((periodTimeMs >= RAIL_PUSH_IN1_EVENT_PERIOD_TIME_MS)||(countRailPushIn1EventTrigCount >= (uint32_t)(RAIL_PUSH_IN1_EVENT_PERIOD_TIME_MS/periodTimeMs)))
        {
            countRailPushIn1EventTrigCount = 0;
            //执行一次自动循环任务
            //模块状态切换事件
            ServiceImplRailPushIn1_StateEventRun();
            //按键事件
            ServiceImplRailPushIn1_KeyEventRun();
        }
}

//机构运动之前进行检查
static LH_ERR ServiceImplRailPushIn1CheckConditionBefore(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //检查托盘状态,托盘不在不能动作
        if((PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_LOSS == ServiceStatePushIn1ReadCurrentModuleState())||
            (PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS == ServiceStatePushIn1ReadCurrentModuleState()))
        {
            //托盘丢失
            errorCode = LH_ERR_BOARD_MAIN_RAIL_PUSH_IN1_TRAY_LOSS;
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_TRAY_LOSS;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errorCode;
        }
    //黄灯打开,绿灯关闭
        ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_GREEN,RAIL_OUTPUT_STATE_INVALID,can1ErrorPtr,reportLevelResultPtr);
        ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_YELLOW,RAIL_OUTPUT_STATE_VALID,can1ErrorPtr,reportLevelResultPtr);
    //按键灯关闭
        ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_KEY_LIGHT,RAIL_OUTPUT_STATE_INVALID,can1ErrorPtr,reportLevelResultPtr);
    //设置进样1处于忙状态
        ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_BUSY);
        ServiceStatePushIn1WriteLastModuleState(PUSH_IN_STATE_BUSY);
    //上传进样忙
        Can1RailSendRackRackIn1StateWithSelf(1);
    return errorCode;
}

//机构运动之后进行善后
static void ServiceImplRailPushIn1CheckConditionAfter(void)
{
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置空闲状态
    //黄灯关闭,绿灯打开
        ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_GREEN,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_YELLOW,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
    //按键灯打开
        ServiceImplRailPushIn1ModuleOutputWrite(PUSHIN1_KEY_LIGHT,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
    //设置进样1处于空闲状态
        ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
        ServiceStatePushIn1WriteLastModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
    //上传空闲状态
        Can1RailSendRackRackIn1StateWithSelf(0);
}

//进样1复位
LH_ERR ServiceImplRailPushIn1Reset(RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门和托盘的状态
        errorCode = ServiceImplRailPushIn1CheckConditionBefore(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //复位
        errorCode = ServiceImplRailPushIn1ModuleStepMotorReset(MOTOR_PUSH_IN1,resetCorrectFlag,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //完成动作的善后工作
        ServiceImplRailPushIn1CheckConditionAfter();
    return errorCode;
}

//进样1走步数
LH_ERR ServiceImplRailPushIn1MotorRunSteps(uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //检查门和托盘的状态
        errorCode = ServiceImplRailPushIn1CheckConditionBefore(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //电机走位
        errorCode = ServiceImplRailPushIn1ModuleStepMotorRunSteps(MOTOR_PUSH_IN1,dir,steps,posAfterRun,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //完成动作的善后工作
        ServiceImplRailPushIn1CheckConditionAfter();
    return LH_ERR_NONE;
}

//进样1读取指定端口
LH_ERR ServiceImplRailPushIn1InputRead(uint8_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //读取模块输入
        errorCode = ServiceImplRailPushIn1ModuleInputRead((INPUT_PUSHIN1_NAME)pinIndex,&inputState,can1ErrorPtr,reportLevelResultPtr);
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

//进样1写入指定端口
LH_ERR ServiceImplRailPushIn1OutPutWrite(uint8_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_OUTPUT_STATE outputState;
    //设置写入状态
        outputState = (pinValue == 0)?RAIL_OUTPUT_STATE_INVALID:RAIL_OUTPUT_STATE_VALID;
    //写入模块输出
        errorCode = ServiceImplRailPushIn1ModuleOutputWrite((OUTPUT_PUSHIN1_NAME)pinIndex,outputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    return errorCode;
}


//进样1推进一次
LH_ERR ServiceImplRailPushIn1PushOnce(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门和托盘的状态
        errorCode = ServiceImplRailPushIn1CheckConditionBefore(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //进样1次
        SENSOR_STATE_PUSHIN_EXIST pushInRackExistState;
        errorCode = Can2SubRailPushIn1PushOnceWhileReturn(&pushInRackExistState);
        if(errorCode != LH_ERR_NONE)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //运行的结果写入
        ServiceStatePushIn1WriteExist(pushInRackExistState);
    //完成动作的善后工作
        ServiceImplRailPushIn1CheckConditionAfter();
    return errorCode;
}

//进样1推进多次直到触发
LH_ERR ServiceImplRailPushIn1PushMultiWhileTrig(uint16_t retryMax,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门和托盘的状态
        errorCode = ServiceImplRailPushIn1CheckConditionBefore(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //进样多次直到触发
        SENSOR_STATE_PUSHIN_EXIST pushInRackExistState;
        errorCode = Can2SubRailPushIn1PushMultiWhileTrigWhileReturn(retryMax,&pushInRackExistState);
        if(errorCode != LH_ERR_NONE)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //结果写入
        ServiceStatePushIn1WriteExist(pushInRackExistState);
    //完成动作的善后工作
        ServiceImplRailPushIn1CheckConditionAfter();
    return errorCode;
}

//进样1获取传感器状态
LH_ERR ServiceImplRailPushIn1GetSensorState(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读取试管架到位IO口
        errorCode = ServiceImplRailPushIn1ModuleInputRead(PUSHIN1_RACK_IN_PLACE,&inputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
        ServiceStatePushIn1WriteInPlace(inputState);
    //读取托盘到位IO口
        errorCode = ServiceImplRailPushIn1ModuleInputRead(PUSHIN1_TRAY,&inputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
        ServiceStatePushIn1WriteTrayExist(inputState);
    //读取门开关IO口
        errorCode = ServiceImplRailPushIn1ModuleInputRead(PUSHIN1_DOOR,&inputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
        ServiceStatePushIn1WriteDoorClose(inputState);
    //返回结果
        return errorCode;
}

