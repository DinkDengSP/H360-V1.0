/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:07:00
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-12 13:19:15
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
CPU_STK stackBufferAppTaskRackBarScan[STK_SIZE_APP_TASK_RACK_BAR_SCAN]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskRackBarScan@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_RACK_BAR_SCAN_PERIOD        100
//模块定时执行任务初始化
static void AppTaskFuncRackBarScan_Auto_Init(void);
//模块定时执行任务
static void AppTaskFuncRackBarScan_Auto_Proc(void);

//模块CAN信息处理任务
static void AppTaskFuncRackBarScan_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);


//任务函数
void AppTaskFuncRackBarScan(void *p_arg)
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
    AppTaskFuncRackBarScan_Auto_Init();
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_RACK_BAR_SCAN_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_RACK_BAR_SCAN_PERIOD)
		{
			//上次发送时间和本次发送时间差了TIME_MS_AUTO_TASK_RACK_BAR_SCAN_PERIOD个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncRackBarScan_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_RACK_BAR_SCAN_PERIOD,
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
                AppTaskFuncRackBarScan_CanMsg_Proc(canRecvDatPtr);
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
static void AppTaskFuncRackBarScan_Auto_Init(void)
{
    //条码枪初始化
    BoardBarScanCR100M_Init();
    //状态机初始化
    AppImplRackBarScanFSM_Init();
}
//模块定时执行任务
static void AppTaskFuncRackBarScan_Auto_Proc(void)
{
    //状态机定时循环运转
    AppImplRackBarScanFSM_Run();
}


/***********************************************模块CAN消息处理***********************************************************/
//指定步进电机复位
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机走位
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//模块复位
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_ModuleReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点卡住试管架
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_SwitchStateBlock(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点释放试管架
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_SwitchStateRelease(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//节点放行
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_ReleaseRack(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//单次打开条码枪
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_OpenOnce(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//持续打开条码枪
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_OpenAlways(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//关闭条码枪
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_Close(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//集成扫码
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_Integrate(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//获取节点全部讯息
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清除节点讯息
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_ClearState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);


//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcRackBarScanSetting[] = {
    {AppTaskFuncRackBarScan_CanMsg_Proc_StepMotorReset    ,1  ,""},//指定步进电机复位
    {AppTaskFuncRackBarScan_CanMsg_Proc_StepMotorRunSteps ,5  ,""},//指定步进电机走步数
    {AppTaskFuncRackBarScan_CanMsg_Proc_ModuleReset       ,0  ,""},//模块复位
    {AppTaskFuncRackBarScan_CanMsg_Proc_SwitchStateBlock  ,0  ,""},//节点卡住试管架
    {AppTaskFuncRackBarScan_CanMsg_Proc_SwitchStateRelease,0  ,""},//节点释放试管架
    {AppTaskFuncRackBarScan_CanMsg_Proc_ReleaseRack       ,0  ,""},//节点放行
    {AppTaskFuncRackBarScan_CanMsg_Proc_OpenOnce          ,1  ,""},//单次打开条码枪
    {AppTaskFuncRackBarScan_CanMsg_Proc_OpenAlways        ,0  ,""},//持续打开条码枪
    {AppTaskFuncRackBarScan_CanMsg_Proc_Close             ,0  ,""},//关闭条码枪
    {AppTaskFuncRackBarScan_CanMsg_Proc_Integrate         ,0  ,""},//集成扫码
    {AppTaskFuncRackBarScan_CanMsg_Proc_ReadState         ,0  ,""},//获取节点全部讯息
    {AppTaskFuncRackBarScan_CanMsg_Proc_ClearState        ,0  ,""},//清除节点讯息
};
#define TASK_CAN_MSG_RACK_BAR_SCAN_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcRackBarScanSetting))/(sizeof(AppTaskCanMsgProcRackBarScanSetting[0])))

//模块CAN信息处理任务
static void AppTaskFuncRackBarScan_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_RACK_BAR_SCAN_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcRackBarScanSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcRackBarScanSetting[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcRackBarScanSetting[commandValue].debugDetailMsg);

    //如果存在错误则打印错误结果
    if(errorCode != LH_ERR_NONE)
    {
        LowLevelPrintf("Func: %s,Line: %d,ErrorCode : 0X%08X\r\n", __FUNCTION__,__LINE__,errorCode);
    }
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//指定步进电机复位
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    RACK_BAR_SCAN_SM smIndex = (RACK_BAR_SCAN_SM)(cachePtrLocal->recvCacheArray[4]);
    int32_t smCurrentPos = 0;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg StepMotorReset RackBarScan\r\n");
    //调用复位函数
    errorCode = AppImplRackBarScanStepMotorReset(smIndex,&smCurrentPos);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)smIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,smCurrentPos);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//指定步进电机走位
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    RACK_BAR_SCAN_SM smIndex = (RACK_BAR_SCAN_SM)(cachePtrLocal->recvCacheArray[4]);
    int32_t runSteps = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,5);
    int32_t smCurrentPos = 0;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg StepMotorRunSteps RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanStepMotorRunSteps(smIndex,runSteps,&smCurrentPos);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)smIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,smCurrentPos);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,5);
    }
    //返回结果
    return errorCode;
}

//模块复位
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_ModuleReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_BAR_SCAN* rackBarScanStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ModuleReset RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanModuleReset(&rackBarScanStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackBarScanStatePtr->rackBarScanSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackBarScanStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackBarScanStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//节点卡住试管架
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_SwitchStateBlock(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_BAR_SCAN* rackBarScanStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg SwitchStateBlock RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanSwitchStateBlock(&rackBarScanStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackBarScanStatePtr->rackBarScanSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackBarScanStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackBarScanStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//节点释放试管架
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_SwitchStateRelease(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_BAR_SCAN* rackBarScanStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg SwitchStateRelease RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanSwitchStateRelease(&rackBarScanStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackBarScanStatePtr->rackBarScanSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackBarScanStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackBarScanStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//节点放行
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_ReleaseRack(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_BAR_SCAN* rackBarScanStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ReleaseRack RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanReleaseRack(&rackBarScanStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackBarScanStatePtr->rackBarScanSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackBarScanStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackBarScanStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//单次打开条码枪
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_OpenOnce(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_BAR_SCAN* rackBarScanStatePtr = NULL;
    uint8_t openTimeSecond = (cachePtrLocal->recvCacheArray[4]);
    uint16_t barCodeLength = 0;
    uint8_t* barCodeBuffer = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg OpenOnce RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanOpenOnce(openTimeSecond,&barCodeLength,&barCodeBuffer,&rackBarScanStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t* dataBufferSend = UserMemMallocWhileSuccess(SRAM_IN,barCodeLength+5);
        dataBufferSend[0] = (uint8_t)(rackBarScanStatePtr->rackBarScanSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackBarScanStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackBarScanStatePtr->nodeState);
        CanBaseWriteInt16DataToBuffer(dataBufferSend,3,barCodeLength);
        if(barCodeBuffer != NULL)
        {
            UserMemCopy(dataBufferSend+5,barCodeBuffer,barCodeLength);
        }
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,barCodeLength+5);
        //释放内存
        UserMemFree(SRAM_IN,dataBufferSend);
    }
    //释放内存
    if(barCodeBuffer != NULL)
    {
        UserMemFree(SRAM_IN,barCodeBuffer);
    }
    //返回结果
    return errorCode;
}

//持续打开条码枪
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_OpenAlways(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_BAR_SCAN* rackBarScanStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg OpenAlways RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanOpenAlways(&rackBarScanStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackBarScanStatePtr->rackBarScanSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackBarScanStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackBarScanStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//关闭条码枪
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_Close(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_BAR_SCAN* rackBarScanStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg Close RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanClose(&rackBarScanStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackBarScanStatePtr->rackBarScanSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackBarScanStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackBarScanStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

#define RACK_BAR_SCAN_INTEGRATE_RETURN_DATA_LENGTH      (3+2+(32*5))
//集成扫码
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_Integrate(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_BAR_SCAN* rackBarScanStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg Integrate RackBarScan\r\n");
    //调用实现函数
    RACK_BAR_SCAN_RESULT* barscanResultPtr = NULL;
    barscanResultPtr = UserMemMallocWhileSuccess(SRAM_IN,(sizeof(RACK_BAR_SCAN_RESULT)/sizeof(uint8_t)));
    UserMemSet(barscanResultPtr,0,(sizeof(RACK_BAR_SCAN_RESULT)/sizeof(uint8_t)));
    errorCode = AppImplRackBarScanIntegrate(barscanResultPtr,&rackBarScanStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        //申请内存
        uint8_t* dataBufferSend = UserMemMallocWhileSuccess(SRAM_IN,RACK_BAR_SCAN_INTEGRATE_RETURN_DATA_LENGTH);
        dataBufferSend[0] = (uint8_t)(rackBarScanStatePtr->rackBarScanSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackBarScanStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackBarScanStatePtr->nodeState);
        CanBaseWriteInt16DataToBuffer(dataBufferSend,3,barscanResultPtr->rackID);
        //条码拷贝
        UserMemCopy(dataBufferSend+5,barscanResultPtr->tube1BarCode,32);
        UserMemCopy(dataBufferSend+37,barscanResultPtr->tube2BarCode,32);
        UserMemCopy(dataBufferSend+69,barscanResultPtr->tube3BarCode,32);
        UserMemCopy(dataBufferSend+101,barscanResultPtr->tube4BarCode,32);
        UserMemCopy(dataBufferSend+133,barscanResultPtr->tube5BarCode,32);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,RACK_BAR_SCAN_INTEGRATE_RETURN_DATA_LENGTH);
        //释放内存
        UserMemFree(SRAM_IN,dataBufferSend);
    }
    //释放内存
    UserMemFree(SRAM_IN,barscanResultPtr);
    //返回结果
    return errorCode;
}

//获取节点全部讯息
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    APP_STATE_RACK_BAR_SCAN* rackBarScanStatePtr = NULL;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ReadState RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanReadState(&rackBarScanStatePtr);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[3] = {0};
        dataBufferSend[0] = (uint8_t)(rackBarScanStatePtr->rackBarScanSensorSet.rackExistSensor);
        dataBufferSend[1] = (uint8_t)(rackBarScanStatePtr->rackComeInEventFlag);
        dataBufferSend[2] = (uint8_t)(rackBarScanStatePtr->nodeState);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RACK_BAR_SCAN,dataBufferSend,3);
    }
    //返回结果
    return errorCode;
}

//清除节点讯息
static LH_ERR AppTaskFuncRackBarScan_CanMsg_Proc_ClearState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ClearState RackBarScan\r\n");
    //调用实现函数
    errorCode = AppImplRackBarScanClearState();
    //返回结果
    return errorCode;
}

