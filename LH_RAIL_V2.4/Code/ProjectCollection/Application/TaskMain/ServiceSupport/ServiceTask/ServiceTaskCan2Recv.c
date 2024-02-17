/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:11:43
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-24 16:22:45
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceTaskCan2Recv.c
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
CPU_STK stackBufferTaskServiceCan2Recv[STK_SIZE_TASK_SERVICE_CAN2_RECV]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceCan2Recv@".dtcm_ram";

//CAN2总线接收回调函数
void OS_Can2ReceiveDataCallFunc(MCU_CAN_RECV_DAT* rxMsg)
{
    //将数据转发到处理线程
    OS_ERR err;

    //将申请的数据转发到协议处理线程
    OSTaskQPost((OS_TCB *)&tcbTaskServiceCan2Recv, //向协议处理线程发送消息
                (void *)rxMsg,
                (OS_MSG_SIZE)sizeof(MCU_CAN_RECV_DAT*),
                (OS_OPT)OS_OPT_POST_FIFO,
                (OS_ERR *)&err);

    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //写回空闲队列
        ADT_CircleCanRxPtrQueueWrite(&can2CirclePtrIdleQueue,rxMsg);
    }
}

//任务函数,接收下位机返回的信息
void TaskFuncServiceCan2Recv(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
	//接收的数据指针
	MCU_CAN_RECV_DAT *rxMsg = NULL;
    //初始化协议栈
    Can2ProcDataInit();
    while (1)
	{
        //请求消息,堵塞访问,协议处理任务必须实时性极高
		rxMsg = OSTaskQPend((OS_TICK)0,
								 (OS_OPT)OS_OPT_PEND_BLOCKING,
								 (OS_MSG_SIZE *)&size,
								 (CPU_TS *)0,
								 (OS_ERR *)&err);
		if ((rxMsg != NULL)&&(err == OS_ERR_NONE))
		{
            //数据单元处理
            Can2RecvProc(rxMsg);
            //写回空闲队列
            ADT_CircleCanRxPtrQueueWrite(&can2CirclePtrIdleQueue,rxMsg);
            rxMsg = NULL;
		}
    }
}
