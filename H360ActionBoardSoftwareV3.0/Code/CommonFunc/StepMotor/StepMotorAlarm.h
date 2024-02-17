/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-11 08:59:36
 *FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorAlarm.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_ALARM_H_
#define __STEP_MOTOR_ALARM_H_
#include "StepMotorTable.h"
#include "BoardExti.h"

#define STEP_MOTOR_COUNT        12

//急停中断回调函数数组
extern const BoardExtiTrigCallBackFuncPtr StepMotorEmergencyStopByIntSingalFuncPtrArray[STEP_MOTOR_COUNT];

//步进电机辅助急停1中断函数数组
extern const BoardExtiTrigCallBackFuncPtr StepMotorDaccStopByUtilInt1SingalFuncPtrArray[STEP_MOTOR_COUNT];

//步进电机辅助急停2中断函数数组
extern const BoardExtiTrigCallBackFuncPtr StepMotorDaccStopByUtilInt2SingalFuncPtrArray[STEP_MOTOR_COUNT];


#endif




