/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:10:02
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-14 15:56:47
**FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppTask\AppTaskHandNewCup.c
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
CPU_STK stackBufferAppTaskHandNewCup[STK_SIZE_APP_TASK_HAND_NEW_CUP]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskHandNewCup@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_HAND_NEW_CUP_PERIOD        0
//模块定时任务初始化
static void AppTaskFuncHandNewCup_Auto_Init(void);
#if(TIME_MS_AUTO_TASK_HAND_NEW_CUP_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncHandNewCup_Auto_Proc(void);
#endif

//模块CAN信息处理任务
static void AppTaskFuncHandNewCup_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncHandNewCup_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncHandNewCup(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //模块定时任务初始化
    AppTaskFuncHandNewCup_Auto_Init();
    #if(TIME_MS_AUTO_TASK_HAND_NEW_CUP_PERIOD != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_HAND_NEW_CUP_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_HAND_NEW_CUP_PERIOD)
		{
			//上次发送时间和本次发送时间差了1000个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncHandNewCup_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_HAND_NEW_CUP_PERIOD,
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
                AppTaskFuncHandNewCup_CanMsg_Proc(canRecvDatPtr);
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncHandNewCup_Self_Proc(selfCmdPtr);
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
static void AppTaskFuncHandNewCup_Auto_Init(void)
{

}
#if(TIME_MS_AUTO_TASK_HAND_NEW_CUP_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncHandNewCup_Auto_Proc(void)
{
    return;
}
#endif


/************************************************CAN总线接收到的消息的处理******************************************************/
//新杯机械手复位
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到新杯位
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_MoveToNewCupTray(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到反应盘
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_MoveToPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到垃圾桶
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_MoveToGarbage(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯机械手新杯区取杯
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_CatchCupNewCupTray(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯机械手反应盘取杯
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_CatchCupPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯机械手反应盘放杯
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_PutCupPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯机械手垃圾桶放杯
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_PutCupGarbage(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机复位
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机走指定步数
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcFuncPtrArrayHandNewCup[] = {
    /****前一个是处理函数***************************************参数的最小长度*****/
    {AppTaskFuncHandNewCup_CanMsg_Proc_Reset                ,           0           ,"Reset"                    },//新杯机械手复位
    {AppTaskFuncHandNewCup_CanMsg_Proc_MoveToNewCupTray     ,           2           ,"MoveToNewCupTray"         },//新杯机械手移动到新杯位
    {AppTaskFuncHandNewCup_CanMsg_Proc_MoveToPlateReaction  ,           0           ,"MoveToPlateReaction"      },//新杯机械手移动到反应盘
    {AppTaskFuncHandNewCup_CanMsg_Proc_MoveToGarbage        ,           1           ,"MoveToGarbage"            },//新杯机械手移动到垃圾桶
    {AppTaskFuncHandNewCup_CanMsg_Proc_CatchCupNewCupTray   ,           0           ,"CatchCupNewCupTray"       },//新杯机械手新杯区取杯
    {AppTaskFuncHandNewCup_CanMsg_Proc_CatchCupPlateReaction,           0           ,"CatchCupPlateReaction"    },//新杯机械手反应盘取杯
    {AppTaskFuncHandNewCup_CanMsg_Proc_PutCupPlateReaction  ,           0           ,"PutCupPlateReaction"      },//新杯机械手反应盘放杯
    {AppTaskFuncHandNewCup_CanMsg_Proc_PutCupGarbage        ,           1           ,"PutCupGarbage"            },//新杯机械手垃圾桶放杯
    {AppTaskFuncHandNewCup_CanMsg_Proc_StepMotorReset       ,           1           ,"StepMotorReset"           },//指定步进电机复位
    {AppTaskFuncHandNewCup_CanMsg_Proc_StepMotorRunSteps    ,           5           ,"StepMotorRunSteps"        },//指定步进电机走指定步数
};
#define TASK_CAN_MSG_COMMAND_COUNT_HAND_NEW_CUP  ((sizeof(AppTaskCanMsgProcFuncPtrArrayHandNewCup))/(sizeof(AppTaskCanMsgProcFuncPtrArrayHandNewCup[0])))

//模块CAN信息处理任务
static void AppTaskFuncHandNewCup_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_COMMAND_COUNT_HAND_NEW_CUP)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }
    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcFuncPtrArrayHandNewCup[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcFuncPtrArrayHandNewCup[commandValue].funcPtr(canMsgCachePtr,
                                                                                AppTaskCanMsgProcFuncPtrArrayHandNewCup[commandValue].debugDetailMsg);
    //打印结果
    AppShowEndMsg((uint8_t*)AppHandNewCupDebugEndMsg,errorCode);
    //返回结果前将通道状态置为空闲
    canMsgCachePtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//新杯机械手复位
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //调用函数
    actionResult = AppImplHandNewCupReset();
    //返回结果
    return actionResult;
}

//新杯机械手移动到新杯位
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_MoveToNewCupTray(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    uint8_t lineBackFront = (uint8_t)(cachePtrLocal->recvCacheArray[4]);
    uint8_t colLeftRight = (uint8_t)(cachePtrLocal->recvCacheArray[5]);
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)lineBackFront),((int32_t)colLeftRight),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupMoveToNewCupTray(lineBackFront,colLeftRight);
    //返回结果
    return actionResult;    
}

//新杯机械手移动到反应盘
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_MoveToPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupMoveToPlateReaction();
    //返回结果
    return actionResult;
}

//新杯机械手移动到垃圾桶
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_MoveToGarbage(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    HAND_NEW_CUP_GARBAGE_NO garbageNo = (HAND_NEW_CUP_GARBAGE_NO)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupMoveToGarbage(garbageNo);
    //返回结果
    return actionResult;  
}

//新杯机械手新杯区取杯
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_CatchCupNewCupTray(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    //CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupCatchCupNewCupTray();
    //返回结果
    return actionResult;
}

//新杯机械手反应盘取杯
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_CatchCupPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    SENSOR_STATE cupExistPlateReactionNew;
    actionResult = AppImplHandNewCupCatchCupPlateReaction(&cupExistPlateReactionNew);
    //返回数据
    if(actionResult == LH_ERR_NONE)
    {
        //盘上光纤检测结果
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)(cupExistPlateReactionNew);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_HAND_NEW_CUP,dataBufferSend,1);
    }
    //返回结果
    return actionResult;
}

//新杯机械手反应盘放杯
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_PutCupPlateReaction(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    SENSOR_STATE cupExistPlateReactionNew;
    actionResult = AppImplHandNewCupPutCupPlateReaction(&cupExistPlateReactionNew);
    if(actionResult == LH_ERR_NONE)
    {
        //盘上光纤检测结果
        uint8_t dataBufferSend[1] = {0};
        dataBufferSend[0] = (uint8_t)(cupExistPlateReactionNew);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_HAND_NEW_CUP,dataBufferSend,1);
    }
    //返回结果
    return actionResult;
}

//新杯机械手垃圾桶放杯
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_PutCupGarbage(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    HAND_NEW_CUP_GARBAGE_NO garbageNo = (HAND_NEW_CUP_GARBAGE_NO)(cachePtrLocal->recvCacheArray[4]);

    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupPutCupGarbage(garbageNo);
    //返回结果
    return actionResult;  
}

//指定步进电机复位
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_MOTOR_HAND_NEW_CUP motorIndex = (INDEX_MOTOR_HAND_NEW_CUP)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)motorIndex),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupStepMotorReset(motorIndex);
    //返回结果
    return actionResult;
}

//指定步进电机走指定步数
static LH_ERR AppTaskFuncHandNewCup_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_MOTOR_HAND_NEW_CUP motorIndex = (INDEX_MOTOR_HAND_NEW_CUP)(cachePtrLocal->recvCacheArray[4]);
    int32_t runSteps = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,5);
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)motorIndex),((int32_t)runSteps),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    int32_t posAfterRun;
    actionResult = AppImplHandNewCupStepMotorRunSteps(motorIndex,runSteps,&posAfterRun);
    //返回数据
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
//新杯机械手复位
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到新杯位
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_MoveToNewCupTray(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到反应盘
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_MoveToPlateReaction(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手移动到垃圾桶
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_MoveToGarbage(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手新杯区取杯
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_CatchCupNewCupTray(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手反应盘取杯
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_CatchCupPlateReaction(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手反应盘放杯
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_PutCupPlateReaction(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯机械手垃圾桶放杯
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_PutCupGarbage(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);


//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcFuncPtrArrayHandNewCup[] = {
    {AppTaskFuncHandNewCup_Self_Proc_Reset                ,                   "Reset"                  },//新杯机械手复位
    {AppTaskFuncHandNewCup_Self_Proc_MoveToNewCupTray     ,                   "MoveToNewCupTray"       },//新杯机械手移动到新杯位
    {AppTaskFuncHandNewCup_Self_Proc_MoveToPlateReaction  ,                   "MoveToPlateReaction"    },//新杯机械手移动到反应盘
    {AppTaskFuncHandNewCup_Self_Proc_MoveToGarbage        ,                   "MoveToGarbage"          },//新杯机械手移动到垃圾桶
    {AppTaskFuncHandNewCup_Self_Proc_CatchCupNewCupTray   ,                   "CatchCupNewCupTray"     },//新杯机械手新杯区取杯
    {AppTaskFuncHandNewCup_Self_Proc_CatchCupPlateReaction,                   "CatchCupPlateReaction"  },//新杯机械手反应盘取杯
    {AppTaskFuncHandNewCup_Self_Proc_PutCupPlateReaction  ,                   "PutCupPlateReaction"    },//新杯机械手反应盘放杯
    {AppTaskFuncHandNewCup_Self_Proc_PutCupGarbage        ,                   "PutCupGarbage"          },//新杯机械手垃圾桶放杯
};

//自身支持指令数量
#define TASK_SELF_MSG_COMMAND_COUNT_HAND_NEW_CUP  ((sizeof(AppTaskSelfMsgProcFuncPtrArrayHandNewCup))/(sizeof(AppTaskSelfMsgProcFuncPtrArrayHandNewCup[0])))


//模块自身转发消息处理任务
static void AppTaskFuncHandNewCup_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_HAND_NEW_CUP* cmdResultPtr = (DATA_IPC_HAND_NEW_CUP*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_COMMAND_COUNT_HAND_NEW_CUP)
	{
       AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)"Unsupport",selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,
                        selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcFuncPtrArrayHandNewCup[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,AppTaskSelfMsgProcFuncPtrArrayHandNewCup[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    AppShowEndMsg((uint8_t*)AppHandNewCupDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}


//新杯机械手复位
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupReset();
    //返回结果
    return actionResult;
}

//新杯机械手移动到新杯位
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_MoveToNewCupTray(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    uint8_t lineBackfront = (uint8_t)(selfCmdPtr->commandParam1);
    uint8_t colLeftright = (uint8_t)(selfCmdPtr->commandParam2);

    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)lineBackfront),((int32_t)colLeftright),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupMoveToNewCupTray(lineBackfront,colLeftright);
    //返回结果
    return actionResult;
}

//新杯机械手移动到反应盘
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_MoveToPlateReaction(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupMoveToPlateReaction();
    //返回结果
    return actionResult;
}

//新杯机械手移动到垃圾桶
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_MoveToGarbage(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    HAND_NEW_CUP_GARBAGE_NO garbageNo = (HAND_NEW_CUP_GARBAGE_NO)(selfCmdPtr->commandParam1);
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupMoveToGarbage(garbageNo);
    //返回结果
    return actionResult;
}

//新杯机械手新杯区取杯
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_CatchCupNewCupTray(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupCatchCupNewCupTray();
    //返回结果
    return actionResult;
}

//新杯机械手反应盘取杯
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_CatchCupPlateReaction(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    SENSOR_STATE cupExistPlateReactionNew;
    actionResult = AppImplHandNewCupCatchCupPlateReaction(&cupExistPlateReactionNew);
    //返回结果
    return actionResult;
}

//新杯机械手反应盘放杯
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_PutCupPlateReaction(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    SENSOR_STATE cupExistPlateReactionNew;
    actionResult = AppImplHandNewCupPutCupPlateReaction(&cupExistPlateReactionNew);
    //返回结果
    return actionResult;
}

//新杯机械手垃圾桶放杯
static LH_ERR AppTaskFuncHandNewCup_Self_Proc_PutCupGarbage(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数获取
    HAND_NEW_CUP_GARBAGE_NO garbageNo = (HAND_NEW_CUP_GARBAGE_NO)(selfCmdPtr->commandParam1);
    
    //动作开始，打印开始信息
    AppShowStartMsg((uint8_t*)AppHandNewCupDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数
    actionResult = AppImplHandNewCupPutCupGarbage(garbageNo);
    //返回结果
    return actionResult;
}
