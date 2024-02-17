/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-17 08:52:33
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-17 10:41:11
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceTaskHeart.c
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


//自动运行程序的执行周期,单位毫秒
#define SERVICE_TASK_HEART_AUTO_FUNC_RUNNING_PERIOD_MS            200
//系统循环自动程序初始化
static void TaskServiceHeart_Auto_Init(void);
//系统还处理一些循环自动化程序
static void TaskServiceHeart_Auto_Proc(void);


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void TaskFuncServiceHeart_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgPtr);
//2. 来自自身的消息
static void TaskFuncServiceHeart_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void ServiceTaskFuncHeart(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    //状态机初始化
    TaskServiceHeart_Auto_Init();
    while(1)
    {
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= SERVICE_TASK_HEART_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了SERVICE_TASK_HEART_AUTO_FUNC_RUNNING_PERIOD_MS个时间
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			TaskServiceHeart_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)SERVICE_TASK_HEART_AUTO_FUNC_RUNNING_PERIOD_MS,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(TASK_CMD_SRC_CAN == taskCommandPtr->cmdSrc)
            {
                 //从can来的数据
                CAN_SLAVE_CHANNEL_CACHE* canRecvDatPtr = (CAN_SLAVE_CHANNEL_CACHE*)taskCommandPtr->cmdData;
                //对这个数据进行处理
                TaskFuncServiceHeart_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                TaskFuncServiceHeart_SelfMsg_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
                taskCommandPtr->cmdData = NULL;
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}

/************************************************自动运行程序************************************************/
//系统循环自动程序初始化
static void TaskServiceHeart_Auto_Init(void)
{
    ServiceImplHeart_StateMachineInit();
}

//系统循环自动程序
static void TaskServiceHeart_Auto_Proc(void)
{
    ServiceImplHeart_StateMachineLoop();
}

/************************************************CAN总线接收到的消息的处理******************************************************/


//分发CAN指令,对接信息流
static void TaskFuncServiceHeart_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgPtr)
{

}

/************************************************自身分层API调用的消息的处理**********************************/


//分发队列消息,对接信息流
static void TaskFuncServiceHeart_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    
}

















