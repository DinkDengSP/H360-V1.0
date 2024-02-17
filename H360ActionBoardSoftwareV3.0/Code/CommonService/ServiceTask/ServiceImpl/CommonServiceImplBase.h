/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-02 16:12:54
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceImpl\CommonServiceImplBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __COMMON_SERVICE_IMPL_BASE_H_
#define __COMMON_SERVICE_IMPL_BASE_H_
#include "CanSlaveMain.h"
#include "CanMasterMain.h"
#include "SystemCmd.h"
#include "StepMotor.h"

//电机复位参数填充,CAN指令
void ServiceImplBaseStepMotorResetCmdParamFillWithCanMsg(StepMotorResetCmd* resetCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);
//电机走位参数填充,CAN指令
void ServiceImplBaseStepMotorRunSpecialStepsCmdParamFillWithCanMsg(StepMotorRunStepsCmd* runStepsCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);
//电机走到指定坐标参数填充,CAN指令
void ServiceImplBaseStepMotorRunToCoordinatesCmdParamFillWithCanMsg(StepMotorRun2CoordationCmd* runToCoordinateCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);
//电机回零参数填充,CAN指令
void ServiceImplBaseStepMotorReturnZeroCmdParamFillWithCanMsg(StepMotorReturnZeroCmd* returnZeroCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);
//电机回零参数填充,回零不复位
void ServiceImplBaseStepMotorReturnZeroWithOutResetCmdParamFillWithCanMsg(StepMotorReturnZeroCmd* returnZeroCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,
                                                                            STEP_MOTOR_INDEX stepMotorIndex);
//电机持续运转参数填充,CAN指令
void ServiceImplBaseStepMotorRunAlwaysCmdParamFillWithCanMsg(StepMotorRunAlwaysCmd* runAlwaysCmdPtr,CAN_SLAVE_CHANNEL_CACHE* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);


//步进电机复位参数填充,IPC调用指令
void ServiceImplBaseStepMotorResetCmdParamFillWithSelfMsg(StepMotorResetCmd* resetCmdPtr,SYSTEM_CMD_SELF* selfStepMotorCmdPtr,STEP_MOTOR_INDEX stepMotorIndex);
//电机走位参数填充,IPC调用指令
void ServiceImplBaseStepMotorRunSpecialStepsCmdParamFillWithSelfMsg(StepMotorRunStepsCmd* runStepsCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);
//电机走到指定坐标参数填充,IPC调用指令
void ServiceImplBaseStepMotorRunToCoordinatesCmdParamFillWithSelfMsg(StepMotorRun2CoordationCmd* runToCoordinateCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);
//电机回零参数填充,IPC调用指令
void ServiceImplBaseStepMotorReturnZeroCmdParamFillWithSelfMsg(StepMotorReturnZeroCmd* returnZeroCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);
//电机回零参数填充,IPC调用指令,回零不复位
void ServiceImplBaseStepMotorReturnZeroWithOutResetCmdParamFillWithSelfMsg(StepMotorReturnZeroCmd* returnZeroCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);
//电机持续运转参数填充,IPC调用指令
void ServiceImplBaseStepMotorRunAlwaysCmdParamFillWithSelfMsg(StepMotorRunAlwaysCmd* runAlwaysCmdPtr,SYSTEM_CMD_SELF* stepMotorCmd,STEP_MOTOR_INDEX stepMotorIndex);


#endif




