/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-10 22:54:24
 *FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotor.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_H_
#define __STEP_MOTOR_H_
#include "StepMotorTable.h"

extern STEP_MOTOR_VAR* stepMotorVar[SYSTEM_STEP_MOTOR_COUNT];

//步进电机协议参数读取
uint32_t StepMotorGetParamWithProtocol(uint8_t motorIndex,uint8_t paramIndex,uint16_t utilParamIndex);

//步进电机协议参数写入
void StepMotorSetParamWithProtocol(uint8_t motorIndex,uint8_t paramIndex,uint16_t utilParamIndex,uint32_t writeParam);

//保存步进电机参数
void StepMotorSaveParamWithProtocol(void);


//步进电机参数变量的初始化
void StepMotorVarInit(void);

/*****************************这一块不能被外部调用****************************/
//将步进电机错误代码转换为系统错误代码
LH_ERR StepMotorConvert2LH_ErrCode(STEP_MOTOR_SUB_STATE state);
//步进电机CPLD中断回调函数状态机,被电机中断调用
//注意,该函数内部千万不要调用操作系统方法
void StepMotorCPLD_CallBackProcessWithStateMachine(STEP_MOTOR_INDEX motorIndex);
//辅助复位状态机
void CpldStepMotorResetProc(STEP_MOTOR_INDEX motorIndex);
//走步数
void CpldStepMotorRunStepsProc(STEP_MOTOR_INDEX motorIndex);
//走坐标
void CpldStepMotorRunCoordinate(STEP_MOTOR_INDEX motorIndex);
//持续运行
void CpldStepMotorRunAlways(STEP_MOTOR_INDEX motorIndex);
//减速运行
void CpldStepMotorRunDacc(STEP_MOTOR_INDEX motorIndex);
/******************************************************************************/

//指定步进电机复位
LH_ERR StepMotorReset(StepMotorResetCmd* resetCmdPtr);

//指定步进电机运行指定步数
LH_ERR StepMotorRunSpecialSteps(StepMotorRunStepsCmd* runStepConfigPtr);

//指定步进电机运行到指定位置
LH_ERR StepMotorRun2SpecialCoordinates(StepMotorRun2CoordationCmd* runCoordinatesConfigPtr);

//指定步进电机回零
LH_ERR StepMotorReturnZeroWithSensor(StepMotorReturnZeroCmd* returnZeroConfigPtr);

//指令步进电机回零,但是不进行复位动作,该指令只要用于泵回零,先找到零点,然后关阀然后复位的过程
//防止产生真空负压
LH_ERR StepMotorReturnZeroWithSensorRemoveResetStage(StepMotorReturnZeroCmd* returnZeroConfigPtr);

//指定步进电机持续运转
LH_ERR StepMotorRunAlways(StepMotorRunAlwaysCmd* runAlwaysConfigPtr);

//指定步进电机立即停止
LH_ERR StepMotorStopImmediately(STEP_MOTOR_INDEX motorIndex);

//指定步进电机减速停止
LH_ERR StepMotorStopDeceleration(STEP_MOTOR_INDEX motorIndex);

//指定步进电机急停
LH_ERR StepMotorStopEmergency(STEP_MOTOR_INDEX motorIndex);

#endif





