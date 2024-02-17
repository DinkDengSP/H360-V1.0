/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:22:41
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-23 15:07:17
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplRailPushIn2.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplRailPushIn2.h"

//自动运行任务的周期,单位毫秒
#define RAIL_PUSH_IN2_EVENT_PERIOD_TIME_MS          200
//自动运行任务的触发次数,决定是否执行任务
static uint32_t countRailPushIn2EventTrigCount = 0;

/***********************************************************系统自动化运行的任务*********************************************************/
//进样2上一次按键状态
static RAIL_SENSOR_STATE pushin2KeySensorLastState;
//进样2本次按键状态
static RAIL_SENSOR_STATE pushin2KeySensorCurrentState;

//模块状态初始化
static void ServiceImplRailPushIn2_EventModuleStateInit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputStateDoor;
    RAIL_SENSOR_STATE inputStateTray;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //初始化先设置空闲状态
        ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
    //读取门状态
        errorCode = ServiceImplRailPushIn2ModuleInputRead(PUSHIN2_DOOR,&inputStateDoor,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStatePushIn2WriteDoorClose(inputStateDoor);
    //读取托盘状态
        errorCode = ServiceImplRailPushIn2ModuleInputRead(PUSHIN2_TRAY,&inputStateTray,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStatePushIn2WriteTrayExist(inputStateTray);
    //联合判定状态
        if((inputStateDoor != RAIL_SENSOR_VALID)&&(inputStateTray != RAIL_SENSOR_VALID))
        {
            //状态空闲,门开着,托盘不在
            ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_LOSS);
        }
        else if(inputStateDoor != RAIL_SENSOR_VALID)
        {
            //状态空闲,门开着,托盘在
            ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST);
        }
        else if(inputStateTray != RAIL_SENSOR_VALID)
        {
            //状态空闲,门关着,托盘不在
            ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS);
        }
        else
        {
            //状态空闲,门关着,托盘在位
            ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
        }
    //根据状态进行灯光设置
        if(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState())
        {
            //进样2空闲状态,黄灯关闭,绿灯打开
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_GREEN,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_YELLOW,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
        }
        else
        {
            //进样2其他状态,黄灯打开,绿灯关闭
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_GREEN,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_YELLOW,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        }
    //根据状态进行按键灯光设置,只要托盘在位,灯光就可以操作
        if((PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState())||
            (PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState()))
        {
            //按键灯打开
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_KEY_LIGHT,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        }
        else
        {
            //按键灯关闭
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_KEY_LIGHT,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
        }
    //设置上一次进样2默认状态
        ServiceStatePushIn2WriteLastModuleState(ServiceStatePushIn2ReadCurrentModuleState());
}

//按键状态初始化
static void ServiceImplRailPushIn2_EventKeyInit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读取按键状态
        errorCode = ServiceImplRailPushIn2ModuleInputRead(PUSHIN2_KEY,&inputState,&can1Error,&reportLevelResult);
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
            pushin2KeySensorLastState = RAIL_SENSOR_INVALID;
            pushin2KeySensorCurrentState = RAIL_SENSOR_INVALID;
        }
        else
        {
            pushin2KeySensorLastState = RAIL_SENSOR_VALID;
            pushin2KeySensorCurrentState = RAIL_SENSOR_VALID;
        }
}

//模块状态切换事件
static void ServiceImplRailPushIn2_StateEventRun(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputStateDoor;
    RAIL_SENSOR_STATE inputStateTray;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //先设置空闲状态
        ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
    //读取门状态
        errorCode = ServiceImplRailPushIn2ModuleInputRead(PUSHIN2_DOOR,&inputStateDoor,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStatePushIn2WriteDoorClose(inputStateDoor);
    //读取托盘状态
        errorCode = ServiceImplRailPushIn2ModuleInputRead(PUSHIN2_TRAY,&inputStateTray,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStatePushIn2WriteTrayExist(inputStateTray);
    //联合判定状态
        if((inputStateDoor != RAIL_SENSOR_VALID)&&(inputStateTray != RAIL_SENSOR_VALID))
        {
            //状态空闲,门开着,托盘不在
            ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_LOSS);
        }
        else if(inputStateDoor != RAIL_SENSOR_VALID)
        {
            //状态空闲,门开着,托盘在
            ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST);
        }
        else if(inputStateTray != RAIL_SENSOR_VALID)
        {
            //状态空闲,门关着,托盘不在
            ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS);
        }
        else
        {
            //状态空闲,门关着,托盘在位
            ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
        }
    //根据状态进行灯光设置
        if(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState())
        {
            //进样2空闲状态,黄灯关闭,绿灯打开
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_GREEN,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_YELLOW,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
        }
        else
        {
            //进样2其他状态,黄灯打开,绿灯关闭
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_GREEN,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_YELLOW,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        }
    //根据状态进行按键灯光设置,只要托盘在位,灯光就可以操作
        if((PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState())||
            (PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState()))
        {
            //按键灯打开
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_KEY_LIGHT,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        }
        else
        {
            //按键灯关闭
            ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_KEY_LIGHT,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
        }
    //进样2状态发生变化上传给上位机 -----add wangjing
        if(ServiceStatePushIn2ReadLastModuleState() != ServiceStatePushIn2ReadCurrentModuleState())
        {
            if((PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_LOSS == ServiceStatePushIn2ReadCurrentModuleState())||
                (PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS == ServiceStatePushIn2ReadCurrentModuleState()))
            {
                //上传托盘丢失
                Can1RailSendRackRackIn2StateWithSelf(2);
            }
            else if(PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState())
            {
                //上传门打开
                Can1RailSendRackRackIn2StateWithSelf(3);
            }
            else
            {
                //上传门空闲
                Can1RailSendRackRackIn2StateWithSelf(0);
            }
            //将上一次状态切换为本次状态
            ServiceStatePushIn2WriteLastModuleState(ServiceStatePushIn2ReadCurrentModuleState());
        }
}

//按键事件
static void ServiceImplRailPushIn2_KeyEventRun(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读取按键状态
        errorCode = ServiceImplRailPushIn2ModuleInputRead(PUSHIN2_KEY,&inputState,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
    //按键状态变化,检查当前状态与之前的状态是否一致
        if(inputState != pushin2KeySensorCurrentState)
        {
            //按键状态变化之后切换按键状态
            pushin2KeySensorLastState = pushin2KeySensorCurrentState;
            pushin2KeySensorCurrentState = inputState;
            //触发有效,当前状态有效,前面状态无效
            if((pushin2KeySensorLastState == RAIL_SENSOR_INVALID)&&(pushin2KeySensorCurrentState == RAIL_SENSOR_VALID))
            {
                //进样推进一次
                errorCode = ServiceImplRailPushIn2PushOnce(&can1Error,&reportLevelResult);
                if(errorCode != LH_ERR_NONE)
                {
                    //错误代码打印
                    SystemDebugShowMsgError(errorCode);
                }
                return;
            }
        }
}

//自动循环事件初始化
void ServiceImplRailPushIn2_EventInit(void)
{
    //循环基数
        countRailPushIn2EventTrigCount = 0;
    //模块状态初始化
        ServiceImplRailPushIn2_EventModuleStateInit();
    //按键状态初始化
        ServiceImplRailPushIn2_EventKeyInit();
}
//自动循环事件
void ServiceImplRailPushIn2_EventRun(uint32_t periodTimeMs)
{
    //计数器
        if(periodTimeMs == 0)
        {
            return;
        }
    //心跳时间计时器增加
        countRailPushIn2EventTrigCount++;
    //如果执行周期大于事件周期,立即运行,如果计数器事件次数大于事件次数,也执行
        if((periodTimeMs >= RAIL_PUSH_IN2_EVENT_PERIOD_TIME_MS)||(countRailPushIn2EventTrigCount >= (uint32_t)(RAIL_PUSH_IN2_EVENT_PERIOD_TIME_MS/periodTimeMs)))
        {
            countRailPushIn2EventTrigCount = 0;
            //执行一次自动循环任务
            //模块状态切换事件
            ServiceImplRailPushIn2_StateEventRun();
            //按键事件
            ServiceImplRailPushIn2_KeyEventRun();
        }
}


//机构运动之前进行检查
static LH_ERR ServiceImplRailPushIn2CheckConditionBefore(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //检查托盘状态,托盘不在不能动作
        if((PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_LOSS == ServiceStatePushIn2ReadCurrentModuleState())||
            (PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS == ServiceStatePushIn2ReadCurrentModuleState()))
        {
            //托盘丢失
            errorCode = LH_ERR_BOARD_MAIN_RAIL_PUSH_IN2_TRAY_LOSS;
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN2_TRAY_LOSS;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errorCode;
        }
    //黄灯打开,绿灯关闭
        ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_GREEN,RAIL_OUTPUT_STATE_INVALID,can1ErrorPtr,reportLevelResultPtr);
        ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_YELLOW,RAIL_OUTPUT_STATE_VALID,can1ErrorPtr,reportLevelResultPtr);
    //按键灯关闭
        ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_KEY_LIGHT,RAIL_OUTPUT_STATE_INVALID,can1ErrorPtr,reportLevelResultPtr);
    //设置进样2处于忙状态
        ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_BUSY);
        ServiceStatePushIn2WriteLastModuleState(PUSH_IN_STATE_BUSY);
    //上传进样忙
        Can1RailSendRackRackIn2StateWithSelf((uint8_t)1);
    return errorCode;
}

//机构运动之后进行善后
static void ServiceImplRailPushIn2CheckConditionAfter(void)
{
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置灯光和按键状态
    //黄灯关闭,绿灯打开
        ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_GREEN,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
        ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_YELLOW,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
    //按键灯打开
        ServiceImplRailPushIn2ModuleOutputWrite(PUSHIN2_KEY_LIGHT,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
    //设置进样2处于空闲状态
        ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
        ServiceStatePushIn2WriteLastModuleState(PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST);
    //上传空闲状态
        Can1RailSendRackRackIn2StateWithSelf(0);
}

/*************************************轨道主控自身使用的指令*****************************************/
//进样2复位
LH_ERR ServiceImplRailPushIn2Reset(RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门和托盘的状态
        errorCode = ServiceImplRailPushIn2CheckConditionBefore(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //复位
        errorCode = ServiceImplRailPushIn2ModuleStepMotorReset(MOTOR_PUSH_IN2,resetCorrectFlag,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //完成动作的善后工作
        ServiceImplRailPushIn2CheckConditionAfter();
    return errorCode;
}

//进样2走步数
LH_ERR ServiceImplRailPushIn2MotorRunSteps(uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //检查门和托盘的状态
        errorCode = ServiceImplRailPushIn2CheckConditionBefore(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //电机走位
        errorCode = ServiceImplRailPushIn2ModuleStepMotorRunSteps(MOTOR_PUSH_IN2,dir,steps,posAfterRun,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //完成动作的善后工作
        ServiceImplRailPushIn2CheckConditionAfter();
    return errorCode;
}

//进样2读取指定端口
LH_ERR ServiceImplRailPushIn2InputRead(uint8_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //读取模块输入
        errorCode = ServiceImplRailPushIn2ModuleInputRead((INPUT_PUSHIN2_NAME)pinIndex,&inputState,can1ErrorPtr,reportLevelResultPtr);
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

//进样2写入指定端口
LH_ERR ServiceImplRailPushIn2OutPutWrite(uint8_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_OUTPUT_STATE outputState;
    //设置写入状态
        outputState = (pinValue == 0)?RAIL_OUTPUT_STATE_INVALID:RAIL_OUTPUT_STATE_VALID;
    //写入模块输出
        errorCode = ServiceImplRailPushIn2ModuleOutputWrite((OUTPUT_PUSHIN2_NAME)pinIndex,outputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    return errorCode;
}


//进样2推进一次
LH_ERR ServiceImplRailPushIn2PushOnce(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门和托盘的状态
        errorCode = ServiceImplRailPushIn2CheckConditionBefore(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //进样2推进1次
        SENSOR_STATE_PUSHIN_EXIST pushInRackExistState;
        errorCode = Can2SubRailPushIn2PushOnceWhileReturn(&pushInRackExistState);
        if(errorCode != LH_ERR_NONE)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN2_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //上次运行的结果写入
        ServiceStatePushIn2WriteExist(pushInRackExistState);
    //完成动作的善后工作
        ServiceImplRailPushIn2CheckConditionAfter();
    return errorCode;
}

//进样2推进多次直到触发
LH_ERR ServiceImplRailPushIn2PushMultiWhileTrig(uint16_t retryMax,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门和托盘的状态
        errorCode = ServiceImplRailPushIn2CheckConditionBefore(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //进样多次直到触发
        SENSOR_STATE_PUSHIN_EXIST pushInRackExistState;
        errorCode = Can2SubRailPushIn2PushMultiWhileTrigWhileReturn(retryMax,&pushInRackExistState);
        if(errorCode != LH_ERR_NONE)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN2_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //到位状态检测
        ServiceStatePushIn2WriteExist(pushInRackExistState);
    //完成动作的善后工作
        ServiceImplRailPushIn2CheckConditionAfter();
    return errorCode;
}

//进样2获取传感器状态
LH_ERR ServiceImplRailPushIn2GetSensorState(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读取试管架到位IO口
        errorCode = ServiceImplRailPushIn2ModuleInputRead(PUSHIN2_RACK_IN_PLACE,&inputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
        ServiceStatePushIn2WriteInPlace(inputState);
    //读取托盘到位IO口
        errorCode = ServiceImplRailPushIn2ModuleInputRead(PUSHIN2_TRAY,&inputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
        ServiceStatePushIn2WriteTrayExist(inputState);
    //读取门开关IO口
        errorCode = ServiceImplRailPushIn2ModuleInputRead(PUSHIN2_DOOR,&inputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
        ServiceStatePushIn2WriteDoorClose(inputState);
    //返回结果
    return errorCode;
}
