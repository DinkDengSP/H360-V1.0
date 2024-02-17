/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-07-01 18:29:09
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-11 13:45:39
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SystemTaskConfig.h"


//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskHeart[STK_SIZE_TASK_HEART]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskHeart@".dtcm_ram";

static uint32_t timeCount = 0;

//任务函数
void TaskFuncHeart(void *p_arg)
{
    p_arg = p_arg;
    while(1)
    {
        CoreDelayMs(250);
        //黄灯闪烁
        BoardLedToogle(BOARD_LED_YELLOW);
        timeCount++;
        //定时上报心跳
        if(timeCount >= 12)
        {
            Can1ReportHeart(CAN1_SYSTEM_STATE_DEVICE_BOOT);
            timeCount = 0;
        }
    }
}

