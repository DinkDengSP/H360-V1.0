/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-10-11 17:32:23
 *LastEditors: DengXiaoJun
 *LastEditTime: 2022-01-24 14:40:57
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SRV_IMPL_ONLINE_TUBE_ENTRY0_H_
#define __SRV_IMPL_ONLINE_TUBE_ENTRY0_H_
#include "SrvImplOnlineTubeEntry0Config.h"

//有限状态机初始化
void SrvImplOnlineTubeEntry0FSM_Init(void);

//有限状态机运行
void SrvImplOnlineTubeEntry0FSM_Run(void);

//联机自检
void SrvImplOnlineTubeEntry0SelfCheck(CAN1_ERR_SET* errorSet);

//读取输入
void SrvImplOnlineTubeEntry0ReadInput(uint8_t inputIndex,CAN1_ERR_SET* errorSet);

//写入输出
void SrvImplOnlineTubeEntry0WriteOutput(uint8_t outputIndex,uint8_t setValue,CAN1_ERR_SET* errorSet);

//电机复位
void SrvImplOnlineTubeEntry0StepMotorReset(uint8_t motorIndex,CAN1_ERR_SET* errorSet);

//电机走步数
void SrvImplOnlineTubeEntry0StepMotorRunSteps(uint8_t motorIndex,int32_t steps,CAN1_ERR_SET* errorSet);

//输入节点复位
void SrvImplOnlineTubeEntry0Reset(CAN1_ERR_SET* errorSet);

//清除节点状态
void SrvImplOnlineTubeEntry0ClearState(CAN1_ERR_SET* errorSet);

//读传感器节点信息
void SrvImplOnlineTubeEntry0ReadState(CAN1_ERR_SET* errorSet);

//轨道启动运转
void SrvImplOnlineTubeEntry0RailStart(CAN1_ERR_SET* errorSet);

//轨道停止运转
void SrvImplOnlineTubeEntry0RailStop(CAN1_ERR_SET* errorSet);

//查询流水线是否存在试管,发送指令
void SrvImplOnlineTubeEntry0QueryTube(CAN1_ERR_SET* errorSet);

//申请试管进入,发送指令
void SrvImplOnlineTubeEntry0ApplyHostTubeIn(CAN1_ERR_SET* errorSet);

//读取转盘入口讯息
void SrvImplOnlineTubeEntry0ReadRotateInput(CAN1_ERR_SET* errorSet);

#endif

