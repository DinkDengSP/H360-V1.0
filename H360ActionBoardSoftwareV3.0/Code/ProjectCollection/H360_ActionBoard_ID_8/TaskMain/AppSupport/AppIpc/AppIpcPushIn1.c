/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-14 15:23:13
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcPushIn1.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcPushIn1.h"
#include "AppTaskConfig.h"

//IPC调用的数据结构
static DATA_IPC_PUSH_IN1 dataIPC_PushIn1;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_PushIn1;

//指令列表
typedef enum IPC_CMD_PUSH_IN1
{
    IPC_CMD_PUSH_IN1_RESET = 0X0000,
    IPC_CMD_PUSH_IN1_PUSH_ONCE = 0X0001,
    IPC_CMD_PUSH_IN1_PUSH_MULTI = 0X0002,
}IPC_CMD_PUSH_IN1;


/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_PushIn1DataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_PushIn1,
				  (CPU_CHAR*	)"mutexIPC_PushIn1",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_PushIn1.dataMutexPtr = &mutexIPC_PushIn1;
    dataIPC_PushIn1.state = STATE_IPC_IDLE;
    dataIPC_PushIn1.resultValue = LH_ERR_NONE;
    //进样传感器
    dataIPC_PushIn1.pushInState = SENSOR_STATE_PUSHIN_EXIST_NONE;
}


/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_PushIn1GetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_PushIn1,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_PushIn1.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_PushIn1,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_PushIn1.state;
}

//等待执行完成,阻塞
LH_ERR IPC_PushIn1WaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_PushIn1GetLastCommandStatusAndResult(&errorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            CoreDelayMinTick();
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

/*************************************************内置向指定任务发送消息API****************************************************/

//动作API发送任务给动作API
static LH_ERR IPC_PushIn1SendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_PushIn1,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_PushIn1.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_PushIn1.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_PushIn1);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskPushIn1, //向协议处理线程发送消息
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
        dataIPC_PushIn1.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_PushIn1,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_PushIn1,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_PushIn1.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//进样1复位,不等待完成
LH_ERR IPC_PushIn1ResetWhileAck(RAIL_RESET_CORRECT_FLAG correctFlag)
{
    return IPC_PushIn1SendCommandWhileAck(IPC_CMD_PUSH_IN1_RESET,((int32_t)correctFlag),0,0,0,0);
}

//进样1推进一次,不等待完成
LH_ERR IPC_PushIn1PushOnceWhileAck(void)
{
    return IPC_PushIn1SendCommandWhileAck(IPC_CMD_PUSH_IN1_PUSH_ONCE,0,0,0,0,0);
}

//进样1推进多次直到传感器触发,不等待完成
LH_ERR IPC_PushIn1PushMultiWhileSensorTrigWhileAck(uint16_t retryMax)
{
    return IPC_PushIn1SendCommandWhileAck(IPC_CMD_PUSH_IN1_PUSH_MULTI,((int32_t)retryMax),0,0,0,0);
}


/****************************************************发出指令并等待完成********************************************************/
//进样1复位,等待完成
LH_ERR IPC_PushIn1ResetWhileReturn(RAIL_RESET_CORRECT_FLAG correctFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
    errorCode = IPC_PushIn1SendCommandWhileAck(IPC_CMD_PUSH_IN1_RESET,((int32_t)correctFlag),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_PushIn1WaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//进样1推进一次,等待完成
LH_ERR IPC_PushIn1PushOnceWhileReturn(SENSOR_STATE_PUSHIN_EXIST* rackExistSensorStatePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
    errorCode = IPC_PushIn1SendCommandWhileAck(IPC_CMD_PUSH_IN1_PUSH_ONCE,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_PushIn1WaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //获取返回数据
    *rackExistSensorStatePtr = dataIPC_PushIn1.pushInState;
    //返回结果代码
    return LH_ERR_NONE;
}

//进样1推进多次直到传感器触发,等待完成
LH_ERR IPC_PushIn1PushMultiWhileSensorTrigWhileReturn(uint16_t retryMax,SENSOR_STATE_PUSHIN_EXIST* rackExistSensorStatePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
    errorCode = IPC_PushIn1SendCommandWhileAck(IPC_CMD_PUSH_IN1_PUSH_MULTI,((int32_t)retryMax),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_PushIn1WaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //获取返回数据
    *rackExistSensorStatePtr = dataIPC_PushIn1.pushInState;
    //返回结果代码
    return LH_ERR_NONE;
}


/****************************************************获取运行的有效数据*********************************************************/
//读取上次动作返回的传感器状态
SENSOR_STATE_PUSHIN_EXIST IPC_PushIn1ReadCurrentRackExistState(void)
{
    return dataIPC_PushIn1.pushInState;
}
