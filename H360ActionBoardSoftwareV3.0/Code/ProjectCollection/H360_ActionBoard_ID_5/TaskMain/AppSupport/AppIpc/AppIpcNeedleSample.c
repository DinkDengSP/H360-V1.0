/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:37:02
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-14 16:40:23
**FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppIpc\AppIpcNeedleSample.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcNeedleSample.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_NEEDLE_SAMPLE
{
    IPC_CMD_NEEDLE_SAMPLE_RESET                         = 0x00000000,//样本针复位
    IPC_CMD_NEEDLE_SAMPLE_PRIME_NORMAL                  = 0x00000001,//样本针普通清洗液灌注
    IPC_CMD_NEEDLE_SAMPLE_PRIME_POWERFUL                = 0X00000002,//样本针强力清洗液灌注
    IPC_CMD_NEEDLE_SAMPLE_CLEAN_NORMAL                  = 0X00000003,//样本针普通清洗
    IPC_CMD_NEEDLE_SAMPLE_CLEAN_POWERFUL                = 0X00000004,//样本针强力清洗
    IPC_CMD_NEEDLE_SAMPLE_MAINTAIN_CLEAN                = 0X00000005,//样本针清洗维护
    IPC_CMD_NEEDLE_SAMPLE_ROTATE                        = 0X00000006,//样本针旋转指定位置
    IPC_CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_SAMPLE_RACK     = 0x00000007,//样本针吸试管样本
    IPC_CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE_REACTION  = 0X00000008,//样本针吸反应盘样本
    IPC_CMD_NEEDLE_SAMPLE_LIQUID_INJECT                 = 0X00000009,//样本针注液
    IPC_CMD_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST          = 0X0000000A,//样本针探液校准
    IPC_CMD_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE      = 0x0000000B,//样本针读探液板状态
}IPC_CMD_NEEDLE_SAMPLE;


//IPC调用的数据结构
static DATA_IPC_NEEDLE_SAMPLE dataIPC_NeedleSample;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_NeedleSample;

//初始化IPC调用结构
void IPC_NeedleSampleDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_NeedleSample,
				  (CPU_CHAR*	)"mutexIPC_NeedleSample",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_NeedleSample.dataMutexPtr = &mutexIPC_NeedleSample;
    dataIPC_NeedleSample.state = STATE_IPC_IDLE;
    dataIPC_NeedleSample.resultValue = LH_ERR_NONE;
}

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_NeedleSampleGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_NeedleSample,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_NeedleSample.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_NeedleSample,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_NeedleSample.state;
}

//等待执行完成,阻塞
LH_ERR IPC_NeedleSampleWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_NeedleSampleGetLastCommandStatusAndResult(&errorCode);
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
static LH_ERR IPC_NeedleSampleSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_NeedleSample,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_NeedleSample.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_NeedleSample.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_NeedleSample);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskNeedleSample, //向协议处理线程发送消息
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
        dataIPC_NeedleSample.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_NeedleSample,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_NeedleSample,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_NeedleSample.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}



/****************************************************发出指令不等待完成*******************************************************/
//样本针复位,不等待完成
LH_ERR IPC_NeedleSampleResetWhileAck(void)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//样本针清洗液灌注,不等待完成
LH_ERR IPC_NeedleSamplePrimeNormalWhileAck(NEEDLE_PRIME_OPT opt)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_PRIME_NORMAL,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//样本针强力清洗液灌注,不等待完成
LH_ERR IPC_NeedleSamplePrimePowerfulWhileAck(void)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_PRIME_POWERFUL,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//样本针清洗,不等待完成
LH_ERR IPC_NeedleSampleCleanNormalWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_CLEAN_NORMAL,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
}

//样本针强力清洗,不等待完成
LH_ERR IPC_NeedleSampleCleanPowerfulCleanWhileAck(uint8_t count)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_CLEAN_POWERFUL,((int32_t)count),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//样本针清洗维护,不等待完成
LH_ERR IPC_NeedleSampleMaintainCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_MAINTAIN_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
}

//样本针旋转指定位置,不等待完成
LH_ERR IPC_NeedleSampleRotateWhileAck(NEEDLE_SAMPLE_POS targetPos)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ROTATE,((int32_t)targetPos),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//样本针吸试管样本,不等待完成
LH_ERR IPC_NeedleSampleLiquidAbsorbSampleRackWhileAck(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos,TUBE_TYPE tubeType)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_SAMPLE_RACK,((int32_t)absorbMinUlCount),((int32_t)lastLiquidDetectPos),((int32_t)tubeType),((int32_t)0),((int32_t)0));
}

//样本针吸反应盘样本,不等待完成
LH_ERR IPC_NeedleSampleLiquidAbsorbPlateReactionWhileAck(uint16_t absorbMinUlCount,float cupLiquidAfterAbsorb)
{
    uint32_t cupResidueLiquidConvert = (uint32_t)(cupLiquidAfterAbsorb * 1000);
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE_REACTION,((int32_t)absorbMinUlCount),((int32_t)cupResidueLiquidConvert),((int32_t)0),((int32_t)0),((int32_t)0));
}

//样本针注液,不等待完成
LH_ERR IPC_NeedleSampleLiquidInjectWhileAck(float cupLiquidUl,uint8_t mixCount)
{
    uint32_t cupLiquidConvert = (uint32_t)(cupLiquidUl *1000);
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_INJECT,((int32_t)cupLiquidConvert),((int32_t)mixCount),((int32_t)0),((int32_t)0),((int32_t)0));
}

//样本针探液校准,不等待完成
LH_ERR IPC_NeedleSampleLiquidDetectAdjustWhileAck(void)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//读样本针探液板状态,不等待完成
LH_ERR IPC_NeedleSampleLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG opt)
{
    return IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}


/****************************************************发出指令并等待完成********************************************************/
//样本针复位,等待完成
LH_ERR IPC_NeedleSampleResetWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针清洗液灌注,等待完成
LH_ERR IPC_NeedleSamplePrimeNormalWhileReturn(NEEDLE_PRIME_OPT opt)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_PRIME_NORMAL,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针强力清洗液灌注,等待完成
LH_ERR IPC_NeedleSamplePrimePowerfulWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_PRIME_POWERFUL,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针清洗,等待完成
LH_ERR IPC_NeedleSampleCleanNormalWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_CLEAN_NORMAL,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针强力清洗,等待完成
LH_ERR IPC_NeedleSampleCleanPowerfulWhileReturn(uint8_t count)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_CLEAN_POWERFUL,((int32_t)count),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针清洗维护,等待完成
LH_ERR IPC_NeedleSampleMaintainCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_MAINTAIN_CLEAN,((int32_t)opt),((int32_t)time),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针旋转指定位置,等待完成
LH_ERR IPC_NeedleSampleRotateWhileReturn(NEEDLE_SAMPLE_POS targetPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ROTATE,((int32_t)targetPos),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针吸试管样本,等待完成
LH_ERR IPC_NeedleSampleLiquidAbsorbSampleRackWhileReturn(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos,TUBE_TYPE tubeType)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_SAMPLE_RACK,((int32_t)absorbMinUlCount),((int32_t)lastLiquidDetectPos),((int32_t)tubeType),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针吸反应盘样本,等待完成
LH_ERR IPC_NeedleSampleLiquidAbsorbPlateReactionWhileReturn(uint16_t absorbMinUlCount,float cupLiquidAfterAbsorb)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint32_t cupResidueLiquidConvert = (uint32_t)(cupLiquidAfterAbsorb * 1000);
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE_REACTION,((int32_t)absorbMinUlCount),((int32_t)cupResidueLiquidConvert),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针注液,等待完成
LH_ERR IPC_NeedleSampleLiquidInjectWhileReturn(float cupLiquidUl,uint8_t mixCount)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint32_t cupLiquidConvert = (uint32_t)(cupLiquidUl *1000);
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_INJECT,((int32_t)cupLiquidConvert),((int32_t)mixCount),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针探液校准,等待完成
LH_ERR IPC_NeedleSampleLiquidDetectAdjustWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//读样本针探液板状态,等待完成
LH_ERR IPC_NeedleSampleLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG opt)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_NeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_NeedleSampleWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}


