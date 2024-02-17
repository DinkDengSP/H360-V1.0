/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 14:20:37
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-09 17:11:28
**FilePath: \DeviceMainApplication\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcAssist.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcAssist.h"
#include "ServiceTaskConfig.h"

//IPC调用的数据结构
static IPC_DATA_ASSIST dataIPC_Assist;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_Assist;

//指令列表
typedef enum IPC_CMD_ASSIST
{
    IPC_CMD_ASSIST_READ_SOFTWARE_VERSION            = 0X0000,//读取软件版本
    IPC_CMD_ASSIST_READ_SYSTEM_STATE                = 0X0001,//读取系统状态
    IPC_CMD_ASSIST_HAND_SHAKE                       = 0X0002,//握手
    IPC_CMD_ASSIST_SYSTEM_EMERGENCY_STOP            = 0X0003,//系统紧急急停
    IPC_CMD_ASSIST_READ_ACTION_PARAM                = 0X0004,//读动作参数
    IPC_CMD_ASSIST_SET_ACTION_BOARD_MODEL_VERSION   = 0X0005,//设置下位机模块运行版本
}IPC_CMD_ASSIST;

//IPC调用基础结构初始化
void IPC_ServiceAssistDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_Assist,
				  (CPU_CHAR*	)"mutexIPC_Assist",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_Assist.dataMutexPtr = &mutexIPC_Assist;
    dataIPC_Assist.state = STATE_IPC_IDLE;
    dataIPC_Assist.resultValue = LH_ERR_NONE;
    dataIPC_Assist.can1ReportLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    dataIPC_Assist.can1ReportCode = CAN1_REPORT_ERR_OK;
}

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceAssistGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_Assist,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_Assist.resultValue;
    *can1ReportErrorLevel = dataIPC_Assist.can1ReportLevel;
    *can1ReportErrorCode = dataIPC_Assist.can1ReportCode;
    //释放信号量
    OSMutexPost(&mutexIPC_Assist,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_Assist.state;
}

//等待执行完成,阻塞
LH_ERR IPC_ServiceAssistWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_ServiceAssistGetLastCommandStateAndResult(&errorCode,can1ReportErrorLevel,can1ReportErrorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            //延时指定时间
            CoreDelayMs(5);
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

//动作API发送任务给动作API
static LH_ERR IPC_ServiceAssistSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_Assist,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_Assist.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_Assist.resultValue = LH_ERR_NONE;

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
    systemTaskCmd->commandSrc = SYSTEM_CMD_SRC_SELF;
    //申请到内存了
    systemTaskCmd->systemCommandDataPtr = NULL;

    //一定要申请到
    while(systemTaskCmd->systemCommandDataPtr == NULL)
    {
        systemTaskCmd->systemCommandDataPtr = UserMemMalloc(MEM_DTCM,sizeof(SYSTEM_CMD_SELF));
        if(systemTaskCmd->systemCommandDataPtr == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //辅助指针
    SYSTEM_CMD_SELF* utilSelfCmdPtr = (SYSTEM_CMD_SELF*)(systemTaskCmd->systemCommandDataPtr);
    //消息序号
    utilSelfCmdPtr->commandIndex = (uint16_t)cmdIndex;
    //参数设置
    utilSelfCmdPtr->commandParam1 = param1;
    utilSelfCmdPtr->commandParam2 = param2;
    utilSelfCmdPtr->commandParam3 = param3;
    utilSelfCmdPtr->commandParam4 = param4;
    utilSelfCmdPtr->commandParam5 = param5;
    //设置不同信道特有的数据指针
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_Assist);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskServiceAssist, //向协议处理线程发送消息
                        (void *)systemTaskCmd,
                        (OS_MSG_SIZE)sizeof(SYSTEM_CMD*),
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,直接释放本次申请的内存
        UserMemFree(MEM_DTCM, (void *)systemTaskCmd->systemCommandDataPtr);
        UserMemFree(MEM_DTCM, (void *)systemTaskCmd);
        //指令没发出去,还是回到空闲状态
        dataIPC_Assist.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_Assist,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_Assist,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_Assist.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


//读取软件版本,不等待指令完成,非堵塞
LH_ERR IPC_ServiceAssistReadSoftwareVersionWhileAck(VERSION_BOARD_ID boardID)
{
    return IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_READ_SOFTWARE_VERSION,(int32_t)(boardID),0,0,0,0);
}

//读取系统状态,不等待指令完成,非堵塞
LH_ERR IPC_ServiceAssistReadSystemStateWhileAck(uint8_t mainIndex,uint8_t subIndex)
{
    return IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_READ_SYSTEM_STATE,(int32_t)(mainIndex),(int32_t)(subIndex),0,0,0);
}

//握手,不等待指令完成,非堵塞
LH_ERR IPC_ServiceAssistHandShakeWhileAck(MODE_RUNNING modeRun)
{
    return IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_HAND_SHAKE,(int32_t)(modeRun),0,0,0,0);
}

//系统紧急急停,不等待指令完成,非堵塞
LH_ERR IPC_ServiceAssistSystemEmergencyStopWhileAck(void)
{
    return IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_SYSTEM_EMERGENCY_STOP,0,0,0,0,0);
}

//设置下位机模块运行版本
LH_ERR IPC_ServiceAssistSetActionBoardModelVersionWhileAck(void)
{
    return IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_SET_ACTION_BOARD_MODEL_VERSION,0,0,0,0,0);
}



//读取软件版本,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistReadSoftwareVersionWhileReturn(VERSION_BOARD_ID boardID,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_READ_SOFTWARE_VERSION,(int32_t)(boardID),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceAssistWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//读取系统状态,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistReadSystemStateWhileReturn(uint8_t mainIndex,uint8_t subIndex,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_READ_SYSTEM_STATE,(int32_t)(mainIndex),(int32_t)(subIndex),0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceAssistWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//握手,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistHandShakeWhileReturn(MODE_RUNNING modeRun,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_HAND_SHAKE,(int32_t)(modeRun),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceAssistWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//系统紧急急停,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistSystemEmergencyStopWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_SYSTEM_EMERGENCY_STOP,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceAssistWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//读动作参数,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistReadActionParamWhileReturn(uint8_t mainIndex,uint8_t subIndex,int32_t *liquidDetectPos,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_READ_ACTION_PARAM,mainIndex,subIndex,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceAssistWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    *liquidDetectPos = dataIPC_Assist.liquidDetectPos;
    //使用完数据清零
    dataIPC_Assist.liquidDetectPos = 0;
    //返回结果代码
    return LH_ERR_NONE;
}


//设置下位机模块运行版本,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistSetActionBoardModelVersionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
    errorCode = IPC_ServiceAssistSendCommandWhileAck(IPC_CMD_ASSIST_SET_ACTION_BOARD_MODEL_VERSION,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //等待指令完成
    errorCode = IPC_ServiceAssistWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}


