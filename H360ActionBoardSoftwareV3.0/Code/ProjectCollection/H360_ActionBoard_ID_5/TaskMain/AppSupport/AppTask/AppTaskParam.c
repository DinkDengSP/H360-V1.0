/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:11:13
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-14 16:14:34
**FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppTask\AppTaskParam.c
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
CPU_STK stackBufferAppTaskParam[STK_SIZE_APP_TASK_PARAM]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskParam@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_PARAM_PERIOD        0
//模块定时任务初始化
static void AppTaskFuncParam_Auto_Init(void);
#if(TIME_MS_AUTO_TASK_PARAM_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncParam_Auto_Proc(void);
#endif

//模块CAN信息处理任务
static void AppTaskFuncParam_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncParam_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncParam(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //模块定时任务初始化
    AppTaskFuncParam_Auto_Init();
    #if(TIME_MS_AUTO_TASK_PARAM_PERIOD != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_PARAM_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_PARAM_PERIOD)
		{
			//上次发送时间和本次发送时间差了1000个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncParam_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_PARAM_PERIOD,
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
                AppTaskFuncParam_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncParam_Self_Proc(selfCmdPtr);
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
static void AppTaskFuncParam_Auto_Init(void)
{

}
#if(TIME_MS_AUTO_TASK_PARAM_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncParam_Auto_Proc(void)
{
    return;
}
#endif


/************************************************CAN总线接收到的消息的处理******************************************************/
//读指定序号参数
static LH_ERR AppTaskFuncParam_CanMsg_Proc_Read(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//写入指定序号参数
static LH_ERR AppTaskFuncParam_CanMsg_Proc_Write(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//初始化板上参数为默认值
static LH_ERR AppTaskFuncParam_CanMsg_Proc_SetDefault(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcFuncPtrArrayParam[] = {
    /****前一个是处理函数***************************************参数的最小长度*****/
    {AppTaskFuncParam_CanMsg_Proc_Read          ,           2           ,"Read"         },//读指定序号参数
    {AppTaskFuncParam_CanMsg_Proc_Write         ,           6           ,"Write"        },//写入指定序号参数
    {AppTaskFuncParam_CanMsg_Proc_SetDefault    ,           0           ,"SetDefault"   },//初始化板上参数为默认值
};
#define TASK_CAN_MSG_COMMAND_COUNT_PARAM  ((sizeof(AppTaskCanMsgProcFuncPtrArrayParam))/(sizeof(AppTaskCanMsgProcFuncPtrArrayParam[0])))

//模块CAN信息处理任务
static void AppTaskFuncParam_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_COMMAND_COUNT_PARAM)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }
    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcFuncPtrArrayParam[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcFuncPtrArrayParam[commandValue].funcPtr(canMsgCachePtr,
                                                                            AppTaskCanMsgProcFuncPtrArrayParam[commandValue].debugDetailMsg);
    //打印结果
    AppShowEndMsg((uint8_t*)AppParamDebugEndMsg,errorCode);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//读指定序号参数
static LH_ERR AppTaskFuncParam_CanMsg_Proc_Read(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_CAN_SUB_BOARD5_PARAM_MAIN mainIndex = (INDEX_CAN_SUB_BOARD5_PARAM_MAIN)(cachePtrLocal->recvCacheArray[4]);
    uint8_t subIndex = (uint8_t)(cachePtrLocal->recvCacheArray[5]);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppParamDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)mainIndex),((int32_t)subIndex),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用读参数
    int32_t paramRead;
    actionResult = AppImplParamRead(mainIndex,subIndex,&paramRead);
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[4] = {0};
        CanBaseWriteInt32DataToBuffer(dataBufferSend,0,paramRead);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_PARAM,dataBufferSend,4);
    }
    //返回结果
    return actionResult;
}

//写入指定序号参数
static LH_ERR AppTaskFuncParam_CanMsg_Proc_Write(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_CAN_SUB_BOARD5_PARAM_MAIN mainIndex = (INDEX_CAN_SUB_BOARD5_PARAM_MAIN)(cachePtrLocal->recvCacheArray[4]);
    uint8_t subIndex = (uint8_t)(cachePtrLocal->recvCacheArray[5]);
    int32_t paramWrite = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,6);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppParamDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(mainIndex),((int32_t)subIndex),((int32_t)paramWrite),((int32_t)0),((int32_t)0));
    //调用写参数
    actionResult = AppImplParamWrite(mainIndex,subIndex,paramWrite);
    //返回结果
    return actionResult;
}

//初始化参数位默认值
static LH_ERR AppTaskFuncParam_CanMsg_Proc_SetDefault(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppParamDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用恢复初始值
    actionResult = AppImplParamSetDefault();
    //返回结果
    return actionResult;
}






/************************************************自身分层API调用的消息的处理**********************************/
//初始化板上参数为默认值
static LH_ERR AppTaskFuncParam_Self_Proc_SetDefault(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcFuncPtrArrayParam[] = {
    {AppTaskFuncParam_Self_Proc_SetDefault    ,                      "SetDefault" },//初始化板上参数为默认值
};

//自身支持指令数量
#define TASK_SELF_MSG_COMMAND_COUNT_PARAM  ((sizeof(AppTaskSelfMsgProcFuncPtrArrayParam))/(sizeof(AppTaskSelfMsgProcFuncPtrArrayParam[0])))


//模块自身转发消息处理任务
static void AppTaskFuncParam_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_APP_PARAM* cmdResultPtr = (DATA_IPC_APP_PARAM*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_COMMAND_COUNT_PARAM)
	{
       AppShowStartMsg((uint8_t*)AppParamDebugStartMsg,(uint8_t*)"Unsupport",selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,
                        selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcFuncPtrArrayParam[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,AppTaskSelfMsgProcFuncPtrArrayParam[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    AppShowEndMsg((uint8_t*)AppParamDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//初始化板上参数为默认值
static LH_ERR AppTaskFuncParam_Self_Proc_SetDefault(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppParamDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用复位函数
    actionResult = AppImplParamSetDefault();
    //返回结果
    return actionResult;
}
