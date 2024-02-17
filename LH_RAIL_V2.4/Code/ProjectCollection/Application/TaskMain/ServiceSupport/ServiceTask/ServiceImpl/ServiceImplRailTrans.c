/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:24:02
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-09 09:53:41
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplRailTrans.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplRailTrans.h"
#include "ServiceIpcBarScan.h"

//自动运行任务的周期,单位毫秒
#define RAIL_TRANS_EVENT_PERIOD_TIME_MS          200
//自动运行任务的触发次数,决定是否执行任务
static uint32_t countRailTransEventTrigCount = 0;
//记录急诊位试管架状态
static EMERGENCY_REGION_RACK_STATE lastEmergencyRegionRackState = EMERGENCY_REGION_RACK_NOT_EXIST;
static EMERGENCY_REGION_RACK_STATE currentEmergencyRegionRackState = EMERGENCY_REGION_RACK_NOT_EXIST;

static void ServiceImplRailTrans_StateInit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    EMERGENCY_REGION_RACK_STATE localEmergencyRackState = EMERGENCY_REGION_RACK_NOT_EXIST;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置位置状态
        ServiceStateRailTransWriteMainPos(RAIL_MODULE_POS_COMMON);
        ServiceStateRailTransWriteSubPos(RAIL_SUB_MODULE_COMMON_UNKNOW);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    //读取急诊样本架存在传感器
        errorCode = ServiceImplRailTransModuleInputRead(EMERGENCY_CALL_EXIST,&inputState,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStateRailTransWriteEmergencyCallRackExist(inputState);
    //读取急诊样本架仓关闭到位传感器
        errorCode = ServiceImplRailTransModuleInputRead(EMERGENCY_CALL_IN_PLACE,&inputState,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStateRailTransWriteEmergencyCallDoorClose(inputState);
    //判定状态,控制灯
        if((RAIL_SENSOR_VALID == ServiceStateRailTransReadEmergencyCallDoorClose())&&(RAIL_SENSOR_VALID == ServiceStateRailTransReadEmergencyCallRackExist()))
        {
            //黄灯亮,表示检测到有效的试管架
            ServiceImplRailTransModuleOutputWrite(EMERGENCY_LED_YELLOW,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
            ServiceImplRailTransModuleOutputWrite(EMERGENCY_LED_GREEN,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
            //初始化试管架的状态为存在
            localEmergencyRackState = EMERGENCY_REGION_RACK_EXIST;
        }
        else
        {
            //绿灯亮,表示没有有效的试管架
            ServiceImplRailTransModuleOutputWrite(EMERGENCY_LED_YELLOW,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
            ServiceImplRailTransModuleOutputWrite(EMERGENCY_LED_GREEN,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
            localEmergencyRackState = EMERGENCY_REGION_RACK_NOT_EXIST;
        }
    //设置初始化试管架状态
        lastEmergencyRegionRackState = localEmergencyRackState;
        currentEmergencyRegionRackState = localEmergencyRackState;
}
 
//只需要执行的事件回调函数,不需要计算时间
static void ServiceImplRailTrans_EventStateRun(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    EMERGENCY_REGION_RACK_STATE localEmergencyRackState = EMERGENCY_REGION_RACK_NOT_EXIST;
    //初始化无错误
        CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //如果轨道正忙,设置轨道状态空闲
        if(RAIL_TRANS_STATE_BUSY == ServiceStateRailTransReadModuleState())
        {
            ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
        }
    //读取急诊样本架存在传感器
        errorCode = ServiceImplRailTransModuleInputRead(EMERGENCY_CALL_EXIST,&inputState,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStateRailTransWriteEmergencyCallRackExist(inputState);
    //读取急诊样本架仓关闭到位传感器
        errorCode = ServiceImplRailTransModuleInputRead(EMERGENCY_CALL_IN_PLACE,&inputState,&can1Error,&reportLevelResult);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return;
        }
        ServiceStateRailTransWriteEmergencyCallDoorClose(inputState);
    //判定状态,控制灯
        if((RAIL_SENSOR_VALID == ServiceStateRailTransReadEmergencyCallDoorClose())&&(RAIL_SENSOR_VALID == ServiceStateRailTransReadEmergencyCallRackExist()))
        {
            //黄灯亮,表示检测到有效的试管架
            ServiceImplRailTransModuleOutputWrite(EMERGENCY_LED_YELLOW,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
            ServiceImplRailTransModuleOutputWrite(EMERGENCY_LED_GREEN,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
            //当前试管架的状态为存在
            localEmergencyRackState = EMERGENCY_REGION_RACK_EXIST;
        }
        else
        {
            //绿灯亮,表示没有有效的试管架
            ServiceImplRailTransModuleOutputWrite(EMERGENCY_LED_YELLOW,RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
            ServiceImplRailTransModuleOutputWrite(EMERGENCY_LED_GREEN,RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
            //当前试管架状态为不存在
            localEmergencyRackState = EMERGENCY_REGION_RACK_NOT_EXIST;
        }
    //联合当前试管架的状态和以前的试管架的状态进行联合逻辑判定,确定是否上传急诊状态讯息
        currentEmergencyRegionRackState = localEmergencyRackState;
        //状态发生切换,状态没发生切换就不管
        if(currentEmergencyRegionRackState != lastEmergencyRegionRackState)
        {
            //处理之后的状态继承传递
            lastEmergencyRegionRackState = currentEmergencyRegionRackState;
            //如果是急诊有效的切换,上传状态
            if(currentEmergencyRegionRackState == EMERGENCY_REGION_RACK_EXIST)
            {
                //上传一个急诊位存在信息的打印
                Can1RailSendEmergencyRackExist();
            }
        }
}

//自动循环事件初始化
void ServiceImplRailTrans_EventInit(void)
{
    countRailTransEventTrigCount = 0;
    //状态初始化
    ServiceImplRailTrans_StateInit();
}  
//自动循环事件
void ServiceImplRailTrans_EventRun(uint32_t periodTimeMs)
{
    //周期计数
        if(periodTimeMs == 0)
        {
            return;
        }
    //心跳时间计时器增加
        countRailTransEventTrigCount++;
    //如果执行周期大于事件周期,立即运行,如果计数器事件次数大于事件次数,也执行
        if((periodTimeMs >= RAIL_TRANS_EVENT_PERIOD_TIME_MS)||(countRailTransEventTrigCount >= (uint32_t)(RAIL_TRANS_EVENT_PERIOD_TIME_MS/periodTimeMs)))
        {
            countRailTransEventTrigCount = 0;
            //执行一次自动循环任务
            ServiceImplRailTrans_EventStateRun();
        }
}



/*************************************轨道主控自身使用的指令*****************************************/
//轨道运转复位
LH_ERR ServiceImplRailTransReset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //复位
        errorCode = Can2SubRailTransResetWhileReturn(resetFlag,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RESET;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运转走步数
LH_ERR ServiceImplRailTransMotorRunSteps(uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //电机走位
        errorCode = ServiceImplRailTransModuleStepMotorRunSteps(MOTOR_TRANS,dir,steps,posAfterRun,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return LH_ERR_NONE;
}

//轨道运转读取指定端口
LH_ERR ServiceImplRailTransInputRead(uint8_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //读取模块输入
        errorCode = ServiceImplRailTransModuleInputRead((INPUT_TRANS_NAME)pinIndex,&inputState,can1ErrorPtr,reportLevelResultPtr);
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

//轨道运转写入指定端口
LH_ERR ServiceImplRailTransOutPutWrite(uint8_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_OUTPUT_STATE outputState;
    //设置写入状态
        outputState = (pinValue == 0)?RAIL_OUTPUT_STATE_INVALID:RAIL_OUTPUT_STATE_VALID;
    //写入模块输出
        errorCode = ServiceImplRailTransModuleOutputWrite((OUTPUT_TRANS_NAME)pinIndex,outputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    return errorCode;
}

//轨道运行到进样1
LH_ERR ServiceImplRailTransMove2PushIn1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2PushIn1WhileReturn(&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到进样2
LH_ERR ServiceImplRailTransMove2PushIn2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2PushIn2WhileReturn(&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到出样1
LH_ERR ServiceImplRailTransMove2PushOut1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
        if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
        {
            //夹爪未回收报错
            errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
            SystemDebugShowMsgError(errorCode);
            * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
            * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2PushOut1WhileReturn(&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到出样2
LH_ERR ServiceImplRailTransMove2PushOut2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2PushOut2WhileReturn(&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到急诊区
LH_ERR ServiceImplRailTransMove2EmergencyCall(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2EmergencyCallWhileReturn(&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到旋转位1
LH_ERR ServiceImplRailTransMove2Rotate1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2Rotate1WhileReturn(&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到旋转位2
LH_ERR ServiceImplRailTransMove2Rotate2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2Rotate2WhileReturn(&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到距离比较近的旋转位
LH_ERR ServiceImplRailTransMove2RotateAuto(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2RotateAutoWhileReturn(&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到试管架扫码位
LH_ERR ServiceImplRailTransMove2RackBarScan(RAIL_BAR_SCAN_POS barScanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2RackBarScanWhileReturn(barScanPos,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到指定试管扫码位
LH_ERR ServiceImplRailTransMove2TubeBarScan(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS barScanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2TubeBarScanWhileReturn(tubeIndex,barScanPos,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到指定模块待测位
LH_ERR ServiceImplRailTransMove2WaitTest(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2WaitTestWhileReturn(moduleNo,waitTestNo,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到指定模块重测位
LH_ERR ServiceImplRailTransMove2RepeatTest(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO repeatTestNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2RepeatTestWhileReturn(moduleNo,repeatTestNo,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到指定模块测试区放架位
LH_ERR ServiceImplRailTransMove2TestingPut(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2TestingPutWhileReturn(moduleNo,tubeIndex,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到指定模块测试区取架位
LH_ERR ServiceImplRailTransMove2TestingCatch(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2TestingCatchWhileReturn(moduleNo,tubeIndex,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//轨道运行到指定模块测试区吸样位
LH_ERR ServiceImplRailTransMove2TestingAbsorb(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //轨道运转
        errorCode = Can2SubRailTransMove2TestingAbsorbWhileReturn(moduleNo,tubeIndex,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return errorCode;
}

//获取急诊位状态
LH_ERR ServiceImplRailTransGetEmergencyState(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    RAIL_SENSOR_STATE inputState;
    //读取急诊样本架存在传感器
        errorCode = ServiceImplRailTransModuleInputRead(EMERGENCY_CALL_EXIST,&inputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
        ServiceStateRailTransWriteEmergencyCallRackExist(inputState);
    //读取急诊样本架仓关闭到位传感器
        errorCode = ServiceImplRailTransModuleInputRead(EMERGENCY_CALL_IN_PLACE,&inputState,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
        ServiceStateRailTransWriteEmergencyCallDoorClose(inputState);
    return LH_ERR_NONE;
}

//急诊电子锁锁定
LH_ERR ServiceImplRailTransLockSet(RAIL_OUTPUT_STATE lockSet,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //写入电磁铁状态
        errorCode = ServiceImplRailTransModuleOutputWrite(EMERGENCY_LOCK_CALL,lockSet,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//轨道集成扫码
LH_ERR ServiceImplRailTransMove2BarScanIntegrate(uint8_t* barScanResult,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t tubeBarExist = 0;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //清空集成扫码结果
        ServiceStateClearRailBarScanIntegrateResult();
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
    //关闭条码枪
        errorCode = IPC_RailBarScanCloseWhileReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //轨道运转
        errorCode = Can2SubRailTransMove2BarScanIntegrateWhileReturn(RAIL_BAR_SCAN_POS_START,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //打开条码枪
        errorCode = IPC_RailBarScanOpenWhileReturn(BARSCAN_OPEN_MODE_MULTI,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //轨道运转
        errorCode = Can2SubRailTransMove2BarScanIntegrateWhileReturn(RAIL_BAR_SCAN_POS_END,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //关闭条码枪
        errorCode = IPC_RailBarScanCloseWhileReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    //获取扫码结果
        errorCode = IPC_RailBarScanReadScanResultFromBufferWhileReturn(BARSCAN_INDEX_RACK,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //扫码长度
        uint16_t recvBarLength = ServiceStateRailBarScanReadDataLength();
        if(recvBarLength == 0)
        {
            
            return LH_ERR_NONE;
        }
    //获取缓存区
        uint8_t* recvBarBuffer = NULL;
        do
        {
            recvBarBuffer = UserMemMalloc(MEM_AXI_SRAM,recvBarLength);
            if(recvBarBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(recvBarBuffer == NULL);
    //读取数据
        ServiceStateRailBarScanReadData(recvBarLength,recvBarBuffer);
    //扫码结果转换
        uint16_t rackIDUtilCalc = 0;
        if(recvBarLength == 4)
        {
            //计算后续四个字节
            for(uint8_t index = 0; index < 4; index++)
            {
                //将不符合标准的计算到标准范围内
                if(recvBarBuffer[index] < '0')recvBarBuffer[index] = '0';
                if(recvBarBuffer[index] > '9')recvBarBuffer[index] = '9';
            }
            rackIDUtilCalc += (recvBarBuffer[3])- '0';
            rackIDUtilCalc += ((recvBarBuffer[2])- '0')*10;
            rackIDUtilCalc += ((recvBarBuffer[1])- '0')*100;
            rackIDUtilCalc += ((recvBarBuffer[0])- '0')*1000;
        }
        else
        {
            rackIDUtilCalc = 0XFFFF;
        }
    //释放内存
        UserMemFree(MEM_AXI_SRAM,recvBarBuffer);
        //转换试管架编码
        barScanResult[0] = (uint8_t)(rackIDUtilCalc);
        barScanResult[1] = (uint8_t)(rackIDUtilCalc>>8);

    //试管扫码辅助计数
        uint8_t indexUtil = 0;
    //获取试管条码
        for(indexUtil = 0; indexUtil < 5; indexUtil++)
        {
            //获取扫码结果
            errorCode = IPC_RailBarScanReadScanResultFromBufferWhileReturn((BARSCAN_INDEX)(BARSCAN_INDEX_TUBE1+indexUtil),can1ErrorPtr,reportLevelResultPtr);
            if(errorCode != LH_ERR_NONE)
            {
                SystemDebugShowMsgError(errorCode);
                return errorCode;
            }
            uint16_t recvBarLength = ServiceStateRailBarScanReadDataLength();
            if(recvBarLength != 0)
            {
                tubeBarExist |=(1<<indexUtil);
            }
            //写入试管条码
            if(recvBarLength > 32)
            {
                //读取数据
                ServiceStateRailBarScanReadData(32,barScanResult+2 +(indexUtil*32));
            }
            else
            {
                //读取数据
                ServiceStateRailBarScanReadData(recvBarLength,barScanResult+2 +(indexUtil*32));
            }
        }
        /* 保存集成扫码结果 */
        ServiceStateSetRailBarScanIntegrateResult(rackIDUtilCalc,tubeBarExist,(barScanResult+2));
    return LH_ERR_NONE;
}

//轨道运行到集成扫码指定位置-起始,结束
LH_ERR ServiceImplRailTransMove2BarScanPosIntegrate(RAIL_BAR_SCAN_POS scanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //轨道当前位置
        RAIL_MODULE_POS modulePos;
        RAIL_SUB_MODULE subRegionPos;
    //检查轨道机械手夹爪是否回收
    if(ServiceStateRailHandReadClawState() != RAIL_HAND_CLAW_STATE_BACK)
    {
        //夹爪未回收报错
        errorCode = LH_ERR_BOARD_MAIN_RAIL_TRANS_MOVE_RAIL_HAND_CLAW_STATE_FRONT;
        SystemDebugShowMsgError(errorCode);
        * can1ErrorPtr = CAN1_REPORT_ERR_CODE_RAIL_HAND_CLAW_STATE_FRONT;
        * reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        return errorCode;
    }
    //设置轨道状态忙
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_BUSY);
        if(scanPos != RAIL_BAR_SCAN_POS_END)
        {
            scanPos = RAIL_BAR_SCAN_POS_START;
        }
    //轨道运转
        errorCode = Can2SubRailTransMove2BarScanIntegrateWhileReturn(scanPos,&modulePos,&subRegionPos);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
   
    //设置轨道传送带位置
        ServiceStateRailTransWriteMainPos(modulePos);
        ServiceStateRailTransWriteSubPos(subRegionPos);
    //设置轨道状态空闲
        ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE_IDLE);
    return LH_ERR_NONE;
}
