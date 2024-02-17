/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:08:49
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-09 11:26:01
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
CPU_STK stackBufferAppTaskParamUtil[STK_SIZE_APP_TASK_PARAM_UTIL]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskParamUtil@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_PARAM_UTIL_PERIOD        100
//模块定时执行任务初始化
static void AppTaskFuncParamUtil_Auto_Init(void);
//模块定时执行任务
static void AppTaskFuncParamUtil_Auto_Proc(void);

//模块CAN信息处理任务
static void AppTaskFuncParamUtil_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);


//任务函数
void AppTaskFuncParamUtil(void *p_arg)
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
    AppTaskFuncParamUtil_Auto_Init();
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_PARAM_UTIL_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_PARAM_UTIL_PERIOD)
		{
			//上次发送时间和本次发送时间差了TIME_MS_AUTO_TASK_PARAM_UTIL_PERIOD个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncParamUtil_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_PARAM_UTIL_PERIOD,
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
                AppTaskFuncParamUtil_CanMsg_Proc(canRecvDatPtr);
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
static void AppTaskFuncParamUtil_Auto_Init(void)
{
    AppImplParamUtilFSM_Init();
}
//模块定时执行任务
static void AppTaskFuncParamUtil_Auto_Proc(void)
{
    AppImplParamUtilFSM_Run();
}


/***********************************************模块CAN消息处理***********************************************************/
//参数恢复默认值
static LH_ERR AppTaskFuncParamUtil_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//读取参数
static LH_ERR AppTaskFuncParamUtil_CanMsg_Proc_ReadParam(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//写入参数
static LH_ERR AppTaskFuncParamUtil_CanMsg_Proc_WriteParam(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);


//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcParamUtilSetting[] = {
    {AppTaskFuncParamUtil_CanMsg_Proc_Reset       ,0  ,""},//参数恢复默认值
    {AppTaskFuncParamUtil_CanMsg_Proc_ReadParam   ,2  ,""},//读取参数
    {AppTaskFuncParamUtil_CanMsg_Proc_WriteParam  ,6  ,""},//写入参数
};
#define TASK_CAN_MSG_PARAM_UTIL_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcParamUtilSetting))/(sizeof(AppTaskCanMsgProcParamUtilSetting[0])))

//模块CAN信息处理任务
static void AppTaskFuncParamUtil_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_PARAM_UTIL_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcParamUtilSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcParamUtilSetting[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcParamUtilSetting[commandValue].debugDetailMsg);

    //如果存在错误则打印错误结果
    if(errorCode != LH_ERR_NONE)
    {
        LowLevelPrintf("Func: %s,Line: %d,ErrorCode : 0X%08X\r\n", __FUNCTION__,__LINE__,errorCode);
    }
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//参数恢复默认值
static LH_ERR AppTaskFuncParamUtil_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ResetParam\r\n");
    AppImplParamUtilResetAllParam();
    return LH_ERR_NONE;
}

//读取参数
static LH_ERR AppTaskFuncParamUtil_CanMsg_Proc_ReadParam(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //获取主序号和子序号
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    uint8_t mainIndex = (uint8_t)(cachePtrLocal->recvCacheArray[4]);
    uint8_t subIndex = (uint8_t)(cachePtrLocal->recvCacheArray[5]);
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg ReadParam,mainIndex: %d,subIndex: %d\r\n",mainIndex,subIndex);
    //调用读参数
    int32_t paramRead = 0;
    errorCode = AppImplParamUtilReadParam(mainIndex,subIndex,&paramRead);
    if(errorCode == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[4] = {0};
        CanBaseWriteInt32DataToBuffer(dataBufferSend,0,paramRead);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_PARAM,dataBufferSend,4);
    }
    //返回结果
    return errorCode;
}

//写入参数
static LH_ERR AppTaskFuncParamUtil_CanMsg_Proc_WriteParam(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //获取主序号和子序号
    LH_ERR errorCode = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    uint8_t mainIndex = (uint8_t)(cachePtrLocal->recvCacheArray[4]);
    uint8_t subIndex = (uint8_t)(cachePtrLocal->recvCacheArray[5]);
    int32_t writeData = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,6);
    //动作开始,打印开始信息
    AppShowLogMsg("CanMsg WriteParam,mainIndex: %d,subIndex: %d,WriteData: 0X%08X\r\n",mainIndex,subIndex,writeData);
    //调用写参数
    errorCode = AppImplParamUtilWriteParam(mainIndex,subIndex,writeData);
    return errorCode;
}
