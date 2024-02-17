/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 14:20:26
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-15 16:01:10
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcLiquidAuto.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcLiquidAuto.h"
#include "ServiceTaskConfig.h"

//IPC调用的数据结构
static IPC_DATA_LIQUID_AUTO dataIPC_LiquidAuto;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_LiquidAuto;

//指令列表
typedef enum IPC_CMD_LIQUID_AUTO
{
    IPC_CMD_LIQUID_AUTO_ENABLE_WASTE_DISCHARGE_AUTO                         = 0X0000,//使能自动抽废液
    IPC_CMD_LIQUID_AUTO_ENABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO              = 0X0001,//使能自动三针清洗液稀释
    IPC_CMD_LIQUID_AUTO_ENABLE_PURE_WATER_SUPPLEMENT_AUTO                   = 0X0002,//使能自动存水补充
    IPC_CMD_LIQUID_AUTO_DISABLE_WASTE_DISCHARGE_AUTO                        = 0X0003,//禁止自动抽废液
    IPC_CMD_LIQUID_AUTO_DISABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO             = 0X0004,//禁止自动三针清洗液稀释
    IPC_CMD_LIQUID_AUTO_DISABLE_PURE_WATER_SUPPLEMENT_AUTO                  = 0X0005,//禁止纯水自动补充
    IPC_CMD_LIQUID_AUTO_ENFORCE_THREE_NEEDLE_CLEAN_DILUENT_ONCE             = 0X0006,//强制执行一次稀释液配置
    IPC_CMD_LIQUID_AUTO_ENFORCE_PURE_WATER_SUPPLEMENT_ONCE                  = 0X0007,//强制执行一次补纯水
    IPC_CMD_LIQUID_AUTO_SET_THREE_NEEDLE_AUTO_DILUENT_CONCENTRATE_BOTTLE    = 0X0008,//设定三针自动稀释浓缩液瓶号
}IPC_CMD_LIQUID_AUTO;

//IPC调用基础结构初始化
void IPC_ServiceLiquidAutoDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_LiquidAuto,
				  (CPU_CHAR*	)"mutexIPC_LiquidAuto",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_LiquidAuto.dataMutexPtr = &mutexIPC_LiquidAuto;
    dataIPC_LiquidAuto.state = STATE_IPC_IDLE;
    dataIPC_LiquidAuto.resultValue = LH_ERR_NONE;
    dataIPC_LiquidAuto.can1ReportLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    dataIPC_LiquidAuto.can1ReportCode = CAN1_REPORT_ERR_OK;
    //数据初始化
    
}

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceLiquidAutoGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_LiquidAuto,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_LiquidAuto.resultValue;
    *can1ReportErrorLevel = dataIPC_LiquidAuto.can1ReportLevel;
    *can1ReportErrorCode = dataIPC_LiquidAuto.can1ReportCode;
    //释放信号量
    OSMutexPost(&mutexIPC_LiquidAuto,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_LiquidAuto.state;
}

//等待执行完成,阻塞
LH_ERR IPC_ServiceLiquidAutoWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_ServiceLiquidAutoGetLastCommandStateAndResult(&errorCode,can1ReportErrorLevel,can1ReportErrorCode);
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
static LH_ERR IPC_ServiceLiquidAutoSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_LiquidAuto,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_LiquidAuto.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_LiquidAuto.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_LiquidAuto);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskServiceLiquidAuto, //向协议处理线程发送消息
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
        dataIPC_LiquidAuto.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_LiquidAuto,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_LiquidAuto,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_LiquidAuto.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


//使能自动抽废液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnableWasteDischargeAutoWhileAck(void)
{
    return IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENABLE_WASTE_DISCHARGE_AUTO,0,0,0,0,0);
}

//使能自动三针清洗液稀释,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnableThreeNeedleCleanDiluentAutoWhileAck(void)
{
    return IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO,0,0,0,0,0);
}

//使能自动存水补充,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnablePureWaterSupplementAutoWhileAck(void)
{
    return IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENABLE_PURE_WATER_SUPPLEMENT_AUTO,0,0,0,0,0);
}

//禁止自动抽废液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoDisableWasteDischargeAutoWhileAck(void)
{
    return IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_DISABLE_WASTE_DISCHARGE_AUTO,0,0,0,0,0);
}

//禁止自动三针清洗液稀释,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoDisableThreeNeedleCleanDiluentAutoWhileAck(void)
{
    return IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_DISABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO,0,0,0,0,0);
}

//禁止纯水自动补充,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoDisablePureWaterSupplementAutoWhileAck(void)
{
    return IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_DISABLE_PURE_WATER_SUPPLEMENT_AUTO,0,0,0,0,0);
}
//液路自动化强制三针清洗液稀释一次,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnforceThreeNeedleCleanDiluentOnceWhileAck(void)
{
    return IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENFORCE_THREE_NEEDLE_CLEAN_DILUENT_ONCE,0,0,0,0,0);
}
//液路自动化强制补充存水一次,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnforcePureWaterSupplementOnceWhileAck(void)
{
    return IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENFORCE_PURE_WATER_SUPPLEMENT_ONCE,0,0,0,0,0);
}
//设定三针自动稀释浓缩液瓶号,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoSetThreeNeedleAutoDiluentConcentrateBottleWhileAck(CONCENTRATE_BOTTLE_SELECT concentrateBottle)
{
    return IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_SET_THREE_NEEDLE_AUTO_DILUENT_CONCENTRATE_BOTTLE,concentrateBottle,0,0,0,0);
}



//使能自动抽废液,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnableWasteDischargeAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENABLE_WASTE_DISCHARGE_AUTO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceLiquidAutoWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//使能自动三针清洗液稀释,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnableThreeNeedleCleanDiluentAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceLiquidAutoWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//使能自动存水补充,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnablePureWaterSupplementAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENABLE_PURE_WATER_SUPPLEMENT_AUTO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceLiquidAutoWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//禁止自动抽废液,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoDisableWasteDischargeAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_DISABLE_WASTE_DISCHARGE_AUTO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceLiquidAutoWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//禁止自动三针清洗液稀释,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoDisableThreeNeedleCleanDiluentAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_DISABLE_THREE_NEEDLE_CLEAN_DILUENT_AUTO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceLiquidAutoWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//禁止纯水自动补充,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoDisablePureWaterSupplementAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_DISABLE_PURE_WATER_SUPPLEMENT_AUTO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceLiquidAutoWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//液路自动化强制三针清洗液稀释一次,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnforceThreeNeedleCleanDiluentOnceWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENFORCE_THREE_NEEDLE_CLEAN_DILUENT_ONCE,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceLiquidAutoWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}
//液路自动化强制补充存水一次,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnforcePureWaterSupplementOnceWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_ENFORCE_PURE_WATER_SUPPLEMENT_ONCE,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceLiquidAutoWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//设定三针自动稀释浓缩液瓶号,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoSetThreeNeedleAutoDiluentConcentrateBottleWhileReturn(CONCENTRATE_BOTTLE_SELECT concentrateBottle,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceLiquidAutoSendCommandWhileAck(IPC_CMD_LIQUID_AUTO_SET_THREE_NEEDLE_AUTO_DILUENT_CONCENTRATE_BOTTLE,concentrateBottle,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceLiquidAutoWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}






















