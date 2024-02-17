/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-20 16:30:00
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-24 20:10:37
**FilePath: \App\TaskMain\AppSupport\AppIpc\AppIpcStateParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcStateParam.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_STATE_PARAM
{
    IPC_CMD_STATE_PARAM_SET_MODULE          = 0X0000,//设置模块数量
    IPC_CMD_STATE_PARAM_EMERGENCY_STOP      = 0X0001,//急停
}IPC_CMD_STATE_PARAM;


//IPC调用的数据结构
static IPC_DATA_STATE_PARAM dataIPC_StateParam;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_StateParam;

void IPC_StateParamDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_StateParam,
				  (CPU_CHAR*	)"mutexIPC_StateParam",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_StateParam.dataMutexPtr = &mutexIPC_StateParam;
    dataIPC_StateParam.state = STATE_IPC_IDLE;
    dataIPC_StateParam.resultValue = LH_ERR_NONE;
    dataIPC_StateParam.can1ErrorCode = CAN1_REPORT_OK;
    dataIPC_StateParam.can1ReportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
}

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_StateParamGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_StateParam,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了,说明执行结束
    //读取结果代码
    *resultCode = dataIPC_StateParam.resultValue;
    //读取中位机错误代码
    *can1ErrorPtr = dataIPC_StateParam.can1ErrorCode;
    //读取中位机错误等级
    *reportLevelResultPtr = dataIPC_StateParam.can1ReportErrorLevel;
    //释放信号量
    OSMutexPost(&mutexIPC_StateParam,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_StateParam.state;
}

//等待执行完成,阻塞
LH_ERR IPC_StateParamWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_StateParamGetLastCommandStateAndResult(&errorCode,can1ErrorPtr,reportLevelResultPtr);
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
static LH_ERR IPC_StateParamSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_StateParam,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_StateParam.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_StateParam.resultValue = LH_ERR_NONE;
    dataIPC_StateParam.can1ErrorCode = CAN1_REPORT_OK;
    dataIPC_StateParam.can1ReportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_StateParam);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskAppStateParam, //向协议处理线程发送消息
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
        dataIPC_StateParam.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_StateParam,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_StateParam,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_StateParam.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}

/****************************************************发出指令不等待完成*******************************************************/
//设置模块数量,不等待完成
LH_ERR IPC_StateParamSetModuleCountWhileAck(uint8_t moduleCount)
{
    return IPC_StateParamSendCommandWhileAck(IPC_CMD_STATE_PARAM_SET_MODULE,(int32_t)moduleCount,0,0,0,0);
}

//急停,不等待完成
LH_ERR IPC_StateParamEmergencyStopWhileAck(void)
{
    return IPC_StateParamSendCommandWhileAck(IPC_CMD_STATE_PARAM_EMERGENCY_STOP,0,0,0,0,0);
}



/****************************************************发出指令并等待完成********************************************************/
//设置模块数量,等待完成
LH_ERR IPC_StateParamSetModuleCountWhileReturn(uint8_t moduleCount,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_StateParamSendCommandWhileAck(IPC_CMD_STATE_PARAM_SET_MODULE,(int32_t)moduleCount,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_StateParamWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//急停,等待完成
LH_ERR IPC_StateParamEmergencyStopWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_StateParamSendCommandWhileAck(IPC_CMD_STATE_PARAM_EMERGENCY_STOP,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_StateParamWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}














