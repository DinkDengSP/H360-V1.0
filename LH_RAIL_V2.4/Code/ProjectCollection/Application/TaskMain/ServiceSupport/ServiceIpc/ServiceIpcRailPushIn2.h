/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:35:59
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-24 16:46:55
 *FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailPushIn2.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_RAIL_PUSH_IN2_H_
#define __SERVICE_IPC_RAIL_PUSH_IN2_H_
#include "ServiceIpcBase.h"


//初始化IPC调用数据结构
void IPC_RailPushIn2DataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailPushIn2GetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_RailPushIn2WaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


/****************************************************发出指令不等待完成*******************************************************/
//进样2复位,不等待完成
LH_ERR IPC_RailPushIn2ResetWhileAck(RAIL_RESET_CORRECT_FLAG correctFlag);
//进样2推进一次,不等待完成
LH_ERR IPC_RailPushIn2PushOnceWhileAck(void);
//进样2推进多次直到传感器触发,不等待完成
LH_ERR IPC_RailPushIn2PushMultiWhileSensorTrigWhileAck(uint16_t retryMax);
//进样2获取传感器状态
LH_ERR IPC_RailPushIn2GetSensorStateWhileAck(void);


/****************************************************发出指令并等待完成********************************************************/
//进样2复位,等待完成
LH_ERR IPC_RailPushIn2ResetWhileReturn(RAIL_RESET_CORRECT_FLAG correctFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//进样2推进一次,等待完成
LH_ERR IPC_RailPushIn2PushOnceWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//进样2推进多次直到传感器触发,等待完成
LH_ERR IPC_RailPushIn2PushMultiWhileSensorTrigWhileReturn(uint16_t retryMax,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//进样2获取传感器状态
LH_ERR IPC_RailPushIn2GetSensorStateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);



#endif


