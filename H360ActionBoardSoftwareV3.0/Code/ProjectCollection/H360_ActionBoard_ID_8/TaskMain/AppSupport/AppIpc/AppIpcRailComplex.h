/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 14:50:20
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcRailComplex.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_RAIL_COMPLEX_H_
#define __APP_IPC_RAIL_COMPLEX_H_
#include "AppIpcBase.h"

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_RailComplexDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailComplexGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_RailComplexWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//轨道整体复位
LH_ERR IPC_RailComplexResetAllWhileAck(void);
//设置轨道模块数量
LH_ERR IPC_RailComplexSetModuleCountWhileAck(uint8_t moduleCount);
//双轨道同时进架多次直到触发
LH_ERR IPC_RailComplexPushInTrigMultiTogetherWhileAck(uint8_t retryMax);


/*****************************************************发出指令等待完成*******************************************************/
//轨道整体复位
LH_ERR IPC_RailComplexResetAllWhileReturn(void);
//设置轨道模块数量
LH_ERR IPC_RailComplexSetModuleCountWhileReturn(uint8_t moduleCount);
//双轨道同时进架多次直到触发
LH_ERR IPC_RailComplexPushInTrigMultiTogetherWhileReturn(uint8_t retryMax,SENSOR_STATE_PUSHIN_EXIST* pushin1SensorPtr,SENSOR_STATE_PUSHIN_EXIST* pushin2SensorPtr);


/****************************************************获取运行的有效数据*********************************************************/
//查询上次执行结果,进样1和进样2的状态
void IPC_RailComplexGetPushInState(SENSOR_STATE_PUSHIN_EXIST* pushin1SensorPtr,SENSOR_STATE_PUSHIN_EXIST* pushin2SensorPtr);


#endif
