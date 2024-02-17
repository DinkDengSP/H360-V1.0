/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-26 10:48:00
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-27 14:04:37
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SystemTaskConfig.h"
#include "bsp_user.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START]@".ccram";
#pragma pack()
//启动任务的堆栈
OS_TCB tcbTaskStart@".ccram";
//板上硬件初始化
static void BoardDeviceInit(void);


//系统服务任务配置数组
static const OS_TASK_CREATE_CONFIG ServiceTaskArray[] = {
    //CAN总线接收数据处理任务
    {&tcbServiceTaskCanMsgProc , NAME_SERVICE_TASK_CAN_MSG_PROC, ServiceTaskFuncCanMsgProc, PRIO_SERVICE_TASK_CAN_MSG_PROC, &stackBufferServiceTaskCanMsgProc[0],STK_SIZE_SERVICE_TASK_CAN_MSG_PROC / 10, STK_SIZE_SERVICE_TASK_CAN_MSG_PROC, MCU_CAN_RECEIVE_BUFFER_LENGTH},
    //心跳任务
    {&tcbServiceTaskHeart      , NAME_SERVICE_TASK_HEART       , ServiceTaskFuncHeart     , PRIO_SERVICE_TASK_HEART       , &stackBufferServiceTaskHeart[0]     ,STK_SIZE_SERVICE_TASK_HEART / 10       , STK_SIZE_SERVICE_TASK_HEART       , 2                             },
    //IAP任务
    {&tcbTaskBoardIap          , NAME_TASK_BOARD_IAP           , ServiceTaskFuncBoardIap  , PRIO_TASK_BOARD_IAP           , &stackBufferTaskBoardIap[0]         ,STK_SIZE_TASK_BOARD_IAP / 10           , STK_SIZE_TASK_BOARD_IAP           , 2                             },
};
//系统服务任务的数量,用于初始化
#define COUNT_SERVICE_TASK       (sizeof(ServiceTaskArray)/sizeof(ServiceTaskArray[0]))


//系统应用任务配置数组
static const OS_TASK_CREATE_CONFIG AppTaskArray[] = {
    {&tcbAppTaskDemo, NAME_APP_TASK_DEMO, AppTaskFuncDemo, PRIO_APP_TASK_DEMO, &stackBufferAppTaskDemo[0],STK_SIZE_APP_TASK_DEMO / 10, STK_SIZE_APP_TASK_DEMO,2},
};
//系统应用任务的数量,用于初始化
#define COUNT_APP_TASK            (sizeof(AppTaskArray)/sizeof(AppTaskArray[0]))


void TaskFuncStart(void *p_arg)
{
    (void)p_arg;
    uint16_t indexUtil = 0;

    OS_ERR  os_err;
    CPU_SR_ALLOC();
    //默认为空的追踪函数
    OS_TRACE_INIT(); 
    //使能定时器以及启动定时器运行
    BSP_OS_TickEnable();
    //状态初始化
    #if OS_CFG_STAT_TASK_EN > 0u
        OSStatTaskCPUUsageInit(&os_err);                           
    #endif
    //中断时间测量初始化
    #ifdef CPU_CFG_INT_DIS_MEAS_EN
        CPU_IntDisMeasMaxCurReset();
    #endif
    //板上硬件设备初始化
    BoardDeviceInit();
    //进入临界区
    CPU_CRITICAL_ENTER();
    /****************************创建Service任务以及与之相关的状态,参数,IPC********************************/
    for (indexUtil = 0; indexUtil < COUNT_SERVICE_TASK; indexUtil++)
    {
        OSTaskCreate((OS_TCB *)ServiceTaskArray[indexUtil].tcbTaskPtr,           //任务控制块
                     (CPU_CHAR *)ServiceTaskArray[indexUtil].taskNamePtr,        //任务名字
                     (OS_TASK_PTR)ServiceTaskArray[indexUtil].taskFuncPtr,       //任务函数
                     (void *)0,                                                  //传递给任务函数的参数
                     (OS_PRIO)ServiceTaskArray[indexUtil].taskPrio,              //任务优先级
                     (CPU_STK *)ServiceTaskArray[indexUtil].taskStkBasePtr,      //任务堆栈基地址
                     (CPU_STK_SIZE)ServiceTaskArray[indexUtil].taskStkSizeLimit, //任务堆栈深度限位
                     (CPU_STK_SIZE)ServiceTaskArray[indexUtil].taskStkSize,      //任务堆栈大小
                     (OS_MSG_QTY)ServiceTaskArray[indexUtil].msgQueenSize,       //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                     (OS_TICK)0,                                                 //当使能时间片轮转时的时间片长度，为0时为默认长度，
                     (void *)0,                                                  //用户补充的存储区
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,          //任务选项
                     (OS_ERR *)&os_err);                                         //存放该函数错误时的返回值
    } 
    /****************************创建App任务以及与之相关的状态,参数,IPC********************************/
    for (indexUtil = 0; indexUtil < COUNT_APP_TASK; indexUtil++)
    {
        OSTaskCreate((OS_TCB *)AppTaskArray[indexUtil].tcbTaskPtr,           //任务控制块
                     (CPU_CHAR *)AppTaskArray[indexUtil].taskNamePtr,        //任务名字
                     (OS_TASK_PTR)AppTaskArray[indexUtil].taskFuncPtr,       //任务函数
                     (void *)0,                                              //传递给任务函数的参数
                     (OS_PRIO)AppTaskArray[indexUtil].taskPrio,              //任务优先级
                     (CPU_STK *)AppTaskArray[indexUtil].taskStkBasePtr,      //任务堆栈基地址
                     (CPU_STK_SIZE)AppTaskArray[indexUtil].taskStkSizeLimit, //任务堆栈深度限位
                     (CPU_STK_SIZE)AppTaskArray[indexUtil].taskStkSize,      //任务堆栈大小
                     (OS_MSG_QTY)AppTaskArray[indexUtil].msgQueenSize,       //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                     (OS_TICK)0,                                             //当使能时间片轮转时的时间片长度，为0时为默认长度，
                     (void *)0,                                              //用户补充的存储区
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,      //任务选项
                     (OS_ERR *)&os_err);                                     //存放该函数错误时的返回值
    } 
    //退出临界区
    CPU_CRITICAL_EXIT();
    //删除start_task任务自身
    OSTaskDel((OS_TCB *)0, &os_err);
}

void BoardDeviceInit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BOARD_IAP_JUMP_TO_BOOT_CASE iapState;
    //随机数初始化
    MCU_RandomInit();
    //LED初始化
    BoardLedInit(BOARD_LED_RED,BOARD_LED_LIGHT);
    BoardLedInit(BOARD_LED_GREEN,BOARD_LED_LIGHT);
    BoardLedInit(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //配置按键初始化
    BoardConfigKeyInitAll();
    //BEEP初始化
    BoardBeepInit(BEEP_OFF);
    //串口初始化
    MCU_Uart1Init(115200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,NULL);
    MCU_Uart2Init(115200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,NULL);
    MCU_Uart3Init(115200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,NULL);
    MCU_Uart6Init(115200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,NULL);
    //系统启动
    SystemPrintf("System Bootloader Start,%s\r\n",SoftwareVersionInfo);
    //CAN总线初始化
    MCU_CAN1_Init(MCU_CAN_BAUD_1000K,OS_Can1ReceiveDataCallFunc);
    //SRAM初始化
    BoardSRAM_Init();
    //SRAM自检
    do
    {
        errorCode = BoardSRAM_SelfCheck();
        if(errorCode != LH_ERR_NONE)
        {
            CoreDelayMs(500);
            BoardLedToogle(BOARD_LED_RED);
            SystemPrintf("BoardSRAM_SelfCheck ErrorCode : 0X%08X\r\n",errorCode);
        }
    } while (errorCode != LH_ERR_NONE);
    //动态内存管理初始化
    UserMemInit(SRAM_IN);
    UserMemInit(SRAM_CCM);
    UserMemInit(SRAM_IS62);
    //初始化铁电存储器
    BoardMB85RS2MT_Init();
    //存储器自检
    do
    {
        errorCode = BoardMB85RS2MT_SelfCheck();
        if(errorCode != LH_ERR_NONE)
        {
            CoreDelayMs(500);
            BoardLedToogle(BOARD_LED_RED);
            SystemPrintf("BoardMB85RS2MT_SelfCheck ErrorCode : 0X%08X\r\n",errorCode);
        }
    } while (errorCode != LH_ERR_NONE);
    //写入boot版本
    BoardVersion_WriteBootVersion();
    //IAP初始化,决定是否运行到APP
    iapState = BoardIAP_Init();
    if(iapState == BOARD_IAP_JUMP_TO_BOOT_CASE_UPDATE)
    {
        //这是APP主动要求进入BootLoader,此时需要上报准备好升级
        SystemPrintf("LHCloud Send PrepareOK\r\n");
        bootLoaderNeedResp = 0XFF;
    }
    else if(iapState == BOARD_IAP_JUMP_TO_BOOT_CASE_FORCE)
    {
        bootLoaderNeedResp = 0XFE;
    }

#if(BOARD_SDCARD_ENABLE ==1)
    //SD卡初始化
    BoardSD_PortInit();
    SD_ERROR sdErr;
    do
    {
        sdErr = BoardSD_Init();
        if(sdErr != SD_OK)
        {
            CoreDelayMs(500);
            BoardLedToogle(BOARD_LED_RED);
            SystemPrintf("BoardSD_Init ErrorCode : 0X%08X\r\n",sdErr);
        }
    } while (sdErr != SD_OK);
    ConsoleSendCardMessage();
#endif

    //测试Beep
    BoardBeepSetState(BEEP_ON);
    CoreDelayMs(1000);
    BoardBeepSetState(BEEP_OFF);
    //LED灯全部熄灭
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_DARK);
    BoardLedWrite(BOARD_LED_GREEN,BOARD_LED_DARK);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_DARK);
    //系统启动完成
    SystemPrintf("System Bootloader Start Over!\r\n");
}
