/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:08:31
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-15 15:07:45
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
CPU_STK stackBufferAppTaskRackOutSwitch[STK_SIZE_APP_TASK_RACK_OUT_SWITCH]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskRackOutSwitch@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_RACK_OUT_SWITCH_PERIOD        100
//模块定时执行任务初始化
static void AppTaskFuncRackOutSwitch_Auto_Init(void);
//模块定时执行任务
static void AppTaskFuncRackOutSwitch_Auto_Proc(void);

//模块CAN信息处理任务
static void AppTaskFuncRackOutSwitch_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);


//任务函数
void AppTaskFuncRackOutSwitch(void *p_arg)
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
    AppTaskFuncRackOutSwitch_Auto_Init();
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_RACK_OUT_SWITCH_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_RACK_OUT_SWITCH_PERIOD)
		{
			//上次发送时间和本次发送时间差了TIME_MS_AUTO_TASK_RACK_OUT_SWITCH_PERIOD个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncRackOutSwitch_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_RACK_OUT_SWITCH_PERIOD,
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
                AppTaskFuncRackOutSwitch_CanMsg_Proc(canRecvDatPtr);
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
static void AppTaskFuncRackOutSwitch_Auto_Init(void)
{
    AppImplRackOutSwitchFSM_Init();
}
//模块定时执行任务
static void AppTaskFuncRackOutSwitch_Auto_Proc(void)
{
    AppImplRackOutSwitchFSM_Run();
}


/***********************************************模块CAN消息处理***********************************************************/
//指定步进电机复位
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机走位
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点复位
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ModuleReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点切换到试管架进入位置
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ToRackComeInPos(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点切换到试管架离开位置
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ToRackMoveOutPos(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点执行一次试管架换轨
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_TrigOnce(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//获取节点全部讯息
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清除节点全部讯息
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ClearState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcRackOutSwitchSetting[] = {
    {AppTaskFuncRackOutSwitch_CanMsg_Proc_StepMotorReset    ,1  ,""},//指定步进电机复位
    {AppTaskFuncRackOutSwitch_CanMsg_Proc_StepMotorRunSteps ,5  ,""},//指定步进电机走步数
    {AppTaskFuncRackOutSwitch_CanMsg_Proc_ModuleReset       ,0  ,""},//节点复位
    {AppTaskFuncRackOutSwitch_CanMsg_Proc_ToRackComeInPos   ,0  ,""},//节点切换到试管架进入位置
    {AppTaskFuncRackOutSwitch_CanMsg_Proc_ToRackMoveOutPos  ,0  ,""},//节点切换到试管架离开位置
    {AppTaskFuncRackOutSwitch_CanMsg_Proc_TrigOnce          ,0  ,""},//节点执行一次试管架换轨
    {AppTaskFuncRackOutSwitch_CanMsg_Proc_ReadState         ,0  ,""},//获取节点全部讯息
    {AppTaskFuncRackOutSwitch_CanMsg_Proc_ClearState        ,0  ,""},//清除节点全部讯息
};
#define TASK_CAN_MSG_RACK_OUT_SWITCH_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcRackOutSwitchSetting))/(sizeof(AppTaskCanMsgProcRackOutSwitchSetting[0])))

//模块CAN信息处理任务
static void AppTaskFuncRackOutSwitch_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_RACK_OUT_SWITCH_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcRackOutSwitchSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcRackOutSwitchSetting[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcRackOutSwitchSetting[commandValue].debugDetailMsg);

    //如果存在错误则打印错误结果
    if(errorCode != LH_ERR_NONE)
    {
        LowLevelPrintf("Func: %s,Line: %d,ErrorCode : 0X%08X\r\n", __FUNCTION__,__LINE__,errorCode);
    }
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//指定步进电机复位
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    RACK_OUT_SWITCH_SM smIndex = (RACK_OUT_SWITCH_SM)(cachePtrLocal->recvCacheArray[4]);
    int32_t smCurrentPos = 0;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg StepMotorReset RackOutSwitch\r\n");
    //调用复位函数
    errorCode = AppImplRackOutSwitchStepMotorReset(smIndex,&smCurrentPos);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)smIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,smCurrentPos);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_OUT_SWITCH,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//指定步进电机走位
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    RACK_OUT_SWITCH_SM smIndex = (RACK_OUT_SWITCH_SM)(cachePtrLocal->recvCacheArray[4]);
    int32_t runSteps = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,5);
    int32_t smCurrentPos = 0;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg StepMotorRunSteps RackOutSwitch\r\n");
    //调用实现函数
    errorCode = AppImplRackOutSwitchStepMotorRunSteps(smIndex,runSteps,&smCurrentPos);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)smIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,smCurrentPos);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_OUT_SWITCH,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}


//节点复位
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ModuleReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ModuleReset RackOutSwitch\r\n");
    //调用实现函数
    errorCode = AppImplRackOutSwitchModuleReset(&rackOutSwitchStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[4] = {0};
        dataBufferSend[0] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackComeInState);
        dataBufferSend[1] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackMoveOutState);
        dataBufferSend[2] = (uint8_t)(rackOutSwitchStatePtr->rackComeInEventFlag);
        dataBufferSend[3] = (uint8_t)(rackOutSwitchStatePtr->railSwitchState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_OUT_SWITCH,dataBufferSend,4);
    }
    //返回结果
    return errorCode;
}

//节点切换到试管架进入位置
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ToRackComeInPos(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ToRackComeInPos RackOutSwitch\r\n");
    //调用实现函数
    errorCode = AppImplRackOutSwitchToRackComeInPos(&rackOutSwitchStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[4] = {0};
        dataBufferSend[0] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackComeInState);
        dataBufferSend[1] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackMoveOutState);
        dataBufferSend[2] = (uint8_t)(rackOutSwitchStatePtr->rackComeInEventFlag);
        dataBufferSend[3] = (uint8_t)(rackOutSwitchStatePtr->railSwitchState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_OUT_SWITCH,dataBufferSend,4);
    }
    //返回结果
    return errorCode;
}

//节点切换到试管架离开位置
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ToRackMoveOutPos(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ToRackMoveOutPos RackOutSwitch\r\n");
    //调用实现函数
    errorCode = AppImplRackOutSwitchToRackMoveOutPos(&rackOutSwitchStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[4] = {0};
        dataBufferSend[0] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackComeInState);
        dataBufferSend[1] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackMoveOutState);
        dataBufferSend[2] = (uint8_t)(rackOutSwitchStatePtr->rackComeInEventFlag);
        dataBufferSend[3] = (uint8_t)(rackOutSwitchStatePtr->railSwitchState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_OUT_SWITCH,dataBufferSend,4);
    }
    //返回结果
    return errorCode;
}

//节点执行一次试管架换轨
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_TrigOnce(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg TrigOnce RackOutSwitch\r\n");
    //调用实现函数
    errorCode = AppImplRackOutSwitchTrigOnce(&rackOutSwitchStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[4] = {0};
        dataBufferSend[0] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackComeInState);
        dataBufferSend[1] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackMoveOutState);
        dataBufferSend[2] = (uint8_t)(rackOutSwitchStatePtr->rackComeInEventFlag);
        dataBufferSend[3] = (uint8_t)(rackOutSwitchStatePtr->railSwitchState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_OUT_SWITCH,dataBufferSend,4);
    }
    //返回结果
    return errorCode;
}

//获取节点全部讯息
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ReadState RackOutSwitch\r\n");
    //调用实现函数
    errorCode = AppImplRackOutSwitchReadState(&rackOutSwitchStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[4] = {0};
        dataBufferSend[0] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackComeInState);
        dataBufferSend[1] = (uint8_t)(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackMoveOutState);
        dataBufferSend[2] = (uint8_t)(rackOutSwitchStatePtr->rackComeInEventFlag);
        dataBufferSend[3] = (uint8_t)(rackOutSwitchStatePtr->railSwitchState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_OUT_SWITCH,dataBufferSend,4);
    }
    //返回结果
    return errorCode;
}

//清除节点全部讯息
static LH_ERR AppTaskFuncRackOutSwitch_CanMsg_Proc_ClearState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ClearState RackOutSwitch\r\n");
    //调用实现函数
    errorCode = AppImplRackOutSwitchClearState();
    //返回结果
    return errorCode;
}
