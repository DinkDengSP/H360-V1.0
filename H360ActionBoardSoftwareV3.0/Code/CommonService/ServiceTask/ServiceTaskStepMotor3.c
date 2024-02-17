/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-12 19:09:18
**FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceTaskStepMotor3.c
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
CPU_STK stackBufferServiceTaskStepMotor3[STK_SIZE_SERVICE_TASK_STEP_MOTOR3]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskStepMotor3@".ccram";


//模块CAN信息处理任务
static void ServiceTaskFuncStepMotor3_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void ServiceTaskFuncStepMotor3_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void ServiceTaskFuncServiceTaskStepMotor3(void *p_arg)
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
                ServiceTaskFuncStepMotor3_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                ServiceTaskFuncStepMotor3_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}


/************************************************CAN总线接收到的消息的处理******************************************************/
//电机复位
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);
//电机走特定步数
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_RunSpecialSteps(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);
//电机走到特定坐标
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_Run2SpecialCoordinates(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);
//电机回零+检测传感器
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_ReturnZeroWithSensor(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);
//电机回零不复位
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_ReturnZeroWithoutReset(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);
//电机持续运转
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_RunAlways(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);
//电机立即停止
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_StopImmediately(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);
//电机减速停止
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_StopDeceleration(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);
//读取电机当前坐标
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_ReadCurrentPosition(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
//前一个是处理函数 后一个参数的最小长度
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT ServiceTaskCanMsgProcStepMotor3Setting[] = {
    {ServiceTaskFuncStepMotor3_CanMsg_Proc_Reset                    ,           8           ,   ""          },
    {ServiceTaskFuncStepMotor3_CanMsg_Proc_RunSpecialSteps          ,           13          ,   ""          },
    {ServiceTaskFuncStepMotor3_CanMsg_Proc_Run2SpecialCoordinates   ,           13          ,   ""          },
    {ServiceTaskFuncStepMotor3_CanMsg_Proc_ReturnZeroWithSensor     ,           13          ,   ""          },
    {ServiceTaskFuncStepMotor3_CanMsg_Proc_RunAlways                ,           3           ,   ""          },
    {ServiceTaskFuncStepMotor3_CanMsg_Proc_StopImmediately          ,           0           ,   ""          },
    {ServiceTaskFuncStepMotor3_CanMsg_Proc_StopDeceleration         ,           0           ,   ""          },
    {ServiceTaskFuncStepMotor3_CanMsg_Proc_ReadCurrentPosition      ,           0           ,   ""          },
    {ServiceTaskFuncStepMotor3_CanMsg_Proc_ReturnZeroWithoutReset   ,           9           ,   ""          },
};
//支持的指令数量
#define TASK_CAN_MSG_STEP_MOTOR3_COMMAND_COUNT  ((sizeof(ServiceTaskCanMsgProcStepMotor3Setting))/(sizeof(ServiceTaskCanMsgProcStepMotor3Setting[0])))

//模块CAN信息处理任务
static void ServiceTaskFuncStepMotor3_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_STEP_MOTOR3_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +ServiceTaskCanMsgProcStepMotor3Setting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = ServiceTaskCanMsgProcStepMotor3Setting[commandValue].funcPtr(canMsgCachePtr,ServiceTaskCanMsgProcStepMotor3Setting[commandValue].debugDetailMsg);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//电机复位
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return ServiceImplStepMotor3ResetWithCanMsg(canMsgCachePtr);
}

//电机走特定步数
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_RunSpecialSteps(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return ServiceImplStepMotor3RunSpecialStepsWithCanMsg(canMsgCachePtr);
}

//电机走到特定坐标
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_Run2SpecialCoordinates(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return ServiceImplStepMotor3RunToCoordinatesWithCanMsg(canMsgCachePtr);
}

//电机回零+检测传感器
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_ReturnZeroWithSensor(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return ServiceImplStepMotor3ReturnZeroWithCanMsg(canMsgCachePtr);
}

//电机回零不复位
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_ReturnZeroWithoutReset(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return ServiceImplStepMotor3ReturnZeroWithoutResetWithCanMsg(canMsgCachePtr);
}

//电机持续运转
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_RunAlways(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return ServiceImplStepMotor3RunAlwaysWithCanMsg(canMsgCachePtr);
}

//电机立即停止
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_StopImmediately(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return ServiceImplStepMotor3StopImmediatelyWithCanMsg(canMsgCachePtr);
}

//电机减速停止
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_StopDeceleration(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return ServiceImplStepMotor3StopDecelerationWithCanMsg(canMsgCachePtr);
}

//读取电机当前坐标
static LH_ERR ServiceTaskFuncStepMotor3_CanMsg_Proc_ReadCurrentPosition(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    return ServiceImplStepMotor3ReadCurrentPositionWithCanMsg(canMsgCachePtr);
}




/************************************************自身分层API调用的消息的处理**********************************/
//步进电机复位
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//步进电机走指定步数
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_RunSteps(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//步进电机走坐标
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_Run2Coordinate(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//步进电机回零
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_ReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//步进电机回零不复位
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_ReturnZeroWithoutReset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//步进电机持续运转
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_RunAlways(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//步进电机立即停止
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_StopImmediately(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//步进电机减速停止
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_StopDeceleration(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//步进电机更新状态
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_ReflushState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//空,现在没用
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_Nop(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//步进电机设置状态
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_SetState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT ServiceTaskSelfMsgProcStepMotor3Setting[] = {
    {ServiceTaskFuncStepMotor3_Self_Proc_Reset                      ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_RunSteps                   ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_Run2Coordinate             ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_ReturnZero                 ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_RunAlways                  ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_StopImmediately            ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_StopDeceleration           ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_ReflushState               ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_ReturnZeroWithoutReset     ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_Nop                        ,  ""           },
    {ServiceTaskFuncStepMotor3_Self_Proc_SetState                   ,  ""           },
};
//自身支持指令数量
#define TASK_SELF_MSG_STEP_MOTOR3_COMMAND_COUNT  ((sizeof(ServiceTaskSelfMsgProcStepMotor3Setting))/(sizeof(ServiceTaskSelfMsgProcStepMotor3Setting[0])))


//模块自身转发消息处理任务
static void ServiceTaskFuncStepMotor3_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_STEPMOTOR* cmdResultPtr = (DATA_IPC_STEPMOTOR*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_STEP_MOTOR3_COMMAND_COUNT)
	{
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = ServiceTaskSelfMsgProcStepMotor3Setting[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,ServiceTaskSelfMsgProcStepMotor3Setting[selfCmdPtr->commandIndex].debugDetailMsg);
	}
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//步进电机复位
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3ResetWithSelfMsg(selfCmdPtr);
    return errCode;
}

//步进电机走指定步数
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_RunSteps(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3RunStepsWithSelfMsg(selfCmdPtr);
    return errCode;
}

//步进电机走坐标
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_Run2Coordinate(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3Run2CoordinatesWithSelfMsg(selfCmdPtr);
    return errCode;
}

//步进电机回零
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_ReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3ReturnZeroWithSelfMsg(selfCmdPtr);
    return errCode;
}

//步进电机回零不复位
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_ReturnZeroWithoutReset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3ReturnZeroWithoutResetWithSelfMsg(selfCmdPtr);
    return errCode;
}

//步进电机持续运转
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_RunAlways(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3RunAlwaysWithSelfMsg(selfCmdPtr);
    return errCode;
}

//步进电机立即停止
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_StopImmediately(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3StopImmediatelyWithSelfMsg(selfCmdPtr);
    return errCode;
}

//步进电机减速停止
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_StopDeceleration(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3StopDecelerationWithSelfMsg(selfCmdPtr);
    return errCode;
}

//步进电机更新状态
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_ReflushState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3ReadCurrentPositionWithSelfMsg(selfCmdPtr);
    return errCode;
}

//空现在没用
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_Nop(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    return errCode;
}
//步进电机设置状态
static LH_ERR ServiceTaskFuncStepMotor3_Self_Proc_SetState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    errCode = ServiceImplStepMotor3SetStateWithSelfMsg(selfCmdPtr);
    return errCode;
}
