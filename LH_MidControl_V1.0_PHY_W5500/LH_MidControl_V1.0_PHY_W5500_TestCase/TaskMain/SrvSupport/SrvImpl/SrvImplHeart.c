/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 17:54:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 15:15:21 +0800
************************************************************************************************/ 
#include "SrvImplHeart.h"
#include "SrvImplStart.h"
#include "tx_execution_profile.h"
#include "SystemTaskConfig.h"

/*-------------------------------系统运行状态统计-------------------------------*/
#if(SRV_HEART_THREAD_PERFORMANCE_STAT_ENABLE != 0)
//上次统计任务信息的时间
static int32_t lastTickThreadPerformanceStat = 0;
//上次显示系统任务情况的时间
static int32_t lastTickThreadPerformanceShow = 0;

/* CPU百分比 */
__IO double OSCPUUsage;    
//系统统计的各项时间  
EXECUTION_TIME profileThreadTotalTime, profileIsrTime, profileIdleTime; 
//计算CPU实时使用率的变量
EXECUTION_TIME TolTime, IdleTime, deltaTolTime, deltaIdleTime; 

//线程信息统计任务初始化
void SrvImplHeartSubThreadPerformanceStateInit(void)
{
    //上次显示系统任务情况的时间
    lastTickThreadPerformanceShow = tx_time_get();
    //上次统计任务信息的时间
    lastTickThreadPerformanceStat = tx_time_get();  
    //线程总消耗时间
    _tx_execution_thread_total_time_get(&profileThreadTotalTime);
    //中断时间
    _tx_execution_isr_time_get(&profileIsrTime);
    //空闲时间
    _tx_execution_idle_time_get(&profileIdleTime);
    /* 计算CPU利用率 */
	IdleTime = profileIdleTime;
	TolTime = profileThreadTotalTime + profileIsrTime + profileIdleTime; 
}

//线程信息统计任务周期运转
void SrvImplHeartSubThreadPerformanceStateRunPeriod(void)
{
    /* 定义一个任务控制块指针 */
    TX_THREAD      *p_tcb;	
    EXECUTION_TIME threadRunTime;
    if(tx_time_get() - lastTickThreadPerformanceStat >= SRV_HEART_PERIOD_MS_THREAD_INFO_STATE)
    {
        //时间更新
        lastTickThreadPerformanceStat = tx_time_get();
        //线程总消耗时间
        _tx_execution_thread_total_time_get(&profileThreadTotalTime);
        //中断时间
        _tx_execution_isr_time_get(&profileIsrTime);
        //空闲时间
        _tx_execution_idle_time_get(&profileIdleTime);
        //实时计算变化增量
        deltaIdleTime = profileIdleTime - IdleTime;
        deltaTolTime = profileThreadTotalTime + profileIsrTime + profileIdleTime - TolTime;
        OSCPUUsage = (double)deltaIdleTime/deltaTolTime;
        OSCPUUsage = 100- OSCPUUsage*100;
        IdleTime = profileIdleTime;
        TolTime = profileThreadTotalTime + profileIsrTime + profileIdleTime;
    }
    if(tx_time_get() - lastTickThreadPerformanceShow >= SRV_HEART_PERIOD_MS_THREAD_INFO_SHOW)
    {
        lastTickThreadPerformanceShow = tx_time_get();  
        p_tcb = &tcbSystemTaskStart;
        //线程总消耗时间
        _tx_execution_thread_total_time_get(&profileThreadTotalTime);
        //中断时间
        _tx_execution_isr_time_get(&profileIsrTime);
        //空闲时间
        _tx_execution_idle_time_get(&profileIdleTime);
        /* 打印标题 */
        SystemPrintf("=============================AzureComponent Application==============================\r\n");
        SystemPrintf("CPU usage = %5.2f%%\r\n", OSCPUUsage);
        SystemPrintf("TotalTaskRunTime = %.9fs\r\n", (double)profileThreadTotalTime/SystemCoreClock);
        SystemPrintf("IdleTaskRunTime = %.9fs\r\n", (double)profileIdleTime/SystemCoreClock);
        SystemPrintf("IntRunTime = %.9fs\r\n", (double)profileIsrTime/SystemCoreClock);
        SystemPrintf("SystemRunTime = %.9fs\r\n", (double)(profileThreadTotalTime + profileIsrTime + profileIdleTime)/SystemCoreClock);	
        SystemPrintf("====================================================================================\r\n");
        SystemPrintf("   Prio     StackSize   CurStack    MaxStack   RunTime   RunCount   Taskname\r\n");
        /* 遍历任务控制列表TCB list)，打印所有的任务的优先级和名称 */
        while (p_tcb != (TX_THREAD *)0) 
        {
            //获取线程运行时间
            _tx_execution_thread_time_get(p_tcb,&threadRunTime);
            //打印线程执行情况
            SystemPrintf("   %2d        %5d      %5d       %5d      %.3f S     %5d     %s\r\n", 
                        p_tcb->tx_thread_priority,
                        p_tcb->tx_thread_stack_size,
                        (int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_ptr,
                        (int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_highest_ptr,
                        (double)(threadRunTime)/SystemCoreClock,
                        p_tcb->tx_thread_run_count,
                        p_tcb->tx_thread_name);
            //下一个
            p_tcb = p_tcb->tx_thread_created_next;
            if(p_tcb == &tcbSystemTaskStart) break;
        }
        SystemPrintf("====================================================================================\r\n");
    }
}
#endif

/*-------------------------------运行时LED心跳显示------------------------------*/
#if(SRV_HEART_BOARD_LED_TOOGLE_ENABLE != 0)
//上次LED心跳时间
static int32_t lastTickBoardLedTogle = 0;
//运行时LED心跳显示初始化
static void SrvImplHeartSubBoardLedToogleInit(void)
{
    lastTickBoardLedTogle = tx_time_get();
    //关闭全部的LED
    SrvImplStartBoardLedAllOff();
}
//运行时LED心跳显示周期运行
static void SrvImplHeartSubBoardLedToogleRunPeriod(void)
{
    if(tx_time_get() - lastTickBoardLedTogle < SRV_HEART_PERIOD_MS_BOARD_LED_TOOGLE)
    {
        return;
    }
    lastTickBoardLedTogle = tx_time_get();
    BoardLedToogle(BOARD_LED_GREEN);
    
}
#endif

/*-------------------------------运行时LED心跳显示------------------------------*/
#if(SRV_HEART_BOARD_LED_YELLOW_TOOGLE_ENABLE != 0)
//上次LED心跳时间
static int32_t lastTickBoardYellowLedTogle = 0;
//运行时LED心跳显示初始化
static void SrvImplHeartSubBoardYellowLedToogleInit(void)
{
    lastTickBoardYellowLedTogle = tx_time_get();
}

//运行时LED心跳显示周期运行
static void SrvImplHeartSubBoardYellowLedToogleRunPeriod(void)
{
    if(tx_time_get() - lastTickBoardYellowLedTogle < SRV_HEART_PERIOD_MS_BOARD_LED_YELLOW_TOOGLE)
    {
        return;
    }
    lastTickBoardYellowLedTogle = tx_time_get();
    BoardLedToogle(BOARD_LED_YELLOW);
    
}
#endif

/*-------------------------------配置按键状态实时显示------------------------------*/
#if(SRV_HEART_CONFIG_KEY_SHOW_ENABLE != 0)
//上次LED心跳时间
static int32_t lastTickBoardConfigKeyShow = 0;
//运行时配置按键状态实时显示初始化
static void SrvImplHeartSubConfigKeyShowInit(void)
{
    lastTickBoardConfigKeyShow = tx_time_get();
}
//运行时配置按键状态实时显示周期运行
static void SrvImplHeartSubBoardConfigKeyShowRunPeriod(void)
{
    if(tx_time_get() - lastTickBoardConfigKeyShow < SRV_HEART_PERIOD_MS_CONFIG_KEY_SHOW)
    {
        return;
    }
    lastTickBoardConfigKeyShow = tx_time_get();
    CONFIG_KEY_VAL configKeyValue;
    configKeyValue = BoardKeyConfigReadAll();
    BoardKeyConfigWriteLedAll(configKeyValue);
}
#endif

/*-------------------------------独立看门狗-------------------------------------*/
#if(SRV_HEART_IWDG_ENABLE != 0)
//上次独立看门狗喂狗时间戳
static int32_t lastHeartIwdgFeedTimeTick = 0;
//运行时独立看门狗初始化
static void SrvImplHeartSubIwdgInit(void)
{
    lastHeartIwdgFeedTimeTick = tx_time_get();
    //关闭全部的LED
    MCU_IWDG_Init(SRV_HEART_IWDG_OVER_FLOW_TIME);
}
/*---------------------------独立看门狗定时喂狗--------------------------------*/
static void SrvImplHeartSubIwdgFeedRunPeriod(void)
{
    if(tx_time_get() - lastHeartIwdgFeedTimeTick < (SRV_HEART_IWDG_OVER_FLOW_TIME/2))
    {
        return;
    }
    lastHeartIwdgFeedTimeTick = tx_time_get();
    //独立看门狗喂狗
    MCU_IWDG_Feed();
}
#endif

/*-------------------------------窗口看门狗-------------------------------------*/
#if(SRV_HEART_WWDG_ENABLE != 0)
//上次窗口看门狗喂狗时间戳
static int32_t lastTickHeartWwdgFeed = 0;
//窗口看门狗喂狗回调函数
void SrvImplHeartWWDG_EarlyCallBack(void)
{
    BoardLedToogle(BOARD_LED_YELLOW);
}
//运行时窗口看门狗初始化
static void SrvImplHeartSubWwdgInit(void)
{
    lastTickHeartWwdgFeed = tx_time_get();
    //关闭全部的LED
    MCU_WWDG_Init(SrvImplHeartWWDG_EarlyCallBack);
}
//运行时窗口看门狗定时喂狗
static void SrvImplHeartSubWwdgFeedRunPeriod(void)
{
    if(tx_time_get() - lastTickHeartWwdgFeed < (SRV_HEART_WWDG_OVER_FLOW_TIME/2))
    {
        return;
    }
    lastTickHeartWwdgFeed = tx_time_get();
    //窗口看门狗喂狗
    MCU_WWDG_Feed();
}
#endif

/*-------------------------------系统RTC时间显示--------------------------------*/
#if(SRV_HEART_RTC_TIMER_SHOW_ENABLE != 0)
//上次RTC时间显示时间戳
static int32_t lastHeartRtcTimerShowTimeTick = 0;
//系统RTC时间显示初始化
static void SrvImplHeartSubRtcTimerShowInit(void)
{
    lastHeartRtcTimerShowTimeTick = tx_time_get();
}
//系统RTC时间显示周期运行
static void SrvImplHeartSubRtcTimerShowRunPeriod(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(tx_time_get() - lastHeartRtcTimerShowTimeTick < SRV_HEART_PERIOD_MS_RTC_TIMER_SHOW)
    {
        return;
    }
    lastHeartRtcTimerShowTimeTick = tx_time_get();
    //时间展示
    RTC_TIMER* rtcTimerPtr = NULL;
    errorSub = MCU_RTC_GetTimeRecordSync(&rtcTimerPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("MCU_RTC_GetTimeRecordSync Failed,Code: 0X%08X\r\n",errorSub);
        return;
    }
    //当前时间打印
    SystemPrintf("CurrentWeek: %d,CurrentTime %04d-%02d-%02d,%02d:%02d:%02d:%03d\r\n",rtcTimerPtr->weekDay,
                    rtcTimerPtr->year,rtcTimerPtr->month,rtcTimerPtr->day,rtcTimerPtr->hour,rtcTimerPtr->minute,
                    rtcTimerPtr->second,rtcTimerPtr->subSecond);
}

#endif


/*-------------------------------内存使用率展示--------------------------------*/
#if(SRV_HEART_MEM_USAGE_SHOW_ENABLE != 0)
//上次内存使用率展示时间戳
static int32_t lastHeartMemUsageShowTimeTick = 0;
//系统内存使用率展示初始化
static void SrvImplHeartSubMemUsageShowInit(void)
{
    lastHeartMemUsageShowTimeTick = tx_time_get();
}
//系统内存使用率展示周期运行
static void SrvImplHeartSubMemUsageShowRunPeriod(void)
{
    if(tx_time_get() - lastHeartMemUsageShowTimeTick < SRV_HEART_PERIOD_MS_MEM_USAGE_SHOW)
    {
        return;
    }
    lastHeartMemUsageShowTimeTick = tx_time_get();
    uint8_t axiSramMemUsage = 0;
    uint8_t d2Sram1MemUsage = 0;
    uint8_t d3Sram4MemUsage = 0;
    uint8_t sdram1MemUsage = 0;
    uint8_t sdram2MemUsage = 0;
    axiSramMemUsage = UserMemPerUsed(MEM_AXI_SRAM);
    d2Sram1MemUsage = UserMemPerUsed(MEM_D2_SRAM1);
    d3Sram4MemUsage = UserMemPerUsed(MEM_D3_SRAM4);
    sdram1MemUsage = UserMemPerUsed(MEM_SDRAM1);
    sdram2MemUsage = UserMemPerUsed(MEM_SDRAM2);
    //显示
    SystemPrintf("AXI_SRAM Usage: %d%%. D2_SRAM1 Usage: %d%%.\r\nD3_SRAM4 Usage: %d%%. SDRAM1 Usage: %d%%. SDRAM2 Usage: %d%%.\r\n",
                    axiSramMemUsage,d2Sram1MemUsage,d3Sram4MemUsage,sdram1MemUsage,sdram2MemUsage);
}
#endif



//初始化心跳函数
void SrvImplHeartInit(void)
{
    //系统线程运行性能统计初始化
#if(SRV_HEART_THREAD_PERFORMANCE_STAT_ENABLE != 0)
    SrvImplHeartSubThreadPerformanceStateInit();
#endif
    //运行时LED心跳显示初始化
#if(SRV_HEART_BOARD_LED_TOOGLE_ENABLE != 0)
    SrvImplHeartSubBoardLedToogleInit();
#endif
    //运行时LED心跳显示初始化
#if(SRV_HEART_BOARD_LED_YELLOW_TOOGLE_ENABLE != 0)
    SrvImplHeartSubBoardYellowLedToogleInit();
#endif
    //独立看门狗初始化
#if(SRV_HEART_IWDG_ENABLE != 0)
    SrvImplHeartSubIwdgInit();
#endif
    //窗口看门狗初始化
#if(SRV_HEART_WWDG_ENABLE != 0)
    SrvImplHeartSubWwdgInit();
#endif
    //系统RTC时间显示初始化
#if(SRV_HEART_RTC_TIMER_SHOW_ENABLE != 0)
    SrvImplHeartSubRtcTimerShowInit();
#endif
    //系统内存使用率展示初始化
#if(SRV_HEART_MEM_USAGE_SHOW_ENABLE != 0)
    SrvImplHeartSubMemUsageShowInit();
#endif
    //是否使能配置按键状态实时显示
#if(SRV_HEART_CONFIG_KEY_SHOW_ENABLE != 0)
    SrvImplHeartSubConfigKeyShowInit();
#endif
}

//周期性运行心跳函数
void SrvImplHeartRunPeriod(void)
{
    //系统线程运行性能统计周期运行
#if(SRV_HEART_THREAD_PERFORMANCE_STAT_ENABLE != 0)
    SrvImplHeartSubThreadPerformanceStateRunPeriod();
#endif 
    //运行时LED心跳显示周期运行
#if(SRV_HEART_BOARD_LED_TOOGLE_ENABLE != 0)
    SrvImplHeartSubBoardLedToogleRunPeriod();
#endif
    //运行时LED心跳显示周期运行
#if(SRV_HEART_BOARD_LED_YELLOW_TOOGLE_ENABLE != 0)
    SrvImplHeartSubBoardYellowLedToogleRunPeriod();
#endif
    //独立看门狗周期运行喂狗
#if(SRV_HEART_IWDG_ENABLE != 0)
    SrvImplHeartSubIwdgFeedRunPeriod();
#endif
    //窗口看门狗周期喂狗
#if(SRV_HEART_WWDG_ENABLE != 0)
    SrvImplHeartSubWwdgFeedRunPeriod();
#endif
    //系统RTC时间显示周期运行
#if(SRV_HEART_RTC_TIMER_SHOW_ENABLE != 0)
    SrvImplHeartSubRtcTimerShowRunPeriod();
#endif
    //系统内存使用率展示周期运行
#if(SRV_HEART_MEM_USAGE_SHOW_ENABLE != 0)
    SrvImplHeartSubMemUsageShowRunPeriod();
#endif
    //置按键状态实时显示周期运行
#if(SRV_HEART_CONFIG_KEY_SHOW_ENABLE != 0)
    SrvImplHeartSubBoardConfigKeyShowRunPeriod();
#endif
}