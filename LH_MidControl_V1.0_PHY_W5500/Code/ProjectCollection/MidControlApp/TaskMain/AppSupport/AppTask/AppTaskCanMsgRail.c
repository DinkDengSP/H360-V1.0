/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-11 11:18:37
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-12 16:09:38
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"
#include "AppImplInc.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
uint64_t stackBufferAppTaskCanMsgRail[STK_SIZE_APP_TASK_CAN_MSG_RAIL/8]@".dtcm_ram";
#pragma pack()
//任务的控制块
TX_THREAD tcbAppTaskCanMsgRail@".dtcm_ram";
//任务消息队列缓冲区,4字节对齐
#pragma pack(8)
uint32_t queueBufferAppTaskCanMsgRail[LENGTH_QUEUE_APP_TASK_CAN_MSG_RAIL]@".dtcm_ram";
#pragma pack()
//任务消息队列控制块
TX_QUEUE queueAppTaskCanMsgRail@".dtcm_ram";


//轨道CAN总线信息处理
void TaskFuncAppCanMsgRailSubProc(CAN_RECV_PACK* canRecPackPtr);

//任务函数
void TaskFuncAppCanMsgRail(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //从消息队列中获取到的消息指针
    CAN_RECV_PACK* canRecPackPtr = NULL;
    while(1)
    {
        canRecPackPtr = NULL;
        //申请信号量
        txResult = tx_queue_receive(&queueAppTaskCanMsgRail,&canRecPackPtr,TX_WAIT_FOREVER);
        if((txResult == TX_SUCCESS)&&(canRecPackPtr != NULL))
        {
            //解析处理CAN总线传输过来的数据,不同单元传递过来的数据分开处理
            TaskFuncAppCanMsgRailSubProc(canRecPackPtr);
            //释放内存
            if(canRecPackPtr->recvDataBuffer != NULL)
            {
                UserMemFree(MEM_SDRAM1,canRecPackPtr->recvDataBuffer);
            }
            //处理完成,释放内存
            UserMemFree(MEM_SDRAM1,canRecPackPtr);
        }
    }
}


//轨道CAN总线信息处理
void TaskFuncAppCanMsgRailSubProc(CAN_RECV_PACK* canRecPackPtr)
{
    //先回传ACK,然后进行处理
    if(CAN_CMD_COMM_LOG != canRecPackPtr->command)
    {
        //只要不是日志上传且不是ACK,就快速发送一个ACK
        CanSendAckDefault(canRecPackPtr->srcModuleNo,canRecPackPtr->srcBoardAddr,CAN_ACK_STATE_OK);
    }
    AppTaskAppCommandSendByCanRailCommand(canRecPackPtr->srcModuleNo,canRecPackPtr->srcBoardAddr,canRecPackPtr->command,
                                canRecPackPtr->recvDataBuffer,canRecPackPtr->recvDataLength);
}

