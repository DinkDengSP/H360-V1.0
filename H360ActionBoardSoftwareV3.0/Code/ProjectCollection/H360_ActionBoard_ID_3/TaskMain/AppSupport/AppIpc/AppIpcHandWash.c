/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:40:58
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-07 12:18:27
**FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppIpc\AppIpcHandWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcHandWash.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_HAND_WASH
{
    IPC_CMD_HAND_WASH_RESET                         = 0x00,//清洗机械手复位          
    IPC_CMD_HAND_WASH_ROTATE_TO_PLATE_REACTION      = 0X01,//清洗机械手旋转到反应盘 
    IPC_CMD_HAND_WASH_ROTATE_TO_PLATE_WASH          = 0X02,//清洗机械手旋转到清洗盘    
    IPC_CMD_HAND_WASH_ROTATE_TO_MEASURE_UNIT        = 0X03,//清洗机械手旋转到测量模块  
    IPC_CMD_HAND_WASH_CATCH_CUP_ON_PLATE_REACTION   = 0X04,//清洗机械手反应盘取杯    
    IPC_CMD_HAND_WASH_PUT_CUP_ON_PLATE_REACTION     = 0X05,//清洗机械手反应盘放杯     
    IPC_CMD_HAND_WASH_CATCH_CUP_ON_PLATE_WASH       = 0X06,//清洗机械手清洗盘取杯       
    IPC_CMD_HAND_WASH_PUT_CUP_ON_PLATE_WASH         = 0X07,//清洗机械手清洗盘放杯         
    IPC_CMD_HAND_WASH_CATCH_CUP_ON_MEASURE_UNIT     = 0X08,//清洗机械手测量模块取杯
    IPC_CMD_HAND_WASH_PUT_CUP_ON_MEASURE_UNIT       = 0X09,//清洗机械手测量模块放杯          
}IPC_CMD_HAND_WASH;

//IPC调用的数据结构
static DATA_IPC_HAND_WASH dataIPC_HandWash;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_HandWash;

//初始化IPC调用结构
void IPC_HandWashDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_HandWash,
				  (CPU_CHAR*	)"mutexIPC_HandWash",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_HandWash.dataMutexPtr = &mutexIPC_HandWash;
    dataIPC_HandWash.state = STATE_IPC_IDLE;
    dataIPC_HandWash.resultValue = LH_ERR_NONE;
}


/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_HandWashGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_HandWash,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_HandWash.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_HandWash,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_HandWash.state;
}

//等待执行完成,阻塞
LH_ERR IPC_HandWashWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_HandWashGetLastCommandStatusAndResult(&errorCode);
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
static LH_ERR IPC_HandWashSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_HandWash,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_HandWash.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_HandWash.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_HandWash);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskHandWash, //向协议处理线程发送消息
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
        dataIPC_HandWash.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_HandWash,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_HandWash,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_HandWash.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}

/****************************************************发出指令不等待完成*******************************************************/
//清洗机械手复位
LH_ERR IPC_HandWashResetRotateWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_RESET,((int32_t)0),0,0,0,0);
}

//清洗机械手旋转到反应盘
LH_ERR IPC_HandWashRotateToPlateReactionWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_TO_PLATE_REACTION,0,0,0,0,0);
}

//清洗机械手旋转到清洗盘
LH_ERR IPC_HandWashRotateToPlateWashWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_TO_PLATE_WASH,0,0,0,0,0);
}

//清洗机械手旋转到测量模块
LH_ERR IPC_HandWashRotateToMeasureUnitWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_TO_MEASURE_UNIT,0,0,0,0,0);
}

//清洗机械手反应盘取杯
LH_ERR IPC_HandWashCatchCupOnPlateReactionWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_CUP_ON_PLATE_REACTION,0,0,0,0,0);
}

//清洗机械手反应盘放杯
LH_ERR IPC_HandWashPutCupOnPlateReactionWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_CUP_ON_PLATE_REACTION,0,0,0,0,0);
}

//清洗机械手清洗盘取杯
LH_ERR IPC_HandWashCatchCupOnPlateWashWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_CUP_ON_PLATE_WASH,0,0,0,0,0);
}

//清洗机械手清洗盘放杯
LH_ERR IPC_HandWashPutCupOnPlateWashWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_CUP_ON_PLATE_WASH,0,0,0,0,0);
}

//清洗机械手测量模块抓杯
LH_ERR IPC_HandWashCatchCupOnMeasureUnitWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_CUP_ON_MEASURE_UNIT,0,0,0,0,0);
}

//清洗机械手测量模块放杯
LH_ERR IPC_HandWashPutCupOnMeasureUnitWhileAck(void)
{
    return IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_CUP_ON_MEASURE_UNIT,0,0,0,0,0); 
}

/****************************************************发出指令并等待完成********************************************************/
//清洗机械手复位
LH_ERR IPC_HandWashResetRotateWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_RESET,((int32_t)0),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手旋转到反应盘
LH_ERR IPC_HandWashRotateToPlateReactionWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_TO_PLATE_REACTION,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手旋转到清洗盘
LH_ERR IPC_HandWashRotateToPlateWashWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_TO_PLATE_WASH,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手旋转到测量模块
LH_ERR IPC_HandWashRotateToMeasureUnitWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_TO_MEASURE_UNIT,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手反应盘取杯
LH_ERR IPC_HandWashCatchCupOnPlateReactionWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_CUP_ON_PLATE_REACTION,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手反应盘放杯
LH_ERR IPC_HandWashPutCupOnPlateReactionWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_CUP_ON_PLATE_REACTION,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手清洗盘取杯
LH_ERR IPC_HandWashCatchCupOnPlateWashWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_CUP_ON_PLATE_WASH,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手清洗盘放杯
LH_ERR IPC_HandWashPutCupOnPlateWashWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_CUP_ON_PLATE_WASH,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手测量模块抓杯
LH_ERR IPC_HandWashCatchCupOnMeasureUnitWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_CUP_ON_MEASURE_UNIT,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手测量模块放杯
LH_ERR IPC_HandWashPutCupOnMeasureUnitWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_HandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_CUP_ON_MEASURE_UNIT,0,0,0,0,0); 
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_HandWashWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}
