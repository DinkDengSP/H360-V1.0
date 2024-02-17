/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:06:44
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-14 15:13:36
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceTaskBarScan.c
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
CPU_STK stackBufferTaskServiceBarScan[STK_SIZE_TASK_SERVICE_BAR_SCAN]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceBarScan@".dtcm_ram";

//自动运行程序的执行周期,单位毫秒
#define BAR_SCAN_AUTO_FUNC_RUNNING_PERIOD_MS            0
//系统循环自动程序初始化
static void ServiceTaskFuncBarScan_Auto_Init(void);
#if(BAR_SCAN_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void ServiceTaskFuncBarScan_Auto_Proc(void);
#endif


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncBarScan_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr);
//2. 来自自身的消息
static void ServiceTaskFuncBarScan_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceBarScan(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    ServiceTaskFuncBarScan_Auto_Init();
    #if(BAR_SCAN_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(BAR_SCAN_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= BAR_SCAN_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了BAR_SCAN_AUTO_FUNC_RUNNING_PERIOD_MS个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			ServiceTaskFuncBarScan_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)BAR_SCAN_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                ServiceTaskFuncBarScan_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncBarScan_SelfMsg_Proc(selfCmdDataPtr);
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
static void ServiceTaskFuncBarScan_Auto_Init(void)
{
    ServiceImplBarScan_EventInit();
}
#if(BAR_SCAN_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统循环运行程序,处理自动化任务
static void ServiceTaskFuncBarScan_Auto_Proc(void)
{
    ServiceImplBarScan_EventRun(BAR_SCAN_AUTO_FUNC_RUNNING_PERIOD_MS);
}
#endif



/*************************************************1. 来自CAN1的消息******************************************/
//条码枪打开
void ServiceTaskFuncBarScan_Can1Msg_Proc_Open(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//条码枪关闭
void ServiceTaskFuncBarScan_Can1Msg_Proc_Close(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//条码枪触发一次
void ServiceTaskFuncBarScan_Can1Msg_Proc_TrigOnce(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT BarScanCan1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_DEBUG_RAIL_BARSCAN_OPEN        ,ServiceTaskFuncBarScan_Can1Msg_Proc_Open       ,"Open"         },
    {CAN1_CMD_DEBUG_RAIL_BARSCAN_CLOSE       ,ServiceTaskFuncBarScan_Can1Msg_Proc_Close      ,"Close"        },
    {CAN1_CMD_DEBUG_RAIL_BARSCAN_TRIG_ONCE   ,ServiceTaskFuncBarScan_Can1Msg_Proc_TrigOnce   ,"TrigOnce"     },
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_BAR_SCAN_CMD_COUNT        (sizeof(BarScanCan1CommandProcFuncPtrArray)/sizeof(BarScanCan1CommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncBarScan_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_BAR_SCAN_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == BarScanCan1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(BarScanCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                BarScanCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,BarScanCan1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//条码枪打开
void ServiceTaskFuncBarScan_Can1Msg_Proc_Open(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    BARSCAN_OPEN_MODE barscanOpenMode = (BARSCAN_OPEN_MODE)(can1CommandPtr->recvDataBuffer[0]);
    if(barscanOpenMode > BARSCAN_OPEN_MODE_CONTINUE)barscanOpenMode = BARSCAN_OPEN_MODE_CONTINUE;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)BarScanDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailBarScanOpen(barscanOpenMode,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)BarScanDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_BARSCAN_OPEN,can1Error,reportLevelResult,actionResult,0,NULL);
}

//条码枪关闭
void ServiceTaskFuncBarScan_Can1Msg_Proc_Close(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)BarScanDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailBarScanClose(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)BarScanDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_BARSCAN_CLOSE,can1Error,reportLevelResult,actionResult,0,NULL);
}

//条码枪触发一次
void ServiceTaskFuncBarScan_Can1Msg_Proc_TrigOnce(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)BarScanDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //打开条码枪
    actionResult = ServiceImplRailBarScanOpen(BARSCAN_OPEN_MODE_ONCE,&can1Error,&reportLevelResult);
    //延时等待
    CoreDelayMs(5000);
    //获取条码枪数据
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailBarScanReadData(&can1Error,&reportLevelResult);
    }
    //关闭条码枪
    actionResult = ServiceImplRailBarScanClose(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)BarScanDebugEndMsg,actionResult);
    //该返回的数据返回
    uint16_t barCodeLength = 0;
    barCodeLength = ServiceStateRailBarScanReadDataLength();
    if(barCodeLength > 255)
    {
        barCodeLength = 255;
    }
    //申请返回数据的内存
    uint8_t* respBufferPtr = NULL;
    do
    {
        respBufferPtr = UserMemMalloc(MEM_SRAM,barCodeLength+1);
        if(respBufferPtr == NULL)
        {
            CoreDelayMinTick();
        }
    }while(respBufferPtr == NULL);
    respBufferPtr[0] = (uint16_t)barCodeLength;
    //读取数据
    ServiceStateRailBarScanReadData(barCodeLength,respBufferPtr+1);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_BARSCAN_TRIG_ONCE,can1Error,reportLevelResult,actionResult,barCodeLength+1,respBufferPtr);
    //释放内存
    UserMemFree(MEM_SRAM,respBufferPtr);
}

/*************************************************2. 来自自身的消息******************************************/
//检测条码枪是否存在
static LH_ERR BarScanSelfCommandProcFunc_CheckExist(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//打开条码枪
static LH_ERR BarScanSelfCommandProcFunc_Open(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//关闭条码枪
static LH_ERR BarScanSelfCommandProcFunc_Close(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//读取条码枪数据
static LH_ERR BarScanSelfCommandProcFunc_ReadData(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//从缓冲区读取条码枪数据
static LH_ERR BarScanSelfCommandProcFunc_ReadDataFromBuffer(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//回调函数数组
const SYSTEM_TASK_SELF_MSG_PROC_UNIT BarScanSelfCommandProcFuncPtrArray[] = {
    {BarScanSelfCommandProcFunc_CheckExist      , "CheckExist"  },
    {BarScanSelfCommandProcFunc_Open            , "Open"        },
    {BarScanSelfCommandProcFunc_Close           , "Close"       },
    {BarScanSelfCommandProcFunc_ReadData        , "ReadData"    },
    {BarScanSelfCommandProcFunc_ReadDataFromBuffer        , "ReadDataFromBuffer"    },
};
//自身支持指令数量
#define TASK_SELF_MSG_BAR_SCAN_CMD_COUNT  ((sizeof(BarScanSelfCommandProcFuncPtrArray))/(sizeof(BarScanSelfCommandProcFuncPtrArray[0])))

//匹配发送过来的指令序号
static void ServiceTaskFuncBarScan_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_BAR_SCAN* selfCmdResultDataPtr = (IPC_DATA_BAR_SCAN*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (selfCmdResultDataPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	selfCmdResultDataPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_BAR_SCAN_CMD_COUNT)
	{
        SystemDebugShowMsgStart((uint8_t*)BarScanDebugStartMsg,(uint8_t*)"unsupport",
                                selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                                selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                                );
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = BarScanSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,&(selfCmdResultDataPtr->can1ErrorCode),&(selfCmdResultDataPtr->can1ReportErrorLevel),
                                                                                            BarScanSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    SystemDebugShowMsgEnd((uint8_t*)BarScanDebugEndMsg,actionResult);
	//处理完成,设置状态
	selfCmdResultDataPtr->state = STATE_IPC_CMD_COMPLETE;
	selfCmdResultDataPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(selfCmdResultDataPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//检测条码枪是否存在
static LH_ERR BarScanSelfCommandProcFunc_CheckExist(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)BarScanDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailBarScanCheckExist(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//打开条码枪
static LH_ERR BarScanSelfCommandProcFunc_Open(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    BARSCAN_OPEN_MODE barScanOpenMode = (BARSCAN_OPEN_MODE)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)BarScanDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailBarScanOpen(barScanOpenMode,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//关闭条码枪
static LH_ERR BarScanSelfCommandProcFunc_Close(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)BarScanDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailBarScanClose(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//读取条码枪数据
static LH_ERR BarScanSelfCommandProcFunc_ReadData(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)BarScanDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailBarScanReadData(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//读取条码枪数据
static LH_ERR BarScanSelfCommandProcFunc_ReadDataFromBuffer(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    BARSCAN_INDEX barScanIndex = (BARSCAN_INDEX)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)BarScanDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailBarScanReadDataFromBuffer(barScanIndex,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}
