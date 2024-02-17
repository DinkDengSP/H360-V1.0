/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-26 10:25:56
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-26 10:54:55
 *FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcAssist.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_ASSIST_H_
#define __SERVICE_IPC_ASSIST_H_
#include "ServiceIpcBase.h"

//初始化IPC调用数据结构
void IPC_AssistDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_AssistGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_AssistWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

/****************************************************发出指令不等待完成*******************************************************/
//读取版本号,不等待返回
LH_ERR IPC_AssistReadBoardVersionWhileAck(uint8_t boardID);
//通讯握手,不等待返回
LH_ERR IPC_AssistHandShakeWhileAck(uint8_t handShakeMode);
//系统急停,不等待返回
LH_ERR IPC_AssistEmergencyStopWhileAck(void);

/****************************************************发出指令并等待完成********************************************************/
//读取版本号,等待返回
LH_ERR IPC_AssistReadBoardVersionWhileReturn(uint8_t boardID,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//通讯握手,等待返回
LH_ERR IPC_AssistHandShakeWhileReturn(uint8_t handShakeMode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//系统急停,等待返回
LH_ERR IPC_AssistEmergencyStopWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

#endif






