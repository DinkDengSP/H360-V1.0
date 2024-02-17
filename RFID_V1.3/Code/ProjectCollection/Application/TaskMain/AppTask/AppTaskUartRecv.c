#include"AppTaskConfig.h"
//任务堆栈
CPU_STK stackBufferTaskUart1Recv[STK_SIZE_TASK_UART1_RECV];
//任务控制块
OS_TCB tcbTaskUart1Recv;
//串口接收分析
UART_CMD_RECV_STATE uart1CmdRecvState;
//空闲中断处理函数
void Uart1IdleIntRecvFunc(uint8_t *dataBuff,uint16_t dataLen)
{
    uint8_t *recvData = NULL;
    OS_ERR err;
    if(dataLen == 0)
    {
        return;
    }
    //申请内存
    recvData = UserMemMalloc(MEM_MAIN,dataLen);
    if(recvData == NULL)
    {
        SystemPrintf("Uart1 Idle Malloc err\r\n");
        return;
    }
    //复制接收
    UserMemCopy(recvData,dataBuff,dataLen);
    if(OSRunning)
    {
        //转发接收数据
        OSTaskQPost(    &tcbTaskUart1Recv,
                        (void *)recvData,
                        (OS_MSG_SIZE) dataLen,
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
    
        if(err != OS_ERR_NONE)
        {
            //释放内存
            UserMemFree(MEM_MAIN,recvData);
            SystemPrintf("Uart1 Idle QtaskQPost err,Error:%d\r\n",err);
            return;
        }
    }
    else
    {
        //释放内存
        UserMemFree(MEM_MAIN,recvData); 
        SystemPrintf("Os Not Runing!!!\r\n");
    }
}

//串口接收协议分析，只能分析一帧数据
static LH_ERR UartRecvWithProtocol(uint8_t *recvData,uint16_t size);
//串口数据分发
static LH_ERR UartRecvPackDispatchCmd( UART_CMD_UTIL *uart1CmdUtilPtr);
//串口数据分析
static LH_ERR UartRecvPackAnalysisCmd(UART_CMD_RECV_STATE *CmdrecvState);
//任务函数
void TaskFuncUart1Recv(void *p_arg)
{
    OS_ERR err;
    LH_ERR errorCode = LH_ERR_NONE;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    uint8_t *recvDat = NULL;
    uint16_t recvDatLen = 0;
    Uart1CmdRecvStateClear(&uart1CmdRecvState);
    //串口初始化
    MCU_Uart1Init(115200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,Uart1IdleIntRecvFunc);
    while (1)
    {
        recvDat = OSTaskQPend(  (OS_TICK)0,
                                (OS_OPT)OS_OPT_PEND_BLOCKING,
                                (OS_MSG_SIZE *)&size,
                                (CPU_TS *)0,
                                (OS_ERR *)&err);
        if((recvDat != NULL) &&(err == OS_ERR_NONE))
        {
            //接收数据长度不能为0
            recvDatLen = (uint16_t)size;
            if(recvDatLen == 0)
            {
                continue;
            }
            errorCode = UartRecvWithProtocol(recvDat,recvDatLen);
            
            if(errorCode != LH_ERR_NONE)
            {
                //数据解析失败
                Uart1CmdRecvStateClear(&uart1CmdRecvState);
                SystemPrintf("Uart Recv Protcol err:0x08X\r\n",errorCode);
            }
            //释放内存
            UserMemFree(MEM_MAIN,recvDat);
            //清空接收
            recvDat = NULL;
            recvDatLen = 0;
        }
        else
        {
            SystemPrintf("TaskFuncUart1Recv QSTaskQPend Faile!!,OS_ERR:0x08X\r\n",err);
        }
    }
}

//串口接收协议分析，只能分析一帧数据
static LH_ERR UartRecvWithProtocol(uint8_t *recvData,uint16_t size)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint16_t uartFlagCode = 0;
    uint8_t sumCheck = 0;
    //接收数据拷贝
    UserMemCopy((uart1CmdRecvState.recvDat)+uart1CmdRecvState.recvDataLength,recvData,size);
    //数据长度增加
    uart1CmdRecvState.recvDataLength += size;
    //接收数据长度超过一个最小的命令帧长度
    if(uart1CmdRecvState.recvDataLength >= UART_CMD_MIN_PROTOCOL_LEN)
    {
        //分析协议头
        uartFlagCode = ConvertArrayToUint16HighFirst(uart1CmdRecvState.recvDat,0);
        //协议头正常
        if(uartFlagCode == UART_CMD_HEAD_CODE)
        {
            //读取数据包的数量
            uart1CmdRecvState.recvBodyLength = ConvertArrayToUint16HighFirst(uart1CmdRecvState.recvDat,UART_CMD_BODY_LEN_OFFSET);
            //判断数据是否接收完整
            if(uart1CmdRecvState.recvBodyLength + UART_CMD_MIN_PROTOCOL_LEN <= uart1CmdRecvState.recvDataLength)
            {
                //数据接收完成，验证协议尾
                uartFlagCode = ConvertArrayToUint16HighFirst(uart1CmdRecvState.recvDat,uart1CmdRecvState.recvBodyLength + UART_CMD_MIN_PROTOCOL_LEN-2);
                if(uartFlagCode == UART_CMD_TAIL_CODE)
                {
                    //协议尾正常，进行校验
                    sumCheck = Uart1DataSumCheck(uart1CmdRecvState.recvDat+UART_FUNC_CODE_OFFSET,uart1CmdRecvState.recvBodyLength+3);
                    if(sumCheck == uart1CmdRecvState.recvDat[uart1CmdRecvState.recvBodyLength+UART_CMD_MIN_PROTOCOL_LEN-3])
                    {
                      
                        //校验完成，接收数据包正确，数据分发
                        UartRecvPackAnalysisCmd(&uart1CmdRecvState);
                       
                        //清空接收缓冲区
                        Uart1CmdRecvStateClear(&uart1CmdRecvState);
                    }
                    else
                    {
                        //和校验不通过
                        errorCode = LH_ERR_UART1_PROTOCOL_SUM_CHECK;
                    }
                }
                else
                {
                    //帧尾错误
                    errorCode = LH_ERR_UART1_PROTOCOL_TAIL;
                }
            }
        } 
        else
        {
            //帧头错误
            errorCode = LH_ERR_UART1_PROTOCOL_HEAD;
        }
    }
    return errorCode;
}

//串口数据分析
static LH_ERR UartRecvPackAnalysisCmd(UART_CMD_RECV_STATE *CmdrecvState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    UART_CMD_UTIL *uart1CmdUtilPtr = NULL;
    uart1CmdUtilPtr = UserMemMalloc(MEM_MAIN,(sizeof(UART_CMD_UTIL)/sizeof(uint8_t)));
    if(uart1CmdUtilPtr == NULL)
    {
        SystemPrintf("UartRecvPackDispatchCmd UserMemMalloc Fiald\r\n");
        return LH_ERR_USER_MALLOC_MEM_MAIN;
    }
    //结构体赋值,功能码
    uart1CmdUtilPtr->funcCode = CmdrecvState->recvDat[UART_FUNC_CODE_OFFSET];
    //数据长度
    uart1CmdUtilPtr->cmdDatLen = CmdrecvState->recvBodyLength;
    //数据内容
    if(uart1CmdUtilPtr->cmdDatLen != 0)
    {
         uart1CmdUtilPtr->cmdDat = UserMemMalloc(MEM_MAIN,uart1CmdUtilPtr->cmdDatLen);
         if(uart1CmdUtilPtr->cmdDat == NULL)
         {
            SystemPrintf("UartRecvPackDispatchCmd UserMemMalloc Fiald\r\n");
            UserMemFree(MEM_MAIN,uart1CmdUtilPtr);
            return LH_ERR_USER_MALLOC_MEM_MAIN;
         }
         UserMemCopy(uart1CmdUtilPtr->cmdDat,&(CmdrecvState->recvDat[UART_CMD_BODY_DATA_OFFSET]),uart1CmdUtilPtr->cmdDatLen);
    }
    //数据进行分发
    errorCode = UartRecvPackDispatchCmd(uart1CmdUtilPtr);
    if(errorCode != LH_ERR_NONE)
    {
        if(uart1CmdUtilPtr->cmdDat != NULL)
        {
            UserMemFree(MEM_MAIN,uart1CmdUtilPtr->cmdDat);
        }
        UserMemFree(MEM_MAIN,uart1CmdUtilPtr);
        return errorCode;
    }
    return errorCode;
}
//串口命令分发单元
UART_CMD_DISPATCH_UNTIL uart1CmdDispatchUnit[] = {
    {UART_CMD_JUMP_APP,                         &tcbTaskHeart},//跳转到APP
    {UART_CMD_JUMP_BOOT,                        &tcbTaskHeart},//跳转到Boot
    {UART_CMD_UPDATE_START,                     &tcbTaskHeart},//升级开始
    {UART_CMD_UPDATE_DATA,                      &tcbTaskHeart},//升级数据
    {UART_CMD_UPDATE_END,                       &tcbTaskHeart},//升级结束
    {UART_CMD_READ_SYSTEM_MODE,                 &tcbTaskHeart},//读系统状态
    {UART_CMD_READ_SOFT_VERSION,                &tcbTaskHeart},//读软件版本
    {UART_CMD_READ_SECTION,                     &tcbTaskRfid},//读指定扇区
    {UART_CMD_WRITE_SECTION,                    &tcbTaskRfid},//写指定扇区
    {UART_CMD_READ_REAGENT_DATA,                &tcbTaskRfid},//读试剂状态
    {UART_CMD_WRITE_REAGENT_DATA,               &tcbTaskRfid},//写试剂状态
    {UART_CMD_READ_ASSIST_REAGENT_DATA,         &tcbTaskRfid},//读辅助试剂状态
    {UART_CMD_WRITE_ASSIST_REAGENT_DATA,        &tcbTaskRfid},//写辅助试剂状态
    {UART_CMD_READ_RFID_CARD_ID,                &tcbTaskRfid},//读取RFID卡片ID
};
//串口命令分发单元处理函数
#define UART1_CMD_DISPATCH_UNIT_LENGTH          (sizeof(uart1CmdDispatchUnit)/sizeof(uart1CmdDispatchUnit[0]))

//串口数据分发
static LH_ERR UartRecvPackDispatchCmd( UART_CMD_UTIL *uart1CmdUtilPtr)
{
    OS_ERR err;
    uint8_t i;
    LH_ERR errorCode = LH_ERR_NONE;
    SYSTEM_CMD * systemCmdPtr = NULL;

        //系统命令赋值
    systemCmdPtr = UserMemMalloc(MEM_MAIN,(sizeof(SYSTEM_CMD)/sizeof(uint8_t)));
    if(systemCmdPtr == NULL)
    {
        SystemPrintf("UartRecvPackDispatchCmd UserMemMalloc Fiald\r\n");
        return LH_ERR_USER_MALLOC_MEM_MAIN;
    }
    //命令赋值
    systemCmdPtr->cmdSrc = SYSTEM_CMD_SRC_UART;
    systemCmdPtr->cmdDataPtr = (void *)uart1CmdUtilPtr;
    //数据分发
    for(i = 0;i < UART1_CMD_DISPATCH_UNIT_LENGTH;i++)
    {
        if((uart1CmdUtilPtr->funcCode)==(uart1CmdDispatchUnit[i].funcCode))
        {
            OSTaskQPost(    uart1CmdDispatchUnit[i].tcbTagetPtr,
                            (void *)systemCmdPtr,
                            (OS_MSG_SIZE)(sizeof(SYSTEM_CMD *)),
                            (OS_OPT)OS_OPT_POST_FIFO,
                            (OS_ERR *)&err);
            if(err != OS_ERR_NONE)
            {
                UserMemFree(MEM_MAIN,systemCmdPtr);
                SystemPrintf("UartRecvPackDispatchCmd OSTaskQPost Fiald,ERR:%d.\r\n",err);
                return LH_ERR_UART_DISPATCH_ERR;
            }
            break;
        }
    }
    //函数功能码错误
    if(i >= UART1_CMD_DISPATCH_UNIT_LENGTH)
    {
        return LH_ERR_UART1_PROTOCOL_FUNC_ERR;
    }
    return errorCode;
}

