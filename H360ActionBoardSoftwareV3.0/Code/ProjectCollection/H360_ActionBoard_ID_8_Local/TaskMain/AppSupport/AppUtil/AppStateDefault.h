/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:10:33
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-11-09 13:38:00
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_STATE_DEFAULT_H_
#define __APP_STATE_DEFAULT_H_
#include "AppStateDataType.h"

//默认状态-进样仓
extern const APP_STATE_RACK_STORE_IN stateRackStoreInDefault;

//默认状态-出样仓
extern const APP_STATE_RACK_STORE_OUT stateRackStoreOutDefault;

//默认状态-轨道进样节点
extern const APP_STATE_RACK_ENTRY stateRackEntryDefault;

//默认状态-轨道进样扫码节点
extern const APP_STATE_RACK_BAR_SCAN stateRackBarScanDefault;

//默认状态-轨道进样测试节点
extern const APP_STATE_RACK_TEST stateRackTestDefault;

//默认状态-轨道进样换轨节点
extern const APP_STATE_RACK_IN_SWITCH stateRackInSwitchDefault;

//默认状态-轨道出样等待节点1
extern const APP_STATE_RACK_WAIT_OUT1 stateRackWaitOut1Default;

//默认状态-轨道出样等待节点2
extern const APP_STATE_RACK_WAIT_OUT2 stateRackWaitOut2Default;

//默认状态-轨道出样换轨节点
extern const APP_STATE_RACK_OUT_SWITCH stateRackOutSwitchDefault;


#endif
