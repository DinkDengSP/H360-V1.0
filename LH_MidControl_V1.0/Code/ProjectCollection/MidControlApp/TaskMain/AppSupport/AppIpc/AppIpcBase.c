/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 21:41:54
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-18 18:18:53
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcBase.h"

//IPC通讯，ACK事件标志组
TX_EVENT_FLAGS_GROUP ipcDataEventGroupAck;
//IPC通讯，Complete事件标志组
TX_EVENT_FLAGS_GROUP ipcDataEventGroupComplete;


//初始化APP IPC的基本数据结构
void IPC_AppBaseDataInit(void)
{
    tx_event_flags_create(&ipcDataEventGroupAck,"ipcDataEventGroupAck");
    tx_event_flags_create(&ipcDataEventGroupComplete,"ipcDataEventGroupComplete");
}

//IPC调用基础结构初始化
void IPC_DataInit(IPC_DATA_BASE* ipcDataPtr,uint8_t* mutexNamePtr,TX_QUEUE* targetQueuePtr,TX_EVENT_FLAGS_GROUP* ackFlagsGroupPtr,TX_EVENT_FLAGS_GROUP* completeFlagsGroupPtr,
                    uint8_t eventFlagLeftShiftOffset)
{
    //创建互斥信号量
    tx_mutex_create(&(ipcDataPtr->dataMutex),(CHAR*)mutexNamePtr,TX_INHERIT);
    //设置信号量组
    ipcDataPtr->eventGroupFlagsAckPtr = ackFlagsGroupPtr;
    ipcDataPtr->eventGroupFlagsCompletePtr = completeFlagsGroupPtr;
    ipcDataPtr->eventLeftShiftOffset = eventFlagLeftShiftOffset;
    //IPC目标指向
    ipcDataPtr->targetQueuePtr = targetQueuePtr;
    ipcDataPtr->errorParam = LH_ERR_NONE;
    ipcDataPtr->errorCode = TCP_ERR_NONE;
    ipcDataPtr->errorLevel = TCP_ERR_LEVEL_NORMAL;
    //清除对应标志组
    tx_event_flags_set(ipcDataPtr->eventGroupFlagsAckPtr,(~(1UL<<(ipcDataPtr->eventLeftShiftOffset))),TX_AND);
    tx_event_flags_set(ipcDataPtr->eventGroupFlagsCompletePtr,(~(1UL<<(ipcDataPtr->eventLeftShiftOffset))),TX_AND);
}

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_BaseGetLastCommandStateAndResult(IPC_DATA_BASE* ipcDataPtr,LH_ERR* errorParam,TCP_ERR_LEVEL* errorLevel,TCP_ERR* errorCode)
{
    uint32_t txResult = TX_SUCCESS;
    ULONG actualFlag = 0;
    //申请信号量
    txResult = tx_mutex_get(&(ipcDataPtr->dataMutex),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        //申请不到信号量,IPC正在被其他线程调用
        return STATE_IPC_OTHER_CALL;
    }
    //申请到了,查看ACK事件标志组
    txResult = tx_event_flags_get(ipcDataPtr->eventGroupFlagsAckPtr,(1UL<<(ipcDataPtr->eventLeftShiftOffset)),TX_AND,&actualFlag,TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        //释放互斥量
        tx_mutex_put(&(ipcDataPtr->dataMutex));
        //还在等待线程响应
        return STATE_IPC_WAIT_ACK;
    }
    //ACK已经得到了,现在检查complete事件标志组
    txResult = tx_event_flags_get(ipcDataPtr->eventGroupFlagsCompletePtr,(1UL<<(ipcDataPtr->eventLeftShiftOffset)),TX_AND,&actualFlag,TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        //释放互斥量
        tx_mutex_put(&(ipcDataPtr->dataMutex));
        //还在等待完成返回
        return STATE_IPC_WAIT_RETURN;
    }
    //指令已经完成
    *errorCode =  ipcDataPtr->errorCode;
    *errorLevel = ipcDataPtr->errorLevel;
    *errorParam = ipcDataPtr->errorParam;
    //清除事件标志组
    tx_event_flags_set(ipcDataPtr->eventGroupFlagsAckPtr,(~(1UL<<(ipcDataPtr->eventLeftShiftOffset))),TX_AND);
    tx_event_flags_set(ipcDataPtr->eventGroupFlagsCompletePtr,(~(1UL<<(ipcDataPtr->eventLeftShiftOffset))),TX_AND);
    //释放互斥信号量,这个地方需要将申请的互斥量释放完全
    tx_mutex_put(&(ipcDataPtr->dataMutex));
    while(ipcDataPtr->dataMutex.tx_mutex_ownership_count != 0)
    {
        tx_mutex_put(&(ipcDataPtr->dataMutex));
    }
    //释放完成,返回结果
    return STATE_IPC_CMD_COMPLETE;
}

//等待执行完成,阻塞
LH_ERR IPC_BaseWaitLastReturn(IPC_DATA_BASE* ipcDataPtr,TCP_ERR_LEVEL* errorLevel,TCP_ERR* errorCode)
{
    uint32_t txResult = TX_SUCCESS;
    ULONG actualFlag = 0;
    //申请信号量
    txResult = tx_mutex_get(&(ipcDataPtr->dataMutex),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        //申请不到信号量,IPC正在被其他线程调用
        return LH_ERR_BOARD_MAIN_COMM_IPC_OTHER_CALL;
    }
    //首先等待ACK
    txResult = tx_event_flags_get(ipcDataPtr->eventGroupFlagsAckPtr,(1UL<<(ipcDataPtr->eventLeftShiftOffset)),TX_AND,&actualFlag,TX_WAIT_FOREVER);
    if(txResult != TX_SUCCESS)
    {
        tx_mutex_put(&(ipcDataPtr->dataMutex));
        while(ipcDataPtr->dataMutex.tx_mutex_ownership_count != 0)
        {
            tx_mutex_put(&(ipcDataPtr->dataMutex));
        }
        return LH_ERR_BOARD_MAIN_COMM_IPC_EVENT_FLAGS_UNKNOW;
    }
    //然后等待Complete
    txResult = tx_event_flags_get(ipcDataPtr->eventGroupFlagsCompletePtr,(1UL<<(ipcDataPtr->eventLeftShiftOffset)),TX_AND,&actualFlag,TX_WAIT_FOREVER);
    if(txResult != TX_SUCCESS)
    {
        tx_mutex_put(&(ipcDataPtr->dataMutex));
        while(ipcDataPtr->dataMutex.tx_mutex_ownership_count != 0)
        {
            tx_mutex_put(&(ipcDataPtr->dataMutex));
        }
        return LH_ERR_BOARD_MAIN_COMM_IPC_EVENT_FLAGS_UNKNOW;
    }
    //等待完成，看结果
    //指令已经完成
    *errorCode =  ipcDataPtr->errorCode;
    *errorLevel = ipcDataPtr->errorLevel;
    //清除事件标志组
    tx_event_flags_set(ipcDataPtr->eventGroupFlagsAckPtr,(~(1UL<<(ipcDataPtr->eventLeftShiftOffset))),TX_AND);
    tx_event_flags_set(ipcDataPtr->eventGroupFlagsCompletePtr,(~(1UL<<(ipcDataPtr->eventLeftShiftOffset))),TX_AND);
    //释放互斥信号量,这个地方需要将申请的互斥量释放完全
    tx_mutex_put(&(ipcDataPtr->dataMutex));
    while(ipcDataPtr->dataMutex.tx_mutex_ownership_count != 0)
    {
        tx_mutex_put(&(ipcDataPtr->dataMutex));
    }
    //释放完成,返回结果
    return ipcDataPtr->errorParam;
}

//动作API发送任务给动作API,等待ACK不等待返回
LH_ERR IPC_BaseSendCommandWhileAck(IPC_DATA_BASE* ipcDataPtr,uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5,uint8_t* paramBuffer,
                                        uint16_t paramBufferLength)
{
    //申请互斥量
    uint32_t txResult = TX_SUCCESS;
    ULONG actualFlag = 0;
    //系统消息指针
    SYSTEM_MSG* systemMsgPtr = NULL;
    //申请信号量
    txResult = tx_mutex_get(&(ipcDataPtr->dataMutex),TX_WAIT_FOREVER);
    if(txResult != TX_SUCCESS)
    {
        //申请不到信号量,IPC正在被其他线程调用
        return LH_ERR_BOARD_MAIN_COMM_IPC_OTHER_CALL;
    }
    //申请到了
    //清除事件标志组
    tx_event_flags_set(ipcDataPtr->eventGroupFlagsAckPtr,(~(1UL<<(ipcDataPtr->eventLeftShiftOffset))),TX_AND);
    tx_event_flags_set(ipcDataPtr->eventGroupFlagsCompletePtr,(~(1UL<<(ipcDataPtr->eventLeftShiftOffset))),TX_AND);
    ipcDataPtr->errorParam = LH_ERR_NONE;
    ipcDataPtr->errorLevel = TCP_ERR_LEVEL_NORMAL;
    ipcDataPtr->errorCode = TCP_ERR_NONE;
    //申请系统消息内存
    systemMsgPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,(sizeof(SYSTEM_MSG)/sizeof(uint8_t)));
    //申请到系统指令包,设置初始值
    systemMsgPtr->msgSrc = SYSTEM_CMD_SRC_SELF;
    systemMsgPtr->msgDataPtr = NULL;
    //申请自身指令包的结构数据
    systemMsgPtr->msgDataPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,(sizeof(SYSTEM_MSG_IPC)/sizeof(uint8_t)));
    //辅助指针
    SYSTEM_MSG_IPC* utilSelfCmdPtr = (SYSTEM_MSG_IPC*)(systemMsgPtr->msgDataPtr);
    //消息序号
    utilSelfCmdPtr->cmdIndex = (uint16_t)cmdIndex;
    //参数设置
    utilSelfCmdPtr->cmdParam1 = param1;
    utilSelfCmdPtr->cmdParam2 = param2;
    utilSelfCmdPtr->cmdParam3 = param3;
    utilSelfCmdPtr->cmdParam4 = param4;
    utilSelfCmdPtr->cmdParam5 = param5;
    //设置不同信道特有的数据指针
    utilSelfCmdPtr->ipcBasePtr = (void*)(ipcDataPtr);
    //附带参数长度和参数缓冲区指针
    utilSelfCmdPtr->paramBuffer = NULL;
    utilSelfCmdPtr->paramLength = 0;
    //调用带缓冲区数据,就要申请缓冲区数据
    if(paramBufferLength != 0)
    {
        utilSelfCmdPtr->paramBuffer = UserMemMallocWhileSuccess(MEM_SDRAM1,paramBufferLength);
        //数据拷贝
        UserMemCopy(utilSelfCmdPtr->paramBuffer,paramBuffer,paramBufferLength);
        utilSelfCmdPtr->paramLength = paramBufferLength;
    }
    //线程数据发送
    txResult = tx_queue_send(ipcDataPtr->targetQueuePtr,(void*)(&systemMsgPtr),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        //发送失败,直接释放本次申请的内存
        if(paramBufferLength != 0)
        {
            UserMemFree(MEM_SDRAM1, (void *)utilSelfCmdPtr->paramBuffer);
        }
        UserMemFree(MEM_SDRAM1, (void *)systemMsgPtr->msgDataPtr);
        UserMemFree(MEM_SDRAM1, (void *)systemMsgPtr);
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        tx_mutex_put(&(ipcDataPtr->dataMutex));
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //成功发送，等待指令指令相应
    txResult = tx_event_flags_get(ipcDataPtr->eventGroupFlagsAckPtr,(1UL<<(ipcDataPtr->eventLeftShiftOffset)),TX_AND,&actualFlag,TX_WAIT_FOREVER);
    if(txResult != TX_SUCCESS)
    {
        //等待ACK失败
        if(paramBufferLength != 0)
        {
            UserMemFree(MEM_SDRAM1, (void *)utilSelfCmdPtr->paramBuffer);
        }
        UserMemFree(MEM_SDRAM1, (void *)systemMsgPtr->msgDataPtr);
        UserMemFree(MEM_SDRAM1, (void *)systemMsgPtr);
        //释放信号量
        tx_mutex_put(&(ipcDataPtr->dataMutex));
        while(ipcDataPtr->dataMutex.tx_mutex_ownership_count != 0)
        {
            tx_mutex_put(&(ipcDataPtr->dataMutex));
        }
        return LH_ERR_BOARD_MAIN_COMM_IPC_EVENT_FLAGS_UNKNOW;
    }
    return LH_ERR_NONE;
}

//动作API发送任务给动作API,等待返回
LH_ERR IPC_BaseSendCommandWhileReturn(IPC_DATA_BASE* ipcDataPtr,uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5,uint8_t* paramBuffer,
                                        uint16_t paramBufferLength,TCP_ERR_LEVEL* errorLevel,TCP_ERR* errorCode)
{
    LH_ERR errorParam = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
    errorParam = IPC_BaseSendCommandWhileAck(ipcDataPtr,cmdIndex,param1,param2,param3,param4,param5,paramBuffer,paramBufferLength);
    if(errorParam != LH_ERR_NONE)
    {
        return errorParam;
    }
    //等待指令完成
    errorParam = IPC_BaseWaitLastReturn(ipcDataPtr,errorLevel,errorCode);
    if(errorParam != LH_ERR_NONE)
    {
        return errorParam;
    }
    //返回结果代码
    return errorParam;
}
