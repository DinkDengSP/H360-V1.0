/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-10 19:09:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-10 19:30:27 +0800
************************************************************************************************/ 
#include "SrvImplBase.h"
#include "SrvTaskConfig.h"
#include "SrvImplNetSocket.h"


//清空队列中全部消息
static void SrvImplNetMainSendClearAllMsg(TCP_SEND_CMD_DATA* netSendMsgPtr)
{
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //将丢弃的消息打印出来
    SystemPrintf("NetMainSend ClearMsg Cmd: 0X%04X,DataLen: %d\r\n",netSendMsgPtr->command,
                    netSendMsgPtr->dataLength);
    //释放消息数据体内存
    if((netSendMsgPtr->dataBufferPtr != NULL)&&(netSendMsgPtr->dataLength != 0))
    {
        UserMemFree(MEM_SDRAM1,netSendMsgPtr->dataBufferPtr);
    }
    UserMemFree(MEM_SDRAM1,netSendMsgPtr);
    do
    {
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskNetMainSend,&netSendMsgPtr,TX_NO_WAIT);
        if((txResult == TX_SUCCESS)&&(netSendMsgPtr != NULL))
        {
            //将丢弃的消息打印出来
            SystemPrintf("NetMainSend ClearMsg Cmd: 0X%04X,DataLen: %d\r\n",netSendMsgPtr->command,
                            netSendMsgPtr->dataLength);
            //将得到的消息释放掉
            if((netSendMsgPtr->dataLength != 0) &&(netSendMsgPtr->dataBufferPtr != NULL))
            {
                UserMemFree(MEM_SDRAM1,netSendMsgPtr->dataBufferPtr);
            }
            //释放内存
            UserMemFree(MEM_SDRAM1,netSendMsgPtr);
        }
        netSendMsgPtr = NULL;
        //一直到申请不到数据
    }while(txResult == TX_SUCCESS);
}

//丢弃当前讯息
static void SrvImplNetMainSendThrowAwayCurrentMsg(TCP_SEND_CMD_DATA* netSendMsgPtr)
{
    //先把当前的释放掉
    if((netSendMsgPtr->dataLength != 0) &&(netSendMsgPtr->dataBufferPtr != NULL))
    {
        UserMemFree(MEM_SDRAM1,netSendMsgPtr->dataBufferPtr);
    }
    //释放内存
    UserMemFree(MEM_SDRAM1,netSendMsgPtr);
}

//等待网线连接
static uint8_t SrvImplNetMainSendWaitPhyConnect(TCP_SEND_CMD_DATA* netSendMsgPtr)
{
    //等待时间
    int32_t timeWaitStartStamp = 0;
    //检查网口是否连接
    if((SUCCESS != SrvStateCheckSocketBaseInitOver())||(NX_TRUE != SrvStateCheckSocketBaseCheckPhyState()))
    {
        timeWaitStartStamp = tx_time_get();
        //打印断网讯息
        SystemPrintf("NetMainSend NetInitState: %d,PhyConnectState: %d\r\n",SrvStateCheckSocketBaseInitOver(),
                        SrvStateCheckSocketBaseCheckPhyState());
        //如果网口未连接,需要等待网口物理连接
        while((SUCCESS != SrvStateCheckSocketBaseInitOver())||(NX_TRUE != SrvStateCheckSocketBaseCheckPhyState()))
        {
            //等待网络连接
            if(tx_time_get() - timeWaitStartStamp >= NET_SEND_WAIT_NET_CONNECT_MAX_TIME_MS)
            {
                //脱机时间大于一定值,将内存中的全部数据清空
                SystemPrintf("NetMainSend NetPhyNotConnect TimeOut,ClearAllMsg\r\n");
                //打印清空讯息
                SrvImplNetMainSendClearAllMsg(netSendMsgPtr);
                return 0;
            }
            //队列已经满了
            if(queueSrvTaskNetMainSend.tx_queue_available_storage == 0)
            {
                SystemPrintf("NetMainSend NetPhyNotConnect QueueFull,ThrowAway CurrentMsg,Cmd: 0X%04X,DataLen: %d\r\n",
                                netSendMsgPtr->command,netSendMsgPtr->dataLength);
                //队列中没有可用空间了,这一条消息清掉
                SrvImplNetMainSendThrowAwayCurrentMsg(netSendMsgPtr);
                return 0;
            }
            //等待网线连接
            CoreDelayMinTick();
        }
        //网线已连接
        return 1;
    }
    else
    {
        return 1;
    }
}


//等待Socket连接
static uint8_t SrvImplNetMainSendWaitSocketConnect(TCP_SEND_CMD_DATA* netSendMsgPtr)
{
    //等待时间
    int32_t timeWaitStartStamp = 0;
    ULONG mainSocketState = SrvImplNetSocketMainGetState();
    if(NX_TCP_ESTABLISHED != mainSocketState)
    {
        //网络未连接
        timeWaitStartStamp = tx_time_get();
        //打印断网讯息
        SystemPrintf("NetMainSend MainSocket Not Connect,State: %s\r\n",SrvState_NetX_GetSocketState(mainSocketState));
        //如果Socket未连接,需要等待Socket连接
        while(NX_TCP_ESTABLISHED != SrvImplNetSocketMainGetState())
        {
            //等待网络连接
            if(tx_time_get() - timeWaitStartStamp >= NET_SEND_WAIT_NET_CONNECT_MAX_TIME_MS)
            {
                //脱机时间大于一定值,将内存中的全部数据清空
                SystemPrintf("NetMainSend MainSocket NotConnect TimeOut,ClearAllMsg\r\n");
                //打印清空讯息
                SrvImplNetMainSendClearAllMsg(netSendMsgPtr);
                return 0;
            }
            //队列已经满了
            if(queueSrvTaskNetMainSend.tx_queue_available_storage == 0)
            {
                SystemPrintf("NetMainSend NetPhyNotConnect QueueFull,ThrowAway CurrentMsg,Cmd: 0X%04X,DataLen: %d\r\n",
                                netSendMsgPtr->command,netSendMsgPtr->dataLength);
                //队列中没有可用空间了,这一条消息清掉
                SrvImplNetMainSendThrowAwayCurrentMsg(netSendMsgPtr);
                return 0;
            }
            //等待Socket连接
            CoreDelayMinTick();
        }
        //Socket已连接
        return 1;
    }
    else
    {
        return 1;
    }
}

//清除信号量
void  SrvImplNetMainSendClearAckSemaphore(void)
{
    //全部申请掉
    while (TX_SUCCESS == tx_semaphore_get(TcpBaseUtilGetNetAckSemPtr(),TX_NO_WAIT));
}

//处理主网口接收到的数据讯息
void SrvImplNetMainSendDataPorcess(TCP_SEND_CMD_DATA* netSendMsgPtr)
{
    //辅助变量
    uint8_t indexUtil = 0;
    uint32_t txResult = TX_SUCCESS;
    //等待网线连接
    if(0 == SrvImplNetMainSendWaitPhyConnect(netSendMsgPtr))
    {
        //发生异常,数据发不出去
        return;
    }
    if(0 == SrvImplNetMainSendWaitSocketConnect(netSendMsgPtr))
    {
        //Socket没连接,数据发不出去
        return;
    }
    ULONG mainSocketState = SrvImplNetSocketMainGetState();
    //到这,物理连接检测通过,需要检测Socket连接
    for(indexUtil = 0; indexUtil < TCP_DATA_SEND_RETRY_MAX;indexUtil++)
    {
        //如果重发计数大于0而且指令是ACK
        if((indexUtil > 0)&&(netSendMsgPtr->command == TCP_CMD_COMMUNICATE_ACK))
        {
            break;
        }
        mainSocketState = SrvImplNetSocketMainGetState();
        if(NX_TCP_ESTABLISHED != mainSocketState)
        {
            //发送过程中检测到Socket断开,结束发送
            SystemPrintf("NetSending CheckSocket State: %s,Exit Current Send,Cmd: 0X%04X,DataLen: %d\r\n",
                            SrvState_NetX_GetSocketState(mainSocketState),netSendMsgPtr->command,
                            netSendMsgPtr->dataLength);
            //发送过程中检测到Socket断开,结束发送
            if((netSendMsgPtr->dataLength != 0) &&(netSendMsgPtr->dataBufferPtr != NULL))
            {
                UserMemFree(MEM_SDRAM1,netSendMsgPtr->dataBufferPtr);
            }
            //释放内存
            UserMemFree(MEM_SDRAM1,netSendMsgPtr);
            return;
        }
        //打印重发讯息
        if(indexUtil > 0)
        {
            SystemPrintf("NetSending Retry %d,Cmd: 0X%04X,DataLen: %d\r\n",indexUtil,
                            netSendMsgPtr->command,netSendMsgPtr->dataLength);
        }
        //发送之前清空ACK信号量
        SrvImplNetMainSendClearAckSemaphore();
        //数据发送
        uint32_t nxResult = SrvImplNetSocketMainPortSendBuf(netSendMsgPtr->dataBufferPtr,netSendMsgPtr->dataLength);
        if(nxResult != NX_SUCCESS)
        {
            //信息打印
            SystemPrintf("NetSending Failed: 0X%02X,Retry %d,Cmd: 0X%04X,DataLen: %d\r\n",nxResult,indexUtil,
                            netSendMsgPtr->command,netSendMsgPtr->dataLength);
            if(netSendMsgPtr->command == TCP_CMD_COMMUNICATE_ACK)
            {
                //ACK信息,失败了就失败了,
                break;
            }
            //查看是否需要重发
            if(indexUtil < 2)
            {
                //发过两次了就不重发不等待了,不然就延时重发
                CoreDelayMs(1000);
            }
            continue;
        }
        if(netSendMsgPtr->command == TCP_CMD_STATUS)
        {
            //状态上报,更新一次数据发送时间
            TcpBaseUtilSetLastHeartTickNow();
        }
        //指令是ACK,无需等待信号量
        if(netSendMsgPtr->command == TCP_CMD_COMMUNICATE_ACK)
        {
            break;
        }
        //如果是其他指令或者数据的发送,需要等待上位机的ACK,
        txResult = tx_semaphore_get(TcpBaseUtilGetNetAckSemPtr(), TCP_DATA_SEND_WAIT_ACK_TIME_OUT_MAX_MS);
        if (txResult != TX_SUCCESS)
        {
            //超时没获取到ACK
            SystemPrintf("[MID]W>PendTcpAckSemTimeOut,Error : %d,Cmd : %d\r\n", txResult,netSendMsgPtr->command);
            //退出重发
            continue;
        }
        //运行到该位置说明收到了上位机回传的ack信息,要查看这个ACK信息对不对
        if (TcpBaseUtilGetNetAckFlag() == TCP_NET_ACK_OK)
        {
            //收到了正确的ACK标志,发送完成
            break;
        }
        else
        {
            //收到的ack显示ack标志位0,上位机这一帧数据可能没收全
            SystemPrintf("[MID]W>TcpRecvAckFlag : %d\r\n", TcpBaseUtilGetNetAckFlag());
            //下次重发
            continue;
        }
    }
    //发送完成打印日志
    SystemPrintf("NetSend OnePackOver,Cmd: 0X%04X,DataLen: %d\r\n",netSendMsgPtr->command,
                     netSendMsgPtr->dataLength);
    //发送完成,释放内存
    if((netSendMsgPtr->dataLength != 0) &&(netSendMsgPtr->dataBufferPtr != NULL))
    {
        UserMemFree(MEM_SDRAM1,netSendMsgPtr->dataBufferPtr);
    }
    //释放内存
    UserMemFree(MEM_SDRAM1,netSendMsgPtr);
    return;
}
