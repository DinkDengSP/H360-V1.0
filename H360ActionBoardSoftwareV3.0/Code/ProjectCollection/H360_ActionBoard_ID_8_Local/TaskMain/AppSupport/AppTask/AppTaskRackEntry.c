/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:06:50
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-13 10:28:13
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
CPU_STK stackBufferAppTaskRackEntry[STK_SIZE_APP_TASK_RACK_ENTRY]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskRackEntry@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_RACK_ENTRY_PERIOD        100
//模块定时执行任务初始化
static void AppTaskFuncRackEntry_Auto_Init(void);
//模块定时执行任务
static void AppTaskFuncRackEntry_Auto_Proc(void);

//模块CAN信息处理任务
static void AppTaskFuncRackEntry_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);


//任务函数
void AppTaskFuncRackEntry(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    //自动执行任务初始化
    AppTaskFuncRackEntry_Auto_Init();
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_RACK_ENTRY_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_RACK_ENTRY_PERIOD)
		{
			//上次发送时间和本次发送时间差了TIME_MS_AUTO_TASK_RACK_ENTRY_PERIOD个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncRackEntry_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_RACK_ENTRY_PERIOD,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(TASK_CMD_SRC_CAN == taskCommandPtr->cmdSrc)
            {
                //从can来的数据
                CAN_SLAVE_CHANNEL_CACHE* canRecvDatPtr = (CAN_SLAVE_CHANNEL_CACHE*)taskCommandPtr->cmdData;
                //对这个数据进行处理
                AppTaskFuncRackEntry_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}

/*********************************************自身定时任务处理*********************************************/
//模块定时执行任务初始化
static void AppTaskFuncRackEntry_Auto_Init(void)
{
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = NULL;
    if(railConveyorBeltMotorType == 0)
    {
        AppImplRackEntry0FSM_Init();
        //先停止轨道
        AppImplRackEntry0RailStop(&rackEntryStatePtr);
        //然后启动轨道
        AppImplRackEntry0RailStart(1,1,&rackEntryStatePtr);
    }
    else
    {
        AppImplRackEntry1FSM_Init();
        //先停止轨道
        AppImplRackEntry1RailStop(&rackEntryStatePtr);
        //然后启动轨道
        AppImplRackEntry1RailStart(1,1,&rackEntryStatePtr);
    }
}
//模块定时执行任务
static void AppTaskFuncRackEntry_Auto_Proc(void)
{
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        AppImplRackEntry0FSM_Run();
    }
    else
    {
        AppImplRackEntry1FSM_Run();
    }
}


/***********************************************模块CAN消息处理***********************************************************/
//指定步进电机复位
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机走位
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点复位
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_ModuleReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点卡住试管架
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_SwitchStateBlock(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点释放试管架
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_SwitchStateRelease(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点放行
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_ReleaseRack(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道启动
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_RailStart(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道停止
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_RailStop(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//获取轨道全部状态
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清除轨道记录状态
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_ClearState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcRackEntrySetting[] = {
    {AppTaskFuncRackEntry_CanMsg_Proc_StepMotorReset        ,1  ,""},//指定步进电机复位
    {AppTaskFuncRackEntry_CanMsg_Proc_StepMotorRunSteps     ,5  ,""},//指定步进电机走步数
    {AppTaskFuncRackEntry_CanMsg_Proc_ModuleReset           ,0  ,""},//节点复位
    {AppTaskFuncRackEntry_CanMsg_Proc_SwitchStateBlock      ,0  ,""},//节点卡住试管架
    {AppTaskFuncRackEntry_CanMsg_Proc_SwitchStateRelease    ,0  ,""},//节点释放试管架
    {AppTaskFuncRackEntry_CanMsg_Proc_ReleaseRack           ,0  ,""},//节点放行
    {AppTaskFuncRackEntry_CanMsg_Proc_RailStart             ,2  ,""},//轨道启动
    {AppTaskFuncRackEntry_CanMsg_Proc_RailStop              ,0  ,""},//轨道停止
    {AppTaskFuncRackEntry_CanMsg_Proc_ReadState             ,0  ,""},//获取轨道全部状态
    {AppTaskFuncRackEntry_CanMsg_Proc_ClearState            ,0  ,""},//清除轨道记录状态
};
#define TASK_CAN_MSG_RACK_ENTRY_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcRackEntrySetting))/(sizeof(AppTaskCanMsgProcRackEntrySetting[0])))

//模块CAN信息处理任务
static void AppTaskFuncRackEntry_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
{
    LH_ERR errorCode =LH_ERR_NONE;

    //首先,参数长度必须大于4
    if(canMsgCachePtr->currentLength < 4)
    {
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_LENGTH);
        //返回ACK错误
        return;
    }

    //读取指令码,识别指令码是否支持
    uint32_t commandValue = CanBaseReadDataConvertToUint32(canMsgCachePtr->recvCacheArray,0);
    if(commandValue >= TASK_CAN_MSG_RACK_ENTRY_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcRackEntrySetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcRackEntrySetting[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcRackEntrySetting[commandValue].debugDetailMsg);

    //如果存在错误则打印错误结果
    if(errorCode != LH_ERR_NONE)
    {
        LowLevelPrintf("Func: %s,Line: %d,ErrorCode : 0X%08X\r\n", __FUNCTION__,__LINE__,errorCode);
    }
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//指定步进电机复位
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    uint8_t smIndex = (uint8_t)(cachePtrLocal->recvCacheArray[4]);
    int32_t smCurrentPos = 0;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg StepMotorReset RackEntry\r\n");
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用复位函数
        errorCode = AppImplRackEntry0StepMotorReset((RACK_ENTRY0_SM)smIndex,&smCurrentPos);
    }
    else
    {
        //调用复位函数
        errorCode = AppImplRackEntry1StepMotorReset((RACK_ENTRY1_SM)smIndex,&smCurrentPos);
    }
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)smIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,smCurrentPos);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_ENTRY,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//指定步进电机走位
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    uint8_t smIndex = (uint8_t)(cachePtrLocal->recvCacheArray[4]);
    int32_t runSteps = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,5);
    int32_t smCurrentPos = 0;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg StepMotorRunSteps RackEntry\r\n");
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用复位函数
        errorCode = AppImplRackEntry0StepMotorRunSteps((RACK_ENTRY0_SM)smIndex,runSteps,&smCurrentPos);
    }
    else
    {
        //调用复位函数
        errorCode = AppImplRackEntry1StepMotorRunSteps((RACK_ENTRY1_SM)smIndex,runSteps,&smCurrentPos);
    }
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)smIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,smCurrentPos);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_ENTRY,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//节点复位
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_ModuleReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = NULL;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ModuleReset RackEntry\r\n");
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用实现函数
        errorCode = AppImplRackEntry0ModuleReset(&rackEntryStatePtr);
    }
    else
    {
        //调用实现函数
        errorCode = AppImplRackEntry1ModuleReset(&rackEntryStatePtr);
    }
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackEntryStatePtr->rackEntrySensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackEntryStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackEntryStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_ENTRY,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//节点卡住试管架
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_SwitchStateBlock(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = NULL;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg SwitchStateBlock RackEntry\r\n");
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用实现函数
        errorCode = AppImplRackEntry0SwitchStateBlock(&rackEntryStatePtr);
    }
    else
    {
        //调用实现函数
        errorCode = AppImplRackEntry1SwitchStateBlock(&rackEntryStatePtr);
    }
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackEntryStatePtr->rackEntrySensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackEntryStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackEntryStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_ENTRY,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//节点释放试管架
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_SwitchStateRelease(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = NULL;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg SwitchStateRelease RackEntry\r\n");
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用实现函数
        errorCode = AppImplRackEntry0SwitchStateRelease(&rackEntryStatePtr);
    }
    else
    {
        //调用实现函数
        errorCode = AppImplRackEntry1SwitchStateRelease(&rackEntryStatePtr);
    }
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackEntryStatePtr->rackEntrySensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackEntryStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackEntryStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_ENTRY,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//节点放行
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_ReleaseRack(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = NULL;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ReleaseRack RackEntry\r\n");
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用实现函数
        errorCode = AppImplRackEntry0ReleaseRack(&rackEntryStatePtr);
    }
    else
    {
        //调用实现函数
        errorCode = AppImplRackEntry1ReleaseRack(&rackEntryStatePtr);
    }
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackEntryStatePtr->rackEntrySensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackEntryStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackEntryStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_ENTRY,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//轨道启动
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_RailStart(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = NULL;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    uint8_t railInDir = (cachePtrLocal->recvCacheArray[4]);
    uint8_t railOutDir = (cachePtrLocal->recvCacheArray[5]);
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg RailStart RackEntry,RailInDir: %d,RailOutDir: %d\r\n",railInDir,railOutDir);
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用实现函数
        errorCode = AppImplRackEntry0RailStart(railInDir,railOutDir,&rackEntryStatePtr);
    }
    else
    {
        //调用实现函数
        errorCode = AppImplRackEntry1RailStart(railInDir,railOutDir,&rackEntryStatePtr);
    }
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackEntryStatePtr->rackEntrySensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackEntryStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackEntryStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_ENTRY,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//轨道停止
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_RailStop(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = NULL;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg RailStop RackEntry\r\n");
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用实现函数
        errorCode = AppImplRackEntry0RailStop(&rackEntryStatePtr);
    }
    else
    {
        //调用实现函数
        errorCode = AppImplRackEntry1RailStop(&rackEntryStatePtr);
    }
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackEntryStatePtr->rackEntrySensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackEntryStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackEntryStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_ENTRY,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//获取轨道全部状态
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = NULL;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ReadState RackEntry\r\n");
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用实现函数
        errorCode = AppImplRackEntry0ReadState(&rackEntryStatePtr);
    }
    else
    {
        //调用实现函数
        errorCode = AppImplRackEntry1ReadState(&rackEntryStatePtr);
    }
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackEntryStatePtr->rackEntrySensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackEntryStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackEntryStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_ENTRY,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//清除轨道记录状态
static LH_ERR AppTaskFuncRackEntry_CanMsg_Proc_ClearState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ClearState RackEntry\r\n");
    //读取轨道类型参数
    int32_t railConveyorBeltMotorType = 0;
    AppParamReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE,&railConveyorBeltMotorType);
    if(railConveyorBeltMotorType == 0)
    {
        //调用实现函数
        errorCode = AppImplRackEntry0ClearState();
    }
    else
    {
        //调用实现函数
        errorCode = AppImplRackEntry1ClearState();
    }
    //返回结果
    return errorCode;
}

