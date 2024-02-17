/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:12:37
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-07 15:32:02
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceTaskHeart.c
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
CPU_STK stackBufferTaskServiceHeart[STK_SIZE_TASK_SERVICE_HEART]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceHeart@".dtcm_ram";

//自动运行程序的执行周期,单位毫秒
#define HEART_AUTO_FUNC_RUNNING_PERIOD_MS            200
//系统循环自动程序初始化
static void ServiceTaskFuncHeart_Auto_Init(void);
#if(HEART_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void ServiceTaskFuncHeart_Auto_Proc(void);
#endif


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncHeart_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr);
//2. 来自自身的消息
static void ServiceTaskFuncHeart_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceHeart(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    ServiceTaskFuncHeart_Auto_Init();
    #if(HEART_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(HEART_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= HEART_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了HEART_AUTO_FUNC_RUNNING_PERIOD_MS个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			ServiceTaskFuncHeart_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)HEART_AUTO_FUNC_RUNNING_PERIOD_MS,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(SYSTEM_CMD_SRC_CAN1 == taskCommandPtr->cmdSrc)
            {
                //从CAN1传过来的指令
                CAN1_CMD_RECV* can1RecvDatPtr = (CAN1_CMD_RECV*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncHeart_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncHeart_SelfMsg_Proc(selfCmdDataPtr);
                //处理完成
                UserMemFree(MEM_DTCM,selfCmdDataPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            taskCommandPtr->cmdDataPtr = NULL;
            UserMemFree(MEM_DTCM,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}

/************************************************自动运行程序************************************************/
//系统循环自动程序初始化
static void ServiceTaskFuncHeart_Auto_Init(void)
{
    ServiceImplHeart_EventInit();
}
#if(HEART_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统循环运行程序,处理自动化任务
static void ServiceTaskFuncHeart_Auto_Proc(void)
{
    ServiceImplHeart_EventRun(HEART_AUTO_FUNC_RUNNING_PERIOD_MS);
}
#endif


/*************************************************1. 来自CAN1的消息******************************************/

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncHeart_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    return;
}


/*************************************************2. 来自自身的消息******************************************/
//匹配发送过来的指令序号
static void ServiceTaskFuncHeart_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_HEART* selfCmdResultDataPtr = (IPC_DATA_HEART*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (selfCmdResultDataPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	selfCmdResultDataPtr->state = STATE_IPC_WAIT_RETURN;
    //打印日志
	SystemDebugShowMsgStart((uint8_t*)HeartDebugStartMsg,(uint8_t*)"unsupport",
                                selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                                selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                                );
    //不支持的指令
    actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
    //模拟时序,假动作
    SystemDebugShowMsgEnd((uint8_t*)HeartDebugEndMsg,actionResult);
	//处理完成,设置状态
	selfCmdResultDataPtr->state = STATE_IPC_CMD_COMPLETE;
	selfCmdResultDataPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(selfCmdResultDataPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}
