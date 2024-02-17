/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 20:53:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:54:25 +0800
************************************************************************************************/ 
#include "SrvImplNetRecv.h"
#include "SrvImplNetSocket.h"
#include "SrvTaskConfig.h"

//任务全局对象初始化
void SrvImplNetRecvVarInit(void)
{

}

//W5500接受数据处理
static void SrvImplNetRecvMsgProcessW5500(NET_RECV_PACK* netRecvPackPtr)
{
    uint32_t sendResult;
    if((netRecvPackPtr->recvDatBufPtr != NULL)&&(netRecvPackPtr->recvDataLength != 0))
    {
        //原样发出去
        sendResult = SrvImplNetW5500SocketMainSendBuf(netRecvPackPtr->recvDatBufPtr,netRecvPackPtr->recvDataLength);
        if(sendResult != NX_SUCCESS)
        {
            SystemPrintf("SrvImplNetW5500SocketMainSendBuf Failed: 0X%02X\r\n",sendResult);
        }
    }
    //日志打印
    SystemPrintf("W5500 SocketMain RecvData,Len: %d,PeerIP: :%d.%d.%d.%d,PeerPort: %d\r\n",netRecvPackPtr->recvDataLength,
                    DWORD_TO_BYTE_HH(netRecvPackPtr->peerIpAddress),DWORD_TO_BYTE_HL(netRecvPackPtr->peerIpAddress),
                    DWORD_TO_BYTE_LH(netRecvPackPtr->peerIpAddress),DWORD_TO_BYTE_LL(netRecvPackPtr->peerIpAddress),
                    netRecvPackPtr->peerPort);

}

//LAN8700接受数据处理
static void SrvImplNetRecvMsgProcessLAN8700(NET_RECV_PACK* netRecvPackPtr)
{
    uint32_t sendResult;
    if((netRecvPackPtr->recvDatBufPtr != NULL)&&(netRecvPackPtr->recvDataLength != 0))
    {
        //原样发出去
        sendResult = SrvImplNetLan8700SocketMainSendBuf(netRecvPackPtr->recvDatBufPtr,netRecvPackPtr->recvDataLength);
        if(sendResult != NX_SUCCESS)
        {
            SystemPrintf("SrvImplNetLan8700SocketMainSendBuf Failed: 0X%02X\r\n",sendResult);
        }
    }
    //日志打印
    SystemPrintf("DM9216 SocketMain RecvData,Len: %d,PeerIP: :%d.%d.%d.%d,PeerPort: %d\r\n",netRecvPackPtr->recvDataLength,
                    DWORD_TO_BYTE_HH(netRecvPackPtr->peerIpAddress),DWORD_TO_BYTE_HL(netRecvPackPtr->peerIpAddress),
                    DWORD_TO_BYTE_LH(netRecvPackPtr->peerIpAddress),DWORD_TO_BYTE_LL(netRecvPackPtr->peerIpAddress),
                    netRecvPackPtr->peerPort);
}

//任务接收消息处理
void SrvImplNetRecvMsgProcess(void* taskMsgPtr)
{
    NET_RECV_PACK* netRecvPackPtr = (NET_RECV_PACK*)taskMsgPtr;
    //检查数据源
    if(netRecvPackPtr->netPackSrc == NET_PACK_SRC_W5500)
    {
        SrvImplNetRecvMsgProcessW5500(netRecvPackPtr);
    }
    else if(netRecvPackPtr->netPackSrc == NET_PACK_SRC_LAN8700)
    {
        SrvImplNetRecvMsgProcessLAN8700(netRecvPackPtr);
    }
    //处理完成,数据释放
    if((netRecvPackPtr->recvDataLength != 0)&&(netRecvPackPtr->recvDatBufPtr != NULL))
    {
        
        UserMemFree(MEM_REGION_SRV,netRecvPackPtr->recvDatBufPtr);
        netRecvPackPtr->recvDatBufPtr = NULL;
    }
    UserMemFree(MEM_REGION_SRV,netRecvPackPtr);
}

//任务状态机初始化
void SrvImplNetRecvFSM_Init(void)
{
    
}

//任务状态机周期运行
void SrvImplNetRecvFSM_RunPeriod(void)
{
    
}

