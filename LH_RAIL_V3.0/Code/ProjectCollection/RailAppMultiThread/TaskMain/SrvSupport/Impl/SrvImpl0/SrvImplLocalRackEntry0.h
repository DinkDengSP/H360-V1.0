/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-10-11 14:19:32
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-23 13:30:11
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SRV_IMPL_LOCAL_RACK_ENTRY0_H_
#define __SRV_IMPL_LOCAL_RACK_ENTRY0_H_
#include "SrvImplLocalRackEntry0Config.h"

//有限状态机初始化
void SrvImplLocalRackEntry0FSM_Init(void);

//有限状态机运行
void SrvImplLocalRackEntry0FSM_Run(void);

//读取指定输入
void SrvImplLocalRackEntry0ReadInput(uint8_t inputIndex,CAN1_ERR_SET* errorSet);

//写出指定输出
void SrvImplLocalRackEntry0WriteOutput(uint8_t outputIndex,uint8_t setValue,CAN1_ERR_SET* errorSet);

//电机复位
void SrvImplLocalRackEntry0StepMotorReset(uint8_t motorIndex,CAN1_ERR_SET* errorSet);

//电机走步数
void SrvImplLocalRackEntry0StepMotorRunSteps(uint8_t motorIndex,int32_t steps,CAN1_ERR_SET* errorSet);

//模块复位
void SrvImplLocalRackEntry0Reset(CAN1_ERR_SET* errorSet);

//节点卡住试管架
void SrvImplLocalRackEntry0SwitchBlock(CAN1_ERR_SET* errorSet);

//节点释放试管架
void SrvImplLocalRackEntry0SwitchRelease(CAN1_ERR_SET* errorSet);

//节点放行
void SrvImplLocalRackEntry0ReleaseRack(CAN1_ERR_SET* errorSet);

//轨道启动
void SrvImplLocalRackEntry0RailStart(LOCAL_RAIL_DIR railInDir,LOCAL_RAIL_DIR railOutDir,CAN1_ERR_SET* errorSet);

//轨道停止
void SrvImplLocalRackEntry0RailStop(CAN1_ERR_SET* errorSet);

//读取状态
void SrvImplLocalRackEntry0ReadState(CAN1_ERR_SET* errorSet);

//清除状态
void SrvImplLocalRackEntry0ClearState(CAN1_ERR_SET* errorSet);


#endif

