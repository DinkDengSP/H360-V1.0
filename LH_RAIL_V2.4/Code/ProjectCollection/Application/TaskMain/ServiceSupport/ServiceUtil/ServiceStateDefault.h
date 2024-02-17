/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:01:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-25 09:36:01
 *FilePath: \App\TaskMain\ServiceSupport\ServiceUtil\ServiceStateDefault.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_STATE_DEFAULT_H_
#define __SERVICE_STATE_DEFAULT_H_
#include "ServiceStateType.h"

//进样1默认状态
extern const SERVICE_STATE_PUSH_IN pushIn1StateDefault;

//进样2默认状态
extern const SERVICE_STATE_PUSH_IN pushIn2StateDefault;

//出样1默认状态
extern const SERVICE_STATE_PUSH_OUT pushOut1StateDefault;

//出样2默认状态
extern const SERVICE_STATE_PUSH_OUT pushOut2StateDefault;

//轨道机械手默认状态
extern const SERVICE_STATE_RAIL_HAND railHandStateDefault;

//轨道传送带默认状态
extern const SERVICE_STATE_RAIL_TRANS railTransStateDefault;

//轨道条码枪默认数据
extern const SERVICE_STATE_RAIL_BARSCAN railBarScanStateDefault;

//轨道复合状态默认状态
extern const SERVICE_STATE_RAIL_COMPLEX railComplexStateDefault;

//轨道辅助功能默认状态
extern const SERVICE_STATE_RAIL_UTIL railUtilStateDefault;

//集成扫码结果
extern const SERVICE_STATE_BARSCAN_RESULT_INTEGRATE barScanResultIntegrateDefault;



#endif




