/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-12 19:09:04
**FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceTaskOutput.c
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
CPU_STK stackBufferServiceTaskOutputWrite[STK_SIZE_SERVICE_TASK_OUTPUT_WRITE]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskOutputWrite@".ccram";



//模块CAN信息处理任务
static void ServiceTaskFuncOutput_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void ServiceTaskFuncOutput_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void ServiceTaskFuncOutputWrite(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    while(1)
    {
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)0,
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
                ServiceTaskFuncOutput_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                ServiceTaskFuncOutput_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}



/************************************************CAN总线接收到的消息的处理******************************************************/
//写入单个IO
static LH_ERR ServiceTaskFuncOutput_CanMsg_Proc_WriteSingel(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT ServiceTaskCanMsgProcOutputSetting[] = {
    /****前一个是处理函数***************************************参数的最小长度*****/
    {ServiceTaskFuncOutput_CanMsg_Proc_WriteSingel         ,           2           ,""}
};
#define TASK_CAN_MSG_OUTPUT_COMMAND_COUNT  ((sizeof(ServiceTaskCanMsgProcOutputSetting))/(sizeof(ServiceTaskCanMsgProcOutputSetting[0])))

//模块CAN信息处理任务
static void ServiceTaskFuncOutput_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_OUTPUT_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +ServiceTaskCanMsgProcOutputSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = ServiceTaskCanMsgProcOutputSetting[commandValue].funcPtr(canMsgCachePtr,ServiceTaskCanMsgProcOutputSetting[commandValue].debugDetailMsg);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//写入单个IO
static LH_ERR ServiceTaskFuncOutput_CanMsg_Proc_WriteSingel(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //读取序号和想要设置的状态
    uint8_t indexPin = cachePtrLocal->recvCacheArray[4];
    uint8_t pinValue = cachePtrLocal->recvCacheArray[5];
    //序号错误
    if (indexPin >= BOARD_OUT_PIN_COUNT) 
    {
        return LH_ERR_SERVICE_WRITE_OUTPUT_INDEX;
    }
    //数据校正,要么是0要么是1
    if(pinValue != 0)
    {
        pinValue = 1;
    }
    //写入数据
    ServiceImplOutputWrite((BOARD_OUT_PIN)indexPin,(BitAction)pinValue);
    //返回结果
    return LH_ERR_NONE;
}


/************************************************自身分层API调用的消息的处理**********************************/
//输入写入单个IO的状态
static LH_ERR ServiceTaskFuncOutput_Self_Proc_WriteSingle(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT ServiceTaskSelfMsgProcOutputSetting[] = {
    {ServiceTaskFuncOutput_Self_Proc_WriteSingle    ,   ""},
};

//自身支持指令数量
#define TASK_SELF_MSG_OUTPUT_COMMAND_COUNT  ((sizeof(ServiceTaskSelfMsgProcOutputSetting))/(sizeof(ServiceTaskSelfMsgProcOutputSetting[0])))

//模块自身转发消息处理任务
static void ServiceTaskFuncOutput_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_OUTPUT* cmdResultPtr = (DATA_IPC_OUTPUT*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_OUTPUT_COMMAND_COUNT)
	{
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = ServiceTaskSelfMsgProcOutputSetting[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,ServiceTaskSelfMsgProcOutputSetting[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//输出写入单个IO的状态
static LH_ERR ServiceTaskFuncOutput_Self_Proc_WriteSingle(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //解析参数
    //DATA_IPC_OUTPUT* cmdResultPtr = (DATA_IPC_OUTPUT*)(selfCmdPtr->commandDataSpecialChannel);
    BOARD_OUT_PIN pinIndex = (BOARD_OUT_PIN)(selfCmdPtr->commandParam1);
    BitAction writeValue = (BitAction)(selfCmdPtr->commandParam2);
    //调用函数
    ServiceImplOutputWrite(pinIndex,writeValue);
    return errCode;
}
