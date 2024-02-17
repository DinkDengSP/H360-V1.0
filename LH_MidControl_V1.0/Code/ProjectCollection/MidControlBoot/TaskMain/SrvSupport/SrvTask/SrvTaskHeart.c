/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-03 14:07:33
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-12 17:31:11
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SrvTaskConfig.h"

//任务堆栈
uint64_t stackBufferSrvTaskHeart[STK_SIZE_SRV_TASK_HEART/8]@".dtcm_ram2";
//任务的控制块
TX_THREAD tcbSrvTaskHeart@".dtcm_ram2";

//自动运行程序的执行周期,单位毫秒
#define SRV_HEART_AUTO_FUNC_RUNNING_PERIOD_MS            200
//系统循环自动程序初始化
static void SrvTaskFuncHeart_Init(void);
//心跳周期性任务
static void SrvTaskFuncHeart_RunPeriod(void);


//任务函数
void TaskFuncSrvHeart(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //心跳相关线程初始化
    SrvTaskFuncHeart_Init();
    while(1)
    {
        CoreDelayMs(SRV_HEART_AUTO_FUNC_RUNNING_PERIOD_MS);
        SrvTaskFuncHeart_RunPeriod();
        //版上按键与LED映射更新
        BoardKeyConfigWriteLedAll(BoardKeyConfigReadAll());
    }
}

//系统循环自动程序初始化
static void SrvTaskFuncHeart_Init(void)
{
    //等待CAN协议初始化
    CoreDelayMs(100);
}

//BOOT下上传中位机BOOT状态
static const SYSTEM_MODULE_STATE modeStateDefault = {TCP_MODULE_STATE_MID_BOOT,{0,37,0,0},{0,5,0,0,},{0,37,0,0}};

//心跳线程周期性任务
static void SrvTaskFuncHeart_RunPeriod(void)
{
    uint32_t timeTick = 0;;
    //LED闪灯
    BoardLedToogle(BOARD_LED_YELLOW);
    //心跳定时上传
    timeTick = tx_time_get();
    //网卡已连接状态,且心跳时间有效
    if((SOCK_ESTABLISHED == TcpBaseUtilGetMainPortState()) && (HEART_TICK_ERR_UNUSED != TcpBaseUtilGetLastHeartTick()))
    {
        SYSTEM_MODULE_STATE* moduleStatePtr = (SYSTEM_MODULE_STATE*)(&modeStateDefault);
        //超过一定时间没有数据上传了
        if(timeTick - TcpBaseUtilGetLastHeartTick() >= TCPIP_STATE_AUTO_UPDATE_PERIOD_MS)
        {
            //已经连接而且心跳周期到位,上传心跳
            TcpBaseSendModuleState(moduleStatePtr,moduleStatePtr,moduleStatePtr,moduleStatePtr,moduleStatePtr);
        }
    }
}


