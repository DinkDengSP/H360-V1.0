/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-31 13:28:40
**LastEditors: DengXiaoJun
**LastEditTime: 2021-01-13 14:07:33
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"
#include "SystemMode.h"
//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskHeart[STK_SIZE_TASK_HEART];
#pragma pack()

//启动任务的堆栈
OS_TCB tcbTaskHeart;

//自动运行程序的执行周期,单位毫秒
#define HEART_AUTO_FUNC_RUNNING_PERIOD_MS            500

//系统服务任务接收几个方面的消息
//1. 来自CAN的消息
static void ServiceTaskFuncHeart_CanMsg_Proc(CAN_CMD_RECV* can1CommandPtr);
//2. 来自UART的消息
static void ServiceTaskFuncHeart_UartMsg_Proc(UART_CMD_UTIL *uartCmdUtilPtr);



uint8_t uartCmd = 0;
uint8_t uartDat = 0;
void TaskFuncHeart(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    uint8_t count = 0;
    //设置系统状态
    SytemModeInit(SYSTEM_MODE_TYPE_APP);
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    while(1)
    {
       //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)HEART_AUTO_FUNC_RUNNING_PERIOD_MS,
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
                ServiceTaskFuncHeart_CanMsg_Proc(canRecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_MAIN,canRecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_UART == taskCommandPtr->cmdSrc)
            {
                //从Uart传的指令
                UART_CMD_UTIL *uartRecvDatPtr = (UART_CMD_UTIL*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncHeart_UartMsg_Proc(uartRecvDatPtr);
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
        else
        {
            BoardLedToogle(BOARD_LED_GREEN);
            if(++count > 10)
            {
                count = 0;
                SystemPrintf("MEMUSED: %d.\r\n",UserMemPerUsed(MEM_MAIN));
            }

        }
    }
}


//运行时软件急停
void ServiceTaskFuncHeart_CanMsg_Proc_EmergencyStop(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr);
//整机复位
void ServiceTaskFuncHeart_CanMsg_Proc_MachineReset(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr);
//开始升级,升级信息    
void ServiceTaskFuncHeart_CanMsg_Proc_UpdataStart(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr);
//升级数据     
void ServiceTaskFuncHeart_CanMsg_Proc_UpdataData(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr);
//升级结束，程序CRC校验码     
void ServiceTaskFuncHeart_CanMsg_Proc_UpdataEnd(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr);
//升级准备，跳转Boot等      
void ServiceTaskFuncHeart_CanMsg_Proc_UpdataParpare(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr);
//读取版本号
void ServiceTaskFuncHeart_CanMsg_Proc_Version(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr);
//读取Boot版本号      
void ServiceTaskFuncHeart_CanMsg_Proc_BootVersion(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr);



//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT HeartCanCommandProcFuncPtrArray[] = {
    {CAN_CMD_SOFT_EMERGENCY_STOP    ,ServiceTaskFuncHeart_CanMsg_Proc_EmergencyStop     ,"EmergencyStop"    },//运行时软件急停
    {CAN_CMD_MACHINE_RESET          ,ServiceTaskFuncHeart_CanMsg_Proc_MachineReset      ,"MachineReset"     },//整机复位
    {CAN_CMD_CELL_UPDATA_START      ,ServiceTaskFuncHeart_CanMsg_Proc_UpdataStart       ,"UpdataStart"      },//开始升级,升级信息
    {CAN_CMD_CELL_UPDATA_DATA       ,ServiceTaskFuncHeart_CanMsg_Proc_UpdataData        ,"UpdataData"       },//升级数据
    {CAN_CMD_CELL_UPDATA_END        ,ServiceTaskFuncHeart_CanMsg_Proc_UpdataEnd         ,"UpdataEnd"        },//升级结束，程序CRC校验码
    {CAN_CMD_CELL_UPDATA_PREPARE    ,ServiceTaskFuncHeart_CanMsg_Proc_UpdataParpare     ,"UpdataParpare"    },//升级准备，跳转Boot等
    {CAN_CMD_CELL_VERSION           ,ServiceTaskFuncHeart_CanMsg_Proc_Version           ,"Version"          },//读取版本号
    {CAN_CMD_CELL_BOOT_VERSION      ,ServiceTaskFuncHeart_CanMsg_Proc_BootVersion       ,"BootVersion"      },//读取Boot版本号
};
//CAN指令支持的数量
#define TASK_CAN_MSG_HEART_CMD_COUNT        (sizeof(HeartCanCommandProcFuncPtrArray)/sizeof(HeartCanCommandProcFuncPtrArray[0]))


//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncHeart_CanMsg_Proc(CAN_CMD_RECV* canCommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN_MSG_HEART_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(canCommandPtr->command == HeartCanCommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(HeartCanCommandProcFuncPtrArray[indexUtil].canCmdProcFuncPtr != NULL)
            {
                //调用函数处理
                HeartCanCommandProcFuncPtrArray[indexUtil].canCmdProcFuncPtr(canCommandPtr,HeartCanCommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//运行时软件急停
void ServiceTaskFuncHeart_CanMsg_Proc_EmergencyStop(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 

//整机复位
void ServiceTaskFuncHeart_CanMsg_Proc_MachineReset(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 

//开始升级,升级信息    
void ServiceTaskFuncHeart_CanMsg_Proc_UpdataStart(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 
 
//升级数据     
void ServiceTaskFuncHeart_CanMsg_Proc_UpdataData(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 
  
//升级结束，程序CRC校验码     
void ServiceTaskFuncHeart_CanMsg_Proc_UpdataEnd(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 
  
//升级准备，跳转Boot等      
void ServiceTaskFuncHeart_CanMsg_Proc_UpdataParpare(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 
    
//读取版本号
void ServiceTaskFuncHeart_CanMsg_Proc_Version(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 
    
//读取Boot版本号      
void ServiceTaskFuncHeart_CanMsg_Proc_BootVersion(CAN_CMD_RECV* canCommandPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 

//跳转到APP
void ServiceTaskFuncHeart_UartMsg_Proc_JumpApp(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//跳转到Boot
void ServiceTaskFuncHeart_UartMsg_Proc_JumpBoot(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//升级开始
void ServiceTaskFuncHeart_UartMsg_Proc_UpdateStart(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//升级数据
void ServiceTaskFuncHeart_UartMsg_Proc_UpdateData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//升级结束
void ServiceTaskFuncHeart_UartMsg_Proc_UpdateEnd(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//读系统状态
void ServiceTaskFuncHeart_UartMsg_Proc_ReadSystemMode(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//读软件版本
void ServiceTaskFuncHeart_UartMsg_Proc_ReadSoftVersion(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr);
//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_UART_MSG_PROC_UNIT HeartUartCommandProcFuncPtrArray[] = {
    {UART_CMD_JUMP_APP              ,ServiceTaskFuncHeart_UartMsg_Proc_JumpApp          ,"JumpApp"          },//跳转到APP
    {UART_CMD_JUMP_BOOT             ,ServiceTaskFuncHeart_UartMsg_Proc_JumpBoot         ,"JumpBoot"         },//跳转到Boot
    {UART_CMD_UPDATE_START          ,ServiceTaskFuncHeart_UartMsg_Proc_UpdateStart      ,"UpdateStart"      },//升级开始
    {UART_CMD_UPDATE_DATA           ,ServiceTaskFuncHeart_UartMsg_Proc_UpdateData       ,"UpdateData"       },//升级数据
    {UART_CMD_UPDATE_END            ,ServiceTaskFuncHeart_UartMsg_Proc_UpdateEnd        ,"UpdateEnd"        },//升级数据
    {UART_CMD_READ_SYSTEM_MODE      ,ServiceTaskFuncHeart_UartMsg_Proc_ReadSystemMode   ,"ReadSystemMode"   },//读系统状态
    {UART_CMD_READ_SOFT_VERSION     ,ServiceTaskFuncHeart_UartMsg_Proc_ReadSoftVersion  ,"ReadSoftVersion"  },//读软件版本
};
//Uart指令支持的数量
#define TASK_UART_MSG_HEART_CMD_COUNT        (sizeof(HeartUartCommandProcFuncPtrArray)/sizeof(HeartUartCommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncHeart_UartMsg_Proc(UART_CMD_UTIL *uartCmdUtilPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_UART_MSG_HEART_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(uartCmdUtilPtr->funcCode == HeartUartCommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(HeartUartCommandProcFuncPtrArray[indexUtil].uartCmdProcFuncPtr != NULL)
            {
                //调用函数处理
                HeartUartCommandProcFuncPtrArray[indexUtil].uartCmdProcFuncPtr(uartCmdUtilPtr,HeartUartCommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//跳转到APP
void ServiceTaskFuncHeart_UartMsg_Proc_JumpApp(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    //任务开始
    AppShowStartMsg(AppHeartDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //返回数据
    Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,LH_ERR_NONE,0,NULL);
    //任务结束
    AppShowEndMsg(AppHeartDebugEndMsg,LH_ERR_NONE);
} 

//跳转到Boot
void ServiceTaskFuncHeart_UartMsg_Proc_JumpBoot(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    //任务开始
    AppShowStartMsg(AppHeartDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //写入升级标志
    BoardIapWriteUpdateFlag();
    //写入返回数据
    Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,LH_ERR_NONE,0,NULL);
    //延时等待
    CoreDelayMs(500);
    //跳转到boot
    BoardIapJumpBoot();
    //任务结束
    AppShowEndMsg(AppHeartDebugEndMsg,LH_ERR_NONE);
} 

//到开始升级
void ServiceTaskFuncHeart_UartMsg_Proc_UpdateStart(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 

//升级数据
void ServiceTaskFuncHeart_UartMsg_Proc_UpdateData(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 

//升级结束
void ServiceTaskFuncHeart_UartMsg_Proc_UpdateEnd(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    asm("nop");
} 

//读系统状态
void ServiceTaskFuncHeart_UartMsg_Proc_ReadSystemMode(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    SYSTEM_MODE_TYPE sysMode = SYSTEM_MODE_TYPE_BOOT;
    //任务开始
    AppShowStartMsg(AppHeartDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //读取系统状态
    sysMode = SytemModeRead();
    //写入返回数据
    Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,LH_ERR_NONE,1,(uint8_t *)(&sysMode));
    //任务结束
    AppShowEndMsg(AppHeartDebugEndMsg,LH_ERR_NONE);
} 

//读软件版本
void ServiceTaskFuncHeart_UartMsg_Proc_ReadSoftVersion(UART_CMD_UTIL* uartCmdUtilPtr,uint8_t* detailDebugMsgPtr)
{
    //软件版本
    uint32_t boardVersion = 0;
    uint32_t svnVersion = 0;
    //任务开始
    AppShowStartMsg(AppHeartDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //读取软件版本
    SystemReadSoftVersion(&boardVersion,&svnVersion);
    //数据赋值
    uint8_t returnbuff[8];
    //数据赋值
    ConvertUInt32ToArrayHighFirst(returnbuff,0,boardVersion);
    ConvertUInt32ToArrayHighFirst(returnbuff,4,svnVersion);
    //写入返回数据
    Uart1CmdSendAckPack(uartCmdUtilPtr->funcCode,LH_ERR_NONE,8,returnbuff);
    //任务结束
    AppShowEndMsg(AppHeartDebugEndMsg,LH_ERR_NONE);
} 