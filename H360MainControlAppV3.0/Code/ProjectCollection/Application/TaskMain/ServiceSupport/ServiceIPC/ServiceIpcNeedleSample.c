/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 14:17:14
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-03 14:26:48
**FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcNeedleSample.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcNeedleSample.h"
#include "ServiceTaskConfig.h"

//IPC调用的数据结构
static IPC_DATA_NEEDLE_SAMPLE dataIPC_NeedleSample;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_NeedleSample;

//指令列表
typedef enum IPC_CMD_NEEDLE_SAMPLE
{
    IPC_CMD_NEEDLE_SAMPLE_RESET_ALL                 = 0X0000,//样本针复位
    IPC_CMD_NEEDLE_SAMPLE_ROTATE_POS                = 0X0001,//样本针旋转到指定位置
    IPC_CMD_NEEDLE_SAMPLE_CLEAN                     = 0X0002,//样本针清洗
    IPC_CMD_NEEDLE_SAMPLE_FORCE_CLEAN               = 0X0003,//样本针强力清洗
    IPC_CMD_NEEDLE_SAMPLE_PRIME                     = 0X0004,//样本针灌注
    IPC_CMD_NEEDLE_SAMPLE_FORCE_PRIME               = 0X0005,//样本针强力灌注
    IPC_CMD_NEEDLE_SAMPLE_ABSORB_RACK               = 0X0006,//样本针试管架吸液
    IPC_CMD_NEEDLE_SAMPLE_ABSORB_PLATE_REACTION     = 0X0007,//样本针反应盘吸液
    IPC_CMD_NEEDLE_SAMPLE_INJECT                    = 0X0008,//样本针注液
    IPC_CMD_NEEDLE_SAMPLE_INJECT_MIX                = 0X0009,//样本针注液带混匀
    IPC_CMD_NEEDLE_SAMPLE_MAINTAIN                  = 0X000A,//样本针管路维护
    IPC_CMD_NEEDLE_SAMPLE_ADJUST                    = 0X000B,//样本针校准
    IPC_CMD_NEEDLE_SAMPLE_INJECT_WITH_MAINTAIN      = 0X000C,//样本针携带维护
}IPC_CMD_NEEDLE_SAMPLE;

//IPC调用基础结构初始化
void IPC_ServiceNeedleSampleDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_NeedleSample,
				  (CPU_CHAR*	)"mutexIPC_NeedleSample",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_NeedleSample.dataMutexPtr = &mutexIPC_NeedleSample;
    dataIPC_NeedleSample.state = STATE_IPC_IDLE;
    dataIPC_NeedleSample.resultValue = LH_ERR_NONE;
    dataIPC_NeedleSample.can1ReportLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    dataIPC_NeedleSample.can1ReportCode = CAN1_REPORT_ERR_OK;
    //数据初始化

}

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceNeedleSampleGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_NeedleSample,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_NeedleSample.resultValue;
    *can1ReportErrorLevel = dataIPC_NeedleSample.can1ReportLevel;
    *can1ReportErrorCode = dataIPC_NeedleSample.can1ReportCode;
    //释放信号量
    OSMutexPost(&mutexIPC_NeedleSample,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_NeedleSample.state;
}

//等待执行完成,阻塞
LH_ERR IPC_ServiceNeedleSampleWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_ServiceNeedleSampleGetLastCommandStateAndResult(&errorCode,can1ReportErrorLevel,can1ReportErrorCode);
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
static LH_ERR IPC_ServiceNeedleSampleSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_NeedleSample,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_NeedleSample.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_NeedleSample.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_NeedleSample);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskServiceNeedleSample, //向协议处理线程发送消息
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
        dataIPC_NeedleSample.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_NeedleSample,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_NeedleSample,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_NeedleSample.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}



//样本针复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleResetAllWhileAck(void)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_RESET_ALL,0,0,0,0,0);
}

//样本针旋转到指定位置,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleRotatePosWhileAck(NEEDLE_SAMPLE_POS targetPos)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ROTATE_POS,(int32_t)(targetPos),0,0,0,0);
}

//样本针清洗,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleCleanWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_CLEAN,(int32_t)(cleanOpt),(int32_t)(cleanTime),0,0,0);
}

//样本针强力清洗,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleForceCleanWhileAck(uint8_t cleanCount)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_FORCE_CLEAN,(int32_t)(cleanCount),0,0,0,0);
}

//样本针灌注,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSamplePrimeWhileAck(NEEDLE_PRIME_OPT primeOpt)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_PRIME,(int32_t)(primeOpt),0,0,0,0);
}

//样本针强力灌注,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleForcePrimeWhileAck(void)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_FORCE_PRIME,0,0,0,0,0);
}

//样本针吸液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleAbsorbRackWhileAck(uint16_t absorbCountMinUL,TUBE_TYPE tubeType,uint8_t newSampleTube)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ABSORB_RACK,(int32_t)(absorbCountMinUL),tubeType,newSampleTube,0,0);
}

//样本针反应盘吸液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleAbsorbPlateReactionWhileAck(uint16_t absorbCountMinUL,float cupLiquidAfterAbsorb)
{
    //杯中液量进行转换乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidAfterAbsorb * 1000);
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ABSORB_PLATE_REACTION,(int32_t)(absorbCountMinUL),(int32_t)(cupLiquidUlDown),0,0,0);
}

//样本针注液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleInjectWhileAck(float cupLiquidUl)
{
        //实际下发为液量乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidUl * 1000);
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_INJECT,(int32_t)(cupLiquidUlDown),0,0,0,0);
}

//样本针注液带混匀,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleInjectMixWhileAck(float cupLiquidUl,uint8_t mixCount)
{
        //实际下发为液量乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidUl * 1000);
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_INJECT_MIX,(int32_t)(cupLiquidUlDown),(int32_t)(mixCount),0,0,0);
}

//样本针管路维护,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleMainTainWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_MAINTAIN,(int32_t)cleanOpt,(int32_t)cleanTime,0,0,0);
}

//样本针校准,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleAdjustWhileAck(CMD_SOURCE_TEST_FLOW cmdSource)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ADJUST,(int32_t)(cmdSource),0,0,0,0);
}

//样本针携带维护,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleInjectWithMaintainWhileAck(uint8_t maintainCount,uint16_t injectUl)
{
    return IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_INJECT_WITH_MAINTAIN,maintainCount,injectUl,0,0,0);
}





//样本针复位,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_RESET_ALL,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针旋转到指定位置,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleRotatePosWhileReturn(NEEDLE_SAMPLE_POS targetPos,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ROTATE_POS,(int32_t)(targetPos),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针清洗,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleCleanWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_CLEAN,(int32_t)(cleanOpt),(int32_t)(cleanTime),0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针强力清洗,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleForceCleanWhileReturn(uint8_t cleanCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_FORCE_CLEAN,(int32_t)(cleanCount),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针灌注,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSamplePrimeWhileReturn(NEEDLE_PRIME_OPT primeOpt,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_PRIME,(int32_t)(primeOpt),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针强力灌注,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleForcePrimeWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_FORCE_PRIME,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针吸液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleAbsorbRackWhileReturn(uint16_t absorbCountMinUL,TUBE_TYPE tubeType,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ABSORB_RACK,(int32_t)(absorbCountMinUL),tubeType,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针反应盘吸液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleAbsorbPlateReactionWhileReturn(uint16_t absorbCountMinUL,float cupLiquidAfterAbsorb,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
        //实际下发为液量乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidAfterAbsorb * 1000);
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ABSORB_PLATE_REACTION,(int32_t)(absorbCountMinUL),(int32_t)(cupLiquidUlDown),0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针注液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleInjectWhileReturn(float cupLiquidUl,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
        //实际下发为液量乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidUl * 1000);
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_INJECT,(int32_t)(cupLiquidUlDown),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针注液带混匀,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleInjectMixWhileReturn(float cupLiquidUl,uint8_t mixCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
        //实际下发为液量乘以1000
    uint32_t cupLiquidUlDown = (uint32_t)(cupLiquidUl * 1000);
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_INJECT_MIX,(int32_t)(cupLiquidUlDown),(int32_t)(mixCount),0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针管路维护,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleMainTainWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_MAINTAIN,(int32_t)cleanOpt,(int32_t)cleanTime,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//样本针校准,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleAdjustWhileReturn(CMD_SOURCE_TEST_FLOW cmdSource,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_ADJUST,(int32_t)(cmdSource),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}


//样本针携带维护,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleInjectWithMaintainWhileReturn(uint8_t maintainCount,uint16_t injectUl,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceNeedleSampleSendCommandWhileAck(IPC_CMD_NEEDLE_SAMPLE_INJECT_WITH_MAINTAIN,maintainCount,injectUl,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceNeedleSampleWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}






















