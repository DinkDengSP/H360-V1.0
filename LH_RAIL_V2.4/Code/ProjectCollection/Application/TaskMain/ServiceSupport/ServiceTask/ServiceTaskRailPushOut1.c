/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:03:10
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-07 15:30:45
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceTaskRailPushOut1.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskServiceRailPushOut1[STK_SIZE_TASK_SERVICE_RAIL_PUSH_OUT1]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceRailPushOut1@".dtcm_ram";

//自动运行程序的执行周期,单位毫秒
#define RAIL_PUSH_OUT1_AUTO_FUNC_RUNNING_PERIOD_MS            200
//系统循环自动程序初始化
static void ServiceTaskFuncRailPushOut1_Auto_Init(void);
#if(RAIL_PUSH_OUT1_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void ServiceTaskFuncRailPushOut1_Auto_Proc(void);
#endif


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncRailPushOut1_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr);
//2. 来自自身的消息
static void ServiceTaskFuncRailPushOut1_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceRailPushOut1(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    ServiceTaskFuncRailPushOut1_Auto_Init();
    #if(RAIL_PUSH_OUT1_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(RAIL_PUSH_OUT1_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= RAIL_PUSH_OUT1_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了RAIL_PUSH_OUT1_AUTO_FUNC_RUNNING_PERIOD_MS个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			ServiceTaskFuncRailPushOut1_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)RAIL_PUSH_OUT1_AUTO_FUNC_RUNNING_PERIOD_MS,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(SYSTEM_CMD_SRC_CAN1 == taskCommandPtr->cmdSrc)
            {
                //从CAN1传过来的指令
                CAN1_CMD_RECV* can1RecvDatPtr = (CAN1_CMD_RECV*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncRailPushOut1_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncRailPushOut1_SelfMsg_Proc(selfCmdDataPtr);
                //处理完成
                UserMemFree(MEM_DTCM,selfCmdDataPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            taskCommandPtr->cmdDataPtr = NULL;
            UserMemFree(MEM_DTCM,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}

/************************************************自动运行程序************************************************/
//系统循环自动程序初始化
static void ServiceTaskFuncRailPushOut1_Auto_Init(void)
{
    ServiceImplRailPushOut1_EventInit();
}
#if(RAIL_PUSH_OUT1_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统循环运行程序,处理自动化任务
static void ServiceTaskFuncRailPushOut1_Auto_Proc(void)
{
    ServiceImplRailPushOut1_EventRun(RAIL_PUSH_OUT1_AUTO_FUNC_RUNNING_PERIOD_MS);
}
#endif



/*************************************************1. 来自CAN1的消息******************************************/
//出样1复位
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_Reset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//出样1走步数
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_RunSteps(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//出样1推进一次
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_PushOnce(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//出样1读取输入
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_InputRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//出样1写入输出
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_OutputWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT RailPushOut1Can1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_MOTOR_RESET      ,ServiceTaskFuncRailPushOut1_Can1Msg_Proc_Reset         ,"Reset"        },
    {CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_MOTOR_RUN_STEPS  ,ServiceTaskFuncRailPushOut1_Can1Msg_Proc_RunSteps      ,"RunSteps"     },
    {CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_PUSH_ONCE        ,ServiceTaskFuncRailPushOut1_Can1Msg_Proc_PushOnce      ,"PushOnce"     },
    {CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_INPUT_READ       ,ServiceTaskFuncRailPushOut1_Can1Msg_Proc_InputRead     ,"InputRead"    },
    {CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_OUTPUT_WRITE     ,ServiceTaskFuncRailPushOut1_Can1Msg_Proc_OutputWrite   ,"OutputWrite"  },
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_RAIL_PUSH_OUT1_CMD_COUNT        (sizeof(RailPushOut1Can1CommandProcFuncPtrArray)/sizeof(RailPushOut1Can1CommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncRailPushOut1_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_RAIL_PUSH_OUT1_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == RailPushOut1Can1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(RailPushOut1Can1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                RailPushOut1Can1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,RailPushOut1Can1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//出样1复位
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_Reset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailPushOut1Reset(RAIL_RESET_CORRECT_ON,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailPushOut1DebugEndMsg,actionResult);
    //该返回的数据返回
    asm("nop");
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_MOTOR_RESET,can1Error,reportLevelResult,actionResult,0,NULL);
}

//出样1走步数
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_RunSteps(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //运行方向
    uint8_t dir = can1CommandPtr->recvDataBuffer[0];
    //运动步数
    uint32_t steps = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->recvDataBuffer,1);
    //运动完坐标
    int32_t posAfterRun = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,dir,steps,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailPushOut1MotorRunSteps(dir,steps,&posAfterRun,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailPushOut1DebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[4] = {0};
    Can1ProcUtilConvertInt32ToArray(returnData,0,posAfterRun);
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_MOTOR_RUN_STEPS,can1Error,reportLevelResult,actionResult,4,returnData);
}

//出样1推进一次
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_PushOnce(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailPushOut1PushOnceWithReturnZero(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailPushOut1DebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_PUSH_ONCE,can1Error,reportLevelResult,actionResult,0,NULL);
}

//出样1读取输入
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_InputRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //输入序号
    uint8_t pinIndex = can1CommandPtr->recvDataBuffer[0];
    //读取到的输入值
    uint8_t pinValue = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,pinIndex,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailPushOut1InputRead(pinIndex,&pinValue,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailPushOut1DebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[2] = {0};
    returnData[0] = pinIndex;
    returnData[1] = pinValue;
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_INPUT_READ,can1Error,reportLevelResult,actionResult,2,returnData);
}

//出样1写入输出
void ServiceTaskFuncRailPushOut1_Can1Msg_Proc_OutputWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //输出序号
    uint8_t pinIndex = can1CommandPtr->recvDataBuffer[0];
    //输出值
    uint8_t pinValue = can1CommandPtr->recvDataBuffer[1];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,pinIndex,pinValue,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailPushOut1OutPutWrite(pinIndex,pinValue,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailPushOut1DebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[1] = {0};
    returnData[0] = pinIndex;
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_PUSH_OUT1_OUTPUT_WRITE,can1Error,reportLevelResult,actionResult,1,returnData);
}


/*************************************************2. 来自自身的消息******************************************/
//出样1复位
static LH_ERR RailPushOut1SelfCommandProcFunc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//出样1推进一次然后回零
static LH_ERR RailPushOut1SelfCommandProcFunc_PushOnceWithReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//出样1推进一次不回零
static LH_ERR RailPushOut1SelfCommandProcFunc_PushOnceWithOutReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//出样1回零
static LH_ERR RailPushOut1SelfCommandProcFunc_ReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//出样1获取当前推进到第几次
static LH_ERR RailPushOut1SelfCommandProcFunc_GetPushIndex(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//出样1推架子到顶端
static LH_ERR RailPushOut1SelfCommandProcFunc_PushMax(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//出样1读取当前传感器状态
static LH_ERR RailPushOut1SelfCommandProcFunc_GetSensorState(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//出样1状态控制
static LH_ERR RailPushOut1SelfCommandProcFunc_SetState(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//回调函数数组
const SYSTEM_TASK_SELF_MSG_PROC_UNIT RailPushOut1SelfCommandProcFuncPtrArray[] = {
    {RailPushOut1SelfCommandProcFunc_Reset                          , "Reset"                       },
    {RailPushOut1SelfCommandProcFunc_PushOnceWithReturnZero         , "PushOnceWithReturnZero"      },
    {RailPushOut1SelfCommandProcFunc_PushOnceWithOutReturnZero      , "PushOnceWithOutReturnZero"   },
    {RailPushOut1SelfCommandProcFunc_ReturnZero                     , "ReturnZero"                  },
    {RailPushOut1SelfCommandProcFunc_GetPushIndex                   , "GetPushIndex"                },
    {RailPushOut1SelfCommandProcFunc_PushMax                        , "PushMax"                     },
    {RailPushOut1SelfCommandProcFunc_GetSensorState                 , "GetSensorState"              },
    {RailPushOut1SelfCommandProcFunc_SetState                       , "SetState"                    },
};
//自身支持指令数量
#define TASK_SELF_MSG_RAIL_PUSH_OUT1_CMD_COUNT  ((sizeof(RailPushOut1SelfCommandProcFuncPtrArray))/(sizeof(RailPushOut1SelfCommandProcFuncPtrArray[0])))

//匹配发送过来的指令序号
static void ServiceTaskFuncRailPushOut1_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_RAIL_PUSH_OUT1* selfCmdResultDataPtr = (IPC_DATA_RAIL_PUSH_OUT1*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (selfCmdResultDataPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	selfCmdResultDataPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_RAIL_PUSH_OUT1_CMD_COUNT)
	{
        SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,(uint8_t*)"unsupport",
                                selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                                selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                                );
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = RailPushOut1SelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,&(selfCmdResultDataPtr->can1ErrorCode),&(selfCmdResultDataPtr->can1ReportErrorLevel),
                                                                                            RailPushOut1SelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    SystemDebugShowMsgEnd((uint8_t*)RailPushOut1DebugEndMsg,actionResult);
	//处理完成,设置状态
	selfCmdResultDataPtr->state = STATE_IPC_CMD_COMPLETE;
	selfCmdResultDataPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(selfCmdResultDataPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}


//出样1复位
static LH_ERR RailPushOut1SelfCommandProcFunc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_RESET_CORRECT_FLAG resetFlag = (RAIL_RESET_CORRECT_FLAG)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,resetFlag,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailPushOut1Reset(resetFlag,can1ErrorPtr,reportLevelResultPtr);
    //更新一下传感器状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailPushOut1GetSensorState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//出样1推进一次然后回零
static LH_ERR RailPushOut1SelfCommandProcFunc_PushOnceWithReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailPushOut1PushOnceWithReturnZero(can1ErrorPtr,reportLevelResultPtr);;
    //更新一下传感器状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailPushOut1GetSensorState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//出样1推进一次不回零
static LH_ERR RailPushOut1SelfCommandProcFunc_PushOnceWithOutReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailPushOut1PushOnceWithOutReturnZero(can1ErrorPtr,reportLevelResultPtr);;
    //更新一下传感器状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailPushOut1GetSensorState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//出样1回零
static LH_ERR RailPushOut1SelfCommandProcFunc_ReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailPushOut1ReturnZero(can1ErrorPtr,reportLevelResultPtr);
    //更新一下传感器状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailPushOut1GetSensorState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//出样1获取当前推进到第几次
static LH_ERR RailPushOut1SelfCommandProcFunc_GetPushIndex(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailPushOut1GetPushIndex(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//出样1推架子到顶端
static LH_ERR RailPushOut1SelfCommandProcFunc_PushMax(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailPushOut1PushMax(can1ErrorPtr,reportLevelResultPtr);
    //更新一下传感器状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailPushOut1GetSensorState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//出样1读取当前传感器状态
static LH_ERR RailPushOut1SelfCommandProcFunc_GetSensorState(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailPushOut1GetSensorState(can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}

//出样1状态控制
static LH_ERR RailPushOut1SelfCommandProcFunc_SetState(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    RAIL_OUTPUT_STATE   railState = (RAIL_OUTPUT_STATE)selfCmdPtr->commandParam1;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailPushOut1DebugStartMsg,detailDebugMsgPtr,railState,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailPushOut1SetState(railState,can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}
