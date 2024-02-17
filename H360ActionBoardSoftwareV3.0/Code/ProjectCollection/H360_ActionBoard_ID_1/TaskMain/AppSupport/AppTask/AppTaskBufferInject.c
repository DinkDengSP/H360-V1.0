/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:17:28
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 17:11:39
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppTaskBufferInject.c
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
CPU_STK stackBufferAppTaskBufferInject[STK_SIZE_APP_TASK_BUFFER_INJECT]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskBufferInject@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_BUFFER_INJECT_PERIOD        0
//模块定时任务初始化
static void AppTaskFuncBufferInject_Auto_Init(void);
#if(TIME_MS_AUTO_TASK_BUFFER_INJECT_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncBufferInject_Auto_Proc(void);
#endif

//模块CAN信息处理任务
static void AppTaskFuncBufferInject_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncBufferInject_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncBufferInject(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //模块定时任务初始化
    AppTaskFuncBufferInject_Auto_Init();
    #if(TIME_MS_AUTO_TASK_BUFFER_INJECT_PERIOD != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_BUFFER_INJECT_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_BUFFER_INJECT_PERIOD)
		{
			//上次发送时间和本次发送时间差了1000个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncBufferInject_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_BUFFER_INJECT_PERIOD,
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
                AppTaskFuncBufferInject_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncBufferInject_Self_Proc(selfCmdPtr);
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
static void AppTaskFuncBufferInject_Auto_Init(void)
{
    
}
#if(TIME_MS_AUTO_TASK_BUFFER_INJECT_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncBufferInject_Auto_Proc(void)
{
    return;
}
#endif



/************************************************CAN总线接收到的消息的处理******************************************************/
//缓冲液泵阀复位
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//缓冲液灌注
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_Prime(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//缓冲液注液
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_Inject(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//缓冲液清洗维护
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_MaintainPrime(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机复位
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机走指定步数
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcFuncPtrArrayBufferInject[] = {
    {AppTaskFuncBufferInject_CanMsg_Proc_Reset              ,0  ,"Reset"                },
    {AppTaskFuncBufferInject_CanMsg_Proc_Prime              ,1  ,"Prime"                },
    {AppTaskFuncBufferInject_CanMsg_Proc_Inject             ,1  ,"Inject"               },
    {AppTaskFuncBufferInject_CanMsg_Proc_MaintainPrime      ,1  ,"MaintainPrime"        },
    {AppTaskFuncBufferInject_CanMsg_Proc_StepMotorReset     ,1  ,"StepMotorReset"       },
    {AppTaskFuncBufferInject_CanMsg_Proc_StepMotorRunSteps  ,5  ,"StepMotorRunSteps"    },
};
#define TASK_CAN_MSG_COMMAND_COUNT_BUFFER_INJECT  ((sizeof(AppTaskCanMsgProcFuncPtrArrayBufferInject))/(sizeof(AppTaskCanMsgProcFuncPtrArrayBufferInject[0])))

//模块CAN信息处理任务
static void AppTaskFuncBufferInject_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_COMMAND_COUNT_BUFFER_INJECT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }
    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcFuncPtrArrayBufferInject[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength) 
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcFuncPtrArrayBufferInject[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcFuncPtrArrayBufferInject[commandValue].debugDetailMsg);

    //打印结果
    AppShowEndMsg((uint8_t*)AppBufferInjectDebugEndMsg,errorCode);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//缓冲液泵阀复位
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplBufferSolutionInjectReset();
    //返回结果
    return actionResult;
}

//缓冲液灌注
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_Prime(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    BOTTLE_BUFFER_SOLUTION bottltSelect = (BOTTLE_BUFFER_SOLUTION)(cachePtrLocal->recvCacheArray[4]);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)bottltSelect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplBufferSolutionInjectPrime(bottltSelect);
    //返回结果
    return actionResult;
}

//缓冲液注液
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_Inject(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    BOTTLE_BUFFER_SOLUTION bottltSelect = (BOTTLE_BUFFER_SOLUTION)(cachePtrLocal->recvCacheArray[4]);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)bottltSelect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplBufferSolutionInjectInject(bottltSelect);
    //返回结果
    return actionResult;
}

//缓冲液清洗维护
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_MaintainPrime(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    BOTTLE_BUFFER_SOLUTION bottltSelect = (BOTTLE_BUFFER_SOLUTION)(cachePtrLocal->recvCacheArray[4]);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)bottltSelect),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplBufferSolutionInjectMaintainPrime(bottltSelect);
    //返回结果
    return actionResult;
}

//指定步进电机复位
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_MOTOR_BUFFER_SOLUTION motorIndex = (INDEX_MOTOR_BUFFER_SOLUTION)(cachePtrLocal->recvCacheArray[4]);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)motorIndex),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplBufferSolutionInjectStepMotorReset(motorIndex);
    //返回结果
    return actionResult;
}

//指定步进电机走指定步数
static LH_ERR AppTaskFuncBufferInject_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_MOTOR_BUFFER_SOLUTION motorIndex = (INDEX_MOTOR_BUFFER_SOLUTION)(cachePtrLocal->recvCacheArray[4]);
    int32_t runSteps = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,5);
    int32_t posAfterRun;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)motorIndex),((int32_t)runSteps),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplBufferSolutionInjectStepMotorRunSteps(motorIndex,runSteps,&posAfterRun);
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)motorIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,posAfterRun);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_MODULE_MOTOR,dataBufferSend,5);
    }
    //返回结果
    return actionResult;
}






/************************************************自身分层API调用的消息的处理**********************************/
//缓冲液泵阀复位
static LH_ERR AppTaskFuncBufferInject_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//缓冲液灌注
static LH_ERR AppTaskFuncBufferInject_Self_Proc_Prime(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//缓冲液注液
static LH_ERR AppTaskFuncBufferInject_Self_Proc_Inject(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//缓冲液清洗维护
static LH_ERR AppTaskFuncBufferInject_Self_Proc_MaintainPrime(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcFuncPtrArrayBufferInject[] = {
    {AppTaskFuncBufferInject_Self_Proc_Reset            ,"Reset"        },
    {AppTaskFuncBufferInject_Self_Proc_Prime            ,"Prime"        },
    {AppTaskFuncBufferInject_Self_Proc_Inject           ,"Inject"       },
    {AppTaskFuncBufferInject_Self_Proc_MaintainPrime    ,"MaintainPrime"},
};

//自身支持指令数量
#define TASK_SELF_MSG_COMMAND_COUNT_BUFFER_INJECT  ((sizeof(AppTaskSelfMsgProcFuncPtrArrayBufferInject))/(sizeof(AppTaskSelfMsgProcFuncPtrArrayBufferInject[0])))


//模块自身转发消息处理任务
static void AppTaskFuncBufferInject_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_BUFFER_INJECT* cmdResultPtr = (DATA_IPC_BUFFER_INJECT*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_COMMAND_COUNT_BUFFER_INJECT)
	{
       AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)"Unsupport",selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,
                        selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcFuncPtrArrayBufferInject[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                        AppTaskSelfMsgProcFuncPtrArrayBufferInject[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    AppShowEndMsg((uint8_t*)AppBufferInjectDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//缓冲液泵阀复位
static LH_ERR AppTaskFuncBufferInject_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplBufferSolutionInjectReset();
    return actionResult;
}

//缓冲液灌注
static LH_ERR AppTaskFuncBufferInject_Self_Proc_Prime(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    BOTTLE_BUFFER_SOLUTION bottltSelect = (BOTTLE_BUFFER_SOLUTION)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(bottltSelect),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplBufferSolutionInjectPrime(bottltSelect);
    return actionResult;
}

//缓冲液注液
static LH_ERR AppTaskFuncBufferInject_Self_Proc_Inject(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    BOTTLE_BUFFER_SOLUTION bottltSelect = (BOTTLE_BUFFER_SOLUTION)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(bottltSelect),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplBufferSolutionInjectInject(bottltSelect);
    return actionResult;
}

//缓冲液清洗维护
static LH_ERR AppTaskFuncBufferInject_Self_Proc_MaintainPrime(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    BOTTLE_BUFFER_SOLUTION bottltSelect = (BOTTLE_BUFFER_SOLUTION)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBufferInjectDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(bottltSelect),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplBufferSolutionInjectMaintainPrime(bottltSelect);
    return actionResult;
}



















