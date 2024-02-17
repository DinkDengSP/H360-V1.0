#include "ServiceTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferServiceTaskSerialDebugRS2[STK_SIZE_SERVICE_TASK_SERIAL_DEBUG_RS2]@".ccram";;
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskSerialDebugRS2@".ccram";;

//使用Uart3,主要针对串口指令解析
static uint16_t currentRecvUart3BufferLength = 0;
static uint8_t currentRecvUart3Buffer[SERIAL_RS2_BUFFER_MAX_LENGTH] = {0};

//中断回调函数
void SystemUart3RecvCallBack(uint8_t recvDataByte)
{
    //数据太多了
    if(currentRecvUart3BufferLength == SERIAL_RS2_BUFFER_MAX_LENGTH)
    {
        return;
    }
    else
    {
        currentRecvUart3Buffer[currentRecvUart3BufferLength++] = recvDataByte;
        //进行一些解析,协议处理?
        //防止报错
        currentRecvUart3Buffer[0] = currentRecvUart3Buffer[0];
    }
}

//模块CAN信息处理任务
static void ServiceTaskFuncSerialDebugRS2_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);


//任务函数
void ServiceTaskFuncSerialDebugRS2(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    while(1)
    {
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)0,
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
                ServiceTaskFuncSerialDebugRS2_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}




/************************************************CAN总线接收到的消息的处理******************************************************/
//串口输出缓存区
static LH_ERR ServiceTaskFuncSerialDebugRS2_CanMsg_Proc_WriteBuffer(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
//前一个是处理函数 后一个参数的最小长度
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT ServiceTaskCanMsgProcSerialDebugRS2Setting[] = {
    {ServiceTaskFuncSerialDebugRS2_CanMsg_Proc_WriteBuffer                     ,           3           ,""  },
};

#define TASK_CAN_MSG_SERIAL_DEBUG_RS2_COMMAND_COUNT  ((sizeof(ServiceTaskCanMsgProcSerialDebugRS2Setting))/(sizeof(ServiceTaskCanMsgProcSerialDebugRS2Setting[0])))


//模块CAN信息处理任务
static void ServiceTaskFuncSerialDebugRS2_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
{
    LH_ERR errorCode =LH_ERR_NONE;

    //首先,参数长度必须大于4
    if(canMsgCachePtr->currentLength < 4)
    {
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_LENGTH);
        //返回ACK错误
        return;
    }

    //读取指令码,识别指令码是否支持
    uint32_t commandValue = CanBaseReadDataConvertToUint32(canMsgCachePtr->recvCacheArray,0);
    if(commandValue >= TASK_CAN_MSG_SERIAL_DEBUG_RS2_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +ServiceTaskCanMsgProcSerialDebugRS2Setting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = ServiceTaskCanMsgProcSerialDebugRS2Setting[commandValue].funcPtr(canMsgCachePtr,
                    ServiceTaskCanMsgProcSerialDebugRS2Setting[commandValue].debugDetailMsg);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//串口输出缓存区
static LH_ERR ServiceTaskFuncSerialDebugRS2_CanMsg_Proc_WriteBuffer(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return LH_ERR_NONE;
}


