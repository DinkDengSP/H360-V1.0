/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:23:23
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-14 15:08:42
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplBarScan.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplBarScan.h"
//自动运行任务的周期,单位毫秒
#define BAR_SCAN_EVENT_PERIOD_TIME_MS          200
//自动运行任务的触发次数,决定是否执行任务
static uint32_t countBarScanEventTrigCount = 0;

//只需要执行的事件回调函数,不需要计算时间
static void ServiceImplBarScan_EventJustRun(void)
{
    asm("nop");
}

//自动循环事件初始化
void ServiceImplBarScan_EventInit(void)
{
    countBarScanEventTrigCount = 0;
}
//自动循环事件
void ServiceImplBarScan_EventRun(uint32_t periodTimeMs)
{
    if(periodTimeMs == 0)
    {
        return;
    }
    //心跳时间计时器增加
    countBarScanEventTrigCount++;
    //如果执行周期大于事件周期,立即运行,如果计数器事件次数大于事件次数,也执行
    if((periodTimeMs >= BAR_SCAN_EVENT_PERIOD_TIME_MS)||(countBarScanEventTrigCount >= (uint32_t)(BAR_SCAN_EVENT_PERIOD_TIME_MS/periodTimeMs)))
    {
        countBarScanEventTrigCount = 0;
        //执行一次自动循环任务
        ServiceImplBarScan_EventJustRun();
    }
}


/*************************************轨道主控自身使用的指令*****************************************/
//检测条码枪是否存在
LH_ERR ServiceImplRailBarScanCheckExist(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查是否在线
        errorCode = Can2SubRailBarScanCheckExistWhileReturn();
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BARSCAN_NO_RESP;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return errorCode;
}

//打开条码枪
LH_ERR ServiceImplRailBarScanOpen(BARSCAN_OPEN_MODE openMode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //打开条码枪
        errorCode = Can2SubRailBarScanOpenWhileReturn(openMode);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BARSCAN_OPEN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return errorCode;
}

//关闭条码枪
LH_ERR ServiceImplRailBarScanClose(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //关闭条码枪
        errorCode = Can2SubRailBarScanCloseWhileReturn();
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BARSCAN_CLOSE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return errorCode;
}

//读取条码枪数据
LH_ERR ServiceImplRailBarScanReadData(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //临时存储
        uint16_t barCodeDataLength = 0;
        uint8_t* barScanSrcData = NULL;
    //申请内存
        do
        {
            barScanSrcData= UserMemMalloc(MEM_SRAM,BAR_SCAN_MAX_DATA_LENGTH);
            if(barScanSrcData == NULL)
            {
                CoreDelayMs(5);
            }
        }while(barScanSrcData == NULL);
    //读取条码枪讯息
        errorCode = Can2SubRailBarScanReadDataWhileReturn(&barCodeDataLength,barScanSrcData);
        if(errorCode != LH_ERR_NONE)
        {
            UserMemFree(MEM_SRAM,barScanSrcData);
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BARSCAN_READ;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //数据写入,先清除上次的数据
        ServiceStateRailBarScanClearLastResult();
    //然后根据长度写入
        ServiceStateRailBarScanWriteData(barCodeDataLength,barScanSrcData);
    //释放内存
        UserMemFree(MEM_SRAM,barScanSrcData);
    return LH_ERR_NONE;
}

//读取条码枪数据
LH_ERR ServiceImplRailBarScanReadDataFromBuffer(BARSCAN_INDEX barScanIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //临时存储
        uint16_t barCodeDataLength = 0;
        uint8_t* barScanSrcData = NULL;
    //申请内存
        do
        {
            barScanSrcData= UserMemMalloc(MEM_SRAM,BAR_SCAN_MAX_DATA_LENGTH);
            if(barScanSrcData == NULL)
            {
                CoreDelayMs(5);
            }
        }while(barScanSrcData == NULL);
    //读取条码枪讯息
        errorCode = Can2SubRailBarScanReadDataFromBufferWhileReturn(barScanIndex,&barCodeDataLength,barScanSrcData);
        if(errorCode != LH_ERR_NONE)
        {
            UserMemFree(MEM_SRAM,barScanSrcData);
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_BARSCAN_READ;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //数据写入,先清除上次的数据
        ServiceStateRailBarScanClearLastResult();
    //然后根据长度写入
        ServiceStateRailBarScanWriteData(barCodeDataLength,barScanSrcData);
    //释放内存
        UserMemFree(MEM_SRAM,barScanSrcData);
    return LH_ERR_NONE;
}

