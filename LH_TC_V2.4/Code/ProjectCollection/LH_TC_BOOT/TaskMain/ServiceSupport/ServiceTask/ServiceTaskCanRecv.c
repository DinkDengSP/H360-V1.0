/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-16 16:24:11
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-06 11:47:21
**FilePath: \LH_TC_BOOT_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceTaskCanRecv.c
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
CPU_STK stackBufferServiceTaskCanRecv[STK_SIZE_SERVICE_TASK_CAN_RECV];
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskCanRecv;

//CAN总线接收回调函数
void OS_CanReceiveDataCallFunc(CanRxMsg* rxMsg)
{
    OS_ERR err;
    //消息转发
    OSTaskQPost((OS_TCB *)&tcbServiceTaskCanRecv, //向协议处理线程发送消息
                    (void *)rxMsg,
                    (OS_MSG_SIZE)sizeof(CanRxMsg*),
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,数据放回空闲队列
        ADT_CirclePtrQueueWrite(&can1CirclePtrIdleQueue,rxMsg);
        return;
    }
}

//系统配置支持的指令分组
static const CAN_SLAVE_DISPATCH_CONFIG canSlaveDispatchConfigArray[CAN_PROTOCOL_BOARD_CHANNEL_MAX] = {
    {0      ,       &tcbServiceTaskSensor      },
    {1      ,       &tcbServiceTaskHeart       },
    {2      ,       NULL                       },
    {3      ,       NULL                       },
    {4      ,       NULL                       },
    {5      ,       NULL                       },
    {6      ,       NULL                       },
    {7      ,       NULL                       },
    {8      ,       NULL                       },
    {9      ,       NULL                       },
    {10     ,       NULL                       },
    {11     ,       NULL                       },
    {12     ,       NULL                       },
    {13     ,       NULL                       },
    {14     ,       NULL                       },
    {15     ,       NULL                       },
    {16     ,       NULL                       },
    {17     ,       NULL                       },
    {18     ,       &tcbServiceTaskIap         },//以上为系统服务任务,基础大同小异,以下为应用服务程序,各个大不相同
    {19     ,       NULL                       },
    {20     ,       NULL                       },
    {21     ,       NULL                       },
    {22     ,       NULL                       },
    {23     ,       NULL                       },
    {24     ,       NULL                       },
    {25     ,       NULL                       },
    {26     ,       NULL                       },
    {27     ,       NULL                       },
    {28     ,       NULL                       },
    {29     ,       NULL                       },
    {30     ,       NULL                       },
    {31     ,       NULL                       },
};

//任务函数
void ServiceTaskFuncCanRecv(void *p_arg)
{
    OS_ERR err;
    //接收的数据指针
	CanRxMsg *rxMsg = NULL;
    //任务接收数据长度
	OS_MSG_SIZE size;
    p_arg = p_arg;
    //初始化协议栈,接收部分
    CanSlaveInit((CAN_SLAVE_DISPATCH_CONFIG*)canSlaveDispatchConfigArray);
    //ID快速处理
    CAN_RECV_FAST_ID fastIDRecv = {0};
    while (1)
	{
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        rxMsg = OSTaskQPend((OS_TICK)0,
                             (OS_OPT)OS_OPT_PEND_BLOCKING,
                             (OS_MSG_SIZE *)&size,
                             (CPU_TS *)0,
                             (OS_ERR *)&err);
        if ((rxMsg != NULL)&&(err == OS_ERR_NONE))
        {
            //快速ID处理
            CanBaseCreateFastID(rxMsg->ExtId,&fastIDRecv);
            //如果是指令包,调用从机协议族处理
            if(fastIDRecv.packType == CAN_BASE_PACK_CMD)
            {
                //从机单元数据处理
                CanSlaveProcRecvData(&fastIDRecv,rxMsg);
                //从机单元数据转发到不同的处理线程
                CanSlaveDispatchData2SpecialTask();
            }
            //处理完成,将数据放回空闲区域
            ADT_CirclePtrQueueWrite(&can1CirclePtrIdleQueue,rxMsg);
            rxMsg = NULL;
        }
    }
}
