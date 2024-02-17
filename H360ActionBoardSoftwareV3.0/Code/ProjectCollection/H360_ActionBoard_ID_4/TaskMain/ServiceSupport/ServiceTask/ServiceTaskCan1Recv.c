#include "ServiceTaskConfig.h"
#include "AppTaskConfig.h"
#include "SelfBoardID.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferServiceTaskCan1Recv[STK_SIZE_SERVICE_TASK_CAN1_RECV]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskCan1Recv@".ccram";

//CAN总线接收回调函数
void OS_Can1ReceiveDataCallFunc(CanRxMsg* rxMsg)
{
    OS_ERR err;
    //消息转发
    OSTaskQPost((OS_TCB *)&tcbServiceTaskCan1Recv, //向协议处理线程发送消息
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
    {0  ,&tcbServiceTaskHeart           },
    {1  ,&tcbServiceTaskInputRead       },
    {2  ,&tcbServiceTaskOutputWrite     },
    {3  ,&tcbServiceTaskSerialUserRS1   },
    {4  ,&tcbServiceTaskSerialDebugRS2  },
    {5  ,&tcbServiceTaskStepMotor1      },
    {6  ,&tcbServiceTaskStepMotor2      },
    {7  ,&tcbServiceTaskStepMotor3      },
    {8  ,&tcbServiceTaskStepMotor4      },
    {9  ,&tcbServiceTaskStepMotor5      },
    {10 ,&tcbServiceTaskStepMotor6      },
    {11 ,&tcbServiceTaskStepMotor7      },
    {12 ,&tcbServiceTaskStepMotor8      },
    {13 ,&tcbServiceTaskStepMotor9      },
    {14 ,&tcbServiceTaskStepMotor10     },
    {15 ,&tcbServiceTaskStepMotor11     },
    {16 ,&tcbServiceTaskStepMotor12     },
    {17 ,&tcbServiceTaskParam           },
    {18 ,&tcbServiceTaskDebugIAP        },
    //以上为系统服务任务,基础大同小异,以下为应用服务程序,各个大不相同
    {19 ,&tcbAppTaskMixReagent          },
    {20 ,&tcbAppTaskNeedleReagent       },
    {21 ,&tcbAppTaskMixBead             },
    {22 ,&tcbAppTaskPlateReaction       },
    {23 ,&tcbAppTaskParam               },
    {24 ,NULL},
    {25 ,NULL},
    {26 ,NULL},
    {27 ,NULL},
    {28 ,NULL},
    {29 ,NULL},
    {30 ,NULL},
    {31 ,NULL},
};

//任务函数
void ServiceTaskFuncCan1Recv(void *p_arg)
{
    OS_ERR err;
    //接收的数据指针
	CanRxMsg *rxMsg = NULL;
    //任务接收数据长度
	OS_MSG_SIZE size;
    p_arg = p_arg;
    //初始化协议栈,接收部分
    CanSlaveInit((CAN_SLAVE_DISPATCH_CONFIG*)canSlaveDispatchConfigArray);
    //初始化协议栈,发送部分
    CanMasterProcDataInit();
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
            else
            {
                //主机单元数据处理
                CanMasterRecvProc(&fastIDRecv,rxMsg);
            }
            //处理完成,将数据放回空闲区域
            ADT_CirclePtrQueueWrite(&can1CirclePtrIdleQueue,rxMsg);
            rxMsg = NULL;
        }
    }
}
