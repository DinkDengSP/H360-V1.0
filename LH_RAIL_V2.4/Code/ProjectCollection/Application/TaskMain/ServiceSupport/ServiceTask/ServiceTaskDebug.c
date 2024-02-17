/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:21:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-15 11:12:42
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceTaskDebug.c
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
CPU_STK stackBufferTaskServiceDebug[STK_SIZE_TASK_SERVICE_DEBUG]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceDebug@".dtcm_ram";

//自动运行程序的执行周期,单位毫秒
#define DEBUG_AUTO_FUNC_RUNNING_PERIOD_MS            0
//系统循环自动程序初始化
static void ServiceTaskFuncDebug_Auto_Init(void);
#if(DEBUG_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void ServiceTaskFuncDebug_Auto_Proc(void);
#endif


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncDebug_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr);
//2. 来自自身的消息
static void ServiceTaskFuncDebug_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceDebug(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    ServiceTaskFuncDebug_Auto_Init();
    #if(DEBUG_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(DEBUG_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= DEBUG_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了DEBUG_AUTO_FUNC_RUNNING_PERIOD_MS个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			ServiceTaskFuncDebug_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)DEBUG_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                ServiceTaskFuncDebug_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncDebug_SelfMsg_Proc(selfCmdDataPtr);
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
static void ServiceTaskFuncDebug_Auto_Init(void)
{
    ServiceImplDebug_EventInit();
}
#if(DEBUG_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统循环运行程序,处理自动化任务
static void ServiceTaskFuncDebug_Auto_Proc(void)
{
    ServiceImplDebug_EventRun(DEBUG_AUTO_FUNC_RUNNING_PERIOD_MS);
}
#endif



/*************************************************1. 来自CAN1的消息******************************************/
//步进电机复位
void ServiceTaskFuncDebug_Can1Msg_Proc_StepMotorReset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//步进电机走位
void ServiceTaskFuncDebug_Can1Msg_Proc_StepMotorRunSteps(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//输入读取
void ServiceTaskFuncDebug_Can1Msg_Proc_InputRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//输出写入
void ServiceTaskFuncDebug_Can1Msg_Proc_OutputWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//参数读取
void ServiceTaskFuncDebug_Can1Msg_Proc_ParamRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//参数写入
void ServiceTaskFuncDebug_Can1Msg_Proc_ParamWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取版本号
void ServiceTaskFuncDebug_Can1Msg_Proc_ReadVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取电机参数
void ServiceTaskFuncDebug_Can1Msg_Proc_ReadMotorParam(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//写入电机参数
void ServiceTaskFuncDebug_Can1Msg_Proc_WriteMotorParam(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//握手
void ServiceTaskFuncDebug_Can1Msg_Proc_HandShake(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//进入IAP模式
void ServiceTaskFuncDebug_Can1Msg_Proc_EnterIapMode(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//IAP数据传输
void ServiceTaskFuncDebug_Can1Msg_Proc_IapTransData(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//IAP开始
void ServiceTaskFuncDebug_Can1Msg_Proc_IapStart(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//查询IAP结果
void ServiceTaskFuncDebug_Can1Msg_Proc_CheckIapResult(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//软件即停
void ServiceTaskFuncDebug_Can1Msg_Proc_EmergencyStop(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);


//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT DebugCan1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_DEBUG_ASSIST_STEP_MOTOR_RESET         ,ServiceTaskFuncDebug_Can1Msg_Proc_StepMotorReset       ,"StepMotorReset"       },
    {CAN1_CMD_DEBUG_ASSIST_STEP_MOTOR_RUN_STEPS     ,ServiceTaskFuncDebug_Can1Msg_Proc_StepMotorRunSteps    ,"StepMotorRunSteps"    },
    {CAN1_CMD_DEBUG_ASSIST_INPUT_READ               ,ServiceTaskFuncDebug_Can1Msg_Proc_InputRead            ,"InputRead"            },
    {CAN1_CMD_DEBUG_ASSIST_OUTPUT_WRITE             ,ServiceTaskFuncDebug_Can1Msg_Proc_OutputWrite          ,"OutputWrite"          },
    {CAN1_CMD_DEBUG_ASSIST_PARAM_READ               ,ServiceTaskFuncDebug_Can1Msg_Proc_ParamRead            ,"ParamRead"            },
    {CAN1_CMD_DEBUG_ASSIST_PARAM_WRITE              ,ServiceTaskFuncDebug_Can1Msg_Proc_ParamWrite           ,"ParamWrite"           },
    {CAN1_CMD_DEBUG_ASSIST_READ_VERSION             ,ServiceTaskFuncDebug_Can1Msg_Proc_ReadVersion          ,"ReadVersion"          },
    {CAN1_CMD_DEBUG_ASSIST_READ_MOTOR_PARAM         ,ServiceTaskFuncDebug_Can1Msg_Proc_ReadMotorParam       ,"ReadMotorParam"       },
    {CAN1_CMD_DEBUG_ASSIST_WRITE_MOTOR_PARAM        ,ServiceTaskFuncDebug_Can1Msg_Proc_WriteMotorParam      ,"WriteMotorParam"      },
    {CAN1_CMD_DEBUG_ASSIST_HAND_SHAKE               ,ServiceTaskFuncDebug_Can1Msg_Proc_HandShake            ,"HandShake"            },
    {CAN1_CMD_DEBUG_ASSIST_ENTER_IAP_MODE           ,ServiceTaskFuncDebug_Can1Msg_Proc_EnterIapMode         ,"EnterIapMode"         },
    {CAN1_CMD_DEBUG_ASSIST_IAP_TRANS_DATA           ,ServiceTaskFuncDebug_Can1Msg_Proc_IapTransData         ,"IapTransData"         },
    {CAN1_CMD_DEBUG_ASSIST_IAP_START                ,ServiceTaskFuncDebug_Can1Msg_Proc_IapStart             ,"IapStart"             },
    {CAN1_CMD_DEBUG_ASSIST_CHECK_IAP_RESULT         ,ServiceTaskFuncDebug_Can1Msg_Proc_CheckIapResult       ,"CheckIapResult"       },
    {CAN1_CMD_DEBUG_ASSIST_EMERGENCY_STOP           ,ServiceTaskFuncDebug_Can1Msg_Proc_EmergencyStop        ,"EmergencyStop"        },
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_DEBUG_CMD_COUNT        (sizeof(DebugCan1CommandProcFuncPtrArray)/sizeof(DebugCan1CommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncDebug_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_DEBUG_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == DebugCan1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(DebugCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                DebugCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,DebugCan1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//步进电机复位
void ServiceTaskFuncDebug_Can1Msg_Proc_StepMotorReset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,motorIndex,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugMotorReset(motorIndex,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[1] = {0};
    returnData[0] = motorIndex;
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_STEP_MOTOR_RESET,can1Error,reportLevelResult,actionResult,1,returnData);
}

//步进电机走位
void ServiceTaskFuncDebug_Can1Msg_Proc_StepMotorRunSteps(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    //运行曲线
    uint8_t curveS = can1CommandPtr->recvDataBuffer[6];
    //运动完坐标
    int32_t posAfterRun = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,motorIndex,dir,steps,curveS,0);
    //调用实现函数
    actionResult = ServiceImplDebugMotorRunSteps(motorIndex,dir,steps,curveS,&posAfterRun,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[5] = {0};
    returnData[0] = motorIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,1,posAfterRun);
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_STEP_MOTOR_RUN_STEPS,can1Error,reportLevelResult,actionResult,5,returnData);
}

//输入读取
void ServiceTaskFuncDebug_Can1Msg_Proc_InputRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //输入序号
    uint16_t pinIndex = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,0);
    //读取到的输入值
    uint8_t pinValue = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,pinIndex,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugInputRead(pinIndex,&pinValue,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[3] = {0};
    Can1ProcUtilConvertUint16ToArray(returnData,0,pinIndex);
    returnData[2] = pinValue;
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_INPUT_READ,can1Error,reportLevelResult,actionResult,3,returnData);
}

//输出写入
void ServiceTaskFuncDebug_Can1Msg_Proc_OutputWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //输出序号
    uint16_t pinIndex = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,0);
    //输出值
    uint8_t pinValue = can1CommandPtr->recvDataBuffer[2];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,pinIndex,pinValue,0,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugOutputWrite(pinIndex,pinValue,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[2] = {0};
    Can1ProcUtilConvertUint16ToArray(returnData,0,pinIndex);
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_OUTPUT_WRITE,can1Error,reportLevelResult,actionResult,2,returnData);
}

//参数读取
void ServiceTaskFuncDebug_Can1Msg_Proc_ParamRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    uint8_t mainIndex = can1CommandPtr->recvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->recvDataBuffer[1];
    int32_t paramRead = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugReadSystemParam(mainIndex,subIndex,&paramRead,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    uint8_t returnData[6] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,2,paramRead);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_PARAM_READ,can1Error,reportLevelResult,actionResult,6,returnData);
}

//参数写入
void ServiceTaskFuncDebug_Can1Msg_Proc_ParamWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    uint8_t mainIndex = can1CommandPtr->recvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->recvDataBuffer[1];
    int32_t paramWrite = Can1ProcUtilConvertArrayToInt32(can1CommandPtr->recvDataBuffer,2);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,mainIndex,subIndex,paramWrite,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugWriteSystemParam(mainIndex,subIndex,paramWrite,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[2] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_PARAM_WRITE,can1Error,reportLevelResult,actionResult,2,returnData);
}

//读取版本号
void ServiceTaskFuncDebug_Can1Msg_Proc_ReadVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //版本信息
    BOARD_APP_VERSION  appVersion;
    //解析指令参数
    uint8_t boardNo = can1CommandPtr->recvDataBuffer[0];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,boardNo,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugReadBoardVersion(boardNo,&appVersion.mainVersion,&appVersion.subVersion,&appVersion.debugVersion,&appVersion.svnVersion,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[8] = {0};
    returnData[0] = boardNo;
    returnData[1] = appVersion.mainVersion;
    returnData[2] = appVersion.subVersion;
    returnData[3] = appVersion.debugVersion;
    Can1ProcUtilConvertUint32ToArray(returnData,4,appVersion.svnVersion);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_READ_VERSION,can1Error,reportLevelResult,actionResult,8,returnData);
}

//读取电机参数
void ServiceTaskFuncDebug_Can1Msg_Proc_ReadMotorParam(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    uint8_t mainIndex = can1CommandPtr->recvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->recvDataBuffer[1];
    int32_t paramRead = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,mainIndex,subIndex,0,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugReadMotorParam(mainIndex,subIndex,&paramRead,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[6] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    Can1ProcUtilConvertInt32ToArray(returnData,2,paramRead);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_READ_MOTOR_PARAM,can1Error,reportLevelResult,actionResult,6,returnData);
}

//写入电机参数
void ServiceTaskFuncDebug_Can1Msg_Proc_WriteMotorParam(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    uint8_t mainIndex = can1CommandPtr->recvDataBuffer[0];
    uint8_t subIndex = can1CommandPtr->recvDataBuffer[1];
    int32_t paramWrite = Can1ProcUtilConvertArrayToInt32(can1CommandPtr->recvDataBuffer,2);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,mainIndex,subIndex,paramWrite,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugWriteMotorParam(mainIndex,subIndex,paramWrite,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[2] = {0};
    returnData[0] = mainIndex;
    returnData[1] = subIndex;
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_WRITE_MOTOR_PARAM,can1Error,reportLevelResult,actionResult,2,returnData);
}

//握手
void ServiceTaskFuncDebug_Can1Msg_Proc_HandShake(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    uint8_t shakeMode = can1CommandPtr->recvDataBuffer[0];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,shakeMode,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugHandShake(shakeMode,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_HAND_SHAKE,can1Error,reportLevelResult,actionResult,1,&shakeMode);
}

//进入IAP模式
void ServiceTaskFuncDebug_Can1Msg_Proc_EnterIapMode(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplBase_UtilCommon(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_ENTER_IAP_MODE,can1Error,reportLevelResult,actionResult,0,NULL);
}

//IAP数据传输
void ServiceTaskFuncDebug_Can1Msg_Proc_IapTransData(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplBase_UtilCommon(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_IAP_TRANS_DATA,can1Error,reportLevelResult,actionResult,0,NULL);
}

//IAP开始
void ServiceTaskFuncDebug_Can1Msg_Proc_IapStart(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplBase_UtilCommon(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_IAP_START,can1Error,reportLevelResult,actionResult,0,NULL);
}

//查询IAP结果
void ServiceTaskFuncDebug_Can1Msg_Proc_CheckIapResult(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplBase_UtilCommon(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_CHECK_IAP_RESULT,can1Error,reportLevelResult,actionResult,0,NULL);
}

//软件急停
void ServiceTaskFuncDebug_Can1Msg_Proc_EmergencyStop(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplDebugEmergencyStop(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_ASSIST_EMERGENCY_STOP,can1Error,reportLevelResult,actionResult,0,NULL);
}


/*************************************************2. 来自自身的消息******************************************/
//读取版本号
static LH_ERR DebugSelfCommandProcFunc_ReadBoardVersion(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//通讯握手
static LH_ERR DebugSelfCommandProcFunc_HandShake(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//急停
static LH_ERR DebugSelfCommandProcFunc_EmergencyStop(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);

//回调函数数组
const SYSTEM_TASK_SELF_MSG_PROC_UNIT DebugSelfCommandProcFuncPtrArray[] = {
    {DebugSelfCommandProcFunc_ReadBoardVersion , "ReadBoardVersion"},
    {DebugSelfCommandProcFunc_HandShake        , "HandShake"       },
    {DebugSelfCommandProcFunc_EmergencyStop    , "EmergencyStop"   },
};
//自身支持指令数量
#define TASK_SELF_MSG_DEBUG_CMD_COUNT  ((sizeof(DebugSelfCommandProcFuncPtrArray))/(sizeof(DebugSelfCommandProcFuncPtrArray[0])))

//匹配发送过来的指令序号
static void ServiceTaskFuncDebug_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_DEBUG* selfCmdResultDataPtr = (IPC_DATA_DEBUG*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (selfCmdResultDataPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	selfCmdResultDataPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_DEBUG_CMD_COUNT)
	{
        SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,(uint8_t*)"unsupport",
                                selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                                selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                                );
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = DebugSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,&(selfCmdResultDataPtr->can1ErrorCode),&(selfCmdResultDataPtr->can1ReportErrorLevel),
                                                                                            DebugSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    SystemDebugShowMsgEnd((uint8_t*)DebugDebugEndMsg,actionResult);
	//处理完成,设置状态
	selfCmdResultDataPtr->state = STATE_IPC_CMD_COMPLETE;
	selfCmdResultDataPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(selfCmdResultDataPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//读取版本号
static LH_ERR DebugSelfCommandProcFunc_ReadBoardVersion(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    uint8_t boardNo = (uint8_t)(selfCmdPtr->commandParam1);
    //版本信息
    BOARD_APP_VERSION  appVersion;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplDebugReadBoardVersion(boardNo,&appVersion.mainVersion,&appVersion.subVersion,&appVersion.debugVersion,&appVersion.svnVersion,can1ErrorPtr,reportLevelResultPtr);
    //将函数实现的结果进行转换,该设置的状态进行设置
    if(actionResult == LH_ERR_NONE)
    {
        ServiceStateUtilWriteBoardVersion(appVersion.mainVersion,appVersion.subVersion,appVersion.debugVersion,appVersion.svnVersion);
    }
    return actionResult;
}

//通讯握手
static LH_ERR DebugSelfCommandProcFunc_HandShake(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    uint8_t shakeMode = (uint8_t)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplDebugHandShake(shakeMode,can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}

//急停
static LH_ERR DebugSelfCommandProcFunc_EmergencyStop(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)DebugDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplDebugEmergencyStop(can1ErrorPtr,reportLevelResultPtr);
    return actionResult;
}
