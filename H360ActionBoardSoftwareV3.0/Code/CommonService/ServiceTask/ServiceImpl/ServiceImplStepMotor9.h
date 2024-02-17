/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-10 10:00:48
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-11 09:42:05
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceImpl\ServiceImplStepMotor9.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_STEP_MOTOR9_H_
#define __SERVICE_IMPL_STEP_MOTOR9_H_
#include "CommonServiceImplBase.h"




//CAN总线接收数据处理,步进电机复位
LH_ERR ServiceImplStepMotor9ResetWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机运动指定步数
LH_ERR ServiceImplStepMotor9RunSpecialStepsWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机运动到指定坐标
LH_ERR ServiceImplStepMotor9RunToCoordinatesWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机回零
LH_ERR ServiceImplStepMotor9ReturnZeroWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机回零不复位
LH_ERR ServiceImplStepMotor9ReturnZeroWithoutResetWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机持续运转不停止
LH_ERR ServiceImplStepMotor9RunAlwaysWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机立即停止,不带减速
LH_ERR ServiceImplStepMotor9StopImmediatelyWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机减速停止,带减速
LH_ERR ServiceImplStepMotor9StopDecelerationWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机立即急停,除了复位,别的指令都会失效
LH_ERR ServiceImplStepMotor9StopEmergencyWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,读取步进电机当前的坐标
LH_ERR ServiceImplStepMotor9ReadCurrentPositionWithCanMsg(void* canMsgCachePtr);



//自身指令接收处理,步进电机复位
LH_ERR ServiceImplStepMotor9ResetWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机运行指定步数
LH_ERR ServiceImplStepMotor9RunStepsWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机运行到指定坐标
LH_ERR ServiceImplStepMotor9Run2CoordinatesWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机回零
LH_ERR ServiceImplStepMotor9ReturnZeroWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机回零但是不复位
LH_ERR ServiceImplStepMotor9ReturnZeroWithoutResetWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机持续运转不停止
LH_ERR ServiceImplStepMotor9RunAlwaysWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机立即停止
LH_ERR ServiceImplStepMotor9StopImmediatelyWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机减速停止
LH_ERR ServiceImplStepMotor9StopDecelerationWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机急停
LH_ERR ServiceImplStepMotor9StopEmergencyWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机读取当前电机位置
LH_ERR ServiceImplStepMotor9ReadCurrentPositionWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,设置电机状态
LH_ERR ServiceImplStepMotor9SetStateWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);


#endif

