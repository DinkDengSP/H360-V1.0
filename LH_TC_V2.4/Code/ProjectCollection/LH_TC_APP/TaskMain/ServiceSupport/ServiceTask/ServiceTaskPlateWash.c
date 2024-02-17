/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-16 20:01:37
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-03 15:49:48
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceTaskPlateWash.c
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
CPU_STK stackBufferServiceTaskPlateWash[STK_SIZE_SERVICE_TASK_PLATE_WASH]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskPlateWash@".ccram";


//自动运行程序的执行周期,单位毫秒
#define SERVICE_TASK_PLATE_WASH_AUTO_FUNC_RUNNING_PERIOD_MS            500
//系统循环自动程序初始化
static void TaskServicePlateWash_Auto_Init(void);
//系统还处理一些循环自动化程序
static void TaskServicePlateWash_Auto_Proc(void);


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void TaskFuncServicePlateWash_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgPtr);
//2. 来自自身的消息
static void TaskFuncServicePlateWash_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void ServiceTaskFuncPlateWash(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //延时等待传感器第一段信息更新
    CoreDelayMs(2000);
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    //状态机初始化
    TaskServicePlateWash_Auto_Init();
    while(1)
    {
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= SERVICE_TASK_PLATE_WASH_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了SERVICE_TASK_PLATE_WASH_AUTO_FUNC_RUNNING_PERIOD_MS个时间
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			TaskServicePlateWash_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)SERVICE_TASK_PLATE_WASH_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                TaskFuncServicePlateWash_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                TaskFuncServicePlateWash_SelfMsg_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
                taskCommandPtr->cmdData = NULL;
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}

/************************************************自动运行程序************************************************/
//系统循环自动程序初始化
static void TaskServicePlateWash_Auto_Init(void)
{
    ServiceImplPlateWash_StateMachineInit();
}

//系统循环自动程序
static void TaskServicePlateWash_Auto_Proc(void)
{
    ServiceImplPlateWash_StateMachineLoop();
}

/************************************************CAN总线接收到的消息的处理******************************************************/
//读取当前清洗盘温度状态
static LH_ERR ServiceTaskFuncPlateWash_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT serviceTaskCanMsgProcPlateWashSetting[] = {
    {ServiceTaskFuncPlateWash_CanMsg_Proc_ReadState                          ,           0           ,"ReadState"},
};
#define TASK_CAN_MSG_PLATE_WASH_COMMAND_COUNT  ((sizeof(serviceTaskCanMsgProcPlateWashSetting))/(sizeof(serviceTaskCanMsgProcPlateWashSetting[0])))


//分发CAN指令,对接信息流
static void TaskFuncServicePlateWash_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_PLATE_WASH_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +serviceTaskCanMsgProcPlateWashSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = serviceTaskCanMsgProcPlateWashSetting[commandValue].funcPtr(canMsgCachePtr,
                    serviceTaskCanMsgProcPlateWashSetting[commandValue].debugDetailMsg);

    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//读取当前清洗盘温度状态
static LH_ERR ServiceTaskFuncPlateWash_CanMsg_Proc_ReadState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //控温状态
    LH_ERR errorState = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //传感器状态
    float sensorValue = 0.0;
    errorState = ServiceImplPlateWash_ReadState(&sensorValue);
    if(errorState != LH_ERR_NONE)
    {
        return errorState;
    }
    //返回数据,申请内存
    uint8_t* dataBufferSend = NULL;
    do
    {
        dataBufferSend = UserMemMalloc(SRAM_CCM,4);
        if(dataBufferSend == NULL)
        {
            CoreDelayMinTick();
        }
    } while (dataBufferSend == NULL);
    //温度值
    CanBaseWriteFloatDataToBuffer(dataBufferSend,0,sensorValue);
    //返回数据包
    CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_TC_CHANNEL_PLATE_WASH,dataBufferSend,4);
    //发送完成,释放内存
    UserMemFree(SRAM_CCM,dataBufferSend);
    return LH_ERR_NONE;
}

/************************************************自身分层API调用的消息的处理**********************************/


//分发队列消息,对接信息流
static void TaskFuncServicePlateWash_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    
}

















