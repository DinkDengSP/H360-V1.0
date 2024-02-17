/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:08:52
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-08 09:30:54
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceTaskRailComplex.c
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
CPU_STK stackBufferTaskServiceRailComplex[STK_SIZE_TASK_SERVICE_RAIL_COMPLEX]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceRailComplex@".dtcm_ram";

//自动运行程序的执行周期,单位毫秒
#define RAIL_COMPLEX_AUTO_FUNC_RUNNING_PERIOD_MS            200
//系统循环自动程序初始化
static void ServiceTaskFuncRailComplex_Auto_Init(void);
#if(RAIL_COMPLEX_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void ServiceTaskFuncRailComplex_Auto_Proc(void);
#endif


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncRailComplex_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr);
//2. 来自自身的消息
static void ServiceTaskFuncRailComplex_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceRailComplex(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    ServiceTaskFuncRailComplex_Auto_Init();
    #if(RAIL_COMPLEX_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(RAIL_COMPLEX_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= RAIL_COMPLEX_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了RAIL_COMPLEX_AUTO_FUNC_RUNNING_PERIOD_MS个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			ServiceTaskFuncRailComplex_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)RAIL_COMPLEX_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                ServiceTaskFuncRailComplex_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncRailComplex_SelfMsg_Proc(selfCmdDataPtr);
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
static void ServiceTaskFuncRailComplex_Auto_Init(void)
{
    ServiceImplRailComplex_EventInit();
}
#if(RAIL_COMPLEX_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统循环运行程序,处理自动化任务
static void ServiceTaskFuncRailComplex_Auto_Proc(void)
{
    ServiceImplRailComplex_EventRun(RAIL_COMPLEX_AUTO_FUNC_RUNNING_PERIOD_MS);
}
#endif




/*************************************************1. 来自CAN1的消息******************************************/
//轨道复合集成复位
void ServiceTaskFuncRailComplex_Can1Msg_Proc_MachineReset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道复合设置模块数量
void ServiceTaskFuncRailComplex_Can1Msg_Proc_SetModuleCount(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道定位自动校准
void ServiceTaskFuncRailComplex_Can1Msg_Proc_AutoAdjust(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道开始自动机械疲劳测试
void ServiceTaskFuncRailComplex_Can1Msg_Proc_AutoMechanicalFatigueTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT RailComplexCan1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_DEBUG_RAIL_COMPLEX_RESET          ,ServiceTaskFuncRailComplex_Can1Msg_Proc_MachineReset       ,"MachineReset"     },
    {CAN1_CMD_DEBUG_RAIL_COMPLEX_SET_MODULE     ,ServiceTaskFuncRailComplex_Can1Msg_Proc_SetModuleCount     ,"SetModuleCount"   },
    {CAN1_CMD_DEBUG_RAIL_COMPLEX_SELF_ADJUST    ,ServiceTaskFuncRailComplex_Can1Msg_Proc_AutoAdjust         ,"AutoAdjust"       },
    {CAN1_CMD_DEBUG_RAIL_COMPLEX_AUTO_MECHANICAL_FATIGURE_TEST    ,ServiceTaskFuncRailComplex_Can1Msg_Proc_AutoMechanicalFatigueTest         ,"AutoMechanicalFatigueTest"       },
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_RAIL_COMPLEX_CMD_COUNT        (sizeof(RailComplexCan1CommandProcFuncPtrArray)/sizeof(RailComplexCan1CommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncRailComplex_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_RAIL_COMPLEX_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == RailComplexCan1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(RailComplexCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                RailComplexCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,RailComplexCan1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//轨道复合集成复位
void ServiceTaskFuncRailComplex_Can1Msg_Proc_MachineReset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailComplexDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailComplexResetAll(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailComplexDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_COMPLEX_RESET,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道复合设置模块数量
void ServiceTaskFuncRailComplex_Can1Msg_Proc_SetModuleCount(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //设置模块数量
    uint8_t moduleCount = can1CommandPtr->recvDataBuffer[0];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailComplexDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailComplexSetModuleCount(moduleCount,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailComplexDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_COMPLEX_SET_MODULE,can1Error,reportLevelResult,actionResult,0,NULL);
}


//轨道定位自动校准
void ServiceTaskFuncRailComplex_Can1Msg_Proc_AutoAdjust(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //设置模块数量
    uint8_t moduleCount = can1CommandPtr->recvDataBuffer[0];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailComplexDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailComplexSelfAutoAdjust(moduleCount,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailComplexDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_COMPLEX_SELF_ADJUST,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道开始自动机械疲劳测试
void ServiceTaskFuncRailComplex_Can1Msg_Proc_AutoMechanicalFatigueTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //设置测试次数
    uint32_t testCountSet = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->recvDataBuffer,0);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailComplexDebugStartMsg,detailDebugMsgPtr,testCountSet,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailComplexAutoMechanicalFatigueTest(testCountSet,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailComplexDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_COMPLEX_AUTO_MECHANICAL_FATIGURE_TEST,can1Error,reportLevelResult,actionResult,0,NULL);
}

/*************************************************2. 来自自身的消息******************************************/
//轨道运转复位
static LH_ERR RailComplexSelfCommandProcFunc_ResetAll(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道设置模块数量
static LH_ERR RailComplexSelfCommandProcFunc_SetModuleCount(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//双轨道同时推进直到一次触发
static LH_ERR RailComplexSelfCommandProcFunc_PushMultiWhileTrig(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道依靠传感器自动校准
static LH_ERR RailComplexSelfCommandProcFunc_SelfAdjust(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);


//回调函数数组
const SYSTEM_TASK_SELF_MSG_PROC_UNIT RailComplexSelfCommandProcFuncPtrArray[] = {
    {RailComplexSelfCommandProcFunc_ResetAll            , "ResetAll"            },
    {RailComplexSelfCommandProcFunc_SetModuleCount      , "SetModuleCount"      },
    {RailComplexSelfCommandProcFunc_PushMultiWhileTrig  , "PushMultiWhileTrig"  },
    {RailComplexSelfCommandProcFunc_SelfAdjust          , "SelfAdjust"          },
};
//自身支持指令数量
#define TASK_SELF_MSG_RAIL_COMPLEX_CMD_COUNT  ((sizeof(RailComplexSelfCommandProcFuncPtrArray))/(sizeof(RailComplexSelfCommandProcFuncPtrArray[0])))

//匹配发送过来的指令序号
static void ServiceTaskFuncRailComplex_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_RAIL_COMPLEX* selfCmdResultDataPtr = (IPC_DATA_RAIL_COMPLEX*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (selfCmdResultDataPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	selfCmdResultDataPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_RAIL_COMPLEX_CMD_COUNT)
	{
        SystemDebugShowMsgStart((uint8_t*)RailComplexDebugStartMsg,(uint8_t*)"unsupport",
                                selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                                selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                                );
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = RailComplexSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,&(selfCmdResultDataPtr->can1ErrorCode),&(selfCmdResultDataPtr->can1ReportErrorLevel),
                                                                                            RailComplexSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    SystemDebugShowMsgEnd((uint8_t*)RailComplexDebugEndMsg,actionResult);
	//处理完成,设置状态
	selfCmdResultDataPtr->state = STATE_IPC_CMD_COMPLETE;
	selfCmdResultDataPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(selfCmdResultDataPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//轨道运转复位
static LH_ERR RailComplexSelfCommandProcFunc_ResetAll(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailComplexDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailComplexResetAll(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道设置模块数量
static LH_ERR RailComplexSelfCommandProcFunc_SetModuleCount(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    uint8_t moduleCount = (uint8_t)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailComplexDebugStartMsg,detailDebugMsgPtr,moduleCount,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailComplexSetModuleCount(moduleCount,can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}

//双轨道同时推进直到一次触发
static LH_ERR RailComplexSelfCommandProcFunc_PushMultiWhileTrig(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    uint8_t retryMax = (uint8_t)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailComplexDebugStartMsg,detailDebugMsgPtr,retryMax,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailComplexPushMultiWhileTrig(retryMax,can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}

//轨道依靠传感器自动校准
static LH_ERR RailComplexSelfCommandProcFunc_SelfAdjust(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailComplexDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //从系统状态中获取校准讯息
    uint8_t moduleCount = ServiceStateRailComplexReadModuleCount();
    //调用函数实现
    actionResult = ServiceImplRailComplexSelfAutoAdjust(moduleCount,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}
