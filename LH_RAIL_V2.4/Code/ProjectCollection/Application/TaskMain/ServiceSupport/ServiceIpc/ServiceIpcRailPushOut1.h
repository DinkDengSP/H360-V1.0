/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:36:06
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-24 16:47:52
 *FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailPushOut1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_RAIL_PUSH_OUT1_H_
#define __SERVICE_IPC_RAIL_PUSH_OUT1_H_
#include "ServiceIpcBase.h"


//初始化IPC调用数据结构
void IPC_RailPushOut1DataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailPushOut1GetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_RailPushOut1WaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

/****************************************************发出指令不等待完成*******************************************************/
//出样1复位
LH_ERR IPC_RailPushOut1ResetWhileAck(RAIL_RESET_CORRECT_FLAG correctFlag);
//出样1推进一次架子并回零
LH_ERR IPC_RailPushOut1PushOnceWithReturnZeroWhileAck(void);
//出样1推进一次架子,但是不回零
LH_ERR IPC_RailPushOut1PushOnceNotReturnZeroWhileAck(void);
//出样1回零
LH_ERR IPC_RailPushOut1ReturnZeroWhileAck(void);
//获取当前出样1所处的位置
LH_ERR IPC_RailPushOut1GetIndexWhileAck(void);
//出样将试管架推到顶部
LH_ERR IPC_RailPushOut1PushRackTopWhileAck(void);
//获取出样传感器状态
LH_ERR IPC_RailPushOut1GetSensorStateWhileAck(void);
//出样1状态控制
LH_ERR IPC_RailPushOut1SetStateWhileAck(RAIL_OUTPUT_STATE railState);

/****************************************************发出指令并等待完成********************************************************/
//出样1复位,等待完成
LH_ERR IPC_RailPushOut1ResetWhileReturn(RAIL_RESET_CORRECT_FLAG correctFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样1推进一次架子并回零,等待完成
LH_ERR IPC_RailPushOut1PushOnceWithReturnZeroWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样1推进一次架子,但是不回零,等待完成
LH_ERR IPC_RailPushOut1PushOnceNotReturnZeroWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样1回零,等待完成
LH_ERR IPC_RailPushOut1ReturnZeroWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//获取当前出样1所处的位置
LH_ERR IPC_RailPushOut1GetIndexWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样将试管架推到顶部
LH_ERR IPC_RailPushOut1PushRackTopWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//获取出样传感器状态
LH_ERR IPC_RailPushOut1GetSensorStateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样1状态控制
LH_ERR IPC_RailPushOut1SetStateWhileReturn(RAIL_OUTPUT_STATE railState,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
#endif


