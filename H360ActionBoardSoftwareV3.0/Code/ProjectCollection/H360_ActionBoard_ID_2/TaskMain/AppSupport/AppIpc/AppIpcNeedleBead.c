/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:15:22
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-10 19:04:26
**FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppIpc\AppIpcNeedleBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcNeedleBead.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_NEEDLE_BEAD
{
    IPC_CMD_NEEDLE_BEAD_RESET                      = 0x00,//磁珠针复位
    IPC_CMD_NEEDLE_BEAD_CLEAN                      = 0X01,//磁珠针清洗
    IPC_CMD_NEEDLE_BEAD_PRIME                      = 0X02,//磁珠针灌注
    IPC_CMD_NEEDLE_BEAD_MAINTAIN_CLEAN             = 0X03,//磁珠针清洗维护
    IPC_CMD_NEEDLE_BEAD_ROTATE                     = 0X04,//磁珠针旋转指定位置
    IPC_CMD_NEEDLE_BEAD_LIQUID_ABSORB              = 0X05,//磁珠针吸液
    IPC_CMD_NEEDLE_BEAD_LIQUID_INJECT              = 0X06,//磁珠针注液 
    IPC_CMD_NEEDLE_BEAD_LIQUID_DETECT_ADJUST       = 0X07,//磁珠针探液校准   
    IPC_CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE   = 0X08,//磁珠针探液板状态 
    IPC_CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK       = 0x09,//磁珠针试剂余量校准
}IPC_CMD_NEEDLE_BEAD;

//IPC调用的数据结构
static DATA_IPC_NEEDLE_BEAD dataIPC_NeedleBead;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_NeedleBead;

//初始化IPC调用结构
void IPC_NeedleBeadDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_NeedleBead,
				  (CPU_CHAR*	)"mutexIPC_NeedleBead",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_NeedleBead.dataMutexPtr = &mutexIPC_NeedleBead;
    dataIPC_NeedleBead.state = STATE_IPC_IDLE;
    dataIPC_NeedleBead.resultValue = LH_ERR_NONE;
}



/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_NeedleBeadGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_NeedleBead,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_NeedleBead.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_NeedleBead,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_NeedleBead.state;
}

//等待执行完成,阻塞
LH_ERR IPC_NeedleBeadWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_NeedleBeadGetLastCommandStatusAndResult(&errorCode);
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
static LH_ERR IPC_NeedleBeadSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_NeedleBead,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_NeedleBead.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_NeedleBead.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_NeedleBead);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskNeedleBead, //向协议处理线程发送消息
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
        dataIPC_NeedleBead.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_NeedleBead,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_NeedleBead,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_NeedleBead.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//磁珠针复位,带零位偏移,不等待执行完成
LH_ERR IPC_NeedleBeadResetWhileAck(void)
{
    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//磁珠针清洗,不等待执行完成
LH_ERR IPC_NeedleBeadCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
}

//磁珠针灌注,不等待执行完成
LH_ERR IPC_NeedleBeadPrimeWhileAck(NEEDLE_PRIME_OPT opt)
{
    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_PRIME,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//磁珠针清洗维护,不等待执行完成
LH_ERR IPC_NeedleBeadMaintainCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_MAINTAIN_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
}

//磁珠针旋转指定位置,不等待执行完成
LH_ERR IPC_NeedleBeadRotateWhileAck(NEEDLE_BEAD_POS targetPos)
{
    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_ROTATE,((int32_t)targetPos),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//磁珠针吸液,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidAbsorbWhileAck(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos)
{

    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_ABSORB,((int32_t)absorbMinUlCount),((int32_t)lastLiquidDetectPos),((int32_t)0),((int32_t)0),((int32_t)0));
}

//磁珠针注液,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidInjectWhileAck(float cupLiquidUl,uint8_t mixCount)
{
    uint32_t cupLiquidConvert = (uint32_t)(cupLiquidUl * 1000);
    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_INJECT,((int32_t)cupLiquidConvert),((int32_t)mixCount),((int32_t)0),((int32_t)0),((int32_t)0));
}

//磁珠针探液校准,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidDetectAdjustWhileAck(void)
{
    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_DETECT_ADJUST,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//读磁珠针探液板状态,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config)
{
    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE,((int32_t)config),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//磁珠针试剂余量校准,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidResidueCheckWhileAck(void)
{
    return IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}


/****************************************************发出指令并等待完成********************************************************/
//磁珠针复位,带零位偏移,等待执行完成
LH_ERR IPC_NeedleBeadResetWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针清洗,等待执行完成
LH_ERR IPC_NeedleBeadCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针灌注,等待执行完成
LH_ERR IPC_NeedleBeadPrimeWhileReturn(NEEDLE_PRIME_OPT opt)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_PRIME,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针清洗维护,等待执行完成
LH_ERR IPC_NeedleBeadMaintainCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_MAINTAIN_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针旋转指定位置,等待执行完成
LH_ERR IPC_NeedleBeadRotateWhileReturn(NEEDLE_BEAD_POS targetPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_ROTATE,((int32_t)targetPos),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针吸液,等待执行完成
LH_ERR IPC_NeedleBeadLiquidAbsorbWhileReturn(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_ABSORB,((int32_t)absorbMinUlCount),((int32_t)lastLiquidDetectPos),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针注液,等待执行完成
LH_ERR IPC_NeedleBeadLiquidInjectWhileReturn(float cupLiquidUl,uint8_t mixCount)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint32_t cupLiquidConvert = (uint32_t)(cupLiquidUl * 1000);
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_INJECT,((int32_t)cupLiquidConvert),((int32_t)mixCount),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针探液校准,等待执行完成
LH_ERR IPC_NeedleBeadLiquidDetectAdjustWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_DETECT_ADJUST,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//读磁珠针探液板状态,等待执行完成
LH_ERR IPC_NeedleBeadLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE,((int32_t)config),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针试剂余量校准,等待执行完成
LH_ERR IPC_NeedleBeadLiquidResidueCheckWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleBeadWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}
