/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-19 15:52:28
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-29 09:32:34
 *FilePath: \H360_ActionBoard_ID_4d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceIpc\ServiceIpcStepMotor.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_STEP_MOTOR_H_
#define __SERVICE_IPC_STEP_MOTOR_H_
#include "CommonServiceIpcBase.h"

typedef struct MODULE_STEP_MOTOR_RESET_RUN_CONFIG
{
    STEP_MOTOR_INDEX motorIndex;//步进电机映射
    uint8_t resetCorrectMainIndex;//步进电机复位修正参数的主序号
    uint8_t resetCorrectSubIndex;//步进电机复位修正参数的子序号
}MODULE_STEP_MOTOR_RESET_RUN_CONFIG;

/*****************************************************变量填充默认值*********************************************************/
//复位指令的初始化
void IPC_StepMotorUtilResetCmdDataStructInit(StepMotorResetCmd* resetCmdPtr);
//走步数指令的初始化
void IPC_StepMotorUtilRunStepsCmdDataStructInit(StepMotorRunStepsCmd* runStepCmdPtr);
//走坐标指令的初始化
void IPC_StepMotorUtilRunCoordinateDataStructInit(StepMotorRun2CoordationCmd* runToPosCmdPtr);
//回零指令的初始化
void IPC_StepMotorUtilReturnZeroDataStructInit(StepMotorReturnZeroCmd* returnZeroCmdPtr);
//持续运转指令的初始化
void IPC_StepMotorUtilRunAlwaysDataStructInit(StepMotorRunAlwaysCmd* runAlwaysCmdPtr);

/*****************************************************等待任务完成*********************************************************/
//输入任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_StepMotorGetLastCommandStatusAndResult(STEP_MOTOR_INDEX smIndex,LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_StepMotorWaitLastReturn(STEP_MOTOR_INDEX smIndex);

/******************************************只要求ACK不要求返回的指令**********************************************/
//电机复位
LH_ERR IPC_StepMotorResetWhileAck(StepMotorResetCmd* resetCmdPtr);

//电机走步数
LH_ERR IPC_StepMotorRunStepsWhileAck(StepMotorRunStepsCmd* runStepCmdPtr);

//电机走坐标
LH_ERR IPC_StepMotorRun2CoordinateWhileAck(StepMotorRun2CoordationCmd* runToPosCmdPtr);

//电机回零
LH_ERR IPC_StepMotorReturnZeroWhileAck(StepMotorReturnZeroCmd* returnZeroCmdPtr);

//电机持续运行
LH_ERR IPC_StepMotorRunAlwaysWhileAck(StepMotorRunAlwaysCmd* runAlwaysCmdPtr);

//电机立即停止
LH_ERR IPC_StepMotorStopImmediatelyWhileAck(STEP_MOTOR_INDEX smIndex);

//电机减速停止
LH_ERR IPC_StepMotorStopDecelerationWhileAck(STEP_MOTOR_INDEX smIndex);

//更新当前电机坐标
LH_ERR IPC_StepMotorReflushStateWhileAck(STEP_MOTOR_INDEX smIndex);

//电机回零但是不复位
LH_ERR IPC_StepMotorReturnZeroWithoutResetWhileAck(StepMotorReturnZeroCmd* returnZeroCmdPtr);




/******************************************发出指令并等待返回的指令***********************************************/
//电机复位
LH_ERR IPC_StepMotorResetWhileReturn(StepMotorResetCmd* resetCmdPtr);

//电机走步数
LH_ERR IPC_StepMotorRunStepsWhileReturn(StepMotorRunStepsCmd* runStepCmdPtr);

//电机走坐标
LH_ERR IPC_StepMotorRun2CoordinateWhileReturn(StepMotorRun2CoordationCmd* runToPosCmdPtr);

//电机回零
LH_ERR IPC_StepMotorReturnZeroWhileReturn(StepMotorReturnZeroCmd* returnZeroCmdPtr);

//电机持续运行
LH_ERR IPC_StepMotorRunAlwaysWhileReturn(StepMotorRunAlwaysCmd* runAlwaysCmdPtr);

//电机立即停止
LH_ERR IPC_StepMotorStopImmediatelyWhileReturn(STEP_MOTOR_INDEX smIndex);

//电机减速停止
LH_ERR IPC_StepMotorStopDecelerationWhileReturn(STEP_MOTOR_INDEX smIndex);

//更新当前电机坐标
LH_ERR IPC_StepMotorReflushStateWhileReturn(STEP_MOTOR_INDEX smIndex,int32_t* currentCoordinate);

//电机回零不复位
LH_ERR IPC_StepMotorReturnZeroWithoutResetWhileReturn(StepMotorReturnZeroCmd* returnZeroCmdPtr);

//设置电机状态
LH_ERR IPC_StepMotorSetStateWhileReturn(STEP_MOTOR_INDEX smIndex,STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState);
/*******************************************结果数据读取**************************************************/
//读取电机当前坐标
int32_t  IPC_StepMotorCurrentCoordinate(STEP_MOTOR_INDEX smIndex);
//获取当前指定电机是否被指令急停
LH_ERR IPC_StepMotorCheckEmergencyStop(STEP_MOTOR_INDEX smIndex);
//强行设置电机当前坐标
int32_t  IPC_StepMotorModifyCoordinate(STEP_MOTOR_INDEX smIndex,int32_t modifyCoordinate);










#endif




