/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:36:19
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-24 16:53:08
 *FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailComplex.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_RAIL_COMPLEX_H_
#define __SERVICE_IPC_RAIL_COMPLEX_H_
#include "ServiceIpcBase.h"


//初始化IPC调用数据结构
void IPC_RailComplexDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailComplexGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_RailComplexWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

/****************************************************发出指令不等待完成*******************************************************/
//电机全部复位,不等待返回
LH_ERR IPC_RailComplexResetAllWhileAck(void);
//设置模块数量,不等待返回
LH_ERR IPC_RailComplexSetModuleCountWhileAck(uint8_t setCount);
//进样1 2同时推进一次,不等待返回
LH_ERR IPC_RailComplexPushMultiWhileTrigWhileAck(void);
//自动校准
LH_ERR IPC_RailComplexSelfAdjustWhileAck(void);

/****************************************************发出指令并等待完成********************************************************/
//电机全部复位,等待返回
LH_ERR IPC_RailComplexResetAllWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//设置模块数量,等待返回
LH_ERR IPC_RailComplexSetModuleCountWhileReturn(uint8_t setCount,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//进样1 2同时推进一次,等待返回
LH_ERR IPC_RailComplexPushMultiWhileTrigWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//自动校准
LH_ERR IPC_RailComplexSelfAdjustWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


#endif


