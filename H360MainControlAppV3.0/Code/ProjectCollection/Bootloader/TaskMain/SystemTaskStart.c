/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-08 18:20:28
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-11 17:00:57
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SystemTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START]@".dtcm_ram";
#pragma pack()

//启动任务的堆栈
OS_TCB tcbTaskStart@".dtcm_ram";


//系统服务任务配置数组
static const OS_TASK_CREATE_CONFIG TaskArray[] = {
    //服务层自动运行
    {&tcbTaskHeart      , NAME_TASK_HEART       , TaskFuncHeart     , PRIO_TASK_HEART       , &stackBufferTaskHeart[0]      ,STK_SIZE_TASK_HEART / 10       , STK_SIZE_TASK_HEART       , 2},
    //CAN2接收处理
    {&tcbTaskCan2Recv   , NAME_TASK_CAN2_RECV   , TaskFuncCan2Recv  , PRIO_TASK_CAN2_RECV   , &stackBufferTaskCan2Recv[0]   ,STK_SIZE_TASK_CAN2_RECV / 10   , STK_SIZE_TASK_CAN2_RECV   , MCU_CAN_MSG_FIFO_LENGTH},
    //CAN1接收处理
    {&tcbTaskCan1Recv   , NAME_TASK_CAN1_RECV   , TaskFuncCan1Recv  , PRIO_TASK_CAN1_RECV   , &stackBufferTaskCan1Recv[0]   ,STK_SIZE_TASK_CAN1_RECV / 10   , STK_SIZE_TASK_CAN1_RECV   , MCU_CAN_MSG_FIFO_LENGTH},
    //IAP
    {&tcbTaskBoardIap   , NAME_TASK_BOARD_IAP   , TaskFuncIap       , PRIO_TASK_BOARD_IAP   , &stackBufferTaskBoardIap[0]   ,STK_SIZE_TASK_BOARD_IAP / 10   , STK_SIZE_TASK_BOARD_IAP   , 2},
};
//系统任务的数量,用于初始化
#define COUNT_TASK       (sizeof(TaskArray)/sizeof(TaskArray[0]))



//板上外设初始化
void BoardDeviceInit(void);

//启动任务
void TaskFuncStart(void *p_arg)
{
    p_arg = p_arg;
    //辅助变量
        uint16_t indexUtil = 0;
    //临界段变量
        OS_ERR  os_err;
        CPU_SR_ALLOC();
    //默认为空的追踪函数
        OS_TRACE_INIT(); 
    //状态初始化
        #if OS_CFG_STAT_TASK_EN > 0u
            OSStatTaskCPUUsageInit(&os_err);                           
        #endif
    //中断时间测量初始化
        #ifdef CPU_CFG_INT_DIS_MEAS_EN
            CPU_IntDisMeasMaxCurReset();
        #endif
    //板上设备初始化
        BoardDeviceInit();
    //进入临界区
        CPU_CRITICAL_ENTER();
    /**************************************************创建系统服务任务**************************************************/
    for (indexUtil = 0; indexUtil < COUNT_TASK; indexUtil++)
    {
        OSTaskCreate((OS_TCB *)TaskArray[indexUtil].tcbTaskPtr,           //任务控制块
                     (CPU_CHAR *)TaskArray[indexUtil].taskNamePtr,        //任务名字
                     (OS_TASK_PTR)TaskArray[indexUtil].taskFuncPtr,       //任务函数
                     (void *)0,                                                        //传递给任务函数的参数
                     (OS_PRIO)TaskArray[indexUtil].taskPrio,              //任务优先级
                     (CPU_STK *)TaskArray[indexUtil].taskStkBasePtr,      //任务堆栈基地址
                     (CPU_STK_SIZE)TaskArray[indexUtil].taskStkSizeLimit, //任务堆栈深度限位
                     (CPU_STK_SIZE)TaskArray[indexUtil].taskStkSize,      //任务堆栈大小
                     (OS_MSG_QTY)TaskArray[indexUtil].msgQueenSize,       //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                     (OS_TICK)0,                                                       //当使能时间片轮转时的时间片长度，为0时为默认长度，
                     (void *)0,                                                        //用户补充的存储区
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP|OS_OPT_TASK_NO_TLS,                 //任务选项
                     (OS_ERR *)&os_err);                                               //存放该函数错误时的返回值
    } 
    /******************************初始化App调用结构**************************/
    //退出临界区
        CPU_CRITICAL_EXIT();
    //删除start_task任务自身
        OSTaskDel((OS_TCB *)0, &os_err);
}


//板上外设初始化
void BoardDeviceInit(void)
{
    LH_ERR deviceResult = LH_ERR_NONE;
    BOARD_IAP_JUMP_TO_BOOT_CASE IAP_State;
    //LED初始化,点亮
    BoardLedInit(BOARD_LED_RED,BOARD_LED_LIGHT);
    BoardLedInit(BOARD_LED_GREEN,BOARD_LED_LIGHT);
    BoardLedInit(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //Beep初始化
    BoardBeepInit(BEEP_OFF);
    //板号管理初始化
    BoardModuleID_Init();
    //内存管理初始化
    UserMemInit(MEM_DTCM);
    UserMemInit(MEM_AXI_SRAM);
    UserMemInit(MEM_D2_SRAM1);
    UserMemInit(MEM_D2_SRAM2);
    UserMemInit(MEM_D3_SRAM4);
    //IO初始化
    BoardInOutAllInitOnce();
    //CAN1初始化,与中位机通讯
    MCU_CAN1_Init(MCU_CAN_BPS_1000K,BoardModuleRead(),CAN1_BOARDID_MAIN_CONTROL,OS_Can1ReceiveDataCallFunc);
    //CAN2初始化,与下位机通讯
    MCU_CAN2_Init(MCU_CAN_BPS_1000K,DEVICE_MAIN_BOARD_ID,OS_Can2ReceiveDataCallFunc);
    //串口初始化,用作动作Action调试
    BoardRS_Init(BOARD_RS1_UART3_VERSION0, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,
                MCU_UART_HARD_CONTROL_NONE, NULL);
    //串口初始化,用作流程调试
    BoardRS_Init(BOARD_RS2_UART1_VERSION0, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,
                MCU_UART_HARD_CONTROL_NONE, NULL);
    //还剩下一个串口,目前还不知道做啥用,RS4 UART6
    BoardRS_Init(BOARD_RS4_UART6, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,
                MCU_UART_HARD_CONTROL_NONE, NULL);
    //获取打印模块号
    SystemPrintf("Device Bootloader,ModuleNO : %d\r\n",BoardModuleRead());
    //SRAM初始化
    do
    {
        deviceResult = BoardSRAM_Init();
        if(deviceResult == LH_ERR_NONE)
        {
            break;
        }
        SystemPrintf("BoardSRAM_Init ErrorCode : 0X%08X\r\n",deviceResult);
        BoardLedToogle(BOARD_LED_RED);
        CoreDelayMs(100);
    }while(1);
    //铁电存储器初始化
    do
    {
        deviceResult = BoardMB85RS2MT_Init();
        if(deviceResult == LH_ERR_NONE)
        {
            break;
        }
        SystemPrintf("BoardMB85RS2MT_Init ErrorCode : 0X%08X\r\n",deviceResult);
        BoardLedToogle(BOARD_LED_RED);
        CoreDelayMs(100);
    }while(1);
    BoardVersion_WriteBootVersion();
    //板号自检
    while(BOARD_MODULE_ID_1 != BoardModuleRead())
    {
        SystemPrintf("BoardModuleID ErrorCode : %d\r\n",BoardModuleRead());
        BoardLedToogle(BOARD_LED_RED);
        BoardLedToogle(BOARD_LED_YELLOW);
        CoreDelayMs(100);
    }
    //写入Boot版本
    BoardVersion_WriteBootVersion();
    //初始化完成,全部关闭
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_DARK);
    BoardLedWrite(BOARD_LED_GREEN,BOARD_LED_DARK);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_DARK);
    //IAP单元初始化,如果不是强制跳转或者准备升级,直接跳转到APP
    IAP_State = BoardIAP_Init();
    if(IAP_State == BOARD_IAP_JUMP_TO_BOOT_CASE_UPDATE)
    {
        //这是APP主动要求进入BootLoader,此时需要上报准备好升级
        //SystemPrintf("Device Send PrepareOK\r\n");
        bootLoaderNeedResp = 0XFF;
    }
    else if(IAP_State == BOARD_IAP_JUMP_TO_BOOT_CASE_FORCE)
    {
        //强制跳转到Boot,上报跳转成功
        bootLoaderNeedResp = 0XFE;
    }
    //运行到这里,说明系统进入正常BootLoader流程
}


