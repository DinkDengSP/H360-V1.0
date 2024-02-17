/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2021-09-10 08:56:58
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppUtil\AppUtilMsg.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_STATE_H_
#define __APP_STATE_H_
#include "AppStateDefault.h"

//系统状态初始化
void AppStateInit(void);

//系统状态全部设置为默认值
void AppStateSetAllDefault(void);

//各个节点状态设置为默认值
void AppStateSetTubeEntryDefault(void);
void AppStateSetTubeRotateDefault(void);
void AppStateSetTubeBarScanDefault(void);
void AppStateSetTubeClampDefault(void);
void AppStateSetTubeExitDefault(void);

//获取各个节点状态的指针
APP_STATE_TUBE_ENTRY* AppStateGetTubeEntryStatePtr(void);
APP_STATE_TUBE_ROTATE* AppStateGetTubeRotateStatePtr(void);
APP_STATE_TUBE_BAR_SCAN* AppStateGetTubeBarScanStatePtr(void);
APP_STATE_TUBE_CLAMP* AppStateGetTubeClampStatePtr(void);
APP_STATE_TUBE_EXIT* AppStateGetTubeExitStatePtr(void);

//-----------------------------------------入口节点-----------------------------------------------
//写进管事件
void AppStateTubeEntryWriteTubeEntryEvent(EVENT_FLAG eventFlag);
//读进管事件
EVENT_FLAG AppStateTubeEntryReadTubeEntryEvent(void);

//-----------------------------------------旋转节点-----------------------------------------------

//-----------------------------------------试管扫码节点-----------------------------------------------

//-----------------------------------------测试试管夹紧-----------------------------------------------
//写试管加紧状态
void AppStatetTubeClampWriteTubeClampState(TUBE_CLAMP_STATE tubeclampState);
//读试管加紧状态
TUBE_CLAMP_STATE AppStatetTubeClampReadTubeClampState(void);

//-----------------------------------------出口节点-----------------------------------------------
//写出管事件
void AppStateTubeExitWriteTubeExitEvent(EVENT_FLAG eventFlag);
//读出管事件
EVENT_FLAG AppStateTubeExitReadTubeExitEvent(void);

#endif







