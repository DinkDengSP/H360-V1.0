#include "ServiceIpcSerialUserRS1.h"
#include "ServiceTaskConfig.h"

//IPC调用的数据结构
static DATA_IPC_SERIAL_USER_RS1 dataIPC_SerialUserRS1;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_SerialUserRS1;

//串口读取
typedef enum IPC_CMD_SERIAL_USER_RS1
{
    IPC_CMD_SERIAL_USER_RS1_SEND_BUFFER = 0X0000,
    IPC_CMD_SERIAL_USER_RS1_SEND_BUFFER_DELAY_READ_RECEIVE = 0X0001,
    IPC_CMD_SERIAL_USER_RS1_READ_RECEIVE = 0X0002,
}IPC_CMD_SERIAL_USER_RS1;

//输入任务的数据初始化
void IPC_SerialUserRS1DataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_SerialUserRS1,
				  (CPU_CHAR*	)"mutexIPC_SerialUserRS1",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_SerialUserRS1.dataMutexPtr = &mutexIPC_SerialUserRS1;
    dataIPC_SerialUserRS1.state = STATE_IPC_IDLE;
    dataIPC_SerialUserRS1.resultValue = LH_ERR_NONE;
    //数据初始化
    dataIPC_SerialUserRS1.sendDataLength = 0;
    dataIPC_SerialUserRS1.recvDataLength = 0;
    UserMemSet(dataIPC_SerialUserRS1.sendCommandBuffer,0,SERIAL_RS1_BUFFER_MAX_LENGTH);
    UserMemSet(dataIPC_SerialUserRS1.recvDataBuffer,0,SERIAL_RS1_BUFFER_MAX_LENGTH);
}

//输入任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_SerialUserRS1GetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_SerialUserRS1,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_SerialUserRS1.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_SerialUserRS1,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_SerialUserRS1.state;    
}

//动作API发送任务给动作API
static LH_ERR IPC_SerialUserRS1SendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_SerialUserRS1,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_SerialUserRS1.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_SerialUserRS1.resultValue = LH_ERR_NONE;

    //申请系统消息内存
    while(systemTaskCmd == NULL)
    {
        systemTaskCmd = UserMemMalloc(SRAM_CCM,sizeof(TASK_CMD));
        if(systemTaskCmd == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //只是指令是自身指令
    systemTaskCmd->cmdSrc = TASK_CMD_SRC_SELF;
    //申请到内存了
    systemTaskCmd->cmdData = NULL;

    //一定要申请到
    while(systemTaskCmd->cmdData == NULL)
    {
        systemTaskCmd->cmdData = UserMemMalloc(SRAM_CCM,sizeof(SYSTEM_CMD_SELF));
        if(systemTaskCmd->cmdData == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //辅助指针
    SYSTEM_CMD_SELF* utilSelfCmdPtr = (SYSTEM_CMD_SELF*)(systemTaskCmd->cmdData);
    //消息序号
    utilSelfCmdPtr->commandIndex = (uint16_t)cmdIndex;
    //参数设置
    utilSelfCmdPtr->commandParam1 = param1;
    utilSelfCmdPtr->commandParam2 = param2;
    utilSelfCmdPtr->commandParam3 = param3;
    utilSelfCmdPtr->commandParam4 = param4;
    utilSelfCmdPtr->commandParam5 = param5;
    //设置不同信道特有的数据指针
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_SerialUserRS1);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbServiceTaskSerialUserRS1, //向协议处理线程发送消息
                        (void *)systemTaskCmd,
                        (OS_MSG_SIZE)sizeof(TASK_CMD*),
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,直接释放本次申请的内存
        UserMemFree(SRAM_CCM, (void *)systemTaskCmd->cmdData);
        UserMemFree(SRAM_CCM, (void *)systemTaskCmd);
        //指令没发出去,还是回到空闲状态
        dataIPC_SerialUserRS1.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_SerialUserRS1,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_SerialUserRS1,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_SerialUserRS1.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


//发送数据
LH_ERR IPC_SerialUserRS1SendDataBufferWhileAck(uint8_t* dataBufferPtr,uint16_t dataLength)
{
    //首先将数据拷贝到缓存区
    UserMemCopy(dataIPC_SerialUserRS1.sendCommandBuffer,dataBufferPtr,dataLength);
    //然后设置发送长度
    dataIPC_SerialUserRS1.sendDataLength = dataLength;

    //发送数据出去
    return IPC_SerialUserRS1SendCommandWhileAck(IPC_CMD_SERIAL_USER_RS1_SEND_BUFFER,0,0,0,0,0);
}


//发送数据并延时等待返回
LH_ERR IPC_SerialUserRS1SendDataBufferDelayReadReceiveWhileAck(uint8_t* dataBufferPtr,uint16_t dataLength,uint32_t receiveDelayMs)
{
    //首先将数据拷贝到缓存区
    UserMemCopy(dataIPC_SerialUserRS1.sendCommandBuffer,dataBufferPtr,dataLength);
    //然后设置发送长度
    dataIPC_SerialUserRS1.sendDataLength = dataLength;
    //清除上一次接收到的数据和数据长度
    dataIPC_SerialUserRS1.recvDataLength = 0;
    UserMemSet(dataIPC_SerialUserRS1.recvDataBuffer,0,SERIAL_RS1_BUFFER_MAX_LENGTH);
    //将数据发送出去
    return IPC_SerialUserRS1SendCommandWhileAck(IPC_CMD_SERIAL_USER_RS1_SEND_BUFFER_DELAY_READ_RECEIVE,receiveDelayMs,0,0,0,0);
}


//读取当前串口接收到的数据
LH_ERR IPC_SerialUserRS1ReadReceiveWhileAck(void)
{
    //清空共享缓存
    dataIPC_SerialUserRS1.sendDataLength = 0;
    dataIPC_SerialUserRS1.recvDataLength = 0;
    UserMemSet(dataIPC_SerialUserRS1.sendCommandBuffer,0,SERIAL_RS1_BUFFER_MAX_LENGTH);
    UserMemSet(dataIPC_SerialUserRS1.recvDataBuffer,0,SERIAL_RS1_BUFFER_MAX_LENGTH);
    //发出指令
    return IPC_SerialUserRS1SendCommandWhileAck(IPC_CMD_SERIAL_USER_RS1_READ_RECEIVE,0,0,0,0,0);
}


//读取上次得到的数据,注意,此处得到的是一个指向指针的指针,传入的是一个指针空值
//如果有数据要发出,函数会自己申请内存,返回结果,所以,应用程序在检测到dataBufferPtr不为空值的时候
//数据使用完成,需要释放内存
uint16_t  IPC_SerialUserRS1LastReceiveData(uint8_t** dataBufferPtr)
{
    if(dataIPC_SerialUserRS1.recvDataLength == 0)
    {
        *dataBufferPtr = NULL;
        return 0;
    }
    else
    {
        //申请内存
        do
        {
            *dataBufferPtr = UserMemMalloc(SRAM_CCM,dataIPC_SerialUserRS1.recvDataLength);
            if(*dataBufferPtr == NULL)
            {
                CoreDelayMinTick();
            }
        }while(*dataBufferPtr == NULL);
        UserMemCopy(*dataBufferPtr,dataIPC_SerialUserRS1.recvDataBuffer,dataIPC_SerialUserRS1.recvDataLength);
        return dataIPC_SerialUserRS1.recvDataLength;
    }
}










