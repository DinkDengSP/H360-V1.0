/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:58:24
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-28 11:22:33
 *FilePath: \App\TaskMain\AppSupport\AppIpc\AppIpcSampleRackMove.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_SAMPLE_RACK_MOVE_H_
#define __APP_IPC_SAMPLE_RACK_MOVE_H_
#include "AppIpcBase.h"


//初始化IPC调用数据结构
void IPC_SampleRackMoveDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_SampleRackMoveGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_SampleRackMoveWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);



/****************************************************发出指令不等待完成*******************************************************/
//从进样位置抓取试管架到扫码位置,不等待完成
LH_ERR IPC_SampleRackMoveCatchPushInToRackBarScanWhileAck(uint8_t pushinIndex);
//从急诊抓架子到扫码位置
LH_ERR IPC_SampleRackMoveCatchEmergencyCallToRackBarScanWhileAck(void);
//将扫码完成的试管架移动到指定位置,不等待完成
LH_ERR IPC_SampleRackMovePutScanedRack2AnyWhereWhileAck(uint16_t rackID,uint8_t targetModuleID,uint8_t targetMainRegion,uint8_t targetSubRegion);
//机械手带着试管架扫码,不等待完成
LH_ERR IPC_SampleRackMoveMoveRackAndScanWhileAck(void);
//机械手从待测区取架子到指定位置,不等待完成
LH_ERR IPC_SampleRackMoveCatchRackFromWaitTest2AnyWhereWhileAck(uint16_t rackID,uint8_t srcModuleID,uint8_t targetModuleId,uint8_t targetMainRegion,uint8_t targetSubRegion);
//机械手从重测位取架子到指定位置,不等待完成
LH_ERR IPC_SampleRackMoveCatchRackFromRepeatTest2AnyWhereWhileAck(uint16_t rackID,uint8_t srcModuleID,uint8_t targetModuleId,uint8_t targetMainRegion,uint8_t targetSubRegion);
//机械手从测试区取架子到指定位置,不等待完成
LH_ERR IPC_SampleRackMoveCatchRackFromTesting2AnyWhereWhileAck(uint16_t rackID,uint8_t srcModuleID,uint8_t targetModuleId,uint8_t targetMainRegion,uint8_t targetSubRegion);


/****************************************************发出指令并等待完成********************************************************/
//从进样位置抓取试管架到扫码位置,等待完成
LH_ERR IPC_SampleRackMoveCatchPushInToRackBarScanWhileReturn(uint8_t pushinIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//从急诊抓架子到扫码位置
LH_ERR IPC_SampleRackMoveCatchEmergencyCallToRackBarScanWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//将扫码完成的试管架移动到指定位置,等待完成
LH_ERR IPC_SampleRackMovePutScanedRack2AnyWhereWhileReturn(uint16_t rackID,uint8_t targetModuleID,uint8_t targetMainRegion,uint8_t targetSubRegion,
                                                                CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手带着试管架扫码,等待完成
LH_ERR IPC_SampleRackMoveMoveRackAndScanWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手从待测区取架子到指定位置,等待完成
LH_ERR IPC_SampleRackMoveCatchRackFromWaitTest2AnyWhereWhileReturn(uint16_t rackID,uint8_t srcModuleID,uint8_t targetModuleId,uint8_t targetMainRegion,uint8_t targetSubRegion,
                                                                    CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手从重测位取架子到指定位置,等待完成
LH_ERR IPC_SampleRackMoveCatchRackFromRepeatTest2AnyWhereWhileReturn(uint16_t rackID,uint8_t srcModuleID,uint8_t targetModuleId,uint8_t targetMainRegion,uint8_t targetSubRegion,
                                                                        CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
                                                                        
//机械手从测试区取架子到指定位置,等待完成
LH_ERR IPC_SampleRackMoveCatchRackFromTesting2AnyWhereWhileReturn(uint16_t rackID,uint8_t srcModuleID,uint8_t targetModuleId,uint8_t targetMainRegion,uint8_t targetSubRegion,
                                                                    CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);




#endif





