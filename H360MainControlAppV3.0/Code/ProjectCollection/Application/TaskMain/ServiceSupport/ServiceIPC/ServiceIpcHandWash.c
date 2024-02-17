/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 14:21:20
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-09 16:51:38
**FilePath: \DeviceMainApplication\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcHandWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcHandWash.h"
#include "ServiceTaskConfig.h"

//IPC调用的数据结构
static IPC_DATA_HAND_WASH dataIPC_HandWash;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_HandWash;

//指令列表
typedef enum IPC_CMD_HAND_WASH
{
    IPC_CMD_HAND_WASH_RESET_ALL                 = 0X0000,//清洗机械手复位
    IPC_CMD_HAND_WASH_ROTATE_PLATE_WASH         = 0X0001,//清洗机械手旋转到清洗盘
    IPC_CMD_HAND_WASH_ROTATE_PLATE_REACTION     = 0X0002,//清洗机械手旋转到反应盘
    IPC_CMD_HAND_WASH_ROTATE_MEASURE_MODULE     = 0X0003,//清洗机械手旋转到测量模块
    IPC_CMD_HAND_WASH_CATCH_PLATE_REACTION      = 0X0004,//清洗机械手从反应盘抓杯
    IPC_CMD_HAND_WASH_PUT_PLATE_REACTION        = 0X0005,//清洗机械手往反应盘放杯
    IPC_CMD_HAND_WASH_CATCH_PLATE_WASH          = 0X0006,//清洗机械手从清洗盘抓杯
    IPC_CMD_HAND_WASH_PUT_PLATE_WASH            = 0X0007,//清洗机械手往清洗盘放杯
    IPC_CMD_HAND_WASH_CATCH_MEASURE_MODULE      = 0X0008,//清洗机械手穷测量模块抓杯
    IPC_CMD_HAND_WASH_PUT_MEASURE_MODULE        = 0X0009,//清洗机械手往测量模块放杯
}IPC_CMD_HAND_WASH;

//IPC调用基础结构初始化
void IPC_ServiceHandWashDataInit(void)
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
    dataIPC_HandWash.can1ReportLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    dataIPC_HandWash.can1ReportCode = CAN1_REPORT_ERR_OK;
}


//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceHandWashGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
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
    *can1ReportErrorLevel = dataIPC_HandWash.can1ReportLevel;
    *can1ReportErrorCode = dataIPC_HandWash.can1ReportCode;
    //释放信号量
    OSMutexPost(&mutexIPC_HandWash,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_HandWash.state;
}

//等待执行完成,阻塞
LH_ERR IPC_ServiceHandWashWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_ServiceHandWashGetLastCommandStateAndResult(&errorCode,can1ReportErrorLevel,can1ReportErrorCode);
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
static LH_ERR IPC_ServiceHandWashSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_HandWash);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskServiceHandWash, //向协议处理线程发送消息
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
        dataIPC_HandWash.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_HandWash,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_HandWash,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_HandWash.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}








//清洗机械手复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashResetAllWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_RESET_ALL,0,0,0,0,0);
}

//清洗机械手旋转到清洗盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashRotatePlateWashWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_PLATE_WASH,0,0,0,0,0);
}

//清洗机械手旋转到反应盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashRotatePlateReactionWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_PLATE_REACTION,0,0,0,0,0);
}

//清洗机械手旋转到测量模块,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashRotateMeasureModuleWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_MEASURE_MODULE,0,0,0,0,0);
}

//清洗机械手从反应盘抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashCatchPlateReactionWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_PLATE_REACTION,0,0,0,0,0);
}

//清洗机械手往反应盘放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashPutPlateReactionWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_PLATE_REACTION,0,0,0,0,0);
}

//清洗机械手从清洗盘抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashCatchPlateWashWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_PLATE_WASH,0,0,0,0,0);
}

//清洗机械手往清洗盘放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashPutPlateWashWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_PLATE_WASH,0,0,0,0,0);
}

//清洗机械手穷测量模块抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashCatchMeasureModuleWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_MEASURE_MODULE,0,0,0,0,0);
}

//清洗机械手往测量模块放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashPutMeasureModuleWhileAck(void)
{
    return IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_MEASURE_MODULE,0,0,0,0,0);
}







//清洗机械手复位,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_RESET_ALL,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手旋转到清洗盘,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashRotatePlateWashWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_PLATE_WASH,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手旋转到反应盘,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashRotatePlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_PLATE_REACTION,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手旋转到测量模块,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashRotateMeasureModuleWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_ROTATE_MEASURE_MODULE,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手从反应盘抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashCatchPlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_PLATE_REACTION,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手往反应盘放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashPutPlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_PLATE_REACTION,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手从清洗盘抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashCatchPlateWashWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_PLATE_WASH,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手往清洗盘放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashPutPlateWashWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_PLATE_WASH,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手穷测量模块抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashCatchMeasureModuleWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_CATCH_MEASURE_MODULE,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//清洗机械手往测量模块放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashPutMeasureModuleWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceHandWashSendCommandWhileAck(IPC_CMD_HAND_WASH_PUT_MEASURE_MODULE,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceHandWashWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}







