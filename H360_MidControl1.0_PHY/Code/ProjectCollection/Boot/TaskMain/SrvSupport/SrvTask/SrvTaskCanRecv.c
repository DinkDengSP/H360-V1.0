/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-16 18:24:52 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 18:26:55 +0800
************************************************************************************************/ 
#include "SrvTaskConfig.h"
#include "SrvImplCanRecv.h"

//任务的控制块
TX_THREAD tcbSrvTaskCanRecv@".dtcm_ram";
//任务堆栈
uint64_t stackBufferSrvTaskCanRecv[STK_SIZE_SRV_TASK_CAN_RECV/8]@".dtcm_ram";
//任务消息队列控制块
TX_QUEUE queueSrvTaskCanRecv@".dtcm_ram";
//消息缓冲区
uint32_t queueBufferSrvTaskCanRecv[LENGTH_QUEUE_SRV_TASK_CAN_RECV]@".dtcm_ram";


//任务相关变量初始化
static void SrvTaskCanRecvFuncVarInit(void);
//任务消息处理
static void SrvTaskCanRecvFuncMsgProcess(void* taskMsgPtr);
//定义执行周期,定义周期函数
#if(SRV_TASK_CAN_RECV_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskCanRecvFunc_FSM_Init(void);
//周期函数运转
static void SrvTaskCanRecvFunc_FSM_RunPeriod(void);
#endif


//任务函数
void SrvTaskCanRecvFuncEntry(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //任务消息队列
    void* taskMsgPtr = NULL;
#if(SRV_TASK_CAN_RECV_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
    //状态机初始化
    SrvTaskCanRecvFunc_FSM_Init();
    //上一次的外部信号检测发送的时间
	ULONG lastFSM_RunTimeTickStamp = tx_time_get();
    //更新定时时钟
    ULONG currentTimeTickStamp = tx_time_get();
#endif
    //任务相关变量初始化
    SrvTaskCanRecvFuncVarInit();
    while(1)
    {
#if(SRV_TASK_CAN_RECV_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
        //定义自动周期运行,检查当前时间
        currentTimeTickStamp = tx_time_get();
        if(currentTimeTickStamp - lastFSM_RunTimeTickStamp >= SRV_TASK_CAN_RECV_FSM_PERIOD_MS)
        {
            //执行周期函数
            SrvTaskCanRecvFunc_FSM_RunPeriod();
            //更新时间
            lastFSM_RunTimeTickStamp = currentTimeTickStamp;
        }
#endif
        taskMsgPtr = NULL;
#if(SRV_TASK_CAN_RECV_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskCanRecv,(void*)(&taskMsgPtr),TX_NO_WAIT);
#else
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskCanRecv,(void*)(&taskMsgPtr),TX_WAIT_FOREVER);
#endif
        if((txResult == TX_SUCCESS)&&(taskMsgPtr != NULL))
        {
            //对消息进行处理
            SrvTaskCanRecvFuncMsgProcess(taskMsgPtr);
            //清空指针
            taskMsgPtr = NULL;
        }
        else
        {
            //没有收到数据,异常
#if(SRV_TASK_CAN_RECV_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
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
static void SrvTaskCanRecvFuncVarInit(void)
{
    asm("nop");
}

//任务消息处理
static void SrvTaskCanRecvFuncMsgProcess(void* taskMsgPtr)
{
    
}


//定义执行周期,定义周期函数
#if(SRV_TASK_CAN_RECV_FSM_PERIOD_MS != SYS_TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskCanRecvFunc_FSM_Init(void)
{
    asm("nop");
}

//周期函数运转
void SrvTaskCanRecvFunc_FSM_RunPeriod(void)
{
    asm("nop");
}

#endif

