/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-14 14:48:41
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcPushIn1.h
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_PUSH_IN1_H_
#define __APP_IPC_PUSH_IN1_H_
#include "AppIpcBase.h"


/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_PushIn1DataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_PushIn1GetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_PushIn1WaitLastReturn(void);


/****************************************************发出指令不等待完成*******************************************************/
//进样1复位,不等待完成
LH_ERR IPC_PushIn1ResetWhileAck(RAIL_RESET_CORRECT_FLAG correctFlag);
//进样1推进一次,不等待完成
LH_ERR IPC_PushIn1PushOnceWhileAck(void);
//进样1推进多次直到传感器触发,不等待完成
LH_ERR IPC_PushIn1PushMultiWhileSensorTrigWhileAck(uint16_t retryMax);


/****************************************************发出指令并等待完成********************************************************/
//进样1复位,等待完成
LH_ERR IPC_PushIn1ResetWhileReturn(RAIL_RESET_CORRECT_FLAG correctFlag);
//进样1推进一次,等待完成
LH_ERR IPC_PushIn1PushOnceWhileReturn(SENSOR_STATE_PUSHIN_EXIST* rackExistSensorStatePtr);
//进样1推进多次直到传感器触发,等待完成
LH_ERR IPC_PushIn1PushMultiWhileSensorTrigWhileReturn(uint16_t retryMax,SENSOR_STATE_PUSHIN_EXIST* rackExistSensorStatePtr);


/****************************************************获取运行的有效数据*********************************************************/
//读取上次动作返回的传感器状态
SENSOR_STATE_PUSHIN_EXIST IPC_PushIn1ReadCurrentRackExistState(void);

#endif





