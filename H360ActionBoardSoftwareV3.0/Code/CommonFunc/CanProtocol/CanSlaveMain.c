#include "CanSlaveMain.h"
#include "SystemCmd.h"

//原始CAN数据长度缓存
static uint8_t canSlaveRecvDataLength;
//原始CAN数据缓存
static uint8_t canSlaveRecvDataBuffer[8] = {0};

//互斥信号量,因为多个信道可能同时使用发送API,必须要加以限制
static OS_MUTEX mutexCanSlaveSendData;

//通道缓存,有多少的分发配置,就有多少缓存,一个通道对应一个任务分发
static CAN_SLAVE_CHANNEL_CACHE canSlaveCacheArray[CAN_PROTOCOL_BOARD_CHANNEL_MAX];

//CAN总线接收缓存初始化
void CanSlaveInit(CAN_SLAVE_DISPATCH_CONFIG* dispatchConfigPtr)
{
    OS_ERR err;
    //对通道缓存进行初始化
    uint8_t index = 0;
    //将缓存区初始化
    for(index = 0; index < CAN_PROTOCOL_BOARD_CHANNEL_MAX; index++)
    {
        UserMemSet((void*)(&canSlaveCacheArray[index]),0,CAN_SLAVE_CHANNEL_CHAHE_SIZE);
        //内部分发配置设置
        canSlaveCacheArray[index].dispatchTaskPtrTCB = NULL;
        //设置一下缓存的通道号
        canSlaveCacheArray[index].selfChannelNumber = index;
    }
    //初始化转发配置
    for(index = 0; index < CAN_PROTOCOL_BOARD_CHANNEL_MAX; index++)
    {
        canSlaveCacheArray[dispatchConfigPtr[index].channelNumber].dispatchTaskPtrTCB = dispatchConfigPtr[index].dispatchTaskPtrTCB;
    }
    //申请互斥信号量来操作
    OSMutexCreate((OS_MUTEX*	)&mutexCanSlaveSendData,
				  (CPU_CHAR*	)"mutexCanSlaveSendData",
                  (OS_ERR*		)&err);
}

//接收数据转发到指定信道处理
void CanSlaveDispatchData2SpecialTask(void)
{
    uint8_t channelIndex = 0;
    OS_ERR err;
    //轮询所有通道.当通道状态是OK的时候,转发到处理信道,并设置通道忙
    for(channelIndex = 0; channelIndex < CAN_PROTOCOL_BOARD_CHANNEL_MAX; channelIndex++)
    {
        if(canSlaveCacheArray[channelIndex].currentState == CAN_SLAVE_CHANNEL_OK)
        {
            //如果TCB指针没配置,那么直接报错
            if(canSlaveCacheArray[channelIndex].dispatchTaskPtrTCB == NULL)
            {
                //发一个指令不支持
                CanSlaveSendAckWithRecvCache(&canSlaveCacheArray[channelIndex],CAN_BASE_ACK_CHANNEL_UNSUPPORT);
                //转发失败
                canSlaveCacheArray[channelIndex].currentState = CAN_SLAVE_CHANNEL_IDLE;
                break;
            }
            //内部转发信号量
            TASK_CMD* taskCommandPtr = NULL;

            //转发,首先申请内存空间
            do
            {
                taskCommandPtr = UserMemMalloc(SRAM_CCM,sizeof(TASK_CMD)/sizeof(uint8_t));
                if(taskCommandPtr == NULL)
                {
                    CoreDelayMinTick();
                }
            }while(taskCommandPtr == NULL);

            //生成一个来自CAN的系统消息
            taskCommandPtr->cmdSrc = TASK_CMD_SRC_CAN;
            taskCommandPtr->cmdData = (void*)(&(canSlaveCacheArray[channelIndex]));

            //向协议处理线程发送消息,注意,任务需要释放这一段消息的内存
            OSTaskQPost((OS_TCB *)(canSlaveCacheArray[channelIndex].dispatchTaskPtrTCB), 
                        (void *)(taskCommandPtr),
                        (OS_MSG_SIZE)sizeof(TASK_CMD *),
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
            if(err == OS_ERR_NONE)
            {
                //转发成功
                canSlaveCacheArray[channelIndex].currentState = CAN_SLAVE_CHANNEL_BUSY;
                //ACK交给协议信道去处理
            }
            else
            {
                //发一个总线忙
                CanSlaveSendAckWithRecvCache(&canSlaveCacheArray[channelIndex],CAN_BASE_ACK_CHANNEL_BUSY);
                //转发失败
                canSlaveCacheArray[channelIndex].currentState = CAN_SLAVE_CHANNEL_IDLE;
                //内存释放
                UserMemFree(SRAM_CCM,taskCommandPtr);
            }
        }
    }
}


//发送ACK
void CanSlaveSendAckWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,CAN_BASE_ACK ackCode)
{
    OS_ERR err;
    //首先申请内存
    CanTxMsg* txMsg = NULL;
    //循环申请
    do
    {
        txMsg = UserMemMalloc(SRAM_IN,(sizeof(CanTxMsg)/sizeof(uint8_t)));
        if(txMsg == NULL)
        {
            CoreDelayMinTick();
        }
    }while(txMsg == NULL);

    //创建ID
    txMsg->ExtId = CanSlaveCreateAckPackID_ByParam(cachePtr->senderBoardAddr,BoardKeyID_GetCurrent(),cachePtr->selfChannelNumber,cachePtr->lastFrameCode);
    txMsg->DLC = 4;
    txMsg->IDE = CAN_Id_Extended;
    txMsg->RTR = CAN_RTR_Data;
    txMsg->Data[3] = (uint8_t)ackCode;
    ackCode >>= 8;
    txMsg->Data[2] = (uint8_t)ackCode;
    ackCode >>= 8;
    txMsg->Data[1] = (uint8_t)ackCode;
    ackCode >>= 8;
    txMsg->Data[0] = (uint8_t)ackCode;
    
    //先申请互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
	OSMutexPend (&mutexCanSlaveSendData,0,OS_OPT_PEND_BLOCKING,0,&err);
    //发送数据
    MCU_CAN1_TransMessage(txMsg);
    //释放协议信号量
    OSMutexPost(&mutexCanSlaveSendData,OS_OPT_POST_NO_SCHED,&err);
    //释放内存
    UserMemFree(SRAM_IN, txMsg);
}

//发送结果代码
void CanSlaveSendResultWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,LH_ERR errCode)
{
    //首先申请内存
    OS_ERR err;
    //首先申请内存
    CanTxMsg* txMsg = NULL;
    //循环申请
    do
    {
        txMsg = UserMemMalloc(SRAM_IN,(sizeof(CanTxMsg)/sizeof(uint8_t)));
        if(txMsg == NULL)
        {
            CoreDelayMinTick();
        }
    }while(txMsg == NULL);

    //创建ID
    txMsg->ExtId = CanSlaveCreateResultPackID_ByParam(cachePtr->senderBoardAddr,BoardKeyID_GetCurrent(),cachePtr->selfChannelNumber,cachePtr->lastFrameCode);
    txMsg->DLC = 4;
    txMsg->IDE = CAN_Id_Extended;
    txMsg->RTR = CAN_RTR_Data;
    //创建数据包
    txMsg->Data[3] = (uint8_t)errCode;
    errCode >>= 8;
    txMsg->Data[2] = (uint8_t)errCode;
    errCode >>= 8;
    txMsg->Data[1] = (uint8_t)errCode;
    errCode >>= 8;
    txMsg->Data[0] = (uint8_t)errCode;

    //先申请互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
	OSMutexPend (&mutexCanSlaveSendData,0,OS_OPT_PEND_BLOCKING,0,&err);
    //发送数据
    MCU_CAN1_TransMessage(txMsg);
    //释放协议信号量
    OSMutexPost(&mutexCanSlaveSendData,OS_OPT_POST_NO_SCHED,&err);
    //释放内存
    UserMemFree(SRAM_IN, txMsg);
}

//发送数据代码,发送长包起始包
static void CanSlaveSendDataLongPackStartWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,uint16_t bufferLength,CanTxMsg* txMsg)
{
    //创建ID
    txMsg->ExtId = CanSlaveCreateLongDataPackStartID_ByParam(cachePtr->senderBoardAddr,BoardKeyID_GetCurrent(),cachePtr->selfChannelNumber,cachePtr->lastFrameCode);

    txMsg->DLC = 3;
    txMsg->IDE = CAN_Id_Extended;
    txMsg->RTR = CAN_RTR_Data;

    //计算包总数
    uint8_t packCountAll = 0;
    //计算需要的包的总数,dataLength 指定的数据之外还带有数据类型标识,所以加两个字节
    uint16_t allDataLength = 2+bufferLength;
    if(allDataLength%7 == 0)//七字节整数倍可以传输完成
    {
        //整数包
        packCountAll = (allDataLength/7);
    }
    else
    {
        //非整数包,在数据基础上还要再加一包把不足一包的数据发出去
        packCountAll = 1 + (allDataLength/7);
    }

    //数据字长
    txMsg->Data[1] = (uint8_t)allDataLength;
    txMsg->Data[0] = (uint8_t)(allDataLength >> 8);
    //包总数
    txMsg->Data[2] = (uint8_t)(packCountAll);
    //发送数据
    MCU_CAN1_TransMessage(txMsg);
}

//发送数据代码,发送长包过程包
static void CanSlaveSendDataLongPackProcedureWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,CAN_DATA_FLAG dataFlag,uint8_t* bufferPtr,uint16_t bufferLength,CanTxMsg* txMsg)
{
    //创建ID
    txMsg->ExtId = CanSlaveCreateLongDataPackProcedureID_ByParam(cachePtr->senderBoardAddr,BoardKeyID_GetCurrent(),cachePtr->selfChannelNumber,cachePtr->lastFrameCode);
    txMsg->IDE = CAN_Id_Extended;
    txMsg->RTR = CAN_RTR_Data;

    //需要发出的包总数
    uint8_t packCountNeedSend = 0;
    //需要发出的字节总数
    uint16_t byteCountNeedSend = 0;
    //当前已经发出的包总数
    uint8_t packCountCurrentSend = 0;
    //当前已经发出的字节总数
    uint16_t byteCountCurrentSend = 0;

    //计算要发出的字节和包数量
    byteCountNeedSend = bufferLength+2;
    if(byteCountNeedSend%7 == 0)
    {
        //整数包
        packCountNeedSend = (byteCountNeedSend/7);
    }
    else
    {
        //非整数包,在数据基础上还要再加一包把不足一包的数据发出去
        packCountNeedSend = 1 + (byteCountNeedSend/7);
    }
    for(packCountCurrentSend = 0 ; packCountCurrentSend < packCountNeedSend; packCountCurrentSend++)
    {
        //包号
        txMsg->Data[0] = packCountCurrentSend;
        if(packCountCurrentSend == 0)
        {
            //第一包
            txMsg->Data[2] = (uint8_t)dataFlag;
            txMsg->Data[1] = (uint8_t)(dataFlag >> 8);
            for(uint8_t indexUtil = 0; indexUtil < 5; indexUtil++)
            {
                txMsg->Data[3+indexUtil] = bufferPtr[indexUtil];
            }
            //记录标号
            byteCountCurrentSend = 7;
            txMsg->DLC = 8;
            //发送数据
            MCU_CAN1_TransMessage(txMsg);
        }
        else
        {
            //其余数据包
            for(uint8_t indexUtil = 0; indexUtil < 7; indexUtil++)
            {
                txMsg->Data[1+indexUtil] = bufferPtr[byteCountCurrentSend-2];
                byteCountCurrentSend++;
                //数据全部填充完成
                if(byteCountCurrentSend > byteCountNeedSend)
                {
                    txMsg->DLC = indexUtil+1;
                    //发送数据
                    MCU_CAN1_TransMessage(txMsg);
                    return;
                }
            }
            txMsg->DLC = 8;
            //发送数据
            MCU_CAN1_TransMessage(txMsg);
        }
    }
}

//发送数据代码,发送长包结果包
static void CanSlaveSendDataLongPackEndWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,CanTxMsg* txMsg)
{
    //创建ID
    txMsg->ExtId = CanSlaveCreateLongDataPackEndID_ByParam(cachePtr->senderBoardAddr,BoardKeyID_GetCurrent(),cachePtr->selfChannelNumber,cachePtr->lastFrameCode);
    txMsg->DLC = 0;
    txMsg->IDE = CAN_Id_Extended;
    txMsg->RTR = CAN_RTR_Data;
    //发送数据
    MCU_CAN1_TransMessage(txMsg);
}

//发送数据代码,短包一次性发送
static void CanSlaveSendDataShortPackWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,CAN_DATA_FLAG dataFlag,uint8_t* bufferPtr,uint16_t bufferLength)
{
    //首先申请内存
    OS_ERR err;
    //首先申请内存
    CanTxMsg* txMsg = NULL;
    //循环申请
    do
    {
        txMsg = UserMemMalloc(SRAM_IN,(sizeof(CanTxMsg)/sizeof(uint8_t)));
        if(txMsg == NULL)
        {
            CoreDelayMinTick();
        }
    }while(txMsg == NULL);

    //创建ID
    txMsg->ExtId = CanSlaveCreateShortDataPackID_ByParam(cachePtr->senderBoardAddr,BoardKeyID_GetCurrent(),cachePtr->selfChannelNumber,cachePtr->lastFrameCode);
    txMsg->DLC = 2 + bufferLength;
    txMsg->IDE = CAN_Id_Extended;
    txMsg->RTR = CAN_RTR_Data;
    //数据标识
    txMsg->Data[1] = (uint8_t)dataFlag;
    dataFlag >>= 8;
    txMsg->Data[0] = (uint8_t)dataFlag;
    //序号
    uint8_t index = 0;
    for(index = 0; index < bufferLength;index++)
    {
        txMsg->Data[2+index] = bufferPtr[index];
    }

    //先申请互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
	OSMutexPend (&mutexCanSlaveSendData,0,OS_OPT_PEND_BLOCKING,0,&err);
    //发送数据
    MCU_CAN1_TransMessage(txMsg);
    //释放协议信号量
    OSMutexPost(&mutexCanSlaveSendData,OS_OPT_POST_NO_SCHED,&err);
    //释放内存
    UserMemFree(SRAM_IN, txMsg);
}

//发送数据长包代码
void CanSlaveSendDataLongPackWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,CAN_DATA_FLAG dataFlag,uint8_t* bufferPtr,uint16_t bufferLength)
{
    OS_ERR err;
    //首先申请内存
    CanTxMsg* txMsg = NULL;
    //循环申请
    do
    {
        txMsg = UserMemMalloc(SRAM_IN,(sizeof(CanTxMsg)/sizeof(uint8_t)));
        if(txMsg == NULL)
        {
            CoreDelayMinTick();
        }
    }while(txMsg == NULL);
    
    //先申请互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    OSMutexPend (&mutexCanSlaveSendData,0,OS_OPT_PEND_BLOCKING,0,&err);
    //发送起始数据包
    CanSlaveSendDataLongPackStartWithRecvCache(cachePtr,bufferLength,txMsg);
    //发送过程数据包
    CanSlaveSendDataLongPackProcedureWithRecvCache(cachePtr,dataFlag,bufferPtr,bufferLength,txMsg);
    //发送结果数据包
    CanSlaveSendDataLongPackEndWithRecvCache(cachePtr,txMsg);
    //释放协议信号量
    OSMutexPost(&mutexCanSlaveSendData,OS_OPT_POST_NO_SCHED,&err);
    //等待完成,释放内存
    UserMemFree(SRAM_IN, txMsg);
}

//发送数据代码
void CanSlaveSendDataPackWithRecvCache(CAN_SLAVE_CHANNEL_CACHE* cachePtr,CAN_DATA_FLAG dataFlag,uint8_t* bufferPtr,uint16_t bufferLength)
{
    if(bufferLength > 6)
    {
        //长包
        CanSlaveSendDataLongPackWithRecvCache(cachePtr,dataFlag,bufferPtr,bufferLength);
    }
    else
    {
        //短包
        CanSlaveSendDataShortPackWithRecvCache(cachePtr,dataFlag,bufferPtr,bufferLength);
    }
}

//复位相应的传输信道
static void CanSlaveResetChannel(uint8_t channelNumber)
{
    canSlaveCacheArray[channelNumber].exceptLength = 0;
    canSlaveCacheArray[channelNumber].exceptPackCount = 0;
    canSlaveCacheArray[channelNumber].currentLength = 0;
    canSlaveCacheArray[channelNumber].currentPackCount = 0;
    canSlaveCacheArray[channelNumber].currentState = CAN_SLAVE_CHANNEL_IDLE;
    canSlaveCacheArray[channelNumber].senderBoardAddr = 0;
    canSlaveCacheArray[channelNumber].ackCode = CAN_BASE_ACK_OK;
    canSlaveCacheArray[channelNumber].lastFrameCode = 0;
}

//接收过程的错误处理
static void CanSlaveErrorCheckProcess(uint8_t channelNumber)
{
    if(canSlaveCacheArray[channelNumber].currentState == CAN_SLAVE_CHANNEL_ERR)
    {
        //到这里,说明包接收报错了,将错误代码返回
        CanSlaveSendAckWithRecvCache(&canSlaveCacheArray[channelNumber],canSlaveCacheArray[channelNumber].ackCode);
        //然后复位接收缓存状态
        CanSlaveResetChannel(channelNumber);
    }
}

//当相应的信道处于长包等待数据或者等待结果包的情况下,收到新的数据包,立即清除以前的状态
static void CanSlaveProcWithNewStartPack(CAN_RECV_FAST_ID* fastCanIDPtr)
{
    //在接受过程中,如果接收到新的起始数据包,立即切换成初始化状态
    if((canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState == CAN_SLAVE_CHANNEL_WAIT_DATA)||
        (canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState == CAN_SLAVE_CHANNEL_WAIT_END))
    {
        if((fastCanIDPtr->packLenFlag == CAN_BASE_PACK_LEN_SHORT)&&(fastCanIDPtr->singleCode == CAN_BASE_SINGLE_CODE_SHORT))
        {
            CanSlaveResetChannel(fastCanIDPtr->channelNumber);
        }
        if((fastCanIDPtr->packLenFlag == CAN_BASE_PACK_LEN_LONG)&&(fastCanIDPtr->singleCode == CAN_BASE_SINGLE_CODE_LONG_START))
        {
            CanSlaveResetChannel(fastCanIDPtr->channelNumber);
        }
    }
}

static void CanSlaveStateMachineIdleProc(CAN_RECV_FAST_ID* fastCanIDPtr)
{
    //短包
    if((fastCanIDPtr->packLenFlag == CAN_BASE_PACK_LEN_SHORT)&&(fastCanIDPtr->singleCode == CAN_BASE_SINGLE_CODE_SHORT))
    {
        canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptLength = canSlaveRecvDataLength;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptPackCount = 1;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentLength = canSlaveRecvDataLength;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentPackCount = 1;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].lastFrameCode = fastCanIDPtr->frameCode;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].senderBoardAddr = fastCanIDPtr->senderBoardAddr;
        //接收完成
        canSlaveCacheArray[fastCanIDPtr->channelNumber].ackCode = CAN_BASE_ACK_OK;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState = CAN_SLAVE_CHANNEL_OK;
        //拷贝数据
        for(uint8_t i = 0; i < canSlaveRecvDataLength; i++)
        {
            canSlaveCacheArray[fastCanIDPtr->channelNumber].recvCacheArray[i] = canSlaveRecvDataBuffer[i];
        }
    }
    //长包起始包,且长度必须为3
    else if((fastCanIDPtr->packLenFlag == CAN_BASE_PACK_LEN_LONG)&&(fastCanIDPtr->singleCode == CAN_BASE_SINGLE_CODE_LONG_START)&&(canSlaveRecvDataLength == 3))
    {
        //预期长度
        canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptLength = canSlaveRecvDataBuffer[0];
        canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptLength <<= 8;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptLength += canSlaveRecvDataBuffer[1];
        //预期包数
        canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptPackCount = canSlaveRecvDataBuffer[2];
        //当前长度与包数
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentLength = 0;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentPackCount = 0;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].lastFrameCode = fastCanIDPtr->frameCode;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].senderBoardAddr = fastCanIDPtr->senderBoardAddr;
        //状态修改
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState = CAN_SLAVE_CHANNEL_WAIT_DATA;
    }
    else
    {
        //在等待起始数据包的情况下,没收到起始数据包,不处理不接收
    }
}

//等待数据包状态的处理
static void CanSlaveStateMachineWaitDataProc(CAN_RECV_FAST_ID* fastCanIDPtr)
{
    //如果接收到的数据的发送板和本次接收到的数据的发送板对不上,这一包数据不要
    //这一句主要是防止同时收到两块板子的指令,数据错乱
    if(canSlaveCacheArray[fastCanIDPtr->channelNumber].senderBoardAddr != fastCanIDPtr->senderBoardAddr)
    {
        //数据丢弃
        return;
    }
    //开始等待数据包
    if((fastCanIDPtr->packLenFlag == CAN_BASE_PACK_LEN_LONG)&&(fastCanIDPtr->singleCode == CAN_BASE_SINGLE_CODE_LONG_DATA)&&
        (canSlaveRecvDataBuffer[0] == canSlaveCacheArray[fastCanIDPtr->channelNumber].currentPackCount))
    {
        //长包 数据包 且当前接收包号与收到的包号一致,包号增加
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentPackCount++;
        //数据拷贝
        for(uint8_t i = 1; i < canSlaveRecvDataLength; i++)
        {
            canSlaveCacheArray[fastCanIDPtr->channelNumber].recvCacheArray[canSlaveCacheArray[fastCanIDPtr->channelNumber].currentLength] = canSlaveRecvDataBuffer[i];
            //数据增加
            canSlaveCacheArray[fastCanIDPtr->channelNumber].currentLength++;
        }
        //数据长度和数据包数均不满足要求
        if((canSlaveCacheArray[fastCanIDPtr->channelNumber].currentLength < canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptLength)&&
            (canSlaveCacheArray[fastCanIDPtr->channelNumber].currentPackCount < canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptPackCount))
        {
            //继续等待接收
            canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState = CAN_SLAVE_CHANNEL_WAIT_DATA;
            canSlaveCacheArray[fastCanIDPtr->channelNumber].ackCode = CAN_BASE_ACK_OK;
        }
        else if((canSlaveCacheArray[fastCanIDPtr->channelNumber].currentLength == canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptLength)&&
            (canSlaveCacheArray[fastCanIDPtr->channelNumber].currentPackCount == canSlaveCacheArray[fastCanIDPtr->channelNumber].exceptPackCount))
        {
            //数据长度和数据包数均满足要求,等待结果包
            canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState = CAN_SLAVE_CHANNEL_WAIT_END;
            canSlaveCacheArray[fastCanIDPtr->channelNumber].ackCode = CAN_BASE_ACK_OK;
        }
        else
        {
            //数据长度和数据包数不满足要求,出错
            canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState = CAN_SLAVE_CHANNEL_ERR;
            canSlaveCacheArray[fastCanIDPtr->channelNumber].ackCode = CAN_BASE_ACK_LONG_DATA;
        }
        
    }
    else
    {
        //长包数据包错误
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState = CAN_SLAVE_CHANNEL_ERR;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].ackCode = CAN_BASE_ACK_LONG_DATA;
    }
}

//等待长包结果包的状态处理
static void CanSlaveStateMachineWaitEndProc(CAN_RECV_FAST_ID* fastCanIDPtr)
{
    //如果接收到的数据的发送板和本次接收到的数据的发送板对不上,这一包数据不要
    //这一句主要是防止同时收到两块板子的指令,数据错乱
    if(canSlaveCacheArray[fastCanIDPtr->channelNumber].senderBoardAddr != fastCanIDPtr->senderBoardAddr)
    {
        //数据丢弃
        return;
    }
    //接收完数据包,等待结束包
    if((fastCanIDPtr->packLenFlag == CAN_BASE_PACK_LEN_LONG) && (fastCanIDPtr->singleCode == CAN_BASE_SINGLE_CODE_LONG_END))
    {
        //接收完成
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState = CAN_SLAVE_CHANNEL_OK;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].ackCode = CAN_BASE_ACK_OK;
    }
    else
    {
        //应该接受结果包没收到结果包
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState = CAN_SLAVE_CHANNEL_ERR;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].ackCode = CAN_BASE_ACK_LONG_END;
    }
}


//从机指令处理
void CanSlaveProcWithCommand(CAN_RECV_FAST_ID* fastCanIDPtr)
{
    //首先,帧号不能为0
    if(fastCanIDPtr->frameCode == 0)
    {
        //帧号不对
        canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState = CAN_SLAVE_CHANNEL_ERR;
        canSlaveCacheArray[fastCanIDPtr->channelNumber].ackCode = CAN_BASE_ACK_FRAME_CODE;
    }
    //对新数据包的处理,当相应的信道处于长包等待数据或者等待结果包的情况下,收到新的数据包,立即清除以前的状态
    CanSlaveProcWithNewStartPack(fastCanIDPtr);
    //状态机调度
    if(canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState == CAN_SLAVE_CHANNEL_IDLE)
    {
        //空闲状态的数据处理
        CanSlaveStateMachineIdleProc(fastCanIDPtr);
    }
    else if(canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState == CAN_SLAVE_CHANNEL_WAIT_DATA)
    {
        //收到长包起始包,等待数据包
        CanSlaveStateMachineWaitDataProc(fastCanIDPtr);
    }
    else if(canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState == CAN_SLAVE_CHANNEL_WAIT_END)
    {
        //收到长包数据包,等待结果包
        CanSlaveStateMachineWaitEndProc(fastCanIDPtr);
    }
    else if(canSlaveCacheArray[fastCanIDPtr->channelNumber].currentState == CAN_SLAVE_CHANNEL_BUSY)
    {
        //当前状态忙,
        CanSlaveSendAckWithRecvCache(&canSlaveCacheArray[fastCanIDPtr->channelNumber],CAN_BASE_ACK_CHANNEL_BUSY);
    }
}

//CAN从机处理数据单元
void CanSlaveProcRecvData(CAN_RECV_FAST_ID* fastCanIDPtr,CanRxMsg* rxMsg)
{
    //从机指令只处理指令数据包
    //然后对数据进行拷贝
    canSlaveRecvDataLength = rxMsg->DLC;
    for(uint8_t i = 0; i < canSlaveRecvDataLength; i++)
    {
        canSlaveRecvDataBuffer[i] = rxMsg->Data[i];
    }
    //处理完成

    //检查通道号是否正确
    if(fastCanIDPtr->channelNumber >= CAN_PROTOCOL_BOARD_CHANNEL_MAX)
    {
        return;
    }

    //数据包处理
    CanSlaveProcWithCommand(fastCanIDPtr);
    //处理过程中产生的错误的修复
    CanSlaveErrorCheckProcess(fastCanIDPtr->channelNumber);
}
