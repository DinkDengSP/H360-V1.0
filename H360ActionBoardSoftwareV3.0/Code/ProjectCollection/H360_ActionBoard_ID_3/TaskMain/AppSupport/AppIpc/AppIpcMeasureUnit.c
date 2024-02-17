/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:41:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-07 12:53:11
**FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppIpc\AppIpcMeasureUnit.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcMeasureUnit.h"
#include "AppTaskConfig.h"
//IPC调用的数据结构
static DATA_IPC_MEASURE_UNIT dataIPC_MeasureUnit;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_MeasureUnit;

//指令列表
typedef enum IPC_CMD_MEASURE_UNIT
{
    IPC_CMD_MEASURE_UNIT_RESET                  = 0X00,
    IPC_CMD_MEASURE_UNIT_DOOR_OPEN_FULL         = 0X01,
    IPC_CMD_MEASURE_UNIT_DOOR_OPEN_HALF         = 0X02,
    IPC_CMD_MEASURE_UNIT_DOOR_CLOSE             = 0X03,
    IPC_CMD_MEASURE_UNIT_WINDOW_OPEN            = 0X04,
    IPC_CMD_MEASURE_UNIT_WINDOW_CLOSE           = 0X05,
    IPC_CMD_MEASURE_UNIT_PRIME_ONCE             = 0X06,
    IPC_CMD_MEASURE_UNIT_INJECT_ONCE            = 0X07,
    IPC_CMD_MEASURE_UNIT_INJECT_MAINTAIN        = 0X08,
    IPC_CMD_MEASURE_UNIT_READ_DARK              = 0X09,
    IPC_CMD_MEASURE_UNIT_READ_BACKGROUND        = 0X0A,
    IPC_CMD_MEASURE_UNIT_READ_LIGHT_WITH_INJECT = 0X0B,
    IPC_CMD_MEASURE_UNIT_READ_LIGHT_COMPLEX     = 0X0C,
    IPC_CMD_MEASURE_UNIT_READ_LIGHT_CHANGE_RATE = 0X0D,
}IPC_CMD_MEASURE_UNIT;



//初始化IPC调用结构
void IPC_MeasureUnitDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_MeasureUnit,
				  (CPU_CHAR*	)"mutexIPC_MeasureUnit",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_MeasureUnit.dataMutexPtr = &mutexIPC_MeasureUnit;
    dataIPC_MeasureUnit.state = STATE_IPC_IDLE;
    dataIPC_MeasureUnit.resultValue = LH_ERR_NONE;
}


/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_MeasureUnitGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_MeasureUnit,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_MeasureUnit.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_MeasureUnit,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_MeasureUnit.state;
}

//等待执行完成,阻塞
LH_ERR IPC_MeasureUnitWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_MeasureUnitGetLastCommandStatusAndResult(&errorCode);
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
static LH_ERR IPC_MeasureUnitSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_MeasureUnit,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_MeasureUnit.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_MeasureUnit.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_MeasureUnit);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskMeasureUnit, //向协议处理线程发送消息
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
        dataIPC_MeasureUnit.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_MeasureUnit,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_MeasureUnit,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_MeasureUnit.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}

/****************************************************发出指令不等待完成*******************************************************/
//测量模块初始化
LH_ERR IPC_MeasureUnitResetWhileAck(void)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_RESET,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//测量室上门全开
LH_ERR IPC_MeasureUnitDoorOpenFullWhileAck(void)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_DOOR_OPEN_FULL,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//测量室上门半开
LH_ERR IPC_MeasureUnitDoorOpenHalfWhileAck(void)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_DOOR_OPEN_HALF,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//测量室上门关闭
LH_ERR IPC_MeasureUnitDoorCloseWhileAck(void)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_DOOR_CLOSE,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//测量室侧门打开
LH_ERR IPC_MeasureUnitWindowOpenWhileAck(void)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_WINDOW_OPEN,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//测量室侧门关闭
LH_ERR IPC_MeasureUnitWindowCloseWhileAck(void)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_WINDOW_CLOSE,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//激发液灌注
LH_ERR IPC_MeasureUnitPrimeOnceWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_PRIME_ONCE,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//激发液注液
LH_ERR IPC_MeasureUnitInjectOnceWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_INJECT_ONCE,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//测量模块清洗维护
LH_ERR IPC_MeasureUnitInjectMaintainWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_INJECT_MAINTAIN,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//测量模块暗记数
LH_ERR IPC_MeasureUnitReadDarkWhileAck(void)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_DARK,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0)); 
}

//测量模块读本底
LH_ERR IPC_MeasureUnitReadBackGroundWhileAck(void)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_BACKGROUND,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0)); 
}

//测量模块注液测光
LH_ERR IPC_MeasureUnitReadLightWithInjectWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_LIGHT_WITH_INJECT,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//测量模块集成单次测量
LH_ERR IPC_MeasureUnitReadLightComplexWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_LIGHT_COMPLEX,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

//测量模块读光子变化率
LH_ERR IPC_MeasureUnitReadLightChangeRateWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount)
{
    return IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_LIGHT_CHANGE_RATE,(int32_t)(bottle),(int32_t)(setChangeCount),(int32_t)(0),(int32_t)(0),(int32_t)(0));
}

/****************************************************发出指令并等待完成********************************************************/
//测量模块初始化
LH_ERR IPC_MeasureUnitResetWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_RESET,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量室上门全开
LH_ERR IPC_MeasureUnitDoorOpenFullWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_DOOR_OPEN_FULL,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量室上门半开
LH_ERR IPC_MeasureUnitDoorOpenHalfWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_DOOR_OPEN_HALF,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量室上门关闭
LH_ERR IPC_MeasureUnitDoorCloseWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_DOOR_CLOSE,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量室侧门打开
LH_ERR IPC_MeasureUnitWindowOpenWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_WINDOW_OPEN,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量室侧门关闭
LH_ERR IPC_MeasureUnitWindowCloseWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_WINDOW_CLOSE,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//激发液灌注
LH_ERR IPC_MeasureUnitPrimeOnceWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_PRIME_ONCE,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//激发液注液
LH_ERR IPC_MeasureUnitInjectOnceWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_INJECT_ONCE,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量模块清洗维护
LH_ERR IPC_MeasureInjectMaintainWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_INJECT_MAINTAIN,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量模块暗记数
LH_ERR IPC_MeasureUnitReadDarkWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_DARK,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0)); 
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量模块读本底
LH_ERR IPC_MeasureUnitReadBackGroundWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_BACKGROUND,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0)); 
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量模块注液测光
LH_ERR IPC_MeasureUnitReadLightWithInjectWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_LIGHT_WITH_INJECT,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量模块集成单次测量
LH_ERR IPC_MeasureUnitReadLightComplexWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_LIGHT_COMPLEX,(int32_t)(bottle),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//测量模块读光子变化率
LH_ERR IPC_MeasureUnitReadLightChangeRateWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_MeasureUnitSendCommandWhileAck(IPC_CMD_MEASURE_UNIT_READ_LIGHT_CHANGE_RATE,(int32_t)(bottle),(int32_t)(setChangeCount),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_MeasureUnitWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}


