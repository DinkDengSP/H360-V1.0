/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 14:51:42
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcRailTrans.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_RAIL_TRANS_H_
#define __APP_IPC_RAIL_TRANS_H_
#include "AppIpcBase.h"

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_RailTransDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailTransGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_RailTransWaitLastReturn(void);

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
LH_ERR IPC_RailTransMove2SpecialModuleWaitTestWhileAck(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo);
//轨道移动到重测位,不等待完成
LH_ERR IPC_RailTransMove2SpecialModuleReTestWhileAck(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo);
//轨道移动到测试区放架位,不等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingPutWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);
//轨道移动到测试区取架位,不等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingCatchWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);
//轨道移动到测试吸样位,不等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingAbsorbWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);

/****************************************************发出指令并等待完成********************************************************/
//轨道复位,等待完成
LH_ERR IPC_RailTransResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);
//轨道移动到进样1,等待完成
LH_ERR IPC_RailTransMove2PushIn1WhileReturn(void);
//轨道移动到进样2,等待完成
LH_ERR IPC_RailTransMove2PushIn2WhileReturn(void);
//轨道移动到出样1,等待完成
LH_ERR IPC_RailTransMove2PushOut1WhileReturn(void);
//轨道移动到出样2,等待完成
LH_ERR IPC_RailTransMove2PushOut2WhileReturn(void);
//轨道运转到急诊位,等待完成
LH_ERR IPC_RailTransMove2EmergencyCallWhileReturn(void);
//轨道移动到旋转位1,等待完成
LH_ERR IPC_RailTransMove2Rotate1WhileReturn(void);
//轨道移动到旋转位2,等待完成
LH_ERR IPC_RailTransMove2Rotate2WhileReturn(void);
//轨道移动到靠现在位置最近的旋转位,等待完成
LH_ERR IPC_RailTransMove2RotateAutoWhileReturn(void);
//轨道移动到试管架扫码位,等待完成
LH_ERR IPC_RailTransMove2RackBarScanWhileReturn(RAIL_BAR_SCAN_POS scanPos);
//轨道移动到试管扫码位,等待完成
LH_ERR IPC_RailTransMove2TubeBarScanWhileReturn(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS scanPos);
//轨道移动到待测位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleWaitTestWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo);
//轨道移动到重测位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleReTestWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo);
//轨道移动到测试区放架位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingPutWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);
//轨道移动到测试区取架位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingCatchWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);
//轨道移动到测试吸样位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingAbsorbWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);

/****************************************************获取运行的有效数据*********************************************************/
//获取当前轨道所处的位置
void IPC_RailTransGetCurrentModuleAndRegion(RAIL_MODULE_POS* currentMainModulePtr,RAIL_SUB_MODULE* currentSubRegionPtr);


#endif
