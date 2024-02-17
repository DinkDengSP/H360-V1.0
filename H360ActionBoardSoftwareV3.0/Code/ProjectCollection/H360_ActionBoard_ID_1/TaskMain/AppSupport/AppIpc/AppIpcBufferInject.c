/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:36:01
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-06 13:39:30
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcBufferInject.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcBufferInject.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_BUFFER_INJECT
{
    IPC_CMD_BUFFER_INJECT_RESET             = 0X0000,//缓冲液泵阀复位
    IPC_CMD_BUFFER_INJECT_PRIME             = 0X0001,//缓冲液灌注
    IPC_CMD_BUFFER_INJECT_INJECT            = 0X0002,//缓冲液注液
    IPC_CMD_BUFFER_INJECT_MAINTAIN_PRIME    = 0X0003,//缓冲液清洗维护
}IPC_CMD_BUFFER_INJECT;

//IPC调用的数据结构
static DATA_IPC_BUFFER_INJECT dataIPC_BufferInject;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_BufferInject;

//初始化IPC调用结构
void IPC_BufferInjectDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_BufferInject,
				  (CPU_CHAR*	)"mutexIPC_BufferInject",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_BufferInject.dataMutexPtr = &mutexIPC_BufferInject;
    dataIPC_BufferInject.state = STATE_IPC_IDLE;
    dataIPC_BufferInject.resultValue = LH_ERR_NONE;
}


/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_BufferInjectGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_BufferInject,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_BufferInject.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_BufferInject,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_BufferInject.state;
}

//等待执行完成,阻塞
LH_ERR IPC_BufferInjectWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_BufferInjectGetLastCommandStatusAndResult(&errorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            CoreDelayMinTick();
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

/****************************************************内部向任务发送消息的API*******************************************************/
//动作API发送任务给动作API
static LH_ERR IPC_BufferInjectSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_BufferInject,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_BufferInject.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_BufferInject.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_BufferInject);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskBufferInject, //向协议处理线程发送消息
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
        dataIPC_BufferInject.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_BufferInject,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_BufferInject,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_BufferInject.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//缓冲液泵阀复位,不等待完成
LH_ERR IPC_BufferInjectResetWhileAck(void)
{
    return IPC_BufferInjectSendCommandWhileAck(IPC_CMD_BUFFER_INJECT_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//缓冲液灌注,不等待完成
LH_ERR IPC_BufferInjectPrimeWhileAck(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    return IPC_BufferInjectSendCommandWhileAck(IPC_CMD_BUFFER_INJECT_PRIME,((int32_t)bottltSelect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//缓冲液注液,不等待完成
LH_ERR IPC_BufferInjectInjectWhileAck(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    return IPC_BufferInjectSendCommandWhileAck(IPC_CMD_BUFFER_INJECT_INJECT,((int32_t)bottltSelect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//缓冲液清洗维护,不等待完成
LH_ERR IPC_BufferInjectMaintainPrimeWhileAck(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    return IPC_BufferInjectSendCommandWhileAck(IPC_CMD_BUFFER_INJECT_MAINTAIN_PRIME,((int32_t)bottltSelect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}


/****************************************************发出指令并等待完成********************************************************/
//缓冲液泵阀复位,等待完成
LH_ERR IPC_BufferInjectResetWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_BufferInjectSendCommandWhileAck(IPC_CMD_BUFFER_INJECT_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_BufferInjectWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//缓冲液灌注,等待完成
LH_ERR IPC_BufferInjectPrimeWhileReturn(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_BufferInjectSendCommandWhileAck(IPC_CMD_BUFFER_INJECT_PRIME,((int32_t)bottltSelect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_BufferInjectWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//缓冲液注液,等待完成
LH_ERR IPC_BufferInjectInjectWhileReturn(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_BufferInjectSendCommandWhileAck(IPC_CMD_BUFFER_INJECT_INJECT,((int32_t)bottltSelect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_BufferInjectWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//缓冲液清洗维护,等待完成
LH_ERR IPC_BufferInjectMaintainPrimeWhileReturn(BOTTLE_BUFFER_SOLUTION bottltSelect)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_BufferInjectSendCommandWhileAck(IPC_CMD_BUFFER_INJECT_MAINTAIN_PRIME,((int32_t)bottltSelect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_BufferInjectWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}


