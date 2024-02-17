/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-13 14:02:08
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppTask\AppTaskPushOut2.c
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
CPU_STK stackBufferAppTaskPushOut2[STK_SIZE_APP_TASK_PUSH_OUT2]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskPushOut2@".ccram";


//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_PUSH_OUT2_PERIOD        200
//模块定时执行任务初始化
static void AppTaskFuncPushOut2_Auto_Init(void);
//模块定时执行任务
static void AppTaskFuncPushOut2_Auto_Proc(void);

//模块CAN信息处理任务
static void AppTaskFuncPushOut2_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncPushOut2_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncPushOut2(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    //自动执行任务初始化
    AppTaskFuncPushOut2_Auto_Init();
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_PUSH_OUT2_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_PUSH_OUT2_PERIOD)
		{
			//上次发送时间和本次发送时间差了TIME_MS_AUTO_TASK_PUSH_OUT2_PERIOD个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncPushOut2_Auto_Proc();
		}

        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_PUSH_OUT2_PERIOD,
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
                AppTaskFuncPushOut2_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncPushOut2_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}



/*********************************************自身定时任务处理*********************************************/
//模块定时执行任务初始化
static void AppTaskFuncPushOut2_Auto_Init(void)
{
    return;
}
//模块定时执行任务
static void AppTaskFuncPushOut2_Auto_Proc(void)
{
    return;
}



/************************************************CAN总线接收到的消息的处理******************************************************/
//出样2复位
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//出样2推进一次并返回零位
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_PushOnceWithReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//出样2推进一次不返回零位
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_PushOnceNotReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//出样2回零
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_ReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//出样2获取当前处于推进第几次的位置
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_GetIndex(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//出样2推进一次不返回零位,按键专用
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_ButtonPushOnceNotReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcPushOut2Setting[] = {
    {AppTaskFuncPushOut2_CanMsg_Proc_Reset                          ,           1           , "Reset"                       },
    {AppTaskFuncPushOut2_CanMsg_Proc_PushOnceWithReturnZero         ,           0           , "PushOnceWithReturnZero"      },
    {AppTaskFuncPushOut2_CanMsg_Proc_PushOnceNotReturnZero          ,           0           , "PushOnceNotReturnZero"       },
    {AppTaskFuncPushOut2_CanMsg_Proc_ReturnZero                     ,           0           , "ReturnZero"                  },
    {AppTaskFuncPushOut2_CanMsg_Proc_GetIndex                       ,           0           , "GetIndex"                    },
    {AppTaskFuncPushOut2_CanMsg_Proc_ButtonPushOnceNotReturnZero    ,           0           , "ButtonPushOnceNotReturnZero" },
};
#define TASK_CAN_MSG_PUSH_OUT2_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcPushOut2Setting))/(sizeof(AppTaskCanMsgProcPushOut2Setting[0])))

//模块CAN信息处理任务
static void AppTaskFuncPushOut2_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_PUSH_OUT2_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcPushOut2Setting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcPushOut2Setting[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcPushOut2Setting[commandValue].debugDetailMsg);
                    
    //打印结果
    AppShowEndMsg((uint8_t*)AppPushOut2DebugEndMsg,errorCode);

    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//出样2复位
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //复位修正
    RAIL_RESET_CORRECT_FLAG resetCorrectFlag = RAIL_RESET_CORRECT_OFF;
    resetCorrectFlag = ((RAIL_RESET_CORRECT_FLAG)(cachePtrLocal->recvCacheArray[4]));

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplPushOut2Reset(resetCorrectFlag);
    //返回结果
    return errorCode;
}

//出样2推进一次并返回零位
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_PushOnceWithReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplPushOut2PushOnceWithReturnZero();
    //返回结果
    return errorCode;
}

//出样2推进一次不返回零位
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_PushOnceNotReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t pushOutRackIndex = 0;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //写入数据
    errorCode = AppImplPushOut2PushOnceNotReturnZero(&pushOutRackIndex);
    if(errorCode == LH_ERR_NONE)
    {
        //返回当前进样状态
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)pushOutRackIndex;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_PUSHOUT2_INDEX,dataBufferSend,1);
    }
    //返回结果
    return errorCode;
}


//出样2回零
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_ReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //写入数据
    errorCode = AppImplPushOut2ReturnZero();
    //返回结果
    return errorCode;
}

//出样2获取当前处于推进第几次的位置
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_GetIndex(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t pushOutRackIndex = 0;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplPushOut2GetIndex(&pushOutRackIndex);
    if(errorCode == LH_ERR_NONE)
    {
        //返回当前进样状态
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)pushOutRackIndex;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_PUSHOUT2_INDEX,dataBufferSend,1);
    }
    //返回结果
    return errorCode;
}

//出样2推进一次不返回零位
static LH_ERR AppTaskFuncPushOut2_CanMsg_Proc_ButtonPushOnceNotReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t pushOutRackIndex = 0;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //写入数据
    errorCode = AppImplPushOut2ButtonPushOnceNotReturnZero(&pushOutRackIndex);
    if(errorCode == LH_ERR_NONE)
    {
        //返回当前进样状态
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)pushOutRackIndex;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_PUSHOUT2_INDEX,dataBufferSend,1);
    }
    //返回结果
    return errorCode;
}

/************************************************自身分层API调用的消息的处理**********************************/
//出样2复位
static LH_ERR AppTaskFuncPushOut2_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//出样2推进一次并返回零位
static LH_ERR AppTaskFuncPushOut2_Self_Proc_PushOnceWithReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//出样2推进一次不返回零位
static LH_ERR AppTaskFuncPushOut2_Self_Proc_PushOnceNotReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//出样2回零
static LH_ERR AppTaskFuncPushOut2_Self_Proc_ReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//出样2获取当前推进到第几次
static LH_ERR AppTaskFuncPushOut2_Self_Proc_GetIndex(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcUndefineSetting[] = {
    {AppTaskFuncPushOut2_Self_Proc_Reset                    ,"Reset"                   },
    {AppTaskFuncPushOut2_Self_Proc_PushOnceWithReturnZero   ,"PushOnceWithReturnZero"  },
    {AppTaskFuncPushOut2_Self_Proc_PushOnceNotReturnZero    ,"PushOnceNotReturnZero"   },
    {AppTaskFuncPushOut2_Self_Proc_ReturnZero               ,"ReturnZero"              },
    {AppTaskFuncPushOut2_Self_Proc_GetIndex                 ,"GetIndex"                },
};

//自身支持指令数量
#define TASK_SELF_MSG_PUSH_OUT2_COMMAND_COUNT  ((sizeof(AppTaskSelfMsgProcUndefineSetting))/(sizeof(AppTaskSelfMsgProcUndefineSetting[0])))


//模块自身转发消息处理任务
static void AppTaskFuncPushOut2_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_PUSH_OUT2* cmdResultPtr = (DATA_IPC_PUSH_OUT2*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_PUSH_OUT2_COMMAND_COUNT)
	{
       AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)"Unsupport",
                    selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                    selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                    );
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcUndefineSetting[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                            AppTaskSelfMsgProcUndefineSetting[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //打印结果
    AppShowEndMsg((uint8_t*)AppPushOut2DebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//出样2复位
static LH_ERR AppTaskFuncPushOut2_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //复位修正
    RAIL_RESET_CORRECT_FLAG resetCorrectFlag = (RAIL_RESET_CORRECT_FLAG)(selfCmdPtr->commandParam1);
    //调用函数
    errCode = AppImplPushOut2Reset(resetCorrectFlag);
    return errCode;
}

//出样2推进一次并返回零位
static LH_ERR AppTaskFuncPushOut2_Self_Proc_PushOnceWithReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //调用函数
    errCode = AppImplPushOut2PushOnceWithReturnZero();
    return errCode;
}

//出样2推进一次不返回零位
static LH_ERR AppTaskFuncPushOut2_Self_Proc_PushOnceNotReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    uint8_t currentPushOutRackIndex = 0;
    DATA_IPC_PUSH_OUT2* cmdResultPtr = (DATA_IPC_PUSH_OUT2*)(selfCmdPtr->commandDataSpecialChannel);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //调用函数
    errCode = AppImplPushOut2PushOnceNotReturnZero(&currentPushOutRackIndex);
    if(errCode == LH_ERR_NONE)
    {
        cmdResultPtr->currentRackIndex = currentPushOutRackIndex;
    }
    return errCode;
}

//出样2回零
static LH_ERR AppTaskFuncPushOut2_Self_Proc_ReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut2DebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //调用函数
    errCode = AppImplPushOut2ReturnZero();
    return errCode;
}

//出样2获取当前推进到第几次
static LH_ERR AppTaskFuncPushOut2_Self_Proc_GetIndex(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    uint8_t currentPushOutRackIndex = 0;
    DATA_IPC_PUSH_OUT2* cmdResultPtr = (DATA_IPC_PUSH_OUT2*)(selfCmdPtr->commandDataSpecialChannel);
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppPushOut1DebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //调用函数
    errCode = AppImplPushOut2GetIndex(&currentPushOutRackIndex);
    if(errCode == LH_ERR_NONE)
    {
        cmdResultPtr->currentRackIndex = currentPushOutRackIndex;
    }
    return errCode;
}
