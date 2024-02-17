/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:21:47
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-22 16:53:10
**FilePath: \Application\TaskMain\ServiceSupport\ServiceTask\ServiceTaskHandNewCup.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"
#include "ServiceIpcInc.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskServiceHandNewCup[STK_SIZE_TASK_SERVICE_HAND_NEW_CUP]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceHandNewCup@".dtcm_ram";


//自动运行程序的执行周期,单位毫秒
#define SERVICE_HAND_NEW_CUP_AUTO_FUNC_RUNNING_PERIOD_MS            0
//系统循环自动程序初始化
static void TaskServiceHandNewCup_Auto_Init(void);
#if(SERVICE_HAND_NEW_CUP_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统还处理一些循环自动化程序
static void TaskServiceHandNewCup_Auto_Proc(void);
#endif

//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void TaskFuncServiceHandNewCup_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr);
//2. 来自自身的消息
static void TaskFuncServiceHandNewCup_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceHandNewCup(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    TaskServiceHandNewCup_Auto_Init();
    #if(SERVICE_HAND_NEW_CUP_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(SERVICE_HAND_NEW_CUP_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= SERVICE_HAND_NEW_CUP_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了SERVICE_HAND_NEW_CUP_AUTO_FUNC_RUNNING_PERIOD_MS个时间
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			TaskServiceHandNewCup_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)SERVICE_HAND_NEW_CUP_AUTO_FUNC_RUNNING_PERIOD_MS,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(SYSTEM_CMD_SRC_CAN1 == taskCommandPtr->commandSrc)
            {
                //从CAN1传过来的指令
                SYSTEM_CMD_CAN1* can1RecvDatPtr = (SYSTEM_CMD_CAN1*)taskCommandPtr->systemCommandDataPtr;
                //对这个数据进行处理
                TaskFuncServiceHandNewCup_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->commandSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->systemCommandDataPtr;
                //对这个数据进行处理
                TaskFuncServiceHandNewCup_SelfMsg_Proc(selfCmdDataPtr);
                //处理完成
                UserMemFree(MEM_DTCM,selfCmdDataPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            taskCommandPtr->systemCommandDataPtr = NULL;
            UserMemFree(MEM_DTCM,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}

/************************************************自动运行程序************************************************/
//系统循环自动程序初始化
static void TaskServiceHandNewCup_Auto_Init(void)
{
    ServiceImplHandNewCup_StateMachineInit();
}
#if(SERVICE_HAND_NEW_CUP_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统循环自动程序
static void TaskServiceHandNewCup_Auto_Proc(void)
{
    ServiceImplHandNewCup_StateMachineLoop();
}
#endif


/*************************************************1. 来自CAN1的消息******************************************/
//预定义CAN1消息处理函数
//新杯机械手全部复位
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_ResetAll(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到新杯栈
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_Move2NewStack(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到反应盘
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_Move2PlateReaction(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到垃圾桶
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_Move2Garbage(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手从新杯区取杯
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_CatchNewStack(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手从反应盘取杯
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_CatchPlateReaction(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//往反应盘放杯
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_PutPlateReaction(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//往垃圾桶放杯
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_PutGarbage(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//模块读取输入
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//模块写入输出
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//指定电机复位
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//指定电机走指定步数
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_MotorRunSteps(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//CAN消息ID对应的处理函数,映射成表驱动
static const SERVICE_TASK_CAN1_PROC_UNIT ServiceCan1MsgProcUnitArray_HandNewCup[] = {
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_RESET_ALL                  ,   "ResetAll"              ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_ResetAll            },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_MOVE_2_NEW_STACK           ,   "Move2NewStack"         ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_Move2NewStack       },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_MOVE_2_PLATE_REACTION      ,   "Move2PlateReaction"    ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_Move2PlateReaction  },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_MOVE_2_GARBAGE             ,   "Move2Garbage"          ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_Move2Garbage        },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_CATCH_NEW_STACK            ,   "CatchNewStack"         ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_CatchNewStack       },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_CATCH_PLATE_REACTION       ,   "CatchPlateReaction"    ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_CatchPlateReaction  },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_PUT_PLATE_REACTION         ,   "PutPlateReaction"      ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_PutPlateReaction    },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_PUT_GARBAGE                ,   "PutGarbage"            ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_PutGarbage          },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_INPUT_READ                 ,   "InputRead"             ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_InputRead           },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_OUTPUT_WRITE               ,   "OutputWrite"           ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_OutputWrite         },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_MOTOR_RESET                ,   "MotorReset"            ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_MotorReset          },
    {CAN1_DEBUG_CMD_HAND_NEW_CUP_MOTOR_RUN_STEP             ,   "MotorRunSteps"         ,        TaskFuncServiceHandNewCup_Can1Msg_Proc_MotorRunSteps       },
};
//消息处理映射函数表的元素数量
#define COUNT_CAN1_CMD_PROC_UNIT_ARRAY_HAND_NEW_CUP      (sizeof(ServiceCan1MsgProcUnitArray_HandNewCup)/sizeof(ServiceCan1MsgProcUnitArray_HandNewCup[0]))

//处理CAN1收到的任务:从映射表中查找对应的处理函数,将相应的指令转发给处理函数
static void TaskFuncServiceHandNewCup_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < COUNT_CAN1_CMD_PROC_UNIT_ARRAY_HAND_NEW_CUP; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == ServiceCan1MsgProcUnitArray_HandNewCup[indexUtil].command)
        {
            //调用处理函数
            if(ServiceCan1MsgProcUnitArray_HandNewCup[indexUtil].funcPtr != NULL)
            {
                //结果代码
                LH_ERR actionResult = LH_ERR_NONE;
                //函数调用
                actionResult = ServiceCan1MsgProcUnitArray_HandNewCup[indexUtil].funcPtr(can1CommandPtr,ServiceCan1MsgProcUnitArray_HandNewCup[indexUtil].debugDetailMsg);
                //打印结束处理信息
	            ServicePrintfShowEnd((uint8_t*)DebugStringEnd_HandNewCup,actionResult);
                break;
            }
        }
    }
}

//新杯机械手全部复位
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_ResetAll(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_ResetAll(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//新杯机械手移动到新杯栈
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_Move2NewStack(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t colLeftRight = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t lineBackFront = can1CommandPtr->can1RecvDataBuffer[1];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,colLeftRight,lineBackFront,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_Move2NewStack(colLeftRight,lineBackFront,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//新杯机械手移动到反应盘
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_Move2PlateReaction(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_Move2PlateReaction(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//新杯机械手移动到垃圾桶
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_Move2Garbage(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    GARBAGE_SELECT garbageSelect = (GARBAGE_SELECT)(can1CommandPtr->can1RecvDataBuffer[0]);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,garbageSelect,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_Move2Garbage(garbageSelect,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//新杯机械手从新杯区取杯
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_CatchNewStack(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_CatchCupFromNewStack(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//新杯机械手从反应盘取杯
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_CatchPlateReaction(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE cupPlateReactionExistSensor;
    actionResult = ServiceImplHandNewCup_CatchCupFromPlateReaction(&cupPlateReactionExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)cupPlateReactionExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//往反应盘放杯
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_PutPlateReaction(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE cupPlateReactionExistSensor;
    actionResult = ServiceImplHandNewCup_PutCupToPlateReaction(&cupPlateReactionExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)cupPlateReactionExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//往垃圾桶放杯
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_PutGarbage(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    GARBAGE_SELECT garbageSelect = (GARBAGE_SELECT)(can1CommandPtr->can1RecvDataBuffer[0]);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,garbageSelect,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_PutCupToGarbage(garbageSelect,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//模块读取输入
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t inputIndex = can1CommandPtr->can1RecvDataBuffer[0];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,inputIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE inputState;
    actionResult = ServiceImplHandNewCup_InputRead(inputIndex,&inputState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)inputIndex;
    returnData[1] = (uint8_t)inputState;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//模块写入输出
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t outputIndex = can1CommandPtr->can1RecvDataBuffer[0];
    OUT_STATE outputState = (OUT_STATE)(can1CommandPtr->can1RecvDataBuffer[1]);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,outputIndex,outputState,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_OutputWrite(outputIndex,outputState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)outputIndex;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//指定电机复位
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t motorIndex = can1CommandPtr->can1RecvDataBuffer[0];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,motorIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_MotorReset(motorIndex,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)motorIndex;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//指定电机走指定步数
LH_ERR TaskFuncServiceHandNewCup_Can1Msg_Proc_MotorRunSteps(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t motorIndex = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t motorDir = can1CommandPtr->can1RecvDataBuffer[1];
    int32_t motorSteps = (int32_t)(Can1ProcUtilConvertArrayToUint32(can1CommandPtr->can1RecvDataBuffer,2));
    if(motorDir == 0)
    {
        motorSteps = 0- motorSteps;
    }
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,motorIndex,motorDir,motorSteps,0,0);
    //程序进行处理,调用真实实现
    int32_t posAfterRun = 0;
    actionResult = ServiceImplHandNewCup_MotorRunSteps(motorIndex,motorSteps,&posAfterRun,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[5] = {0};
    //电机序号
    returnData[0] = motorIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,1,posAfterRun);
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,5,returnData);
    //返回错误代码
    return actionResult;
}


/*************************************************2. 来自自身的消息******************************************/
//新杯机械手复位
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_ResetAll(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到新杯栈
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_Move2NewStack(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到反应盘
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_Move2PlateReaction(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到垃圾桶
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_Move2Garbage(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手从新杯栈抓杯
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_CatchCupFromNewStack(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手往反应盘放杯
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_PutCupToPlateReaction(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手从反应盘抓杯
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_CatchCupFromPlateReaction(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手往垃圾桶放杯
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_PutCupToGarbage(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);

//自身指令回调函数数组
static const SERVICE_TASK_SELF_PROC_UNIT ServiceSelfMsgProcUnitArray_HandNewCup[] = {
	{TaskFuncServiceHandNewCup_SelfMsg_Proc_ResetAll                    ,   "ResetAll"                  },
    {TaskFuncServiceHandNewCup_SelfMsg_Proc_Move2NewStack               ,   "Move2NewStack"             },
    {TaskFuncServiceHandNewCup_SelfMsg_Proc_Move2PlateReaction          ,   "Move2PlateReaction"        },
    {TaskFuncServiceHandNewCup_SelfMsg_Proc_Move2Garbage                ,   "Move2Garbage"              },
    {TaskFuncServiceHandNewCup_SelfMsg_Proc_CatchCupFromNewStack        ,   "CatchCupFromNewStack"      },
    {TaskFuncServiceHandNewCup_SelfMsg_Proc_PutCupToPlateReaction       ,   "PutCupToPlateReaction"     },
    {TaskFuncServiceHandNewCup_SelfMsg_Proc_CatchCupFromPlateReaction   ,   "CatchCupFromPlateReaction" },
    {TaskFuncServiceHandNewCup_SelfMsg_Proc_PutCupToGarbage             ,   "PutCupToGarbage"           },
};
//消息处理映射函数表的元素数量
#define COUNT_SELF_CMD_PROC_UNIT_ARRAY_HAND_NEW_CUP      (sizeof(ServiceSelfMsgProcUnitArray_HandNewCup)/sizeof(ServiceSelfMsgProcUnitArray_HandNewCup[0]))

//自身指令回调处理函数,解析发送过来的参数并调用映射表中的函数
static void TaskFuncServiceHandNewCup_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_HAND_NEW_CUP* cmdResultPtr = (IPC_DATA_HAND_NEW_CUP*)(selfCmdPtr->commandDataSpecialChannel);
	
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= COUNT_SELF_CMD_PROC_UNIT_ARRAY_HAND_NEW_CUP)
	{
		ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)"Unsupport",
				selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
				selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
				);
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
		//回调函数
		actionResult = ServiceSelfMsgProcUnitArray_HandNewCup[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                                        ServiceSelfMsgProcUnitArray_HandNewCup[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//打印结束处理信息
	ServicePrintfShowEnd((uint8_t*)DebugStringEnd_HandNewCup,actionResult);

	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    //动作出错
    if((cmdResultPtr->can1ReportCode != CAN1_REPORT_ERR_OK) || (cmdResultPtr->resultValue != LH_ERR_NONE))
    {
        IPC_Service_ActionWarningSendToCommon(cmdResultPtr->can1ReportLevel,cmdResultPtr->can1ReportCode,cmdResultPtr->resultValue);
    }
    return;
}

//新杯机械手复位
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_ResetAll(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_HAND_NEW_CUP* cmdResultPtr = (IPC_DATA_HAND_NEW_CUP*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_ResetAll(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//新杯机械手移动到新杯栈
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_Move2NewStack(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_HAND_NEW_CUP* cmdResultPtr = (IPC_DATA_HAND_NEW_CUP*)(selfCommandPtr->commandDataSpecialChannel);
    uint8_t leftRight = (uint8_t)(selfCommandPtr->commandParam1);
    uint8_t frontBack = (uint8_t)(selfCommandPtr->commandParam2);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,leftRight,frontBack,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_Move2NewStack(leftRight,frontBack,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//新杯机械手移动到反应盘
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_Move2PlateReaction(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_HAND_NEW_CUP* cmdResultPtr = (IPC_DATA_HAND_NEW_CUP*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_Move2PlateReaction(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//新杯机械手移动到垃圾桶
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_Move2Garbage(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_HAND_NEW_CUP* cmdResultPtr = (IPC_DATA_HAND_NEW_CUP*)(selfCommandPtr->commandDataSpecialChannel);
    GARBAGE_SELECT garbageSelect = (GARBAGE_SELECT)(selfCommandPtr->commandParam1);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,garbageSelect,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_Move2Garbage(garbageSelect,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//新杯机械手从新杯栈抓杯
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_CatchCupFromNewStack(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_HAND_NEW_CUP* cmdResultPtr = (IPC_DATA_HAND_NEW_CUP*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_CatchCupFromNewStack(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//新杯机械手往反应盘放杯
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_PutCupToPlateReaction(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_HAND_NEW_CUP* cmdResultPtr = (IPC_DATA_HAND_NEW_CUP*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE cupPlateReactionExistSensor;
    actionResult = ServiceImplHandNewCup_PutCupToPlateReaction(&cupPlateReactionExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//新杯机械手从反应盘抓杯
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_CatchCupFromPlateReaction(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_HAND_NEW_CUP* cmdResultPtr = (IPC_DATA_HAND_NEW_CUP*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE cupPlateReactionExistSensor;
    actionResult = ServiceImplHandNewCup_CatchCupFromPlateReaction(&cupPlateReactionExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//新杯机械手往垃圾桶放杯
static LH_ERR TaskFuncServiceHandNewCup_SelfMsg_Proc_PutCupToGarbage(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_HAND_NEW_CUP* cmdResultPtr = (IPC_DATA_HAND_NEW_CUP*)(selfCommandPtr->commandDataSpecialChannel);
    GARBAGE_SELECT garbageSelect = (GARBAGE_SELECT)(selfCommandPtr->commandParam1);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_HandNewCup,(uint8_t*)detailDebugMsgPtr,garbageSelect,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplHandNewCup_PutCupToGarbage(garbageSelect,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}


