/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-12 16:35:44
**FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceTaskDebugIAP.c
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
CPU_STK stackBufferServiceTaskDebugIAP[STK_SIZE_SERVICE_TASK_DEBUG_IAP]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbServiceTaskDebugIAP@".ccram";


//模块CAN信息处理任务
static void ServiceTaskFuncDebugIAP_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void ServiceTaskFuncDebugIAP_Self_Proc(SYSTEM_CMD_SELF* selfCmd);


//任务函数
void ServiceTaskFuncDebugIAP(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //初始化当前系统模式
    SystemModeInit(MODE_RELEASE);
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
                ServiceTaskFuncDebugIAP_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmd = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                ServiceTaskFuncDebugIAP_Self_Proc(selfCmd);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmd);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}

/************************************************CAN总线接收到的消息的处理******************************************************/
//读取当前IAP模式,0 表示当前处于BootLoader模式下 1表示当前处于APP模式下
static LH_ERR ServiceTaskFuncDebugIAP_CanMsg_Proc_ReadCurrentIapMode(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//读取当前系统版本号
static LH_ERR ServiceTaskFuncDebugIAP_CanMsg_Proc_ReadCurrentSoftVersion(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//跳转到APP中,并设置标志位自动跳转
static LH_ERR ServiceTaskFuncDebugIAP_CanMsg_Proc_JmpBootloader(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//设置当前运行模式
static LH_ERR ServiceTaskFuncDebugIAP_CanMsg_Proc_SetSystemMode(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
//前一个是处理函数 后一个参数的最小长度
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT ServiceTaskCanMsgProcDebugIAPSetting[] = {
    {ServiceTaskFuncDebugIAP_CanMsg_Proc_ReadCurrentIapMode         ,0  ,"" },
    {ServiceTaskFuncDebugIAP_CanMsg_Proc_ReadCurrentSoftVersion     ,0  ,"" },
    {ServiceTaskFuncDebugIAP_CanMsg_Proc_JmpBootloader              ,0  ,"" },
    {ServiceTaskFuncDebugIAP_CanMsg_Proc_SetSystemMode              ,1  ,"" },
};
//回调函数数量
#define TASK_CAN_MSG_DEBUG_IAP_COMMAND_COUNT  ((sizeof(ServiceTaskCanMsgProcDebugIAPSetting))/(sizeof(ServiceTaskCanMsgProcDebugIAPSetting[0])))


//模块CAN信息处理任务
static void ServiceTaskFuncDebugIAP_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_DEBUG_IAP_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +ServiceTaskCanMsgProcDebugIAPSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = ServiceTaskCanMsgProcDebugIAPSetting[commandValue].funcPtr(canMsgCachePtr,ServiceTaskCanMsgProcDebugIAPSetting[commandValue].debugDetailMsg);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//读取当前IAP模式,0 表示当前处于BootLoader模式下 1表示当前处于APP模式下
static LH_ERR ServiceTaskFuncDebugIAP_CanMsg_Proc_ReadCurrentIapMode(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    uint8_t iapFlag = ServiceImplDebugIAP_ReadCurrentMode();
    //发送数据代码
    CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_IAP_FLAG,&iapFlag,1);
    //返回结果代码
    return LH_ERR_NONE;
}

//读取当前系统版本号
static LH_ERR ServiceTaskFuncDebugIAP_CanMsg_Proc_ReadCurrentSoftVersion(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    BOARD_BOOT_VERSION bootVersion;
    BOARD_APP_VERSION appVersion;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //申请内存
    uint8_t* dataBufferSend = NULL;
    do
    {
        dataBufferSend = UserMemMalloc(SRAM_CCM,7);
    } while (dataBufferSend == NULL);
    if(cachePtrLocal->currentLength == 4)
    {
        //旧版本模式,直接读取APP版本号
        BoardVersion_ReadAppVersion(&appVersion);
        //数据写入
        dataBufferSend[0] = appVersion.mainVersion;
        dataBufferSend[1] = appVersion.subVersion;
        dataBufferSend[2] = appVersion.debugVersion;
        CanBaseWriteUint32DataToBuffer(dataBufferSend,3,appVersion.svnVersion);
    }
    else
    {
        //新版本模式,可选返回APP版本号还是boot版本号
        if(cachePtrLocal->recvCacheArray[4] == 0)
        {
            //返回BOOT版本号
            BoardVersion_ReadBootVersion(&bootVersion);
            //数据写入
            dataBufferSend[0] = bootVersion.mainVersion;
            dataBufferSend[1] = bootVersion.subVersion;
            dataBufferSend[2] = bootVersion.debugVersion;
            CanBaseWriteUint32DataToBuffer(dataBufferSend,3,bootVersion.svnVersion);
        }
        else
        {
            //返回APP版本号
            BoardVersion_ReadAppVersion(&appVersion);
            //数据写入
            dataBufferSend[0] = appVersion.mainVersion;
            dataBufferSend[1] = appVersion.subVersion;
            dataBufferSend[2] = appVersion.debugVersion;
            CanBaseWriteUint32DataToBuffer(dataBufferSend,3,appVersion.svnVersion);
        }
    }
    //返回数据包
    CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_SOFTVERSION,dataBufferSend,7);
    //发送完成,释放内存
    UserMemFree(SRAM_CCM,dataBufferSend);
    //返回结果代码
    return LH_ERR_NONE;
}

//跳转到Bootloader中,并设置标志位自动跳转
static LH_ERR ServiceTaskFuncDebugIAP_CanMsg_Proc_JmpBootloader(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //存在,设置IAP标志
    BoardIAP_WriteFlag();
    //设置通道忙
    cachePtrLocal->currentState = CAN_SLAVE_CHANNEL_IDLE;
    CoreDelayMs(100);
    //跳转到Bootloader中
    ServiceImplDebugIAP_JmpBootloader();
    //返回结果代码
    return LH_ERR_NONE;
}

//设置当前运行模式
static LH_ERR ServiceTaskFuncDebugIAP_CanMsg_Proc_SetSystemMode(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //读取序号
    MODE_RUNNING mode = (MODE_RUNNING)(cachePtrLocal->recvCacheArray[4]);
    //设置模式
    SystemModeSet(mode);
    //返回结果代码
    return LH_ERR_NONE;
}

/************************************************自身分层API调用的消息的处理**********************************/
//模块自身转发消息处理任务,目前还不用实现IAP的自身回调
static void ServiceTaskFuncDebugIAP_Self_Proc(SYSTEM_CMD_SELF* selfCmd)
{
    return;
}
