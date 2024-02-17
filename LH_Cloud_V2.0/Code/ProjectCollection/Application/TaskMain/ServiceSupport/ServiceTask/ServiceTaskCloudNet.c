/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-09-07 20:15:29
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-08 13:36:31
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"
#include "ServiceImplInc.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferServiceTaskCloudNet[STK_SIZE_SERVICE_TASK_CLOUD_NET]@".ccram";
#pragma pack()
//任务控制块
OS_TCB tcbServiceTaskCloudNet@".ccram";

//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncCloudNet_CanMsg_Proc(CAN1_CMD_RECV* can1CommandPtr);

//任务函数
void ServiceTaskFuncCloudNet(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //延时等待接收函数初始化成功
    CoreDelayMs(100);
    while(1)
    {
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)0,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(SYSTEM_CMD_SRC_CAN == taskCommandPtr->cmdSrc)
            {
                //从CAN1传过来的指令
                CAN1_CMD_RECV* can1RecvDatPtr = (CAN1_CMD_RECV*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncCloudNet_CanMsg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(SRAM_CCM,can1RecvDatPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            taskCommandPtr->cmdDataPtr = NULL;
            UserMemFree(SRAM_CCM,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}

/*************************************************1. 来自CAN1的消息******************************************/
//设备连接网络
void ServiceTaskFuncCloudNet_Can1Msg_Proc_Connect(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//设备断开网络连接
void ServiceTaskFuncCloudNet_Can1Msg_Proc_Disconnect(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//设备发送一包数据
void ServiceTaskFuncCloudNet_Can1Msg_Proc_SendWithoutConnect(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//设备读取一次数据
void ServiceTaskFuncCloudNet_Can1Msg_Proc_ReadData(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//设备集成发送并接收数据
void ServiceTaskFuncCloudNet_Can1Msg_Proc_SendRecvWithConnect(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT CloudNetCan1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_CLOUD_NET_CONNECT                 ,ServiceTaskFuncCloudNet_Can1Msg_Proc_Connect               ,"Connect"              },//设备连接网络
    {CAN1_CMD_CLOUD_NET_DISCONNECT              ,ServiceTaskFuncCloudNet_Can1Msg_Proc_Disconnect            ,"Disconnect"           },//设备断开网络连接
    {CAN1_CMD_CLOUD_NET_SEND_WITHOUT_CONNECT    ,ServiceTaskFuncCloudNet_Can1Msg_Proc_SendWithoutConnect    ,"SendWithoutConnect"   },//设备发送一包数据
    {CAN1_CMD_CLOUD_NET_READ_DATA               ,ServiceTaskFuncCloudNet_Can1Msg_Proc_ReadData              ,"ReadData"             },//设备读取一次数据
    {CAN1_CMD_CLOUD_NET_SEND_RECV_WITH_CONNECT  ,ServiceTaskFuncCloudNet_Can1Msg_Proc_SendRecvWithConnect   ,"SendRecvWithConnect"  },//设备集成发送并接收数据
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_CLOUD_NET_CMD_COUNT        (sizeof(CloudNetCan1CommandProcFuncPtrArray)/sizeof(CloudNetCan1CommandProcFuncPtrArray[0]))

//1. 来自CAN1的消息
static void ServiceTaskFuncCloudNet_CanMsg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_CLOUD_NET_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == CloudNetCan1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(CloudNetCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                CloudNetCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,CloudNetCan1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//设备连接网络
void ServiceTaskFuncCloudNet_Can1Msg_Proc_Connect(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数
    uint8_t ip1,ip2,ip3,ip4;
    uint16_t remotePort;
    ip1 = can1CommandPtr->recvDataBuffer[0];
    ip2 = can1CommandPtr->recvDataBuffer[1];
    ip3 = can1CommandPtr->recvDataBuffer[2];
    ip4 = can1CommandPtr->recvDataBuffer[3];
    remotePort = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,4);
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudNet:  ",detailDebugMsgPtr,ip1,ip2,ip3,ip4,remotePort);
    //执行具体的操作
    errorCode = ServiceImplCloudNet_Connect(ip1,ip2,ip3,ip4,remotePort);
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_NET_CONNECT;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    //返回执行结果
    Can1SendCommandResult(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudNet:  ",errorCode);
}

//设备断开网络连接
void ServiceTaskFuncCloudNet_Can1Msg_Proc_Disconnect(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudNet:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudNet_Disconnect();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_NET_DISCONNECT;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    //返回执行结果
    Can1SendCommandResult(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudNet:  ",errorCode);
}

//设备发送一包数据
void ServiceTaskFuncCloudNet_Can1Msg_Proc_SendWithoutConnect(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数
    uint16_t sendLength = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,0);
    uint8_t* sendBufferPtr = can1CommandPtr->recvDataBuffer+2;
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudNet:  ",detailDebugMsgPtr,sendLength,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudNet_SendWithoutConnect(sendLength,sendBufferPtr);
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_NET_SEND_WITHOUT_CONNECT;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    //返回执行结果
    Can1SendCommandResult(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudNet:  ",errorCode);
}

//设备读取一次数据
void ServiceTaskFuncCloudNet_Can1Msg_Proc_ReadData(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数
    uint16_t exceptRecvDataLength = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,0);
    uint16_t exceptRecvDataTimeOutMs = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,2);
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudNet:  ",detailDebugMsgPtr,exceptRecvDataLength,exceptRecvDataTimeOutMs,0,0,0);
    //执行具体的操作
    uint8_t* recvDataBufferPtr = NULL;
    uint16_t realRecvDataLength = 0;
    errorCode = ServiceImplCloudNet_ReadData(exceptRecvDataLength,exceptRecvDataTimeOutMs,&recvDataBufferPtr,&realRecvDataLength);
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_NET_READ_DATA;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,realRecvDataLength,recvDataBufferPtr);
    }
    //如果不为空,则释放内存
    if(recvDataBufferPtr != NULL)
    {
        UserMemFree(SRAM_IS62,recvDataBufferPtr);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudNet:  ",errorCode);
}

//设备集成发送并接收数据
void ServiceTaskFuncCloudNet_Can1Msg_Proc_SendRecvWithConnect(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数
    uint8_t ip1,ip2,ip3,ip4;
    uint16_t remotePort;
    ip1 = can1CommandPtr->recvDataBuffer[0];
    ip2 = can1CommandPtr->recvDataBuffer[1];
    ip3 = can1CommandPtr->recvDataBuffer[2];
    ip4 = can1CommandPtr->recvDataBuffer[3];
    remotePort = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,4);
    uint16_t exceptRecvDataLength = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,6);
    uint16_t exceptRecvDataTimeOutMs = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,8);
    uint16_t sendLength = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,10);
    uint8_t* sendBufferPtr = can1CommandPtr->recvDataBuffer+12;
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudNet:  ",detailDebugMsgPtr,ip1,ip2,ip3,ip4,remotePort);
    SystemDebugShowMsgStart("TaskFuncCloudNet:  ",detailDebugMsgPtr,exceptRecvDataLength,exceptRecvDataTimeOutMs,sendLength,0,0);
    //执行具体的操作
    uint8_t* recvDataBufferPtr = NULL;
    uint16_t realRecvDataLength = 0;
    errorCode = ServiceImplCloudNet_SendRecvWithConnect(ip1,ip2,ip3,ip4,remotePort,exceptRecvDataLength,exceptRecvDataTimeOutMs,sendLength,sendBufferPtr,&recvDataBufferPtr,&realRecvDataLength);
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_NET_SEND_RECV_WITH_CONNECT;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,realRecvDataLength,recvDataBufferPtr);
        SystemPrintf("Recv DataLength : %d,DataBuffer: \r\n",realRecvDataLength);
        for(int i = 0; i < realRecvDataLength; i++)
        {
            SystemPrintf(" 0X%02X",recvDataBufferPtr[i]);
        }
        SystemPrintf("\r\n");
    }
    //如果不为空,则释放内存
    if(recvDataBufferPtr != NULL)
    {
        UserMemFree(SRAM_IS62,recvDataBufferPtr);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudNet:  ",errorCode);
}

