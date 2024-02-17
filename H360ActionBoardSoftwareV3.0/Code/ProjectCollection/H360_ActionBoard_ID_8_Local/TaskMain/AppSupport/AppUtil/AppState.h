/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:10:05
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-09 13:46:09
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_STATE_H_
#define __APP_STATE_H_
#include "AppStateDefault.h"

//APP状态初始化
void AppStateInit(void);

//APP全部的状态恢复到默认值
void AppStateSetAllDefault(void);

//设置默认值
void AppStateSetRackStoreInDefault(void);
//获取状态指针
APP_STATE_RACK_STORE_IN* AppStateReadRackStoreInState(void);

//设置默认值
void AppStateSetRackStoreOutDefault(void);
//获取状态指针
APP_STATE_RACK_STORE_OUT* AppStateReadRackStoreOutState(void);

//设置默认值
void AppStateSetRackEntryDefault(void);
//获取状态指针
APP_STATE_RACK_ENTRY* AppStateReadRackEntryState(void);

//设置默认值
void AppStateSetRackBarScanDefault(void);
//获取状态指针
APP_STATE_RACK_BAR_SCAN* AppStateReadRackBarScanState(void);

//设置默认值
void AppStateSetRackTestDefault(void);
//获取状态指针
APP_STATE_RACK_TEST* AppStateReadRackTestState(void);

//设置默认值
void AppStateSetRackInSwitchDefault(void);
//获取状态指针
APP_STATE_RACK_IN_SWITCH* AppStateReadRackInSwitchState(void);

//设置默认值
void AppStateSetRackWaitOut1Default(void);
//获取状态指针
APP_STATE_RACK_WAIT_OUT1* AppStateReadRackWaitOut1State(void);

//设置默认值
void AppStateSetRackWaitOut2Default(void);
//获取状态指针
APP_STATE_RACK_WAIT_OUT2* AppStateReadRackWaitOut2State(void);

//设置默认值
void AppStateSetRackOutSwitchDefault(void);
//获取状态指针
APP_STATE_RACK_OUT_SWITCH* AppStateReadRackOutSwitchState(void);



#endif
