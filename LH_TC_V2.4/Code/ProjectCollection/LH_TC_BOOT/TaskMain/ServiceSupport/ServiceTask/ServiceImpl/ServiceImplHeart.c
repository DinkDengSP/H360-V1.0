/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-16 18:59:02
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-16 19:13:44
**FilePath: \LH_TC_BOOT_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplHeart.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplHeart.h"

static uint32_t runningCount = 0;
//自动运行任务初始化
void ServiceImplHeart_StateMachineInit(void)
{
    BoardSystemLedWrite(BOARD_LED_GREEN,IO_SIGNAL_INVALID);
    runningCount = 0;
    return;
}


//自动运行任务
void ServiceImplHeart_StateMachineLoop(void)
{
    runningCount++;
    if((runningCount%5) == 0)
    {
        BoardSystemLedToogle(BOARD_LED_YELLOW);
    }
}













