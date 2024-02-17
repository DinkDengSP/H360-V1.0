/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:21:47
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-15 16:51:19
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceTaskCupTrayStackManage.c
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
CPU_STK stackBufferTaskServiceCupTrayStackManage[STK_SIZE_TASK_SERVICE_CUP_TRAY_STACK_MANAGE]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceCupTrayStackManage@".dtcm_ram";


//自动运行程序的执行周期,单位毫秒
#define SERVICE_CUP_TRAY_STACK_MANAGE_AUTO_FUNC_RUNNING_PERIOD_MS            200
//系统循环自动程序初始化
static void TaskServiceCupTrayStackManage_Auto_Init(void);
#if(SERVICE_CUP_TRAY_STACK_MANAGE_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统还处理一些循环自动化程序
static void TaskServiceCupTrayStackManage_Auto_Proc(void);
#endif

//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void TaskFuncServiceCupTrayStackManage_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr);
//2. 来自自身的消息
static void TaskFuncServiceCupTrayStackManage_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceCupTrayStackManage(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //延时启动
    CoreDelayMs(3000);
    //状态机初始化
    TaskServiceCupTrayStackManage_Auto_Init();
    #if(SERVICE_CUP_TRAY_STACK_MANAGE_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(SERVICE_CUP_TRAY_STACK_MANAGE_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= SERVICE_CUP_TRAY_STACK_MANAGE_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了SERVICE_CUP_TRAY_STACK_MANAGE_AUTO_FUNC_RUNNING_PERIOD_MS个时间
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			TaskServiceCupTrayStackManage_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)SERVICE_CUP_TRAY_STACK_MANAGE_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                TaskFuncServiceCupTrayStackManage_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->commandSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->systemCommandDataPtr;
                //对这个数据进行处理
                TaskFuncServiceCupTrayStackManage_SelfMsg_Proc(selfCmdDataPtr);
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
static void TaskServiceCupTrayStackManage_Auto_Init(void)
{
    ServiceImplCupTrayStackManage_StateMachineInit();
}
#if(SERVICE_CUP_TRAY_STACK_MANAGE_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统循环自动程序
static void TaskServiceCupTrayStackManage_Auto_Proc(void)
{
    //实际循环
    ServiceImplCupTrayStackManage_StateMachineLoop();
}
#endif


/*************************************************1. 来自CAN1的消息******************************************/
//预定义CAN1消息处理函数
//查询杯栈门状态
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_QueryState(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//初始化新杯栈
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitNewStack(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//初始化空杯栈
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitEmptyStack(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//初始化推杆
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitPush(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//初始化夹手
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitClamp(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//初始化杯栈管理全部功能
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitAll(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//夹手打开
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_ClampOpen(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//夹手关闭
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_ClampClose(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//上传新盘
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_UploadNewTray(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//回收空盘
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_RecoveryEmptyTray(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//从新盘区推一个新盘到测试区
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_PushTrayToTesting(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//垃圾桶锁定上升
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_GarbageLockUp(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//垃圾桶解锁下降
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_GarbageReleaseDown(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//杯栈门打开到位的时候,新盘上升指定层
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_NewTrayRiseUpWhileOpendoorInplace(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//新盘回零
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_NewTrayReturnZero(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//杯栈门打开到位的时候,空盘上升指定层
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_EmptyTrayRiseUpWhileOpendoorInplace(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//空盘回零
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_EmptyTrayReturnZero(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取输入
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//写入输出
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//电机复位
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//电机走步数
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_MotorRunSteps(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//CAN消息ID对应的处理函数,映射成表驱动
static const SERVICE_TASK_CAN1_PROC_UNIT ServiceCan1MsgProcUnitArray_CupTrayStackManage[] = {
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_QUERY_STATE                               ,   "QueryState"                            ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_QueryState},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_INIT_NEW_STACK                            ,   "InitNewStack"                          ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitNewStack},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_INIT_EMPTY_STACK                          ,   "InitEmptyStack"                        ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitEmptyStack},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_INIT_PUSH                                 ,   "InitPush"                              ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitPush},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_INIT_CLAMP                                ,   "InitClamp"                             ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitClamp},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_INIT_ALL                                  ,   "InitAll"                               ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitAll},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_CLAMP_OPEN                                ,   "ClampOpen"                             ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_ClampOpen},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_CLAMP_CLOSE                               ,   "ClampClose"                            ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_ClampClose},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_UPLOAD_NEW_TRAY                           ,   "UploadNewTray"                         ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_UploadNewTray},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_RECOVERY_EMPTY_TRAY                       ,   "RecoveryEmptyTray"                     ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_RecoveryEmptyTray},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_PUSH_TRAY_2_TESTING                              ,   "PushTrayToTesting"                     ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_PushTrayToTesting},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_LOCK_UP                           ,   "GarbageLockUp"                         ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_GarbageLockUp},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_RELEASE_DOWN                      ,   "GarbageReleaseDown"                    ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_GarbageReleaseDown},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_NEW_TRAY_RISEUP_WHILE_OPENDOOR_INPLACE    ,   "NewTrayRiseUpWhileOpendoorInplace"     ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_NewTrayRiseUpWhileOpendoorInplace},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_NEW_TRAY_RETURN_ZERO                      ,   "NewTrayReturnZero"                     ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_NewTrayReturnZero},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_RISEUP_WHILE_OPENDOOR_INPLACE  ,   "EmptyTrayRiseUpWhileOpendoorInplace"   ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_EmptyTrayRiseUpWhileOpendoorInplace},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_RETURN_ZERO                    ,   "EmptyTrayReturnZero"                   ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_EmptyTrayReturnZero},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_INPUT_READ                                ,   "InputRead"                             ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InputRead},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_OUTPUT_WRITE                              ,   "OutputWrite"                           ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_OutputWrite},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_MOTOR_RESET                               ,   "MotorReset"                            ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_MotorReset},
    {CAN1_DEBUG_CMD_CUP_TRAY_STACK_MANAGE_MOTOR_RUN_STEP                            ,   "MotorRunSteps"                         ,        TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_MotorRunSteps},
};
//消息处理映射函数表的元素数量
#define COUNT_CAN1_CMD_PROC_UNIT_ARRAY_CUP_TRAY_STACK_MANAGE      (sizeof(ServiceCan1MsgProcUnitArray_CupTrayStackManage)/sizeof(ServiceCan1MsgProcUnitArray_CupTrayStackManage[0]))

//处理CAN1收到的任务:从映射表中查找对应的处理函数,将相应的指令转发给处理函数
static void TaskFuncServiceCupTrayStackManage_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < COUNT_CAN1_CMD_PROC_UNIT_ARRAY_CUP_TRAY_STACK_MANAGE; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == ServiceCan1MsgProcUnitArray_CupTrayStackManage[indexUtil].command)
        {
            //调用处理函数
            if(ServiceCan1MsgProcUnitArray_CupTrayStackManage[indexUtil].funcPtr != NULL)
            {
                //结果代码
                LH_ERR actionResult = LH_ERR_NONE;
                //函数调用
                actionResult = ServiceCan1MsgProcUnitArray_CupTrayStackManage[indexUtil].funcPtr(can1CommandPtr,ServiceCan1MsgProcUnitArray_CupTrayStackManage[indexUtil].debugDetailMsg);
                //打印结束处理信息
	            ServicePrintfShowEnd((uint8_t*)DebugStringEnd_CupTrayStackManage,actionResult);
                break;
            }
        }
    }
}

//查询杯栈门状态
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_QueryState(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    STACK_MANAGE_DOOR_STATE doorState;
    actionResult = ServiceImplCupTrayStackManage_QueryState(&doorState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)doorState;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//初始化新杯栈
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitNewStack(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t newTrayCount;
    SENSOR_STATE newTrayTopSensor;
    actionResult = ServiceImplCupTrayStackManage_InitNewStack(&newTrayCount,&newTrayTopSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)newTrayCount;
    returnData[1] = (uint8_t)newTrayTopSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//初始化空杯栈
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitEmptyStack(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t emptyTrayCount;
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_InitEmptyStack(&emptyTrayCount,&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)emptyTrayCount;
    returnData[1] = (uint8_t)testTrayExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//初始化推杆
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitPush(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE newTrayTopSensor;
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_InitPush(&newTrayTopSensor,&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)newTrayTopSensor;
    returnData[1] = (uint8_t)testTrayExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//初始化夹手
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitClamp(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_InitClamp(&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)testTrayExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//初始化杯栈管理全部功能
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InitAll(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t newTrayCount;
    uint8_t emptyTrayCount;
    SENSOR_STATE newTrayCheckSensor;
    SENSOR_STATE testTrayExistSensor;
    SENSOR_STATE garbage1ExistSensor;
    SENSOR_STATE garbage2ExistSensor;
    actionResult = ServiceImplCupTrayStackManage_InitAll(&newTrayCount,&emptyTrayCount,&newTrayCheckSensor,&testTrayExistSensor,&garbage1ExistSensor,
                                                            &garbage2ExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[6] = {0};
    returnData[0] = (uint8_t)newTrayCount;
    returnData[1] = (uint8_t)emptyTrayCount;
    returnData[2] = (uint8_t)newTrayCheckSensor;
    returnData[3] = (uint8_t)testTrayExistSensor;
    returnData[4] = (uint8_t)garbage1ExistSensor;
    returnData[5] = (uint8_t)garbage2ExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,6,returnData);
    //返回错误代码
    return actionResult;
}

//夹手打开
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_ClampOpen(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_ClampOpen(&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)testTrayExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//夹手关闭
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_ClampClose(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_ClampClose(&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)testTrayExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//上传新盘
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_UploadNewTray(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t newTrayCount;
    SENSOR_STATE newTrayTopSensor;
    actionResult = ServiceImplCupTrayStackManage_UploadNewTray(&newTrayCount,&newTrayTopSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)newTrayCount;
    returnData[1] = (uint8_t)newTrayTopSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//回收空盘
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_RecoveryEmptyTray(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t emptyTrayCount;
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_RecoveryEmptyTray(&emptyTrayCount,&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)emptyTrayCount;
    returnData[1] = (uint8_t)testTrayExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//从新盘区推一个新盘到测试区
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_PushTrayToTesting(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE newTrayTopSensor;
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_PushTray2Testing(&newTrayTopSensor,&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)newTrayTopSensor;
    returnData[1] = (uint8_t)testTrayExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//垃圾桶锁定上升
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_GarbageLockUp(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,garbageSelect,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE garbage1ExistSensor;
    SENSOR_STATE garbage2ExistSensor;
    actionResult = ServiceImplCupTrayStackManage_GarbageLockUp(garbageSelect,&garbage1ExistSensor,&garbage2ExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)garbage1ExistSensor;
    returnData[1] = (uint8_t)garbage2ExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//垃圾桶解锁下降
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_GarbageReleaseDown(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,garbageSelect,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE garbage1ExistSensor;
    SENSOR_STATE garbage2ExistSensor;
    actionResult = ServiceImplCupTrayStackManage_GarbageReleaseDown(garbageSelect,&garbage1ExistSensor,&garbage2ExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)garbage1ExistSensor;
    returnData[1] = (uint8_t)garbage2ExistSensor;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//杯栈门打开到位的时候,新盘上升指定层
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_NewTrayRiseUpWhileOpendoorInplace(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t layers = can1CommandPtr->can1RecvDataBuffer[0];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,layers,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplCupTrayStackManage_NewTrayRiseupWhileDoorOpenInPlace(layers,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//新盘回零
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_NewTrayReturnZero(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplCupTrayStackManage_NewTrayReturnZero(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//杯栈门打开到位的时候,空盘上升指定层
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_EmptyTrayRiseUpWhileOpendoorInplace(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t layers = can1CommandPtr->can1RecvDataBuffer[0];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,layers,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplCupTrayStackManage_EmptyTrayRiseupWhileDoorOpenInPlace(layers,&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//空盘回零
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_EmptyTrayReturnZero(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
     //程序进行处理,调用真实实现
    actionResult = ServiceImplCupTrayStackManage_EmptyTrayReturnZero(&commErrorCode,&commErrorLevel);
    //返回结果数据
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//读取输入
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_InputRead(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,inputIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE inputState;
    actionResult = ServiceImplCupTrayStackManage_InputRead(inputIndex,&inputState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[2] = {0};
    returnData[0] = (uint8_t)inputIndex;
    returnData[1] = (uint8_t)inputState;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,2,returnData);
    //返回错误代码
    return actionResult;
}

//写入输出
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_OutputWrite(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,outputIndex,outputState,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplCupTrayStackManage_OutputWrite(outputIndex,outputState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)outputIndex;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//电机复位
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_MotorReset(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,motorIndex,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplCupTrayStackManage_MotorReset(motorIndex,&commErrorCode,&commErrorLevel);
    //返回结果数据
    uint8_t returnData[1] = {0};
    returnData[0] = (uint8_t)motorIndex;
    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,1,returnData);
    //返回错误代码
    return actionResult;
}

//电机走步数
LH_ERR TaskFuncServiceCupTrayStackManage_Can1Msg_Proc_MotorRunSteps(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
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
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,motorIndex,motorDir,motorSteps,0,0);
    //程序进行处理,调用真实实现
    int32_t posAfterRun = 0;
    actionResult = ServiceImplCupTrayStackManage_MotorRunSteps(motorIndex,motorSteps,&posAfterRun,&commErrorCode,&commErrorLevel);
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
//查询杯栈管理门状态
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_QueryState(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//新杯栈初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitNewStack(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//空杯栈初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitEmptyStack(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//推手初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitPush(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//夹手初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitClamp(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//全部初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitAll(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//上传一个新盘
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_UploadNewTray(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//回收一个旧盘
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_RecoveryEmptyTray(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//从新杯托盘推一个托盘到测试区
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_PushTray2Testing(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//垃圾桶解锁上升
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_GarbageLockUp(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//垃圾桶释放下降
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_GarbageReleaseDown(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//更新传感器状态
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_ReflushSensorState(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);

//自身指令回调函数数组
static const SERVICE_TASK_SELF_PROC_UNIT ServiceSelfMsgProcUnitArray_CupTrayStackManage[] = {
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_QueryState              ,   "QueryState"        },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitNewStack            ,   "InitNewStack"      },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitEmptyStack          ,   "InitEmptyStack"    },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitPush                ,   "InitPush"          },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitClamp               ,   "InitClamp"         },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitAll                 ,   "InitAll"           },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_UploadNewTray           ,   "UploadNewTray"     },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_RecoveryEmptyTray       ,   "RecoveryEmptyTray" },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_PushTray2Testing        ,   "PushTray2Testing"  },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_GarbageLockUp           ,   "GarbageLockUp"     },
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_GarbageReleaseDown      ,   "GarbageReleaseDown"},
    {TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_ReflushSensorState      ,   "ReflushSensorState"},
};
//消息处理映射函数表的元素数量
#define COUNT_SELF_CMD_PROC_UNIT_ARRAY_CUP_TRAY_STACK_MANAGE      (sizeof(ServiceSelfMsgProcUnitArray_CupTrayStackManage)/sizeof(ServiceSelfMsgProcUnitArray_CupTrayStackManage[0]))

//自身指令回调处理函数,解析发送过来的参数并调用映射表中的函数
static void TaskFuncServiceCupTrayStackManage_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCmdPtr->commandDataSpecialChannel);
	
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= COUNT_SELF_CMD_PROC_UNIT_ARRAY_CUP_TRAY_STACK_MANAGE)
	{
		ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)"Unsupport",
				selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
				selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
				);
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
		//回调函数
		actionResult = ServiceSelfMsgProcUnitArray_CupTrayStackManage[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                                        ServiceSelfMsgProcUnitArray_CupTrayStackManage[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//打印结束处理信息
	ServicePrintfShowEnd((uint8_t*)DebugStringEnd_CupTrayStackManage,actionResult);

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

//查询杯栈管理门状态
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_QueryState(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    STACK_MANAGE_DOOR_STATE doorState;
    actionResult = ServiceImplCupTrayStackManage_QueryState(&doorState,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//新杯栈初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitNewStack(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t newTrayCount;
    SENSOR_STATE newTrayTopSensor;
    actionResult = ServiceImplCupTrayStackManage_InitNewStack(&newTrayCount,&newTrayTopSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//空杯栈初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitEmptyStack(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t emptyTrayCount;
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_InitEmptyStack(&emptyTrayCount,&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//推手初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitPush(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE newTrayTopSensor;
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_InitPush(&newTrayTopSensor,&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//夹手初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitClamp(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_InitClamp(&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//全部初始化
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_InitAll(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t newTrayCount;
    uint8_t emptyTrayCount;
    SENSOR_STATE newTrayCheckSensor;
    SENSOR_STATE testTrayExistSensor;
    SENSOR_STATE garbage1ExistSensor;
    SENSOR_STATE garbage2ExistSensor;
    actionResult = ServiceImplCupTrayStackManage_InitAll(&newTrayCount,&emptyTrayCount,&newTrayCheckSensor,&testTrayExistSensor,&garbage1ExistSensor,
                                                            &garbage2ExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//上传一个新盘
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_UploadNewTray(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t newTrayCount;
    SENSOR_STATE newTrayTopSensor;
    actionResult = ServiceImplCupTrayStackManage_UploadNewTray(&newTrayCount,&newTrayTopSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//回收一个旧盘
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_RecoveryEmptyTray(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    uint8_t emptyTrayCount;
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_RecoveryEmptyTray(&emptyTrayCount,&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//从新杯托盘推一个托盘到测试区
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_PushTray2Testing(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE newTrayTopSensor;
    SENSOR_STATE testTrayExistSensor;
    actionResult = ServiceImplCupTrayStackManage_PushTray2Testing(&newTrayTopSensor,&testTrayExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//垃圾桶解锁上升
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_GarbageLockUp(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);
    //解析和处理参数
    GARBAGE_SELECT garbageSelect = (GARBAGE_SELECT)(selfCommandPtr->commandParam1);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,garbageSelect,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE garbage1ExistSensor;
    SENSOR_STATE garbage2ExistSensor;
    actionResult = ServiceImplCupTrayStackManage_GarbageLockUp(garbageSelect,&garbage1ExistSensor,&garbage2ExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//垃圾桶释放下降
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_GarbageReleaseDown(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    GARBAGE_SELECT garbageSelect = (GARBAGE_SELECT)(selfCommandPtr->commandParam1);
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,garbageSelect,0,0,0,0);
    //程序进行处理,调用真实实现
    SENSOR_STATE garbage1ExistSensor;
    SENSOR_STATE garbage2ExistSensor;
    actionResult = ServiceImplCupTrayStackManage_GarbageReleaseDown(garbageSelect,&garbage1ExistSensor,&garbage2ExistSensor,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//更新传感器状态
static LH_ERR TaskFuncServiceCupTrayStackManage_SelfMsg_Proc_ReflushSensorState(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_CUP_TRAY_STACK_MANAGE* cmdResultPtr = (IPC_DATA_CUP_TRAY_STACK_MANAGE*)(selfCommandPtr->commandDataSpecialChannel);

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_CupTrayStackManage,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    actionResult = ServiceImplCupTrayStackManage_ReflushSensorState(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}
