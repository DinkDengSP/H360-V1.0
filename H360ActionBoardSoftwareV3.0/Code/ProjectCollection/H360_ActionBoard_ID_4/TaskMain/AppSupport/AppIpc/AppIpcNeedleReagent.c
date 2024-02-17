/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:06:51
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-12 13:51:47
**FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppIpc\AppIpcNeedleReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcNeedleReagent.h"
#include "AppTaskConfig.h"

//通道指令
typedef enum IPC_CMD_NEEDLE_REAGENT
{
    IPC_CMD_NEEDLE_REAGENT_RESET                    = 0X0000,//试剂针复位
    IPC_CMD_NEEDLE_REAGENT_CLEAN                    = 0X0001,//试剂针清洗
    IPC_CMD_NEEDLE_REAGENT_PRIME                    = 0X0002,//试剂针灌注
    IPC_CMD_NEEDLE_REAGENT_MAINTAIN_CLEAN           = 0X0003,//试剂针清洗维护
    IPC_CMD_NEEDLE_REAGENT_ROTATE                   = 0X0004,//试剂针旋转指定位置
    IPC_CMD_NEEDLE_REAGENT_LIQUID_ABSORB            = 0X0005,//试剂针吸液
    IPC_CMD_NEEDLE_REAGENT_LIQUID_INJECT            = 0X0006,//试剂针注液
    IPC_CMD_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST     = 0X0007,//试剂针探液校准
    IPC_CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE = 0X0008,//读试剂针探液板状态
    IPC_CMD_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK     = 0X0009,//试剂针试剂余量探测
}IPC_CMD_NEEDLE_REAGENT;

//IPC调用的数据结构
static DATA_IPC_NEEDLE_REAGENT dataIPC_NeedleReagent;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_NeedleReagent;

//初始化IPC调用结构
void IPC_NeedleReagentDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_NeedleReagent,
				  (CPU_CHAR*	)"mutexIPC_NeedleReagent",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_NeedleReagent.dataMutexPtr = &mutexIPC_NeedleReagent;
    dataIPC_NeedleReagent.state = STATE_IPC_IDLE;
    dataIPC_NeedleReagent.resultValue = LH_ERR_NONE;
}


/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_NeedleReagentGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_NeedleReagent,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_NeedleReagent.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_NeedleReagent,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_NeedleReagent.state;
}

//等待执行完成,阻塞
LH_ERR IPC_NeedleReagentWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_NeedleReagentGetLastCommandStatusAndResult(&errorCode);
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
static LH_ERR IPC_NeedleReagentSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_NeedleReagent,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_NeedleReagent.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_NeedleReagent.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_NeedleReagent);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskNeedleReagent, //向协议处理线程发送消息
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
        dataIPC_NeedleReagent.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_NeedleReagent,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_NeedleReagent,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_NeedleReagent.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//试剂针复位,带零位偏移,不等待完成
LH_ERR IPC_NeedleReagentResetWhileAck(void)
{
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//试剂针清洗,不等待完成
LH_ERR IPC_NeedleReagentCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
}

//试剂针灌注,不等待完成
LH_ERR IPC_NeedleReagentPrimeWhileAck(NEEDLE_PRIME_OPT opt)
{
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_PRIME,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//试剂针清洗维护,不等待完成
LH_ERR IPC_NeedleReagentMaintainCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_MAINTAIN_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
}

//试剂针旋转指定位置,不等待完成
LH_ERR IPC_NeedleReagentRotateWhileAck(NEEDLE_REAGENT_POS targetPos)
{
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_ROTATE,((int32_t)targetPos),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//试剂针吸液,不等待完成
LH_ERR IPC_NeedleReagentLiquidAbsorbWhileAck(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos)
{
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_ABSORB,((int32_t)absorbMinUlCount),((int32_t)lastLiquidDetectPos),((int32_t)0),((int32_t)0),((int32_t)0));
}

//试剂针注液,不等待完成
LH_ERR IPC_NeedleReagentLiquidInjectWhileAck(float cupLiquidUl,uint8_t mixCount)
{
    //转换液量为实际液量*1000
    uint32_t cupLiquidConvert = (uint32_t)(cupLiquidUl * 1000);
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_INJECT,((int32_t)cupLiquidConvert),((int32_t)mixCount),((int32_t)0),((int32_t)0),((int32_t)0));
}

//试剂针探液校准,不等待完成
LH_ERR IPC_NeedleReagentLiquidDetectAdjustWhileAck(void)
{
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//读试剂针探液板状态,不等待完成
LH_ERR IPC_NeedleReagentLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config)
{
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE,((int32_t)config),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//试剂针试剂余量探测,不等待完成
LH_ERR IPC_NeedleReagentLiquidResidueCheckWhileAck(void)
{
    return IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}



/****************************************************发出指令并等待完成********************************************************/
//试剂针复位,带零位偏移,等待完成
LH_ERR IPC_NeedleReagentResetWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//试剂针清洗,等待完成
LH_ERR IPC_NeedleReagentCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//试剂针灌注,等待完成
LH_ERR IPC_NeedleReagentPrimeWhileReturn(NEEDLE_PRIME_OPT opt)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_PRIME,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//试剂针清洗维护,等待完成
LH_ERR IPC_NeedleReagentMaintainCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_MAINTAIN_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//试剂针旋转指定位置,等待完成
LH_ERR IPC_NeedleReagentRotateWhileReturn(NEEDLE_REAGENT_POS targetPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_ROTATE,((int32_t)targetPos),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//试剂针吸液,等待完成
LH_ERR IPC_NeedleReagentLiquidAbsorbWhileReturn(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_ABSORB,((int32_t)absorbMinUlCount),((int32_t)lastLiquidDetectPos),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//试剂针注液,等待完成
LH_ERR IPC_NeedleReagentLiquidInjectWhileReturn(float cupLiquidUl,uint8_t mixCount)
{
    LH_ERR errorCode = LH_ERR_NONE;
        //转换液量为实际液量*1000
    uint32_t cupLiquidConvert = (uint32_t)(cupLiquidUl * 1000);
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_INJECT,((int32_t)cupLiquidConvert),((int32_t)mixCount),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//试剂针探液校准,等待完成
LH_ERR IPC_NeedleReagentLiquidDetectAdjustWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//读试剂针探液板状态,等待完成
LH_ERR IPC_NeedleReagentLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE,((int32_t)config),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//试剂针试剂余量探测,等待完成
LH_ERR IPC_NeedleReagentLiquidResidueCheckWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleReagentSendCommandWhileAck(IPC_CMD_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleReagentWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}



