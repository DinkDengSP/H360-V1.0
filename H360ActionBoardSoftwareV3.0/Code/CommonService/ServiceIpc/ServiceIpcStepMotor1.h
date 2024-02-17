/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-29 10:22:23
 *FilePath: \H360_ActionBoard_ID_4d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceIpc\ServiceIpcStepMotor1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_STEP_MOTOR1_H_
#define __SERVICE_IPC_STEP_MOTOR1_H_
#include "CommonServiceIpcBase.h"

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_StepMotor1DataInit(void);

/*****************************************************等待任务完成*********************************************************/
//输入任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_StepMotor1GetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_StepMotor1WaitLastReturn(void);

/******************************************只要求ACK不要求返回的指令**********************************************/
//电机复位,不等待返回
LH_ERR IPC_StepMotor1ResetWhileAck(int32_t correction,uint32_t timeOut);

//电机走步数,不等待返回
LH_ERR IPC_StepMotor1RunStepsWhileAck(int32_t steps,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机走坐标,不等待返回
LH_ERR IPC_StepMotor1Run2CoordinateWhileAck(int32_t targetCoordinate,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机回零,不等待返回
LH_ERR IPC_StepMotor1ReturnZeroWhileAck(int32_t correctionTarget,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机持续运行,不等待返回
LH_ERR IPC_StepMotor1RunAlwaysWhileAck(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t dir);

//电机立即停止,不等待返回
LH_ERR IPC_StepMotor1StopImmediatelyWhileAck(void);

//电机减速停止,不等待返回
LH_ERR IPC_StepMotor1StopDecelerationWhileAck(void);

//更新当前电机坐标,不等待返回
LH_ERR IPC_StepMotor1ReflushStateWhileAck(void);

//电机回零但是不复位,不等待返回
LH_ERR IPC_StepMotor1ReturnZeroWithoutResetWhileAck(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//设置电机状态
LH_ERR IPC_StepMotor1SetStateWhileAck(STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState);


/******************************************发出指令并等待返回的指令***********************************************/
//电机复位,等待返回
LH_ERR IPC_StepMotor1ResetWhileReturn(int32_t correction,uint32_t timeOut);

//电机走步数,等待返回
LH_ERR IPC_StepMotor1RunStepsWhileReturn(int32_t steps,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机走坐标,等待返回
LH_ERR IPC_StepMotor1Run2CoordinateWhileReturn(int32_t targetCoordinate,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机回零,等待返回
LH_ERR IPC_StepMotor1ReturnZeroWhileReturn(int32_t correctionTarget,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机持续运行,等待返回
LH_ERR IPC_StepMotor1RunAlwaysWhileReturn(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t dir);

//电机立即停止,等待返回
LH_ERR IPC_StepMotor1StopImmediatelyWhileReturn(void);

//电机减速停止,等待返回
LH_ERR IPC_StepMotor1StopDecelerationWhileReturn(void);

//更新当前电机坐标,等待返回
LH_ERR IPC_StepMotor1ReflushStateWhileReturn(int32_t* currentCoordinate);

//电机回零不复位,等待返回
LH_ERR IPC_StepMotor1ReturnZeroWithoutResetWhileReturn(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//设置电机状态
LH_ERR IPC_StepMotor1SetStateWhileReturn(STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState);
/*******************************************结果数据读取**************************************************/
//读取电机当前坐标,,等待返回
int32_t  IPC_StepMotor1CurrentCoordinate(void);

//检查当前电机是否被急停
LH_ERR IPC_StepMotor1CheckEmergencyStop(void);

//强行设置电机坐标
LH_ERR IPC_StepMotor1ModifyCoordinate(int32_t modifyCoordinate);

#endif

