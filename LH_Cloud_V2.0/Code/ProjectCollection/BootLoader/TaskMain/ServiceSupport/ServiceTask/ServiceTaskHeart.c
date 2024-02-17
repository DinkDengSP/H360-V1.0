/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-26 13:14:05
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-26 20:12:04
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferServiceTaskHeart[STK_SIZE_SERVICE_TASK_HEART]@".ccram";
#pragma pack()
//任务控制块
OS_TCB tcbServiceTaskHeart@".ccram";

void ServiceTaskFuncHeartReflushConfigKey(void)
{
    uint8_t keyValue = BoardConfigKeyReadAll();
    BoardConfigKeyWriteLedAll(keyValue);
}

//任务函数
void ServiceTaskFuncHeart(void *p_arg)
{
    p_arg = p_arg;
    while(1)
    {
        //Bootloader黄灯闪烁
        CoreDelayMs(100);
        BoardLedToogle(BOARD_LED_YELLOW);
        ServiceTaskFuncHeartReflushConfigKey();
    }
}
