#include "ServiceTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferServiceTaskSerialUserRS1[STK_SIZE_SERVICE_TASK_SERIAL_USER_RS1]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskSerialUserRS1@".ccram";


//只有在通用板的情况下,才需要设定回调函数
#if(SELF_BOARD_ID == BOARD_ID_0)
static uint16_t currentRecvUart1BufferLength = 0;
static uint8_t currentRecvUart1Buffer[SERIAL_RS1_BUFFER_MAX_LENGTH] = {0};
//中断回调函数
void SystemUart1RecvCallBack(uint8_t recvDataByte)
{
    //数据太多了
    if(currentRecvUart1BufferLength == SERIAL_RS1_BUFFER_MAX_LENGTH)
    {
        return;
    }
    else
    {
        currentRecvUart1Buffer[currentRecvUart1BufferLength++] = recvDataByte;
    }
}
#endif

//模块定时执行任务
static void ServiceTaskFuncSerialUserRS1_Auto_Proc(void);
//模块CAN信息处理任务
static void ServiceTaskFuncSerialUserRS1_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void ServiceTaskFuncSerialUserRS1_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void ServiceTaskFuncSerialUserRS1(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //如果是通用板就执行这个初始化,否则就不管
#if(SELF_BOARD_ID == BOARD_ID_0)
    BoardRS_Init(RS_PORT_USER,115200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,
                                MCU_UART_HARD_CONTROL_NONE,SystemUart1RecvCallBack);
#endif
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    while(1)
    {
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= 1000)
		{
			//上次发送时间和本次发送时间差了1000个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			ServiceTaskFuncSerialUserRS1_Auto_Proc();
		}

        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}

        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)1000,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);

        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(TASK_CMD_SRC_CAN == taskCommandPtr->cmdSrc)
            {
                //从can来的数据
                CAN_SLAVE_CHANNEL_CACHE* canRecvDatPtr = (CAN_SLAVE_CHANNEL_CACHE*)taskCommandPtr->cmdData;
                //对这个数据进行处理
                ServiceTaskFuncSerialUserRS1_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                ServiceTaskFuncSerialUserRS1_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}



/*********************************************自身定时任务处理*********************************************/
//模块定时执行任务
static void ServiceTaskFuncSerialUserRS1_Auto_Proc(void)
{
    return;
}



/************************************************CAN总线接收到的消息的处理******************************************************/
//串口输出缓存区
static LH_ERR ServiceTaskFuncSerialUserRS1_CanMsg_Proc_WriteBuffer(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//串口输出缓存区并等待一段时间后将数据返回
static LH_ERR ServiceTaskFuncSerialUserRS1_CanMsg_Proc_WriteBufferAndDelayRead(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//读取串口当前缓存数据
static LH_ERR ServiceTaskFuncSerialUserRS1_CanMsg_Proc_Read(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
//前一个是处理函数 后一个参数的最小长度
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT ServiceTaskCanMsgProcSerialUserRS1Setting[] = {
    {ServiceTaskFuncSerialUserRS1_CanMsg_Proc_WriteBuffer                               ,           3           ,"" },
    {ServiceTaskFuncSerialUserRS1_CanMsg_Proc_WriteBufferAndDelayRead                   ,           7           ,"" },
    {ServiceTaskFuncSerialUserRS1_CanMsg_Proc_Read                                      ,           0           ,"" },
};

#define TASK_CAN_MSG_SERIAL_USER_RS1_COMMAND_COUNT  ((sizeof(ServiceTaskCanMsgProcSerialUserRS1Setting))/(sizeof(ServiceTaskCanMsgProcSerialUserRS1Setting[0])))


//模块CAN信息处理任务
static void ServiceTaskFuncSerialUserRS1_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
{
    LH_ERR errorCode =LH_ERR_NONE;
    //首先,参数长度必须大于4
    if(canMsgCachePtr->currentLength < 4)
    {
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_LENGTH);
        //返回ACK错误
        return;
    }
    //读取指令码
    uint32_t commandValue = CanBaseReadDataConvertToUint32(canMsgCachePtr->recvCacheArray,0);
    //识别指令码是否支持
    if(commandValue >= TASK_CAN_MSG_SERIAL_USER_RS1_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }
    //识别长度是否支持
    if((4 +ServiceTaskCanMsgProcSerialUserRS1Setting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }
    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = ServiceTaskCanMsgProcSerialUserRS1Setting[commandValue].funcPtr(canMsgCachePtr,ServiceTaskCanMsgProcSerialUserRS1Setting[commandValue].debugDetailMsg);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//串口输出缓存区
static LH_ERR ServiceTaskFuncSerialUserRS1_CanMsg_Proc_WriteBuffer(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //往串口写入数据,2字节写入数据长度,至少一个字节数据
	uint16_t writeStringLength = 0;
    writeStringLength = CanBaseReadDataConvertToUint16(cachePtrLocal->recvCacheArray,4);
    if(writeStringLength != (cachePtrLocal->currentLength -6))
    {
        //写入参数长度与指示长度不匹配
		errCode = LH_ERR_SERVICE_SERIAL1_WRITE_LENGTH;
    }
    else
    {
        //写入串口数据
        ServiceImplSerialUserRS1WriteBuffer(cachePtrLocal->recvCacheArray+6,writeStringLength);
    }
    return errCode;
}

//串口输出缓存区并等待一段时间后将数据返回
static LH_ERR ServiceTaskFuncSerialUserRS1_CanMsg_Proc_WriteBufferAndDelayRead(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //结果缓存
    LH_ERR errCode = LH_ERR_NONE;
    //往串口写入数据,2字节写入数据长度,至少一个字节数据
	uint16_t writeStringLength = 0;
    writeStringLength = CanBaseReadDataConvertToUint16(cachePtrLocal->recvCacheArray,4);
    uint32_t delayReturnMs = 0;
    delayReturnMs = CanBaseReadDataConvertToUint32(cachePtrLocal->recvCacheArray,6);
    //开始数据校测
    if(writeStringLength != (cachePtrLocal->currentLength -10))
    {
        //写入参数长度与指示长度不匹配
		errCode = LH_ERR_SERVICE_SERIAL1_WRITE_LENGTH;
    }
    else
    {
#if(SELF_BOARD_ID == BOARD_ID_0)
        //清空接收数据
        currentRecvUart1BufferLength = 0;
        //写入串口数据
        ServiceImplSerialUserRS1WriteBuffer(cachePtrLocal->recvCacheArray+10,writeStringLength);
        if(delayReturnMs == 0)
		{
			//不需要返回
			return LH_ERR_NONE;
		}
        //延时
        CoreDelayMs(delayReturnMs);
        if(currentRecvUart1BufferLength != 0)
        {
            //读取返回,申请内存
            uint8_t* dataBufferSend = NULL;
            do
            {
                dataBufferSend = UserMemMalloc(SRAM_CCM,currentRecvUart1BufferLength);
            } while (dataBufferSend == NULL);
            //将端口值返回
            UserMemCopy((void*)dataBufferSend,(void*)currentRecvUart1Buffer,currentRecvUart1BufferLength);
            //返回数据包
            CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_SERIAL_RS1,dataBufferSend,currentRecvUart1BufferLength);
            //发送完成,释放内存
            UserMemFree(SRAM_CCM,dataBufferSend);
        }
        //返回完成
        currentRecvUart1BufferLength = 0;
#else
        //写入串口数据
        ServiceImplSerialUserRS1WriteBuffer(cachePtrLocal->recvCacheArray+10,writeStringLength);
        //不需要返回
		return LH_ERR_NONE;
#endif
    }
    return errCode;
}

//读取串口当前缓存数据
static LH_ERR ServiceTaskFuncSerialUserRS1_CanMsg_Proc_Read(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
#if(SELF_BOARD_ID == BOARD_ID_0)
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    if(currentRecvUart1BufferLength != 0)
    {
        //读取返回,申请内存
        uint8_t* dataBufferSend = NULL;
        do
        {
            dataBufferSend = UserMemMalloc(SRAM_CCM,currentRecvUart1BufferLength);
        } while (dataBufferSend == NULL);

        //将端口值返回
        UserMemCopy((void*)dataBufferSend,(void*)currentRecvUart1Buffer,currentRecvUart1BufferLength);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_SERIAL_RS1,dataBufferSend,currentRecvUart1BufferLength);
        //发送完成,释放内存
        UserMemFree(SRAM_CCM,dataBufferSend);
    }
#endif
    return LH_ERR_NONE;
}

/************************************************自身分层API调用的消息的处理**********************************/
//串口1,发送一包数据
static LH_ERR ServiceTaskFuncSerialUserRS1_Self_Proc_SendDataBuffer(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//串口1,发送一包数据并等待指定时间数据返回
static LH_ERR ServiceTaskFuncSerialUserRS1_Self_Proc_SendDataBufferDelayReadReceive(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//串口1,读取当前缓存数据
static LH_ERR ServiceTaskFuncSerialUserRS1_Self_Proc_ReadReceive(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT ServiceTaskSelfMsgProcSerialUserRS1Setting[] = {
    {ServiceTaskFuncSerialUserRS1_Self_Proc_SendDataBuffer                  ,"" },
    {ServiceTaskFuncSerialUserRS1_Self_Proc_SendDataBufferDelayReadReceive  ,"" },
    {ServiceTaskFuncSerialUserRS1_Self_Proc_ReadReceive                     ,"" },
};

//自身支持指令数量
#define TASK_SELF_MSG_SERIAL_USER_RS1_COMMAND_COUNT  ((sizeof(ServiceTaskSelfMsgProcSerialUserRS1Setting))/(sizeof(ServiceTaskSelfMsgProcSerialUserRS1Setting[0])))


//模块自身转发消息处理任务
static void ServiceTaskFuncSerialUserRS1_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_SERIAL_USER_RS1* cmdResultPtr = (DATA_IPC_SERIAL_USER_RS1*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_SERIAL_USER_RS1_COMMAND_COUNT)
	{
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = ServiceTaskSelfMsgProcSerialUserRS1Setting[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
            ServiceTaskSelfMsgProcSerialUserRS1Setting[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//串口调试1,发送一包数据
static LH_ERR ServiceTaskFuncSerialUserRS1_Self_Proc_SendDataBuffer(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //解析参数
    DATA_IPC_SERIAL_USER_RS1* cmdResultPtr = (DATA_IPC_SERIAL_USER_RS1*)(selfCmdPtr->commandDataSpecialChannel);
    uint8_t* sendBufferPtr = cmdResultPtr->sendCommandBuffer;
    uint16_t sendDataLength = cmdResultPtr->sendDataLength;
    //调用函数
    ServiceImplSerialUserRS1WriteBuffer(sendBufferPtr,sendDataLength);
    return errCode;
}

//串口1,发送一包数据并等待指定时间数据返回
static LH_ERR ServiceTaskFuncSerialUserRS1_Self_Proc_SendDataBufferDelayReadReceive(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //解析参数
    DATA_IPC_SERIAL_USER_RS1* cmdResultPtr = (DATA_IPC_SERIAL_USER_RS1*)(selfCmdPtr->commandDataSpecialChannel);
    uint8_t* sendBufferPtr = cmdResultPtr->sendCommandBuffer;
    uint16_t sendDataLength = cmdResultPtr->sendDataLength;
    //调用函数
    uint32_t delayMs = (uint32_t)(selfCmdPtr->commandParam1);
#if(SELF_BOARD_ID == BOARD_ID_0)
        //清空接收数据
        currentRecvUart1BufferLength = 0;
        //写入串口数据
        ServiceImplSerialUserRS1WriteBuffer(sendBufferPtr,sendDataLength);
        if(delayMs == 0)
		{
			//不需要返回
			return LH_ERR_NONE;
		}
        //延时
        CoreDelayMs(delayMs);
        if(currentRecvUart1BufferLength != 0)
        {
            cmdResultPtr->recvDataLength = currentRecvUart1BufferLength;
            UserMemCopy(cmdResultPtr->recvDataBuffer,currentRecvUart1Buffer,currentRecvUart1BufferLength);
        }
        //返回完成
        currentRecvUart1BufferLength = 0;
#else
        //写入串口数据
        ServiceImplSerialUserRS1WriteBuffer(sendBufferPtr,sendDataLength);
        cmdResultPtr->recvDataLength = 0;
#endif
    return errCode;
}

//串口1,读取当前缓存数据
static LH_ERR ServiceTaskFuncSerialUserRS1_Self_Proc_ReadReceive(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //解析参数
    DATA_IPC_SERIAL_USER_RS1* cmdResultPtr = (DATA_IPC_SERIAL_USER_RS1*)(selfCmdPtr->commandDataSpecialChannel);
#if(SELF_BOARD_ID == BOARD_ID_0)
    if(currentRecvUart1BufferLength != 0)
    {
        cmdResultPtr->recvDataLength = currentRecvUart1BufferLength;
        UserMemCopy(cmdResultPtr->recvDataBuffer,currentRecvUart1Buffer,currentRecvUart1BufferLength);
    }
    //返回完成
    currentRecvUart1BufferLength = 0;
#else
    cmdResultPtr->recvDataLength = 0;
#endif
    return errCode;
}
