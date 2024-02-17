/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-17 09:25:03
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-17 13:08:18
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplHeart.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplHeart.h"

//系统运行灯
static uint32_t systemRunningLedCount = 0;

static void ServiceImplHeart_SystemRunningLedInit(void)
{
    systemRunningLedCount = 0;
}

static void ServiceImplHeart_SystemRunningLedLoop(void)
{
    systemRunningLedCount++;
    if(systemRunningLedCount % 5 == 0)
    {
        systemRunningLedCount = 0;
        BoardSystemLedToogle(BOARD_LED_GREEN);
    }
}

//自动化运行程序初始化
void ServiceImplHeart_StateMachineInit(void)
{
    ServiceImplHeart_SystemRunningLedInit();
}


//自动化运行程序循环运转
void ServiceImplHeart_StateMachineLoop(void)
{
    ServiceImplHeart_SystemRunningLedLoop();
}









