/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-13 14:01:00
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppTask\AppTaskBarScan.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferAppTaskBarScan[STK_SIZE_APP_TASK_BAR_SCAN]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskBarScan@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_BAR_SCAN_PERIOD        100
//模块定时执行任务初始化
static void AppTaskFuncBarScan_Auto_Init(void);
//模块定时执行任务
static void AppTaskFuncBarScan_Auto_Proc(void);

//模块CAN信息处理任务
static void AppTaskFuncBarScan_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncBarScan_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncBarScan(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //根据扫码枪配置参数不同，初始化函数不同。
    int32_t paramUtil = 0;
    AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_BARCODE_SCANNER_TYPE,&paramUtil);
    if(paramUtil == 0)
    {
        BoardBarScanCM300I_Init();
    }else
    {
        BoardBarScanCR100M_Init();
    }
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    //自动执行任务初始化
    AppTaskFuncBarScan_Auto_Init();
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_BAR_SCAN_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_BAR_SCAN_PERIOD)
		{
			//上次发送时间和本次发送时间差了TIME_MS_AUTO_TASK_BAR_SCAN_PERIOD个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncBarScan_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_BAR_SCAN_PERIOD,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(TASK_CMD_SRC_CAN == taskCommandPtr->cmdSrc)
            {
                //从can来的数据
                CAN_SLAVE_CHANNEL_CACHE* canRecvDatPtr = (CAN_SLAVE_CHANNEL_CACHE*)taskCommandPtr->cmdData;
                //对这个数据进行处理
                AppTaskFuncBarScan_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncBarScan_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}



/*********************************************自身定时任务处理*********************************************/
//模块定时执行任务初始化
static void AppTaskFuncBarScan_Auto_Init(void)
{
    AppImplBarScanAutoInit();
    return;
}
//模块定时执行任务
static void AppTaskFuncBarScan_Auto_Proc(void)
{
    AppImplBarScanAutoPro();
    return;
}



/************************************************CAN总线接收到的消息的处理******************************************************/
//检测轨道扫码条码枪是否存在
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_CheckExist(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//打开条码枪
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_Open(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//关闭条码枪
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_Close(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//读取接收到的条码数据
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_ReadRecvData(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//读取接收到的条码数据
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_ReadRecvDataFromBuffer(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcBarScanSetting[] = {
    {AppTaskFuncBarScan_CanMsg_Proc_CheckExist                     ,           0           ,"CheckExist"                },
    {AppTaskFuncBarScan_CanMsg_Proc_Open                           ,           1           ,"Open"                      },
    {AppTaskFuncBarScan_CanMsg_Proc_Close                          ,           0           ,"Close"                     },
    {AppTaskFuncBarScan_CanMsg_Proc_ReadRecvData                   ,           0           ,"ReadRecvData"              },
    {AppTaskFuncBarScan_CanMsg_Proc_ReadRecvDataFromBuffer         ,           1           ,"ReadRecvDataFromBuffer"    },
};
#define TASK_CAN_MSG_BAR_SCAN_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcBarScanSetting))/(sizeof(AppTaskCanMsgProcBarScanSetting[0])))

//模块CAN信息处理任务
static void AppTaskFuncBarScan_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
{
    LH_ERR errorCode =LH_ERR_NONE;

    //首先,参数长度必须大于4
    if(canMsgCachePtr->currentLength < 4)
    {
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_LENGTH);
        //返回ACK错误
        return;
    }

    //读取指令码,识别指令码是否支持
    uint32_t commandValue = CanBaseReadDataConvertToUint32(canMsgCachePtr->recvCacheArray,0);
    if(commandValue >= TASK_CAN_MSG_BAR_SCAN_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcBarScanSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcBarScanSetting[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcBarScanSetting[commandValue].debugDetailMsg);
    
    //打印结果
    AppShowEndMsg((uint8_t*)AppBarScanDebugEndMsg,errorCode);
    
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//检测轨道扫码条码枪是否存在
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_CheckExist(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //检查条码枪是否存在
    errorCode = AppImplBarScanCheckExist();
    //返回结果
    return errorCode;
}

//打开条码枪
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_Open(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //打开扫码配置
    BARSCAN_OPEN_MODE barscanOpenMode = BARSCAN_OPEN_MODE_ONCE;
    barscanOpenMode = ((BARSCAN_OPEN_MODE)(cachePtrLocal->recvCacheArray[4]));
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)barscanOpenMode),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数打开条码枪
    errorCode = AppImplBarScanOpen(barscanOpenMode);
    //返回结果
    return errorCode;
}

//关闭条码枪
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_Close(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //调用函数关闭条码枪
    errorCode = AppImplBarScanClose();
    //返回结果
    return errorCode;
}

//读取接收到的条码数据
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_ReadRecvData(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //读取序号和想要设置的状态
    uint8_t* barcodeDataBufferPtr = NULL;
    uint16_t barcodeDataLength = 0;
    //先读取数据长度
    AppImplBarScanReadRecvDataLengthFromBuffer(BARSCAN_INDEX_INVALID,&barcodeDataLength);
    //多申请两个字节的内存,用于存放长度
    do
    {
        barcodeDataBufferPtr = UserMemMalloc(SRAM_CCM,barcodeDataLength+2);
        if(barcodeDataBufferPtr == NULL)
        {
            CoreDelayMinTick();
        }
    }while (barcodeDataBufferPtr == NULL);
    
    //如果接收到的数据长度不为0,则申请内存返回数据,若为0,则不需要读取数据
    if(barcodeDataLength != 0)
    {
        //从缓冲区读取接收到的条码数据
        AppImplBarScanReadRecvDataFromBuffer(BARSCAN_INDEX_INVALID,(barcodeDataBufferPtr+2));
    }
    //写入长度
    CanBaseWriteUint16DataToBuffer(barcodeDataBufferPtr,0,barcodeDataLength);
    //返回数据包
    CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_BARCODE_GENERAL,barcodeDataBufferPtr,barcodeDataLength+2);
    //发送完成,释放内存
    if(barcodeDataBufferPtr != NULL)
    {
        UserMemFree(SRAM_CCM,barcodeDataBufferPtr);
    }
    //返回结果
    return errorCode;
}

//读取接收到的条码数据
static LH_ERR AppTaskFuncBarScan_CanMsg_Proc_ReadRecvDataFromBuffer(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    BARSCAN_INDEX barScanIndex = (BARSCAN_INDEX)(cachePtrLocal->recvCacheArray[4]);
    //读取序号和想要设置的状态
    uint8_t* barcodeDataBufferPtr = NULL;
    uint16_t barcodeDataLength = 0;
    //先读取数据长度
    AppImplBarScanReadRecvDataLengthFromBuffer(barScanIndex,&barcodeDataLength);
    //多申请两个字节的内存,用于存放长度
    do
    {
        barcodeDataBufferPtr = UserMemMalloc(SRAM_CCM,barcodeDataLength+2);
        if(barcodeDataBufferPtr == NULL)
        {
            CoreDelayMinTick();
        }
    }while (barcodeDataBufferPtr == NULL);
    
    //如果接收到的数据长度不为0,则申请内存返回数据,若为0,则不需要读取数据
    if(barcodeDataLength != 0)
    {
        //从缓冲区读取接收到的条码数据
        AppImplBarScanReadRecvDataFromBuffer(barScanIndex,(barcodeDataBufferPtr+2));
    }
    //写入长度
    CanBaseWriteUint16DataToBuffer(barcodeDataBufferPtr,0,barcodeDataLength);
    //返回数据包
    CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_BARCODE_GENERAL,barcodeDataBufferPtr,barcodeDataLength+2);
    //发送完成,释放内存
    if(barcodeDataBufferPtr != NULL)
    {
        UserMemFree(SRAM_CCM,barcodeDataBufferPtr);
    }
    //返回结果
    return errorCode;
}

/************************************************自身分层API调用的消息的处理**********************************/
//检测轨道扫码条码枪是否存在
static LH_ERR AppTaskFuncBarScan_Self_Proc_CheckExist(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//打开条码枪
static LH_ERR AppTaskFuncBarScan_Self_Proc_Open(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//关闭条码枪
static LH_ERR AppTaskFuncBarScan_Self_Proc_Close(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//读取接收到的条码数据
static LH_ERR AppTaskFuncBarScan_Self_Proc_ReadRecvData(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcBarScanSetting[] = {
    {AppTaskFuncBarScan_Self_Proc_CheckExist        ,"CheckExist"    },
    {AppTaskFuncBarScan_Self_Proc_Open              ,"Open"          },
    {AppTaskFuncBarScan_Self_Proc_Close             ,"Close"         },
    {AppTaskFuncBarScan_Self_Proc_ReadRecvData      ,"ReadRecvData"  },
};

//自身支持指令数量
#define TASK_SELF_MSG_BAR_SCAN_COMMAND_COUNT  ((sizeof(AppTaskSelfMsgProcBarScanSetting))/(sizeof(AppTaskSelfMsgProcBarScanSetting[0])))


//模块自身转发消息处理任务
static void AppTaskFuncBarScan_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_BAR_SCAN* cmdResultPtr = (DATA_IPC_BAR_SCAN*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_BAR_SCAN_COMMAND_COUNT)
	{
       AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)"Unsupport",
                    selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                    selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                    );
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcBarScanSetting[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                            AppTaskSelfMsgProcBarScanSetting[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //打印结果
    AppShowEndMsg((uint8_t*)AppBarScanDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//检测轨道扫码条码枪是否存在
static LH_ERR AppTaskFuncBarScan_Self_Proc_CheckExist(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //调用函数
    errCode = AppImplBarScanCheckExist();
    return errCode;
}

//打开条码枪
static LH_ERR AppTaskFuncBarScan_Self_Proc_Open(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //调用函数
    errCode = AppImplBarScanOpen(BARSCAN_OPEN_MODE_ONCE);
    return errCode;
}

//关闭条码枪
static LH_ERR AppTaskFuncBarScan_Self_Proc_Close(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //调用函数
    errCode = AppImplBarScanClose();
    return errCode;
}

//读取接收到的条码数据
static LH_ERR AppTaskFuncBarScan_Self_Proc_ReadRecvData(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppBarScanDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    DATA_IPC_BAR_SCAN* cmdResultPtr = (DATA_IPC_BAR_SCAN*)(selfCmdPtr->commandDataSpecialChannel);
    BARSCAN_INDEX barScanIndex = (BARSCAN_INDEX)(selfCmdPtr->commandParam1);
    uint8_t* barcodeDataBufferPtr = NULL;
    uint16_t barcodeLength = 0;
    //先读取数据长度
    AppImplBarScanReadRecvDataLengthFromBuffer(barScanIndex,&barcodeLength);
    //清空接收缓存
    UserMemSet(cmdResultPtr->barcodeDataBufferPtr,0,DATA_IPC_RAIL_COMPLEX_BARCODE_LENGTH_LIMIT);

    barcodeDataBufferPtr = cmdResultPtr->barcodeDataBufferPtr;
    cmdResultPtr->barcodeLength = barcodeLength;
    //拷贝数据
    AppImplBarScanReadRecvDataFromBuffer(barScanIndex,barcodeDataBufferPtr);
    return errCode;
}
