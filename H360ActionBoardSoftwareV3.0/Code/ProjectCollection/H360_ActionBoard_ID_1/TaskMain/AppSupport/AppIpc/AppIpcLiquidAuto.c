/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:35:50
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-06 13:51:19
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcLiquidAuto.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcLiquidAuto.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_LIQUID_AUTO
{
    IPC_CMD_LIQUID_AUTO_INIT                                                = 0X0000,//液路自动化单元初始化
    IPC_CMD_LIQUID_AUTO_FORCE_TRIG_WASTE_LIQUID                             = 0X0001,//强制执行一次抽废液
    IPC_CMD_LIQUID_AUTO_ENABLE_WASTE_LIQUID_AUTOMATIC                       = 0X0002,//打开自动抽废液功能
    IPC_CMD_LIQUID_AUTO_DISABLE_WASTE_LIQUID_AUTOMATIC                      = 0X0003,//关闭自动抽废液功能
    IPC_CMD_LIQUID_AUTO_CHECK_STATE_WASTE_LIQUID                            = 0X0004,//查看自动抽废液状态
    IPC_CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_NEEDLE_SAMPLE_POWERFUL_CLEAN  = 0X0005,//关闭样本针强力清洗液稀释液路
    IPC_CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_NEEDLE_SAMPLE_POWERFUL_CLEAN   = 0X0006,//打开样本针强力清洗液稀释液路
    IPC_CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_THREE_NEEDLE_CLEAN            = 0X0007,//关闭三针清洗液稀释液路
    IPC_CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_THREE_NEEDLE_CLEAN             = 0X0008,//打开三针清洗液稀释液路
    IPC_CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_WASH_NEEDLE_CLEAN             = 0X0009,//关闭清洗针清洗液稀释液路
    IPC_CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_WASH_NEEDLE_CLEAN              = 0X000A,//打开清洗针清洗液稀释液路
}IPC_CMD_LIQUID_AUTO;

//IPC调用的数据结构
static DATA_IPC_LIQUID_AUTO dataIPC_LiquidAuto;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_LiquidAuto;

//初始化IPC调用结构
void IPC_LiquidAutoDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_LiquidAuto,
				  (CPU_CHAR*	)"mutexIPC_LiquidAuto",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_LiquidAuto.dataMutexPtr = &mutexIPC_LiquidAuto;
    dataIPC_LiquidAuto.state = STATE_IPC_IDLE;
    dataIPC_LiquidAuto.resultValue = LH_ERR_NONE;
}

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_LiquidAutoGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_LiquidAuto,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_LiquidAuto.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_LiquidAuto,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_LiquidAuto.state;
}

//等待执行完成,阻塞
LH_ERR IPC_LiquidAutoWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_LiquidAutoGetLastCommandStatusAndResult(&errorCode);
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
static LH_ERR IPC_LiquidAutoSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_LiquidAuto,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_LiquidAuto.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_LiquidAuto.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_LiquidAuto);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskLiquidAuto, //向协议处理线程发送消息
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
        dataIPC_LiquidAuto.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_LiquidAuto,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_LiquidAuto,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_LiquidAuto.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//液路自动化单元初始化,不等待完成
LH_ERR IPC_LiquidAutoInitWhileAck(void)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//强制执行一次抽废液,不等待完成
LH_ERR IPC_LiquidAutoForceTrigWasteLiquidWhileAck(void)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_FORCE_TRIG_WASTE_LIQUID,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//打开自动抽废液功能,不等待完成
LH_ERR IPC_LiquidAutoEnableWasteLiquidAutomaticWhileAck(void)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENABLE_WASTE_LIQUID_AUTOMATIC,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//关闭自动抽废液功能,不等待完成
LH_ERR IPC_LiquidAutoDisableWasteLiquidAutomaticWhileAck(void)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_DISABLE_WASTE_LIQUID_AUTOMATIC,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//查看自动抽废液状态,不等待完成
LH_ERR IPC_LiquidAutoCheckStateWasteLiquidWhileAck(void)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_CHECK_STATE_WASTE_LIQUID,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//关闭样本针强力清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionNeedleSamplePowerfulCleanWhileAck(void)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_NEEDLE_SAMPLE_POWERFUL_CLEAN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//打开样本针强力清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionNeedleSamplePowerfulCleanWhileAck(DILUTION_CHANNEL channel)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_NEEDLE_SAMPLE_POWERFUL_CLEAN,((int32_t)channel),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//关闭三针清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileAck(void)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_THREE_NEEDLE_CLEAN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//打开三针清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileAck(DILUTION_CHANNEL channel)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_THREE_NEEDLE_CLEAN,((int32_t)channel),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//关闭清洗针清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionWashNeedleCleanWhileAck(void)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_WASH_NEEDLE_CLEAN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//打开清洗针清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionWashNeedleCleanWhileAck(DILUTION_CHANNEL channel)
{
    return IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_WASH_NEEDLE_CLEAN,((int32_t)channel),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}


/****************************************************发出指令并等待完成********************************************************/
//液路自动化单元初始化,等待完成
LH_ERR IPC_LiquidAutoInitWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//强制执行一次抽废液,等待完成
LH_ERR IPC_LiquidAutoForceTrigWasteLiquidWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_FORCE_TRIG_WASTE_LIQUID,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//打开自动抽废液功能,等待完成
LH_ERR IPC_LiquidAutoEnableWasteLiquidAutomaticWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENABLE_WASTE_LIQUID_AUTOMATIC,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//关闭自动抽废液功能,等待完成
LH_ERR IPC_LiquidAutoDisableWasteLiquidAutomaticWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_DISABLE_WASTE_LIQUID_AUTOMATIC,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//查看自动抽废液状态,等待完成
LH_ERR IPC_LiquidAutoCheckStateWasteLiquidWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_CHECK_STATE_WASTE_LIQUID,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//关闭样本针强力清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionNeedleSamplePowerfulCleanWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_NEEDLE_SAMPLE_POWERFUL_CLEAN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//打开样本针强力清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionNeedleSamplePowerfulCleanWhileReturn(DILUTION_CHANNEL channel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_NEEDLE_SAMPLE_POWERFUL_CLEAN,((int32_t)channel),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//关闭三针清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_THREE_NEEDLE_CLEAN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//打开三针清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileReturn(DILUTION_CHANNEL channel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_THREE_NEEDLE_CLEAN,((int32_t)channel),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//关闭清洗针清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionWashNeedleCleanWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_CLOSE_LIQUID_DILUTION_WASH_NEEDLE_CLEAN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//打开清洗针清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionWashNeedleCleanWhileReturn(DILUTION_CHANNEL channel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_LiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_OPEN_LIQUID_DILUTION_WASH_NEEDLE_CLEAN,((int32_t)channel),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_LiquidAutoWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}


