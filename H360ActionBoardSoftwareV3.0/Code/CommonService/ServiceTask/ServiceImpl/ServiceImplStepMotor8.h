/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-10 10:00:38
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-11 09:41:49
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceImpl\ServiceImplStepMotor8.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_STEP_MOTOR8_H_
#define __SERVICE_IMPL_STEP_MOTOR8_H_
#include "CommonServiceImplBase.h"




//CAN总线接收数据处理,步进电机复位
LH_ERR ServiceImplStepMotor8ResetWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机运动指定步数
LH_ERR ServiceImplStepMotor8RunSpecialStepsWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机运动到指定坐标
LH_ERR ServiceImplStepMotor8RunToCoordinatesWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机回零
LH_ERR ServiceImplStepMotor8ReturnZeroWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机回零不复位
LH_ERR ServiceImplStepMotor8ReturnZeroWithoutResetWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机持续运转不停止
LH_ERR ServiceImplStepMotor8RunAlwaysWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机立即停止,不带减速
LH_ERR ServiceImplStepMotor8StopImmediatelyWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机减速停止,带减速
LH_ERR ServiceImplStepMotor8StopDecelerationWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机立即急停,除了复位,别的指令都会失效
LH_ERR ServiceImplStepMotor8StopEmergencyWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,读取步进电机当前的坐标
LH_ERR ServiceImplStepMotor8ReadCurrentPositionWithCanMsg(void* canMsgCachePtr);



//自身指令接收处理,步进电机复位
LH_ERR ServiceImplStepMotor8ResetWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机运行指定步数
LH_ERR ServiceImplStepMotor8RunStepsWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机运行到指定坐标
LH_ERR ServiceImplStepMotor8Run2CoordinatesWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机回零
LH_ERR ServiceImplStepMotor8ReturnZeroWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机回零但是不复位
LH_ERR ServiceImplStepMotor8ReturnZeroWithoutResetWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机持续运转不停止
LH_ERR ServiceImplStepMotor8RunAlwaysWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机立即停止
LH_ERR ServiceImplStepMotor8StopImmediatelyWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机减速停止
LH_ERR ServiceImplStepMotor8StopDecelerationWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机急停
LH_ERR ServiceImplStepMotor8StopEmergencyWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机读取当前电机位置
LH_ERR ServiceImplStepMotor8ReadCurrentPositionWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,设置电机状态
LH_ERR ServiceImplStepMotor8SetStateWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);


#endif

