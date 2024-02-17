/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-26 13:16:44
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-26 13:17:18
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferAppTaskDemo[STK_SIZE_APP_TASK_DEMO]@".ccram";
#pragma pack()
//任务控制块
OS_TCB tcbAppTaskDemo@".ccram";

//任务函数
void AppTaskFuncDemo(void *p_arg)
{
    p_arg = p_arg;
    while(1)
    {
        CoreDelayMs(60000);
    }
}
