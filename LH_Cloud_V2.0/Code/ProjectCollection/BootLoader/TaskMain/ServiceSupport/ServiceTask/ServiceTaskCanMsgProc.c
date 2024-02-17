/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-09-02 14:06:02
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-03 15:19:18
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
CPU_STK stackBufferServiceTaskCanMsgProc[STK_SIZE_SERVICE_TASK_CAN_MSG_PROC]@".ccram";
#pragma pack()
//任务控制块
OS_TCB tcbServiceTaskCanMsgProc@".ccram";


//CAN总线接收回调函数
void OS_Can1ReceiveDataCallFunc(CanRxMsg* rxMsg)
{
    //将数据转发到处理线程
    OS_ERR err;
    //将申请的数据转发到协议处理线程
    OSTaskQPost((OS_TCB *)&tcbServiceTaskCanMsgProc, //向协议处理线程发送消息
                (void *)rxMsg,
                (OS_MSG_SIZE)sizeof(CanRxMsg*),
                (OS_OPT)OS_OPT_POST_FIFO,
                (OS_ERR *)&err);

    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //将数据写回空闲队列
        ADT_CirclePtrQueueWrite(&can1CirclePtrIdleQueue,rxMsg);
    }
}

//创建系统处理框架,将CAN口信息转发到指定的线程进行处理
//前一个是ID,后一个是转发处理的任务的堆栈
const CAN1_DISPATCH_UNIT can1DispatchArray[] = {
    /************************调试指令*************************************************该指令转发的信道地址************/
    {CAN1_CMD_TEST_FLOW_CMD_CELL_VERSION                                ,       &tcbTaskBoardIap },//读取软件版本号
    {CAN1_CMD_TEST_FLOW_CMD_CELL_BOOT_VERSION                           ,       &tcbTaskBoardIap },//读取Boot版本
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_PREPARE                         ,       &tcbTaskBoardIap },//升级准备
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_START                           ,       &tcbTaskBoardIap },//升级开始
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_DATA                            ,       &tcbTaskBoardIap },//升级数据
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_END                             ,       &tcbTaskBoardIap },//升级结束
    {CAN1_CMD_TEST_FLOW_CMD_CELL_PROGRAM_JUMP                           ,       &tcbTaskBoardIap },//程序跳转
};
//转发数组的长度
#define DISPATCH_ELEMENT_LENGTH                         sizeof(can1DispatchArray)/sizeof(can1DispatchArray[0])


//任务函数
void ServiceTaskFuncCanMsgProc(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
	//接收的数据指针
	CanRxMsg *rxMsg = NULL;
    //初始化协议栈
    Can1ProcDataInit();
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
            Can1RecvProc(rxMsg);
            //数据转发
            Can1DispatchReceive((CAN1_DISPATCH_UNIT*)can1DispatchArray,DISPATCH_ELEMENT_LENGTH);
            //处理完成的数据写回空闲队列
            ADT_CirclePtrQueueWrite(&can1CirclePtrIdleQueue,rxMsg);
            rxMsg = NULL;
		}
    }
}
