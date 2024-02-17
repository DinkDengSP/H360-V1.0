/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-14 15:23:44
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcRailComplex.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcRailComplex.h"
#include "AppTaskConfig.h"

//IPC调用的数据结构
static DATA_IPC_RAIL_COMPLEX dataIPC_RailComplex;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_RailComplex;

//指令列表
typedef enum IPC_CMD_RAIL_COMPLEX
{
    IPC_CMD_RAIL_COMPLEX_RESET_ALL        = 0X0000,
    IPC_CMD_RAIL_COMPLEX_SET_MODULE_COUNT = 0X0001,
    IPC_CMD_RAIL_COMPLEX_PUSH_TOGETHER    = 0X0002,
}IPC_CMD_RAIL_COMPLEX;


/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
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
    //变量赋值
    dataIPC_RailComplex.currentModuleCount = 1;
    dataIPC_RailComplex.pushIn1State = SENSOR_STATE_PUSHIN_EXIST_NONE;
    dataIPC_RailComplex.pushIn2State = SENSOR_STATE_PUSHIN_EXIST_NONE;
}

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailComplexGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_RailComplex,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_RailComplex.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_RailComplex,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_RailComplex.state;   
}

//等待执行完成,阻塞
LH_ERR IPC_RailComplexWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_RailComplexGetLastCommandStatusAndResult(&errorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            CoreDelayMinTick();
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

/***************************************************内部向任务发送指令的API*****************************************************/
static LH_ERR IPC_RailComplexSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
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

    //申请系统消息内存
    while(systemTaskCmd == NULL)
    {
        systemTaskCmd = UserMemMalloc(SRAM_CCM,sizeof(TASK_CMD));
        if(systemTaskCmd == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //只是指令是自身指令
    systemTaskCmd->cmdSrc = TASK_CMD_SRC_SELF;
    //申请到内存了
    systemTaskCmd->cmdData = NULL;

    //一定要申请到
    while(systemTaskCmd->cmdData == NULL)
    {
        systemTaskCmd->cmdData = UserMemMalloc(SRAM_CCM,sizeof(SYSTEM_CMD_SELF));
        if(systemTaskCmd->cmdData == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //辅助指针
    SYSTEM_CMD_SELF* utilSelfCmdPtr = (SYSTEM_CMD_SELF*)(systemTaskCmd->cmdData);
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
    OSTaskQPost((OS_TCB *)&tcbAppTaskRailComplex, //向协议处理线程发送消息
                        (void *)systemTaskCmd,
                        (OS_MSG_SIZE)sizeof(TASK_CMD*),
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,直接释放本次申请的内存
        UserMemFree(SRAM_CCM, (void *)systemTaskCmd->cmdData);
        UserMemFree(SRAM_CCM, (void *)systemTaskCmd);
        //指令没发出去,还是回到空闲状态
        dataIPC_RailComplex.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_RailComplex,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_RailComplex,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_RailComplex.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}

/****************************************************发出指令不等待完成*******************************************************/
//轨道整体复位
LH_ERR IPC_RailComplexResetAllWhileAck(void)
{
    return IPC_RailComplexSendCommandWhileAck(IPC_CMD_RAIL_COMPLEX_RESET_ALL,0,0,0,0,0);
}

//设置轨道模块数量
LH_ERR IPC_RailComplexSetModuleCountWhileAck(uint8_t moduleCount)
{
    return IPC_RailComplexSendCommandWhileAck(IPC_CMD_RAIL_COMPLEX_SET_MODULE_COUNT,((int32_t)moduleCount),0,0,0,0);
}

//双轨道同时进架多次直到触发
LH_ERR IPC_RailComplexPushInTrigMultiTogetherWhileAck(uint8_t retryMax)
{
    return IPC_RailComplexSendCommandWhileAck(IPC_CMD_RAIL_COMPLEX_PUSH_TOGETHER,((int32_t)retryMax),0,0,0,0);
}


/*****************************************************发出指令等待完成*******************************************************/
//轨道整体复位
LH_ERR IPC_RailComplexResetAllWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailComplexSendCommandWhileAck(IPC_CMD_RAIL_COMPLEX_RESET_ALL,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailComplexWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//设置轨道模块数量
LH_ERR IPC_RailComplexSetModuleCountWhileReturn(uint8_t moduleCount)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailComplexSendCommandWhileAck(IPC_CMD_RAIL_COMPLEX_SET_MODULE_COUNT,((int32_t)moduleCount),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailComplexWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//双轨道同时进架多次直到触发
LH_ERR IPC_RailComplexPushInTrigMultiTogetherWhileReturn(uint8_t retryMax,SENSOR_STATE_PUSHIN_EXIST* pushin1SensorPtr,SENSOR_STATE_PUSHIN_EXIST* pushin2SensorPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailComplexSendCommandWhileAck(IPC_CMD_RAIL_COMPLEX_PUSH_TOGETHER,((int32_t)retryMax),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailComplexWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //数据结果获取
    *pushin1SensorPtr = dataIPC_RailComplex.pushIn1State;
    *pushin2SensorPtr = dataIPC_RailComplex.pushIn2State;
    //返回结果代码
    return LH_ERR_NONE;
}


/****************************************************获取运行的有效数据*********************************************************/
//查询上次执行结果,进样1和进样2的状态
void IPC_RailComplexGetPushInState(SENSOR_STATE_PUSHIN_EXIST* pushin1SensorPtr,SENSOR_STATE_PUSHIN_EXIST* pushin2SensorPtr)
{
    *pushin1SensorPtr = dataIPC_RailComplex.pushIn1State;
    *pushin2SensorPtr = dataIPC_RailComplex.pushIn2State;
}










