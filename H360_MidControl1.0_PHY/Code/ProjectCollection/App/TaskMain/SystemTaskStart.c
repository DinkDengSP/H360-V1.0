/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-14 17:09:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 20:22:20 +0800
************************************************************************************************/ 
#include "SystemTaskConfig.h"
#include "SrvImplStart.h"
#include "SrvImplHeart.h"

//任务控制块,8字节对齐
//任务堆栈
uint64_t stackBufferSystemTaskStart[STK_SIZE_SYSTEM_TASK_START/8]@".axi_sram";
//启动任务的堆栈
TX_THREAD tcbSystemTaskStart@".axi_sram";

//任务函数
void FuncTaskStartEntry(ULONG threadInput)
{
    (void)threadInput;
    //设置中断分组
    CoreNVIC_SetGroupDefault();
    //延时函数重新初始化
    CoreDelayInit();
	//内核开启后，恢复HAL里的时间基准,打开SysTick中断
    HAL_ResumeTick();	
    //系统设备初始化并自检
    SrvImplStartDeviceInitCheck();
    //服务层运行时数据初始化
    SrvImplStartCreateSrvTaskRunningVar();
    //创建服务层任务
    SrvImplStartCreateSrvTask();
    //初始化模块层数据
    SrvImplStartCreateModuleTaskRunningVar();
    //创建模块层任务
    SrvImplStartCreateModuleTask();
    //初始化应用层数据
    SrvImplStartCreateAppTaskRunningVar();
    //创建应用层任务
    SrvImplStartCreateAppTask();
    //初始化定时心跳任务队列
    SrvImplHeartInit();
    //系统启动
    SystemPrintf("BootLoader System Start\r\n");
#if(SRV_HEART_IWDG_ENABLE == 1)
    //全部初始化完成,开启看门狗
    MCU_IWDG_Init(SRV_HEART_IWDG_OVER_FLOW_TIME);
#endif
    while(1)
    {
        //周期循环
        CoreDelayMs(SRV_HEART_PERIOD_TIME_MS);
        //调用定时任务
        SrvImplHeartRunPeriod();
    }
}

