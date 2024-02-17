/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:38:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-15 13:31:15
**FilePath: \Application\TaskMain\ServiceSupport\ServiceTask\ServiceTaskAssist.c
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
CPU_STK stackBufferTaskServiceAssist[STK_SIZE_TASK_SERVICE_ASSIST]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceAssist@".dtcm_ram";

/* 心跳打印输出周期 */
#define SERVICE_ASSIST_HEART_PERIOD_SECOND  600

//自动运行程序的执行周期,单位毫秒
#define SERVICE_ASSIST_AUTO_FUNC_RUNNING_PERIOD_MS            100
//系统循环自动程序初始化
static void TaskServiceAssist_Auto_Init(void);
#if(SERVICE_ASSIST_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统还处理一些循环自动化程序
static void TaskServiceAssist_Auto_Proc(void);
#endif

uint8_t programForceJumpFlag = 0;
//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void TaskFuncServiceAssist_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr);
//2. 来自自身的消息
static void TaskFuncServiceAssist_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceAssist(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    TaskServiceAssist_Auto_Init();
    #if(SERVICE_ASSIST_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    //
    if(programForceJumpFlag == 0xFF)
    {
        CoreDelayMs(1000);
        programForceJumpFlag = 0;
        //发送强制跳转结果
        Can1ProgramFoceJumpResult(1,1);
    }
    while(1)
    {
        #if(SERVICE_ASSIST_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= SERVICE_ASSIST_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了SERVICE_ASSIST_AUTO_FUNC_RUNNING_PERIOD_MS个时间
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			TaskServiceAssist_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)SERVICE_ASSIST_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                TaskFuncServiceAssist_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->commandSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->systemCommandDataPtr;
                //对这个数据进行处理
                TaskFuncServiceAssist_SelfMsg_Proc(selfCmdDataPtr);
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
uint16_t heartTimeCount = 0;
static void TaskServiceAssist_Auto_Init(void)
{
    ServiceImplAssist_StateMachineInit();
}
#if(SERVICE_ASSIST_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
static void TaskServiceAssist_Auto_Proc(void)
{
    ServiceImplAssist_StateMachineLoop();
    heartTimeCount++;
    if(heartTimeCount >= SERVICE_ASSIST_HEART_PERIOD_SECOND)
    {
        heartTimeCount = 0;
        SystemPrintf("HeartLED_TEST\r\n");
    }
}
#endif


/*************************************************1. 来自CAN1的消息******************************************/
//预定义CAN1消息处理函数
//指定系统电机复位
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//指定系统电机走步数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_MotorRunSteps(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取输入
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//写入输出
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取版本号
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ReadVersion(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取动作参数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ReadActionParam(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//写入动作参数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_WriteActionParam(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取系统状态
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ReadState(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//写入系统状态
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_WriteState(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取电机参数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ReadMotorParam(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//写入电机参数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_WriteMotorParam(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//系统握手
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_HandShake(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//下位机版进入IAP模式
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionEnterIapMode(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//下位机板传输IAP数据
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionTransIapData(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//下位机板进行IAP升级
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionIapUpdate(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//下位机板检查IAP结果
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionCheckIapResult(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//系统急停
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionEmergencyStop(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//设备信息写入与读取
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_MACHINE_MSG_RW(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//设置下位机模块运行版本
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_SetActionBoardModelVersion(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//CAN消息ID对应的处理函数,映射成表驱动
static const SERVICE_TASK_CAN1_PROC_UNIT ServiceCan1MsgProcUnitArray_Assist[] = {
    {CAN1_DEBUG_CMD_ASSIST_MOTOR_RESET                  ,   "MotorReset"            ,   TaskFuncServiceAssist_Can1Msg_Proc_MotorReset           },
    {CAN1_DEBUG_CMD_ASSIST_MOTOR_RUN_STEP               ,   "MotorRunSteps"         ,   TaskFuncServiceAssist_Can1Msg_Proc_MotorRunSteps        },
    {CAN1_DEBUG_CMD_ASSIST_INPUT_READ                   ,   "InputRead"             ,   TaskFuncServiceAssist_Can1Msg_Proc_InputRead            },
    {CAN1_DEBUG_CMD_ASSIST_OUTPUT_WRITE                 ,   "OutputWrite"           ,   TaskFuncServiceAssist_Can1Msg_Proc_OutputWrite          },
    {CAN1_DEBUG_CMD_ASSIST_READ_VERSION                 ,   "ReadVersion"           ,   TaskFuncServiceAssist_Can1Msg_Proc_ReadVersion          },
    {CAN1_DEBUG_CMD_ASSIST_READ_ACTION_PARAM            ,   "ReadActionParam"       ,   TaskFuncServiceAssist_Can1Msg_Proc_ReadActionParam      },
    {CAN1_DEBUG_CMD_ASSIST_WRITE_ACTION_PARAM           ,   "WriteActionParam"      ,   TaskFuncServiceAssist_Can1Msg_Proc_WriteActionParam     },
    {CAN1_DEBUG_CMD_ASSIST_READ_STATE                   ,   "ReadState"             ,   TaskFuncServiceAssist_Can1Msg_Proc_ReadState            },
    {CAN1_DEBUG_CMD_ASSIST_WRITE_STATE                  ,   "WriteState"            ,   TaskFuncServiceAssist_Can1Msg_Proc_WriteState           },
    {CAN1_DEBUG_CMD_ASSIST_READ_MOTOR_PARAM             ,   "ReadMotorParam"        ,   TaskFuncServiceAssist_Can1Msg_Proc_ReadMotorParam       },
    {CAN1_DEBUG_CMD_ASSIST_WRITE_MOTOR_PARAM            ,   "WriteMotorParam"       ,   TaskFuncServiceAssist_Can1Msg_Proc_WriteMotorParam      },
    {CAN1_DEBUG_CMD_ASSIST_HAND_SHAKE                   ,   "HandShake"             ,   TaskFuncServiceAssist_Can1Msg_Proc_HandShake            },
    {CAN1_DEBUG_CMD_ASSIST_ACTION_ENTER_IAP_MODE        ,   "ActionEnterIapMode"    ,   TaskFuncServiceAssist_Can1Msg_Proc_ActionEnterIapMode   },
    {CAN1_DEBUG_CMD_ASSIST_ACTION_TRANS_IAP_DATA        ,   "ActionTransIapData"    ,   TaskFuncServiceAssist_Can1Msg_Proc_ActionTransIapData   },
    {CAN1_DEBUG_CMD_ASSIST_ACTION_IAP_UPDATE            ,   "ActionIapUpdate"       ,   TaskFuncServiceAssist_Can1Msg_Proc_ActionIapUpdate      },
    {CAN1_DEBUG_CMD_ASSIST_ACTION_CHECK_IAP_RESULT      ,   "ActionCheckIapResult"  ,   TaskFuncServiceAssist_Can1Msg_Proc_ActionCheckIapResult },
    {CAN1_DEBUG_CMD_ASSIST_ACTION_EMERGENCY             ,   "ActionEmergencyStop"   ,   TaskFuncServiceAssist_Can1Msg_Proc_ActionEmergencyStop  },
    {CAN1_DEBUG_CMD_ASSIST_MACHINE_MESSAGE              ,   "MachineMessageRW"      ,   TaskFuncServiceAssist_Can1Msg_Proc_MACHINE_MSG_RW       },
    {CAN1_DEBUG_CMD_ASSIST_MACHINE_MESSAGE              ,   "SetActionBoardModelVersion"  ,   TaskFuncServiceAssist_Can1Msg_Proc_SetActionBoardModelVersion  },
};
//消息处理映射函数表的元素数量
#define COUNT_CAN1_CMD_PROC_UNIT_ARRAY_ASSIST      (sizeof(ServiceCan1MsgProcUnitArray_Assist)/sizeof(ServiceCan1MsgProcUnitArray_Assist[0]))

//处理CAN1收到的任务:从映射表中查找对应的处理函数,将相应的指令转发给处理函数
static void TaskFuncServiceAssist_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < COUNT_CAN1_CMD_PROC_UNIT_ARRAY_ASSIST; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == ServiceCan1MsgProcUnitArray_Assist[indexUtil].command)
        {
            //调用处理函数
            if(ServiceCan1MsgProcUnitArray_Assist[indexUtil].funcPtr != NULL)
            {
                //结果代码
                LH_ERR actionResult = LH_ERR_NONE;
                //函数调用
                actionResult = ServiceCan1MsgProcUnitArray_Assist[indexUtil].funcPtr(can1CommandPtr,ServiceCan1MsgProcUnitArray_Assist[indexUtil].debugDetailMsg);
                //打印结束处理信息
	            ServicePrintfShowEnd((uint8_t*)DebugStringEnd_Assist,actionResult);
                break;
            }
        }
    }
}

//指定系统电机复位
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,motorIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_MotorReset(motorIndex,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,&motorIndex);
    //返回错误代码
    return actionResult;
}

//指定系统电机走步数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_MotorRunSteps(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    uint8_t curveSelect = can1CommandPtr->can1RecvDataBuffer[6];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,motorIndex,motorDir,motorSteps,curveSelect,0);
    //程序进行处理,调用真实实现
    int32_t posAfterRun = 0;
    actionResult = ServiceImplAssist_MotorRunSteps(motorIndex,motorSteps,curveSelect,&posAfterRun,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[5] = {0};
    //电机序号
    returnData[0] = motorIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,1,posAfterRun);
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,5,returnData);
    //返回错误代码
    return actionResult;
}

//读取输入
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint16_t inputIndex = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->can1RecvDataBuffer,0);
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,inputIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    BitAction inputValue;
    actionResult = ServiceImplAssist_InputRead(inputIndex,&inputValue,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[3] = {0};
    //输入序号
    Can1ProcUtilConvertUint16ToArray(returnData,0,inputIndex);
    returnData[2] = (uint8_t)(inputValue);
    //发送
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,3,returnData);
    //返回错误代码
    return actionResult;
}

//写入输出
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint16_t outputIndex = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->can1RecvDataBuffer,0);
    BitAction setValue = (BitAction)(can1CommandPtr->can1RecvDataBuffer[2]);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,outputIndex,setValue,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_OutputWrite(outputIndex,setValue,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    Can1ProcUtilConvertUint16ToArray(returnData,0,outputIndex);
    //发送
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//读取版本号
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ReadVersion(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    VERSION_BOARD_ID boardID = (VERSION_BOARD_ID)(can1CommandPtr->can1RecvDataBuffer[0]);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,boardID,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t mainVersion = 0;
    uint8_t subVersion = 0;
    uint8_t debugVersion = 0;
    uint32_t svnVersion = 0;
    actionResult = ServiceImplAssist_ReadSoftwareVersion(boardID,&mainVersion,&subVersion,&debugVersion,&svnVersion,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[8] = {0};
    //板序号
    returnData[0] = (uint8_t)(boardID);
    //版本号写入
    returnData[1] = mainVersion;
    returnData[2] = subVersion;
    returnData[3] = debugVersion;
    Can1ProcUtilConvertUint32ToArray(returnData,4,svnVersion);
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,8,returnData);
    //返回错误代码
    return actionResult;
}

//读取动作参数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ReadActionParam(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t mainIndex = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->can1RecvDataBuffer[1];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,mainIndex,subIndex,0,0,0);
    //程序进行处理,调用真实实现
    int32_t paramResult = 0;
    actionResult = ServiceImplAssist_ReadActionParam(mainIndex,subIndex,&paramResult,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[6] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,2,paramResult);
    //发送
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,6,returnData);
    //返回错误代码
    return actionResult;
}

//写入动作参数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_WriteActionParam(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t mainIndex = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->can1RecvDataBuffer[1];
    int32_t writeParam = Can1ProcUtilConvertArrayToInt32(can1CommandPtr->can1RecvDataBuffer,2);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,mainIndex,subIndex,writeParam,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_WriteActionParam(mainIndex,subIndex,writeParam,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    //发送
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//读取系统状态
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ReadState(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t mainIndex = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->can1RecvDataBuffer[1];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,mainIndex,subIndex,0,0,0);
    //程序进行处理,调用真实实现
    int32_t stateResult = 0;
    actionResult = ServiceImplAssist_ReadSystemState(mainIndex,subIndex,&stateResult,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[6] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,2,stateResult);
    //发送
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,6,returnData);
    //返回错误代码
    return actionResult;
}

//写入系统状态
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_WriteState(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //解析和处理参数
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t mainIndex = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->can1RecvDataBuffer[1];
    int32_t writeState = Can1ProcUtilConvertArrayToInt32(can1CommandPtr->can1RecvDataBuffer,2);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,mainIndex,subIndex,writeState,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_WriteSystemState(mainIndex,subIndex,writeState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    //发送
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//读取电机参数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ReadMotorParam(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t mainIndex = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->can1RecvDataBuffer[1];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,mainIndex,subIndex,0,0,0);
    //程序进行处理,调用真实实现
    int32_t paramResult = 0;
    actionResult = ServiceImplAssist_ReadMotorParam(mainIndex,subIndex,&paramResult,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[6] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,2,paramResult);
    //发送
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,6,returnData);
    //返回错误代码
    return actionResult;
}

//写入电机参数
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_WriteMotorParam(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t mainIndex = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->can1RecvDataBuffer[1];
    int32_t writeParam = Can1ProcUtilConvertArrayToInt32(can1CommandPtr->can1RecvDataBuffer,2);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,mainIndex,subIndex,writeParam,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_WriteMotorParam(mainIndex,subIndex,writeParam,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    //发送
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//系统握手
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_HandShake(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    MODE_RUNNING shakeMode = (MODE_RUNNING)(can1CommandPtr->can1RecvDataBuffer[0]);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,shakeMode,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_HandShake(shakeMode,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)shakeMode;
    //发送
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//下位机版进入IAP模式
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionEnterIapMode(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_EnterIapMode(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//下位机板传输IAP数据
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionTransIapData(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint32_t startAddr = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->can1RecvDataBuffer,0);
    uint16_t dataLength = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->can1RecvDataBuffer,4);
    uint8_t* dataBuffer = (can1CommandPtr->can1RecvDataBuffer) + 6;
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_TransIapData(startAddr,dataLength,dataBuffer,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//下位机板进行IAP升级
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionIapUpdate(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_IapUpdate(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//下位机板检查IAP结果
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionCheckIapResult(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_CheckIapResult(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//系统急停
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_ActionEmergencyStop(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_SystemEmergencyStop(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}
//设备信息写入与读取
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_MACHINE_MSG_RW(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    uint8_t *msgBufPtr = NULL;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
        //读写标记
        uint8_t readWriteFlag = (uint8_t)can1CommandPtr->can1RecvDataBuffer[0];
        //读写偏移
        uint16_t readWriteOffset = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->can1RecvDataBuffer,1);
        //读写长度
        uint16_t msgLength = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->can1RecvDataBuffer,3);
    //申请内存
    do
    {
        msgBufPtr = UserMemMalloc(MEM_AXI_SRAM,1029);
        if(msgBufPtr == NULL)
        {
            CoreDelayMinTick();
        }
    }while(msgBufPtr == NULL);
    //将要写入的数据保存在缓存
    if(readWriteFlag == 1)
    {
        UserMemCopy(&msgBufPtr[5],&can1CommandPtr->can1RecvDataBuffer[5],msgLength);
    }
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,readWriteFlag,readWriteOffset,msgLength,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_MachineMsgReadWrite(&msgBufPtr[5],readWriteFlag,readWriteOffset,msgLength,&commErrorCode,&commErrorLevel);
    //返回结果数据
    if(readWriteFlag == 0)
    {
        //读设备信息返回
        msgBufPtr[0] = readWriteFlag;
        *(uint16_t *)&msgBufPtr[1] = readWriteOffset;
        *(uint16_t *)&msgBufPtr[3] = msgLength;
        Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,msgLength+5,msgBufPtr);
    }
    else
    {
        msgBufPtr[0] = readWriteFlag;
        *(uint16_t *)&msgBufPtr[1] = readWriteOffset;
        *(uint16_t *)&msgBufPtr[3] = msgLength;
        //写设备信息返回
        Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,5,msgBufPtr); 
    }
    UserMemFree(MEM_AXI_SRAM,msgBufPtr);
    //返回错误代码
    return actionResult;
}

//设置下位机模块运行版本
LH_ERR TaskFuncServiceAssist_Can1Msg_Proc_SetActionBoardModelVersion(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_SetActionBoardModelVersion(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL); 
    //返回错误代码
    return actionResult;
}

/*************************************************2. 来自自身的消息******************************************/
//读取软件版本
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_ReadSoftwareVersion(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//读取系统状态
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_ReadSystemState(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//握手
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_HandShake(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//系统紧急急停
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_SystemEmergencyStop(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//读取动作参数
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_ReadActionParam(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//设置下位机模块运行版本
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_SetActionBoardModelVersion(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);

//自身指令回调函数数组
static const SERVICE_TASK_SELF_PROC_UNIT ServiceSelfMsgProcUnitArray_Assist[] = {
	{TaskFuncServiceAssist_SelfMsg_Proc_ReadSoftwareVersion         ,   "ReadSoftwareVersion"   },
    {TaskFuncServiceAssist_SelfMsg_Proc_ReadSystemState             ,   "ReadSystemState"       },
    {TaskFuncServiceAssist_SelfMsg_Proc_HandShake                   ,   "HandShake"             },
    {TaskFuncServiceAssist_SelfMsg_Proc_SystemEmergencyStop         ,   "SystemEmergencyStop"   },
    {TaskFuncServiceAssist_SelfMsg_Proc_ReadActionParam             ,   "ReadActionParam"       },
    {TaskFuncServiceAssist_SelfMsg_Proc_SetActionBoardModelVersion  ,   "SetActionBoardModelVersion" },
};
//消息处理映射函数表的元素数量
#define COUNT_SELF_CMD_PROC_UNIT_ARRAY_ASSIST      (sizeof(ServiceSelfMsgProcUnitArray_Assist)/sizeof(ServiceSelfMsgProcUnitArray_Assist[0]))

//自身指令回调处理函数,解析发送过来的参数并调用映射表中的函数
static void TaskFuncServiceAssist_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_ASSIST* cmdResultPtr = (IPC_DATA_ASSIST*)(selfCmdPtr->commandDataSpecialChannel);
	
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= COUNT_SELF_CMD_PROC_UNIT_ARRAY_ASSIST)
	{
		ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)"Unsupport",
				selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
				selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
				);
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
		//回调函数
		actionResult = ServiceSelfMsgProcUnitArray_Assist[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                                        ServiceSelfMsgProcUnitArray_Assist[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//打印结束处理信息
	ServicePrintfShowEnd((uint8_t*)DebugStringEnd_Assist,actionResult);

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

//读取软件版本
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_ReadSoftwareVersion(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    
    //板号
    VERSION_BOARD_ID boardID = (VERSION_BOARD_ID)(selfCommandPtr->commandParam1);
    //版本号
    uint8_t mainVersion = 0;
    uint8_t subVersion = 0;
    uint8_t debugVersion = 0;
    uint32_t svnVersion = 0;
    
    //解析和处理参数
    IPC_DATA_ASSIST* cmdResultPtr = (IPC_DATA_ASSIST*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,boardID,0,0,0,0);

    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_ReadSoftwareVersion(boardID,&mainVersion,&subVersion,&debugVersion,&svnVersion,&commErrorCode,&commErrorLevel);
    //中位机错误码
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//读取系统状态
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_ReadSystemState(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    int32_t stateRead = 0;
    //解析和处理参数
	IPC_DATA_ASSIST* cmdResultPtr = (IPC_DATA_ASSIST*)(selfCommandPtr->commandDataSpecialChannel);
    uint8_t mainIndex = (uint8_t)(selfCommandPtr->commandParam1);
    uint8_t subIndex = (uint8_t)(selfCommandPtr->commandParam2);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,mainIndex,subIndex,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_ReadSystemState(mainIndex,subIndex,&stateRead,&commErrorCode,&commErrorLevel);
    //返回结果数据
    
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//握手
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_HandShake(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_ASSIST* cmdResultPtr = (IPC_DATA_ASSIST*)(selfCommandPtr->commandDataSpecialChannel);
    MODE_RUNNING handShakeMode = (MODE_RUNNING)(selfCommandPtr->commandParam1);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,handShakeMode,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_HandShake(handShakeMode,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//系统紧急急停
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_SystemEmergencyStop(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_ASSIST* cmdResultPtr = (IPC_DATA_ASSIST*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_SystemEmergencyStop(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//读取动作参数
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_ReadActionParam(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    //主索引
	IPC_DATA_ASSIST* cmdResultPtr = (IPC_DATA_ASSIST*)(selfCommandPtr->commandDataSpecialChannel);
    uint8_t mainIndex = (uint8_t)(selfCommandPtr->commandParam1);
    //子索引
    uint8_t subIndex = (uint8_t)(selfCommandPtr->commandParam2);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,mainIndex,subIndex,0,0,0);
    //程序进行处理,调用真实实现
    int32_t paramResult = 0;
    actionResult = ServiceImplAssist_ReadActionParam(mainIndex,subIndex,&paramResult,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    cmdResultPtr->liquidDetectPos = paramResult;
    //返回错误代码
    return actionResult;
}


//设置下位机模块运行版本
static LH_ERR TaskFuncServiceAssist_SelfMsg_Proc_SetActionBoardModelVersion(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_ASSIST* cmdResultPtr = (IPC_DATA_ASSIST*)(selfCommandPtr->commandDataSpecialChannel);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_Assist,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplAssist_SetActionBoardModelVersion(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

