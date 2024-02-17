/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:02:39
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-22 15:56:10
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplBufferSolutionInject.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_BUFFER_SOLUTION_INJECT_H_
#define __SERVICE_IMPL_BUFFER_SOLUTION_INJECT_H_
#include "ServiceImplBufferSolutionInjectConfig.h"

//自动运行状态机初始化
LH_ERR ServiceImplBufferSolutionInject_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplBufferSolutionInject_StateMachineLoop(void);

//缓冲液注液初始化,返回A瓶和B瓶的状态
LH_ERR ServiceImplBufferSolutionInject_Reset(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//缓冲液注液,可以选瓶,注液完成返回A瓶和B瓶的状态
LH_ERR ServiceImplBufferSolutionInject_Trig(BOTTLE_SELECT bottleSelect,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//缓冲液灌注,可以选瓶,注液完成返回A瓶和B瓶的状态
LH_ERR ServiceImplBufferSolutionInject_Prime(BOTTLE_SELECT bottleSelect,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplBufferSolutionInject_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplBufferSolutionInject_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplBufferSolutionInject_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplBufferSolutionInject_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);



#endif


