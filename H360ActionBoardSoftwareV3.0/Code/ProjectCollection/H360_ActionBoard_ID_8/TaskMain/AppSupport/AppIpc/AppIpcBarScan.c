/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-14 15:22:18
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcBarScan.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcBarScan.h"
#include "AppTaskConfig.h"

//IPC调用的数据结构
static DATA_IPC_BAR_SCAN dataIPC_BarScan;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_BarScan;

//指令列表
typedef enum IPC_CMD_BAR_SCAN
{
    IPC_CMD_BAR_SCAN_CHECK_EXIST      = 0X0000,
    IPC_CMD_BAR_SCAN_OPEN             = 0X0001,
    IPC_CMD_BAR_SCAN_CLOSE            = 0X0002,
    IPC_CMD_BAR_SCAN_READ_RECV_DATA   = 0X0003,
}IPC_CMD_BAR_SCAN;

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_BarScanDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_BarScan,
				  (CPU_CHAR*	)"mutexIPC_BarScan",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_BarScan.dataMutexPtr = &mutexIPC_BarScan;
    dataIPC_BarScan.state = STATE_IPC_IDLE;
    dataIPC_BarScan.resultValue = LH_ERR_NONE;
    //内存清空
    UserMemSet(dataIPC_BarScan.barcodeDataBufferPtr,0,DATA_IPC_RAIL_COMPLEX_BARCODE_LENGTH_LIMIT);
    dataIPC_BarScan.barcodeLength = 0;
    return;
}

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_BarScanGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_BarScan,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_BarScan.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_BarScan,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_BarScan.state;   
}

//等待执行完成
LH_ERR IPC_BarScanWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_BarScanGetLastCommandStatusAndResult(&errorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            CoreDelayMinTick();
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

/****************************************************内部向指定任务发送消息API*******************************************************/
static LH_ERR IPC_BarScanSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_BarScan,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_BarScan.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_BarScan.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_BarScan);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskBarScan, //向协议处理线程发送消息
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
        dataIPC_BarScan.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_BarScan,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_BarScan,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_BarScan.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}

/****************************************************发出指令不等待完成*******************************************************/
//不等待完成,检测轨道扫码条码枪是否存在
LH_ERR IPC_BarScanCheckExistWhileAck(void)
{
    return IPC_BarScanSendCommandWhileAck(IPC_CMD_BAR_SCAN_CHECK_EXIST,0,0,0,0,0);
}

//不等待完成,打开条码枪
LH_ERR IPC_BarScanOpenWhileAck(void)
{
    return IPC_BarScanSendCommandWhileAck(IPC_CMD_BAR_SCAN_OPEN,0,0,0,0,0);
}

//不等待完成,关闭条码枪
LH_ERR IPC_BarScanCloseWhileAck(void)
{
    return IPC_BarScanSendCommandWhileAck(IPC_CMD_BAR_SCAN_CLOSE,0,0,0,0,0);
}

//不等待完成,读取接收到的条码数据
LH_ERR IPC_BarScanReadRecvDataWhileAck(void)
{
    return IPC_BarScanSendCommandWhileAck(IPC_CMD_BAR_SCAN_READ_RECV_DATA,0,0,0,0,0);
}

/*****************************************************发出指令等待完成*******************************************************/
//等待完成,检测轨道扫码条码枪是否存在
LH_ERR IPC_BarScanCheckExistWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_BarScanSendCommandWhileAck(IPC_CMD_BAR_SCAN_CHECK_EXIST,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_BarScanWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//等待完成,打开条码枪
LH_ERR IPC_BarScanOpenWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_BarScanSendCommandWhileAck(IPC_CMD_BAR_SCAN_OPEN,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_BarScanWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//等待完成,关闭条码枪
LH_ERR IPC_BarScanCloseWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_BarScanSendCommandWhileAck(IPC_CMD_BAR_SCAN_CLOSE,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_BarScanWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//等待完成,读取接收到的条码数据
LH_ERR IPC_BarScanReadRecvDataWhileReturn(uint8_t** barcodeDataPtr,uint16_t* barCodeDataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_BarScanSendCommandWhileAck(IPC_CMD_BAR_SCAN_READ_RECV_DATA,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_BarScanWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    *barcodeDataPtr = dataIPC_BarScan.barcodeDataBufferPtr;
    *barCodeDataLength = dataIPC_BarScan.barcodeLength;
    //返回结果代码
    return LH_ERR_NONE;
}

/****************************************************获取运行的有效数据*********************************************************/
//获取扫码长度
uint16_t IPC_BarScanGetBarCodeDataLength(void)
{
    return dataIPC_BarScan.barcodeLength;
}

//获取条码数据
void IPC_BarScanGetBarCodeDataPtr(uint8_t** barcodeDataPtr)
{
    *barcodeDataPtr = dataIPC_BarScan.barcodeDataBufferPtr;
    return;
}




































