/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:37:13
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-25 17:40:57
**FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailTrans.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_RAIL_TRANS_H_
#define __SERVICE_IPC_RAIL_TRANS_H_
#include "ServiceIpcBase.h"


//初始化IPC调用数据结构
void IPC_RailTransDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailTransGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_RailTransWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


/****************************************************发出指令不等待完成*******************************************************/
//轨道复位,不等待完成
LH_ERR IPC_RailTransResetWhileAck(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);
//轨道移动到进样1,不等待完成
LH_ERR IPC_RailTransMove2PushIn1WhileAck(void);
//轨道移动到进样2,不等待完成
LH_ERR IPC_RailTransMove2PushIn2WhileAck(void);
//轨道移动到出样1,不等待完成
LH_ERR IPC_RailTransMove2PushOut1WhileAck(void);
//轨道移动到出样2,不等待完成
LH_ERR IPC_RailTransMove2PushOut2WhileAck(void);
//轨道运转到急诊位,不等待完成
LH_ERR IPC_RailTransMove2EmergencyCallWhileAck(void);
//轨道移动到旋转位1,不等待完成
LH_ERR IPC_RailTransMove2Rotate1WhileAck(void);
//轨道移动到旋转位2,不等待完成
LH_ERR IPC_RailTransMove2Rotate2WhileAck(void);
//轨道移动到靠现在位置最近的旋转位,不等待完成
LH_ERR IPC_RailTransMove2RotateAutoWhileAck(void);
//轨道移动到试管架扫码位,不等待完成
LH_ERR IPC_RailTransMove2RackBarScanWhileAck(RAIL_BAR_SCAN_POS scanPos);
//轨道移动到试管扫码位,不等待完成
LH_ERR IPC_RailTransMove2TubeBarScanWhileAck(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS scanPos);
//轨道移动到待测位,不等待完成
LH_ERR IPC_RailTransMove2WaitTestWhileAck(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo);
//轨道移动到重测位,不等待完成
LH_ERR IPC_RailTransMove2ReTestWhileAck(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo);
//轨道移动到测试区放架位,不等待完成
LH_ERR IPC_RailTransMove2TestingPutWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);
//轨道移动到测试区取架位,不等待完成
LH_ERR IPC_RailTransMove2TestingCatchWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);
//轨道移动到测试吸样位,不等待完成
LH_ERR IPC_RailTransMove2TestingAbsorbWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);
//获取当前急诊状态,不等待返回
LH_ERR IPC_RailTransGetCurrentEmergencyCallStateWhileAck(void);
//锁定急诊位,不等待返回
LH_ERR IPC_RailTransLockEmergencyCallWhileAck(RAIL_OUTPUT_STATE lockSet);

/****************************************************发出指令并等待完成********************************************************/
//轨道复位,等待完成
LH_ERR IPC_RailTransResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到进样1,等待完成
LH_ERR IPC_RailTransMove2PushIn1WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到进样2,等待完成
LH_ERR IPC_RailTransMove2PushIn2WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到出样1,等待完成
LH_ERR IPC_RailTransMove2PushOut1WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到出样2,等待完成
LH_ERR IPC_RailTransMove2PushOut2WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道运转到急诊位,等待完成
LH_ERR IPC_RailTransMove2EmergencyCallWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到旋转位1,等待完成
LH_ERR IPC_RailTransMove2Rotate1WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到旋转位2,等待完成
LH_ERR IPC_RailTransMove2Rotate2WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到靠现在位置最近的旋转位,等待完成
LH_ERR IPC_RailTransMove2RotateAutoWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到试管架扫码位,等待完成
LH_ERR IPC_RailTransMove2RackBarScanWhileReturn(RAIL_BAR_SCAN_POS scanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到试管扫码位,等待完成
LH_ERR IPC_RailTransMove2TubeBarScanWhileReturn(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS scanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到待测位,等待完成
LH_ERR IPC_RailTransMove2WaitTestWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到重测位,等待完成
LH_ERR IPC_RailTransMove2ReTestWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到测试区放架位,等待完成
LH_ERR IPC_RailTransMove2TestingPutWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到测试区取架位,等待完成
LH_ERR IPC_RailTransMove2TestingCatchWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到测试吸样位,等待完成
LH_ERR IPC_RailTransMove2TestingAbsorbWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//获取当前急诊状态,等待返回
LH_ERR IPC_RailTransGetCurrentEmergencyCallStateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//锁定急诊位,等待返回
LH_ERR IPC_RailTransLockEmergencyCallWhileReturn(RAIL_OUTPUT_STATE lockSet,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动集成扫码
LH_ERR IPC_RailTransMove2BarScanIntegrateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//轨道移动到集成扫码指定位置,等待完成
LH_ERR IPC_RailTransMove2BarScanPosIntegrateWhileReturn(RAIL_BAR_SCAN_POS scanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
#endif


