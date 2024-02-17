/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-03 14:08:16
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-12 17:32:57
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SrvTaskConfig.h"
#include "SrvImplNetRecv.h"

//任务堆栈
uint64_t stackBufferSrvTaskNetRecv[STK_SIZE_SRV_TASK_NET_RECV/8]@".dtcm_ram2";
//任务的控制块
TX_THREAD tcbSrvTaskNetRecv@".dtcm_ram2";
//任务消息队列缓冲区,4字节对齐
uint32_t queueBufferSrvTaskNetRecv[LENGTH_QUEUE_SRV_TASK_NET_RECV]@".dtcm_ram2";
//任务消息队列控制块
TX_QUEUE queueSrvTaskNetRecv@".dtcm_ram2";

//任务函数
void TaskFuncSrvNetRecv(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    LH_ERR errorCode = LH_ERR_NONE;
    //从消息队列中获取到的消息指针
    NET_FSM_DATA_PACK* netRecvPackMsgPtr = NULL;
    while(1)
    {
        if(netRecvPackMsgPtr != NULL)
        {
            //每次运行前释放内存
            if(netRecvPackMsgPtr->dataBuffer != NULL)
            {
                UserMemFree(MEM_AXI_SRAM,netRecvPackMsgPtr->dataBuffer);
            }
            UserMemFree(MEM_AXI_SRAM,netRecvPackMsgPtr);
        }
        //申请数据之前先把指针清空
        netRecvPackMsgPtr = NULL;
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskNetRecv,(void*)(&netRecvPackMsgPtr),TX_WAIT_FOREVER);
        if((txResult == TX_SUCCESS)&&(netRecvPackMsgPtr != NULL))
        {
            if(netRecvPackMsgPtr->dataLength == 0)
            {
                //接收到的数据长度为0,退出本次循环
                continue;
            }
            //分析接收到的数据
            errorCode = SrvImplNetRecvWithProtocol(netRecvPackMsgPtr->dataBuffer,netRecvPackMsgPtr->dataLength);
            if(errorCode != LH_ERR_NONE)
            {
                SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,ErrorCode : 0X%08X\r\n",__FUNCTION__,__LINE__,errorCode);
            }
        }
    }
}




