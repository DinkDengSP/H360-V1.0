/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-29 10:22:56
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceIPC\ServiceIpcStepMotor5.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_STEP_MOTOR5_H_
#define __SERVICE_IPC_STEP_MOTOR5_H_
#include "CommonServiceIpcBase.h"


//IPC调用基础结构初始化
void IPC_StepMotor5DataInit(void);

//输入任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_StepMotor5GetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_StepMotor5WaitLastReturn(void);

/******************************************只要求ACK不要求返回的指令**********************************************/
//电机复位
LH_ERR IPC_StepMotor5ResetWhileAck(int32_t correction,uint32_t timeOut);

//电机走步数
LH_ERR IPC_StepMotor5RunStepsWhileAck(int32_t steps,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机走坐标
LH_ERR IPC_StepMotor5Run2CoordinateWhileAck(int32_t targetCoordinate,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机回零
LH_ERR IPC_StepMotor5ReturnZeroWhileAck(int32_t correctionTarget,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机持续运行
LH_ERR IPC_StepMotor5RunAlwaysWhileAck(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t dir);

//电机立即停止
LH_ERR IPC_StepMotor5StopImmediatelyWhileAck(void);

//电机减速停止
LH_ERR IPC_StepMotor5StopDecelerationWhileAck(void);

//更新当前电机坐标
LH_ERR IPC_StepMotor5ReflushStateWhileAck(void);

//电机回零但是不复位
LH_ERR IPC_StepMotor5ReturnZeroWithoutResetWhileAck(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//设置电机状态
LH_ERR IPC_StepMotor5SetStateWhileAck(STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState);
/******************************************发出指令并等待返回的指令***********************************************/
//电机复位
LH_ERR IPC_StepMotor5ResetWhileReturn(int32_t correction,uint32_t timeOut);

//电机走步数
LH_ERR IPC_StepMotor5RunStepsWhileReturn(int32_t steps,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机走坐标
LH_ERR IPC_StepMotor5Run2CoordinateWhileReturn(int32_t targetCoordinate,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机回零
LH_ERR IPC_StepMotor5ReturnZeroWhileReturn(int32_t correctionTarget,uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//电机持续运行
LH_ERR IPC_StepMotor5RunAlwaysWhileReturn(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t dir);

//电机立即停止
LH_ERR IPC_StepMotor5StopImmediatelyWhileReturn(void);

//电机减速停止
LH_ERR IPC_StepMotor5StopDecelerationWhileReturn(void);

//更新当前电机坐标
LH_ERR IPC_StepMotor5ReflushStateWhileReturn(int32_t* currentCoordinate);

//电机回零不复位
LH_ERR IPC_StepMotor5ReturnZeroWithoutResetWhileReturn(uint8_t selectCurve,uint8_t speedRatio,SPEED_RATIO_MODE ratioMode,uint8_t util1StopEnable,uint8_t util2StopEnable,uint32_t timeOut);

//设置电机状态
LH_ERR IPC_StepMotor5SetStateWhileReturn(STEP_MOTOR_MAIN_STATE motorMainState,STEP_MOTOR_SUB_STATE motorSubState);
/*******************************************结果数据读取**************************************************/
//读取电机当前坐标
int32_t  IPC_StepMotor5CurrentCoordinate(void);

//检查当前电机是否被急停
LH_ERR IPC_StepMotor5CheckEmergencyStop(void);


//强行设置电机坐标
LH_ERR IPC_StepMotor5ModifyCoordinate(int32_t modifyCoordinate);

#endif


