/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-10 10:56:38
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 09:05:50
**FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceImpl\CommonServiceImplBase.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CommonServiceImplBase.h"

//电机复位参数填充,CAN指令
void ServiceImplBaseStepMotorResetCmdParamFillWithCanMsg(StepMotorResetCmd* resetCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    uint32_t paramFillUtil;
    //读取修正,复位修正
    paramFillUtil = CanBaseReadDataConvertToUint32(stepMotorCmd->recvCacheArray,4);
    //32位有符号数的转换
    resetCmdPtr->correctionCoordinate = *((int32_t*)(&paramFillUtil));
    //读取超时时间
    resetCmdPtr->timeOutSet = CanBaseReadDataConvertToUint32(stepMotorCmd->recvCacheArray,8);
    //操作电机序号
    resetCmdPtr->motorIndex = stepMotorIndex;
}

//电机走位参数填充,CAN指令
void ServiceImplBaseStepMotorRunSpecialStepsCmdParamFillWithCanMsg(StepMotorRunStepsCmd* runStepsCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    uint32_t paramFillUtil;
    
    //读取步数,可以是正数也可以是负数,进行一次转换
    paramFillUtil = CanBaseReadDataConvertToUint32(stepMotorCmd->recvCacheArray,4);
    //32位无符号转有符号,步数可以是负的
    runStepsCmdPtr->specialSteps = *((int32_t*)(&paramFillUtil));
    //读取选择的曲线
    runStepsCmdPtr->selectConfig = stepMotorCmd->recvCacheArray[8];
    //读取速度比例
    runStepsCmdPtr->speedRatio = stepMotorCmd->recvCacheArray[9];
    //读取速度比例模式
    runStepsCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)stepMotorCmd->recvCacheArray[10];
    //读取辅助急停1的使能情况
    runStepsCmdPtr->utilStop1Enable = stepMotorCmd->recvCacheArray[11];
    //读取辅助急停2的使能情况
    runStepsCmdPtr->utilStop2Enable = stepMotorCmd->recvCacheArray[12];
    //运行超时
    runStepsCmdPtr->timeOutSet = CanBaseReadDataConvertToUint32(stepMotorCmd->recvCacheArray,13);

    //操作电机序号
    runStepsCmdPtr->motorIndex = stepMotorIndex;
}


//电机走到指定坐标参数填充,CAN指令
void ServiceImplBaseStepMotorRunToCoordinatesCmdParamFillWithCanMsg(StepMotorRun2CoordationCmd* runToCoordinateCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    uint32_t paramFillUtil = 0;
    
    //读取步数
    paramFillUtil = CanBaseReadDataConvertToUint32(stepMotorCmd->recvCacheArray,4);
    //32位无符号转有符号,坐标可以是负的
    runToCoordinateCmdPtr->targetCoordinate = *((int32_t*)(&paramFillUtil));
    //读取选择的曲线
    runToCoordinateCmdPtr->selectConfig = stepMotorCmd->recvCacheArray[8];
    //读取速度比例
    runToCoordinateCmdPtr->speedRatio = stepMotorCmd->recvCacheArray[9];
    //读取速度比例模式
    runToCoordinateCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)stepMotorCmd->recvCacheArray[10];
    //读取辅助急停1的使能情况
    runToCoordinateCmdPtr->utilStop1Enable = stepMotorCmd->recvCacheArray[11];
    //读取辅助急停2的使能情况
    runToCoordinateCmdPtr->utilStop2Enable = stepMotorCmd->recvCacheArray[12];
    //运行超时
    runToCoordinateCmdPtr->timeOutSet = CanBaseReadDataConvertToUint32(stepMotorCmd->recvCacheArray,13);

    //操作电机序号
    runToCoordinateCmdPtr->motorIndex = stepMotorIndex;
}

//电机回零参数填充,CAN指令
void ServiceImplBaseStepMotorReturnZeroCmdParamFillWithCanMsg(StepMotorReturnZeroCmd* returnZeroCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    uint32_t paramFillUtil = 0;
    
    //读取步数
    paramFillUtil = CanBaseReadDataConvertToUint32(stepMotorCmd->recvCacheArray,4);
    //32位无符号转有符号,复位修正可以带符号
    returnZeroCmdPtr->correctionCoordinate = *((int32_t*)(&paramFillUtil));

    //读取选择的曲线
    returnZeroCmdPtr->selectConfig = stepMotorCmd->recvCacheArray[8];
    //读取速度比例
    returnZeroCmdPtr->speedRatio = stepMotorCmd->recvCacheArray[9];
    //读取速度比例模式
    returnZeroCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)stepMotorCmd->recvCacheArray[10];
    //读取辅助急停1的使能情况
    returnZeroCmdPtr->utilStop1Enable = stepMotorCmd->recvCacheArray[11];
    //读取辅助急停2的使能情况
    returnZeroCmdPtr->utilStop2Enable = stepMotorCmd->recvCacheArray[12];
    //运行超时
    returnZeroCmdPtr->timeOutSet = CanBaseReadDataConvertToUint32(stepMotorCmd->recvCacheArray,13);

    //操作电机序号
    returnZeroCmdPtr->motorIndex = stepMotorIndex;
}

//电机回零参数填充,回零不复位
void ServiceImplBaseStepMotorReturnZeroWithOutResetCmdParamFillWithCanMsg(StepMotorReturnZeroCmd* returnZeroCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,
                                                                            STEP_MOTOR_INDEX stepMotorIndex)
{
    //32位无符号转有符号,复位修正可以带符号
    returnZeroCmdPtr->correctionCoordinate = 0;

    //读取选择的曲线
    returnZeroCmdPtr->selectConfig = stepMotorCmd->recvCacheArray[4];
    //读取速度比例
    returnZeroCmdPtr->speedRatio = stepMotorCmd->recvCacheArray[5];
    //读取速度比例模式
    returnZeroCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)stepMotorCmd->recvCacheArray[6];
    //读取辅助急停1的使能情况
    returnZeroCmdPtr->utilStop1Enable = stepMotorCmd->recvCacheArray[7];
    //读取辅助急停2的使能情况
    returnZeroCmdPtr->utilStop2Enable = stepMotorCmd->recvCacheArray[8];
    //运行超时
    returnZeroCmdPtr->timeOutSet = CanBaseReadDataConvertToUint32(stepMotorCmd->recvCacheArray,9);

    //操作电机序号
    returnZeroCmdPtr->motorIndex = stepMotorIndex;
}

//电机持续运转参数填充,CAN指令
void ServiceImplBaseStepMotorRunAlwaysCmdParamFillWithCanMsg(StepMotorRunAlwaysCmd* runAlwaysCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    //读取选择的曲线
    runAlwaysCmdPtr->selectConfig = stepMotorCmd->recvCacheArray[4];
    //读取速度比例
    runAlwaysCmdPtr->speedRatio = stepMotorCmd->recvCacheArray[5];
    //读取速度比例模式
    runAlwaysCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)stepMotorCmd->recvCacheArray[6];
    //持续运转的运行方向
    runAlwaysCmdPtr->dir = stepMotorVar[stepMotorIndex]->attri.baseConfig.dir;

    //操作电机序号
    runAlwaysCmdPtr->motorIndex = stepMotorIndex;
}



//步进电机复位参数填充,IPC调用指令
void ServiceImplBaseStepMotorResetCmdParamFillWithSelfMsg(StepMotorResetCmd* resetCmdPtr,SYSTEM_CMD_SELF* selfStepMotorCmdPtr,STEP_MOTOR_INDEX stepMotorIndex)
{
    //读取复位修正
    resetCmdPtr->correctionCoordinate = selfStepMotorCmdPtr->commandParam1;
    //读取超时时间
    resetCmdPtr->timeOutSet = selfStepMotorCmdPtr->commandParam2;

    //步进电机序号
    resetCmdPtr->motorIndex = stepMotorIndex;
}

//电机走位参数填充,IPC调用指令
void ServiceImplBaseStepMotorRunSpecialStepsCmdParamFillWithSelfMsg(StepMotorRunStepsCmd* runStepsCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    //步数
    runStepsCmdPtr->specialSteps = stepMotorCmd->commandParam1;
    //读取选择的曲线   点数判断
    runStepsCmdPtr->selectConfig = (uint8_t)((stepMotorCmd->commandParam2)>>16);
    //读取速度比例 
    runStepsCmdPtr->speedRatio = (uint8_t)((stepMotorCmd->commandParam2)>>8);
    //读取速度比例模式
    runStepsCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)((stepMotorCmd->commandParam2));
    //读取辅助急停1的使能情况
    runStepsCmdPtr->utilStop1Enable = (uint8_t)((stepMotorCmd->commandParam3)>>8);
    //读取辅助急停2的使能情况
    runStepsCmdPtr->utilStop2Enable = (uint8_t)(stepMotorCmd->commandParam3);
    //运行超时
    runStepsCmdPtr->timeOutSet = stepMotorCmd->commandParam4;

    //步进电机序号
    runStepsCmdPtr->motorIndex = stepMotorIndex;
}

//电机走到指定坐标参数填充,IPC调用指令
void ServiceImplBaseStepMotorRunToCoordinatesCmdParamFillWithSelfMsg(StepMotorRun2CoordationCmd* runToCoordinateCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    //32位无符号转有符号,步数可以是负的
    runToCoordinateCmdPtr->targetCoordinate = stepMotorCmd->commandParam1;
    //读取选择的曲线
    runToCoordinateCmdPtr->selectConfig = (uint8_t)((stepMotorCmd->commandParam2)>>16);
    //读取速度比例
    runToCoordinateCmdPtr->speedRatio = (uint8_t)((stepMotorCmd->commandParam2)>>8);
    //读取速度比例模式
    runToCoordinateCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)((stepMotorCmd->commandParam2));
    //读取辅助急停1的使能情况
    runToCoordinateCmdPtr->utilStop1Enable = (uint8_t)((stepMotorCmd->commandParam3)>>8);
    //读取辅助急停2的使能情况
    runToCoordinateCmdPtr->utilStop2Enable = (uint8_t)(stepMotorCmd->commandParam3);
    //运行超时
    runToCoordinateCmdPtr->timeOutSet = stepMotorCmd->commandParam4;

    //步进电机序号
    runToCoordinateCmdPtr->motorIndex = stepMotorIndex;
}

//电机回零参数填充,IPC调用指令
void ServiceImplBaseStepMotorReturnZeroCmdParamFillWithSelfMsg(StepMotorReturnZeroCmd* returnZeroCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    //32位无符号转有符号,步数可以是负的
    returnZeroCmdPtr->correctionCoordinate = stepMotorCmd->commandParam1;
    //读取选择的曲线
    returnZeroCmdPtr->selectConfig = (uint8_t)((stepMotorCmd->commandParam2)>>16);
    //读取速度比例
    returnZeroCmdPtr->speedRatio = (uint8_t)((stepMotorCmd->commandParam2)>>8);
    //读取速度比例模式
    returnZeroCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)((stepMotorCmd->commandParam2));
    //读取辅助急停1的使能情况
    returnZeroCmdPtr->utilStop1Enable = (uint8_t)((stepMotorCmd->commandParam3)>>8);
    //读取辅助急停2的使能情况
    returnZeroCmdPtr->utilStop2Enable = (uint8_t)(stepMotorCmd->commandParam3);
    //运行超时
    returnZeroCmdPtr->timeOutSet = stepMotorCmd->commandParam4;

    //步进电机序号
    returnZeroCmdPtr->motorIndex = stepMotorIndex;
}

//电机回零参数填充,IPC调用指令,回零不复位
void ServiceImplBaseStepMotorReturnZeroWithOutResetCmdParamFillWithSelfMsg(StepMotorReturnZeroCmd* returnZeroCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    //32位无符号转有符号,步数可以是负的
    returnZeroCmdPtr->correctionCoordinate = 0;
    //读取选择的曲线
    returnZeroCmdPtr->selectConfig = (uint8_t)((stepMotorCmd->commandParam1)>>16);
    //读取速度比例
    returnZeroCmdPtr->speedRatio = (uint8_t)((stepMotorCmd->commandParam1)>>8);
    //读取速度比例模式
    returnZeroCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)((stepMotorCmd->commandParam1));
    //读取辅助急停1的使能情况
    returnZeroCmdPtr->utilStop1Enable = (uint8_t)((stepMotorCmd->commandParam2)>>8);
    //读取辅助急停2的使能情况
    returnZeroCmdPtr->utilStop2Enable = (uint8_t)(stepMotorCmd->commandParam2);
    //运行超时
    returnZeroCmdPtr->timeOutSet = stepMotorCmd->commandParam3;

    //步进电机序号
    returnZeroCmdPtr->motorIndex = stepMotorIndex;
}


//电机持续运转参数填充,IPC调用指令
void ServiceImplBaseStepMotorRunAlwaysCmdParamFillWithSelfMsg(StepMotorRunAlwaysCmd* runAlwaysCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex)
{
    //读取选择的曲线
    runAlwaysCmdPtr->selectConfig = (uint8_t)((stepMotorCmd->commandParam1)>>16);
    //读取速度比例
    runAlwaysCmdPtr->speedRatio = (uint8_t)((stepMotorCmd->commandParam1)>>8);
    //读取速度比例模式
    runAlwaysCmdPtr->speedRatioMode = (SPEED_RATIO_MODE)((stepMotorCmd->commandParam1));
    if(stepMotorCmd->commandParam2 != 0)
    {
        //不为0就是正向
        runAlwaysCmdPtr->dir = stepMotorVar[stepMotorIndex]->attri.baseConfig.dir;
    }
    else
    {
        //为0就是反向
        runAlwaysCmdPtr->dir = (BOARD_CPLD_MOTOR_DIR)(!(stepMotorVar[stepMotorIndex]->attri.baseConfig.dir));
    }
    //步进电机序号
    runAlwaysCmdPtr->motorIndex = stepMotorIndex;
}
























