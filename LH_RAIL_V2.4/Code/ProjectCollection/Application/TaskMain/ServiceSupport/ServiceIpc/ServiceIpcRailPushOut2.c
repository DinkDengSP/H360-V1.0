/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:33:28
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-24 17:30:21
**FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailPushOut2.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcRailPushOut2.h"
#include "ServiceTaskConfig.h"

//指令列表
typedef enum IPC_CMD_PUSH_OUT2
{
    IPC_CMD_PUSH_OUT2_RESET                         = 0X0000,   //出样1复位
    IPC_CMD_PUSH_OUT2_PUSH_ONCE_WITH_RETURN_ZERO    = 0X0001,   //出样1推进一次并回零
    IPC_CMD_PUSH_OUT2_PUSH_ONCE_WITHOUT_RETURN_ZERO = 0X0002,   //出样1推进一次不回零
    IPC_CMD_PUSH_OUT2_RETURN_ZERO                   = 0X0003,   //出样1回零
    IPC_CMD_PUSH_OUT2_GET_PUSH_INDEX                = 0X0004,   //出样1获取推进序号
    IPC_CMD_PUSH_OUT2_PUSH_MAX                      = 0X0005,   //出样1推样本架到出口端
    IPC_CMD_PUSH_OUT2_GET_SENSOR_STATE              = 0X0006,   //出样1获取传感器状态
    IPC_CMD_PUSH_OUT2_SET_STATE                     = 0X0007    //出样2状态控制
} IPC_CMD_PUSH_OUT2;

//IPC调用的数据结构
static IPC_DATA_RAIL_PUSH_OUT2 dataIPC_RailPushOut2;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_RailPushOut2;

void IPC_RailPushOut2DataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_RailPushOut2,
				  (CPU_CHAR*	)"mutexIPC_RailPushOut2",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_RailPushOut2.dataMutexPtr = &mutexIPC_RailPushOut2;
    dataIPC_RailPushOut2.state = STATE_IPC_IDLE;
    dataIPC_RailPushOut2.resultValue = LH_ERR_NONE;
    dataIPC_RailPushOut2.can1ErrorCode = CAN1_REPORT_OK;
    dataIPC_RailPushOut2.can1ReportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
}

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailPushOut2GetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_RailPushOut2,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了,说明执行结束
    //读取结果代码
    *resultCode = dataIPC_RailPushOut2.resultValue;
    //读取中位机错误代码
    *can1ErrorPtr = dataIPC_RailPushOut2.can1ErrorCode;
    //读取中位机错误等级
    *reportLevelResultPtr = dataIPC_RailPushOut2.can1ReportErrorLevel;
    //释放信号量
    OSMutexPost(&mutexIPC_RailPushOut2,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_RailPushOut2.state;
}

//等待执行完成,阻塞
LH_ERR IPC_RailPushOut2WaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_RailPushOut2GetLastCommandStateAndResult(&errorCode,can1ErrorPtr,reportLevelResultPtr);
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
static LH_ERR IPC_RailPushOut2SendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_RailPushOut2,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_RailPushOut2.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_RailPushOut2.resultValue = LH_ERR_NONE;
    dataIPC_RailPushOut2.can1ErrorCode = CAN1_REPORT_OK;
    dataIPC_RailPushOut2.can1ReportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_RailPushOut2);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskServiceRailPushOut2, //向协议处理线程发送消息
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
        dataIPC_RailPushOut2.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_RailPushOut2,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_RailPushOut2,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_RailPushOut2.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//出样2复位
LH_ERR IPC_RailPushOut2ResetWhileAck(RAIL_RESET_CORRECT_FLAG correctFlag)
{
    return IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_RESET,(int32_t)correctFlag,0,0,0,0);
}

//出样2推进一次架子并回零
LH_ERR IPC_RailPushOut2PushOnceWithReturnZeroWhileAck(void)
{
    return IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_PUSH_ONCE_WITH_RETURN_ZERO,0,0,0,0,0);
}

//出样2推进一次架子,但是不回零
LH_ERR IPC_RailPushOut2PushOnceNotReturnZeroWhileAck(void)
{
    return IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_PUSH_ONCE_WITHOUT_RETURN_ZERO,0,0,0,0,0);
}

//出样2回零
LH_ERR IPC_RailPushOut2ReturnZeroWhileAck(void)
{
    return IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_RETURN_ZERO,0,0,0,0,0);
}

//获取当前出样2所处的位置
LH_ERR IPC_RailPushOut2GetIndexWhileAck(void)
{
    return IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_GET_PUSH_INDEX,0,0,0,0,0);
}

//出样将试管架推到顶部
LH_ERR IPC_RailPushOut2PushRackTopWhileAck(void)
{
    return IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_PUSH_MAX,0,0,0,0,0);
}

//获取出样传感器状态
LH_ERR IPC_RailPushOut2GetSensorStateWhileAck(void)
{
    return IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_GET_SENSOR_STATE,0,0,0,0,0);
}
//出样2状态控制
LH_ERR IPC_RailPushOut2SetStateWhileAck(RAIL_OUTPUT_STATE railState)
{
    return IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_SET_STATE,railState,0,0,0,0);
}

/****************************************************发出指令并等待完成********************************************************/
//出样2复位,等待完成
LH_ERR IPC_RailPushOut2ResetWhileReturn(RAIL_RESET_CORRECT_FLAG correctFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_RESET,(int32_t)correctFlag,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailPushOut2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//出样2推进一次架子并回零,等待完成
LH_ERR IPC_RailPushOut2PushOnceWithReturnZeroWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_PUSH_ONCE_WITH_RETURN_ZERO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailPushOut2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//出样2推进一次架子,但是不回零,等待完成
LH_ERR IPC_RailPushOut2PushOnceNotReturnZeroWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_PUSH_ONCE_WITHOUT_RETURN_ZERO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailPushOut2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//出样2回零,等待完成
LH_ERR IPC_RailPushOut2ReturnZeroWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_RETURN_ZERO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailPushOut2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//获取当前出样2所处的位置
LH_ERR IPC_RailPushOut2GetIndexWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_GET_PUSH_INDEX,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailPushOut2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//出样将试管架推到顶部
LH_ERR IPC_RailPushOut2PushRackTopWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_PUSH_MAX,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailPushOut2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//获取出样传感器状态
LH_ERR IPC_RailPushOut2GetSensorStateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_GET_SENSOR_STATE,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailPushOut2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//出样2状态控制
LH_ERR IPC_RailPushOut2SetStateWhileReturn(RAIL_OUTPUT_STATE railState,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailPushOut2SendCommandWhileAck(IPC_CMD_PUSH_OUT2_SET_STATE,railState,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailPushOut2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

