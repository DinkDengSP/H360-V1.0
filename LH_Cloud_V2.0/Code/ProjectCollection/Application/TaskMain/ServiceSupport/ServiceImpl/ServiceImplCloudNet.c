/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-09-08 11:05:13
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-23 18:32:15
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplBase.h"
#include "ServiceImplCloudNet.h"
#include "SystemUtilInc.h"
#include "BoardInc.h"

//设备连接网络
LH_ERR ServiceImplCloudNet_Connect(uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = BoardEC20_SocketConnect(APP_DEFAULT_SOCKET_NUM,ip1,ip2,ip3,ip4,remotePort);
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//设备断开网络连接
LH_ERR ServiceImplCloudNet_Disconnect(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = BoardEC20_SocketDisconnect(APP_DEFAULT_SOCKET_NUM);
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//设备发送一包数据
LH_ERR ServiceImplCloudNet_SendWithoutConnect(uint16_t sendLength,uint8_t* sendBufferPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = BoardEC20_SocketSendMsgWithoutConnect(APP_DEFAULT_SOCKET_NUM,sendBufferPtr,sendLength);
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//设备读取一次数据
LH_ERR ServiceImplCloudNet_ReadData(uint16_t exceptLength,uint16_t timeOutMs,uint8_t** recvBuffer,uint16_t* realReadLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR err;
    OS_TICK startTick = OSTimeGet(&err);
    OS_TICK currentTick = OSTimeGet(&err);
    //参数为0,立即返回
    if((exceptLength == 0)||(timeOutMs == 0))
    {
        *recvBuffer = NULL;
        *realReadLength = 0;
        return LH_ERR_NONE;
    }
    if(exceptLength > EC20_RECV_DATA_BUFFER_MAX)//接受长度不能超过最大长度限制
    {
        *recvBuffer = NULL;
        *realReadLength = 0;
        SystemDebugShowMsgError(LH_ERR_CLOUD_EC20_NET_READ_LENGTH_MAX);
        return LH_ERR_CLOUD_EC20_NET_READ_LENGTH_MAX;
    }
    uint16_t waitReadBufferLength = 0;
    //申请信号量
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //循环检测接收到的数据长度
    do
    {
        //检查接收到的数据长度
        errorCode = BoardEC20_SocketGetLengthWaitRead(APP_DEFAULT_SOCKET_NUM,&waitReadBufferLength);
        if(errorCode != LH_ERR_NONE)
        {
            break;
        }
        if(waitReadBufferLength >= exceptLength)
        {
            //收到了指定长度数据
            break;
        }
        //更新时间
        currentTick = OSTimeGet(&err);
        if(currentTick - startTick > timeOutMs)
        {
            //超时,不再检测
            break;
        }
    }while(1);
    //检测结果
    if(errorCode != LH_ERR_NONE)
    {
        *recvBuffer = NULL;
        *realReadLength = 0;
        SystemDebugShowMsgError(errorCode);
        //释放信号量
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //超时没有数据收到
    if(waitReadBufferLength == 0)
    {
        *recvBuffer = NULL;
        *realReadLength = 0;
        //释放信号量
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //查看要读取的数据长度,如果接收到的数据长度长于预期,则读取预期,否则,全部读取
    if(waitReadBufferLength < exceptLength)exceptLength = waitReadBufferLength;
    //申请内存准备读取数据
    *recvBuffer = UserMemMalloc(SRAM_IS62,exceptLength);
    if(*recvBuffer == NULL)
    {
        //申请内存失败
        *recvBuffer = NULL;
        *realReadLength = 0;
        SystemDebugShowMsgError(LH_ERR_CLOUD_EC20_NET_READ_MALLOC);
        //释放信号量
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return LH_ERR_CLOUD_EC20_NET_READ_MALLOC;
    }
    //读取数据
    errorCode = BoardEC20_SocketReadBufferByLength(APP_DEFAULT_SOCKET_NUM,*recvBuffer,exceptLength,realReadLength);
    if(errorCode != LH_ERR_NONE)
    {
        //释放掉申请的内存
        UserMemFree(SRAM_IS62,*recvBuffer);
        *recvBuffer = NULL;
        *realReadLength = 0;
        SystemDebugShowMsgError(errorCode);
        //释放信号量
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //期望接收到的数据和实际收到的数据量不符合
    if(exceptLength != (*realReadLength))
    {
        SystemPrintf("ExceptRecvLength: %d,RealReadData: %d\r\n",exceptLength,*realReadLength);
    }
    //释放信号量
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return errorCode;
}

//设备集成发送并接收数据
LH_ERR ServiceImplCloudNet_SendRecvWithConnect(uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort,
                                            uint16_t exceptLength,uint16_t timeOutMs,uint16_t sendLength,uint8_t* sendBufferPtr,
                                            uint8_t** recvBuffer,uint16_t* realReadLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //第一步连接IP
    errorCode = BoardEC20_SocketConnect(APP_DEFAULT_SOCKET_NUM,ip1,ip2,ip3,ip4,remotePort);
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
        //释放信号量
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //第二步清除缓冲区
    errorCode = BoardEC20_SocketClearBuf(APP_DEFAULT_SOCKET_NUM);
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
        //释放信号量
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //第三步发送数据
    errorCode = BoardEC20_SocketSendMsgWithoutConnect(APP_DEFAULT_SOCKET_NUM,sendBufferPtr,sendLength);
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
        //释放信号量
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //第四步等待返回
    errorCode = ServiceImplCloudNet_ReadData(exceptLength,timeOutMs,recvBuffer,realReadLength);
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
        LH_ERR errorCodeSub = LH_ERR_NONE;
        //断开连接
        errorCodeSub = BoardEC20_SocketDisconnect(APP_DEFAULT_SOCKET_NUM);
        //打印错误代码
        if(errorCodeSub != LH_ERR_NONE)
        {
            SystemDebugShowMsgError(errorCodeSub);
            //释放信号量
            if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
            return errorCodeSub;
        }
        //释放信号量
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //读取速度和关闭Sockt之间加一个延时,先等待服务器关闭
    CoreDelayMs(100);
    //检查是否已经关闭服务器
    uint8_t disconnectFlag = 0;
    EC20_SocketCheckIsClose(&disconnectFlag);
    if(disconnectFlag == 1)
    {
        //已经断开了连接,直接返回
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //第五步断开连接
    errorCode = BoardEC20_SocketDisconnect(APP_DEFAULT_SOCKET_NUM);
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
        //释放信号量
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //释放信号量
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return errorCode;
}










