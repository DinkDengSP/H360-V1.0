/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:12:16
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-23 14:34:06
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceTaskPlateReagent.c
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
CPU_STK stackBufferTaskServicePlateReagent[STK_SIZE_TASK_SERVICE_PLATE_REAGENT]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServicePlateReagent@".dtcm_ram";


//自动运行程序的执行周期,单位毫秒
#define SERVICE_PLATE_REAGENT_AUTO_FUNC_RUNNING_PERIOD_MS            200
//系统循环自动程序初始化
static void TaskServicePlateReagent_Auto_Init(void);
#if(SERVICE_PLATE_REAGENT_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统还处理一些循环自动化程序
static void TaskServicePlateReagent_Auto_Proc(void);
#endif

//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void TaskFuncServicePlateReagent_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr);
//2. 来自自身的消息
static void TaskFuncServicePlateReagent_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServicePlateReagent(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    TaskServicePlateReagent_Auto_Init();
    #if(SERVICE_PLATE_REAGENT_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(SERVICE_PLATE_REAGENT_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= SERVICE_PLATE_REAGENT_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了SERVICE_PLATE_REAGENT_AUTO_FUNC_RUNNING_PERIOD_MS个时间
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			TaskServicePlateReagent_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)SERVICE_PLATE_REAGENT_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                TaskFuncServicePlateReagent_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->commandSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->systemCommandDataPtr;
                //对这个数据进行处理
                TaskFuncServicePlateReagent_SelfMsg_Proc(selfCmdDataPtr);
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
static void TaskServicePlateReagent_Auto_Init(void)
{
    ServiceImplPlateReagent_StateMachineInit();
}
#if(SERVICE_PLATE_REAGENT_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统循环自动程序
static void TaskServicePlateReagent_Auto_Proc(void)
{
    ServiceImplPlateReagent_StateMachineLoop();
}
#endif


/*************************************************1. 来自CAN1的消息******************************************/
//预定义CAN1消息处理函数
//试剂盘复位
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_ResetAll(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘运行一定数量杯位
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_RunAnyCups(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘运行到指定杯位带偏移
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_Run2IndexOffset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘运行到指定杯位扫码位并扫码
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_Run2IndexAndScan(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘扫码
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_Scan(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘清除试剂液面探测高度讯息
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_ClearLiquidDetectData(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘设置状态空闲
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_SetIdle(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘读取特定输入
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘写入特定输出
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘指定电机复位
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘指定电机走位
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_MotorRunStep(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//CAN消息ID对应的处理函数,映射成表驱动
static const SERVICE_TASK_CAN1_PROC_UNIT ServiceCan1MsgProcUnitArray_PlateReagent[] = {
    {CAN1_DEBUG_CMD_PLATE_REAGENT_RESET_ALL                     ,   "ResetAll"              ,        TaskFuncServicePlateReagent_Can1Msg_Proc_ResetAll              },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_RUN_ANY_CUPS                  ,   "RunAnyCups"            ,        TaskFuncServicePlateReagent_Can1Msg_Proc_RunAnyCups            },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_RUN_2_INDEX_OFFSET            ,   "Run2IndexOffset"       ,        TaskFuncServicePlateReagent_Can1Msg_Proc_Run2IndexOffset       },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_RUN_2_INDEX_AND_SCAN          ,   "Run2IndexAndScan"      ,        TaskFuncServicePlateReagent_Can1Msg_Proc_Run2IndexAndScan      },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_SCAN                          ,   "Scan"                  ,        TaskFuncServicePlateReagent_Can1Msg_Proc_Scan                  },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_CLEAR_LIQUID_DETECT_DATA      ,   "ClearLiquidDetectData" ,        TaskFuncServicePlateReagent_Can1Msg_Proc_ClearLiquidDetectData },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_SET_IDLE                      ,   "SetIdle"               ,        TaskFuncServicePlateReagent_Can1Msg_Proc_SetIdle               },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_INPUT_READ                    ,   "InputRead"             ,        TaskFuncServicePlateReagent_Can1Msg_Proc_InputRead             },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_OUTPUT_WRITE                  ,   "OutputWrite"           ,        TaskFuncServicePlateReagent_Can1Msg_Proc_OutputWrite           },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_MOTOR_RESET                   ,   "MotorReset"            ,        TaskFuncServicePlateReagent_Can1Msg_Proc_MotorReset            },
    {CAN1_DEBUG_CMD_PLATE_REAGENT_MOTOR_RUN_STEP                ,   "MotorRunStep"          ,        TaskFuncServicePlateReagent_Can1Msg_Proc_MotorRunStep          },
};
//消息处理映射函数表的元素数量
#define COUNT_CAN1_CMD_PROC_UNIT_ARRAY_PLATE_REAGENT      (sizeof(ServiceCan1MsgProcUnitArray_PlateReagent)/sizeof(ServiceCan1MsgProcUnitArray_PlateReagent[0]))

//处理CAN1收到的任务:从映射表中查找对应的处理函数,将相应的指令转发给处理函数
static void TaskFuncServicePlateReagent_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < COUNT_CAN1_CMD_PROC_UNIT_ARRAY_PLATE_REAGENT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == ServiceCan1MsgProcUnitArray_PlateReagent[indexUtil].command)
        {
            //调用处理函数
            if(ServiceCan1MsgProcUnitArray_PlateReagent[indexUtil].funcPtr != NULL)
            {
                //结果代码
                LH_ERR actionResult = LH_ERR_NONE;
                //函数调用
                actionResult = ServiceCan1MsgProcUnitArray_PlateReagent[indexUtil].funcPtr(can1CommandPtr,ServiceCan1MsgProcUnitArray_PlateReagent[indexUtil].debugDetailMsg);
                //打印结束处理信息
	            ServicePrintfShowEnd((uint8_t*)DebugStringEnd_PlateReagent,actionResult);
                break;
            }
        }
    }
}

//试剂盘复位
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_ResetAll(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t currentPlateReagentIndex;
    PLATE_REAGENT_OFFSET currentPlateReagentOffset;
    actionResult = ServiceImplPlateReagent_ResetAll(&currentPlateReagentIndex,&currentPlateReagentOffset,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//试剂盘运行一定数量杯位
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_RunAnyCups(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t cupsCount = can1CommandPtr->can1RecvDataBuffer[0];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,cupsCount,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t currentPlateReagentIndex;
    PLATE_REAGENT_OFFSET currentPlateReagentOffset;
    actionResult = ServiceImplPlateReagent_RunAnyCups(cupsCount,&currentPlateReagentIndex,&currentPlateReagentOffset,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)currentPlateReagentIndex;
    returnData[1] = (uint8_t)currentPlateReagentOffset;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//试剂盘运行到指定杯位带偏移
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_Run2IndexOffset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t targetIndex = can1CommandPtr->can1RecvDataBuffer[0];
    PLATE_REAGENT_OFFSET targetPlateReagentOffset = (PLATE_REAGENT_OFFSET)(can1CommandPtr->can1RecvDataBuffer[1]);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,targetIndex,targetPlateReagentOffset,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t currentPlateReagentIndex;
    PLATE_REAGENT_OFFSET currentPlateReagentOffset;
    actionResult = ServiceImplPlateReagent_Run2IndexWithOffset(targetIndex,targetPlateReagentOffset,&currentPlateReagentIndex,&currentPlateReagentOffset,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)currentPlateReagentIndex;
    returnData[1] = (uint8_t)currentPlateReagentOffset;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//试剂盘运行到指定杯位扫码位并扫码
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_Run2IndexAndScan(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t targetIndex = can1CommandPtr->can1RecvDataBuffer[0];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,targetIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t currentPlateReagentIndex;
    PLATE_REAGENT_OFFSET currentPlateReagentOffset;
    uint16_t barCodeLength = 0;
    uint8_t* barCodeBufferPtr = NULL;
    actionResult = ServiceImplPlateReagent_Run2IndexScanAndQRScan(targetIndex,&currentPlateReagentIndex,&currentPlateReagentOffset,&barCodeBufferPtr,
                                                                        &barCodeLength,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t* returnDataBuffer = NULL;
    do
    {
        returnDataBuffer = UserMemMalloc(MEM_AXI_SRAM,3+barCodeLength);
        if(returnDataBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(returnDataBuffer == NULL);
    returnDataBuffer[0] = currentPlateReagentIndex;
    Can1ProcUtilConvertUint16ToArray(returnDataBuffer,1,barCodeLength);
    if(barCodeLength != 0)
    {
        UserMemCopy(returnDataBuffer+3,barCodeBufferPtr,barCodeLength);
    }
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,3+barCodeLength,returnDataBuffer);
    //释放内存
    if(barCodeBufferPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,barCodeBufferPtr);
    }
    UserMemFree(MEM_AXI_SRAM,returnDataBuffer);
    //返回错误代码
    return actionResult;
}

//试剂盘扫码
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_Scan(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint16_t barCodeLength = 0;
    uint8_t* barCodeBufferPtr = NULL;
    actionResult = ServiceImplPlateReagent_BarScanOnce(&barCodeBufferPtr,&barCodeLength,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t* returnDataBuffer = NULL;
    do
    {
        returnDataBuffer = UserMemMalloc(MEM_AXI_SRAM,2+barCodeLength);
        if(returnDataBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(returnDataBuffer == NULL);
    Can1ProcUtilConvertUint16ToArray(returnDataBuffer,0,barCodeLength);
    if(barCodeLength != 0)
    {
        UserMemCopy(returnDataBuffer+2,barCodeBufferPtr,barCodeLength);
    }
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2+barCodeLength,returnDataBuffer);
    //释放内存
    if(barCodeBufferPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,barCodeBufferPtr);
    }
    UserMemFree(MEM_AXI_SRAM,returnDataBuffer);
    //返回错误代码
    return actionResult;
}

//试剂盘清除试剂液面探测高度讯息
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_ClearLiquidDetectData(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t clearIndex = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t clearConfig = can1CommandPtr->can1RecvDataBuffer[1];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,clearIndex,clearConfig,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplPlateReagent_ClearLiquidDetectData(clearIndex,clearConfig,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//试剂盘设置状态空闲
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_SetIdle(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplPlateReagent_SetIdle(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//试剂盘读取特定输入
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,inputIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE inputState;
    actionResult = ServiceImplPlateReagent_InputRead(inputIndex,&inputState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)inputIndex;
    returnData[1] = (uint8_t)inputState;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//试剂盘写入特定输出
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,outputIndex,outputState,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplPlateReagent_OutputWrite(outputIndex,outputState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)outputIndex;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//试剂盘指定电机复位
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,motorIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplPlateReagent_MotorReset(motorIndex,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)motorIndex;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//试剂盘指定电机走位
LH_ERR TaskFuncServicePlateReagent_Can1Msg_Proc_MotorRunStep(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,motorIndex,motorDir,motorSteps,0,0);
    //程序进行处理,调用真实实现
    int32_t posAfterRun = 0;
    actionResult = ServiceImplPlateReagent_MotorRunSteps(motorIndex,motorSteps,&posAfterRun,&commErrorCode,&commErrorLevel);
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
//试剂盘复位
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_ResetAll(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘运行指定个杯位
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_RunAnyCups(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘运行到指定杯位带偏移
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_Run2IndexWithOffset(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘运行到指定杯位的扫码位并扫码
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_Run2IndexScanAndQRScan(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘扫码一次
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_BarScanOnce(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//试剂盘清除探液数据
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_ClearLiquidDetectData(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//设置盘设置状态空闲
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_SetIdle(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);

//自身指令回调函数数组
static const SERVICE_TASK_SELF_PROC_UNIT ServiceSelfMsgProcUnitArray_PlateReagent[] = {
	{TaskFuncServicePlateReagent_SelfMsg_Proc_ResetAll                  ,   "ResetAll"              },
    {TaskFuncServicePlateReagent_SelfMsg_Proc_RunAnyCups                ,   "RunAnyCups"            },
    {TaskFuncServicePlateReagent_SelfMsg_Proc_Run2IndexWithOffset       ,   "Run2IndexWithOffset"   },
    {TaskFuncServicePlateReagent_SelfMsg_Proc_Run2IndexScanAndQRScan    ,   "Run2IndexScanAndQRScan"},
    {TaskFuncServicePlateReagent_SelfMsg_Proc_BarScanOnce               ,   "BarScanOnce"           },
    {TaskFuncServicePlateReagent_SelfMsg_Proc_ClearLiquidDetectData     ,   "ClearLiquidDetectData" },
    {TaskFuncServicePlateReagent_SelfMsg_Proc_SetIdle                   ,   "SetIdle"               },
};
//消息处理映射函数表的元素数量
#define COUNT_SELF_CMD_PROC_UNIT_ARRAY_PLATE_REAGENT      (sizeof(ServiceSelfMsgProcUnitArray_PlateReagent)/sizeof(ServiceSelfMsgProcUnitArray_PlateReagent[0]))

//自身指令回调处理函数,解析发送过来的参数并调用映射表中的函数
static void TaskFuncServicePlateReagent_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_PLATE_REAGENT* cmdResultPtr = (IPC_DATA_PLATE_REAGENT*)(selfCmdPtr->commandDataSpecialChannel);
	
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= COUNT_SELF_CMD_PROC_UNIT_ARRAY_PLATE_REAGENT)
	{
		ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)"Unsupport",
				selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
				selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
				);
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
		//回调函数
		actionResult = ServiceSelfMsgProcUnitArray_PlateReagent[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                                        ServiceSelfMsgProcUnitArray_PlateReagent[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//打印结束处理信息
	ServicePrintfShowEnd((uint8_t*)DebugStringEnd_PlateReagent,actionResult);

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

//试剂盘复位
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_ResetAll(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_PLATE_REAGENT* cmdResultPtr = (IPC_DATA_PLATE_REAGENT*)(selfCommandPtr->commandDataSpecialChannel);
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t currentPlateReagentIndex;
    PLATE_REAGENT_OFFSET currentPlateReagentOffset;
    actionResult = ServiceImplPlateReagent_ResetAll(&currentPlateReagentIndex,&currentPlateReagentOffset,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//试剂盘运行指定数量杯位
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_RunAnyCups(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_PLATE_REAGENT* cmdResultPtr = (IPC_DATA_PLATE_REAGENT*)(selfCommandPtr->commandDataSpecialChannel);
    uint8_t cupsCount = (uint8_t)(selfCommandPtr->commandParam1);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,cupsCount,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t currentPlateReagentIndex;
    PLATE_REAGENT_OFFSET currentPlateReagentOffset;
    actionResult = ServiceImplPlateReagent_RunAnyCups(cupsCount,&currentPlateReagentIndex,&currentPlateReagentOffset,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//试剂盘运行到指定杯位带偏移
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_Run2IndexWithOffset(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_PLATE_REAGENT* cmdResultPtr = (IPC_DATA_PLATE_REAGENT*)(selfCommandPtr->commandDataSpecialChannel);
    uint8_t targetIndex = (uint8_t)(selfCommandPtr->commandParam1);
    PLATE_REAGENT_OFFSET targetPlateReagentOffset = (PLATE_REAGENT_OFFSET)(selfCommandPtr->commandParam2);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,targetIndex,targetPlateReagentOffset,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t currentPlateReagentIndex;
    PLATE_REAGENT_OFFSET currentPlateReagentOffset;
    actionResult = ServiceImplPlateReagent_Run2IndexWithOffset(targetIndex,targetPlateReagentOffset,&currentPlateReagentIndex,&currentPlateReagentOffset,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//试剂盘运行到指定杯位的扫码位并扫码
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_Run2IndexScanAndQRScan(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_PLATE_REAGENT* cmdResultPtr = (IPC_DATA_PLATE_REAGENT*)(selfCommandPtr->commandDataSpecialChannel);
    uint8_t targetIndex = (uint8_t)(selfCommandPtr->commandParam1);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,targetIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t currentPlateReagentIndex;
    PLATE_REAGENT_OFFSET currentPlateReagentOffset;
    uint16_t barCodeLength = 0;
    uint8_t* barCodeBufferPtr = NULL;
    actionResult = ServiceImplPlateReagent_Run2IndexScanAndQRScan(targetIndex,&currentPlateReagentIndex,&currentPlateReagentOffset,&barCodeBufferPtr,
                                                                        &barCodeLength,&commErrorCode,&commErrorLevel);
    if(barCodeBufferPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,barCodeBufferPtr);
    }
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//试剂盘扫码一次
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_BarScanOnce(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_PLATE_REAGENT* cmdResultPtr = (IPC_DATA_PLATE_REAGENT*)(selfCommandPtr->commandDataSpecialChannel);
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint16_t barCodeLength = 0;
    uint8_t* barCodeBufferPtr = NULL;
    actionResult = ServiceImplPlateReagent_BarScanOnce(&barCodeBufferPtr,&barCodeLength,&commErrorCode,&commErrorLevel);
    if(barCodeBufferPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,barCodeBufferPtr);
    }
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//试剂盘清除探液数据
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_ClearLiquidDetectData(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_PLATE_REAGENT* cmdResultPtr = (IPC_DATA_PLATE_REAGENT*)(selfCommandPtr->commandDataSpecialChannel);
    uint8_t clearIndex = (uint8_t)(selfCommandPtr->commandParam1);
    uint8_t clearConfig = (uint8_t)(selfCommandPtr->commandParam2);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,clearIndex,clearConfig,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplPlateReagent_ClearLiquidDetectData(clearIndex,clearConfig,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//设置盘设置状态空闲
static LH_ERR TaskFuncServicePlateReagent_SelfMsg_Proc_SetIdle(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_PLATE_REAGENT* cmdResultPtr = (IPC_DATA_PLATE_REAGENT*)(selfCommandPtr->commandDataSpecialChannel);
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_PlateReagent,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplPlateReagent_SetIdle(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}


