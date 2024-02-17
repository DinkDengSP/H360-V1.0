/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:51:25
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-15 10:32:07
**FilePath: \App\TaskMain\AppSupport\AppTask\AppTaskStateParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"


//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskAppStateParam[STK_SIZE_TASK_APP_STATE_PARAM]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskAppStateParam@".dtcm_ram";

//自动运行程序的执行周期,单位毫秒
#define STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS            1000
//系统循环自动程序初始化
static void AppTaskFuncStateParam_Auto_Init(void);
#if(STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void AppTaskFuncStateParam_Auto_Proc(void);
#endif


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void AppTaskFuncStateParam_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr);
//2. 来自自身的消息
static void AppTaskFuncStateParam_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncAppStateParam(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    AppTaskFuncStateParam_Auto_Init();
    //初始化升级记录
    AppImplStateParamBoardIapInit();
    #if(STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= (STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS))
		{
			//上次发送时间和本次发送时间差了STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncStateParam_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                AppTaskFuncStateParam_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                AppTaskFuncStateParam_SelfMsg_Proc(selfCmdDataPtr);
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
static void AppTaskFuncStateParam_Auto_Init(void)
{
    AppImplStateParam_EventInit();
}
#if(STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void AppTaskFuncStateParam_Auto_Proc(void)
{
    
    AppImplStateParam_EventRun(STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS);
      
}
#endif



/*************************************************1. 来自CAN1的消息******************************************/
//设置模块数量
void AppTaskFuncStateParam_Can1Msg_Proc_SetModuleCount(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//软件急停
void AppTaskFuncStateParam_Can1Msg_Proc_EmergencyStop(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//在线升级
void AppTaskFuncStateParam_Can1Msg_Proc_UpdatePrepare(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取应用版本号
void AppTaskFuncStateParam_Can1Msg_Proc_ReadAppVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取Boot版本号
void AppTaskFuncStateParam_Can1Msg_Proc_ReadBootVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//升级起始
void ServiceTaskFuncDebug_Can1Msg_Proc_UpdataStart(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//升级数据
void ServiceTaskFuncDebug_Can1Msg_Proc_UpdataData(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//升级数据发送完成
void ServiceTaskFuncDebug_Can1Msg_Proc_UpdataEnd(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//程序跳转
void ServiceTaskFuncDebug_Can1Msg_Proc_ProgramJump(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT StateParamCan1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_TEST_FLOW_RAIL_MODULE_SET_COUNT       ,AppTaskFuncStateParam_Can1Msg_Proc_SetModuleCount   ,"SetModuleCount"  },
    {CAN1_CMD_TEST_FLOW_SOFT_EMERGENCY_STOP         ,AppTaskFuncStateParam_Can1Msg_Proc_EmergencyStop    ,"EmergencyStop"   },
    {CAN1_CMD_TEST_FLOW_CELL_UPDATA_PREPARE         ,AppTaskFuncStateParam_Can1Msg_Proc_UpdatePrepare    ,"UpdatePrepare"  },
    {CAN1_CMD_TEST_FLOW_CELL_VERSION                ,AppTaskFuncStateParam_Can1Msg_Proc_ReadAppVersion   ,"ReadAppVersion"  },
    {CAN1_CMD_TEST_FLOW_CELL_CELL_BOOT_VERSION      ,AppTaskFuncStateParam_Can1Msg_Proc_ReadBootVersion  ,"ReadBootVersion" },
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_START       ,ServiceTaskFuncDebug_Can1Msg_Proc_UpdataStart       ,"UpdataStart"     },
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_DATA        ,ServiceTaskFuncDebug_Can1Msg_Proc_UpdataData        ,"UpdataData"      },
    {CAN1_CMD_TEST_FLOW_CELL_UPDATA_END             ,ServiceTaskFuncDebug_Can1Msg_Proc_UpdataEnd         ,"UpdataEnd"       },
    {CAN1_CMD_TEST_FLOW_CELL_PROGRAM_JUMP           ,ServiceTaskFuncDebug_Can1Msg_Proc_ProgramJump       ,"ProgramJump"     },
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_STATE_PARAM_CMD_COUNT        (sizeof(StateParamCan1CommandProcFuncPtrArray)/sizeof(StateParamCan1CommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void AppTaskFuncStateParam_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_STATE_PARAM_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == StateParamCan1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(StateParamCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                StateParamCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,StateParamCan1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//设置模块数量
void AppTaskFuncStateParam_Can1Msg_Proc_SetModuleCount(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    uint8_t moduleCount = can1CommandPtr->recvDataBuffer[0];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,moduleCount,0,0,0,0);
    //调用实现函数
    actionResult = AppImplStateParamSetModuleCount(moduleCount,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)StateParamDebugEndMsg,actionResult);
    //该报错的数据报错
    if(can1Error != CAN1_REPORT_OK)
    {
        Can1ReportErrorCode(reportLevelResult,can1Error,actionResult);
    }
}

//软件急停
void AppTaskFuncStateParam_Can1Msg_Proc_EmergencyStop(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = AppImplStateParamEmergencyStop(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)StateParamDebugEndMsg,actionResult);
    //该返回的数据返回
    asm("nop");
    //该报错的数据报错
    if(can1Error != CAN1_REPORT_OK)
    {
        Can1ReportErrorCode(reportLevelResult,can1Error,actionResult);
    }
}

//读取应用版本号
void AppTaskFuncStateParam_Can1Msg_Proc_ReadAppVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //版本号讯息
    BOARD_APP_VERSION  appVersion;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //读取本机APP版本
    BoardVersion_ReadAppVersion(&appVersion);
    //回传主控应用版本
    Can1RailSendAppVersionWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,appVersion.mainVersion,appVersion.subVersion,appVersion.debugVersion,appVersion.svnVersion);
    //读取下位机当前版本
    errorCode = Can2SubActionBoardReadVersion(CAN2_SUB_BOARD8,READ_SOFTWARE_VERSION_APP,&appVersion.mainVersion,&appVersion.subVersion,&appVersion.debugVersion,&appVersion.svnVersion);
    if(errorCode == LH_ERR_NONE)
    {
        //回传下位机应用版本
        Can1RailSendAppVersionWithCommand(can1CommandPtr,CAN1_RAIL_ACT_BOARD_ID,appVersion.mainVersion,appVersion.subVersion,appVersion.debugVersion,appVersion.svnVersion);
    }
}

//读取Boot版本号
void AppTaskFuncStateParam_Can1Msg_Proc_ReadBootVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //版本号讯息
    BOARD_BOOT_VERSION  bootVersion;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //读取本机boot版本
    BoardVersion_ReadBootVersion(&bootVersion);
    //回传主控Boot版本
    Can1RailSendBootVersionWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,bootVersion.mainVersion,bootVersion.subVersion,bootVersion.debugVersion,bootVersion.svnVersion);
    //读取下位机当前版本
    errorCode = Can2SubActionBoardReadVersion(CAN2_SUB_BOARD8,READ_SOFTWARE_VERSION_BOOT,&bootVersion.mainVersion,&bootVersion.subVersion,&bootVersion.debugVersion,&bootVersion.svnVersion);
    if(errorCode == LH_ERR_NONE)
    {
        //回传下位机应用版本
        Can1RailSendBootVersionWithCommand(can1CommandPtr,CAN1_RAIL_ACT_BOARD_ID,bootVersion.mainVersion,bootVersion.subVersion,bootVersion.debugVersion,bootVersion.svnVersion);
    }
}


//在线升级
void AppTaskFuncStateParam_Can1Msg_Proc_UpdatePrepare(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    BOARD_IAP_STATE iapResult;
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,boardID,0,0,0,0);
    //如果模块号,板号匹配
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //打印开始升级指令
        MCU_Uart3Printf("RailMainApp Updare PrePare\r\n");
        //延时等待串口数据打印
        CoreDelayMs(10);
        //轨道主控自身升级,写入升级标志
        BoardIAP_WriteFlag();
        //跳转到BootLoader
        BoardIAP_Jump2BootLoader();
    }
    else if(boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        //用于下位机升级,目前还没写
        iapResult = AppImplStateParamIapPrepareUpdata(boardID);
        if(iapResult == BOARD_IAP_ERR)
        {
            //切入BOOT失败
            Can1RailSendSoftwareUpdatePrepareWithSelf(CAN1_RAIL_ACT_BOARD_ID,0);
            SystemDebugShowMsgError(iapResult);
        }
        else
        {
            //切入Boot成功
            Can1RailSendSoftwareUpdatePrepareWithSelf(CAN1_RAIL_ACT_BOARD_ID,1);
        }
    }
    else
    {
        //不支持的板号
        Can1RailSendSoftwareUpdatePrepareWithSelf(CAN1_SELF_BOARD_ID,0);
    }
}

//升级起始
void ServiceTaskFuncDebug_Can1Msg_Proc_UpdataStart(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    BOARD_IAP_STATE iapResult;
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    //程序字节总长度
    uint32_t appDataLengthBytes = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->recvDataBuffer,1);
    //程序数据总包数
    uint16_t appDataPackCountSet = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,5);
    //程序数据每一包的大小
    uint16_t appDataSinglePackLengthByte = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,7);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,boardID,appDataLengthBytes,appDataPackCountSet,appDataSinglePackLengthByte,0);
    //如果模块号,板号匹配
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //APP里面不支持对主控进行直接升级
        Can1RailSendIapStartResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
    }
    else if(boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        //用于下位机升级,目前还没写
        iapResult = AppImplStateParamBoardIapWriteStartMsg(boardID,appDataLengthBytes,appDataPackCountSet,appDataSinglePackLengthByte);
        if(iapResult == BOARD_IAP_ERR)
        {
            //记录升级启动讯息失败
            Can1RailSendIapStartResultWithCommand(can1CommandPtr,CAN1_RAIL_ACT_BOARD_ID,0);
            SystemDebugShowMsgError(iapResult);
        }
        else
        {
            //记录升级数据启动讯息成功
            Can1RailSendIapStartResultWithCommand(can1CommandPtr,CAN1_RAIL_ACT_BOARD_ID,1);
        }
    }
    else
    {
        //不支持的板号
        Can1RailSendIapStartResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
    }
}

//升级数据
void ServiceTaskFuncDebug_Can1Msg_Proc_UpdataData(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    BOARD_IAP_STATE iapResult;
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    //程序数据包序号
    uint16_t appDataPackIndex = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,1);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,boardID,appDataPackIndex,can1CommandPtr->recvDataLength,0,0);
    //如果模块号,板号匹配
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //APP里面不支持对主控进行直接升级
        Can1RailSendIapDataTransResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
    }
    else if(boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        iapResult = AppImplStateParamBoardIapWriteData(boardID,appDataPackIndex,can1CommandPtr->recvDataLength -3,can1CommandPtr->recvDataBuffer+3);
        //数据处理完成,回传数据处理完成
        if(iapResult == BOARD_IAP_ERR)
        {
            //传输升级数据失败
            Can1RailSendIapDataTransResultWithCommand(can1CommandPtr,CAN1_RAIL_ACT_BOARD_ID,0);
            SystemDebugShowMsgError(iapResult);
        }
        else
        {
            //传输升级数据成功
            Can1RailSendIapDataTransResultWithCommand(can1CommandPtr,CAN1_RAIL_ACT_BOARD_ID,1);
        }
        
    }
    else
    {
        //不支持的板号
        Can1RailSendIapDataTransResultWithCommand(can1CommandPtr,boardID,0);
    }
}

//升级数据发送完成
void ServiceTaskFuncDebug_Can1Msg_Proc_UpdataEnd(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    BOARD_IAP_STATE iapResult;
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    //CRC计算结果
    uint32_t crcResult = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->recvDataBuffer,1);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,boardID,crcResult,0,0,0);
    //如果模块号,板号匹配
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //APP里面不支持对主控进行直接升级
        Can1RailSendIapEndResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
    }
    else if(boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        //计算校验和
        iapResult = AppImplStateParamBoardIapCalcCRC32(boardID,crcResult);
        if(iapResult == BOARD_IAP_ERR)
        {
            //数据处理完成,回传完成
            Can1RailSendIapEndResultWithCommand(can1CommandPtr,CAN1_RAIL_ACT_BOARD_ID,0);
            SystemDebugShowMsgError(iapResult);
            return;
        }
        //开始升级
        iapResult = AppImplStateParamIapStartUpdata(boardID);
        if(iapResult == BOARD_IAP_ERR)
        {
            //数据处理完成,回传完成
            Can1RailSendIapEndResultWithCommand(can1CommandPtr,CAN1_RAIL_ACT_BOARD_ID,0);
            SystemDebugShowMsgError(iapResult);
            return;
        }
        //数据处理完成,回传完成
        Can1RailSendIapEndResultWithCommand(can1CommandPtr,CAN1_RAIL_ACT_BOARD_ID,1);
    }
    else
    {
        //不支持的板号
        Can1RailSendIapEndResultWithCommand(can1CommandPtr,boardID,0);
    }
}


//程序跳转
void ServiceTaskFuncDebug_Can1Msg_Proc_ProgramJump(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    BOARD_IAP_STATE iapResult;
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    uint8_t bootOrApp = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,boardID,0,0,0,0);
    //如果模块号,板号匹配
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //写入跳转标记
        BoardIAP_WriteJumpFlag();
        //等待稳定
        CoreDelayMs(200);
        //跳转到Boot中
        BoardIAP_Jump2BootLoader();
    }
    else if(boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        iapResult = AppImplStateParamIapProgramJump(boardID,&bootOrApp);
        if(iapResult == BOARD_IAP_IDLE)
        {
            Can1RailSendIapProgramJumpResultWithCommand(can1CommandPtr,boardID,1,bootOrApp);
        }
        else
        {
            Can1RailSendIapProgramJumpResultWithCommand(can1CommandPtr,boardID,0,bootOrApp);
        }    
    }
    else
    {
        //不支持的板号
        Can1RailSendIapProgramJumpResultWithCommand(can1CommandPtr,boardID,0,1);
    }
}


/*************************************************2. 来自自身的消息******************************************/
//设置模块数量
static LH_ERR StateParamSelfCommandProcFunc_SetModuleCount(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//软件急停
static LH_ERR StateParamSelfCommandProcFunc_EmergencyStop(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);

//回调函数数组
const SYSTEM_TASK_SELF_MSG_PROC_UNIT StateParamSelfCommandProcFuncPtrArray[] = {
    {StateParamSelfCommandProcFunc_SetModuleCount                   , "SetModuleCount"  },
    {StateParamSelfCommandProcFunc_EmergencyStop                    , "EmergencyStop"   },
};
//自身支持指令数量
#define TASK_SELF_MSG_STATE_PARAM_CMD_COUNT  ((sizeof(StateParamSelfCommandProcFuncPtrArray))/(sizeof(StateParamSelfCommandProcFuncPtrArray[0])))

//匹配发送过来的指令序号
static void AppTaskFuncStateParam_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_STATE_PARAM* selfCmdResultDataPtr = (IPC_DATA_STATE_PARAM*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (selfCmdResultDataPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	selfCmdResultDataPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_STATE_PARAM_CMD_COUNT)
	{
        SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,(uint8_t*)"unsupport",
                                selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                                selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                                );
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = StateParamSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,&(selfCmdResultDataPtr->can1ErrorCode),&(selfCmdResultDataPtr->can1ReportErrorLevel),
                                                                                            StateParamSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    SystemDebugShowMsgEnd((uint8_t*)StateParamDebugEndMsg,actionResult);
	//处理完成,设置状态
	selfCmdResultDataPtr->state = STATE_IPC_CMD_COMPLETE;
	selfCmdResultDataPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(selfCmdResultDataPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//设置模块数量
static LH_ERR StateParamSelfCommandProcFunc_SetModuleCount(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    uint8_t moduleCount = (uint8_t)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,moduleCount,0,0,0,0);
    //调用函数实现
    actionResult = AppImplStateParamSetModuleCount(moduleCount,can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

//软件急停
static LH_ERR StateParamSelfCommandProcFunc_EmergencyStop(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)StateParamDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = AppImplStateParamEmergencyStop(can1ErrorPtr,reportLevelResultPtr);;
    return actionResult;
}

