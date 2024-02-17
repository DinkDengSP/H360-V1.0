/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 14:17:24
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-03 15:04:12
**FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcNeedleBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcNeedleBead.h"
#include "ServiceTaskConfig.h"

//IPC调用的数据结构
static IPC_DATA_NEEDLE_BEAD dataIPC_NeedleBead;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_NeedleBead;

//指令列表
typedef enum IPC_CMD_NEEDLE_BEAD
{
    IPC_CMD_NEEDLE_BEAD_RESET_ALL    			= 0X0000,//磁珠针复位
    IPC_CMD_NEEDLE_BEAD_ROTATE_POS   			= 0X0001,//磁珠针旋转到指定位置
    IPC_CMD_NEEDLE_BEAD_CLEAN        			= 0X0002,//磁珠针清洗
    IPC_CMD_NEEDLE_BEAD_PRIME        			= 0X0003,//磁珠针灌注
    IPC_CMD_NEEDLE_BEAD_ABSORB       			= 0X0004,//磁珠针吸液
    IPC_CMD_NEEDLE_BEAD_INJECT       			= 0X0005,//磁珠针注液
    IPC_CMD_NEEDLE_BEAD_INJECT_MIX   			= 0X0006,//磁珠针注液带混匀
    IPC_CMD_NEEDLE_BEAD_MAINTAIN     			= 0X0007,//磁珠针清洗维护
    IPC_CMD_NEEDLE_BEAD_ADJUST       			= 0X0008,//磁珠针探液校准
    IPC_CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK	= 0X0009,//磁珠针试剂余量校准
}IPC_CMD_NEEDLE_BEAD;

//IPC调用基础结构初始化
void IPC_ServiceNeedleBeadDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_NeedleBead,
				  (CPU_CHAR*	)"mutexIPC_NeedleBead",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_NeedleBead.dataMutexPtr = &mutexIPC_NeedleBead;
    dataIPC_NeedleBead.state = STATE_IPC_IDLE;
    dataIPC_NeedleBead.resultValue = LH_ERR_NONE;
    dataIPC_NeedleBead.can1ReportLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    dataIPC_NeedleBead.can1ReportCode = CAN1_REPORT_ERR_OK;

}

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceNeedleBeadGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_NeedleBead,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_NeedleBead.resultValue;
    *can1ReportErrorLevel = dataIPC_NeedleBead.can1ReportLevel;
    *can1ReportErrorCode = dataIPC_NeedleBead.can1ReportCode;
    //释放信号量
    OSMutexPost(&mutexIPC_NeedleBead,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_NeedleBead.state;
}

//等待执行完成,阻塞
LH_ERR IPC_ServiceNeedleBeadWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_ServiceNeedleBeadGetLastCommandStateAndResult(&errorCode,can1ReportErrorLevel,can1ReportErrorCode);
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
static LH_ERR IPC_ServiceNeedleBeadSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_NeedleBead,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_NeedleBead.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_NeedleBead.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_NeedleBead);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskServiceNeedleBead, //向协议处理线程发送消息
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
        dataIPC_NeedleBead.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_NeedleBead,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_NeedleBead,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_NeedleBead.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}



//磁珠针复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadResetAllWhileAck(void)
{
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_RESET_ALL,0,0,0,0,0);
}

//磁珠针旋转到指定位置,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadRotatePosWhileAck(NEEDLE_BEAD_POS targetPos)
{
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_ROTATE_POS,(int32_t)(targetPos),0,0,0,0);
}

//磁珠针清洗,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadCleanWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime)
{
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_CLEAN,(int32_t)(cleanOpt),(int32_t)(cleanTime),0,0,0);
}

//磁珠针灌注,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadPrimeWhileAck(NEEDLE_PRIME_OPT primeOpt)
{
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_PRIME,(int32_t)(primeOpt),0,0,0,0);
}

//磁珠针吸液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadAbsorbWhileAck(uint16_t absorbCountMinUL)
{
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_ABSORB,(int32_t)(absorbCountMinUL),0,0,0,0);
}

//磁珠针注液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadInjectWhileAck(float cupLiquidUl)
{   
    //实际下发为液量乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidUl * 1000);
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_INJECT,(int32_t)(cupLiquidUlDown),0,0,0,0);
}

//磁珠针注液带混匀,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadInjectMixWhileAck(float cupLiquidUl,uint8_t mixCount)
{
    //实际下发为液量乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidUl * 1000);
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_INJECT_MIX,(int32_t)(cupLiquidUlDown),(int32_t)(mixCount),0,0,0);
}

//磁珠针清洗维护,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadMainTainWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime)
{
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_MAINTAIN,(int32_t)(cleanOpt),(int32_t)(cleanTime),0,0,0);
}

//磁珠针探液校准,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadAdjustWhileAck(CMD_SOURCE_TEST_FLOW cmdSource)
{
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_ADJUST,(int32_t)(cmdSource),0,0,0,0);
}

//磁珠针试剂余量检查,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadLiquidResidueCheckWhileAck(void)
{
    return IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK,0,0,0,0,0);
}

//磁珠针复位,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_RESET_ALL,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针旋转到指定位置,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadRotatePosWhileReturn(NEEDLE_BEAD_POS targetPos,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_ROTATE_POS,(int32_t)(targetPos),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针清洗,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadCleanWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_CLEAN,(int32_t)(cleanOpt),(int32_t)(cleanTime),0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针灌注,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadPrimeWhileReturn(NEEDLE_PRIME_OPT primeOpt,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_PRIME,(int32_t)(primeOpt),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针吸液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadAbsorbWhileReturn(uint16_t absorbCountMinUL,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_ABSORB,(int32_t)(absorbCountMinUL),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针注液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadInjectWhileReturn(float cupLiquidUl,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //实际下发为液量乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidUl * 1000);
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_INJECT,(int32_t)(cupLiquidUlDown),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针注液带混匀,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadInjectMixWhileReturn(float cupLiquidUl,uint8_t mixCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //实际下发为液量乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidUl * 1000);
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_INJECT_MIX,(int32_t)(cupLiquidUlDown),(int32_t)(mixCount),0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针清洗维护,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadMainTainWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_MAINTAIN,(int32_t)(cleanOpt),(int32_t)(cleanTime),0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针探液校准,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadAdjustWhileReturn(CMD_SOURCE_TEST_FLOW cmdSource,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_ADJUST,(int32_t)(cmdSource),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//磁珠针试剂余量检查,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadLiquidResidueCheckWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
        LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleBeadSendCommandWhileAck(IPC_CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleBeadWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}



































