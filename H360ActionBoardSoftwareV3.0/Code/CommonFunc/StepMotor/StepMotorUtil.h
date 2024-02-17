/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-11 09:00:56
 *FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorUtil.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_UTIL_H_
#define __STEP_MOTOR_UTIL_H_
#include "StepMotorParam.h"
#include "StepMotorType.h"

//初始化电机状态
void StepMotorStateInit(StepMotorState* motorStatePtr);

//电机状态初始化,针对复位
void StepMotorStateInitForReset(StepMotorState* motorStatePtr);

//电机状态初始化,针对走位
void StepMotorStateInitForRunSteps(StepMotorState* motorStatePtr);

//电机状态初始化,针对走坐标
void StepMotorStateInitForRunCoordinate(StepMotorState* motorStatePtr);

//电机状态初始化,针对电机持续运转
void StepMotorStateInitForRunAlways(StepMotorState* motorStatePtr);

//电机状态初始化,针对电机回零
void StepMotorStateInitForReturnZero(StepMotorState* motorStatePtr);

//检查电机走指定步数的参数是否合格
LH_ERR StepMotorCheckRunStepsVar(StepMotorRunStepsCmd* runStepPtr);

//检查电机走到指定坐标的参数是否合格
LH_ERR StepMotorCheckRun2CoordationVar(StepMotorRun2CoordationCmd* runCoordationPtr);

//检查电机回零指令的参数是否合格
LH_ERR StepMotorCheckReturnZeroVar(StepMotorReturnZeroCmd* returnZeroPtr);

//检查电机持续运转的参数是否合格
LH_ERR StepMotorCheckRunAlways(StepMotorRunAlwaysCmd* runAlwaysPtr);

//检查步进电机急停状态,如果是指令急停,那么报错
LH_ERR StepMotorUtilCheckEmergencyCmdState(STEP_MOTOR_VAR* motorVarPtr);

//检查步进电机电机的目标坐标是否超过限制
LH_ERR StepMotorUtilCheckTargetCoordinateLimit(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate);

//检查和设置步进电机急停引脚状态
LH_ERR StepMotorUtilCheckAndSetEmergencyPinState(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr);

//检查和设置步进电机辅助急停1状态
LH_ERR StepMotorUtilCheckAndSetUtilStop1PinState(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable);

//检查和设置步进电机辅助急停2状态
LH_ERR StepMotorUtilCheckAndSetUtilStop2PinState(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable);

//根据本次电机目的坐标决定方向
void StepMotorUtilCalcDirWithTargetCoordinate(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate);

//发送启动脉冲,启动运行
void StepMotorUtilSendStartPluse(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr);

//发送减速第一步
void StepMotorUtilSendDaccStartPluse(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr);

//步进电机解除全部的中断注册,运行完成后调用
void StepMotorUtilUnregisterIntPort(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr);

//等待步进电机停止运转
void StepMotorUtilWaitMotorStop(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,uint32_t timeOutSet);

//步进电机更新当前坐标
void StepMotorUtilUpdateCurrentCoordinate(STEP_MOTOR_INDEX motorIndex,STEP_MOTOR_VAR* motorVarPtr,int32_t exceptSteps);


#endif






