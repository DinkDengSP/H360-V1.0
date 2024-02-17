/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:33:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-28 09:10:53
**FilePath: \Application\TaskMain\ServiceSupport\ServiceTask\ServiceTaskLiquidAuto.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"
#include "ServiceIpcInc.h"
#include "ServiceStatePrivate.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskServiceLiquidAuto[STK_SIZE_TASK_SERVICE_LIQUID_AUTO]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceLiquidAuto@".dtcm_ram";


//自动运行程序的执行周期,单位毫秒
#define SERVICE_LIQUID_AUTO_AUTO_FUNC_RUNNING_PERIOD_MS            100
//系统循环自动程序初始化
static void TaskServiceLiquidAuto_Auto_Init(void);
#if(SERVICE_LIQUID_AUTO_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统还处理一些循环自动化程序
static void TaskServiceLiquidAuto_Auto_Proc(void);
#endif

//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void TaskFuncServiceLiquidAuto_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr);
//2. 来自自身的消息
static void TaskFuncServiceLiquidAuto_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceLiquidAuto(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    TaskServiceLiquidAuto_Auto_Init();
    #if(SERVICE_LIQUID_AUTO_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(SERVICE_LIQUID_AUTO_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= SERVICE_LIQUID_AUTO_AUTO_FUNC_RUNNING_PERIOD_MS-10)
		{
			//上次发送时间和本次发送时间差了SERVICE_LIQUID_AUTO_AUTO_FUNC_RUNNING_PERIOD_MS个时间
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			TaskServiceLiquidAuto_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)SERVICE_LIQUID_AUTO_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                TaskFuncServiceLiquidAuto_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->commandSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->systemCommandDataPtr;
                //对这个数据进行处理
                TaskFuncServiceLiquidAuto_SelfMsg_Proc(selfCmdDataPtr);
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
static void TaskServiceLiquidAuto_Auto_Init(void)
{
    ServiceImplLiquidAuto_StateMachineInit();
}
#if(SERVICE_LIQUID_AUTO_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统循环自动程序
static void TaskServiceLiquidAuto_Auto_Proc(void)
{
    //实际循环任务
    ServiceImplLiquidAuto_StateMachineLoop();
}
#endif


/*************************************************1. 来自CAN1的消息******************************************/
//预定义CAN1消息处理函数
//液路自动化强制排废液一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnforceWasteDischargeOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化强制三针清洗液稀释一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnforceThreeNeedleCleanDiluentOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化强制补充存水一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnforcePureWaterSupplementOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化使能自动排废液
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnableWasteDischargeAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化使能自动三针清洗稀释
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnableThreeNeedleCleanDiluentAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化使能纯化水自动补充
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnablePureWaterSupplementAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化关闭自动排废液
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_DisableWasteDischargeAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化关闭自动三针清洗稀释
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_DisableThreeNeedleCleanDiluentAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化关闭自动补纯化水
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_DisablePureWaterSupplementAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取特定输入
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//写入特定输出
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//特定电机复位
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//特定电机走指定步数
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_MotorRunSteps(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化配置瓶注纯水一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_ConfigAddPureWaterOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化配置瓶注浓缩液一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_ConfigAddConcentrateOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化配置瓶向工作瓶抽一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_ConfigToTargetOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//CAN消息ID对应的处理函数,映射成表驱动
static const SERVICE_TASK_CAN1_PROC_UNIT ServiceCan1MsgProcUnitArray_LiquidAuto[] = {
    {CAN1_DEBUG_CMD_LIQUID_AUTO_ENFORCE_WASTE_DISCHARGE_ONCE                ,   "EnforceWasteDischargeOnce"             ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnforceWasteDischargeOnce           },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_ENFORCE_THREE_NEEDLE_CLEAN_DILUENT_ONCE     ,   "EnforceThreeNeedleCleanDiluentOnce"    ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnforceThreeNeedleCleanDiluentOnce  },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_ENFORCE_PURE_WATER_SUPPLEMENT_ONCE          ,   "EnforcePureWaterSupplementOnce"        ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnforcePureWaterSupplementOnce      },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_ENABLE_WASTE_DISCHARGE_AUTO                 ,   "EnableWasteDischargeAuto"              ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnableWasteDischargeAuto            },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_ENABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO      ,   "EnableThreeNeedleCleanDiluentAuto"     ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnableThreeNeedleCleanDiluentAuto   },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_ENABLE_PURE_WATER_SUPPLEMENT_AUTO           ,   "EnablePureWaterSupplementAuto"         ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnablePureWaterSupplementAuto       },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_DISABLE_WASTE_DISCHARGE_AUTO                ,   "DisableWasteDischargeAuto"             ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_DisableWasteDischargeAuto           },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_DISABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO     ,   "DisableThreeNeedleCleanDiluentAuto"    ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_DisableThreeNeedleCleanDiluentAuto  },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_DISABLE_PURE_WATER_SUPPLEMENT_AUTO          ,   "DisablePureWaterSupplementAuto"        ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_DisablePureWaterSupplementAuto      },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_INPUT_READ                                  ,   "InputRead"                             ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_InputRead                           },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_OUTPUT_WRITE                                ,   "OutputWrite"                           ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_OutputWrite                         },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_MOTOR_RESET                                 ,   "MotorReset"                            ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_MotorReset                          },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_MOTOR_RUN_STEP                              ,   "MotorRunSteps"                         ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_MotorRunSteps                       },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_CONFIG_ADD_PURE_WATER_ONCE                  ,   "ConfigAddPureWaterOnce"                ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_ConfigAddPureWaterOnce              },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_CONFIG_ADD_CONCENTRATE_ONCE                 ,   "ConfigAddConcentrateOnce"              ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_ConfigAddConcentrateOnce            },
    {CAN1_DEBUG_CMD_LIQUID_AUTO_CONFIG_TO_TARGET_ONCE                       ,   "ConfigToTargetOnce"                    ,        TaskFuncServiceLiquidAuto_Can1Msg_Proc_ConfigToTargetOnce                  },
};
//消息处理映射函数表的元素数量
#define COUNT_CAN1_CMD_PROC_UNIT_ARRAY_LIQUID_AUTO      (sizeof(ServiceCan1MsgProcUnitArray_LiquidAuto)/sizeof(ServiceCan1MsgProcUnitArray_LiquidAuto[0]))

//处理CAN1收到的任务:从映射表中查找对应的处理函数,将相应的指令转发给处理函数
static void TaskFuncServiceLiquidAuto_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < COUNT_CAN1_CMD_PROC_UNIT_ARRAY_LIQUID_AUTO; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == ServiceCan1MsgProcUnitArray_LiquidAuto[indexUtil].command)
        {
            //调用处理函数
            if(ServiceCan1MsgProcUnitArray_LiquidAuto[indexUtil].funcPtr != NULL)
            {
                //结果代码
                LH_ERR actionResult = LH_ERR_NONE;
                //函数调用
                actionResult = ServiceCan1MsgProcUnitArray_LiquidAuto[indexUtil].funcPtr(can1CommandPtr,ServiceCan1MsgProcUnitArray_LiquidAuto[indexUtil].debugDetailMsg);
                //打印结束处理信息
	            ServicePrintfShowEnd((uint8_t*)DebugStringEnd_LiquidAuto,actionResult);
                break;
            }
        }
    }
}

//液路自动化强制排废液一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnforceWasteDischargeOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE fullSensorState;
    actionResult = ServiceImplLiquidAuto_EnforceWasteDischargeOnce(&fullSensorState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)fullSensorState;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//液路自动化强制三针清洗液稀释一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnforceThreeNeedleCleanDiluentOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE bottleStateA;
    SENSOR_STATE bottleStateB;
    SENSOR_STATE bottleDiluentTransferFull;
    SENSOR_STATE bottleDiluentTransferEmpty;
    SENSOR_STATE bottleDiluentTargetFull;
    SENSOR_STATE bottleDiluentTargetEmpty;
    actionResult = ServiceImplLiquidAuto_EnforceThreeNeedleCleanDiluentOnce(&bottleStateA,&bottleStateB,&bottleDiluentTransferFull,&bottleDiluentTransferEmpty,
                                                                                &bottleDiluentTargetFull,&bottleDiluentTargetEmpty,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[6] = {0};
    returnData[0] = (uint8_t)bottleStateA;
    returnData[1] = (uint8_t)bottleStateB;
    returnData[2] = (uint8_t)bottleDiluentTransferFull;
    returnData[3] = (uint8_t)bottleDiluentTransferEmpty;
    returnData[4] = (uint8_t)bottleDiluentTargetFull;
    returnData[5] = (uint8_t)bottleDiluentTargetEmpty;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,6,returnData);
    //返回错误代码
    return actionResult;
}

//液路自动化强制补充存水一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnforcePureWaterSupplementOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE lowLiquidState;
    SENSOR_STATE fullLiquidState;
    actionResult = ServiceImplLiquidAuto_EnforcePureWaterSupplementOnce(&lowLiquidState,&fullLiquidState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)lowLiquidState;
    returnData[1] = (uint8_t)fullLiquidState;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//液路自动化使能自动排废液
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnableWasteDischargeAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_EnableWasteDischargeAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//液路自动化使能自动三针清洗稀释
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnableThreeNeedleCleanDiluentAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_EnableThreeNeedleCleanDiluentAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//液路自动化使能纯化水自动补充
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_EnablePureWaterSupplementAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_EnablePureWaterSupplementAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//液路自动化关闭自动排废液
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_DisableWasteDischargeAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_DisableWasteDischargeAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//液路自动化关闭自动三针清洗稀释
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_DisableThreeNeedleCleanDiluentAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_DisableThreeNeedleCleanDiluentAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//液路自动化关闭自动补纯化水
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_DisablePureWaterSupplementAuto(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_DisablePureWaterSupplementAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//读取特定输入
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,inputIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE inputState;
    actionResult = ServiceImplLiquidAuto_InputRead(inputIndex,&inputState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)inputIndex;
    returnData[1] = (uint8_t)inputState;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//写入特定输出
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,outputIndex,outputState,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_OutputWrite(outputIndex,outputState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)outputIndex;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//特定电机复位
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,motorIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_MotorReset(motorIndex,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)motorIndex;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//特定电机走指定步数
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_MotorRunSteps(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,motorIndex,motorDir,motorSteps,0,0);
    //程序进行处理,调用真实实现
    int32_t posAfterRun = 0;
    actionResult = ServiceImplLiquidAuto_MotorRunSteps(motorIndex,motorSteps,&posAfterRun,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[5] = {0};
    //电机序号
    returnData[0] = motorIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,1,posAfterRun);
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,5,returnData);
    //返回错误代码
    return actionResult;
}


//液路自动化配置瓶注纯水一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_ConfigAddPureWaterOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    uint32_t sensorPulseCount;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
        //目标脉冲数
        uint32_t pulseMaxCount = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->can1RecvDataBuffer,0);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_ConfigAddPureWaterOnce(pulseMaxCount,&sensorPulseCount,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[4] = {0};
    Can1ProcUtilConvertUint32ToArray(returnData,0,sensorPulseCount);
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,4,returnData);
    //返回错误代码
    return actionResult;
}
//液路自动化配置瓶注浓缩液一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_ConfigAddConcentrateOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;

    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
        //浓缩液瓶号
        uint8_t concentrateBottle = can1CommandPtr->can1RecvDataBuffer[0];
        //注液时间
        uint16_t injectMaxTime = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->can1RecvDataBuffer,1);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_ConfigAddConcentrateOnce(concentrateBottle,injectMaxTime,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}
//液路自动化配置瓶向工作瓶抽一次
LH_ERR TaskFuncServiceLiquidAuto_Can1Msg_Proc_ConfigToTargetOnce(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    CONFIG_TO_TARGET_TRIG_FLAG trigFlag = CONFIG_TO_TARGET_TRIG_FLAG_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_ConfigToTargetOnce(&trigFlag,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = trigFlag;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}


/*************************************************2. 来自自身的消息******************************************/
//使能自动抽废液
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnableWasteDischargeAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//使能自动三针清洗液稀释
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnableThreeNeedleCleanDiluentAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//使能自动纯水补充
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnablePureWaterSupplementAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//禁止自动抽废液
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_DisableWasteDischargeAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//禁止自动三针清洗液稀释
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_DisableThreeNeedleCleanDiluentAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//禁止纯水自动补充
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_DisablePureWaterSupplementAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化强制三针清洗液稀释一次
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnforceThreeNeedleCleanDiluentOnce(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//液路自动化强制补充存水一次
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnforcePureWaterSupplementOnce(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//设定三针自动稀释浓缩瓶号
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_SetThreeNeedleDiluentConcentrateBottle(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);

//自身指令回调函数数组
static const SERVICE_TASK_SELF_PROC_UNIT ServiceSelfMsgProcUnitArray_LiquidAuto[] = {
	{TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnableWasteDischargeAuto                    ,   "EnableWasteDischargeAuto"                  },
    {TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnableThreeNeedleCleanDiluentAuto           ,   "EnableThreeNeedleCleanDiluentAuto"         },
    {TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnablePureWaterSupplementAuto               ,   "EnablePureWaterSupplementAuto"             },
    {TaskFuncServiceLiquidAuto_SelfMsg_Proc_DisableWasteDischargeAuto                   ,   "DisableWasteDischargeAuto"                 },
    {TaskFuncServiceLiquidAuto_SelfMsg_Proc_DisableThreeNeedleCleanDiluentAuto          ,   "DisableThreeNeedleCleanDiluentAuto"        },
    {TaskFuncServiceLiquidAuto_SelfMsg_Proc_DisablePureWaterSupplementAuto              ,   "DisablePureWaterSupplementAuto"            },
    {TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnforceThreeNeedleCleanDiluentOnce          ,   "EnforceThreeNeedleCleanDiluentOnce"        },
    {TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnforcePureWaterSupplementOnce              ,   "EnforcePureWaterSupplementOnce"            },
    {TaskFuncServiceLiquidAuto_SelfMsg_Proc_SetThreeNeedleDiluentConcentrateBottle      ,   "SetThreeNeedleDiluentConcentrateBottle"    },
};
//消息处理映射函数表的元素数量
#define COUNT_SELF_CMD_PROC_UNIT_ARRAY_LIQUID_AUTO      (sizeof(ServiceSelfMsgProcUnitArray_LiquidAuto)/sizeof(ServiceSelfMsgProcUnitArray_LiquidAuto[0]))

//自身指令回调处理函数,解析发送过来的参数并调用映射表中的函数
static void TaskFuncServiceLiquidAuto_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCmdPtr->commandDataSpecialChannel);
	
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= COUNT_SELF_CMD_PROC_UNIT_ARRAY_LIQUID_AUTO)
	{
		ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)"Unsupport",
				selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
				selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
				);
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
		//回调函数
		actionResult = ServiceSelfMsgProcUnitArray_LiquidAuto[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                                        ServiceSelfMsgProcUnitArray_LiquidAuto[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//打印结束处理信息
	ServicePrintfShowEnd((uint8_t*)DebugStringEnd_LiquidAuto,actionResult);

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

//使能自动抽废液
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnableWasteDischargeAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_EnableWasteDischargeAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//使能自动三针清洗液稀释
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnableThreeNeedleCleanDiluentAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_EnableThreeNeedleCleanDiluentAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//使能自动存水补充
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnablePureWaterSupplementAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_EnablePureWaterSupplementAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//禁止自动抽废液
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_DisableWasteDischargeAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_DisableWasteDischargeAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//禁止自动三针清洗液稀释
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_DisableThreeNeedleCleanDiluentAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_DisableThreeNeedleCleanDiluentAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//禁止纯水自动补充
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_DisablePureWaterSupplementAuto(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplLiquidAuto_DisablePureWaterSupplementAuto(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//液路自动化强制三针清洗液稀释一次
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnforceThreeNeedleCleanDiluentOnce(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCommandPtr->commandDataSpecialChannel);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE bottleStateA;
    SENSOR_STATE bottleStateB;
    SENSOR_STATE bottleDiluentTransferFull;
    SENSOR_STATE bottleDiluentTransferEmpty;
    SENSOR_STATE bottleDiluentTargetFull;
    SENSOR_STATE bottleDiluentTargetEmpty;
    actionResult = ServiceImplLiquidAuto_EnforceThreeNeedleCleanDiluentOnce(&bottleStateA,&bottleStateB,&bottleDiluentTransferFull,&bottleDiluentTransferEmpty,
                                                                                &bottleDiluentTargetFull,&bottleDiluentTargetEmpty,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//液路自动化强制补充存水一次
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_EnforcePureWaterSupplementOnce(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE lowLiquidState;
    SENSOR_STATE fullLiquidState;
    actionResult = ServiceImplLiquidAuto_EnforcePureWaterSupplementOnce(&lowLiquidState,&fullLiquidState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//设定三针自动稀释浓缩瓶号
static LH_ERR TaskFuncServiceLiquidAuto_SelfMsg_Proc_SetThreeNeedleDiluentConcentrateBottle(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_LIQUID_AUTO* cmdResultPtr = (IPC_DATA_LIQUID_AUTO*)(selfCommandPtr->commandDataSpecialChannel);
    //浓缩液瓶号
    CONCENTRATE_BOTTLE_SELECT concentrateBottleNumber = (CONCENTRATE_BOTTLE_SELECT)selfCommandPtr->commandParam1;
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_LiquidAuto,(uint8_t*)detailDebugMsgPtr,concentrateBottleNumber,0,0,0,0);
    //程序进行处理,调用真实实现
    ServiceImplLiquidAuto_SetThreeNeedleConcentrateBottleNumber(concentrateBottleNumber,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}


