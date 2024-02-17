/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:36:22
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-14 16:39:33
**FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppIpc\AppIpcHandNewCup.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcHandNewCup.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_HAND_NEW_CUP
{
    IPC_CMD_HAND_NEW_CUP_RESET                      = 0x00000000,//新杯机械手复位
    IPC_CMD_HAND_NEW_CUP_MOVE_TO_NEW_CUP_TRAY       = 0x00000001,//新杯机械手移动到新杯区
    IPC_CMD_HAND_NEW_CUP_MOVE_TO_PLATE_REACTION     = 0X00000002,//新杯机械手移动到反应盘
    IPC_CMD_HAND_NEW_CUP_MOVE_TO_GARBAGE            = 0X00000003,//新杯机械手移动到垃圾桶
    IPC_CMD_HAND_NEW_CUP_CATCH_CUP_NEW_CUP_TRAY     = 0X00000004,//新杯机械手新杯区取杯
    IPC_CMD_HAND_NEW_CUP_CATCH_CUP_PLATE_REACTION   = 0X00000005,//新杯机械手反应盘取杯    
    IPC_CMD_HAND_NEW_CUP_PUT_CUP_PLATE_REACTION     = 0X00000006,//新杯机械手反应盘放杯
    IPC_CMD_HAND_NEW_CUP_PUT_CUP_GARBAGE            = 0x00000007,//新杯机械手垃圾桶放杯
}IPC_CMD_HAND_NEW_CUP;

//IPC调用的数据结构
static DATA_IPC_HAND_NEW_CUP dataIPC_HandNewCup;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_HandNewCup;


//初始化IPC调用结构
void IPC_HandNewCupDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_HandNewCup,
				  (CPU_CHAR*	)"mutexIPC_HandNewCup",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_HandNewCup.dataMutexPtr = &mutexIPC_HandNewCup;
    dataIPC_HandNewCup.state = STATE_IPC_IDLE;
    dataIPC_HandNewCup.resultValue = LH_ERR_NONE;
}

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_HandNewCupGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_HandNewCup,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_HandNewCup.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_HandNewCup,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_HandNewCup.state;
}

//等待执行完成,阻塞
LH_ERR IPC_HandNewCupWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_HandNewCupGetLastCommandStatusAndResult(&errorCode);
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
static LH_ERR IPC_HandNewCupSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_HandNewCup,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_HandNewCup.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_HandNewCup.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_HandNewCup);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskHandNewCup, //向协议处理线程发送消息
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
        dataIPC_HandNewCup.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_HandNewCup,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_HandNewCup,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_HandNewCup.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}



/****************************************************发出指令不等待完成*******************************************************/
//新杯机械手复位,不等待完成
LH_ERR IPC_HandNewCupResetWhileAck(void)
{
    return IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯机械手移动到新杯区,不等待完成
LH_ERR IPC_HandNewCupMoveToNewCupTrayCleanWhileAck(uint8_t lineBackFront,uint8_t colLeftRight)
{
    return IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_TO_NEW_CUP_TRAY,((int32_t)lineBackFront),((int32_t)colLeftRight),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯机械手移动到反应盘,不等待完成
LH_ERR IPC_HandNewCupMoveToPlateReactionnWhileAck(void)
{
    return IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_TO_PLATE_REACTION,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯机械手移动到垃圾桶,不等待完成
LH_ERR IPC_HandNewCupMoveToGarbageWhileAck(HAND_NEW_CUP_GARBAGE_NO garbageNo)
{
    return IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_TO_GARBAGE,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯机械手新杯区取杯,不等待完成
LH_ERR IPC_HandNewCupCatchCupNewCupTrayWhileAck(void)
{
    return IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_CATCH_CUP_NEW_CUP_TRAY,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯机械手反应盘取杯,不等待完成
LH_ERR IPC_HandNewCupCatchCupPlateReactionWhileAck(void)
{
    return IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_CATCH_CUP_PLATE_REACTION,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯机械手反应盘放杯,不等待完成
LH_ERR IPC_HandNewCupPutCupPlateReactionWhileAck(void)
{
    return IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_PUT_CUP_PLATE_REACTION,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯机械手垃圾桶放杯,不等待完成
LH_ERR IPC_HandNewCupPutCupGarbageWhileAck(HAND_NEW_CUP_GARBAGE_NO garbageNo)
{
    return IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_PUT_CUP_GARBAGE,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}


/****************************************************发出指令并等待完成********************************************************/
//新杯机械手复位,等待完成
LH_ERR IPC_HandNewCupResetWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_RESET,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandNewCupWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手移动到新杯区,等待完成
LH_ERR IPC_HandNewCupMoveToNewCupTrayWhileReturn(uint8_t lineBackFront,uint8_t colLeftRight)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_TO_NEW_CUP_TRAY,((int32_t)lineBackFront),((int32_t)colLeftRight),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandNewCupWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手移动到反应盘,等待完成
LH_ERR IPC_HandNewCupMoveToPlateReactionWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_TO_PLATE_REACTION,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandNewCupWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手移动到垃圾桶,等待完成
LH_ERR IPC_HandNewCupMoveToGarbageWhileReturn(HAND_NEW_CUP_GARBAGE_NO garbageNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_TO_GARBAGE,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandNewCupWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手新杯区取杯,等待完成
LH_ERR IPC_HandNewCupCatchCupNewCupTrayWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_CATCH_CUP_NEW_CUP_TRAY,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandNewCupWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手反应盘取杯,等待完成
LH_ERR IPC_HandNewCupCatchCupPlateReactionWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_CATCH_CUP_PLATE_REACTION,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandNewCupWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手反应盘放杯,等待完成
LH_ERR IPC_HandNewCupPutCupPlateReactionWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_PUT_CUP_PLATE_REACTION,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandNewCupWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手垃圾桶放杯,等待完成
LH_ERR IPC_HandNewCupPutCupGarbageWhileReturn(HAND_NEW_CUP_GARBAGE_NO garbageNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_PUT_CUP_GARBAGE,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandNewCupWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}



