/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:23:01
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 17:11:57
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppTaskLiquidNeedleSample.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"


//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferAppTaskLiquidNeedleSample[STK_SIZE_APP_TASK_LIQUID_NEEDLE_SAMPLE]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskLiquidNeedleSample@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_LIQUID_NEEDLE_SAMPLE_PERIOD        0
//模块定时任务初始化
static void AppTaskFuncLiquidNeedleSample_Auto_Init(void);
#if(TIME_MS_AUTO_TASK_LIQUID_NEEDLE_SAMPLE_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncLiquidNeedleSample_Auto_Proc(void);
#endif

//模块CAN信息处理任务
static void AppTaskFuncLiquidNeedleSample_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncLiquidNeedleSample_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncLiquidNeedleSample(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //模块定时任务初始化
    AppTaskFuncLiquidNeedleSample_Auto_Init();
    #if(TIME_MS_AUTO_TASK_LIQUID_NEEDLE_SAMPLE_PERIOD != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_LIQUID_NEEDLE_SAMPLE_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_LIQUID_NEEDLE_SAMPLE_PERIOD)
		{
			//上次发送时间和本次发送时间差了1000个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncLiquidNeedleSample_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_LIQUID_NEEDLE_SAMPLE_PERIOD,
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
                AppTaskFuncLiquidNeedleSample_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncLiquidNeedleSample_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}



/*********************************************自身定时任务处理*********************************************/
//模块定时任务初始化
static void AppTaskFuncLiquidNeedleSample_Auto_Init(void)
{

}
#if(TIME_MS_AUTO_TASK_LIQUID_NEEDLE_SAMPLE_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncLiquidNeedleSample_Auto_Proc(void)
{
    return;
}
#endif


/************************************************CAN总线接收到的消息的处理******************************************************/
//样本针泵阀复位
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//样本针清洗泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//样本针清洗泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_CloseClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//样本针强力清洗泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenPowerfulClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//样本针强力清洗泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_ClosePowerfulClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//样本针灌注泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenPrime(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//样本针灌注泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_ClosePrime(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//样本针清洗维护泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenMaintain(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//样本针清洗维护泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_CloseMaintain(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);


//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcFuncPtrArrayLiquidNeedleSample[] = {
    {AppTaskFuncLiquidNeedleSample_CanMsg_Proc_Reset                ,0  ,"Reset"             },
    {AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenClean            ,1  ,"OpenClean"         },
    {AppTaskFuncLiquidNeedleSample_CanMsg_Proc_CloseClean           ,1  ,"CloseClean"        },
    {AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenPowerfulClean    ,0  ,"OpenPowerfulClean" },
    {AppTaskFuncLiquidNeedleSample_CanMsg_Proc_ClosePowerfulClean   ,0  ,"ClosePowerfulClean"},
    {AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenPrime            ,1  ,"OpenPrime"         },
    {AppTaskFuncLiquidNeedleSample_CanMsg_Proc_ClosePrime           ,1  ,"ClosePrime"        },
    {AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenMaintain         ,1  ,"OpenMaintain"      },
    {AppTaskFuncLiquidNeedleSample_CanMsg_Proc_CloseMaintain        ,1  ,"CloseMaintain"     },
};
#define TASK_CAN_MSG_COMMAND_COUNT_LIQUID_NEEDLE_SAMPLE  ((sizeof(AppTaskCanMsgProcFuncPtrArrayLiquidNeedleSample))/(sizeof(AppTaskCanMsgProcFuncPtrArrayLiquidNeedleSample[0])))

//模块CAN信息处理任务
static void AppTaskFuncLiquidNeedleSample_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_COMMAND_COUNT_LIQUID_NEEDLE_SAMPLE)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }
    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcFuncPtrArrayLiquidNeedleSample[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcFuncPtrArrayLiquidNeedleSample[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcFuncPtrArrayLiquidNeedleSample[commandValue].debugDetailMsg);
    
    //打印结果
    AppShowEndMsg((uint8_t*)AppLiquidNeedleSampleDebugEndMsg,errorCode);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//样本针泵阀复位
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidNeedleSampleReset();
    //返回结果
    return actionResult;
}

//样本针清洗泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    NEEDLE_CLEAN_OPT opt = (NEEDLE_CLEAN_OPT)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidNeedleSampleOpenClean(opt);
    //返回结果
    return actionResult;
}

//样本针清洗泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_CloseClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    NEEDLE_CLEAN_OPT opt = (NEEDLE_CLEAN_OPT)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidNeedleSampleCloseClean(opt);
    //返回结果
    return actionResult;
}

//样本针强力清洗泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenPowerfulClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidNeedleSampleOpenPowerfulClean();
    //返回结果
    return actionResult;
}

//样本针强力清洗泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_ClosePowerfulClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidNeedleSampleClosePowerfulClean();
    //返回结果
    return actionResult;
}

//样本针灌注泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenPrime(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    NEEDLE_PRIME_OPT opt = (NEEDLE_PRIME_OPT)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidNeedleSampleOpenPrime(opt);
    //返回结果
    return actionResult;
}

//样本针灌注泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_ClosePrime(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    NEEDLE_PRIME_OPT opt = (NEEDLE_PRIME_OPT)(cachePtrLocal->recvCacheArray[4]);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidNeedleSampleClosePrime(opt);
    //返回结果
    return actionResult;
}

//样本针清洗维护泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_OpenMaintain(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    NEEDLE_MAINTAIN_OPT opt = (NEEDLE_MAINTAIN_OPT)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidNeedleSampleOpenMaintain(opt);
    //返回结果
    return actionResult;
}

//样本针清洗维护泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_CanMsg_Proc_CloseMaintain(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    NEEDLE_MAINTAIN_OPT opt = (NEEDLE_MAINTAIN_OPT)(cachePtrLocal->recvCacheArray[4]);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)opt),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidNeedleSampleCloseMaintain(opt);
    //返回结果
    return actionResult;
}




/************************************************自身分层API调用的消息的处理**********************************/
//样本针泵阀复位
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//样本针清洗泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_OpenClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//样本针清洗泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_CloseClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//样本针强力清洗泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_OpenPowerfulClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//样本针强力清洗泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_ClosePowerfulClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//样本针灌注泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_OpenPrime(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//样本针灌注泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_ClosePrime(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//样本针清洗维护泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_OpenMaintain(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//样本针清洗维护泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_CloseMaintain(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcFuncPtrArrayLiquidNeedleSample[] = {
    {AppTaskFuncLiquidNeedleSample_Self_Proc_Reset              ,"Reset"             },
    {AppTaskFuncLiquidNeedleSample_Self_Proc_OpenClean          ,"OpenClean"         },
    {AppTaskFuncLiquidNeedleSample_Self_Proc_CloseClean         ,"CloseClean"        },
    {AppTaskFuncLiquidNeedleSample_Self_Proc_OpenPowerfulClean  ,"OpenPowerfulClean" },
    {AppTaskFuncLiquidNeedleSample_Self_Proc_ClosePowerfulClean ,"ClosePowerfulClean"},
    {AppTaskFuncLiquidNeedleSample_Self_Proc_OpenPrime          ,"OpenPrime"         },
    {AppTaskFuncLiquidNeedleSample_Self_Proc_ClosePrime         ,"ClosePrime"        },
    {AppTaskFuncLiquidNeedleSample_Self_Proc_OpenMaintain       ,"OpenMaintain"      },
    {AppTaskFuncLiquidNeedleSample_Self_Proc_CloseMaintain      ,"CloseMaintain"     },
};

//自身支持指令数量
#define TASK_SELF_MSG_COMMAND_COUNT_LIQUID_NEEDLE_SAMPLE  ((sizeof(AppTaskSelfMsgProcFuncPtrArrayLiquidNeedleSample))/(sizeof(AppTaskSelfMsgProcFuncPtrArrayLiquidNeedleSample[0])))


//模块自身转发消息处理任务
static void AppTaskFuncLiquidNeedleSample_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_LIQUID_NEEDLE_SAMPLE* cmdResultPtr = (DATA_IPC_LIQUID_NEEDLE_SAMPLE*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_COMMAND_COUNT_LIQUID_NEEDLE_SAMPLE)
	{
       AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)"Unsupport",selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,
                        selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcFuncPtrArrayLiquidNeedleSample[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,AppTaskSelfMsgProcFuncPtrArrayLiquidNeedleSample[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    AppShowEndMsg((uint8_t*)AppLiquidNeedleSampleDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}


//样本针泵阀复位
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidNeedleSampleReset();
    return actionResult;
}

//样本针清洗泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_OpenClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    NEEDLE_CLEAN_OPT opt = (NEEDLE_CLEAN_OPT)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(opt),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidNeedleSampleOpenClean(opt);
    return actionResult;
}

//样本针清洗泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_CloseClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    NEEDLE_CLEAN_OPT opt = (NEEDLE_CLEAN_OPT)(selfCmdPtr->commandParam1);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(opt),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidNeedleSampleCloseClean(opt);
    return actionResult;
}

//样本针强力清洗泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_OpenPowerfulClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidNeedleSampleOpenPowerfulClean();
    return actionResult;
}

//样本针强力清洗泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_ClosePowerfulClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidNeedleSampleClosePowerfulClean();
    return actionResult;
}

//样本针灌注泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_OpenPrime(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    NEEDLE_PRIME_OPT opt = (NEEDLE_PRIME_OPT)(selfCmdPtr->commandParam1);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(opt),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidNeedleSampleOpenPrime(opt);
    return actionResult;
}

//样本针灌注泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_ClosePrime(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    NEEDLE_PRIME_OPT opt = (NEEDLE_PRIME_OPT)(selfCmdPtr->commandParam1);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(opt),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidNeedleSampleClosePrime(opt);
    return actionResult;
}

//样本针清洗维护泵阀打开
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_OpenMaintain(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    NEEDLE_MAINTAIN_OPT opt = (NEEDLE_MAINTAIN_OPT)(selfCmdPtr->commandParam1);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(opt),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidNeedleSampleOpenMaintain(opt);
    return actionResult;
}

//样本针清洗维护泵阀关闭
static LH_ERR AppTaskFuncLiquidNeedleSample_Self_Proc_CloseMaintain(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    NEEDLE_MAINTAIN_OPT opt = (NEEDLE_MAINTAIN_OPT)(selfCmdPtr->commandParam1);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidNeedleSampleDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(opt),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidNeedleSampleCloseMaintain(opt);
    return actionResult;
}




















