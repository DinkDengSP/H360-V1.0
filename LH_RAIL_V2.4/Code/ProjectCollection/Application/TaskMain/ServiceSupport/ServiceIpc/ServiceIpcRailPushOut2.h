/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:36:12
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-24 16:48:45
 *FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailPushOut2.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_RAIL_PUSH_OUT2_H_
#define __SERVICE_IPC_RAIL_PUSH_OUT2_H_
#include "ServiceIpcBase.h"



//初始化IPC调用数据结构
void IPC_RailPushOut2DataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailPushOut2GetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_RailPushOut2WaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


/****************************************************发出指令不等待完成*******************************************************/
//出样2复位
LH_ERR IPC_RailPushOut2ResetWhileAck(RAIL_RESET_CORRECT_FLAG correctFlag);
//出样2推进一次架子并回零
LH_ERR IPC_RailPushOut2PushOnceWithReturnZeroWhileAck(void);
//出样2推进一次架子,但是不回零
LH_ERR IPC_RailPushOut2PushOnceNotReturnZeroWhileAck(void);
//出样2回零
LH_ERR IPC_RailPushOut2ReturnZeroWhileAck(void);
//获取当前出样2所处的位置
LH_ERR IPC_RailPushOut2GetIndexWhileAck(void);
//出样将试管架推到顶部
LH_ERR IPC_RailPushOut2PushRackTopWhileAck(void);
//获取出样传感器状态
LH_ERR IPC_RailPushOut2GetSensorStateWhileAck(void);
//出样2状态控制
LH_ERR IPC_RailPushOut2SetStateWhileAck(RAIL_OUTPUT_STATE railState);

/****************************************************发出指令并等待完成********************************************************/
//出样2复位,等待完成
LH_ERR IPC_RailPushOut2ResetWhileReturn(RAIL_RESET_CORRECT_FLAG correctFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样2推进一次架子并回零,等待完成
LH_ERR IPC_RailPushOut2PushOnceWithReturnZeroWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样2推进一次架子,但是不回零,等待完成
LH_ERR IPC_RailPushOut2PushOnceNotReturnZeroWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样2回零,等待完成
LH_ERR IPC_RailPushOut2ReturnZeroWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//获取当前出样2所处的位置
LH_ERR IPC_RailPushOut2GetIndexWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样将试管架推到顶部
LH_ERR IPC_RailPushOut2PushRackTopWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//获取出样传感器状态
LH_ERR IPC_RailPushOut2GetSensorStateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//出样2状态控制
LH_ERR IPC_RailPushOut2SetStateWhileReturn(RAIL_OUTPUT_STATE railState,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

#endif


