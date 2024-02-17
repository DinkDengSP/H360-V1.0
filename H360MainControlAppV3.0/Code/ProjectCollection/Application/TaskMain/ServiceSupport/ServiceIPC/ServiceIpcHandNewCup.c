/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 14:21:13
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-03 00:10:22
**FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcHandNewCup.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcHandNewCup.h"
#include "ServiceTaskConfig.h"

//IPC调用的数据结构
static IPC_DATA_HAND_NEW_CUP dataIPC_HandNewCup;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_HandNewCup;

//指令列表
typedef enum IPC_CMD_HAND_NEW_CUP
{
    IPC_CMD_HAND_NEW_CUP_RESET_ALL                      = 0X0000,//新杯机械手复位
    IPC_CMD_HAND_NEW_CUP_MOVE_2_NEW_STACK               = 0X0001,//新杯机械手移动到新杯栈
    IPC_CMD_HAND_NEW_CUP_MOVE_2_PLATE_REACTION          = 0X0002,//新杯机械手移动到反应盘
    IPC_CMD_HAND_NEW_CUP_MOVE_2_GARBAGE                 = 0X0003,//新杯机械手移动到垃圾桶
    IPC_CMD_HAND_NEW_CUP_CATCH_CUP_FROM_NEW_STACK       = 0X0004,//新杯机械手从新杯栈抓杯
    IPC_CMD_HAND_NEW_CUP_PUT_CUP_TO_PLATE_REACTION      = 0X0005,//新杯机械手往反应盘放杯
    IPC_CMD_HAND_NEW_CUP_CATCH_CUP_FROM_PLATE_REACTION  = 0X0006,//新杯机械手从反应盘抓杯
    IPC_CMD_HAND_NEW_CUP_PUT_CUP_TO_GARBAGE             = 0X0007,//新杯机械手往垃圾桶放杯
}IPC_CMD_HAND_NEW_CUP;

//IPC调用基础结构初始化
void IPC_ServiceHandNewCupDataInit(void)
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
    dataIPC_HandNewCup.can1ReportLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    dataIPC_HandNewCup.can1ReportCode = CAN1_REPORT_ERR_OK;
}


//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceHandNewCupGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
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
    *can1ReportErrorLevel = dataIPC_HandNewCup.can1ReportLevel;
    *can1ReportErrorCode = dataIPC_HandNewCup.can1ReportCode;
    //释放信号量
    OSMutexPost(&mutexIPC_HandNewCup,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_HandNewCup.state;
}

//等待执行完成,阻塞
LH_ERR IPC_ServiceHandNewCupWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_ServiceHandNewCupGetLastCommandStateAndResult(&errorCode,can1ReportErrorLevel,can1ReportErrorCode);
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
static LH_ERR IPC_ServiceHandNewCupSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_HandNewCup);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskServiceHandNewCup, //向协议处理线程发送消息
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
        dataIPC_HandNewCup.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_HandNewCup,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_HandNewCup,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_HandNewCup.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}



//新杯机械手复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupResetAllWhileAck(void)
{
    return IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_RESET_ALL,0,0,0,0,0);
}

//新杯机械手移动到新杯栈,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupMove2NewStackWhileAck(uint8_t leftRight,uint8_t frontBack)
{
    return IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_2_NEW_STACK,(int32_t)(leftRight),(int32_t)(frontBack),0,0,0);
}

//新杯机械手移动到反应盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupMove2PlateReactionWhileAck(void)
{
    return IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_2_PLATE_REACTION,0,0,0,0,0);
}

//新杯机械手移动到垃圾桶,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupMove2GarbageWhileAck(GARBAGE_SELECT garbageNo)
{
    return IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_2_GARBAGE,(int32_t)(garbageNo),0,0,0,0);
}

//新杯机械手从新杯栈抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupCatchCupFromNewStackWhileAck(void)
{
    return IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_CATCH_CUP_FROM_NEW_STACK,0,0,0,0,0);
}

//新杯机械手往反应盘放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupPutCupToPlateReactionWhileAck(void)
{
    return IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_PUT_CUP_TO_PLATE_REACTION,0,0,0,0,0);
}

//新杯机械手从反应盘抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupCatchCupFromPlateReactionWhileAck(void)
{
    return IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_CATCH_CUP_FROM_PLATE_REACTION,0,0,0,0,0);
}

//新杯机械手往垃圾桶放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupPutCupToGarbageWhileAck(GARBAGE_SELECT garbageNo)
{
    return IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_PUT_CUP_TO_GARBAGE,(int32_t)(garbageNo),0,0,0,0);
}






//新杯机械手复位,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_RESET_ALL,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandNewCupWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手移动到新杯栈,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupMove2NewStackWhileReturn(uint8_t leftRight,uint8_t frontBack,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_2_NEW_STACK,(int32_t)(leftRight),(int32_t)(frontBack),0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandNewCupWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手移动到反应盘,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupMove2PlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_2_PLATE_REACTION,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandNewCupWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手移动到垃圾桶,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupMove2GarbageWhileReturn(GARBAGE_SELECT garbageNo,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_MOVE_2_GARBAGE,(int32_t)(garbageNo),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandNewCupWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手从新杯栈抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupCatchCupFromNewStackWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_CATCH_CUP_FROM_NEW_STACK,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandNewCupWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手往反应盘放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupPutCupToPlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_PUT_CUP_TO_PLATE_REACTION,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandNewCupWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手从反应盘抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupCatchCupFromPlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_CATCH_CUP_FROM_PLATE_REACTION,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandNewCupWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯机械手往垃圾桶放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupPutCupToGarbageWhileReturn(GARBAGE_SELECT garbageNo,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandNewCupSendCommandWhileAck(IPC_CMD_HAND_NEW_CUP_PUT_CUP_TO_GARBAGE,(int32_t)(garbageNo),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandNewCupWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}


























