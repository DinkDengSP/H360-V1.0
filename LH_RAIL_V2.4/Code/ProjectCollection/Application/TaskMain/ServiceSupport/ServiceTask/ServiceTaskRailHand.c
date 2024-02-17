/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:06:16
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-07 15:31:50
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceTaskRailHand.c
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
CPU_STK stackBufferTaskServiceRailHand[STK_SIZE_TASK_SERVICE_RAIL_HAND]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceRailHand@".dtcm_ram";

//自动运行程序的执行周期,单位毫秒
#define RAIL_HAND_AUTO_FUNC_RUNNING_PERIOD_MS            200
//系统循环自动程序初始化
static void ServiceTaskFuncRailHand_Auto_Init(void);
#if(RAIL_HAND_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void ServiceTaskFuncRailHand_Auto_Proc(void);
#endif


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncRailHand_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr);
//2. 来自自身的消息
static void ServiceTaskFuncRailHand_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceRailHand(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    ServiceTaskFuncRailHand_Auto_Init();
    #if(RAIL_HAND_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(RAIL_HAND_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= RAIL_HAND_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了RAIL_HAND_AUTO_FUNC_RUNNING_PERIOD_MS个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			ServiceTaskFuncRailHand_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)RAIL_HAND_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                ServiceTaskFuncRailHand_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncRailHand_SelfMsg_Proc(selfCmdDataPtr);
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
static void ServiceTaskFuncRailHand_Auto_Init(void)
{
    ServiceImplRailHand_EventInit();
}
#if(RAIL_HAND_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统循环运行程序,处理自动化任务
static void ServiceTaskFuncRailHand_Auto_Proc(void)
{
    ServiceImplRailHand_EventRun(RAIL_HAND_AUTO_FUNC_RUNNING_PERIOD_MS);
}
#endif



/*************************************************1. 来自CAN1的消息******************************************/
//轨道机械手复位
void ServiceTaskFuncRailHand_Can1Msg_Proc_ModuleReset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手指定电机复位
void ServiceTaskFuncRailHand_Can1Msg_Proc_StepMotorReset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手指定电机走步数
void ServiceTaskFuncRailHand_Can1Msg_Proc_StepMotorRunSteps(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手读取指定输入
void ServiceTaskFuncRailHand_Can1Msg_Proc_InputRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手写入指定输出
void ServiceTaskFuncRailHand_Can1Msg_Proc_OutputWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手抓取进样1
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchPushIn1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手抓取进样2
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchPushIn2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手抓取急诊位
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchEmergency(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手抓取待测位
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchWaitTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手抓取重测位
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchRepeatTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手抓取测试位
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchTesting(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手放下出样1
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutPushOut1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手放下出样2
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutPushOut2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手放下待测位
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutWaitTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手放下重测位
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutRepeatTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手放下测试位
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutTesting(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手旋转到指定方向
void ServiceTaskFuncRailHand_Can1Msg_Proc_RotateDir(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手夹爪到出样1
void ServiceTaskFuncRailHand_Can1Msg_Proc_ClawReachPushOut1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手夹爪到出样2
void ServiceTaskFuncRailHand_Can1Msg_Proc_ClawReachPushOut2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手收回夹爪
void ServiceTaskFuncRailHand_Can1Msg_Proc_TakeClawBack(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT RailHandCan1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_DEBUG_RAIL_HAND_MODULE_RESET              ,ServiceTaskFuncRailHand_Can1Msg_Proc_ModuleReset        ,"ModuleReset"         },
    {CAN1_CMD_DEBUG_RAIL_HAND_MOTOR_RESET               ,ServiceTaskFuncRailHand_Can1Msg_Proc_StepMotorReset     ,"StepMotorReset"      },
    {CAN1_CMD_DEBUG_RAIL_HAND_MOTOR_RUN_STEPS           ,ServiceTaskFuncRailHand_Can1Msg_Proc_StepMotorRunSteps  ,"StepMotorRunSteps"   },
    {CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_PUSHIN1        ,ServiceTaskFuncRailHand_Can1Msg_Proc_CatchPushIn1       ,"CatchPushIn1"        },
    {CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_PUSHIN2        ,ServiceTaskFuncRailHand_Can1Msg_Proc_CatchPushIn2       ,"CatchPushIn2"        },
    {CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_EMERGENCY      ,ServiceTaskFuncRailHand_Can1Msg_Proc_CatchEmergency     ,"CatchEmergency"      },
    {CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_WAIT_TEST      ,ServiceTaskFuncRailHand_Can1Msg_Proc_CatchWaitTest      ,"CatchWaitTest"       },
    {CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_REPEAT_TEST    ,ServiceTaskFuncRailHand_Can1Msg_Proc_CatchRepeatTest    ,"CatchRepeatTest"     },
    {CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_TESTING        ,ServiceTaskFuncRailHand_Can1Msg_Proc_CatchTesting       ,"CatchTesting"        },
    {CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_PUSHOUT1         ,ServiceTaskFuncRailHand_Can1Msg_Proc_PutPushOut1        ,"PutPushOut1"         },
    {CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_PUSHOUT2         ,ServiceTaskFuncRailHand_Can1Msg_Proc_PutPushOut2        ,"PutPushOut2"         },
    {CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_WAIT_TEST        ,ServiceTaskFuncRailHand_Can1Msg_Proc_PutWaitTest        ,"PutWaitTest"         },
    {CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_REPEAT_TEST      ,ServiceTaskFuncRailHand_Can1Msg_Proc_PutRepeatTest      ,"PutRepeatTest"       },
    {CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_TESTING          ,ServiceTaskFuncRailHand_Can1Msg_Proc_PutTesting         ,"PutTesting"          },
    {CAN1_CMD_DEBUG_RAIL_HAND_ROTATE_DIR                ,ServiceTaskFuncRailHand_Can1Msg_Proc_RotateDir          ,"RotateDir"           },
    {CAN1_CMD_DEBUG_RAIL_HAND_INPUT_READ                ,ServiceTaskFuncRailHand_Can1Msg_Proc_InputRead          ,"InputRead"           },
    {CAN1_CMD_DEBUG_RAIL_HAND_OUTPUT_WRITE              ,ServiceTaskFuncRailHand_Can1Msg_Proc_OutputWrite        ,"OutputWrite"         },
    {CAN1_CMD_DEBUG_RAIL_HAND_HAND_REACH_PUSHOUT1       ,ServiceTaskFuncRailHand_Can1Msg_Proc_ClawReachPushOut1  ,"ClawReachPushOut1"   },
    {CAN1_CMD_DEBUG_RAIL_HAND_HAND_REACH_PUSHOUT2       ,ServiceTaskFuncRailHand_Can1Msg_Proc_ClawReachPushOut2  ,"ClawReachPushOut2"   },
    {CAN1_CMD_DEBUG_RAIL_HAND_TAKE_HAND_BACK            ,ServiceTaskFuncRailHand_Can1Msg_Proc_TakeClawBack       ,"TakeClawBack"        },
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_RAIL_HAND_CMD_COUNT        (sizeof(RailHandCan1CommandProcFuncPtrArray)/sizeof(RailHandCan1CommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncRailHand_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_RAIL_HAND_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == RailHandCan1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(RailHandCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                RailHandCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,RailHandCan1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//轨道机械手复位
void ServiceTaskFuncRailHand_Can1Msg_Proc_ModuleReset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandModuleReset(&can1Error,&reportLevelResult);
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailHandGetState(&can1Error,&reportLevelResult);
    }
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_MODULE_RESET,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手指定电机复位
void ServiceTaskFuncRailHand_Can1Msg_Proc_StepMotorReset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //电机序号
    uint8_t motorIndex = can1CommandPtr->recvDataBuffer[0];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,motorIndex,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandMotorReset(motorIndex,RAIL_RESET_CORRECT_ON,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[1] = {0};
    returnData[0] = motorIndex;
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_MOTOR_RESET,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手指定电机走步数
void ServiceTaskFuncRailHand_Can1Msg_Proc_StepMotorRunSteps(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //电机序号
    uint8_t motorIndex = can1CommandPtr->recvDataBuffer[0];
    //运行方向
    uint8_t dir = can1CommandPtr->recvDataBuffer[1];
    //运动步数
    uint32_t steps = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->recvDataBuffer,2);
    //运动完坐标
    int32_t posAfterRun = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,motorIndex,dir,steps,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandStepMotorRunSteps(motorIndex,dir,steps,&posAfterRun,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[5] = {0};
    returnData[0] = motorIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,1,posAfterRun);
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_MOTOR_RUN_STEPS,can1Error,reportLevelResult,actionResult,5,returnData);
}

//轨道机械手读取指定输入
void ServiceTaskFuncRailHand_Can1Msg_Proc_InputRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,pinIndex,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandInputRead(pinIndex,&pinValue,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[2] = {0};
    returnData[0] = pinIndex;
    returnData[1] = pinValue;
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_INPUT_READ,can1Error,reportLevelResult,actionResult,2,returnData);
}

//轨道机械手写入指定输出
void ServiceTaskFuncRailHand_Can1Msg_Proc_OutputWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,pinIndex,pinValue,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandOutPutWrite(pinIndex,pinValue,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[1] = {0};
    returnData[0] = pinIndex;
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_OUTPUT_WRITE,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手抓取进样1
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchPushIn1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandCatchPushIn1(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_PUSHIN1,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手抓取进样2
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchPushIn2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandCatchPushIn2(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_PUSHIN2,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手抓取急诊位
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchEmergency(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandCatchEmergencyCall(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_EMERGENCY,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手抓取待测位
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchWaitTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandCatchWaitTest(moduleNo,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_WAIT_TEST,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手抓取重测位
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchRepeatTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandCatchRepeatTest(moduleNo,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_REPEAT_TEST,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手抓取测试位
void ServiceTaskFuncRailHand_Can1Msg_Proc_CatchTesting(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandCatchWaitTest(moduleNo,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_CATCH_RACK_TESTING,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手放下出样1
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutPushOut1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandPutPushOut1(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_PUSHOUT1,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手放下出样2
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutPushOut2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandPutPushOut2(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_PUSHOUT2,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手放下待测位
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutWaitTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandPutWaitTest(moduleNo,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_WAIT_TEST,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手放下重测位
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutRepeatTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandPutRepeatTest(moduleNo,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_REPEAT_TEST,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手放下测试位
void ServiceTaskFuncRailHand_Can1Msg_Proc_PutTesting(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandPutWaitTest(moduleNo,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_PUT_RACK_TESTING,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手旋转到指定方向
void ServiceTaskFuncRailHand_Can1Msg_Proc_RotateDir(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    HAND_DIR targetDir = (HAND_DIR)(can1CommandPtr->recvDataBuffer[0]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,targetDir,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandRotate2SpecialDir(targetDir,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_ROTATE_DIR,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手夹爪到出样1
void ServiceTaskFuncRailHand_Can1Msg_Proc_ClawReachPushOut1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandClawReachPushOut1(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_HAND_REACH_PUSHOUT1,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手夹爪到出样2
void ServiceTaskFuncRailHand_Can1Msg_Proc_ClawReachPushOut2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandClawReachPushOut2(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_HAND_REACH_PUSHOUT2,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道机械手收回夹爪
void ServiceTaskFuncRailHand_Can1Msg_Proc_TakeClawBack(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailHandTakeClawBack(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
    //查询机械手上有无架子
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)(ServiceStateRailHandReadHandRackExist());
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_HAND_TAKE_HAND_BACK,can1Error,reportLevelResult,actionResult,1,returnData);
}

/*************************************************2. 来自自身的消息******************************************/
//轨道机械手旋转复位
static LH_ERR RailHandSelfCommandProcFunc_RotateReset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手前后复位
static LH_ERR RailHandSelfCommandProcFunc_BackFrontReset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手升降复位
static LH_ERR RailHandSelfCommandProcFunc_UpDownReset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手整体复位
static LH_ERR RailHandSelfCommandProcFunc_AllReset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手抓取进样1
static LH_ERR RailHandSelfCommandProcFunc_CatchPushIn1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手抓取进样2
static LH_ERR RailHandSelfCommandProcFunc_CatchPushIn2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手放下出样1
static LH_ERR RailHandSelfCommandProcFunc_PutPushOut1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手放下出样2
static LH_ERR RailHandSelfCommandProcFunc_PutPushOut2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手旋转到进样方向
static LH_ERR RailHandSelfCommandProcFunc_Rotate2PushIn(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手旋转到急诊方向
static LH_ERR RailHandSelfCommandProcFunc_Rotate2EmergencyCall(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手旋转到特定方向
static LH_ERR RailHandSelfCommandProcFunc_Rotate2SpecialDir(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手从急诊位抓取
static LH_ERR RailHandSelfCommandProcFunc_CatchEmergencyCall(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手从待测位抓取
static LH_ERR RailHandSelfCommandProcFunc_CatchWaitTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手往待测位放
static LH_ERR RailHandSelfCommandProcFunc_PutWaitTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手从重测位抓取
static LH_ERR RailHandSelfCommandProcFunc_CatchRepeatTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手往重测位放下
static LH_ERR RailHandSelfCommandProcFunc_PutRepeatTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手获取手上试管架状态
static LH_ERR RailHandSelfCommandProcFunc_GetRackState(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手机械手夹爪到出样1
static LH_ERR RailHandSelfCommandProcFunc_ClawReachPushOut1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手机械手夹爪到出样2
static LH_ERR RailHandSelfCommandProcFunc_ClawReachPushOut2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道机械手收回夹爪
static LH_ERR RailHandSelfCommandProcFunc_TakeClawBack(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);

//回调函数数组
const SYSTEM_TASK_SELF_MSG_PROC_UNIT RailHandSelfCommandProcFuncPtrArray[] = {
    {RailHandSelfCommandProcFunc_RotateReset                , "RotateReset"             },
    {RailHandSelfCommandProcFunc_BackFrontReset             , "BackFrontReset"          },
    {RailHandSelfCommandProcFunc_UpDownReset                , "UpDownReset"             },
    {RailHandSelfCommandProcFunc_AllReset                   , "AllReset"                },
    {RailHandSelfCommandProcFunc_CatchPushIn1               , "CatchPushIn1"            },
    {RailHandSelfCommandProcFunc_CatchPushIn2               , "CatchPushIn2"            },
    {RailHandSelfCommandProcFunc_PutPushOut1                , "PutPushOut1"             },
    {RailHandSelfCommandProcFunc_PutPushOut2                , "PutPushOut2"             },
    {RailHandSelfCommandProcFunc_Rotate2PushIn              , "Rotate2PushIn"           },
    {RailHandSelfCommandProcFunc_Rotate2EmergencyCall       , "Rotate2EmergencyCall"    },
    {RailHandSelfCommandProcFunc_Rotate2SpecialDir          , "Rotate2SpecialDir"       },
    {RailHandSelfCommandProcFunc_CatchEmergencyCall         , "CatchEmergencyCall"      },
    {RailHandSelfCommandProcFunc_CatchWaitTest              , "CatchWaitTest"           },
    {RailHandSelfCommandProcFunc_PutWaitTest                , "PutWaitTest"             },
    {RailHandSelfCommandProcFunc_CatchRepeatTest            , "CatchRepeatTest"         },
    {RailHandSelfCommandProcFunc_PutRepeatTest              , "PutRepeatTest"           },
    {RailHandSelfCommandProcFunc_GetRackState               , "GetRackState"            },
    {RailHandSelfCommandProcFunc_ClawReachPushOut1          , "ClawReachPushOut1"       },
    {RailHandSelfCommandProcFunc_ClawReachPushOut2          , "ClawReachPushOut2"       },
    {RailHandSelfCommandProcFunc_TakeClawBack               , "TakeClawBack"            },
};
//自身支持指令数量
#define TASK_SELF_MSG_RAIL_HAND_CMD_COUNT  ((sizeof(RailHandSelfCommandProcFuncPtrArray))/(sizeof(RailHandSelfCommandProcFuncPtrArray[0])))

//匹配发送过来的指令序号
static void ServiceTaskFuncRailHand_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_RAIL_HAND* selfCmdResultDataPtr = (IPC_DATA_RAIL_HAND*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (selfCmdResultDataPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	selfCmdResultDataPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_RAIL_HAND_CMD_COUNT)
	{
        SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,(uint8_t*)"unsupport",
                                selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                                selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                                );
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = RailHandSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,&(selfCmdResultDataPtr->can1ErrorCode),&(selfCmdResultDataPtr->can1ReportErrorLevel),
                                                                                            RailHandSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    SystemDebugShowMsgEnd((uint8_t*)RailHandDebugEndMsg,actionResult);
	//处理完成,设置状态
	selfCmdResultDataPtr->state = STATE_IPC_CMD_COMPLETE;
	selfCmdResultDataPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(selfCmdResultDataPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//轨道机械手旋转复位
static LH_ERR RailHandSelfCommandProcFunc_RotateReset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_RESET_CORRECT_FLAG resetCorrectFlag = (RAIL_RESET_CORRECT_FLAG)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,resetCorrectFlag,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandRotateReset(resetCorrectFlag,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手前后复位
static LH_ERR RailHandSelfCommandProcFunc_BackFrontReset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_RESET_CORRECT_FLAG resetCorrectFlag = (RAIL_RESET_CORRECT_FLAG)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,resetCorrectFlag,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandBackFrontReset(resetCorrectFlag,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手升降复位
static LH_ERR RailHandSelfCommandProcFunc_UpDownReset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_RESET_CORRECT_FLAG resetCorrectFlag = (RAIL_RESET_CORRECT_FLAG)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,resetCorrectFlag,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandUpDownReset(resetCorrectFlag,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手整体复位
static LH_ERR RailHandSelfCommandProcFunc_AllReset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandModuleReset(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手抓取进样1
static LH_ERR RailHandSelfCommandProcFunc_CatchPushIn1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandCatchPushIn1(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手抓取进样2
static LH_ERR RailHandSelfCommandProcFunc_CatchPushIn2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandCatchPushIn2(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手放下出样1
static LH_ERR RailHandSelfCommandProcFunc_PutPushOut1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandPutPushOut1(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手放下出样2
static LH_ERR RailHandSelfCommandProcFunc_PutPushOut2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandPutPushOut2(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手旋转到进样方向
static LH_ERR RailHandSelfCommandProcFunc_Rotate2PushIn(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandRotate2PushIn(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手旋转到急诊方向
static LH_ERR RailHandSelfCommandProcFunc_Rotate2EmergencyCall(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandRotate2EmergencyCall(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手旋转到特定方向
static LH_ERR RailHandSelfCommandProcFunc_Rotate2SpecialDir(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    HAND_DIR targetDir = (HAND_DIR)(selfCmdPtr->commandParam1);
    //设置调用实际函数需要的变量和初始条件
    asm("nop");
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,targetDir,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandRotate2SpecialDir(targetDir,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手从急诊位抓取
static LH_ERR RailHandSelfCommandProcFunc_CatchEmergencyCall(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandCatchEmergencyCall(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手从待测位抓取
static LH_ERR RailHandSelfCommandProcFunc_CatchWaitTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandCatchWaitTest(moduleNo,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手往待测位放
static LH_ERR RailHandSelfCommandProcFunc_PutWaitTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandPutWaitTest(moduleNo,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手从重测位抓取
static LH_ERR RailHandSelfCommandProcFunc_CatchRepeatTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandCatchRepeatTest(moduleNo,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//轨道机械手往重测位放下
static LH_ERR RailHandSelfCommandProcFunc_PutRepeatTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,moduleNo,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandPutRepeatTest(moduleNo,can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}

//轨道机械手获取手上试管架状态
static LH_ERR RailHandSelfCommandProcFunc_GetRackState(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandGetState(can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}

//轨道机械手机械手夹爪到出样1
static LH_ERR RailHandSelfCommandProcFunc_ClawReachPushOut1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandClawReachPushOut1(can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}

//轨道机械手机械手夹爪到出样2
static LH_ERR RailHandSelfCommandProcFunc_ClawReachPushOut2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandClawReachPushOut2(can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}

//轨道机械手收回夹爪
static LH_ERR RailHandSelfCommandProcFunc_TakeClawBack(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailHandDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailHandTakeClawBack(can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}
