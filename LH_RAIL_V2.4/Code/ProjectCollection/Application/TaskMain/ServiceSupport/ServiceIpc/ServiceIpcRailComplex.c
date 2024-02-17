/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:35:08
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-24 17:54:48
**FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailComplex.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcRailComplex.h"
#include "ServiceTaskConfig.h"

//指令列表
typedef enum IPC_CMD_COMPLEX
{
    IPC_CMD_COMPLEX_RESET_ALL             = 0X0000,//混合指令全部复位
    IPC_CMD_COMPLEX_SET_MODULE_COUNT      = 0X0001,//混合指令设置模块数量
    IPC_CMD_COMPLEX_PUSH_MULTI_WHILE_TRIG = 0X0002,//混合指令同时推进
    IPC_CMD_COMPLEX_SELF_ADJUST           = 0X0003,//自动校准
} IPC_CMD_COMPLEX;

//IPC调用的数据结构
static IPC_DATA_RAIL_COMPLEX dataIPC_RailComplex;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_RailComplex;

void IPC_RailComplexDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_RailComplex,
				  (CPU_CHAR*	)"mutexIPC_RailComplex",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_RailComplex.dataMutexPtr = &mutexIPC_RailComplex;
    dataIPC_RailComplex.state = STATE_IPC_IDLE;
    dataIPC_RailComplex.resultValue = LH_ERR_NONE;
    dataIPC_RailComplex.can1ErrorCode = CAN1_REPORT_OK;
    dataIPC_RailComplex.can1ReportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
}

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailComplexGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_RailComplex,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了,说明执行结束
    //读取结果代码
    *resultCode = dataIPC_RailComplex.resultValue;
    //读取中位机错误代码
    *can1ErrorPtr = dataIPC_RailComplex.can1ErrorCode;
    //读取中位机错误等级
    *reportLevelResultPtr = dataIPC_RailComplex.can1ReportErrorLevel;
    //释放信号量
    OSMutexPost(&mutexIPC_RailComplex,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_RailComplex.state;
}

//等待执行完成,阻塞
LH_ERR IPC_RailComplexWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_RailComplexGetLastCommandStateAndResult(&errorCode,can1ErrorPtr,reportLevelResultPtr);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            //延时指定时间
            CoreDelayMs(5);
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);
    //返回结果代码
    return errorCode;
}


/****************************************************发出指令不等待完成*******************************************************/
//动作API发送任务给动作API
static LH_ERR IPC_RailComplexSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_RailComplex,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_RailComplex.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_RailComplex.resultValue = LH_ERR_NONE;
    dataIPC_RailComplex.can1ErrorCode = CAN1_REPORT_OK;
    dataIPC_RailComplex.can1ReportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

    //申请系统消息内存
    while(systemTaskCmd == NULL)
    {
        systemTaskCmd = UserMemMalloc(MEM_DTCM,sizeof(SYSTEM_CMD));
        if(systemTaskCmd == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //只是指令是自身指令
    systemTaskCmd->cmdSrc = SYSTEM_CMD_SRC_SELF;
    //申请到内存了
    systemTaskCmd->cmdDataPtr = NULL;

    //一定要申请到
    while(systemTaskCmd->cmdDataPtr == NULL)
    {
        systemTaskCmd->cmdDataPtr = UserMemMalloc(MEM_DTCM,sizeof(SYSTEM_CMD_SELF));
        if(systemTaskCmd->cmdDataPtr == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //辅助指针
    SYSTEM_CMD_SELF* utilSelfCmdPtr = (SYSTEM_CMD_SELF*)(systemTaskCmd->cmdDataPtr);
    //消息序号
    utilSelfCmdPtr->commandIndex = (uint16_t)cmdIndex;
    //参数设置
    utilSelfCmdPtr->commandParam1 = param1;
    utilSelfCmdPtr->commandParam2 = param2;
    utilSelfCmdPtr->commandParam3 = param3;
    utilSelfCmdPtr->commandParam4 = param4;
    utilSelfCmdPtr->commandParam5 = param5;
    //设置不同信道特有的数据指针
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_RailComplex);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskServiceRailComplex, //向协议处理线程发送消息
                        (void *)systemTaskCmd,
                        (OS_MSG_SIZE)sizeof(SYSTEM_CMD*),
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,直接释放本次申请的内存
        UserMemFree(MEM_DTCM, (void *)systemTaskCmd->cmdDataPtr);
        UserMemFree(MEM_DTCM, (void *)systemTaskCmd);
        //指令没发出去,还是回到空闲状态
        dataIPC_RailComplex.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_RailComplex,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_RailComplex,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_RailComplex.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//电机全部复位,不等待返回
LH_ERR IPC_RailComplexResetAllWhileAck(void)
{
    return IPC_RailComplexSendCommandWhileAck(IPC_CMD_COMPLEX_RESET_ALL,0,0,0,0,0);
}

//设置模块数量,不等待返回
LH_ERR IPC_RailComplexSetModuleCountWhileAck(uint8_t setCount)
{
    return IPC_RailComplexSendCommandWhileAck(IPC_CMD_COMPLEX_SET_MODULE_COUNT,(int32_t)setCount,0,0,0,0);
}

//进样1 2同时推进一次,不等待返回
LH_ERR IPC_RailComplexPushMultiWhileTrigWhileAck(void)
{
    return IPC_RailComplexSendCommandWhileAck(IPC_CMD_COMPLEX_PUSH_MULTI_WHILE_TRIG,0,0,0,0,0);
}

//自动校准
LH_ERR IPC_RailComplexSelfAdjustWhileAck(void)
{
    return IPC_RailComplexSendCommandWhileAck(IPC_CMD_COMPLEX_SELF_ADJUST,0,0,0,0,0);
}


/****************************************************发出指令并等待完成********************************************************/
//电机全部复位,等待返回
LH_ERR IPC_RailComplexResetAllWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailComplexSendCommandWhileAck(IPC_CMD_COMPLEX_RESET_ALL,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailComplexWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//设置模块数量,等待返回
LH_ERR IPC_RailComplexSetModuleCountWhileReturn(uint8_t setCount,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailComplexSendCommandWhileAck(IPC_CMD_COMPLEX_SET_MODULE_COUNT,(int32_t)setCount,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailComplexWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//进样1 2同时推进一次,等待返回
LH_ERR IPC_RailComplexPushMultiWhileTrigWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailComplexSendCommandWhileAck(IPC_CMD_COMPLEX_PUSH_MULTI_WHILE_TRIG,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailComplexWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//自动校准
LH_ERR IPC_RailComplexSelfAdjustWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailComplexSendCommandWhileAck(IPC_CMD_COMPLEX_SELF_ADJUST,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailComplexWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}
