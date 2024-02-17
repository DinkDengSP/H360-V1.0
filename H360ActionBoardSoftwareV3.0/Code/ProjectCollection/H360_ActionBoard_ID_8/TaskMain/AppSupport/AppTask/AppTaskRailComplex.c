/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-13 14:00:42
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppTask\AppTaskRailComplex.c
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
CPU_STK stackBufferAppTaskRailComplex[STK_SIZE_APP_TASK_RAIL_COMPLEX]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskRailComplex@".ccram";


//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_RAIL_COMPLEX_PERIOD        200
//模块定时执行任务初始化
static void AppTaskFuncRailComplex_Auto_Init(void);
//模块定时执行任务
static void AppTaskFuncRailComplex_Auto_Proc(void);

//模块CAN信息处理任务
static void AppTaskFuncRailComplex_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncRailComplex_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncRailComplex(void *p_arg)
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
    AppTaskFuncRailComplex_Auto_Init();
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_RAIL_COMPLEX_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_RAIL_COMPLEX_PERIOD)
		{
			//上次发送时间和本次发送时间差了TIME_MS_AUTO_TASK_RAIL_COMPLEX_PERIOD个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncRailComplex_Auto_Proc();
		}

        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_RAIL_COMPLEX_PERIOD,
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
                AppTaskFuncRailComplex_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncRailComplex_Self_Proc(selfCmdPtr);
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
static void AppTaskFuncRailComplex_Auto_Init(void)
{
    return;
}
//模块定时执行任务
static void AppTaskFuncRailComplex_Auto_Proc(void)
{
    return;
}



/************************************************CAN总线接收到的消息的处理******************************************************/
//轨道整体复位
static LH_ERR AppTaskFuncRailComplex_CanMsg_Proc_ResetAll(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//设置轨道模块数量
static LH_ERR AppTaskFuncRailComplex_CanMsg_Proc_SetModuleCount(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道模块初始化检测坐标并初始化坐标
static LH_ERR AppTaskFuncRailComplex_CanMsg_Proc_PushInTrigMultiTogether(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcRailComplexSetting[] = {
    {AppTaskFuncRailComplex_CanMsg_Proc_ResetAll                        ,           0           ,"ResetAll"               },
    {AppTaskFuncRailComplex_CanMsg_Proc_SetModuleCount                  ,           1           ,"SetModuleCount"         },
    {AppTaskFuncRailComplex_CanMsg_Proc_PushInTrigMultiTogether         ,           1           ,"PushInTrigMultiTogether"},
};
#define TASK_CAN_MSG_RAIL_COMPLEX_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcRailComplexSetting))/(sizeof(AppTaskCanMsgProcRailComplexSetting[0])))

//模块CAN信息处理任务
static void AppTaskFuncRailComplex_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_RAIL_COMPLEX_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcRailComplexSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcRailComplexSetting[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcRailComplexSetting[commandValue].debugDetailMsg);

    //打印结果
    AppShowEndMsg((uint8_t*)AppRailComplexDebugEndMsg,errorCode);

    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//轨道整体复位
static LH_ERR AppTaskFuncRailComplex_CanMsg_Proc_ResetAll(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailComplexDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //写入数据
    errorCode = AppImplRailComplexResetAll();
    //返回结果
    return errorCode;
}

//设置轨道模块数量
static LH_ERR AppTaskFuncRailComplex_CanMsg_Proc_SetModuleCount(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //读取序号和想要设置的状态
    uint8_t moduleCount = cachePtrLocal->recvCacheArray[4];

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailComplexDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailComplexSetModuleCount(moduleCount);
    //返回结果
    return errorCode;
}

//轨道模块初始化检测坐标并初始化坐标
static LH_ERR AppTaskFuncRailComplex_CanMsg_Proc_PushInTrigMultiTogether(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //传感器状态
    SENSOR_STATE_PUSHIN_EXIST pushin1ExistSensorState = SENSOR_STATE_PUSHIN_EXIST_NONE;
    //传感器状态
    SENSOR_STATE_PUSHIN_EXIST pushin2ExistSensorState = SENSOR_STATE_PUSHIN_EXIST_NONE;

    //读取重试次数
    uint16_t retryMax = (uint8_t)(cachePtrLocal->recvCacheArray[4]);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailComplexDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    

    //写入数据
    errorCode = AppImplRailComplexPushInTrigMultiTogether(retryMax,&pushin1ExistSensorState,&pushin2ExistSensorState);
    if(errorCode == LH_ERR_NONE)
    {
        //返回当前进样状态
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)pushin1ExistSensorState;
        dataBufferSend[1] = (uint8_t)pushin2ExistSensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_PUSHIN_EXIST,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

/************************************************自身分层API调用的消息的处理**********************************/
//轨道整体复位
static LH_ERR AppTaskFuncRailComplex_Self_Proc_ResetAll(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//设置轨道模块数量
static LH_ERR AppTaskFuncRailComplex_Self_Proc_SetModuleCount(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道模块初始化检测坐标并初始化坐标
static LH_ERR AppTaskFuncRailComplex_Self_Proc_PushInTrigMultiTogether(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcRailComplexSetting[] = {
    {AppTaskFuncRailComplex_Self_Proc_ResetAll                  ,"ResetAll"                 },
    {AppTaskFuncRailComplex_Self_Proc_SetModuleCount            ,"SetModuleCount"           },
    {AppTaskFuncRailComplex_Self_Proc_PushInTrigMultiTogether   ,"PushInTrigMultiTogether"  },
};

//自身支持指令数量
#define TASK_SELF_MSG_RAIL_COMPLEX_COMMAND_COUNT  ((sizeof(AppTaskSelfMsgProcRailComplexSetting))/(sizeof(AppTaskSelfMsgProcRailComplexSetting[0])))


//模块自身转发消息处理任务
static void AppTaskFuncRailComplex_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_RAIL_COMPLEX* cmdResultPtr = (DATA_IPC_RAIL_COMPLEX*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_RAIL_COMPLEX_COMMAND_COUNT)
	{
       AppShowStartMsg((uint8_t*)AppRailComplexDebugStartMsg,(uint8_t*)"Unsupport",
                    selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                    selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                    );
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcRailComplexSetting[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                            AppTaskSelfMsgProcRailComplexSetting[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //打印结果
    AppShowEndMsg((uint8_t*)AppRailComplexDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//轨道整体复位
static LH_ERR AppTaskFuncRailComplex_Self_Proc_ResetAll(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailComplexDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //调用函数
    errCode = AppImplRailComplexResetAll();
    return errCode;
}

//设置轨道模块数量
static LH_ERR AppTaskFuncRailComplex_Self_Proc_SetModuleCount(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailComplexDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    uint8_t moduleCount = (uint8_t)(selfCmdPtr->commandParam1);
    //调用函数
    errCode = AppImplRailComplexSetModuleCount(moduleCount);
    return errCode;
}

//轨道模块初始化检测坐标并初始化坐标
static LH_ERR AppTaskFuncRailComplex_Self_Proc_PushInTrigMultiTogether(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailComplexDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    uint8_t retryMax = (uint8_t)(selfCmdPtr->commandParam1);
    //传感器状态
    SENSOR_STATE_PUSHIN_EXIST pushin1ExistSensorState = SENSOR_STATE_PUSHIN_EXIST_NONE;
    //传感器状态
    SENSOR_STATE_PUSHIN_EXIST pushin2ExistSensorState = SENSOR_STATE_PUSHIN_EXIST_NONE;
    //状态
    DATA_IPC_RAIL_COMPLEX* cmdResultPtr = (DATA_IPC_RAIL_COMPLEX*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailComplexPushInTrigMultiTogether(retryMax,&pushin1ExistSensorState,&pushin2ExistSensorState);
    if(errCode == LH_ERR_NONE)
    {
        cmdResultPtr->pushIn1State = pushin1ExistSensorState;
        cmdResultPtr->pushIn2State = pushin1ExistSensorState;
    }
    return errCode;
}


