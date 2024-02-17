/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2021-09-02 17:02:45
**FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppUtil\AppState.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppState.h"

static APP_STATE* appStatePtr = NULL;

//系统状态初始化
void AppStateInit(void)
{
    if(appStatePtr != NULL)
    {
        UserMemFree(SRAM_IS62,appStatePtr);
    }
    appStatePtr = UserMemMallocWhileSuccess(SRAM_IS62,LENGTH_APP_STATE);
    //拷贝默认值
    AppStateSetAllDefault();
}

//系统状态全部设置为默认值
void AppStateSetAllDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateTubeEntry)),(void*)(&stateTubeEntryDefault),LENGTH_APP_STATE_TUBE_ENTRY);
    UserMemCopy((void*)(&(appStatePtr->stateTubeRotate)),(void*)(&stateTubeRotateDefault),LENGTH_APP_STATE_TUBE_ROTATE);
    UserMemCopy((void*)(&(appStatePtr->stateTubeBarScan)),(void*)(&stateTubeBarScanDefault),LENGTH_APP_STATE_TUBE_BAR_SCAN);
    UserMemCopy((void*)(&(appStatePtr->stateTubeClamp)),(void*)(&stateTubeClampDefault),LENGTH_APP_STATE_TUBE_CLAMP);
    UserMemCopy((void*)(&(appStatePtr->stateTubeExit)),(void*)(&stateTubeExitDefault),LENGTH_APP_STATE_TUBE_EXIT);
}

//各个节点状态设置为默认值
void AppStateSetTubeEntryDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateTubeEntry)),(void*)(&stateTubeEntryDefault),LENGTH_APP_STATE_TUBE_ENTRY);
}

void AppStateSetTubeRotateDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateTubeRotate)),(void*)(&stateTubeRotateDefault),LENGTH_APP_STATE_TUBE_ROTATE);
}

void AppStateSetTubeBarScanDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateTubeBarScan)),(void*)(&stateTubeBarScanDefault),LENGTH_APP_STATE_TUBE_BAR_SCAN);
}

void AppStateSetTubeClampDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateTubeClamp)),(void*)(&stateTubeClampDefault),LENGTH_APP_STATE_TUBE_CLAMP);
}

void AppStateSetTubeExitDefault(void)
{
    UserMemCopy((void*)(&(appStatePtr->stateTubeExit)),(void*)(&stateTubeExitDefault),LENGTH_APP_STATE_TUBE_EXIT);
}


//获取各个节点状态的指针
APP_STATE_TUBE_ENTRY* AppStateGetTubeEntryStatePtr(void)
{
    return &(appStatePtr->stateTubeEntry);
}

APP_STATE_TUBE_ROTATE* AppStateGetTubeRotateStatePtr(void)
{
    return &(appStatePtr->stateTubeRotate);
}

APP_STATE_TUBE_BAR_SCAN* AppStateGetTubeBarScanStatePtr(void)
{
    return &(appStatePtr->stateTubeBarScan);
}

APP_STATE_TUBE_CLAMP* AppStateGetTubeClampStatePtr(void)
{
    return &(appStatePtr->stateTubeClamp);
}

APP_STATE_TUBE_EXIT* AppStateGetTubeExitStatePtr(void)
{
    return &(appStatePtr->stateTubeExit);
}

//-----------------------------------------入口节点-----------------------------------------------
//写进管事件
void AppStateTubeEntryWriteTubeEntryEvent(EVENT_FLAG eventFlag)
{
    appStatePtr->stateTubeEntry.tubeEntryEventFlag = eventFlag;
}

//读进管事件
EVENT_FLAG AppStateTubeEntryReadTubeEntryEvent(void)
{
    return appStatePtr->stateTubeEntry.tubeEntryEventFlag;
}

//-----------------------------------------旋转节点-----------------------------------------------

//-----------------------------------------试管扫码节点-----------------------------------------------
//-----------------------------------------测试试管夹紧-----------------------------------------------
//写试管加紧状态
void AppStatetTubeClampWriteTubeClampState(TUBE_CLAMP_STATE tubeclampState)
{   
    appStatePtr->stateTubeClamp.tubeClampState = tubeclampState;
}

//读试管加紧状态
TUBE_CLAMP_STATE AppStatetTubeClampReadTubeClampState(void)
{   
    return appStatePtr->stateTubeClamp.tubeClampState;
}

//-----------------------------------------出口节点-----------------------------------------------
//写出管事件
void AppStateTubeExitWriteTubeExitEvent(EVENT_FLAG eventFlag)
{
    appStatePtr->stateTubeExit.tubExitEventFlag = eventFlag;
}

//读出管事件
EVENT_FLAG AppStateTubeExitReadTubeExitEvent(void)
{
    return appStatePtr->stateTubeExit.tubExitEventFlag;
}
