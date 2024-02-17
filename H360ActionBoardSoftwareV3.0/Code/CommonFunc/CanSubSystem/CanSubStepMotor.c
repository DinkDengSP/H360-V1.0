#include "CanSubStepMotor.h"
#include "StepMotor.h"
#include "BoardInc.h"
#include "SelfBoardID.h"

//CAN2模块计算指定电机的板卡号和通道号
static LH_ERR CanSubSM_CalcBoardChannel(CAN_SUB_SM_INDEX stepMotorIndex,CAN_MASTER_SUB_BOARD* boardIndex,CAN_MASTER_CHANNEL* channelIndex)
{
    //超过最大值
    if(stepMotorIndex > CAN_SUB_BOARD10_SM12)
    {
        return LH_ERR_CAN_MASTER_SM_INDEX;
    }
    if(stepMotorIndex <= CAN_SUB_BOARD1_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD1;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD1_SM1));
    }
    else if(stepMotorIndex <= CAN_SUB_BOARD2_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD2;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD2_SM1));
    }
    else if(stepMotorIndex <= CAN_SUB_BOARD3_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD3;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD3_SM1));
    }
    else if(stepMotorIndex <= CAN_SUB_BOARD4_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD4;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD4_SM1));
    }
    else if(stepMotorIndex <= CAN_SUB_BOARD5_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD5;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD5_SM1));
    }
    else if(stepMotorIndex <= CAN_SUB_BOARD6_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD6;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD6_SM1));
    }
    else if(stepMotorIndex <= CAN_SUB_BOARD7_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD7;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD7_SM1));
    }
    else if(stepMotorIndex <= CAN_SUB_BOARD8_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD8;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD8_SM1));
    }
    else if(stepMotorIndex <= CAN_SUB_BOARD9_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD9;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD9_SM1));
    }
    else if(stepMotorIndex <= CAN_SUB_BOARD10_SM12)
    {
        *boardIndex = CAN_MASTER_SUB_BOARD10;
        *channelIndex = (CAN_MASTER_CHANNEL)(CAN_MASTER_CHANNEL_SM1 + (stepMotorIndex - CAN_SUB_BOARD10_SM1));
    }
    return LH_ERR_NONE;
}

//系统内部电机复位
//修正步数 超时时间
LH_ERR CanSubSM_ResetWhileAck(SM_RESET_CMD* resetCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    LH_ERR errCode = LH_ERR_NONE;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(resetCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorResetCmd directResetCmd;
        //计算电机序号
        directResetCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directResetCmd.correctionCoordinate = resetCmdPtr->correctionPosition;
        directResetCmd.timeOutSet = resetCmdPtr->timeOutMs;
        //执行运转
        return StepMotorReset(&directResetCmd);
    }   

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 8;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,0,resetCmdPtr->correctionPosition);
    CanBaseWriteUint32DataToBuffer(canMasterSendCmd.paramBuffer,4,resetCmdPtr->timeOutMs);
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RESET;
    canMasterSendCmd.timeoutMs = resetCmdPtr->timeOutMs;
    //数据发送
    errCode = CanMasterSendCommandWhileAck(&canMasterSendCmd);

    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    return errCode;
}

//系统内部电机执行指定步数
LH_ERR CanSubSM_RunSpecialStepsWhileAck(SM_RUN_STEP_CMD* runStepsCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    LH_ERR errCode = LH_ERR_NONE;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(runStepsCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorRunStepsCmd directRunStepsCmd;
        //计算电机序号
        directRunStepsCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directRunStepsCmd.specialSteps = runStepsCmdPtr->steps;
        directRunStepsCmd.selectConfig = runStepsCmdPtr->curveSelect;
        directRunStepsCmd.speedRatioMode = (SPEED_RATIO_MODE)runStepsCmdPtr->speedMode;
        directRunStepsCmd.speedRatio = runStepsCmdPtr->speedRatio;
        directRunStepsCmd.utilStop1Enable = runStepsCmdPtr->utilStop1Enable;
        directRunStepsCmd.utilStop2Enable = runStepsCmdPtr->utilStop2Enable;
        directRunStepsCmd.timeOutSet = runStepsCmdPtr->timeOutMs;
        //执行运转
        return StepMotorRunSpecialSteps(&directRunStepsCmd);
    }  

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 13;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,0,runStepsCmdPtr->steps);
    canMasterSendCmd.paramBuffer[4] = runStepsCmdPtr->curveSelect;
    canMasterSendCmd.paramBuffer[5] = runStepsCmdPtr->speedRatio;
    canMasterSendCmd.paramBuffer[6] = runStepsCmdPtr->speedMode;
    canMasterSendCmd.paramBuffer[7] = runStepsCmdPtr->utilStop1Enable;
    canMasterSendCmd.paramBuffer[8] = runStepsCmdPtr->utilStop2Enable;
    CanBaseWriteUint32DataToBuffer(canMasterSendCmd.paramBuffer,9,runStepsCmdPtr->timeOutMs);

    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RUN_STEPS;
    canMasterSendCmd.timeoutMs = runStepsCmdPtr->timeOutMs;
    //数据发送
    errCode = CanMasterSendCommandWhileAck(&canMasterSendCmd);
    
    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }

    return errCode;
}

//系统内部电机运行到指定坐标
LH_ERR CanSubSM_RunToCoordinateWhileAck(SM_RUN_COORDINATE_CMD* runCoordinateCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    LH_ERR errCode = LH_ERR_NONE;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(runCoordinateCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorRun2CoordationCmd directRunToCoordinateCmd;
        //计算电机序号
        directRunToCoordinateCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directRunToCoordinateCmd.targetCoordinate = runCoordinateCmdPtr->targetCoordinate;
        directRunToCoordinateCmd.selectConfig = runCoordinateCmdPtr->curveSelect;
        directRunToCoordinateCmd.speedRatioMode = (SPEED_RATIO_MODE)runCoordinateCmdPtr->speedMode;
        directRunToCoordinateCmd.speedRatio = runCoordinateCmdPtr->speedRatio;
        directRunToCoordinateCmd.utilStop1Enable = runCoordinateCmdPtr->utilStop1Enable;
        directRunToCoordinateCmd.utilStop2Enable = runCoordinateCmdPtr->utilStop2Enable;
        directRunToCoordinateCmd.timeOutSet = runCoordinateCmdPtr->timeOutMs;
        //执行运转
        return StepMotorRun2SpecialCoordinates(&directRunToCoordinateCmd);
    }  

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 13;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,0,runCoordinateCmdPtr->targetCoordinate);
    canMasterSendCmd.paramBuffer[4] = runCoordinateCmdPtr->curveSelect;
    canMasterSendCmd.paramBuffer[5] = runCoordinateCmdPtr->speedRatio;
    canMasterSendCmd.paramBuffer[6] = runCoordinateCmdPtr->speedMode;
    canMasterSendCmd.paramBuffer[7] = runCoordinateCmdPtr->utilStop1Enable;
    canMasterSendCmd.paramBuffer[8] = runCoordinateCmdPtr->utilStop2Enable;
    CanBaseWriteUint32DataToBuffer(canMasterSendCmd.paramBuffer,9,runCoordinateCmdPtr->timeOutMs);
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RUN_CORDINATE;
    canMasterSendCmd.timeoutMs = runCoordinateCmdPtr->timeOutMs;

    //数据发送
    errCode = CanMasterSendCommandWhileAck(&canMasterSendCmd);
    
    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    return errCode;
}

//系统内部电机回零
LH_ERR CanSubSM_ReturnZeroWhileAck(SM_RETURN_ZERO_CMD* returnZeroCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    LH_ERR errCode = LH_ERR_NONE;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(returnZeroCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorReturnZeroCmd directReturnZeroCmd;
        //计算电机序号
        directReturnZeroCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directReturnZeroCmd.correctionCoordinate = returnZeroCmdPtr->correctionPosition;
        directReturnZeroCmd.selectConfig = returnZeroCmdPtr->curveSelect;
        directReturnZeroCmd.speedRatioMode = (SPEED_RATIO_MODE)returnZeroCmdPtr->speedMode;
        directReturnZeroCmd.speedRatio = returnZeroCmdPtr->speedRatio;
        directReturnZeroCmd.utilStop1Enable = returnZeroCmdPtr->utilStop1Enable;
        directReturnZeroCmd.utilStop2Enable = returnZeroCmdPtr->utilStop2Enable;
        directReturnZeroCmd.timeOutSet = returnZeroCmdPtr->timeOutMs;
        //执行运转
        return StepMotorReturnZeroWithSensor(&directReturnZeroCmd);
    }  

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 13;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,0,returnZeroCmdPtr->correctionPosition);
    canMasterSendCmd.paramBuffer[4] = returnZeroCmdPtr->curveSelect;
    canMasterSendCmd.paramBuffer[5] = returnZeroCmdPtr->speedRatio;
    canMasterSendCmd.paramBuffer[6] = returnZeroCmdPtr->speedMode;
    canMasterSendCmd.paramBuffer[7] = returnZeroCmdPtr->utilStop1Enable;
    canMasterSendCmd.paramBuffer[8] = returnZeroCmdPtr->utilStop2Enable;
    CanBaseWriteUint32DataToBuffer(canMasterSendCmd.paramBuffer,9,returnZeroCmdPtr->timeOutMs);
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RETURN_ZERO;
    canMasterSendCmd.timeoutMs = returnZeroCmdPtr->timeOutMs;

    //数据发送
    errCode = CanMasterSendCommandWhileAck(&canMasterSendCmd);
    
    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    return errCode;
}

//系统内部电机电机持续运转
LH_ERR CanSubSM_RunAlwaysWhileAck(SM_RUN_ALWAYS_CMD* runAlwaysCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    LH_ERR errCode = LH_ERR_NONE;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(runAlwaysCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorRunAlwaysCmd directRunAlwaysCmd;
        //计算电机序号
        directRunAlwaysCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directRunAlwaysCmd.selectConfig = runAlwaysCmdPtr->curveSelect;
        directRunAlwaysCmd.speedRatio = runAlwaysCmdPtr->speedRatio;
        directRunAlwaysCmd.speedRatioMode = (SPEED_RATIO_MODE)runAlwaysCmdPtr->speedMode;
        //执行运转
        return StepMotorRunAlways(&directRunAlwaysCmd);
    }

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 3;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    canMasterSendCmd.paramBuffer[0] = runAlwaysCmdPtr->curveSelect;
    canMasterSendCmd.paramBuffer[1] = runAlwaysCmdPtr->speedRatio;
    canMasterSendCmd.paramBuffer[2] = runAlwaysCmdPtr->speedMode;
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RUN_ALWAYS;
    canMasterSendCmd.timeoutMs = 5000;

    //数据发送
    errCode = CanMasterSendCommandWhileAck(&canMasterSendCmd);
    
    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    return errCode;
}

//系统内部电机急停
LH_ERR CanSubSM_StopImmediatelyWhileAck(CAN_SUB_SM_INDEX motorIndex)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    LH_ERR errCode = LH_ERR_NONE;

    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(motorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        STEP_MOTOR_INDEX stepMotorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        //执行运转
        return StepMotorStopImmediately(stepMotorIndex);
    }

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 0;
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_STOP_IMME;
    canMasterSendCmd.timeoutMs = 1000;

    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }

    return errCode;
}

//系统内部电机减速停止
LH_ERR CanSubSM_StopGraduallyWhileAck(CAN_SUB_SM_INDEX motorIndex)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    LH_ERR errCode = LH_ERR_NONE;

    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(motorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        STEP_MOTOR_INDEX stepMotorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        //执行运转
        return StepMotorStopDeceleration(stepMotorIndex);
    }

    //不需要申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 0;
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_STOP_DACC;
    canMasterSendCmd.timeoutMs = 5000;

    //数据发送
    errCode = CanMasterSendCommandWhileAck(&canMasterSendCmd);

    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }

    return errCode;
}


/**********************************************************发送电机指令并等待电机执行完成***************************************************/
//系统内部电机复位
//修正步数 超时时间
LH_ERR CanSubSM_ResetWhileReturn(SM_RESET_CMD* resetCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;

    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(resetCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorResetCmd directResetCmd;
        //计算电机序号
        directResetCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directResetCmd.correctionCoordinate = resetCmdPtr->correctionPosition;
        directResetCmd.timeOutSet = resetCmdPtr->timeOutMs;
        //执行运转
        return StepMotorReset(&directResetCmd);
    }   

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 8;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,0,resetCmdPtr->correctionPosition);
    CanBaseWriteUint32DataToBuffer(canMasterSendCmd.paramBuffer,4,resetCmdPtr->timeOutMs);
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RESET;
    canMasterSendCmd.timeoutMs = resetCmdPtr->timeOutMs;
    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);
    
    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    //接收到的数据不需要
    if(canMasterReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//系统内部电机执行执行步数
LH_ERR CanSubSM_RunSpecialStepsWhileReturn(SM_RUN_STEP_CMD* runStepsCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(runStepsCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorRunStepsCmd directRunStepsCmd;
        //计算电机序号
        directRunStepsCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directRunStepsCmd.specialSteps = runStepsCmdPtr->steps;
        directRunStepsCmd.selectConfig = runStepsCmdPtr->curveSelect;
        directRunStepsCmd.speedRatioMode = (SPEED_RATIO_MODE)runStepsCmdPtr->speedMode;
        directRunStepsCmd.speedRatio = runStepsCmdPtr->speedRatio;
        directRunStepsCmd.utilStop1Enable = runStepsCmdPtr->utilStop1Enable;
        directRunStepsCmd.utilStop2Enable = runStepsCmdPtr->utilStop2Enable;
        directRunStepsCmd.timeOutSet = runStepsCmdPtr->timeOutMs;
        //执行运转
        return StepMotorRunSpecialSteps(&directRunStepsCmd);
    }

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 13;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,0,runStepsCmdPtr->steps);
    canMasterSendCmd.paramBuffer[4] = runStepsCmdPtr->curveSelect;
    canMasterSendCmd.paramBuffer[5] = runStepsCmdPtr->speedRatio;
    canMasterSendCmd.paramBuffer[6] = runStepsCmdPtr->speedMode;
    canMasterSendCmd.paramBuffer[7] = runStepsCmdPtr->utilStop1Enable;
    canMasterSendCmd.paramBuffer[8] = runStepsCmdPtr->utilStop2Enable;
    CanBaseWriteUint32DataToBuffer(canMasterSendCmd.paramBuffer,9,runStepsCmdPtr->timeOutMs);
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RUN_STEPS;
    canMasterSendCmd.timeoutMs = runStepsCmdPtr->timeOutMs;
    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);
    
    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    //接收到的数据不需要
    if(canMasterReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }

    return errCode;
}

//系统内部电机运行到指定坐标
LH_ERR CanSubSM_RunToCoordinateWhileReturn(SM_RUN_COORDINATE_CMD* runCoordinateCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(runCoordinateCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorRun2CoordationCmd directRunToCoordinateCmd;
        //计算电机序号
        directRunToCoordinateCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directRunToCoordinateCmd.targetCoordinate = runCoordinateCmdPtr->targetCoordinate;
        directRunToCoordinateCmd.selectConfig = runCoordinateCmdPtr->curveSelect;
        directRunToCoordinateCmd.speedRatioMode = (SPEED_RATIO_MODE)runCoordinateCmdPtr->speedMode;
        directRunToCoordinateCmd.speedRatio = runCoordinateCmdPtr->speedRatio;
        directRunToCoordinateCmd.utilStop1Enable = runCoordinateCmdPtr->utilStop1Enable;
        directRunToCoordinateCmd.utilStop2Enable = runCoordinateCmdPtr->utilStop2Enable;
        directRunToCoordinateCmd.timeOutSet = runCoordinateCmdPtr->timeOutMs;
        //执行运转
        return StepMotorRun2SpecialCoordinates(&directRunToCoordinateCmd);
    }  

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 13;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,0,runCoordinateCmdPtr->targetCoordinate);
    canMasterSendCmd.paramBuffer[4] = runCoordinateCmdPtr->curveSelect;
    canMasterSendCmd.paramBuffer[5] = runCoordinateCmdPtr->speedRatio;
    canMasterSendCmd.paramBuffer[6] = runCoordinateCmdPtr->speedMode;
    canMasterSendCmd.paramBuffer[7] = runCoordinateCmdPtr->utilStop1Enable;
    canMasterSendCmd.paramBuffer[8] = runCoordinateCmdPtr->utilStop2Enable;
    CanBaseWriteUint32DataToBuffer(canMasterSendCmd.paramBuffer,9,runCoordinateCmdPtr->timeOutMs);
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RUN_CORDINATE;
    canMasterSendCmd.timeoutMs = runCoordinateCmdPtr->timeOutMs;

    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);

    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    //接收到的数据不需要
    if(canMasterReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }

    return errCode;
}

//系统内部电机回零
LH_ERR CanSubSM_ReturnZeroWhileReturn(SM_RETURN_ZERO_CMD* returnZeroCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(returnZeroCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorReturnZeroCmd directReturnZeroCmd;
        //计算电机序号
        directReturnZeroCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directReturnZeroCmd.correctionCoordinate = returnZeroCmdPtr->correctionPosition;
        directReturnZeroCmd.selectConfig = returnZeroCmdPtr->curveSelect;
        directReturnZeroCmd.speedRatioMode = (SPEED_RATIO_MODE)returnZeroCmdPtr->speedMode;
        directReturnZeroCmd.speedRatio = returnZeroCmdPtr->speedRatio;
        directReturnZeroCmd.utilStop1Enable = returnZeroCmdPtr->utilStop1Enable;
        directReturnZeroCmd.utilStop2Enable = returnZeroCmdPtr->utilStop2Enable;
        directReturnZeroCmd.timeOutSet = returnZeroCmdPtr->timeOutMs;
        //执行运转
        return StepMotorReturnZeroWithSensor(&directReturnZeroCmd);
    }  

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 13;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,0,returnZeroCmdPtr->correctionPosition);
    canMasterSendCmd.paramBuffer[4] = returnZeroCmdPtr->curveSelect;
    canMasterSendCmd.paramBuffer[5] = returnZeroCmdPtr->speedRatio;
    canMasterSendCmd.paramBuffer[6] = returnZeroCmdPtr->speedMode;
    canMasterSendCmd.paramBuffer[7] = returnZeroCmdPtr->utilStop1Enable;
    canMasterSendCmd.paramBuffer[8] = returnZeroCmdPtr->utilStop2Enable;
    CanBaseWriteUint32DataToBuffer(canMasterSendCmd.paramBuffer,9,returnZeroCmdPtr->timeOutMs);
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RETURN_ZERO;
    canMasterSendCmd.timeoutMs = returnZeroCmdPtr->timeOutMs;

    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);

    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    //接收到的数据不需要
    if(canMasterReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }

    return errCode;
}

//系统内部电机电机持续运转
LH_ERR CanSubSM_RunAlwaysWhileReturn(SM_RUN_ALWAYS_CMD* runAlwaysCmdPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(runAlwaysCmdPtr->stepMotorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        StepMotorRunAlwaysCmd directRunAlwaysCmd;
        //计算电机序号
        directRunAlwaysCmd.motorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        directRunAlwaysCmd.selectConfig = runAlwaysCmdPtr->curveSelect;
        directRunAlwaysCmd.speedRatio = runAlwaysCmdPtr->speedRatio;
        directRunAlwaysCmd.speedRatioMode = (SPEED_RATIO_MODE)runAlwaysCmdPtr->speedMode;
        //执行运转
        return StepMotorRunAlways(&directRunAlwaysCmd);
    }

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 3;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,sizeof(canMasterSendCmd.paramLength));
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //内存数据拷贝
    canMasterSendCmd.paramBuffer[0] = runAlwaysCmdPtr->curveSelect;
    canMasterSendCmd.paramBuffer[1] = runAlwaysCmdPtr->speedRatio;
    canMasterSendCmd.paramBuffer[2] = runAlwaysCmdPtr->speedMode;
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_RUN_ALWAYS;
    canMasterSendCmd.timeoutMs = 5000;

    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);
    
    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    //接收到的数据不需要
    if(canMasterReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }

    return errCode;
}

//系统内部电机急停
LH_ERR CanSubSM_StopImmediatelyWhileReturn(CAN_SUB_SM_INDEX motorIndex)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(motorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        STEP_MOTOR_INDEX stepMotorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        //执行运转
        return StepMotorStopImmediately(stepMotorIndex);
    }

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 0;
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_STOP_IMME;
    canMasterSendCmd.timeoutMs = 1000;
    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);

    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }

    //接收到的数据不需要
    if(canMasterReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//系统内部电机减速停止
LH_ERR CanSubSM_StopGraduallyWhileReturn(CAN_SUB_SM_INDEX motorIndex)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;

    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(motorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        STEP_MOTOR_INDEX stepMotorIndex = (STEP_MOTOR_INDEX)(canMasterSendCmd.channelNo - CAN_MASTER_CHANNEL_SM1);
        //执行运转
        return StepMotorStopDeceleration(stepMotorIndex);
    }

    //不需要申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 0;
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_STOP_DACC;
    canMasterSendCmd.timeoutMs = 5000;
    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);

    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }

    //接收到的数据不需要
    if(canMasterReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }

    return errCode;
}

//系统读取指定电机当前位置
LH_ERR CanSubSM_ReadPositionWhileReturn(CAN_SUB_SM_INDEX motorIndex,int32_t* motorPos)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;

    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(motorIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo));
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //不需要申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 0;
    //指令码设定
    canMasterSendCmd.commandCode = SM_CMD_READ_POS;
    canMasterSendCmd.timeoutMs = 1000;

    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);

    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }

    //首先看指令是否出错
    if(errCode != LH_ERR_NONE)
    {
        //错误产生
        if(canMasterReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
        }
        return errCode;
    }
    //数据为空
    if(canMasterReadData.recvDataPtr == NULL)
    {
        return LH_ERR_CAN_MASTER_SM_DATA_NULL;
    }
    //接收到的数据不需要
    if(canMasterReadData.recvDatLength != 10)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
        return LH_ERR_CAN_MASTER_SM_DATA_LENGTH;
    }
    //读取数据
    if(CAN_BASE_DATA_MOTOR != CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,0))
    {
        //数据标志位错误
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
        return LH_ERR_CAN_MASTER_SM_DATA_FLAG;
    }
    //读取坐标
    *motorPos = CanBaseReadDataConvertToInt32(canMasterReadData.recvDataPtr,2);
    //释放内存
    UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    return errCode;
}

/*********************************************************等待对应的电机动作执行完成**************************************************************/
LH_ERR CanSubSM_WaitLastReturn(CAN_SUB_SM_INDEX motorIndex,uint32_t timeOutMs)
{
    CAN_MASTER_READ_DATA canMasterReadData;
    CAN_MASTER_SUB_BOARD boardID;
    CAN_MASTER_CHANNEL channelNo;
    LH_ERR errCode = LH_ERR_NONE;
    //计算板卡号码和单元号码
    errCode = CanSubSM_CalcBoardChannel(motorIndex,&boardID,&channelNo);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果指向的是本机,那么不需要通讯
    if((uint8_t)(boardID) == (uint8_t)SELF_BOARD_ID)
    {
        //本机指令直接调用
        return LH_ERR_NONE;
    }   

    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;
    errCode = CanMasterSendCommandWaitReturn(boardID,channelNo,timeOutMs,&canMasterReadData);
    
    //接收到的数据不需要
    if(canMasterReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}


//复位指令的参数的初始化
void CanSubSM_ResetCmdDataStructInit(SM_RESET_CMD* cmdPtr)
{
    cmdPtr->stepMotorIndex = CAN_SUB_BOARD1_SM1;
    cmdPtr->correctionPosition = 0;
    cmdPtr->timeOutMs = 60000;
}

//走步数指令的参数的初始化
void CanSubSM_RunStepsCmdDataStructInit(SM_RUN_STEP_CMD* cmdPtr)
{
    cmdPtr->stepMotorIndex = CAN_SUB_BOARD1_SM1;
    cmdPtr->steps = 0;
    cmdPtr->curveSelect = SM_CURVE_0;
    cmdPtr->speedRatio = 100;
    cmdPtr->speedMode = SPEED_MODE_UP_START;
    cmdPtr->utilStop1Enable = DISABLE;
    cmdPtr->utilStop2Enable = DISABLE;
    cmdPtr->timeOutMs = 60000;
}

//走坐标指令的参数的初始化
void CanSubSM_RunCoordinateDataStructInit(SM_RUN_COORDINATE_CMD* cmdPtr)
{
    cmdPtr->stepMotorIndex = CAN_SUB_BOARD1_SM1;
    cmdPtr->targetCoordinate = 0;
    cmdPtr->curveSelect = SM_CURVE_0;
    cmdPtr->speedRatio = 100;
    cmdPtr->speedMode = SPEED_MODE_UP_START;
    cmdPtr->utilStop1Enable = DISABLE;
    cmdPtr->utilStop2Enable = DISABLE;
    cmdPtr->timeOutMs = 60000;
}

//回零指令的参数的初始化
void CanSubSM_ReturnZeroDataStructInit(SM_RETURN_ZERO_CMD* cmdPtr)
{
    cmdPtr->stepMotorIndex = CAN_SUB_BOARD1_SM1;
    cmdPtr->correctionPosition = 0;
    cmdPtr->curveSelect = SM_CURVE_0;
    cmdPtr->speedRatio = 100;
    cmdPtr->speedMode = SPEED_MODE_UP_START;
    cmdPtr->utilStop1Enable = DISABLE;
    cmdPtr->utilStop2Enable = DISABLE;
    cmdPtr->timeOutMs = 60000;
}

//持续运转指令的参数的初始化
void CanSubSM_RunAlwaysDataStructInit(SM_RUN_ALWAYS_CMD* cmdPtr)
{
    cmdPtr->stepMotorIndex = CAN_SUB_BOARD1_SM1;
    cmdPtr->curveSelect = SM_CURVE_0;
    cmdPtr->speedRatio = 100;
    cmdPtr->speedMode = SPEED_MODE_UP_START;
}















