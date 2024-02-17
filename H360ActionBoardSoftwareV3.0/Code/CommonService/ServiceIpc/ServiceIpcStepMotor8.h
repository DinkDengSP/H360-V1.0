/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-29 10:23:17
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceIPC\ServiceIpcStepMotor8.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_STEP_MOTOR8_H_
#define __SERVICE_IPC_STEP_MOTOR8_H_
#include "CommonServiceIpcBase.h"


//IPC调用基础结构初始化
void IPC_StepMotor8DataInit(void);

//输入任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_StepMotor8GetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_StepMotor8WaitLastReturn(void);

/******************************************只要求ACK不要求返回的指令**********************************************/
//电机复位
LH_ERR IPC_StepMotor8ResetWhileAck(int32_t correction,uint32_t timeOut);

//电机走步数
LH_ERR IPC_StepMotor8RunStepsWhileAck(int32_t steps,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机走坐标
LH_ERR IPC_StepMotor8Run2CoordinateWhileAck(int32_t targetCoordinate,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机回零
LH_ERR IPC_StepMotor8ReturnZeroWhileAck(int32_t correctionTarget,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机持续运行
LH_ERR IPC_StepMotor8RunAlwaysWhileAck(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t dir);

//电机立即停止
LH_ERR IPC_StepMotor8StopImmediatelyWhileAck(void);

//电机减速停止
LH_ERR IPC_StepMotor8StopDecelerationWhileAck(void);

//更新当前电机坐标
LH_ERR IPC_StepMotor8ReflushStateWhileAck(void);

//电机回零但是不复位
LH_ERR IPC_StepMotor8ReturnZeroWithoutResetWhileAck(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//设置电机状态
LH_ERR IPC_StepMotor8SetStateWhileAck(STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState);
/******************************************发出指令并等待返回的指令***********************************************/
//电机复位
LH_ERR IPC_StepMotor8ResetWhileReturn(int32_t correction,uint32_t timeOut);

//电机走步数
LH_ERR IPC_StepMotor8RunStepsWhileReturn(int32_t steps,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机走坐标
LH_ERR IPC_StepMotor8Run2CoordinateWhileReturn(int32_t targetCoordinate,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机回零
LH_ERR IPC_StepMotor8ReturnZeroWhileReturn(int32_t correctionTarget,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机持续运行
LH_ERR IPC_StepMotor8RunAlwaysWhileReturn(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t dir);

//电机立即停止
LH_ERR IPC_StepMotor8StopImmediatelyWhileReturn(void);

//电机减速停止
LH_ERR IPC_StepMotor8StopDecelerationWhileReturn(void);

//更新当前电机坐标
LH_ERR IPC_StepMotor8ReflushStateWhileReturn(int32_t* currentCoordinate);

//电机回零不复位
LH_ERR IPC_StepMotor8ReturnZeroWithoutResetWhileReturn(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//设置电机状态
LH_ERR IPC_StepMotor8SetStateWhileReturn(STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState);
/*******************************************结果数据读取**************************************************/
//读取电机当前坐标
int32_t  IPC_StepMotor8CurrentCoordinate(void);

//检查当前电机是否被急停
LH_ERR IPC_StepMotor8CheckEmergencyStop(void);

//强行设置电机坐标
LH_ERR IPC_StepMotor8ModifyCoordinate(int32_t modifyCoordinate);

#endif


