/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:36:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-29 11:58:48
**FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppIpc\AppIpcSampleRackTrack.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcSampleRackTrack.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_SAMPLE_RACK_TRACK
{
    IPC_CMD_SAMPLE_RACK_TRACK_RESET                     = 0x00000000,//试管架传送带复位
    IPC_CMD_SAMPLE_RACK_TRACK_MOVE_TO_TUBE              = 0x00000001,//试管架传送带移动到指定试管位
    IPC_CMD_SAMPLE_RACK_TRACK_BACK_FULL_SAMPLE_RACK     = 0X00000002,//试管架传送带回退一整个试管架
    IPC_CMD_SAMPLE_RACK_TRACK_ENCODER_CORRECT           = 0X00000003,//试管架传送带齿差修正
}IPC_CMD_SAMPLE_RACK_TRACK;

//IPC调用的数据结构
static DATA_IPC_SAMPLE_RACK_TRACK dataIPC_SampleRackTrack;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_SampleRackTrack;

//初始化IPC调用结构
void IPC_SampleRackTrackDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_SampleRackTrack,
				  (CPU_CHAR*	)"mutexIPC_SampleRackTrack",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_SampleRackTrack.dataMutexPtr = &mutexIPC_SampleRackTrack;
    dataIPC_SampleRackTrack.state = STATE_IPC_IDLE;
    dataIPC_SampleRackTrack.resultValue = LH_ERR_NONE;
}

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_SampleRackTrackGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_SampleRackTrack,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_SampleRackTrack.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_SampleRackTrack,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_SampleRackTrack.state;
}

//等待执行完成,阻塞
LH_ERR IPC_SampleRackTrackWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_SampleRackTrackGetLastCommandStatusAndResult(&errorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            CoreDelayMinTick();
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

/****************************************************内部向任务发送消息的API*******************************************************/
//动作API发送任务给动作API
static LH_ERR IPC_SampleRackTrackSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_SampleRackTrack,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_SampleRackTrack.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_SampleRackTrack.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_SampleRackTrack);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskSampleRackTrack, //向协议处理线程发送消息
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
        dataIPC_SampleRackTrack.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_SampleRackTrack,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_SampleRackTrack,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_SampleRackTrack.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//加样传送带复位,带零位偏移,不等待完成
LH_ERR IPC_SampleRackTrackResetWhileAck(void)
{
    return IPC_SampleRackTrackSendCommandWhileAck(IPC_CMD_SAMPLE_RACK_TRACK_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//加样传送带移动到指定试管位,不等待完成
LH_ERR IPC_SampleRackTrackMove2TubeWhileAck(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex)
{
    return IPC_SampleRackTrackSendCommandWhileAck(IPC_CMD_SAMPLE_RACK_TRACK_MOVE_TO_TUBE,((int32_t)rackPos),((int32_t)tubeIndex),((int32_t)0),((int32_t)0),((int32_t)0));
}

//加样传送带回退一整个试管架,不等待完成
LH_ERR IPC_SampleRackTrackBackFullSampleRackWhileAck(void)
{
    return IPC_SampleRackTrackSendCommandWhileAck(IPC_CMD_SAMPLE_RACK_TRACK_BACK_FULL_SAMPLE_RACK,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//加样传送带码盘齿差修正,不等待完成
LH_ERR IPC_SampleRackTrackEncoderCorrectWhileAck(uint8_t correctCount)
{
    return IPC_SampleRackTrackSendCommandWhileAck(IPC_CMD_SAMPLE_RACK_TRACK_ENCODER_CORRECT,((int32_t)correctCount),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

/****************************************************发出指令并等待完成********************************************************/
//加样传送带复位,带零位偏移,等待完成
LH_ERR IPC_SampleRackTrackResetWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_SampleRackTrackSendCommandWhileAck(IPC_CMD_SAMPLE_RACK_TRACK_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_SampleRackTrackWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//加样传送带移动到指定试管位,等待完成
LH_ERR IPC_SampleRackTrackMove2TubeWhileReturn(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_SampleRackTrackSendCommandWhileAck(IPC_CMD_SAMPLE_RACK_TRACK_MOVE_TO_TUBE,((int32_t)rackPos),((int32_t)tubeIndex),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_SampleRackTrackWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//加样传送带回退一整个试管架,等待完成
LH_ERR IPC_SampleRackTrackBackFullSampleRackWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_SampleRackTrackSendCommandWhileAck(IPC_CMD_SAMPLE_RACK_TRACK_BACK_FULL_SAMPLE_RACK,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_SampleRackTrackWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//加样传送带码盘齿差修正,等待完成
LH_ERR IPC_SampleRackTrackEncoderCorrectWhileReturn(uint8_t correctCount)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_SampleRackTrackSendCommandWhileAck(IPC_CMD_SAMPLE_RACK_TRACK_ENCODER_CORRECT,((int32_t)correctCount),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_SampleRackTrackWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}



