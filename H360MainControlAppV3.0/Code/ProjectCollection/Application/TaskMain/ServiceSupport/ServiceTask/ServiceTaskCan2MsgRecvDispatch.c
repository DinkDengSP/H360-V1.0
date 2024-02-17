/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:33:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-23 16:39:16
**FilePath: \Application\TaskMain\ServiceSupport\ServiceTask\ServiceTaskCan2MsgRecvDispatch.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"
#include "Can2ProcMain.h"

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
    OS_ERR err;
    //消息转发
    OSTaskQPost((OS_TCB *)&tcbTaskServiceCan2Recv, //向协议处理线程发送消息
                    (void *)rxMsg,
                    (OS_MSG_SIZE)sizeof(MCU_CAN_RECV_DAT*),
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,数据放回空闲队列
        ADT_CircleCanRxPtrQueueWrite(&can2CirclePtrIdleQueue,rxMsg);
        return;
    }
}

//任务函数,接收下位机返回的信息
void TaskFuncServiceCan2Recv(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    MCU_CAN_RECV_DAT* canMsg = NULL;
    //初始化协议栈
    Can2ProcDataInit();
    //CAN2初始化,与下位机通讯
    MCU_CAN2_Init(MCU_CAN_BPS_1000K,DEVICE_MAIN_BOARD_ID,OS_Can2ReceiveDataCallFunc);
    while (1)
	{
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        canMsg = OSTaskQPend((OS_TICK)0,
                             (OS_OPT)OS_OPT_PEND_BLOCKING,
                             (OS_MSG_SIZE *)&size,
                             (CPU_TS *)0,
                             (OS_ERR *)&err);
        if ((canMsg != NULL)&&(err == OS_ERR_NONE))
        {
            //数据单元处理
            Can2RecvProc(canMsg); 
            //将使用完的消息放回空闲队列
            ADT_CircleCanRxPtrQueueWrite(&can2CirclePtrIdleQueue,canMsg);
            canMsg = NULL;
        }
    }
}



