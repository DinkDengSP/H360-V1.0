/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-31 13:28:40
**LastEditors: DengXiaoJun
**LastEditTime: 2021-01-04 17:18:05
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskCanRecv[STK_SIZE_TASK_CAN_RECV];
#pragma pack()

//启动任务的堆栈
OS_TCB tcbTaskCanRecv;


//CAN总线接收回调函数
void OS_CanReceiveDataCallFunc(CanRxMsg* rxMsg)
{
    OS_ERR err;
    //消息转发
    OSTaskQPost((OS_TCB *)&tcbTaskCanRecv, //向协议处理线程发送消息
                    (void *)rxMsg,
                    (OS_MSG_SIZE)sizeof(CanRxMsg),
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,数据放回空闲队列
        ADT_CircleCanRxPtrQueueWrite(&canCirclePtrIdleQueue,rxMsg);
        return;
    }
}



//创建系统处理框架,将CAN口信息转发到指定的线程进行处理
//前一个是ID,后一个是转发处理的任务的堆栈
const CAN_DISPATCH_UNIT canDispatchArray[] = {
    /************************调试指令*************************************************该指令转发的信道地址************/
    {CAN_CMD_SOFT_EMERGENCY_STOP                                ,       &tcbTaskHeart         },//运行时软件急停
    {CAN_CMD_MACHINE_RESET                                      ,       &tcbTaskHeart         },//整机复位
    {CAN_CMD_CELL_UPDATA_START                                  ,       &tcbTaskHeart         },//开始升级,升级信息
    {CAN_CMD_CELL_UPDATA_DATA                                   ,       &tcbTaskHeart         },//升级数据
    {CAN_CMD_CELL_UPDATA_END                                    ,       &tcbTaskHeart         },//升级结束，程序CRC校验码
    {CAN_CMD_CELL_UPDATA_PREPARE                                ,       &tcbTaskHeart         },//升级准备，跳转Boot等
    {CAN_CMD_CELL_VERSION                                       ,       &tcbTaskHeart         },//读取版本号
    {CAN_CMD_CELL_BOOT_VERSION                                  ,       &tcbTaskHeart         },//读取Boot版本号
};
//转发数组的长度
#define DISPATCH_ELEMENT_LENGTH                         sizeof(canDispatchArray)/sizeof(canDispatchArray[0])


//CAN口数据接收处理任务
void TaskFuncCanRecv(void *p_arg)
{
    OS_ERR err;
    //接收的数据指针
	CanRxMsg *rxMsg = NULL;
    //任务接收数据长度
	OS_MSG_SIZE size;
    p_arg = p_arg;
    while(1)
    {
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        rxMsg = OSTaskQPend((OS_TICK)0,
                            (OS_OPT)OS_OPT_PEND_BLOCKING,
                            (OS_MSG_SIZE *)&size,
                            (CPU_TS *)0,
                            (OS_ERR *)&err);
        if ((rxMsg != NULL)&&(err == OS_ERR_NONE))
		{
            //数据单元处理
            CanRecvProc(canBoardID,rxMsg);
            //数据转发
            CanDispatchReceive((CAN_DISPATCH_UNIT*)canDispatchArray,DISPATCH_ELEMENT_LENGTH);
            //处理完成的数据写回空闲队列
            ADT_CircleCanRxPtrQueueWrite(&canCirclePtrIdleQueue,rxMsg);
            rxMsg = NULL;
		}
    }
}









