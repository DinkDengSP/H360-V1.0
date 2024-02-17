/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:07:08
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-07 10:23:55
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
CPU_STK stackBufferAppTaskRackTest[STK_SIZE_APP_TASK_RACK_TEST]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskRackTest@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_RACK_TEST_PERIOD        100
//模块定时执行任务初始化
static void AppTaskFuncRackTest_Auto_Init(void);
//模块定时执行任务
static void AppTaskFuncRackTest_Auto_Proc(void);

//模块CAN信息处理任务
static void AppTaskFuncRackTest_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);


//任务函数
void AppTaskFuncRackTest(void *p_arg)
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
    AppTaskFuncRackTest_Auto_Init();
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_RACK_TEST_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_RACK_TEST_PERIOD)
		{
			//上次发送时间和本次发送时间差了TIME_MS_AUTO_TASK_RACK_TEST_PERIOD个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncRackTest_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_RACK_TEST_PERIOD,
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
                AppTaskFuncRackTest_CanMsg_Proc(canRecvDatPtr);
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
static void AppTaskFuncRackTest_Auto_Init(void)
{
    AppImplRackTestFSM_Init();
}
//模块定时执行任务
static void AppTaskFuncRackTest_Auto_Proc(void)
{
    AppImplRackTestFSM_Run();
}


/***********************************************模块CAN消息处理***********************************************************/
//指定步进电机复位
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机走位
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点复位
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_ModuleReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点切换下一试管
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_SwitchNextTube(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//获取全部缓存信息
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清除缓存讯息
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_ClearState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//切换释放状态
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_SwitchRelease(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcRackTestSetting[] = {
    {AppTaskFuncRackTest_CanMsg_Proc_StepMotorReset    ,1  ,""},//指定步进电机复位
    {AppTaskFuncRackTest_CanMsg_Proc_StepMotorRunSteps ,5  ,""},//指定步进电机走步数
    {AppTaskFuncRackTest_CanMsg_Proc_ModuleReset       ,0  ,""},//节点复位
    {AppTaskFuncRackTest_CanMsg_Proc_SwitchNextTube    ,1  ,""},//节点切换下一试管
    {AppTaskFuncRackTest_CanMsg_Proc_ReadState         ,0  ,""},//获取全部缓存信息
    {AppTaskFuncRackTest_CanMsg_Proc_ClearState        ,0  ,""},//清除缓存讯息
    {AppTaskFuncRackTest_CanMsg_Proc_SwitchRelease     ,0  ,""},//切换释放状态
};
#define TASK_CAN_MSG_RACK_TEST_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcRackTestSetting))/(sizeof(AppTaskCanMsgProcRackTestSetting[0])))

//模块CAN信息处理任务
static void AppTaskFuncRackTest_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_RACK_TEST_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcRackTestSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcRackTestSetting[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcRackTestSetting[commandValue].debugDetailMsg);

    //如果存在错误则打印错误结果
    if(errorCode != LH_ERR_NONE)
    {
        LowLevelPrintf("Func: %s,Line: %d,ErrorCode : 0X%08X\r\n", __FUNCTION__,__LINE__,errorCode);
    }
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//指定步进电机复位
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    RACK_TEST_SM smIndex = (RACK_TEST_SM)(cachePtrLocal->recvCacheArray[4]);
    int32_t smCurrentPos = 0;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg StepMotorReset RackTest\r\n");
    //调用复位函数
    errorCode = AppImplRackTestStepMotorReset(smIndex,&smCurrentPos);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)smIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,smCurrentPos);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_TEST,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//指定步进电机走位
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    RACK_TEST_SM smIndex = (RACK_TEST_SM)(cachePtrLocal->recvCacheArray[4]);
    int32_t runSteps = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,5);
    int32_t smCurrentPos = 0;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg StepMotorRunSteps RackTest\r\n");
    //调用复位函数
    errorCode = AppImplRackTestStepMotorRunSteps(smIndex,runSteps,&smCurrentPos);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)smIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,smCurrentPos);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_TEST,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//节点复位
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_ModuleReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_TEST* rackTestStatePtr = NULL;
    //调用实现函数
    errorCode = AppImplRackTestModuleReset(&rackTestStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.rackFullSensor);
        dataBufferSend[2] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.encoderSensor);
        dataBufferSend[3] = (uint8_t)(rackTestStatePtr->rackComeInEventFlag);
        dataBufferSend[4] = (uint8_t)(rackTestStatePtr->currentTubeNo);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_TEST,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//节点切换下一试管
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_SwitchNextTube(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_TEST* rackTestStatePtr = NULL;
    uint8_t nextTubeNo = cachePtrLocal->recvCacheArray[4];
    //调用实现函数
    errorCode = AppImplRackTestSwitchNextTube(nextTubeNo,&rackTestStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.rackFullSensor);
        dataBufferSend[2] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.encoderSensor);
        dataBufferSend[3] = (uint8_t)(rackTestStatePtr->rackComeInEventFlag);
        dataBufferSend[4] = (uint8_t)(rackTestStatePtr->currentTubeNo);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_TEST,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//获取全部缓存信息
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_TEST* rackTestStatePtr = NULL;
    //调用实现函数
    errorCode = AppImplRackTestReadState(&rackTestStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.rackFullSensor);
        dataBufferSend[2] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.encoderSensor);
        dataBufferSend[3] = (uint8_t)(rackTestStatePtr->rackComeInEventFlag);
        dataBufferSend[4] = (uint8_t)(rackTestStatePtr->currentTubeNo);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_TEST,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//清除缓存讯息
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_ClearState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //调用实现函数
    errorCode = AppImplRackTestClearState();
    return errorCode;
}

//切换释放状态
static LH_ERR AppTaskFuncRackTest_CanMsg_Proc_SwitchRelease(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_TEST* rackTestStatePtr = NULL;
    //调用实现函数
    errorCode = AppImplRackTestSwitchSwitchRelease(&rackTestStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.rackFullSensor);
        dataBufferSend[2] = (uint8_t)(rackTestStatePtr->rackTestSensorSet.encoderSensor);
        dataBufferSend[3] = (uint8_t)(rackTestStatePtr->rackComeInEventFlag);
        dataBufferSend[4] = (uint8_t)(rackTestStatePtr->currentTubeNo);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_TEST,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}
