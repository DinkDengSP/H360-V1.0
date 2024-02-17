#include "AppTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferAppTaskInOut[STK_SIZE_APP_TASK_INOUT]@".ccram";;
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskInOut@".ccram";

//初始化输入输出的状态
static void AppTaskFuncInOut_EventInit(void);

//模块定时执行任务
static void AppTaskFuncInOut_Auto_Proc(void);
//模块CAN信息处理任务
static void AppTaskFuncInOut_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncInOut_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncInOut(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //延时等待电平稳定
    CoreDelayMs(1000);
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    //初始化输入输出的状态
    AppTaskFuncInOut_EventInit();
    while(1)
    {
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= 1000)
		{
			//上次发送时间和本次发送时间差了1000个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncInOut_Auto_Proc();
		}

        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}

        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)1000,
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
                AppTaskFuncInOut_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncInOut_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}



/*********************************************自身定时任务处理*********************************************/

//初始化输入输出的状态
static void AppTaskFuncInOut_EventInit(void)
{
    //状态初始化
    AppImplInOutInit();
}

//模块定时执行任务
static void AppTaskFuncInOut_Auto_Proc(void)
{
    //检查设置变化
    AppImplInOutCheckSystemSetting();
    //检查输入输出变化
    AppImplInOutReflushSensorAndPort();
}



/************************************************CAN总线接收到的消息的处理******************************************************/
//基础任务结构,任务实际情况未知
static LH_ERR AppTaskFuncInOut_CanMsg_Proc_Undefine(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcInOutSetting[] = {
    /****前一个是处理函数***************************************参数的最小长度*****/
    {AppTaskFuncInOut_CanMsg_Proc_Undefine         ,           2           ,"" }
};
#define TASK_CAN_MSG_INOUT_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcInOutSetting))/(sizeof(AppTaskCanMsgProcInOutSetting[0])))

//模块CAN信息处理任务
static void AppTaskFuncInOut_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_INOUT_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcInOutSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcInOutSetting[commandValue].funcPtr(canMsgCachePtr,AppTaskCanMsgProcInOutSetting[commandValue].debugDetailMsg);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//基础任务结构,任务实际情况未知
static LH_ERR AppTaskFuncInOut_CanMsg_Proc_Undefine(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //读取序号和想要设置的状态
    uint8_t param1 = cachePtrLocal->recvCacheArray[4];
    uint8_t param2 = cachePtrLocal->recvCacheArray[5];
    //写入数据
    AppImplInOutTestSupport(param1,param2);
    //返回结果
    return LH_ERR_NONE;
}

/************************************************自身分层API调用的消息的处理**********************************/
//基础任务结构,实际实现未知
static LH_ERR AppTaskFuncInOut_Self_Proc_Undefine(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcInOutSetting[] = {
    AppTaskFuncInOut_Self_Proc_Undefine,
};

//自身支持指令数量
#define TASK_SELF_MSG_INOUT_COMMAND_COUNT  ((sizeof(AppTaskSelfMsgProcInOutSetting))/(sizeof(AppTaskSelfMsgProcInOutSetting[0])))


//模块自身转发消息处理任务
static void AppTaskFuncInOut_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_OTHER* cmdResultPtr = (DATA_IPC_OTHER*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_INOUT_COMMAND_COUNT)
	{
       AppShowStartMsg((uint8_t*)AppInOutDebugStartMsg,(uint8_t*)"Unsupport",
                    selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                    selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                    );
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcInOutSetting[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                         AppTaskSelfMsgProcInOutSetting[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    AppShowEndMsg((uint8_t*)AppInOutDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}


//基础任务结构,实际实现未知
static LH_ERR AppTaskFuncInOut_Self_Proc_Undefine(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppInOutDebugStartMsg,(uint8_t*)"Undefine",selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    //DATA_IPC_OTHER* cmdResultPtr = (DATA_IPC_OTHER*)(selfCmdPtr->commandDataSpecialChannel);
    int32_t param1 = (int32_t)(selfCmdPtr->commandParam1);
    int32_t param2 = (int32_t)(selfCmdPtr->commandParam2);
    //调用函数
    AppImplInOutTestSupport(param1,param2);
    return errCode;
}
