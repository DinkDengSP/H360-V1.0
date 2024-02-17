/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-09-03 09:56:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-15 16:14:31
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"
#include "ServiceImplBoardIap.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskBoardIap[STK_SIZE_TASK_BOARD_IAP]@".ccram";
#pragma pack()
//任务控制块
OS_TCB tcbTaskBoardIap@".ccram";

//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncBoardIap_CanMsg_Proc(CAN1_CMD_RECV* can1CommandPtr);
uint8_t bootLoaderNeedResp = 0;

//任务函数
void ServiceTaskFuncBoardIap(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //延时等待接收函数初始化成功
    CoreDelayMs(100);
    //初始化升级结构体
    ServiceImplBoardIapInit();
    //发送升级数据反馈
    if(bootLoaderNeedResp == 0XFF)
    {
        //如果是主动切入BootLoader,那么需要回传BootLoader已经准备好标签
        Can1ProtocolSendSoftwareUpdatePrepareWithSelf(CAN1_SELF_BOARD_ID,1);
    }
    else if(bootLoaderNeedResp == 0XFE)
    {
        bootLoaderNeedResp = 0;
        //回复App-->Boot结果
        Can1ProtocolSendProgramJumpWithSelf(CAN1_SELF_BOARD_ID,1,0);
    }
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
                CAN1_CMD_RECV* can1RecvDatPtr = (CAN1_CMD_RECV*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncBoardIap_CanMsg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(SRAM_CCM,can1RecvDatPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            taskCommandPtr->cmdDataPtr = NULL;
            UserMemFree(SRAM_CCM,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}



/*************************************************1. 来自CAN1的消息******************************************/
//读取软件版本
void ServiceTaskFuncBoardIap_Can1Msg_Proc_ReadSoftVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//读取Boot版本
void ServiceTaskFuncBoardIap_Can1Msg_Proc_ReadBootVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//升级准备
void ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdatePrepare(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//升级起始
void ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdataStart(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//升级数据
void ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdataData(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//升级数据发送完成
void ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdataEnd(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//程序跳转
void ServiceTaskFuncBoardIap_Can1Msg_Proc_ProgramJump(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT BoardIapCan1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_TEST_FLOW_CMD_CELL_VERSION            ,ServiceTaskFuncBoardIap_Can1Msg_Proc_ReadSoftVersion  ,"ReadSoftVersion"  },
    {CAN1_CMD_TEST_FLOW_CMD_CELL_BOOT_VERSION       ,ServiceTaskFuncBoardIap_Can1Msg_Proc_ReadBootVersion  ,"ReadBootVersion"  },
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_PREPARE     ,ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdatePrepare    ,"UpdatePrepare"    },
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_START       ,ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdataStart      ,"UpdataStart"      },
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_DATA        ,ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdataData       ,"UpdataData"       },
    {CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_END         ,ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdataEnd        ,"UpdataEnd"        },
    {CAN1_CMD_TEST_FLOW_CMD_CELL_PROGRAM_JUMP       ,ServiceTaskFuncBoardIap_Can1Msg_Proc_ProgramJump      ,"ProgramJump"      },
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_BOARD_IAP_CMD_COUNT        (sizeof(BoardIapCan1CommandProcFuncPtrArray)/sizeof(BoardIapCan1CommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncBoardIap_CanMsg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_BOARD_IAP_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == BoardIapCan1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(BoardIapCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                BoardIapCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,BoardIapCan1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}


//读取软件版本
void ServiceTaskFuncBoardIap_Can1Msg_Proc_ReadSoftVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //版本号讯息
    BOARD_APP_VERSION  appVersion;
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncIap:  ",detailDebugMsgPtr,0,0,0,0,0);
    //读取本机APP版本
    BoardVersion_ReadAppVersion(&appVersion);
    //回传主控应用版本
    Can1ProtocolSendAppVersionWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,appVersion.mainVersion,appVersion.subVersion,appVersion.debugVersion,appVersion.svnVersion);
}

//读取Boot版本
void ServiceTaskFuncBoardIap_Can1Msg_Proc_ReadBootVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //版本号讯息
    BOARD_BOOT_VERSION  bootVersion;
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncIap:  ",detailDebugMsgPtr,0,0,0,0,0);
    //读取本机boot版本
    BoardVersion_ReadBootVersion(&bootVersion);
    //回传主控Boot版本
    Can1ProtocolSendBootVersionWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,bootVersion.mainVersion,bootVersion.subVersion,bootVersion.debugVersion,bootVersion.svnVersion);
}

//升级准备
void ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdatePrepare(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncIap:  ",detailDebugMsgPtr,boardID,0,0,0,0);
    //如果模块号,板号匹配
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //回传升级准备好
        Can1ProtocolSendSoftwareUpdatePrepareWithSelf(CAN1_SELF_BOARD_ID,1);
    }
    else
    {
        //BOOT不支持下位机和其他板卡的升级
        Can1ProtocolSendSoftwareUpdatePrepareWithSelf(CAN1_SELF_BOARD_ID,0);
    }
}

//升级起始
void ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdataStart(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    //程序字节总长度
    uint32_t appDataLengthBytes = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->recvDataBuffer,1);
    //程序数据总包数
    uint16_t appDataPackCountSet = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,5);
    //程序数据每一包的大小
    uint16_t appDataSinglePackLengthByte = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,7);
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncIap:  ",detailDebugMsgPtr,boardID,appDataLengthBytes,appDataPackCountSet,appDataSinglePackLengthByte,0);
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //将下发的包数量和数据长度记录下来
        ServiceImplBoardIapWriteStartMsg(boardID,appDataLengthBytes,appDataPackCountSet,appDataSinglePackLengthByte);
        //数据处理完成,回传数据处理完成
        Can1ProtocolSendIapStartResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,1);
    }
    else
    {
        //Boot不支持下位机和其他板卡的升级
        Can1ProtocolSendIapStartResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
    }
}

//升级数据
void ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdataData(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    BOARD_IAP_STATE iapState;
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    //程序数据总包数
    uint16_t appDataPackIndex = Can1ProcUtilConvertArrayToUint16(can1CommandPtr->recvDataBuffer,1);
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncIap:  ",detailDebugMsgPtr,boardID,appDataPackIndex,can1CommandPtr->recvDataLength,0,0);
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //本机的升级代码
        iapState = ServiceImplBoardIapWriteData(boardID,appDataPackIndex,can1CommandPtr->recvDataLength -3,can1CommandPtr->recvDataBuffer+3);
        //数据处理完成,回传数据处理完成
        if(iapState == BOARD_IAP_ERR)
        {
            Can1ProtocolSendIapDataTransResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
            SystemDebugShowMsgError(iapState);
        }
        else
        {
            Can1ProtocolSendIapDataTransResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,1);
        }
    }
    else
    {
        //BOOT不支持下位机和其他板卡的升级
        Can1ProtocolSendIapDataTransResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
    }
}

//升级数据发送完成
void ServiceTaskFuncBoardIap_Can1Msg_Proc_UpdataEnd(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    BOARD_IAP_STATE iapState;
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    //程序字节总长度
    uint32_t crcResult = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->recvDataBuffer,1);
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncIap:  ",detailDebugMsgPtr,boardID,crcResult,0,0,0);
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //本机的升级代码
        iapState = ServiceImplBoardIapCalcCRC32(boardID,crcResult);
        if(iapState == BOARD_IAP_ERR)
        {
            //数据处理完成,回传完成
            Can1ProtocolSendIapEndResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
            SystemDebugShowMsgError(iapState);
            return;
        }
        //开始升级
        iapState = ServiceImplBoardIapStartUpdata(boardID);
        if(iapState == BOARD_IAP_ERR)
        {
            //数据处理完成,回传完成
            Can1ProtocolSendIapEndResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
            SystemDebugShowMsgError(iapState);
            return;
        }
        //数据处理完成,回传完成
        Can1ProtocolSendIapEndResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,1);
        //延时等待数据发出去
        CoreDelayMs(100);
        //这里跳转到APP
        BoardIAP_Jump2Application();
    }
    else
    {
        //BOOT不支持下位机和其他板卡的升级
        Can1ProtocolSendIapEndResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0);
    }
}

//程序跳转
void ServiceTaskFuncBoardIap_Can1Msg_Proc_ProgramJump(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //解析出板号,以及相关的数据信息
    uint8_t boardID = can1CommandPtr->recvDataBuffer[0];
    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncIap:  ",detailDebugMsgPtr,boardID,0,0,0,0);
    if(boardID == CAN1_SELF_BOARD_ID)
    {
        //写入跳转标志
        BoardIAP_WriteJumpFlag();
        //延时等待数据发出去
        CoreDelayMs(3000);
        //这里跳转到APP
        BoardIAP_Jump2Application();
    }
    else
    {
        //BOOT不支持下位机和其他板卡的跳转
        Can1ProtocolSendProgramJumpResultWithCommand(can1CommandPtr,CAN1_SELF_BOARD_ID,0,0);
    }
}
