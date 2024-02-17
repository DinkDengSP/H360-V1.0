/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-10-11 14:04:59
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-07 11:29:31
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SrvImplLocalRackEntry0.h"

//有限状态机初始化
void SrvImplLocalRackEntry0FSM_Init(void)
{
    asm("nop");
}

//有限状态机运行
void SrvImplLocalRackEntry0FSM_Run(void)
{
    asm("nop");
}

//输入引脚配置
const static INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_RAIL_LOCAL_RACK_ENTRY0_INPUT_EXIST,          VAILD_INPUT_RAIL_LOCAL_RACK_ENTRY0_INPUT_EXIST          },//进样检测光电
    {INPUT_RAIL_LOCAL_RACK_ENTRY0_INPUT_MOTOR_ORIGIN,   VAILD_INPUT_RAIL_LOCAL_RACK_ENTRY0_INPUT_MOTOR_ORIGIN   },//进样防夹电机原点
};
//输入引脚长度
#define LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY          (sizeof(inputSensorPinConfigArray)/sizeof(inputSensorPinConfigArray[0])) 

//输出引脚配置
const static OUTPUT_CTRL_PIN_CONFIG outputCtrlPinConfigArrary[] = {
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_START,            VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_START            },//进样轨道直流电机启停
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_DIR,              VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_DIR              },//进样轨道直流电机方向控制
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_SPEED,            VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_SPEED            },//进样轨道直流电机速度控制
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_EMSTOP,           VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_EMSTOP           },//进样轨道直流电机急停
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_ALARM_RELEASE,    VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_IN_ALARM_RELEASE    },//进样轨道直流电机报警解除
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_START,           VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_START           },//出样轨道直流电机启停
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_DIR,             VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_DIR             },//出样轨道直流电机方向控制
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_SPEED,           VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_SPEED           },//出样轨道直流电机速度控制
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_EMSTOP,          VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_EMSTOP          },//出样轨道直流电机急停
    {OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_ALARM_RELEASE,   VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY0_OUT_RAIL_OUT_ALARM_RELEASE   },//出样轨道直流电机报警解除
};
//输出引脚配置配置数据长度
#define LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY          (sizeof(outputCtrlPinConfigArrary)/sizeof(outputCtrlPinConfigArrary[0])) 


//读取指定输入
void SrvImplLocalRackEntry0ReadInput(uint8_t inputIndex,CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    //索引判断
    if((uint8_t)(inputIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_INPUT_READ_INDEX,CAN1_ERR_LEVEL_EMERGENCY_STOP,LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX);
        //错误代码转换
        return;
    }
    GPIO_PinState pinValue;
    //读引脚电平
    errorCode = SystemInputRead(inputSensorPinConfigArray[inputIndex].inputPinIndex,&pinValue);
    if(errorCode != LH_ERR_NONE)
    {
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_INPUT_READ,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    //有效电平判断
    if(pinValue == inputSensorPinConfigArray[inputIndex].pinVaildState)
    {
        localRackEntryStatePtr->readInputState = SENSOR_STATE_VALID;
    }
    else
    {
        localRackEntryStatePtr->readInputState = SENSOR_STATE_INVALID;
    }
    return;
}

//写出指定输出
void SrvImplLocalRackEntry0WriteOutput(uint8_t outputIndex,uint8_t setValue,CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    GPIO_PinState pinValue = GPIO_PIN_SET;
    //索引判断
    if((uint8_t)(outputIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_OUTPUT_WRITE_INDEX,CAN1_ERR_LEVEL_EMERGENCY_STOP,LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX);
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
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_OUTPUT_WRITE,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    return;
}

//电机复位
void SrvImplLocalRackEntry0StepMotorReset(uint8_t motorIndex,CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t posAfterReset = 0;
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    errorCode = Can2RailLocalRackEntryStepMotorReset((RAIL_LOCAL_RACK_ENTRY_SM_INDEX)motorIndex,&(posAfterReset));
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_MOTOR_RESET,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    localRackEntryStatePtr->stepMotorCurrentPos = posAfterReset;
    return;
}

//电机走步数
void SrvImplLocalRackEntry0StepMotorRunSteps(uint8_t motorIndex,int32_t steps,CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t posAfterReset = 0;
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    errorCode = Can2RailLocalRackEntryStepMotorRunSteps((RAIL_LOCAL_RACK_ENTRY_SM_INDEX)motorIndex,steps,&(posAfterReset));
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_MOTOR_RUN_STEPS,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    localRackEntryStatePtr->stepMotorCurrentPos = posAfterReset;
    return;
}

//模块复位
void SrvImplLocalRackEntry0Reset(CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    CAN2_SUB_STATE_RACK_ENTRY can2SubLocalRackEntry0State;
    errorCode = Can2RailLocalRackEntryReset(&can2SubLocalRackEntry0State);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_RESET,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    //获取到的数据赋值
    localRackEntryStatePtr->rackExistSensor = can2SubLocalRackEntry0State.rackEntrySensorSet.rackExistSensor;
    localRackEntryStatePtr->rackComeInEventFlag = can2SubLocalRackEntry0State.rackComeInEventFlag;
    localRackEntryStatePtr->nodeState = can2SubLocalRackEntry0State.nodeState;
    return;
}

//节点卡住试管架
void SrvImplLocalRackEntry0SwitchBlock(CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    CAN2_SUB_STATE_RACK_ENTRY can2SubLocalRackEntry0State;
    errorCode = Can2RailLocalRackEntrySwitchBlock(&can2SubLocalRackEntry0State);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_SWITCH_BLOCK,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    //获取到的数据赋值
    localRackEntryStatePtr->rackExistSensor = can2SubLocalRackEntry0State.rackEntrySensorSet.rackExistSensor;
    localRackEntryStatePtr->rackComeInEventFlag = can2SubLocalRackEntry0State.rackComeInEventFlag;
    localRackEntryStatePtr->nodeState = can2SubLocalRackEntry0State.nodeState;
    return;
}

//节点释放试管架
void SrvImplLocalRackEntry0SwitchRelease(CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    CAN2_SUB_STATE_RACK_ENTRY can2SubLocalRackEntry0State;
    errorCode = Can2RailLocalRackEntrySwitchRelease(&can2SubLocalRackEntry0State);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_SWITCH_RELEASE,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    //获取到的数据赋值
    localRackEntryStatePtr->rackExistSensor = can2SubLocalRackEntry0State.rackEntrySensorSet.rackExistSensor;
    localRackEntryStatePtr->rackComeInEventFlag = can2SubLocalRackEntry0State.rackComeInEventFlag;
    localRackEntryStatePtr->nodeState = can2SubLocalRackEntry0State.nodeState;
    return;
}

//节点放行
void SrvImplLocalRackEntry0ReleaseRack(CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    CAN2_SUB_STATE_RACK_ENTRY can2SubLocalRackEntry0State;
    errorCode = Can2RailLocalRackEntryReleaseRack(&can2SubLocalRackEntry0State);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_RELEASE_RACK,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    //获取到的数据赋值
    localRackEntryStatePtr->rackExistSensor = can2SubLocalRackEntry0State.rackEntrySensorSet.rackExistSensor;
    localRackEntryStatePtr->rackComeInEventFlag = can2SubLocalRackEntry0State.rackComeInEventFlag;
    localRackEntryStatePtr->nodeState = can2SubLocalRackEntry0State.nodeState;
    return;
}

//轨道启动
void SrvImplLocalRackEntry0RailStart(LOCAL_RAIL_DIR railInDir,LOCAL_RAIL_DIR railOutDir,CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    CAN2_SUB_STATE_RACK_ENTRY can2SubLocalRackEntry0State;
    errorCode = Can2RailLocalRackEntryRailStart(railInDir,railOutDir,&can2SubLocalRackEntry0State);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_RAIL_START,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    //获取到的数据赋值
    localRackEntryStatePtr->rackExistSensor = can2SubLocalRackEntry0State.rackEntrySensorSet.rackExistSensor;
    localRackEntryStatePtr->rackComeInEventFlag = can2SubLocalRackEntry0State.rackComeInEventFlag;
    localRackEntryStatePtr->nodeState = can2SubLocalRackEntry0State.nodeState;
    return;
}

//轨道停止
void SrvImplLocalRackEntry0RailStop(CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    CAN2_SUB_STATE_RACK_ENTRY can2SubLocalRackEntry0State;
    errorCode = Can2RailLocalRackEntryRailStop(&can2SubLocalRackEntry0State);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_RAIL_STOP,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    //获取到的数据赋值
    localRackEntryStatePtr->rackExistSensor = can2SubLocalRackEntry0State.rackEntrySensorSet.rackExistSensor;
    localRackEntryStatePtr->rackComeInEventFlag = can2SubLocalRackEntry0State.rackComeInEventFlag;
    localRackEntryStatePtr->nodeState = can2SubLocalRackEntry0State.nodeState;
    return;
}

//读取状态
void SrvImplLocalRackEntry0ReadState(CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    CAN2_SUB_STATE_RACK_ENTRY can2SubLocalRackEntry0State;
    errorCode = Can2RailLocalRackEntryReadState(&can2SubLocalRackEntry0State);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_READ_STATE,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    //获取到的数据赋值
    localRackEntryStatePtr->rackExistSensor = can2SubLocalRackEntry0State.rackEntrySensorSet.rackExistSensor;
    localRackEntryStatePtr->rackComeInEventFlag = can2SubLocalRackEntry0State.rackComeInEventFlag;
    localRackEntryStatePtr->nodeState = can2SubLocalRackEntry0State.nodeState;
    return;
}

//清除状态
void SrvImplLocalRackEntry0ClearState(CAN1_ERR_SET* errorSet)
{
    LH_ERR errorCode = LH_ERR_NONE;
    CAN2_SUB_STATE_RACK_ENTRY can2SubLocalRackEntry0State;
    //先清除状态
    errorCode = Can2RailLocalRackEntryClearState();
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_CLEAR_STATE,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    //后续在读取状态
    errorCode = Can2RailLocalRackEntryReadState(&can2SubLocalRackEntry0State);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        ImplBaseSetErrorSetMsgMacro(errorSet,CAN1_ERR_LOCAL_RACK_ENTRY_READ_STATE,CAN1_ERR_LEVEL_EMERGENCY_STOP,errorCode);
        return;
    }
    SRV_STATE_LOCAL_RACK_ENTRY* localRackEntryStatePtr = SrvStateGetPtrLocalRackEntry();
    //获取到的数据赋值
    localRackEntryStatePtr->rackExistSensor = can2SubLocalRackEntry0State.rackEntrySensorSet.rackExistSensor;
    localRackEntryStatePtr->rackComeInEventFlag = can2SubLocalRackEntry0State.rackComeInEventFlag;
    localRackEntryStatePtr->nodeState = can2SubLocalRackEntry0State.nodeState;
    return;
}
