/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-23 11:55:20
**FilePath: \H360_ActionBoard_ID_4d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceTaskInput.c
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
CPU_STK stackBufferServiceTaskInputRead[STK_SIZE_SERVICE_TASK_INPUT_READ]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskInputRead@".ccram";



//模块CAN信息处理任务
static void ServiceTaskFuncInput_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void ServiceTaskFuncInput_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);



//任务函数
void ServiceTaskFuncInputRead(void *p_arg)
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
                ServiceTaskFuncInput_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                ServiceTaskFuncInput_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}


/************************************************CAN总线接收到的消息的处理******************************************************/
//读取指定端口的IO状态并返回
static LH_ERR ServiceTaskFuncInput_CanMsg_Proc_ReadSingle(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//系统内步进电机全部急停
static LH_ERR ServiceTaskFuncInput_CanMsg_Proc_AllStepMotorEmergency(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
//前一个是处理函数 后一个参数的最小长度
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT ServiceTaskCanMsgProcInputSetting[] = {
    {ServiceTaskFuncInput_CanMsg_Proc_ReadSingle                ,           1           ,""     },
    {ServiceTaskFuncInput_CanMsg_Proc_AllStepMotorEmergency     ,           0           ,""     }
};
//支持的指令数量
#define TASK_CAN_MSG_INPUT_COMMAND_COUNT  ((sizeof(ServiceTaskCanMsgProcInputSetting))/(sizeof(ServiceTaskCanMsgProcInputSetting[0])))

//模块CAN信息处理任务
static void ServiceTaskFuncInput_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_INPUT_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +ServiceTaskCanMsgProcInputSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = ServiceTaskCanMsgProcInputSetting[commandValue].funcPtr(canMsgCachePtr,
                                            ServiceTaskCanMsgProcInputSetting[commandValue].debugDetailMsg);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//读取指定端口的IO状态并返回
static LH_ERR ServiceTaskFuncInput_CanMsg_Proc_ReadSingle(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    uint8_t pinIndex = 0;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //读取序号
    pinIndex = cachePtrLocal->recvCacheArray[4];
    //端口序号错误
    if(pinIndex >= BOARD_IN_PIN_COUNT)
    {
        return LH_ERR_SERVICE_READ_INPUT_INDEX;
    }
    //读取端口值
    BitAction pinStatus = ServiceImplInputReadValue((BOARD_IN_PIN)pinIndex);
    //申请内存
    uint8_t* dataBufferSend = NULL;
    do
    {
        dataBufferSend = UserMemMalloc(SRAM_CCM,2);
    } while (dataBufferSend == NULL);

    //将端口值返回
    dataBufferSend[0] = pinIndex;
    dataBufferSend[1] = (uint8_t)pinStatus;
    //返回数据包
    CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_IO_IN,dataBufferSend,2);
    //发送完成,释放内存
    UserMemFree(SRAM_CCM,dataBufferSend);
    //返回结果代码
    return LH_ERR_NONE;
}

//系统内步进电机全部急停
static LH_ERR ServiceTaskFuncInput_CanMsg_Proc_AllStepMotorEmergency(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    //12个步进电机全部急停
    ServiceImplInputStopEmergencyWithCanMsg(canMsgCachePtr);
    //返回结果代码
    return LH_ERR_NONE;
}


/************************************************自身分层API调用的消息的处理**********************************/
//输入读取单个IO的状态
static LH_ERR ServiceTaskFuncInput_Self_Proc_ReadSingle(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//输入步进电机全部紧急急停
static LH_ERR ServiceTaskFuncInput_Self_Proc_AllStepMotorEmergency(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);


//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT ServiceTaskSelfMsgProcInputSetting[] = {
    {ServiceTaskFuncInput_Self_Proc_ReadSingle              ,"" },
    {ServiceTaskFuncInput_Self_Proc_AllStepMotorEmergency   ,"" }
};
//自身支持指令数量
#define TASK_SELF_MSG_INPUT_COMMAND_COUNT  ((sizeof(ServiceTaskSelfMsgProcInputSetting))/(sizeof(ServiceTaskSelfMsgProcInputSetting[0])))


//模块自身转发消息处理任务
static void ServiceTaskFuncInput_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_INPUT* cmdResultPtr = (DATA_IPC_INPUT*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_INPUT_COMMAND_COUNT)
	{
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = ServiceTaskSelfMsgProcInputSetting[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                            ServiceTaskSelfMsgProcInputSetting[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//输入读取单个IO的状态
static LH_ERR ServiceTaskFuncInput_Self_Proc_ReadSingle(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //解析参数
    DATA_IPC_INPUT* cmdResultPtr = (DATA_IPC_INPUT*)(selfCmdPtr->commandDataSpecialChannel);
    BOARD_IN_PIN pinIndex = (BOARD_IN_PIN)(selfCmdPtr->commandParam1);
    //调用函数
    BitAction bitValue = ServiceImplInputReadValue(pinIndex);
    //根据结果,更新状态
    cmdResultPtr->pinValue = bitValue;
    return errCode;
}

//输入步进电机全部紧急急停
static LH_ERR ServiceTaskFuncInput_Self_Proc_AllStepMotorEmergency(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //调用函数
    errCode = ServiceImplInputStopEmergencyWithSelfMsg();
    return errCode;
}




