/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-11 08:57:49
 *FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorTable.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_TABLE_H_
#define __STEP_MOTOR_TABLE_H_
#include "StepMotorUtil.h"

//创建步进电机速度表格
//使用步数计算步进电机加速曲线
void StepMotorTableCalcWithStep(int32_t steps,STEP_MOTOR_INDEX motorIndex,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio);

//使用坐标计算步进电机加速曲线
void StepMotorTableCalcWithCoordinate(int32_t targetCoordinate,STEP_MOTOR_INDEX motorIndex,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio);

//回零计算电机加速曲线
void StepMotorTableCalcWithReturnZero(STEP_MOTOR_INDEX motorIndex,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio);

//步进电机计算持续运转转速
void StepMotorTableCalcWithRunAlways(STEP_MOTOR_INDEX motorIndex,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio);

//计算步进电机减速曲线
void StepMotorTableCalcDaccTable(STEP_MOTOR_INDEX motorIndex);

#endif





