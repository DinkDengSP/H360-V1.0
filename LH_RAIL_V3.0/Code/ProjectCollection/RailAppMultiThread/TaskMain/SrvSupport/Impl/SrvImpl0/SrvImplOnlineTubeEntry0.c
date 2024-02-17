/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-10-11 17:13:56
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-24 14:40:08
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SrvImplOnlineTubeEntry0.h"
#include "SrvImplOnlineUartRecv.h"

//有限状态机初始化
void SrvImplOnlineTubeEntry0FSM_Init(void)
{
    asm("nop");
}

//有限状态机运行
void SrvImplOnlineTubeEntry0FSM_Run(void)
{
    asm("nop");
}

//联机自检
void SrvImplOnlineTubeEntry0SelfCheck(CAN1_ERR_SET* errorSet)
{
    ACTION_BOARD_CURRENT_STATE onlineActBoardState;
    //首先读取当前板卡模式,本机状态
    errorSet->errorParam = Can2OtherReadCurrentState(BOARD_ID_RAIL_ONLINE,&(onlineActBoardState));
    if(errorSet->errorParam != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_HAND_SHAKE,CAN1_ERR_LEVEL_NORMAL_STOP,errorSet->errorParam);
        return;
    }
    //本机下位机板卡处于BOOT状态
    if(onlineActBoardState == ACTION_BOARD_CURRENT_STATE_BOOT)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_BOOT,CAN1_ERR_LEVEL_NORMAL_STOP,errorSet->errorParam);
        return;
    }
    return;
}

//输入引脚配置
const static INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_TUBE_ENTRY_INTRANCE_SENSOR,              VAILD_INPUT_TUBE_ENTRY_INTRANCE_SENSOR},//输入节点入口传感器
};
//输入引脚长度
#define LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY          (sizeof(inputSensorPinConfigArray)/sizeof(inputSensorPinConfigArray[0])) 

//输出引脚配置
const static OUTPUT_CTRL_PIN_CONFIG outputCtrlPinConfigArrary[] = {
    {OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_START,            VAILD_OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_START         },//进样轨道直流电机启停
    {OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_DIR,              VAILD_OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_DIR           },//进样轨道直流电机方向控制
    {OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_SPEED,            VAILD_OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_SPEED         },//进样轨道直流电机速度控制
    {OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_EMSTOP,           VAILD_OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_EMSTOP        },//进样轨道直流电机急停
    {OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_ALARM_RELEASE,    VAILD_OUTPUT_TUBE_ENTRY_RAIL_IN_OUTPUT_ALARM_RELEASE },//进样轨道直流电机报警解除
    {OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_START,           VAILD_OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_START        },//出样轨道直流电机使能
    {OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_DIR,             VAILD_OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_DIR          },//出样轨道直流电机方向控制
    {OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_SPEED,           VAILD_OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_SPEED        },//出样轨道直流电机速度控制
    {OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_EMSTOP,          VAILD_OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_EMSTOP       },//出样轨道直流电机急停
    {OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_ALARM_RELEASE,   VAILD_OUTPUT_TUBE_ENTRY_RAIL_OUT_OUTPUT_ALARM_RELEASE},//出样轨道直流电机报警解除
};
//输出引脚配置配置数据长度
#define LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY          (sizeof(outputCtrlPinConfigArrary)/sizeof(outputCtrlPinConfigArrary[0])) 


//读取输入
void SrvImplOnlineTubeEntry0ReadInput(uint8_t inputIndex,CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    SRV_STATE_ONLINE_TUBE_ENTRY* onlineTubeEntryStatePtr = SrvStateGetPtrOnlineTubeEntry();
    //索引判断
    if((uint8_t)(inputIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_INPUT_READ_INDEX,CAN1_ERR_LEVEL_EMERGENCY_STOP,LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX);
        //错误代码转换
        return;
    }
    GPIO_PinState pinValue;
    //读引脚电平
    errorCode = SystemInputRead(inputSensorPinConfigArray[inputIndex].inputPinIndex,&pinValue);
    if(errorCode != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_INPUT_READ,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    //有效电平判断
    if(pinValue == inputSensorPinConfigArray[inputIndex].pinVaildState)
    {
        onlineTubeEntryStatePtr->readInputState = SENSOR_STATE_VALID;
    }
    else
    {
        onlineTubeEntryStatePtr->readInputState = SENSOR_STATE_INVALID;
    }
    return;
}

//写入输出
void SrvImplOnlineTubeEntry0WriteOutput(uint8_t outputIndex,uint8_t setValue,CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    GPIO_PinState pinValue = GPIO_PIN_SET;
    //索引判断
    if((uint8_t)(outputIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_OUTPUT_WRITE_INDEX,CAN1_ERR_LEVEL_EMERGENCY_STOP,LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX);
        return;
    }
    //写出引脚电平
    if(setValue == OUT_STATE_ENABLE)
    {
        pinValue = outputCtrlPinConfigArrary[outputIndex].pinVaildState;
    }
    else
    {
        pinValue = (GPIO_PinState)!(outputCtrlPinConfigArrary[outputIndex].pinVaildState);
    }
    //写输出引脚
    errorCode = SystemOutputWrite(outputCtrlPinConfigArrary[outputIndex].outputPinIndex,pinValue);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_OUTPUT_WRITE,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    return;
}

//电机复位
void SrvImplOnlineTubeEntry0StepMotorReset(uint8_t motorIndex,CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t posAfterReset = 0;
    SRV_STATE_ONLINE_TUBE_ENTRY* onlineTubeEntryStatePtr = SrvStateGetPtrOnlineTubeEntry();
    errorCode = Can2RailOnlineTubeEntryStepMotorReset((RAIL_ONLINE_TUBE_ENTRY_SM_INDEX)motorIndex,&(posAfterReset));
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_MOTOR_RESET,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    onlineTubeEntryStatePtr->stepMotorCurrentPos = posAfterReset;
    return;
}

//电机走步数
void SrvImplOnlineTubeEntry0StepMotorRunSteps(uint8_t motorIndex,int32_t steps,CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t posAfterReset = 0;
    SRV_STATE_ONLINE_TUBE_ENTRY* onlineTubeEntryStatePtr = SrvStateGetPtrOnlineTubeEntry();
    errorCode = Can2RailOnlineTubeEntryStepMotorRunSteps((RAIL_ONLINE_TUBE_ENTRY_SM_INDEX)motorIndex,steps,&(posAfterReset));
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_CLAMP_MOTOR_RUN_STEPS,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    onlineTubeEntryStatePtr->stepMotorCurrentPos = posAfterReset;
    return;
}

//输入节点复位
void SrvImplOnlineTubeEntry0Reset(CAN1_ERR_SET* errorSet)
{
    LH_ERR localErrorParam = LH_ERR_NONE;
    CAN2_SUB_STATE_TUBE_ENTRY onlineTubeEntryState;
    localErrorParam = Can2RailOnlineTubeEntryReset(&onlineTubeEntryState);
    if(localErrorParam != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_RESET,CAN1_ERR_LEVEL_NORMAL_STOP,localErrorParam);
        return;
    }
    //将获取的数据存储
    SRV_STATE_ONLINE_TUBE_ENTRY* stateOnlineTuteEntryPtr = SrvStateGetPtrOnlineTubeEntry();
    stateOnlineTuteEntryPtr->intranceState = onlineTubeEntryState.tubeEntrySensorSet.intranceState;
    stateOnlineTuteEntryPtr->tubeEntryEventFlag = onlineTubeEntryState.tubeEntryEventFlag;
}

//清除节点状态
void SrvImplOnlineTubeEntry0ClearState(CAN1_ERR_SET* errorSet)
{
    LH_ERR localErrorParam = LH_ERR_NONE;
    localErrorParam = Can2RailOnlineTubeEntryClearState();
    if(localErrorParam != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_CLEAR_STATE,CAN1_ERR_LEVEL_NORMAL_STOP,localErrorParam);
        return;
    }
    SRV_STATE_ONLINE_TUBE_ENTRY* stateOnlineTuteEntryPtr = SrvStateGetPtrOnlineTubeEntry();
    stateOnlineTuteEntryPtr->tubeEntryEventFlag = EVENT_FLAG_NONE;
}

//读传感器节点信息
void SrvImplOnlineTubeEntry0ReadState(CAN1_ERR_SET* errorSet)
{
    LH_ERR localErrorParam = LH_ERR_NONE;
    CAN2_SUB_STATE_TUBE_ENTRY onlineTubeEntryState;
    localErrorParam = Can2RailOnlineTubeEntryReadState(&onlineTubeEntryState);
    if(localErrorParam != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_READ_STATE,CAN1_ERR_LEVEL_NORMAL_STOP,localErrorParam);
        return;
    }
    //将获取的事件存储
    SRV_STATE_ONLINE_TUBE_ENTRY* stateOnlineTuteEntryPtr = SrvStateGetPtrOnlineTubeEntry();
    stateOnlineTuteEntryPtr->intranceState = onlineTubeEntryState.tubeEntrySensorSet.intranceState;
    stateOnlineTuteEntryPtr->tubeEntryEventFlag = onlineTubeEntryState.tubeEntryEventFlag;
}

//轨道启动运转
void SrvImplOnlineTubeEntry0RailStart(CAN1_ERR_SET* errorSet)
{
    LH_ERR localErrorParam = LH_ERR_NONE;
    CAN2_SUB_STATE_TUBE_ENTRY onlineTubeEntryState;
    localErrorParam = Can2RailOnlineTubeEntryRailStart(&onlineTubeEntryState);
    if(localErrorParam != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_RAIL_START,CAN1_ERR_LEVEL_NORMAL_STOP,localErrorParam);
        return;
    }
    //将获取的事件存储
    SRV_STATE_ONLINE_TUBE_ENTRY* stateOnlineTuteEntryPtr = SrvStateGetPtrOnlineTubeEntry();
    stateOnlineTuteEntryPtr->intranceState = onlineTubeEntryState.tubeEntrySensorSet.intranceState;
    stateOnlineTuteEntryPtr->tubeEntryEventFlag = onlineTubeEntryState.tubeEntryEventFlag;
}

//轨道停止运转
void SrvImplOnlineTubeEntry0RailStop(CAN1_ERR_SET* errorSet)
{
    LH_ERR localErrorParam = LH_ERR_NONE;
    CAN2_SUB_STATE_TUBE_ENTRY onlineTubeEntryState;
    localErrorParam = Can2RailOnlineTubeEntryRailStop(&onlineTubeEntryState);
    if(localErrorParam != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_RAIL_STOP,CAN1_ERR_LEVEL_NORMAL_STOP,localErrorParam);
        return;
    }
    //将获取的事件存储
    SRV_STATE_ONLINE_TUBE_ENTRY* stateOnlineTuteEntryPtr = SrvStateGetPtrOnlineTubeEntry();
    stateOnlineTuteEntryPtr->intranceState = onlineTubeEntryState.tubeEntrySensorSet.intranceState;
    stateOnlineTuteEntryPtr->tubeEntryEventFlag = onlineTubeEntryState.tubeEntryEventFlag;
}

//查询流水线是否存在试管,发送指令
void SrvImplOnlineTubeEntry0QueryTube(CAN1_ERR_SET* errorSet)
{
    LH_ERR localErrorParam = LH_ERR_NONE;
    //发送指令
    localErrorParam = IPC_SrvOnlineUartMsgQueryTubeWhileReturn(&(errorSet->errorCode),&(errorSet->errorLevel));
    if(localErrorParam != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacroSample(errorSet,localErrorParam);
        return;
    }
    //发送收到ACK,等待完成
    SRV_STATE_ONLINE_UART* stateOnlineUartMsgPtr = SrvStateGetPtrOnlineUart();
    int32_t baseTimeStamp = tx_time_get();
    int32_t currentTimeStamp = baseTimeStamp;
    do
    {
        currentTimeStamp = tx_time_get();
        //检测超时
        if(currentTimeStamp - baseTimeStamp >= UART_CMD_RETURN_RESULT_TIME_MS)
        {
            break;
        }
        //检测接收
        if(stateOnlineUartMsgPtr->recvTubeStateFlag != 0)
        {
            break;
        }
        //延时
        CoreDelayMs(100);
    }while(currentTimeStamp - baseTimeStamp < UART_CMD_RETURN_RESULT_TIME_MS);
    //检测跳出原因
    if(stateOnlineUartMsgPtr->recvTubeStateFlag != 0)
    {
        //正常跳出.直接返回
        return;
    }
    ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_QUERY_TUBE,CAN1_ERR_LEVEL_WARNING,LH_ERR_UART_PROTOCOL_WAIT_RESULT_TIME_OUT);
    return;
}

//申请试管进入,发送指令
void SrvImplOnlineTubeEntry0ApplyHostTubeIn(CAN1_ERR_SET* errorSet)
{
    LH_ERR localErrorParam = LH_ERR_NONE;
    //发送指令
    localErrorParam = IPC_SrvOnlineUartMsgApplyHostTubeInWhileReturn(&(errorSet->errorCode),&(errorSet->errorLevel));
    if(localErrorParam != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacroSample(errorSet,localErrorParam);
        return;
    }
    //发送收到ACK,等待完成
    SRV_STATE_ONLINE_UART* stateOnlineUartMsgPtr = SrvStateGetPtrOnlineUart();
    int32_t baseTimeStamp = tx_time_get();
    int32_t currentTimeStamp = baseTimeStamp;
    do
    {
        currentTimeStamp = tx_time_get();
        //检测超时
        if(currentTimeStamp - baseTimeStamp >= UART_CMD_RETURN_RESULT_TIME_MS)
        {
            break;
        }
        //检测接收
        if(stateOnlineUartMsgPtr->recvTubeInFlag != 0)
        {
            break;
        }
        //延时
        CoreDelayMs(100);
    }while(currentTimeStamp - baseTimeStamp < UART_CMD_RETURN_RESULT_TIME_MS);
    //检测跳出原因
    if(stateOnlineUartMsgPtr->recvTubeInFlag != 0)
    {
        //正常跳出.直接返回
        return;
    }
    //异常跳出超时,返回错误
    ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_APPLY_HOST_TUBE_IN,CAN1_ERR_LEVEL_WARNING,LH_ERR_UART_PROTOCOL_WAIT_RESULT_TIME_OUT);
    return;
}


//读取转盘入口讯息
void SrvImplOnlineTubeEntry0ReadRotateInput(CAN1_ERR_SET* errorSet)
{
    LH_ERR localErrorParam = LH_ERR_NONE;
    SENSOR_STATE rotateInputSensorState;
    localErrorParam = Can2RailOnlineTubeEntryReadRotateInput(&rotateInputSensorState);
    if(localErrorParam != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_ONLINE_TUBE_ENTRY_READ_ROTATE_INPUT,CAN1_ERR_LEVEL_NORMAL_STOP,localErrorParam);
        return;
    }
    //将获取的事件存储
    SRV_STATE_ONLINE_TUBE_ENTRY* stateOnlineTuteEntryPtr = SrvStateGetPtrOnlineTubeEntry();
    stateOnlineTuteEntryPtr->readInputState = rotateInputSensorState;
}