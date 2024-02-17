/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:37:20
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-07 15:20:38
**FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppTask\AppTaskHandWash.c
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
CPU_STK stackBufferAppTaskHandWash[STK_SIZE_APP_TASK_HAND_WASH]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskHandWash@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_HAND_WASH_PERIOD        0
//模块定时任务初始化
static void AppTaskFuncHandWash_Auto_Init(void);
#if(TIME_MS_AUTO_TASK_HAND_WASH_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncHandWash_Auto_Proc(void);
#endif

//模块CAN信息处理任务
static void AppTaskFuncHandWash_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncHandWash_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncHandWash(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //模块定时任务初始化
    AppTaskFuncHandWash_Auto_Init();
    #if(TIME_MS_AUTO_TASK_HAND_WASH_PERIOD != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_HAND_WASH_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_HAND_WASH_PERIOD)
		{
			//上次发送时间和本次发送时间差了1000个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncHandWash_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_HAND_WASH_PERIOD,
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
                AppTaskFuncHandWash_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncHandWash_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}



/*********************************************自身定时任务处理*********************************************/
//模块定时任务初始化
static void AppTaskFuncHandWash_Auto_Init(void)
{

}
#if(TIME_MS_AUTO_TASK_HAND_WASH_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncHandWash_Auto_Proc(void)
{
    return;
}
#endif


/************************************************CAN总线接收到的消息的处理******************************************************/
//清洗机械手复位
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手旋转到反应盘
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_RotatePlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手旋转到清洗盘
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_RotatePlateWash(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手旋转到测量模块
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_RotateMeasureModule(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手反应盘取杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_CatchCupPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手反应盘放杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_PutCupPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手清洗盘取杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_CatchCupPlateWash(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手清洗盘放杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_PutCupPlateWash(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手测量模块取杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_CatchCupOnMeasureUnitModule(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手测量模块放杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_PutCupOnMeasureUnitModule(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机复位
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_MotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机走指定步数
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_MoterRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcFuncPtrArrayHandWash[] = {
    /****前一个是处理函数***************************************参数的最小长度*****/
    {AppTaskFuncHandWash_CanMsg_Proc_Reset                          ,0           ,"Reset"                           },//清洗机械手复位
    {AppTaskFuncHandWash_CanMsg_Proc_RotatePlateReaction            ,0           ,"RotatePlateReaction"             },//清洗机械手旋转到反应盘
    {AppTaskFuncHandWash_CanMsg_Proc_RotatePlateWash                ,0           ,"RotatePlateWash"                 },//清洗机械手旋转到清洗盘
    {AppTaskFuncHandWash_CanMsg_Proc_RotateMeasureModule            ,0           ,"RotateMeasureModule"             },//清洗机械手旋转到测量模块
    {AppTaskFuncHandWash_CanMsg_Proc_CatchCupPlateReaction          ,0           ,"CatchCupPlateReaction"           },//清洗机械手反应盘取杯
    {AppTaskFuncHandWash_CanMsg_Proc_PutCupPlateReaction            ,0           ,"PutCupPlateReaction"             },//清洗机械手反应盘放杯
    {AppTaskFuncHandWash_CanMsg_Proc_CatchCupPlateWash              ,0           ,"CatchCupPlateWash"               },//清洗机械手清洗盘取杯
    {AppTaskFuncHandWash_CanMsg_Proc_PutCupPlateWash                ,0           ,"PutCupPlateWash"                 },//清洗机械手清洗盘放杯
    {AppTaskFuncHandWash_CanMsg_Proc_CatchCupOnMeasureUnitModule    ,0           ,"CatchCupOnMeasureUnitModule"     },//清洗机械手测量模块取杯
    {AppTaskFuncHandWash_CanMsg_Proc_PutCupOnMeasureUnitModule      ,0           ,"PutCupOnMeasureUnitModule"       },//清洗机械手测量模块放杯
    {AppTaskFuncHandWash_CanMsg_Proc_MotorReset                     ,1           ,"MotorReset"                      },//指定步进电机复位
    {AppTaskFuncHandWash_CanMsg_Proc_MoterRunSteps                  ,5           ,"MoterRunSteps"                   },//指定步进电机走指定步数
};
#define TASK_CAN_MSG_COMMAND_COUNT_HAND_WASH  ((sizeof(AppTaskCanMsgProcFuncPtrArrayHandWash))/(sizeof(AppTaskCanMsgProcFuncPtrArrayHandWash[0])))

//模块CAN信息处理任务
static void AppTaskFuncHandWash_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_COMMAND_COUNT_HAND_WASH)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }
    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcFuncPtrArrayHandWash[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcFuncPtrArrayHandWash[commandValue].funcPtr(canMsgCachePtr,
                                                                                AppTaskCanMsgProcFuncPtrArrayHandWash[commandValue].debugDetailMsg);
    //打印结果
    AppShowEndMsg((uint8_t*)AppHandWashDebugEndMsg,errorCode);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//清洗机械手复位
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

     //调用复位函数 默认使用零位偏移
    actionResult = AppImplHandWashReset();

    //返回结果
    return actionResult;
}

//清洗机械手旋转到反应盘
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_RotatePlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashRotateToPlateReaction();
    //返回结果
    return actionResult;
}

//清洗机械手旋转到清洗盘
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_RotatePlateWash(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashRotateToPlateWash();
    //返回结果
    return actionResult;

}

//清洗机械手旋转到测量模块
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_RotateMeasureModule(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashRotateToMeasureUnit();
    //返回结果
    return actionResult;
}

//清洗机械手反应盘取杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_CatchCupPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    SENSOR_STATE cupExistPlateReactionOld;
    actionResult = AppImplHandWashCatchCupOnPlateReaction(&cupExistPlateReactionOld);
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)(cupExistPlateReactionOld);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_HAND_WASH,dataBufferSend,1);
    }
    //返回结果
    return actionResult;
}

//清洗机械手反应盘放杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_PutCupPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    SENSOR_STATE cupExistPlateReactionOld;
    actionResult = AppImplHandWashPutCupOnPlateReaction(&cupExistPlateReactionOld);
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)(cupExistPlateReactionOld);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_HAND_WASH,dataBufferSend,1);
    }
    //返回结果
    return actionResult;
}

//清洗机械手清洗盘取杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_CatchCupPlateWash(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    SENSOR_STATE cupExistPlateWashNew;
    actionResult = AppImplHandWashCatchCupOnPlateWash(&cupExistPlateWashNew);
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)(cupExistPlateWashNew);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_HAND_WASH,dataBufferSend,1);
    }
    //返回结果
    return actionResult;
}

//清洗机械手清洗盘放杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_PutCupPlateWash(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    SENSOR_STATE cupExistPlateWashNew;
    actionResult = AppImplHandWashPutCupOnPlateWash(&cupExistPlateWashNew);
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)(cupExistPlateWashNew);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_HAND_WASH,dataBufferSend,1);
    }
    //返回结果
    return actionResult;
}

//清洗机械手测量模块取杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_CatchCupOnMeasureUnitModule(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashCatchCupOnMeasureUnit();
    //返回结果
    return actionResult;
}

//清洗机械手测量模块放杯
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_PutCupOnMeasureUnitModule(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashPutCupOnMeasureUnit();
    //返回结果
    return actionResult;
}

//指定步进电机复位
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_MotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_MOTOR_HAND_WASH motorIndex = (INDEX_MOTOR_HAND_WASH)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)motorIndex),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashMotorReset(motorIndex);
    //返回结果
    return actionResult;
}

//指定步进电机走指定步数
static LH_ERR AppTaskFuncHandWash_CanMsg_Proc_MoterRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_MOTOR_HAND_WASH motorIndex = (INDEX_MOTOR_HAND_WASH)(cachePtrLocal->recvCacheArray[4]);
    int32_t runSteps = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,5);
    int32_t posAfterRun;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)motorIndex),((int32_t)runSteps),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashMotorRunSteps(motorIndex,runSteps,&posAfterRun);
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)motorIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,posAfterRun);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_MODULE_MOTOR,dataBufferSend,5);
    }
    //返回结果
    return actionResult;
}




/************************************************自身分层API调用的消息的处理**********************************/
//清洗机械手复位
static LH_ERR AppTaskFuncHandWash_Self_Proc_Reset(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手旋转到反应盘
static LH_ERR AppTaskFuncHandWash_Self_Proc_RotatePlateReaction(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手旋转到清洗盘
static LH_ERR AppTaskFuncHandWash_Self_Proc_RotatePlateWash(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手旋转到测量模块
static LH_ERR AppTaskFuncHandWash_Self_Proc_RotateMeasureUnit(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手反应盘取杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_CatchCupPlateReaction(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手反应盘放杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_PutCupPlateReaction(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手清洗盘取杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_CatchCupPlateWash(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手清洗盘放杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_PutCupPlateWash(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手测量模块放杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_CatchCupOnMeasureUnitModule(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//清洗机械手测量模块取杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_PutCupOnMeasureUnitModule(SYSTEM_CMD_SELF* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcFuncPtrArrayHandWash[] = {
        /****前一个是处理函数***************************************参数的最小长度*****/
    {AppTaskFuncHandWash_Self_Proc_Reset                    ,"Reset"                    },//清洗机械手复位
    {AppTaskFuncHandWash_Self_Proc_RotatePlateReaction      ,"RotatePlateReaction"      },//清洗机械手旋转到反应盘
    {AppTaskFuncHandWash_Self_Proc_RotatePlateWash          ,"RotatePlateWash"          },//清洗机械手旋转到清洗盘
    {AppTaskFuncHandWash_Self_Proc_RotateMeasureUnit        ,"RotateMeasureUnit"        },//清洗机械手旋转到测量模块
    {AppTaskFuncHandWash_Self_Proc_CatchCupPlateReaction    ,"CatchCupPlateReaction"    },//清洗机械手反应盘取杯
    {AppTaskFuncHandWash_Self_Proc_PutCupPlateReaction      ,"PutCupPlateReaction"      },//清洗机械手反应盘放杯
    {AppTaskFuncHandWash_Self_Proc_CatchCupPlateWash        ,"CatchCupPlateWash"        },//清洗机械手清洗盘取杯
    {AppTaskFuncHandWash_Self_Proc_PutCupPlateWash          ,"PutCupPlateWash"          },//清洗机械手清洗盘放杯
    {AppTaskFuncHandWash_Self_Proc_CatchCupOnMeasureUnitModule    ,"CatchCupOnMeasureUnitModule"    },//清洗机械手测量模块取杯
    {AppTaskFuncHandWash_Self_Proc_PutCupOnMeasureUnitModule      ,"PutCupOnMeasureUnitModule"      },//清洗机械手测量模块放杯
};

//自身支持指令数量
#define TASK_SELF_MSG_COMMAND_COUNT_HAND_WASH  ((sizeof(AppTaskSelfMsgProcFuncPtrArrayHandWash))/(sizeof(AppTaskSelfMsgProcFuncPtrArrayHandWash[0])))


//模块自身转发消息处理任务
static void AppTaskFuncHandWash_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_HAND_WASH* cmdResultPtr = (DATA_IPC_HAND_WASH*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_COMMAND_COUNT_HAND_WASH)
	{
       AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)"Unsupport",selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,
                        selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcFuncPtrArrayHandWash[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,AppTaskSelfMsgProcFuncPtrArrayHandWash[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    AppShowEndMsg((uint8_t*)AppHandWashDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}


//清洗机械手复位
static LH_ERR AppTaskFuncHandWash_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

     //调用复位函数
    actionResult = AppImplHandWashReset();

    //返回结果
    return actionResult;
}

//清洗机械手旋转到反应盘
static LH_ERR AppTaskFuncHandWash_Self_Proc_RotatePlateReaction(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashRotateToPlateReaction();
    //返回结果
    return actionResult;
}

//清洗机械手旋转到清洗盘
static LH_ERR AppTaskFuncHandWash_Self_Proc_RotatePlateWash(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashRotateToPlateWash();
    //返回结果
    return actionResult;
}

//清洗机械手旋转到测量模块
static LH_ERR AppTaskFuncHandWash_Self_Proc_RotateMeasureUnit(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashRotateToMeasureUnit();
    //返回结果
    return actionResult;
}

//清洗机械手反应盘取杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_CatchCupPlateReaction(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    SENSOR_STATE cupExistPlateReactionOld;
    actionResult = AppImplHandWashCatchCupOnPlateReaction(&cupExistPlateReactionOld);
    //返回结果
    return actionResult;
}

//清洗机械手反应盘放杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_PutCupPlateReaction(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    SENSOR_STATE cupExistPlateReactionOld;
    actionResult = AppImplHandWashPutCupOnPlateReaction(&cupExistPlateReactionOld);
    //返回结果
    return actionResult;
}

//清洗机械手清洗盘取杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_CatchCupPlateWash(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    SENSOR_STATE cupExistPlateWashNew;
    actionResult = AppImplHandWashCatchCupOnPlateWash(&cupExistPlateWashNew);
    //返回结果
    return actionResult;
}

//清洗机械手清洗盘放杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_PutCupPlateWash(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    SENSOR_STATE cupExistPlateWashNew;
    actionResult = AppImplHandWashPutCupOnPlateWash(&cupExistPlateWashNew);
    //返回结果
    return actionResult;
}

//清洗机械手测量模块放杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_CatchCupOnMeasureUnitModule(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashPutCupOnMeasureUnit();
    //返回结果
    return actionResult;
}

//清洗机械手测量模块取杯
static LH_ERR AppTaskFuncHandWash_Self_Proc_PutCupOnMeasureUnitModule(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandWashDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandWashCatchCupOnMeasureUnit();
    //返回结果
    return actionResult;
}
