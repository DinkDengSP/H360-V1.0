/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-10 10:00:17
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-11 09:40:35
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceImpl\ServiceImplStepMotor3.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_STEP_MOTOR3_H_
#define __SERVICE_IMPL_STEP_MOTOR3_H_
#include "CommonServiceImplBase.h"




//CAN总线接收数据处理,步进电机复位
LH_ERR ServiceImplStepMotor3ResetWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机运动指定步数
LH_ERR ServiceImplStepMotor3RunSpecialStepsWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机运动到指定坐标
LH_ERR ServiceImplStepMotor3RunToCoordinatesWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机回零
LH_ERR ServiceImplStepMotor3ReturnZeroWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机回零不复位
LH_ERR ServiceImplStepMotor3ReturnZeroWithoutResetWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机持续运转不停止
LH_ERR ServiceImplStepMotor3RunAlwaysWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机立即停止,不带减速
LH_ERR ServiceImplStepMotor3StopImmediatelyWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机减速停止,带减速
LH_ERR ServiceImplStepMotor3StopDecelerationWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,步进电机立即急停,除了复位,别的指令都会失效
LH_ERR ServiceImplStepMotor3StopEmergencyWithCanMsg(void* canMsgCachePtr);

//CAN总线接收数据处理,读取步进电机当前的坐标
LH_ERR ServiceImplStepMotor3ReadCurrentPositionWithCanMsg(void* canMsgCachePtr);



//自身指令接收处理,步进电机复位
LH_ERR ServiceImplStepMotor3ResetWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机运行指定步数
LH_ERR ServiceImplStepMotor3RunStepsWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机运行到指定坐标
LH_ERR ServiceImplStepMotor3Run2CoordinatesWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机回零
LH_ERR ServiceImplStepMotor3ReturnZeroWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机回零但是不复位
LH_ERR ServiceImplStepMotor3ReturnZeroWithoutResetWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机持续运转不停止
LH_ERR ServiceImplStepMotor3RunAlwaysWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机立即停止
LH_ERR ServiceImplStepMotor3StopImmediatelyWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机减速停止
LH_ERR ServiceImplStepMotor3StopDecelerationWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机急停
LH_ERR ServiceImplStepMotor3StopEmergencyWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,步进电机读取当前电机位置
LH_ERR ServiceImplStepMotor3ReadCurrentPositionWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);

//自身指令接收处理,设置电机状态
LH_ERR ServiceImplStepMotor3SetStateWithSelfMsg(SYSTEM_CMD_SELF* selfCmdPtr);


#endif

