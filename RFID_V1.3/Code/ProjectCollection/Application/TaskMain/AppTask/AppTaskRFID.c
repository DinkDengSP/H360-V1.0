#include "AppTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskRfid[STK_SIZE_TASK_RFID];
#pragma pack()

//启动任务的堆栈
OS_TCB tcbTaskRfid;
//系统服务任务接收几个方面的消息
//1. 来自CAN的消息
static void ServiceTaskFuncRfid_CanMsg_Proc(CAN_CMD_RECV* can1CommandPtr);
//2. 来自UART的消息
static void ServiceTaskFuncRfid_UartMsg_Proc(UART_CMD_UTIL *uartCmdUtilPtr);

//任务函数
void TaskFuncRfid(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    while(1)
    {
       //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)0,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(SYSTEM_CMD_SRC_CAN == taskCommandPtr->cmdSrc)
            {
                //从CAN1传过来的指令
                CAN_CMD_RECV* canRecvDatPtr = (CAN_CMD_RECV*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncRfid_CanMsg_Proc(canRecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_MAIN,canRecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_UART == taskCommandPtr->cmdSrc)
            {
                //从Uart传的指令
                UART_CMD_UTIL *uartRecvDatPtr = (UART_CMD_UTIL*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncRfid_UartMsg_Proc(uartRecvDatPtr);
                if(uartRecvDatPtr->cmdDat != NULL)
                {
                    UserMemFree(MEM_MAIN,uartRecvDatPtr->cmdDat);
                }
                //处置完成,执行后续操作
                UserMemFree(MEM_MAIN,uartRecvDatPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            taskCommandPtr->cmdDataPtr = NULL;
            UserMemFree(MEM_MAIN,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}

//1. 来自CAN的消息
static void ServiceTaskFuncRfid_CanMsg_Proc(CAN_CMD_RECV* can1CommandPtr)
{
    asm("nop");
}



//读扇区
void ServiceTaskFuncRfid_UartMsg_Proc_ReadSection(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//写扇区
void ServiceTaskFuncRfid_UartMsg_Proc_WriteSection(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//读试剂信息
void ServiceTaskFuncRfid_UartMsg_Proc_ReadReagentData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//写试剂信息
void ServiceTaskFuncRfid_UartMsg_Proc_WriteReagentData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//读辅助信息
void ServiceTaskFuncRfid_UartMsg_Proc_ReadAssistData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//写辅助信息
void ServiceTaskFuncRfid_UartMsg_Proc_WriteAssisData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//读取RFID卡片ID
void ServiceTaskFuncRfid_UartMsg_Proc_ReadRFIDCardId(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);

//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_UART_MSG_PROC_UNIT RfidUartCommandProcFuncPtrArray[] = {
    {UART_CMD_READ_SECTION                  ,ServiceTaskFuncRfid_UartMsg_Proc_ReadSection       ,"ReadSection"       },//读扇区
    {UART_CMD_WRITE_SECTION                 ,ServiceTaskFuncRfid_UartMsg_Proc_WriteSection       ,"WriteSection"       },//写扇区
    {UART_CMD_READ_REAGENT_DATA             ,ServiceTaskFuncRfid_UartMsg_Proc_ReadReagentData   ,"ReadReagentData"   },//读试剂信息
    {UART_CMD_WRITE_REAGENT_DATA            ,ServiceTaskFuncRfid_UartMsg_Proc_WriteReagentData ,"WriteReagentData"  },//写试剂信息
    {UART_CMD_READ_ASSIST_REAGENT_DATA      ,ServiceTaskFuncRfid_UartMsg_Proc_ReadAssistData   ,"ReadAssistData"    },//读辅助信息
    {UART_CMD_WRITE_ASSIST_REAGENT_DATA     ,ServiceTaskFuncRfid_UartMsg_Proc_WriteAssisData   ,"WriteAssisData"     },//写辅助信息
    {UART_CMD_READ_RFID_CARD_ID             ,ServiceTaskFuncRfid_UartMsg_Proc_ReadRFIDCardId   ,"ReadRFIDCardId"     },//读取RFID卡片ID
};
//Uart指令支持的数量
#define TASK_UART_MSG_RFID_CMD_COUNT        (sizeof(RfidUartCommandProcFuncPtrArray)/sizeof(RfidUartCommandProcFuncPtrArray[0]))

//2. 来自UART的消息
static void ServiceTaskFuncRfid_UartMsg_Proc(UART_CMD_UTIL *uartCmdUtilPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_UART_MSG_RFID_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(uartCmdUtilPtr->funcCode == RfidUartCommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(RfidUartCommandProcFuncPtrArray[indexUtil].uartCmdProcFuncPtr != NULL)
            {
                //调用函数处理
                RfidUartCommandProcFuncPtrArray[indexUtil].uartCmdProcFuncPtr(uartCmdUtilPtr,RfidUartCommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//读扇区
void ServiceTaskFuncRfid_UartMsg_Proc_ReadSection(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //变量初始化
    RFID_SECTION_NUM sectionNo = RFID_SECTION_NUM_1;
    RFID_BLOCK_NUM blockNo = RFID_BLOCK_NUM_FIRSET;
    uint8_t *datBuff = NULL;
    uint16_t datLen = 0;
    
    //参数赋值
    sectionNo = (RFID_SECTION_NUM)(uartCmdUtilPtr->cmdDat[0]);
    blockNo = (RFID_BLOCK_NUM)(uartCmdUtilPtr->cmdDat[1]);
    //任务开始
    AppShowStartMsg(AppRfidDebugStartMsg,detailDebugMsgPtr,sectionNo,blockNo,0,0,0);
    //实际调用
    errorCode = ServiceImplRfid_ReadSection(sectionNo,blockNo,&datLen,&datBuff);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
    }
    if(datBuff == NULL)
    {
        //返回错误代码
        Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,errorCode,0,NULL);
    }
    else
    {
        //返回数据,
        Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,errorCode,datLen,datBuff);
        //释放内存空间
        UserMemFree(MEM_MAIN,datBuff);
    }
    //任务结束
    AppShowEndMsg(AppRfidDebugEndMsg,errorCode);
}

//写扇区
void ServiceTaskFuncRfid_UartMsg_Proc_WriteSection(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //变量初始化
    RFID_SECTION_NUM sectionNo = RFID_SECTION_NUM_1;
    RFID_BLOCK_NUM blockNo = RFID_BLOCK_NUM_FIRSET;
    uint8_t *datBuff = NULL;
    //参数赋值
    sectionNo = (RFID_SECTION_NUM)(uartCmdUtilPtr->cmdDat[0]);
    blockNo = (RFID_BLOCK_NUM)(uartCmdUtilPtr->cmdDat[1]);
    uint16_t datLen = uartCmdUtilPtr->cmdDatLen - 2;
    datBuff = &(uartCmdUtilPtr->cmdDat[2]);
    //任务开始
    AppShowStartMsg(AppRfidDebugStartMsg,detailDebugMsgPtr,sectionNo,blockNo,datLen,0,0);
    //实际调用
    errorCode = ServiceImplRfid_WriteSection(sectionNo,blockNo,datLen,datBuff);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
    }
    //返回错误代码
    Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,errorCode,0,NULL);
    //任务结束
    AppShowEndMsg(AppRfidDebugEndMsg,errorCode);
}

//读试剂信息
void ServiceTaskFuncRfid_UartMsg_Proc_ReadReagentData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{   
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *datBuff = NULL;
    uint16_t datLen = 0;
    uint8_t code = uartCmdUtilPtr->funcCode;
   
    //任务开始
    AppShowStartMsg(AppRfidDebugStartMsg,detailDebugMsgPtr,code,0,0,0,0);
    //实际调用
    errorCode = ServiceImplRfid_ReadReagentData(&datLen,&datBuff);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
    }
    if(datBuff == NULL)
    {
        //返回错误代码
        Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,errorCode,0,NULL);
    }
    else
    {  
      Uart1CmdSendAckPack(code,errorCode,datLen,datBuff);
      //释放内存空间
      UserMemFree(MEM_MAIN,datBuff-1);
    }
    //任务结束
    AppShowEndMsg(AppRfidDebugEndMsg,errorCode);
}

//写试剂信息
void ServiceTaskFuncRfid_UartMsg_Proc_WriteReagentData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *datBuff = NULL;
    uint16_t datLen = 0;
    datLen = uartCmdUtilPtr->cmdDatLen;
    datBuff = uartCmdUtilPtr->cmdDat;
    //任务开始
    AppShowStartMsg(AppRfidDebugStartMsg,detailDebugMsgPtr,datLen,0,0,0,0);
    //实际调用
    errorCode = ServiceImplRfid_WriteReagentData(datLen,datBuff);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
    }
    //返回错误代码
    Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,errorCode,0,NULL);
    //任务结束
    AppShowEndMsg(AppRfidDebugEndMsg,errorCode);
}

//读辅助信息
void ServiceTaskFuncRfid_UartMsg_Proc_ReadAssistData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *datBuff = NULL;
    uint16_t datLen = 0;
    uint8_t assistNo = 0;

    //参数获取
    assistNo = uartCmdUtilPtr->cmdDat[0];
    //任务开始
    AppShowStartMsg(AppRfidDebugStartMsg,detailDebugMsgPtr,assistNo,0,0,0,0);
    //实际调用
    errorCode = ServiceImplRfid_ReadAssistData(assistNo,&datLen,&datBuff);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
    }
    if(datBuff == NULL)
    {
        //返回错误代码
        Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,errorCode,0,NULL);
    }
    else
    {
        //返回数据,
     
        Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,errorCode,datLen,datBuff);
        //释放内存空间
        UserMemFree(MEM_MAIN,datBuff);
    }
    //任务结束
    AppShowEndMsg(AppRfidDebugEndMsg,errorCode);
}

//写辅助信息
void ServiceTaskFuncRfid_UartMsg_Proc_WriteAssisData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *datBuff = NULL;
    uint16_t datLen = 0;
    uint8_t assistNo = 0;  

    //参数获取
    assistNo = uartCmdUtilPtr->cmdDat[0];
    datLen = uartCmdUtilPtr->cmdDatLen - 1;
    datBuff = &(uartCmdUtilPtr->cmdDat[1]);
    //任务开始
    AppShowStartMsg(AppRfidDebugStartMsg,detailDebugMsgPtr,assistNo,datLen,0,0,0);
    //实际调用
    errorCode = ServiceImplRfid_WriteAssistData(assistNo,datLen,datBuff);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
    }
    //返回错误代码
    Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,errorCode,0,NULL);
    //任务结束
    AppShowEndMsg(AppRfidDebugEndMsg,errorCode);
}

//读取RFID卡片ID
void ServiceTaskFuncRfid_UartMsg_Proc_ReadRFIDCardId(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint32_t CardId;
    //参数获取
    //任务开始
    AppShowStartMsg(AppRfidDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //实际调用
    errorCode = ServiceImplRfid_ReadRFIDCardId(&CardId);
    if(errorCode != LH_ERR_NONE)
    {
        //返回错误代码
        Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,errorCode,0,NULL);
        AppShowErrMsg(errorCode);
    }
    else
    {
        uint8_t databuff[4] = {0};
        ConvertUint32ToArrayLowFirst(databuff,0,CardId);
        //返回卡号
        Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,LH_ERR_NONE,4,databuff);
    }
    //任务结束
    AppShowEndMsg(AppRfidDebugEndMsg,errorCode);
}






