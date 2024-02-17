/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:35:52
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-24 16:45:59
 *FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailPushIn1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_RAIL_PUSH_IN1_H_
#define __SERVICE_IPC_RAIL_PUSH_IN1_H_
#include "ServiceIpcBase.h"


//初始化IPC调用数据结构
void IPC_RailPushIn1DataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailPushIn1GetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_RailPushIn1WaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


/****************************************************发出指令不等待完成*******************************************************/
//进样1复位,不等待完成
LH_ERR IPC_RailPushIn1ResetWhileAck(RAIL_RESET_CORRECT_FLAG correctFlag);
//进样1推进一次,不等待完成
LH_ERR IPC_RailPushIn1PushOnceWhileAck(void);
//进样1推进多次直到传感器触发,不等待完成
LH_ERR IPC_RailPushIn1PushMultiWhileSensorTrigWhileAck(uint16_t retryMax);
//进样1获取传感器状态
LH_ERR IPC_RailPushIn1GetSensorStateWhileAck(void);


/****************************************************发出指令并等待完成********************************************************/
//进样1复位,等待完成
LH_ERR IPC_RailPushIn1ResetWhileReturn(RAIL_RESET_CORRECT_FLAG correctFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//进样1推进一次,等待完成
LH_ERR IPC_RailPushIn1PushOnceWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//进样1推进多次直到传感器触发,等待完成
LH_ERR IPC_RailPushIn1PushMultiWhileSensorTrigWhileReturn(uint16_t retryMax,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//进样1获取传感器状态
LH_ERR IPC_RailPushIn1GetSensorStateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


#endif

