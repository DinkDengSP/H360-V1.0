/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-10 11:17:31
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-20 11:18:39
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppState.h"
#include "AppTaskConfig.h"
//初始化系统状态
void AppStateInit(APP_STATE* appStatePtr)
{
    //如果已经有了指向,释放
    if(appStatePtr != NULL)
    {
        UserMemFree(MEM_D3_SRAM4,appStatePtr);
    }
    //申请内存
    appStatePtr = UserMemMallocWhileSuccess(MEM_D3_SRAM4,LENGTH_APP_STATE);
    //设置为默认值
    UserMemCopy(&(appStatePtr->stModuleState[CAN_CELL_MODULE_1].deviceModuleState),(void*)&deviceModule1DefaultState,LENGTH_SYSTEM_MODULE_STATE);
    UserMemCopy(&(appStatePtr->stModuleState[CAN_CELL_MODULE_2].deviceModuleState),(void*)&deviceModule2DefaultState,LENGTH_SYSTEM_MODULE_STATE);
    UserMemCopy(&(appStatePtr->stModuleState[CAN_CELL_MODULE_3].deviceModuleState),(void*)&deviceModule3DefaultState,LENGTH_SYSTEM_MODULE_STATE);
    UserMemCopy(&(appStatePtr->stModuleState[CAN_CELL_MODULE_4].deviceModuleState),(void*)&deviceModule4DefaultState,LENGTH_SYSTEM_MODULE_STATE);
    UserMemCopy(&(appStatePtr->stModuleState[CAN_CELL_MODULE_5].deviceModuleState),(void*)&deviceModule5DefaultState,LENGTH_SYSTEM_MODULE_STATE);
    //设置模块状态更新时间
    appStatePtr->stModuleState[CAN_CELL_MODULE_1].lastRecvDeviceModuleStateTime = tx_time_get();
    appStatePtr->stModuleState[CAN_CELL_MODULE_2].lastRecvDeviceModuleStateTime = tx_time_get();
    appStatePtr->stModuleState[CAN_CELL_MODULE_3].lastRecvDeviceModuleStateTime = tx_time_get();
    appStatePtr->stModuleState[CAN_CELL_MODULE_4].lastRecvDeviceModuleStateTime = tx_time_get();
    appStatePtr->stModuleState[CAN_CELL_MODULE_5].lastRecvDeviceModuleStateTime = tx_time_get();
    //设置各个流程状态
    UserMemCopy(&(appStatePtr->stModuleWorkingState[CAN_CELL_MODULE_1]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(appStatePtr->stModuleWorkingState[CAN_CELL_MODULE_2]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(appStatePtr->stModuleWorkingState[CAN_CELL_MODULE_3]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(appStatePtr->stModuleWorkingState[CAN_CELL_MODULE_4]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(appStatePtr->stModuleWorkingState[CAN_CELL_MODULE_5]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(appStatePtr->stModuleWorkingState[CAN_CELL_MODULE_PLATFORM]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
}


//获取指定模块的模块状态指针
SYSTEM_MODULE_STATE* AppStateGetModuleStatePtr(STATE_MODULE_NO moduleNo)
{
    APP_STATE* pstStatus = AppStateGetTestStatusPtr();
    if(moduleNo == STATE_MODULE_1)
    {
        return &(pstStatus->stModuleState[CAN_CELL_MODULE_1].deviceModuleState);
    }
    else if(moduleNo == STATE_MODULE_2)
    {
        return &(pstStatus->stModuleState[CAN_CELL_MODULE_2].deviceModuleState);
    }
    else if(moduleNo == STATE_MODULE_3)
    {
        return &(pstStatus->stModuleState[CAN_CELL_MODULE_3].deviceModuleState);
    }
    else if(moduleNo == STATE_MODULE_4)
    {
        return &(pstStatus->stModuleState[CAN_CELL_MODULE_4].deviceModuleState);
    }
    else if(moduleNo == STATE_MODULE_5)
    {
        return &(pstStatus->stModuleState[CAN_CELL_MODULE_PLATFORM].deviceModuleState);
    }
    else
    {
        return &(pstStatus->stModuleState[CAN_CELL_MODULE_PLATFORM].deviceModuleState);
    }
}

//获取指定模块上一次的接收到心跳的时间
uint32_t AppStateGetLastRecvDeviceModuleTime(STATE_MODULE_NO moduleNo)
{
    APP_STATE* pstStatus = AppStateGetTestStatusPtr();
    if(moduleNo == STATE_MODULE_1)
    {
        return (pstStatus->stModuleState[CAN_CELL_MODULE_1].lastRecvDeviceModuleStateTime);
    }
    else if(moduleNo == STATE_MODULE_2)
    {
        return (pstStatus->stModuleState[CAN_CELL_MODULE_2].lastRecvDeviceModuleStateTime);
    }
    else if(moduleNo == STATE_MODULE_3)
    {
        return (pstStatus->stModuleState[CAN_CELL_MODULE_3].lastRecvDeviceModuleStateTime);
    }
    else if(moduleNo == STATE_MODULE_4)
    {
        return (pstStatus->stModuleState[CAN_CELL_MODULE_4].lastRecvDeviceModuleStateTime);
    }
    else if(moduleNo == STATE_MODULE_5)
    {
        return (pstStatus->stModuleState[CAN_CELL_MODULE_PLATFORM].lastRecvDeviceModuleStateTime);
    }
    else
    {
        return (pstStatus->stModuleState[CAN_CELL_MODULE_PLATFORM].lastRecvDeviceModuleStateTime);
    }
}

//设置指定模块接收到心跳的时间为当前时间
uint32_t AppStateSetLastRecvDeviceModuleTime(STATE_MODULE_NO moduleNo)
{
    APP_STATE* pstStatus = AppStateGetTestStatusPtr();
    uint32_t currentTick = tx_time_get();
    if(moduleNo == STATE_MODULE_1)
    {
        pstStatus->stModuleState[CAN_CELL_MODULE_1].lastRecvDeviceModuleStateTime = currentTick;
        return (pstStatus->stModuleState[CAN_CELL_MODULE_1].lastRecvDeviceModuleStateTime);
    }
    else if(moduleNo == STATE_MODULE_2)
    {
        pstStatus->stModuleState[CAN_CELL_MODULE_2].lastRecvDeviceModuleStateTime = currentTick;
        return (pstStatus->stModuleState[CAN_CELL_MODULE_2].lastRecvDeviceModuleStateTime);
    }
    else if(moduleNo == STATE_MODULE_3)
    {
        pstStatus->stModuleState[CAN_CELL_MODULE_3].lastRecvDeviceModuleStateTime = currentTick;
        return (pstStatus->stModuleState[CAN_CELL_MODULE_3].lastRecvDeviceModuleStateTime);
    }
    else if(moduleNo == STATE_MODULE_4)
    {
        pstStatus->stModuleState[CAN_CELL_MODULE_4].lastRecvDeviceModuleStateTime = currentTick;
        return (pstStatus->stModuleState[CAN_CELL_MODULE_4].lastRecvDeviceModuleStateTime);
    }
    else if(moduleNo == STATE_MODULE_5)
    {
        pstStatus->stModuleState[CAN_CELL_MODULE_PLATFORM].lastRecvDeviceModuleStateTime = currentTick;
        return (pstStatus->stModuleState[CAN_CELL_MODULE_PLATFORM].lastRecvDeviceModuleStateTime);
    }
    else
    {
        pstStatus->stModuleState[CAN_CELL_MODULE_PLATFORM].lastRecvDeviceModuleStateTime = currentTick;
        return (pstStatus->stModuleState[CAN_CELL_MODULE_PLATFORM].lastRecvDeviceModuleStateTime);
    }
}

STATE_CMP_RESULT AppStateCompareModuleState(SYSTEM_MODULE_STATE* moduleState1Ptr,SYSTEM_MODULE_STATE* moduleState2Ptr)
{
    uint8_t* moduleState1LocalPtr = (uint8_t*)moduleState1Ptr;
    uint8_t* moduleState2LocalPtr = (uint8_t*)moduleState2Ptr;
    //内存区域匹配
    uint16_t indexUtil = 0;
    for(indexUtil = 0;indexUtil < LENGTH_SYSTEM_MODULE_STATE;indexUtil++)
    {
        if(moduleState1LocalPtr[indexUtil] != moduleState2LocalPtr[indexUtil])
        {
            return STATE_CMP_NOT_EQUAL;
        }
    }
    return STATE_CMP_EQUAL;
}

//将主流程状态设置为默认值
void AppStateSetTestFlowWorkingStateDefault(APP_STATE* pstStatus)
{
    UserMemCopy(&(pstStatus->stModuleWorkingState[CAN_CELL_MODULE_1]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(pstStatus->stModuleWorkingState[CAN_CELL_MODULE_2]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(pstStatus->stModuleWorkingState[CAN_CELL_MODULE_3]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(pstStatus->stModuleWorkingState[CAN_CELL_MODULE_4]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(pstStatus->stModuleWorkingState[CAN_CELL_MODULE_5]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
    UserMemCopy(&(pstStatus->stModuleWorkingState[CAN_CELL_MODULE_PLATFORM]),(void*)&stModuleWorkingStateDefault,LENGTH_APP_STATE_MODULE_WORKING_STATE);
}

//更新主流程工作状态
void AppStateUpdateTestFlowWorkingState(APP_STATE* pstStatus, uint8_t u8ModuleNo, APP_MODULE_STATE wokingState)
{
    pstStatus->stModuleWorkingState[u8ModuleNo].stDeviceModuleState = wokingState;
    pstStatus->stModuleWorkingState[u8ModuleNo].eNextState = wokingState;
    pstStatus->stModuleWorkingState[u8ModuleNo].ePreState = wokingState;
}

