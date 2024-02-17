/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-10 09:59:02
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-08 09:07:39
**FilePath: \H360_ActionBoard_ID_2d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceImpl\ServiceImplStepMotor9.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplStepMotor9.h"
#include "CommonServiceIpcBase.h"
#include "StepMotor.h"




//CAN总线接收数据处理,步进电机复位
LH_ERR ServiceImplStepMotor9ResetWithCanMsg(void* canMsgCachePtr)
{
    //结果代码
    LH_ERR errCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //步进电机复位结构体
    StepMotorResetCmd resetCmd;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorResetCmdParamFillWithCanMsg(&resetCmd,cachePtrLocal,STEP_MOTOR9);
    //调用复位算法
    errCode = StepMotorReset(&resetCmd);
    //返回结果代码
    return errCode;
}

//CAN总线接收数据处理,步进电机运动指定步数
LH_ERR ServiceImplStepMotor9RunSpecialStepsWithCanMsg(void* canMsgCachePtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,走位指令
    StepMotorRunStepsCmd stepMotorRunStepsCmd;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorRunSpecialStepsCmdParamFillWithCanMsg(&stepMotorRunStepsCmd,cachePtrLocal,STEP_MOTOR9);
    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckRunStepsVar(&stepMotorRunStepsCmd);
    if(errCode != LH_ERR_NONE)
    {
        //参数不合规,直接返回
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorRunSpecialSteps(&stepMotorRunStepsCmd);
    //返回结果代码
    return errCode;
}

//CAN总线接收数据处理,步进电机运动到指定坐标
LH_ERR ServiceImplStepMotor9RunToCoordinatesWithCanMsg(void* canMsgCachePtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,走到指定坐标指令
    StepMotorRun2CoordationCmd stepMotorRun2CoordationCmd;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorRunToCoordinatesCmdParamFillWithCanMsg(&stepMotorRun2CoordationCmd,cachePtrLocal,STEP_MOTOR9);
    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckRun2CoordationVar(&stepMotorRun2CoordationCmd);
    if(errCode != LH_ERR_NONE)
    {
        //参数不合规,直接返回
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorRun2SpecialCoordinates(&stepMotorRun2CoordationCmd);
    //返回结果代码
    return errCode;
}

//CAN总线接收数据处理,步进电机回零
LH_ERR ServiceImplStepMotor9ReturnZeroWithCanMsg(void* canMsgCachePtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,回零指令
    StepMotorReturnZeroCmd stepMotorReturnZeroCmd;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorReturnZeroCmdParamFillWithCanMsg(&stepMotorReturnZeroCmd,cachePtrLocal,STEP_MOTOR9);
    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckReturnZeroVar(&stepMotorReturnZeroCmd);
    if(errCode != LH_ERR_NONE)
    {
        //参数不合规,直接返回
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorReturnZeroWithSensor(&stepMotorReturnZeroCmd);
    //返回结果代码
    return errCode;
}

//CAN总线接收数据处理,步进电机回零不复位
LH_ERR ServiceImplStepMotor9ReturnZeroWithoutResetWithCanMsg(void* canMsgCachePtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,回零指令
    StepMotorReturnZeroCmd stepMotorReturnZeroCmd;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorReturnZeroWithOutResetCmdParamFillWithCanMsg(&stepMotorReturnZeroCmd,cachePtrLocal,STEP_MOTOR9);
    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckReturnZeroVar(&stepMotorReturnZeroCmd);
    if(errCode != LH_ERR_NONE)
    {
        //参数不合规,直接返回
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorReturnZeroWithSensorRemoveResetStage(&stepMotorReturnZeroCmd);
    //返回结果代码
    return errCode;
}

//CAN总线接收数据处理,步进电机持续运转不停止
LH_ERR ServiceImplStepMotor9RunAlwaysWithCanMsg(void* canMsgCachePtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,持续运转
    StepMotorRunAlwaysCmd stepMotorRunAlwaysCmd;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorRunAlwaysCmdParamFillWithCanMsg(&stepMotorRunAlwaysCmd,cachePtrLocal,STEP_MOTOR9); 

    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckRunAlways(&stepMotorRunAlwaysCmd);
    if(errCode != LH_ERR_NONE)
    {
        //参数不合规,直接返回
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorRunAlways(&stepMotorRunAlwaysCmd);
    //返回结果代码
    return errCode;
}

//CAN总线接收数据处理,步进电机立即停止,不带减速
LH_ERR ServiceImplStepMotor9StopImmediatelyWithCanMsg(void* canMsgCachePtr)
{
    return StepMotorStopImmediately(STEP_MOTOR9);
}

//CAN总线接收数据处理,步进电机减速停止,带减速
LH_ERR ServiceImplStepMotor9StopDecelerationWithCanMsg(void* canMsgCachePtr)
{
    return StepMotorStopDeceleration(STEP_MOTOR9);
}

//CAN总线接收数据处理,步进电机立即急停,除了复位,别的指令都会失效
LH_ERR ServiceImplStepMotor9StopEmergencyWithCanMsg(void* canMsgCachePtr)
{
    return StepMotorStopEmergency(STEP_MOTOR9);
}

//CAN总线接收数据处理,读取步进电机当前的坐标
LH_ERR ServiceImplStepMotor9ReadCurrentPositionWithCanMsg(void* canMsgCachePtr)
{
    //返回数据,申请内存
    uint8_t* dataBufferSend = NULL;
    //申请内存
    do
    {
        dataBufferSend = UserMemMalloc(SRAM_CCM,4);
        if(dataBufferSend == NULL)
        {
            CoreDelayMinTick();
        }
    } while (dataBufferSend == NULL);

    uint32_t coordinateTemp = 0;
    coordinateTemp = *((uint32_t*)(&(stepMotorVar[STEP_MOTOR9]->var.currentCoordinate)));

    //读取当前电机坐标
    CanBaseWriteUint32DataToBuffer(dataBufferSend,0,coordinateTemp);
    //返回数据包
    CanSlaveSendDataPackWithRecvCache((CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr,CAN_BASE_DATA_MOTOR,dataBufferSend,4);

    //发送完成,释放内存
    UserMemFree(SRAM_CCM,dataBufferSend);

    return LH_ERR_NONE;
}



//自身指令接收处理,步进电机复位
LH_ERR ServiceImplStepMotor9ResetWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //复位指令结构体
    StepMotorResetCmd resetCmd;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorResetCmdParamFillWithSelfMsg(&resetCmd,selfCmdPtr,STEP_MOTOR9);
    //调用复位算法
    errCode = StepMotorReset(&resetCmd);
    if(errCode != LH_ERR_NONE)
    {
        //更新电机状态
        DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
        cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
        return errCode;
    }
    //更新电机状态
    DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
    cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
    //返回结果
    return errCode;
}

//自身指令接收处理,步进电机运行指定步数
LH_ERR ServiceImplStepMotor9RunStepsWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,走位指令
    StepMotorRunStepsCmd stepMotorRunStepsCmd;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorRunSpecialStepsCmdParamFillWithSelfMsg(&stepMotorRunStepsCmd,selfCmdPtr,STEP_MOTOR9);
    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckRunStepsVar(&stepMotorRunStepsCmd);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorRunSpecialSteps(&stepMotorRunStepsCmd);
    if(errCode != LH_ERR_NONE)
    {
        //更新电机状态
        DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
        cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
        return errCode;
    }
    //更新电机状态
    DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
    cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
    //返回结果
    return errCode;
}

//自身指令接收处理,步进电机运行到指定坐标
LH_ERR ServiceImplStepMotor9Run2CoordinatesWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,走到指定坐标指令
    StepMotorRun2CoordationCmd stepMotorRun2CoordationCmd;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorRunToCoordinatesCmdParamFillWithSelfMsg(&stepMotorRun2CoordationCmd,selfCmdPtr,STEP_MOTOR9);
    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckRun2CoordationVar(&stepMotorRun2CoordationCmd);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorRun2SpecialCoordinates(&stepMotorRun2CoordationCmd);
    if(errCode != LH_ERR_NONE)
    {
        //更新电机状态
        DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
        cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
        return errCode;
    }
    //更新电机状态
    DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
    cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
    //返回结果
    return errCode;
}

//自身指令接收处理,步进电机回零
LH_ERR ServiceImplStepMotor9ReturnZeroWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,回零指令
    StepMotorReturnZeroCmd stepMotorReturnZeroCmd;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorReturnZeroCmdParamFillWithSelfMsg(&stepMotorReturnZeroCmd,selfCmdPtr,STEP_MOTOR9);
    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckReturnZeroVar(&stepMotorReturnZeroCmd);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorReturnZeroWithSensor(&stepMotorReturnZeroCmd);
    if(errCode != LH_ERR_NONE)
    {
        //更新电机状态
        DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
        cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
        return errCode;
    }
    //更新电机状态
    DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
    cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
    //返回结果
    return errCode;
}


//自身指令接收处理,步进电机回零但是不复位
LH_ERR ServiceImplStepMotor9ReturnZeroWithoutResetWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,回零指令
    StepMotorReturnZeroCmd stepMotorReturnZeroCmd;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorReturnZeroWithOutResetCmdParamFillWithSelfMsg(&stepMotorReturnZeroCmd,selfCmdPtr,STEP_MOTOR9);
    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckReturnZeroVar(&stepMotorReturnZeroCmd);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorReturnZeroWithSensorRemoveResetStage(&stepMotorReturnZeroCmd);
    if(errCode != LH_ERR_NONE)
    {
        //更新电机状态
        DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
        cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
        return errCode;
    }
    //更新电机状态
    DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
    cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
    //返回结果
    return errCode;
}


//自身指令接收处理,步进电机持续运转不停止
LH_ERR ServiceImplStepMotor9RunAlwaysWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //步进电机运行指令结构体,持续运转
    StepMotorRunAlwaysCmd stepMotorRunAlwaysCmd;
    //根据收到的数据填充结构体
    ServiceImplBaseStepMotorRunAlwaysCmdParamFillWithSelfMsg(&stepMotorRunAlwaysCmd,selfCmdPtr,STEP_MOTOR9);
    //检查参数列表.合规,就可以进行进一步的处理,否则,直接报错
    errCode = StepMotorCheckRunAlways(&stepMotorRunAlwaysCmd);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //调用电机函数进行处理
    errCode = StepMotorRunAlways(&stepMotorRunAlwaysCmd);
    //返回结果
    return errCode;
}

//自身指令接收处理,步进电机立即停止
LH_ERR ServiceImplStepMotor9StopImmediatelyWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    errCode = StepMotorStopImmediately(STEP_MOTOR9);
    return errCode;
}

//自身指令接收处理,步进电机减速停止
LH_ERR ServiceImplStepMotor9StopDecelerationWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    errCode = StepMotorStopDeceleration(STEP_MOTOR9);
    return errCode;
}

//自身指令接收处理,步进电机急停
LH_ERR ServiceImplStepMotor9StopEmergencyWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    errCode = StepMotorStopEmergency(STEP_MOTOR9);
    return errCode;
}

//自身指令接收处理,步进电机读取当前电机位置
LH_ERR ServiceImplStepMotor9ReadCurrentPositionWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)(selfCmdPtr->commandDataSpecialChannel);
    cmdResultPtr->currentCoordinate = stepMotorVar[STEP_MOTOR9]->var.currentCoordinate;
    return LH_ERR_NONE;
}

//自身指令接收处理,设置电机状态
LH_ERR ServiceImplStepMotor9SetStateWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr)
{
    //参数提取
    STEP_MOTOR_MAIN_STATE motorMainState = (STEP_MOTOR_MAIN_STATE)(selfCmdPtr->commandParam1);
    STEP_MOTOR_SUB_STATE motorSubState = (STEP_MOTOR_SUB_STATE)(selfCmdPtr->commandParam2);
    //设置电机状态
    stepMotorVar[STEP_MOTOR9]->var.motorMainState = motorMainState;
    stepMotorVar[STEP_MOTOR9]->var.motorSubState  = motorSubState;
    return LH_ERR_NONE;
}













