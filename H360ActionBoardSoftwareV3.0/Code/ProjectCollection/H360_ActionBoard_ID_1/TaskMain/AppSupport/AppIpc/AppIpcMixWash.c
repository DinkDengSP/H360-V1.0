/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:37:33
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-06 13:47:56
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcMixWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcMixWash.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_MIX_WASH
{
    IPC_CMD_MIX_WASH_RESET             = 0X0000,//清洗摇匀复位
    IPC_CMD_MIX_WASH_UP                = 0X0001,//清洗摇匀升起来
    IPC_CMD_MIX_WASH_DOWN              = 0X0002,//清洗摇匀降下去
    IPC_CMD_MIX_WASH_START_ROTATE      = 0X0003,//清洗摇匀运行一次,不带自动升
    IPC_CMD_MIX_WASH_ROTATE_INTEGRATED = 0X0004,//清洗摇匀集成摇匀一次
}IPC_CMD_MIX_WASH;

//IPC调用的数据结构
static DATA_IPC_MIX_WASH dataIPC_MixWash;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_MixWash;

//初始化IPC调用结构
void IPC_MixWashDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_MixWash,
				  (CPU_CHAR*	)"mutexIPC_MixWash",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_MixWash.dataMutexPtr = &mutexIPC_MixWash;
    dataIPC_MixWash.state = STATE_IPC_IDLE;
    dataIPC_MixWash.resultValue = LH_ERR_NONE;
}


//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_MixWashGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_MixWash,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_MixWash.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_MixWash,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_MixWash.state;
}

//等待执行完成,阻塞
LH_ERR IPC_MixWashWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_MixWashGetLastCommandStatusAndResult(&errorCode);
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
static LH_ERR IPC_MixWashSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_MixWash,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_MixWash.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_MixWash.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_MixWash);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskMixWash, //向协议处理线程发送消息
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
        dataIPC_MixWash.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_MixWash,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_MixWash,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_MixWash.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//清洗混匀模块复位,带零位偏移,不等待完成
LH_ERR IPC_MixWashResetWhileAck(void)
{
    return IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//清洗混匀升起来,不等待完成
LH_ERR IPC_MixWashUpWhileAck(MIX_MODE mixMode)
{
    return IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_UP,((int32_t)mixMode),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//清洗混匀升降降下去,不等待完成
LH_ERR IPC_MixWashDownWhileAck(void)
{
    return IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_DOWN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//清洗混匀旋转混匀一次,不带自动升降,不等待完成
LH_ERR IPC_MixWashStartRotateWhileAck(MIX_MODE mixMode)
{
    return IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_START_ROTATE,((int32_t)mixMode),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//清洗混匀集成混匀一次,不等待完成
LH_ERR IPC_MixWashRotateIntegrateWhileAck(MIX_MODE mixMode)
{
    return IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_ROTATE_INTEGRATED,((int32_t)mixMode),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}


/****************************************************发出指令并等待完成********************************************************/
//清洗混匀模块复位,带零位偏移,等待完成
LH_ERR IPC_MixWashResetWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MixWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗混匀升起来,等待完成
LH_ERR IPC_MixWashUpWhileReturn(MIX_MODE mixMode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_UP,((int32_t)mixMode),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MixWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗混匀升降降下去,等待完成
LH_ERR IPC_MixWashDownWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_DOWN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MixWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗混匀旋转混匀一次,不带自动升降,等待完成
LH_ERR IPC_MixWashStartRotateWhileReturn(MIX_MODE mixMode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_START_ROTATE,((int32_t)mixMode),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MixWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗混匀集成混匀一次,等待完成
LH_ERR IPC_MixWashRotateIntegrateWhileReturn(MIX_MODE mixMode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MixWashSendCommandWhileAck(IPC_CMD_MIX_WASH_ROTATE_INTEGRATED,((int32_t)mixMode),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MixWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}


