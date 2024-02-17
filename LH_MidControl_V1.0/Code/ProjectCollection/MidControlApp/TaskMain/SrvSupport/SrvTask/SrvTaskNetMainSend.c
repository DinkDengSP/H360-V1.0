/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-04 13:26:00
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-12 16:10:55
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SrvTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
uint64_t stackBufferSrvTaskNetMainSend[STK_SIZE_SRV_TASK_NET_MAIN_SEND/8]@".dtcm_ram2";
#pragma pack()
//任务的控制块
TX_THREAD tcbSrvTaskNetMainSend@".dtcm_ram2";
//任务消息队列缓冲区,4字节对齐
#pragma pack(8)
uint32_t queueBufferSrvTaskNetMainSend[LENGTH_QUEUE_SRV_TASK_NET_MAIN_SEND]@".dtcm_ram2";
#pragma pack()
//任务消息队列控制块
TX_QUEUE queueSrvTaskNetMainSend@".dtcm_ram2";

//降低网络通讯速率
static void TaskTcpDataSendWaitStable(uint32_t sendDataLength);

//任务函数
void TaskFuncSrvNetMainSend(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //从消息队列中获取到的消息指针
    TCP_SEND_CMD_DATA* netSendMsgPtr = NULL;
    //TCP数据重发次数计数器
    uint8_t tcpDataSendRepeatCount = 0;
    //TCP数据发送错误记录器
    TCP_DATA_SEND_ERR lastDataSendErrState = TCP_DATA_SEND_ERR_NONE;
    while(1)
    {
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskNetMainSend,&netSendMsgPtr,TX_WAIT_FOREVER);
        if((txResult == TX_SUCCESS)&&(netSendMsgPtr != NULL))
        {
            if(netSendMsgPtr->dataBufferPtr == NULL)
            {
                //数据结构体为空,报警并且结束本次循环
                SystemPrintf("[MID]E>TcpNet SendDataMsgBuffer NULL!!!\r\n");
                //释放消息内存
                UserMemFree(MEM_SDRAM1,netSendMsgPtr);
                continue;
            }
            //获取到有效的数据需要发送
            if (SOCK_ESTABLISHED == TcpBaseUtilGetMainPortState())
            {
                if(netSendMsgPtr->command == TCP_CMD_STATUS)
                {
                    //更新一次数据发送时间
                    TcpBaseUtilSetLastHeartTickNow();
                }
                //网口数据发送,设置循环重发接口
                for(tcpDataSendRepeatCount = 0; tcpDataSendRepeatCount < TCP_DATA_SEND_RETRY_MAX;tcpDataSendRepeatCount++)
                {
                    //设置发送错误代码为无错误
                    lastDataSendErrState = TCP_DATA_SEND_ERR_NONE;
                    //如果数据发送失败,打印失败信息
                    if(LH_ERR_NONE != BoardW5500_SendBufferMain( netSendMsgPtr->dataBufferPtr, netSendMsgPtr->dataLength))
                    {
                        //打印发送错误讯息,网卡芯片异常,数据不出去
                        SystemPrintf("[MID]W>NetChipDataSendFail,RetryMax:3,W5500 Error,Cmd: 0X%04X\r\n",netSendMsgPtr->command);
                        //网卡芯片发送失败的情况下,不开重传,退出发送循环
                        lastDataSendErrState = TCP_DATA_SEND_ERR_W5500;
                        break;
                    }
                    //如果是回传ACK,则不再等待ACK,脱机指令也不会等待ACK
                    if (netSendMsgPtr->command == TCP_CMD_COMMUNICATE_ACK || netSendMsgPtr->command == TCP_CMD_OFF_LINE)
                    {
                        //不等待ACK,发出去就算完事了,退出循环
                        break;
                    }
                    //如果是其他指令或者数据的发送,需要等待上位机的ACK,
                    txResult = tx_semaphore_get(TcpBaseUtilGetNetAckSemPtr(), TCP_DATA_SEND_WAIT_ACK_TIME_OUT_MAX_MS);
                    if (txResult != TX_SUCCESS)
                    {
                        //超时没获取到ACK
                        SystemPrintf("[MID]W>PendTcpAckSemTimeOut,Error : %d,Cmd : %d\r\n", txResult,netSendMsgPtr->command);
                        //重发次数未超过三次,进行下一次重发
                        lastDataSendErrState = TCP_DATA_SEND_ERR_NO_RETURN_RETRY_OUT;
                        continue;
                    }
                    //运行到该位置说明收到了上位机回传的ack信息,要查看这个ACK信息对不对
                    if (TcpBaseUtilGetNetAckFlag() == TCP_NET_ACK_OK)
                    {
                        //收到了正确的ACK标志,发送完成
                        break;
                    }
                    //收到的ack显示ack标志位0,上位机这一帧数据可能没收全
                    SystemPrintf("[MID]W>TcpRecvAckFlag : %d\r\n", TcpBaseUtilGetNetAckFlag());
                    //下一次循环重发
                    lastDataSendErrState = TCP_DATA_SEND_ERR_RETURN_ERR_RETRY_OUT;
                }
                //检查数据发送结果
                if(lastDataSendErrState != TCP_DATA_SEND_ERR_NONE)
                {
                    //打印数据为什么发送失败
                    SystemPrintf("[MID]E>Tcp Send DataErrorOccor! Reason : 0X%02X\r\n", lastDataSendErrState);
                }
                //可选的降低通讯速率函数       
                TaskTcpDataSendWaitStable(netSendMsgPtr->dataLength);
            }
            else
            {
                //数据发送时间清空
                TcpBaseUtilSetLastHeartTick(HEART_TICK_ERR_UNUSED);
                //网口未连接,这一帧数据发送不出去,也不要尝试再发了,直接日志中报错,防止系统莫名其妙的卡住
                SystemPrintf("[MID]W>TcpSendDataFail,NetNotConnect,TcpState : %02X, TcpCmd: %04X, TcpDataLength : %d\r\n", 
                                TcpBaseUtilGetMainPortState(), //网口状态
                                netSendMsgPtr->command,//指令码
                                netSendMsgPtr->dataLength//发送数据长度
                            );
            }
            //释放消息数据体内存
            if(netSendMsgPtr->dataBufferPtr != NULL)
            {
                UserMemFree(MEM_SDRAM1,netSendMsgPtr->dataBufferPtr);
            }
            //释放消息内存
            UserMemFree(MEM_SDRAM1,netSendMsgPtr);
            //设置消息指针为空
            netSendMsgPtr = NULL;
        }
    }
}


//降低网络通讯速率
static void TaskTcpDataSendWaitStable(uint32_t sendDataLength)
{
#if 0
    if (sendDataLength > 1000) 
        CoreDelayMs(800);
    else if (sendDataLength > 500) 
        CoreDelayMs(500);
    else if (sendDataLength > 200) 
        CoreDelayMs(200);
    else 
        CoreDelayMs(100);
#else
    asm("nop");
#endif
}

//网卡数据发送,异步执行
LH_ERR NetChipSendMsgAsync(TCP_SEND_CMD_DATA* cmdDataPtr)
{
    TCP_SEND_CMD_DATA* cmdDataPtrLocal = cmdDataPtr;
    uint32_t txResult = TX_SUCCESS;
    if (SOCK_ESTABLISHED == TcpBaseUtilGetMainPortState())
    {
        txResult = tx_queue_send(&queueSrvTaskNetMainSend,(void*)(&cmdDataPtrLocal),TX_NO_WAIT);
        if(txResult != TX_SUCCESS)
        {
            //队列异常,数据发不出去
            return LH_ERR_BOARD_MAIN_COMM_NET_DISPATCH_ERR;
        }
    }
    else
    {
        return LH_ERR_BOARD_COMM_W5500_SEND_NET_DISCONNECT;
    }
    return LH_ERR_NONE;
}
