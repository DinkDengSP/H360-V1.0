/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-12 19:09:11
**FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceTaskParam.c
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
CPU_STK stackBufferServiceTaskParam[STK_SIZE_SERVICE_TASK_PARAM]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskParam@".ccram";



//模块CAN信息处理任务
static void ServiceTaskFuncParam_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void ServiceTaskFuncParam_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void ServiceTaskFuncServiceTaskParam(void *p_arg)
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
                ServiceTaskFuncParam_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                ServiceTaskFuncParam_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}




/************************************************CAN总线接收到的消息的处理******************************************************/
//读取电机参数
static LH_ERR ServiceTaskFuncParam_CanMsg_Proc_ReadParamStepMotor(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);
//写入电机参数
static LH_ERR ServiceTaskFuncParam_CanMsg_Proc_WriteParamStepMotor(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
//前一个是处理函数 后一个参数的最小长度
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT ServiceTaskCanMsgProcParamSetting[] = {
    {ServiceTaskFuncParam_CanMsg_Proc_ReadParamStepMotor         ,           4           ,  ""  },
    {ServiceTaskFuncParam_CanMsg_Proc_WriteParamStepMotor        ,           8           ,  ""  },
};

#define TASK_CAN_MSG_PARAM_COMMAND_COUNT  ((sizeof(ServiceTaskCanMsgProcParamSetting))/(sizeof(ServiceTaskCanMsgProcParamSetting[0])))


//模块CAN信息处理任务
static void ServiceTaskFuncParam_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_PARAM_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +ServiceTaskCanMsgProcParamSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = ServiceTaskCanMsgProcParamSetting[commandValue].funcPtr(canMsgCachePtr,ServiceTaskCanMsgProcParamSetting[commandValue].debugDetailMsg);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//读取电机参数
static LH_ERR ServiceTaskFuncParam_CanMsg_Proc_ReadParamStepMotor(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //电机序号
    uint8_t motorIndex = cachePtrLocal->recvCacheArray[4];
    //参数序号
    uint8_t mainParamIndex = cachePtrLocal->recvCacheArray[5];
    //辅助定位
    uint16_t subParamIndex = CanBaseReadDataConvertToUint16(cachePtrLocal->recvCacheArray,6);
    //读取到的参数缓存
    int32_t readParamTemp = 0;
    //读取参数
    errorCode = ServiceImplParam_ReadMotorParam(motorIndex,mainParamIndex,subParamIndex,&readParamTemp);
    if(errorCode != LH_ERR_NONE)
    {
        //发生错误,直接返回
        return errorCode;
    }
    //返回数据,申请内存
    uint8_t* dataBufferSend = NULL;
    do
    {
        dataBufferSend = UserMemMalloc(SRAM_CCM,4);
    } while (dataBufferSend == NULL);
    //将读取到的数据返回
    CanBaseWriteInt32DataToBuffer(dataBufferSend,0,readParamTemp);
    //返回数据包
    CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_PARAM,dataBufferSend,4);
    //发送完成,释放内存
    UserMemFree(SRAM_CCM,dataBufferSend);
    return LH_ERR_NONE;
}

//写入电机参数
static LH_ERR ServiceTaskFuncParam_CanMsg_Proc_WriteParamStepMotor(void* canMsgCachePtr,uint8_t*  detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //电机序号
    uint8_t motorIndex = cachePtrLocal->recvCacheArray[4];
    //参数序号
    uint8_t mainParamIndex = cachePtrLocal->recvCacheArray[5];
    //辅助定位
    uint16_t subParamIndex = CanBaseReadDataConvertToUint16(cachePtrLocal->recvCacheArray,6);
    //读取到的参数缓存
    int32_t writeParam = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,8);
    //写入参数
    errorCode = ServiceImplParam_WriteMotorParam(motorIndex,mainParamIndex,subParamIndex,writeParam);
    if(errorCode != LH_ERR_NONE)
    {
        //发生错误,直接返回
        return errorCode;
    }
    return LH_ERR_NONE;
}



/************************************************自身分层API调用的消息的处理**********************************/
//模块自身转发消息处理任务,目前还不用实现参数读写的自身回调
static void ServiceTaskFuncParam_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{

}

