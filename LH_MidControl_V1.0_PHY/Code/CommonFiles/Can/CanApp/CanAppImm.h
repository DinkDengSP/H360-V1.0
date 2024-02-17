/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-06 15:37:27
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-23 20:31:45
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_APP_IMM_H_
#define __CAN_APP_IMM_H_
#include "CanBase.h"

//向免疫发送联机脱机指令
LH_ERR CanImmSendOnOffline(void);

//发送试剂余量探测序号
LH_ERR CanImmSendReagentQuantityPos(void);

//发送试剂余量探测
LH_ERR CanImmSendReagentQuantity(void);

//发送测试参数
LH_ERR CanImmSendTestParam(void);

//启动测试
LH_ERR CanImmSendWorkStart(void);

//发送多试剂位信息
LH_ERR CanImmSendMultiReagentInfo(void);

//发送加样方案
LH_ERR CanImmSendSampleSolution(void);

//发送三针补偿量
LH_ERR CanImmSendNeedleCompensateValue(void);

//发送试剂预留次数
LH_ERR CanImmSendReagentReserveInfo(void);

//发送加样暂停
LH_ERR CanImmSendAddSamplePause(void);

//发送加样继续
LH_ERR CanImmSendAddSampleContinue(void);

//发送试剂条码扫描
LH_ERR CanImmSendReagentBarCodeScan(void);

//发送更新垃圾桶计数
LH_ERR CanImmSendResetGarbageCount(void);

//发送待机时长更新
LH_ERR CanImmSendStandbyCycleUpdate(void);

//运行中发送更新测试参数
LH_ERR CanImmSendReflushTestParam(void);

//发送试管到位
LH_ERR CanImmSendTubeOnPosition(void);

//发送测试讯息
LH_ERR CanImmSendTestItemMsg(void);

//发送是否还有测试讯息
LH_ERR CanImmSendTestItemCount(void);

//发送试管移出完成
LH_ERR CanImmSendTubeMoveOutOver(void);

//发送本机轨道状态
LH_ERR CanImmSendLocalTrackState(void);

//发送联机轨道状态
LH_ERR CanImmSendOnlineTrackState(void);


#endif



