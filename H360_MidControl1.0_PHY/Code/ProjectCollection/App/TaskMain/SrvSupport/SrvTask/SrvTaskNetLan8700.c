/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:00:32 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-31 10:36:38 +0800
************************************************************************************************/ 
#include "SrvTaskConfig.h"
#include "SrvImplNetLAN8700.h"

//任务的控制块
TX_THREAD tcbSrvTaskNetLAN8700@".dtcm_ram";
//任务堆栈
uint64_t stackBufferSrvTaskNetLAN8700[STK_SIZE_SRV_TASK_NET_LAN8700/8]@".dtcm_ram";
//任务消息队列控制块
TX_QUEUE queueSrvTaskNetLAN8700@".dtcm_ram";
//消息缓冲区
uint32_t queueBufferSrvTaskNetLAN8700[LENGTH_QUEUE_SRV_TASK_NET_LAN8700]@".dtcm_ram";
//网卡协议栈
uint64_t stackBufferSrvTaskLAN8700NetX[STK_SIZE_SRV_TASK_NETX_IP_LAN8700/8]@".dtcm_ram";

//任务相关变量初始化
static void SrvTaskNetLAN8700FuncVarInit(void);
//任务消息处理
static void SrvTaskNetLAN8700FuncMsgProcess(void* taskMsgPtr);
//定义执行周期,定义周期函数
#if(SRV_TASK_NET_LAN8700_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskNetLAN8700Func_FSM_Init(void);
//周期函数运转
static void SrvTaskNetLAN8700Func_FSM_RunPeriod(void);
#endif


//任务函数
void SrvTaskNetLAN8700FuncEntry(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //任务消息队列
    void* taskMsgPtr = NULL;
#if(SRV_TASK_NET_LAN8700_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
    //状态机初始化
    SrvTaskNetLAN8700Func_FSM_Init();
    //上一次的外部信号检测发送的时间
	ULONG lastFSM_RunTimeTickStamp = tx_time_get();
    //更新定时时钟
    ULONG currentTimeTickStamp = tx_time_get();
#endif
    //任务相关变量初始化
    SrvTaskNetLAN8700FuncVarInit();
    while(1)
    {
#if(SRV_TASK_NET_LAN8700_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
        //定义自动周期运行,检查当前时间
        currentTimeTickStamp = tx_time_get();
        if(currentTimeTickStamp - lastFSM_RunTimeTickStamp >= SRV_TASK_NET_LAN8700_FSM_PERIOD_MS)
        {
            //执行周期函数
            SrvTaskNetLAN8700Func_FSM_RunPeriod();
            //更新时间
            lastFSM_RunTimeTickStamp = currentTimeTickStamp;
        }
#endif
        taskMsgPtr = NULL;
#if(SRV_TASK_NET_LAN8700_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskNetLAN8700,(void*)(&taskMsgPtr),TX_NO_WAIT);
#else
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskNetLAN8700,(void*)(&taskMsgPtr),TX_WAIT_FOREVER);
#endif
        if((txResult == TX_SUCCESS)&&(taskMsgPtr != NULL))
        {
            //对消息进行处理
            SrvTaskNetLAN8700FuncMsgProcess(taskMsgPtr);
        }
        else
        {
            //没有收到数据,异常
#if(SRV_TASK_NET_LAN8700_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
            //周期函数定时运行
            CoreDelayMs(SRV_TASK_MIN_TICK);
#else
            //无限等待,不应该到这里来,打印错误日志
            SystemPrintf("%s,%d,txResult: 0X%02X\r\n",__func__,__LINE__,txResult);
#endif
        }
    }
}


//任务相关变量初始化
static void SrvTaskNetLAN8700FuncVarInit(void)
{
    asm("nop");
}

//任务消息处理
static void SrvTaskNetLAN8700FuncMsgProcess(void* taskMsgPtr)
{
    
}


//定义执行周期,定义周期函数
#if(SRV_TASK_NET_LAN8700_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskNetLAN8700Func_FSM_Init(void)
{
    
}

//周期函数运转
void SrvTaskNetLAN8700Func_FSM_RunPeriod(void)
{
    //网线插拔检测状态机
    SrvImplNetLan8700AllPortFSM();
    //主通讯端口状态机
    SrvImplNetLan8700MainPortFSM();
    //日志通讯端口状态机
    SrvImplNetLan8700LogPortFSM();
}

#endif
