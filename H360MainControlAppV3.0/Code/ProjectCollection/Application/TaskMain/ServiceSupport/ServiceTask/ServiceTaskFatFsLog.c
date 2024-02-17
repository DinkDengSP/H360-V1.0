#include "ServiceTaskConfig.h"
#include "ServiceIpcInc.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskServiceFatFsLog[STK_SIZE_TASK_SERVICE_FATFS_LOG]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceFatFsLog@".dtcm_ram";


//自动运行程序的执行周期,单位毫秒
#define SERVICE_FATFS_LOG_AUTO_FUNC_RUNNING_PERIOD_MS      0
//系统循环自动程序初始化
static void TaskServiceFatFsLog_Auto_Init(void);
#if(SERVICE_FATFS_LOG_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统还处理一些循环自动化程序
static void TaskServiceFatFsLog_Auto_Proc(void);
#endif

//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void TaskFuncServiceFatFsLog_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr);
//2. 来自自身的消息
static void TaskFuncServiceFatFsLog_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceFatFsLog(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    TaskServiceFatFsLog_Auto_Init();
    //文件系统相关数据初始化
    ServiceImplFatFsLog_DataInit();
    #if(SERVICE_FATFS_LOG_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(SERVICE_FATFS_LOG_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= SERVICE_FATFS_LOG_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了SERVICE_FATFS_LOG_AUTO_FUNC_RUNNING_PERIOD_MS个时间
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			TaskServiceFatFsLog_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)SERVICE_FATFS_LOG_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                TaskFuncServiceFatFsLog_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->commandSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->systemCommandDataPtr;
                //对这个数据进行处理
                TaskFuncServiceFatFsLog_SelfMsg_Proc(selfCmdDataPtr);
                //处理完成
                UserMemFree(MEM_DTCM,selfCmdDataPtr);
                //ActionPrintf("TaskMax:%d,")
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
static void TaskServiceFatFsLog_Auto_Init(void)
{

}
#if(SERVICE_FATFS_LOG_AUTO_FUNC_RUNNING_PERIOD_MS != 0)
//系统循环自动程序
static void TaskServiceFatFsLog_Auto_Proc(void)
{

}
#endif



/*************************************************1. 来自CAN1的消息******************************************/
//获取文件列表
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_GetFileList(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//准备读日志内容
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_PrepareReadFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读日志
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_ReadFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//结束读日志内容
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_EndReadFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//删除指定日志
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_DeleteTargetFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//删除全部日志
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_DeleteAllFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取日志大小
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_GetTargetFileSize(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//CAN消息ID对应的处理函数,映射成表驱动
static const SERVICE_TASK_CAN1_PROC_UNIT ServiceCan1MsgProcUnitArray_FatFsLog[] = { 
    {CAN1_DEBUG_CMD_FATFS_GET_FILE_LIST             ,   "GetFileList"               , TaskFuncServiceFatFsLog_Can1Msg_Proc_GetFileList      },
    {CAN1_DEBUG_CMD_FATFS_PREPARE_READ_FILE         ,   "PrepareReadFile"           , TaskFuncServiceFatFsLog_Can1Msg_Proc_PrepareReadFile  },
    {CAN1_DEBUG_CMD_FATFS_READ_FILE                 ,   "ReadFile"                  , TaskFuncServiceFatFsLog_Can1Msg_Proc_ReadFile         },
    {CAN1_DEBUG_CMD_FATFS_END_READ_FILE             ,   "EndReadFile"               , TaskFuncServiceFatFsLog_Can1Msg_Proc_EndReadFile      },
    {CAN1_DEBUG_CMD_FATFS_DELETE_TARGET_FILE        ,   "DeleteTargetFile"          , TaskFuncServiceFatFsLog_Can1Msg_Proc_DeleteTargetFile },
    {CAN1_DEBUG_CMD_FATFS_DELETE_ALL_FILE           ,   "DeleteAllFile"             , TaskFuncServiceFatFsLog_Can1Msg_Proc_DeleteAllFile    },
    {CAN1_DEBUG_CMD_FATFS_GET_TARGET_FILE_SIZE      ,   "GetTargetFileSize"         , TaskFuncServiceFatFsLog_Can1Msg_Proc_GetTargetFileSize},
};
//消息处理映射函数表的元素数量
#define COUNT_CAN1_CMD_PROC_UNIT_ARRAY_FATFS_LOG      (sizeof(ServiceCan1MsgProcUnitArray_FatFsLog)/sizeof(ServiceCan1MsgProcUnitArray_FatFsLog[0]))

//处理CAN1收到的任务:从映射表中查找对应的处理函数,将相应的指令转发给处理函数
static void TaskFuncServiceFatFsLog_Can1Msg_Proc(SYSTEM_CMD_CAN1* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < COUNT_CAN1_CMD_PROC_UNIT_ARRAY_FATFS_LOG; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == ServiceCan1MsgProcUnitArray_FatFsLog[indexUtil].command)
        {
            //调用处理函数
            if(ServiceCan1MsgProcUnitArray_FatFsLog[indexUtil].funcPtr != NULL)
            {
                //结果代码
                LH_ERR actionResult = LH_ERR_NONE;
                //函数调用
                actionResult = ServiceCan1MsgProcUnitArray_FatFsLog[indexUtil].funcPtr(can1CommandPtr,ServiceCan1MsgProcUnitArray_FatFsLog[indexUtil].debugDetailMsg);
                //打印结束处理信息
	            ServicePrintfShowEnd((uint8_t*)DebugStringEnd_FatFsLog,actionResult);
                break;
            }
        }
    }
}

//获取文件列表
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_GetFileList(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t *fileNameBufferPtr = NULL;
    uint16_t bufferLength = 0;
    uint8_t fileOffset = can1CommandPtr->can1RecvDataBuffer[0];
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)detailDebugMsgPtr,fileOffset,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplFatFsLog_GetFileList(fileOffset,&fileNameBufferPtr,&bufferLength,&commErrorCode,&commErrorLevel);
    //返回结果数据
    if(actionResult == LH_ERR_NONE)
    {
        Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,bufferLength,fileNameBufferPtr);
    }
    else
    {
        Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    }
    //释放内存
    if(fileNameBufferPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,fileNameBufferPtr);
    }
    //返回错误代码
    return actionResult;
}
//准备读日志内容
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_PrepareReadFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t nameLength = can1CommandPtr->can1RecvDataBuffer[0];
    uint8_t *fileNamePtr = NULL;
    uint64_t readByteOffset = 0;
    do
    {
        fileNamePtr = UserMemMalloc(MEM_AXI_SRAM,nameLength);
        if(fileNamePtr == NULL)
        {
            CoreDelayMinTick();
        }
    } while (fileNamePtr == NULL);
    //复制文件名
    UserMemCopy(fileNamePtr,&(can1CommandPtr->can1RecvDataBuffer[1]),nameLength);
    //读取偏移字节
    readByteOffset = Can1ProcUtilConvertArrayToUint64(can1CommandPtr->can1RecvDataBuffer,nameLength+1);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)detailDebugMsgPtr,nameLength,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplFatFsLog_PrepareReadFile(fileNamePtr,nameLength,readByteOffset,&commErrorCode,&commErrorLevel);
    //返回结果数据

    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //释放内存
    if(fileNamePtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,fileNamePtr);
    }
    //返回错误代码
    return actionResult;
}
//读日志
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_ReadFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint16_t readLength = 0;
    uint8_t *fileDataBuffer = NULL;
    do
    {
        fileDataBuffer = UserMemMalloc(MEM_AXI_SRAM,FATFS_READ_WRITE_BUFFER_MAX_LENGTH+2);
        if(fileDataBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(fileDataBuffer == NULL);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplFatFsLog_ReadFile(&fileDataBuffer[2],&readLength,&commErrorCode,&commErrorLevel);
    //返回结果数据
    if(actionResult == LH_ERR_NONE)
    {
        *(uint16_t*)fileDataBuffer = readLength;
        Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,readLength+2,fileDataBuffer);
    }
    else
    {
        Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    }
    //释放内存
    if(fileDataBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,fileDataBuffer);
    }
    //返回错误代码
    return actionResult;
}

//结束读日志内容
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_EndReadFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplFatFsLog_EndReadFile(&commErrorCode,&commErrorLevel);
    //返回结果数据

    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//删除指定日志
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_DeleteTargetFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t nameLength = 0;
    uint8_t *fileNamePtr = NULL;
    nameLength = can1CommandPtr->can1RecvDataBuffer[0];
    do
    {
        fileNamePtr = UserMemMalloc(MEM_AXI_SRAM,nameLength);
        if(fileNamePtr == NULL)
        {
            CoreDelayMinTick();
        }
    }while(fileNamePtr == NULL);
    //复制文件名
    UserMemCopy(fileNamePtr,&(can1CommandPtr->can1RecvDataBuffer[1]),nameLength);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)detailDebugMsgPtr,nameLength,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplFatFsLog_DeleteTargetFile(fileNamePtr,nameLength,&commErrorCode,&commErrorLevel);
    //返回结果数据

    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //释放内存
    if(fileNamePtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,fileNamePtr);
    }
    //返回错误代码
    return actionResult;
}
//删除全部日志
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_DeleteAllFile(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplFatFsLog_DeleteAllFile(&commErrorCode,&commErrorLevel);
    //返回结果数据

    Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    //返回错误代码
    return actionResult;
}

//获取日志大小
LH_ERR TaskFuncServiceFatFsLog_Can1Msg_Proc_GetTargetFileSize(SYSTEM_CMD_CAN1* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机错误等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
    uint8_t *fileName = NULL;
    uint8_t *fileReturnBuffer;
    uint8_t nameLength = 0;
    uint64_t fileSize = 0;
    nameLength = can1CommandPtr->can1RecvDataBuffer[0];
    //申请内存
    do
    {
        fileReturnBuffer = UserMemMalloc(MEM_AXI_SRAM,nameLength+sizeof(nameLength)+sizeof(fileSize));
        if(fileReturnBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(fileReturnBuffer == NULL);
    fileName = &fileReturnBuffer[1];
    //拷贝文件名
    UserMemCopy(fileName,&(can1CommandPtr->can1RecvDataBuffer[1]),nameLength);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)detailDebugMsgPtr,nameLength,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplFatFsLog_GetTargetFileSize(fileName,nameLength,&fileSize,&commErrorCode,&commErrorLevel);
    //返回结果数据
    if(actionResult == LH_ERR_NONE)
    {
        fileReturnBuffer[0] = nameLength;
        UserMemCopy(&fileReturnBuffer[1],fileName,nameLength);
        Can1ProcUtilConvertUint64ToArray(fileReturnBuffer,1+nameLength,fileSize);
        Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,nameLength+sizeof(nameLength)+sizeof(fileSize),fileReturnBuffer);
    }
    else
    {
        Can1ReportDebugResult(can1CommandPtr,commErrorCode,commErrorLevel,actionResult,0,NULL);
    }
    //释放内存
    if(fileReturnBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,fileReturnBuffer);
    }
    //返回错误代码
    return actionResult;
}


/*************************************************2. 来自自身的消息******************************************/
//文件系统日志写入
static LH_ERR TaskFuncServiceFatFsLog_SelfMsg_Proc_WriteFile(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);
//刷新日志
static LH_ERR TaskFuncServiceFatFsLog_SelfMsg_Proc_FlushCacheData(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr);

//自身指令回调函数数组
static const SERVICE_TASK_SELF_PROC_UNIT ServiceSelfMsgProcUnitArray_FatFsLog[] = {
	{TaskFuncServiceFatFsLog_SelfMsg_Proc_WriteFile                    ,   "WriteFile"                  },
    {TaskFuncServiceFatFsLog_SelfMsg_Proc_FlushCacheData               ,   "FlushCacheData"             },
};
//消息处理映射函数表的元素数量
#define COUNT_SELF_CMD_PROC_UNIT_ARRAY_FATFS_LOG      (sizeof(ServiceSelfMsgProcUnitArray_FatFsLog)/sizeof(ServiceSelfMsgProcUnitArray_FatFsLog[0]))


//自身指令回调处理函数,解析发送过来的参数并调用映射表中的函数
static void TaskFuncServiceFatFsLog_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_FATFS_LOG* cmdResultPtr = (IPC_DATA_FATFS_LOG*)(selfCmdPtr->commandDataSpecialChannel);
	
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= COUNT_SELF_CMD_PROC_UNIT_ARRAY_FATFS_LOG)
	{
		ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)"Unsupport",
				selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
				selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
				);
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
		//回调函数
		actionResult = ServiceSelfMsgProcUnitArray_FatFsLog[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                                        ServiceSelfMsgProcUnitArray_FatFsLog[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//打印结束处理信息
	ServicePrintfShowEnd((uint8_t*)DebugStringEnd_FatFsLog,actionResult);

	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//文件系统日志写入
static LH_ERR TaskFuncServiceFatFsLog_SelfMsg_Proc_WriteFile(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_FATFS_LOG* cmdResultPtr = (IPC_DATA_FATFS_LOG*)(selfCommandPtr->commandDataSpecialChannel);
    //要写入数据缓存
    uint8_t *writeAddPtr = (uint8_t *)(selfCommandPtr->commandParam1);
    //要写入数据长度
    uint32_t writeLength = (uint32_t )(selfCommandPtr->commandParam2);
    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)detailDebugMsgPtr,writeAddPtr,writeLength,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplFatFsLog_WriteFile(writeAddPtr,writeLength,&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

//刷新日志
static LH_ERR TaskFuncServiceFatFsLog_SelfMsg_Proc_FlushCacheData(SYSTEM_CMD_SELF* selfCommandPtr,uint8_t* detailDebugMsgPtr)
{
    //结果代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机报错代码
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    //中位机报错等级
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解析和处理参数
	IPC_DATA_FATFS_LOG* cmdResultPtr = (IPC_DATA_FATFS_LOG*)(selfCommandPtr->commandDataSpecialChannel);
    //要写入数据缓存

    //打印指令开始调试讯息,自行解析参数并打印
    ServicePrintfShowStart((uint8_t*)DebugStringStart_FatFsLog,(uint8_t*)detailDebugMsgPtr,0,0,0,0,0);
    //程序进行处理,调用真实实现
    actionResult = ServiceImplFatFsLog_FlushCacheData(&commErrorCode,&commErrorLevel);
    //返回结果数据
    cmdResultPtr->can1ReportCode = commErrorCode;
    cmdResultPtr->can1ReportLevel = commErrorLevel;
    //返回错误代码
    return actionResult;
}

