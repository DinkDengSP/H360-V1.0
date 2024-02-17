/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:10:00
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-09 13:55:02
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppState.h"

static APP_STATE* appStatePtr = NULL;

void AppStateInit(void)
{
    if(appStatePtr != NULL)
    {
        UserMemFree(SRAM_IS62,appStatePtr);
    }
    appStatePtr = NULL;
    appStatePtr = UserMemMallocWhileSuccess(SRAM_IS62,LENGTH_APP_STATE);
    //状态设置为默认状态
    AppStateSetAllDefault();
}

//APP全部的状态恢复到默认值
void AppStateSetAllDefault(void)
{
    //填充默认值
    UserMemCopy((void*)(&(appStatePtr->stateRackStoreIn)),  (void*)(&stateRackStoreInDefault)   ,LENGTH_APP_STATE_RACK_STORE_IN);
    UserMemCopy((void*)(&(appStatePtr->stateRackStoreOut)), (void*)(&stateRackStoreOutDefault)  ,LENGTH_APP_STATE_RACK_STORE_OUT);
    UserMemCopy((void*)(&(appStatePtr->stateRackEntry)),    (void*)(&stateRackEntryDefault)     ,LENGTH_APP_STATE_RACK_ENTRY);
    UserMemCopy((void*)(&(appStatePtr->stateRackBarScan)),  (void*)(&stateRackBarScanDefault)   ,LENGTH_APP_STATE_RACK_BAR_SCAN);
    UserMemCopy((void*)(&(appStatePtr->stateRackTest)),     (void*)(&stateRackTestDefault)      ,LENGTH_APP_STATE_RACK_TEST);
    UserMemCopy((void*)(&(appStatePtr->stateRackInSwitch)), (void*)(&stateRackInSwitchDefault)  ,LENGTH_APP_STATE_RACK_IN_SWITCH);
    UserMemCopy((void*)(&(appStatePtr->stateRackWaitOut1)), (void*)(&stateRackWaitOut1Default)  ,LENGTH_APP_STATE_RACK_WAIT_OUT1);
    UserMemCopy((void*)(&(appStatePtr->stateRackWaitOut2)), (void*)(&stateRackWaitOut2Default)  ,LENGTH_APP_STATE_RACK_WAIT_OUT2);
    UserMemCopy((void*)(&(appStatePtr->stateRackOutSwitch)),(void*)(&stateRackOutSwitchDefault) ,LENGTH_APP_STATE_RACK_OUT_SWITCH);
}


//设置默认值
void AppStateSetRackStoreInDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateRackStoreIn)),  (void*)(&stateRackStoreInDefault)   ,LENGTH_APP_STATE_RACK_STORE_IN);
}

//获取状态指针
APP_STATE_RACK_STORE_IN* AppStateReadRackStoreInState(void)
{
    return &(appStatePtr->stateRackStoreIn);
}


//设置默认值
void AppStateSetRackStoreOutDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateRackStoreOut)), (void*)(&stateRackStoreOutDefault)  ,LENGTH_APP_STATE_RACK_STORE_OUT);
}

//获取状态指针
APP_STATE_RACK_STORE_OUT* AppStateReadRackStoreOutState(void)
{
    return &(appStatePtr->stateRackStoreOut);
}


//设置默认值
void AppStateSetRackEntryDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateRackEntry)),    (void*)(&stateRackEntryDefault)     ,LENGTH_APP_STATE_RACK_ENTRY);
}

//获取状态指针
APP_STATE_RACK_ENTRY* AppStateReadRackEntryState(void)
{
    return &(appStatePtr->stateRackEntry);
}


//设置默认值
void AppStateSetRackBarScanDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateRackBarScan)),  (void*)(&stateRackBarScanDefault)   ,LENGTH_APP_STATE_RACK_BAR_SCAN);
}

//获取状态指针
APP_STATE_RACK_BAR_SCAN* AppStateReadRackBarScanState(void)
{
    return &(appStatePtr->stateRackBarScan);
}


//设置默认值
void AppStateSetRackTestDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateRackTest)),     (void*)(&stateRackTestDefault)      ,LENGTH_APP_STATE_RACK_TEST);
}

//获取状态指针
APP_STATE_RACK_TEST* AppStateReadRackTestState(void)
{
    return &(appStatePtr->stateRackTest);
}


//设置默认值
void AppStateSetRackInSwitchDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateRackInSwitch)), (void*)(&stateRackInSwitchDefault)  ,LENGTH_APP_STATE_RACK_IN_SWITCH);
}

//获取状态指针
APP_STATE_RACK_IN_SWITCH* AppStateReadRackInSwitchState(void)
{
    return &(appStatePtr->stateRackInSwitch);
}


//设置默认值
void AppStateSetRackWaitOut1Default(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateRackWaitOut1)), (void*)(&stateRackWaitOut1Default)  ,LENGTH_APP_STATE_RACK_WAIT_OUT1);
}

//获取状态指针
APP_STATE_RACK_WAIT_OUT1* AppStateReadRackWaitOut1State(void)
{
    return &(appStatePtr->stateRackWaitOut1);
}


//设置默认值
void AppStateSetRackWaitOut2Default(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateRackWaitOut2)), (void*)(&stateRackWaitOut2Default)  ,LENGTH_APP_STATE_RACK_WAIT_OUT2);
}

//获取状态指针
APP_STATE_RACK_WAIT_OUT2* AppStateReadRackWaitOut2State(void)
{
    return &(appStatePtr->stateRackWaitOut2);
}


//设置默认值
void AppStateSetRackOutSwitchDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateRackOutSwitch)),(void*)(&stateRackOutSwitchDefault) ,LENGTH_APP_STATE_RACK_OUT_SWITCH);
}

//获取状态指针
APP_STATE_RACK_OUT_SWITCH* AppStateReadRackOutSwitchState(void)
{
    return &(appStatePtr->stateRackOutSwitch);
}

