/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:21:15
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-15 09:36:04
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppTaskLiquidAuto.c
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
CPU_STK stackBufferAppTaskLiquidAuto[STK_SIZE_APP_TASK_LIQUID_AUTO]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskLiquidAuto@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_LIQUID_AUTO_PERIOD        100
//模块定时任务初始化
static void AppTaskFuncLiquidAuto_Auto_Init(void);
#if(TIME_MS_AUTO_TASK_LIQUID_AUTO_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncLiquidAuto_Auto_Proc(void);
#endif

//模块CAN信息处理任务
static void AppTaskFuncLiquidAuto_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncLiquidAuto_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncLiquidAuto(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //延时初始化液路自动化
    CoreDelayMs(2000);
    //模块定时任务初始化
    AppTaskFuncLiquidAuto_Auto_Init();
    #if(TIME_MS_AUTO_TASK_LIQUID_AUTO_PERIOD != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_LIQUID_AUTO_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_LIQUID_AUTO_PERIOD)
		{
			//上次发送时间和本次发送时间差了1000个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncLiquidAuto_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_LIQUID_AUTO_PERIOD,
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
                AppTaskFuncLiquidAuto_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncLiquidAuto_Self_Proc(selfCmdPtr);
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
static void AppTaskFuncLiquidAuto_Auto_Init(void)
{
    AppImplLiquidAutoStateMachineInit();
}
#if(TIME_MS_AUTO_TASK_LIQUID_AUTO_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncLiquidAuto_Auto_Proc(void)
{
    AppImplLiquidAutoStateMachineLoop();
}
#endif



/************************************************CAN总线接收到的消息的处理******************************************************/
//液路自动化单元初始化
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_Init(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//强制执行一次抽废液
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_ForceTrigWasteLiquid(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//打开自动抽废液功能
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_EnableWasteLiquidAutomatic(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//关闭自动抽废液功能
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_DisableWasteLiquidAutomatic(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//查看自动抽废液状态
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_CheckStateWasteLiquid(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//关闭三针清洗液稀释液路
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_CloseLiquidDilutionThreeNeedleClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//打开三针清洗液稀释液路
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_OpenLiquidDilutionThreeNeedleClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcFuncPtrArrayLiquidAuto[] = {
    {AppTaskFuncLiquidAuto_CanMsg_Proc_Init                                         ,0  ,"Init"                                         },
    {AppTaskFuncLiquidAuto_CanMsg_Proc_ForceTrigWasteLiquid                         ,0  ,"ForceTrigWasteLiquid"                         },
    {AppTaskFuncLiquidAuto_CanMsg_Proc_EnableWasteLiquidAutomatic                   ,0  ,"EnableWasteLiquidAutomatic"                   },
    {AppTaskFuncLiquidAuto_CanMsg_Proc_DisableWasteLiquidAutomatic                  ,0  ,"DisableWasteLiquidAutomatic"                  },
    {AppTaskFuncLiquidAuto_CanMsg_Proc_CheckStateWasteLiquid                        ,0  ,"CheckStateWasteLiquid"                        },
    {AppTaskFuncLiquidAuto_CanMsg_Proc_CloseLiquidDilutionThreeNeedleClean          ,0  ,"CloseLiquidDilutionThreeNeedleClean"          },
    {AppTaskFuncLiquidAuto_CanMsg_Proc_OpenLiquidDilutionThreeNeedleClean           ,1  ,"OpenLiquidDilutionThreeNeedleClean"           },
};
#define TASK_CAN_MSG_COMMAND_COUNT_LIQUID_AUTO  ((sizeof(AppTaskCanMsgProcFuncPtrArrayLiquidAuto))/(sizeof(AppTaskCanMsgProcFuncPtrArrayLiquidAuto[0])))

//模块CAN信息处理任务
static void AppTaskFuncLiquidAuto_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_COMMAND_COUNT_LIQUID_AUTO)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }
    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcFuncPtrArrayLiquidAuto[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcFuncPtrArrayLiquidAuto[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcFuncPtrArrayLiquidAuto[commandValue].debugDetailMsg);
    
    //打印结果
    AppShowEndMsg((uint8_t*)AppLiquidAutoDebugEndMsg,errorCode);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//液路自动化单元初始化
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_Init(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidAutoInit();
    //返回结果
    return actionResult;
}

//强制执行一次抽废液
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_ForceTrigWasteLiquid(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidAutoForceTrigWasteLiquid();
    //返回结果
    return actionResult;
}

//打开自动抽废液功能
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_EnableWasteLiquidAutomatic(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidAutoEnableWasteLiquidAutomatic();
    //返回结果
    return actionResult;
}

//关闭自动抽废液功能
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_DisableWasteLiquidAutomatic(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidAutoDisableWasteLiquidAutomatic();
    //返回结果
    return actionResult;
}

//查看自动抽废液状态
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_CheckStateWasteLiquid(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    LIQUID_AUTOMATIC_STATE liquidAutomicState;
    actionResult = AppImplLiquidAutoCheckStateWasteLiquid(&liquidAutomicState);
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)liquidAutomicState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_LIQUID_AUTOMATIC,dataBufferSend,1);
    }
    //返回结果
    return actionResult;
}

//关闭三针清洗液稀释液路
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_CloseLiquidDilutionThreeNeedleClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidAutoCloseLiquidDilutionThreeNeedleClean();
    //返回结果
    return actionResult;
}

//打开三针清洗液稀释液路
static LH_ERR AppTaskFuncLiquidAuto_CanMsg_Proc_OpenLiquidDilutionThreeNeedleClean(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    DILUTION_CHANNEL diluentChannelSetect = (DILUTION_CHANNEL)(cachePtrLocal->recvCacheArray[4]);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)diluentChannelSetect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplLiquidAutoOpenLiquidDilutionThreeNeedleClean(diluentChannelSetect);
    //返回结果
    return actionResult;
}






/************************************************自身分层API调用的消息的处理**********************************/
//液路自动化单元初始化
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_Init(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//强制执行一次抽废液
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_ForceTrigWasteLiquid(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//打开自动抽废液功能
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_EnableWasteLiquidAutomatic(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//关闭自动抽废液功能
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_DisableWasteLiquidAutomatic(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//查看自动抽废液状态
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_CheckStateWasteLiquid(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//关闭三针清洗液稀释液路
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_CloseLiquidDilutionThreeNeedleClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//打开三针清洗液稀释液路
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_OpenLiquidDilutionThreeNeedleClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcFuncPtrArrayLiquidAuto[] = {
    {AppTaskFuncLiquidAuto_Self_Proc_Init                                           ,"Init"                                         },
    {AppTaskFuncLiquidAuto_Self_Proc_ForceTrigWasteLiquid                           ,"ForceTrigWasteLiquid"                         },
    {AppTaskFuncLiquidAuto_Self_Proc_EnableWasteLiquidAutomatic                     ,"EnableWasteLiquidAutomatic"                   },
    {AppTaskFuncLiquidAuto_Self_Proc_DisableWasteLiquidAutomatic                    ,"DisableWasteLiquidAutomatic"                  },
    {AppTaskFuncLiquidAuto_Self_Proc_CheckStateWasteLiquid                          ,"CheckStateWasteLiquid"                        },
    {AppTaskFuncLiquidAuto_Self_Proc_CloseLiquidDilutionThreeNeedleClean            ,"CloseLiquidDilutionThreeNeedleClean"          },
    {AppTaskFuncLiquidAuto_Self_Proc_OpenLiquidDilutionThreeNeedleClean             ,"OpenLiquidDilutionThreeNeedleClean"           },
};

//自身支持指令数量
#define TASK_SELF_MSG_COMMAND_COUNT_LIQUID_AUTO  ((sizeof(AppTaskSelfMsgProcFuncPtrArrayLiquidAuto))/(sizeof(AppTaskSelfMsgProcFuncPtrArrayLiquidAuto[0])))


//模块自身转发消息处理任务
static void AppTaskFuncLiquidAuto_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_LIQUID_AUTO* cmdResultPtr = (DATA_IPC_LIQUID_AUTO*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_COMMAND_COUNT_LIQUID_AUTO)
	{
       AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)"Unsupport",selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,
                        selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcFuncPtrArrayLiquidAuto[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,AppTaskSelfMsgProcFuncPtrArrayLiquidAuto[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    AppShowEndMsg((uint8_t*)AppLiquidAutoDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}


//液路自动化单元初始化
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_Init(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidAutoInit();
    return actionResult;
}

//强制执行一次抽废液
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_ForceTrigWasteLiquid(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidAutoForceTrigWasteLiquid();
    return actionResult;
}

//打开自动抽废液功能
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_EnableWasteLiquidAutomatic(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidAutoEnableWasteLiquidAutomatic();
    return actionResult;
}

//关闭自动抽废液功能
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_DisableWasteLiquidAutomatic(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidAutoDisableWasteLiquidAutomatic();
    return actionResult;
}

//查看自动抽废液状态
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_CheckStateWasteLiquid(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    LIQUID_AUTOMATIC_STATE liquidAutomicState;
    actionResult = AppImplLiquidAutoCheckStateWasteLiquid(&liquidAutomicState);
    return actionResult;
}

//关闭三针清洗液稀释液路
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_CloseLiquidDilutionThreeNeedleClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidAutoCloseLiquidDilutionThreeNeedleClean();
    return actionResult;
}

//打开三针清洗液稀释液路
static LH_ERR AppTaskFuncLiquidAuto_Self_Proc_OpenLiquidDilutionThreeNeedleClean(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    DILUTION_CHANNEL diluentChannelSetect = (DILUTION_CHANNEL)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppLiquidAutoDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(diluentChannelSetect),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplLiquidAutoOpenLiquidDilutionThreeNeedleClean(diluentChannelSetect);
    return actionResult;
}



























