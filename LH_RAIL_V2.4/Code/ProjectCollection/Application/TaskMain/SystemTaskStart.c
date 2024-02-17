/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-08 18:20:28
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-18 11:38:33
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SystemTaskConfig.h"
#include "AppIpcInc.h"
#include "ServiceIpcInc.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START]@".dtcm_ram";
#pragma pack()

//启动任务的堆栈
OS_TCB tcbTaskStart@".dtcm_ram";

//板上外设初始化
void BoardDeviceInit(void);

//Service层IPC调用结构初始化
static void ServiceIPC_DataInit(void);
//App层IPC调用结构初始化
static void AppIPC_DataInit(void);


//系统服务任务配置数组
static const OS_TASK_CREATE_CONFIG ServiceTaskArray[] = {
    //服务层自动运行
    {&tcbTaskServiceHeart       , NAME_TASK_SERVICE_HEART           , TaskFuncServiceHeart          , PRIO_TASK_SERVICE_HEART           , &stackBufferTaskServiceHeart[0]           ,STK_SIZE_TASK_SERVICE_HEART / 10           , STK_SIZE_TASK_SERVICE_HEART           , 2                         },
    //CAN2接收处理
    {&tcbTaskServiceCan2Recv    , NAME_TASK_SERVICE_CAN2_RECV       , TaskFuncServiceCan2Recv       , PRIO_TASK_SERVICE_CAN2_RECV       , &stackBufferTaskServiceCan2Recv[0]        ,STK_SIZE_TASK_SERVICE_CAN2_RECV / 10       , STK_SIZE_TASK_SERVICE_CAN2_RECV       , MCU_CAN_MSG_FIFO_LENGTH   },
    //CAN1接收处理
    {&tcbTaskServiceCan1Recv    , NAME_TASK_SERVICE_CAN1_RECV       , TaskFuncServiceCan1Recv       , PRIO_TASK_SERVICE_CAN1_RECV       , &stackBufferTaskServiceCan1Recv[0]        ,STK_SIZE_TASK_SERVICE_CAN1_RECV / 10       , STK_SIZE_TASK_SERVICE_CAN1_RECV       , MCU_CAN_MSG_FIFO_LENGTH   },
    //轨道进样1
    {&tcbTaskServiceRailPushIn1 , NAME_TASK_SERVICE_RAIL_PUSH_IN1   , TaskFuncServiceRailPushIn1    , PRIO_TASK_SERVICE_RAIL_PUSH_IN1   , &stackBufferTaskServiceRailPushIn1[0]     ,STK_SIZE_TASK_SERVICE_RAIL_PUSH_IN1 / 10   , STK_SIZE_TASK_SERVICE_RAIL_PUSH_IN1   , 2                         },
    //轨道进样2
    {&tcbTaskServiceRailPushIn2 , NAME_TASK_SERVICE_RAIL_PUSH_IN2   , TaskFuncServiceRailPushIn2    ,PRIO_TASK_SERVICE_RAIL_PUSH_IN2    , &stackBufferTaskServiceRailPushIn2[0]     ,STK_SIZE_TASK_SERVICE_RAIL_PUSH_IN2 / 10   , STK_SIZE_TASK_SERVICE_RAIL_PUSH_IN2   , 2                         },
    //轨道出样1
    {&tcbTaskServiceRailPushOut1, NAME_TASK_SERVICE_RAIL_PUSH_OUT1  , TaskFuncServiceRailPushOut1   , PRIO_TASK_SERVICE_RAIL_PUSH_OUT1  , &stackBufferTaskServiceRailPushOut1[0]    ,STK_SIZE_TASK_SERVICE_RAIL_PUSH_OUT1 / 10  , STK_SIZE_TASK_SERVICE_RAIL_PUSH_OUT1  , 2                         },
    //轨道出样2
    {&tcbTaskServiceRailPushOut2, NAME_TASK_SERVICE_RAIL_PUSH_OUT2  , TaskFuncServiceRailPushOut2   , PRIO_TASK_SERVICE_RAIL_PUSH_OUT2  , &stackBufferTaskServiceRailPushOut2[0]    ,STK_SIZE_TASK_SERVICE_RAIL_PUSH_OUT2 / 10  , STK_SIZE_TASK_SERVICE_RAIL_PUSH_OUT2  , 2                         },
    //轨道传输
    {&tcbTaskServiceRailTrans   , NAME_TASK_SERVICE_RAIL_TRANS      , TaskFuncServiceRailTrans      , PRIO_TASK_SERVICE_RAIL_TRANS      , &stackBufferTaskServiceRailTrans[0]       ,STK_SIZE_TASK_SERVICE_RAIL_TRANS / 10      , STK_SIZE_TASK_SERVICE_RAIL_TRANS      , 2                         },
    //轨道机械手
    {&tcbTaskServiceRailHand    , NAME_TASK_SERVICE_RAIL_HAND       , TaskFuncServiceRailHand       , PRIO_TASK_SERVICE_RAIL_HAND       , &stackBufferTaskServiceRailHand[0]        ,STK_SIZE_TASK_SERVICE_RAIL_HAND / 10       , STK_SIZE_TASK_SERVICE_RAIL_HAND       , 2                         },
    //轨道扫码
    {&tcbTaskServiceBarScan     , NAME_TASK_SERVICE_BAR_SCAN        , TaskFuncServiceBarScan        , PRIO_TASK_SERVICE_BAR_SCAN        , &stackBufferTaskServiceBarScan[0]         ,STK_SIZE_TASK_SERVICE_BAR_SCAN / 10        , STK_SIZE_TASK_SERVICE_BAR_SCAN        , 2                         },
    //轨道复合任务
    {&tcbTaskServiceRailComplex , NAME_TASK_SERVICE_RAIL_COMPLEX    , TaskFuncServiceRailComplex    ,PRIO_TASK_SERVICE_RAIL_COMPLEX     , &stackBufferTaskServiceRailComplex[0]     ,STK_SIZE_TASK_SERVICE_RAIL_COMPLEX / 10    , STK_SIZE_TASK_SERVICE_RAIL_COMPLEX    , 2                         },
    //轨道调试
    {&tcbTaskServiceDebug       , NAME_TASK_SERVICE_DEBUG           , TaskFuncServiceDebug          ,PRIO_TASK_SERVICE_DEBUG            , &stackBufferTaskServiceDebug[0]           ,STK_SIZE_TASK_SERVICE_DEBUG / 10           , STK_SIZE_TASK_SERVICE_DEBUG           , 2                         },
};
//系统服务任务的数量,用于初始化
#define COUNT_SERVICE_TASK       (sizeof(ServiceTaskArray)/sizeof(ServiceTaskArray[0]))


//系统应用任务配置数组
static const OS_TASK_CREATE_CONFIG AppTaskArray[] = {
    //传感器自动处理
    {&tcbTaskAppSampleRackIn        , NAME_TASK_APP_SAMPLE_RACK_IN      , TaskFuncAppSampleRackIn   , PRIO_TASK_APP_SAMPLE_RACK_IN      , &stackBufferTaskAppSampleRackIn[0]    ,STK_SIZE_TASK_APP_SAMPLE_RACK_IN / 10      , STK_SIZE_TASK_APP_SAMPLE_RACK_IN      , 2 },
    //应用层综合进样
    {&tcbTaskAppSampleRackMove      , NAME_TASK_APP_SAMPLE_RACK_MOVE    , TaskFuncAppSampleRackMove , PRIO_TASK_APP_SAMPLE_RACK_MOVE    , &stackBufferTaskAppSampleRackMove[0]  ,STK_SIZE_TASK_APP_SAMPLE_RACK_MOVE / 10    , STK_SIZE_TASK_APP_SAMPLE_RACK_MOVE    , 2 },
    //应用层综合出样
    {&tcbTaskAppSampleRackOut       , NAME_TASK_APP_SAMPLE_RACK_OUT     , TaskFuncAppSampleRackOut  , PRIO_TASK_APP_SAMPLE_RACK_OUT     , &stackBufferTaskAppSampleRackOut[0]   ,STK_SIZE_TASK_APP_SAMPLE_RACK_OUT / 10     , STK_SIZE_TASK_APP_SAMPLE_RACK_OUT     , 2 },
    //应用层轨道综合
    {&tcbTaskAppStateParam          , NAME_TASK_APP_STATE_PARAM         , TaskFuncAppStateParam     , PRIO_TASK_APP_STATE_PARAM         , &stackBufferTaskAppStateParam[0]      ,STK_SIZE_TASK_APP_STATE_PARAM / 10         , STK_SIZE_TASK_APP_STATE_PARAM         , 2 },
    //状态上传任务
    {&tcbTaskAppStateUpLoad         , NAME_TASK_APP_STATE_UPLOAD        , TaskFuncAppStateUpLoad    , PRIO_TASK_APP_STATE_UPLOAD        , &stackBufferTaskAppStateUpLoad[0]     ,STK_SIZE_TASK_APP_STATE_UPLOAD / 10        , STK_SIZE_TASK_APP_STATE_UPLOAD        , 2 },
};
//系统应用任务的数量,用于初始化
#define COUNT_APP_TASK            (sizeof(AppTaskArray)/sizeof(AppTaskArray[0]))


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
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP|OS_OPT_TASK_NO_TLS,           //任务选项
                     (OS_ERR *)&os_err);                                         //存放该函数错误时的返回值
    } 
    /******************************初始化Service调用结构**************************/
    ServiceIPC_DataInit();
    /******************************************************创建应用服务任务***********************************************/
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
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP|OS_OPT_TASK_NO_TLS,      //任务选项
                     (OS_ERR *)&os_err);                                     //存放该函数错误时的返回值
    } 
    /******************************初始化App调用结构**************************/
    AppIPC_DataInit();
    //加载系统服务参数
        ServiceParamInit();
    //加载系统服务状态
        ServiceStateInit();
    //加载系统应用参数
        AppParamInit();
    //加载系统应用状态
        AppStateInit();
    //退出临界区
        CPU_CRITICAL_EXIT();
    //删除start_task任务自身
        OSTaskDel((OS_TCB *)0, &os_err);
}


//板上外设初始化
void BoardDeviceInit(void)
{
    LH_ERR deviceResult = LH_ERR_NONE;
    //LED初始化,点亮
        BoardLedInit(BOARD_LED_RED,BOARD_LED_LIGHT);
        BoardLedInit(BOARD_LED_GREEN,BOARD_LED_LIGHT);
        BoardLedInit(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //Beep初始化
        BoardBeepInit(BEEP_OFF);
    //初始化完成,全部关闭
        BoardLedWrite(BOARD_LED_RED,BOARD_LED_DARK);
        BoardLedWrite(BOARD_LED_GREEN,BOARD_LED_DARK);
        BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_DARK);
    //内存管理初始化
        UserMemInit(MEM_DTCM);
        UserMemInit(MEM_AXI_SRAM);
        UserMemInit(MEM_D2_SRAM1);
        UserMemInit(MEM_D2_SRAM2);
        UserMemInit(MEM_D3_SRAM4);
    //IO初始化
        BoardInOutAllInitOnce();
    //串口初始化,用作动作Action调试
        BoardRS_Init(BOARD_RS1_UART3, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,
                    MCU_UART_HARD_CONTROL_NONE, NULL);
    //串口初始化,用作流程调试
        BoardRS_Init(BOARD_RS2_UART1, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,
                    MCU_UART_HARD_CONTROL_NONE, NULL);
    //还剩下一个串口,目前还不知道做啥用,RS4 UART6
        BoardRS_Init(BOARD_RS4_UART6, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,
                    MCU_UART_HARD_CONTROL_NONE, NULL);
    //模块号识别初始化
        BoardModuleID_Init();
    //打印APP启动以及模块号
        SystemPrintf("RailAppStart ,ModuleNO : %d\r\n",BoardModuleRead());
    //CAN1初始化,与中位机通讯
        MCU_CAN1_Init(MCU_CAN_BPS_1000K,CAN1_SELF_MODULE_NO,CAN1_SELF_BOARD_ID,OS_Can1ReceiveDataCallFunc);
    //CAN2初始化,与下位机通讯
        MCU_CAN2_Init(MCU_CAN_BPS_1000K,DEVICE_MAIN_BOARD_ID,OS_Can2ReceiveDataCallFunc);
    //随机数初始化
        MCU_RandomInit();
    //条码枪串口初始化
        BoardQRScanCM200Init();
    //压力传感器初始化
        BoardPressureSensorInit();
    //显示一下当前芯片温度
        SystemPrintf("CurrentChipTemp : %2.2f\r\n",MCU_ADC_ReadChipTemp());
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
    //内存管理初始化
        UserMemInit(MEM_SRAM);
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
    //清理IAP标志
        BoardIAP_ClearFlag();
    //写入APP版本号
        BoardVersion_WriteAppVersion();
    //W5500初始化
        do
        {
            deviceResult = BoardW5500_Init(NULL);
            if(deviceResult == LH_ERR_NONE)
            {
                break;
            }
            SystemPrintf("BoardW5500_Init ErrorCode : 0X%08X\r\n",deviceResult);
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(100);
        }while(0);
    //SD卡初始化
        do
        {
            deviceResult = BoardSD_Init();
            if(deviceResult == LH_ERR_NONE)
            {
                break;
            }
            SystemPrintf("BoardSD_Init ErrorCode : 0X%08X\r\n",deviceResult);
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(100);
        }while(0);//不要求初始化
    //beep响铃展示,系统每次复位都会响铃,通知系统启动
        BoardBeepSetState(BEEP_ON);
    //延时发生,等待系统内部其他模块启动
        CoreDelayMs(500);
    //关闭蜂鸣器
        BoardBeepSetState(BEEP_OFF);
    //程序跳转
        if(BoardIAP_ReadJumpFlag() == 0X5A)
        {
            BoardIAP_ClearJumpFlag();
            //返回Boot-->App结果
            Can1RailSendProgramJumpResult_Self(1,1);
        }
    //初始化完成,全部关闭
        BoardLedWrite(BOARD_LED_RED,BOARD_LED_DARK);
        BoardLedWrite(BOARD_LED_GREEN,BOARD_LED_DARK);
        BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_DARK);
}


//Service层IPC调用结构初始化
static void ServiceIPC_DataInit(void)
{
    //进样1
    IPC_RailPushIn1DataInit();
    //进样2
    IPC_RailPushIn2DataInit();
    //出样1
    IPC_RailPushOut1DataInit();
    //出样2
    IPC_RailPushOut2DataInit();
    //轨道传输
    IPC_RailTransDataInit();
    //机械手
    IPC_RailHandDataInit();
    //条码扫描
    IPC_BarScanDataInit();
    //复合指令
    IPC_RailComplexDataInit();
    //辅助功能
    IPC_AssistDataInit();
}

//App层IPC调用结构初始化
static void AppIPC_DataInit(void)
{
    //进样
    IPC_SampleRackInDataInit();
    //轨道机械手
    IPC_SampleRackMoveDataInit();
    //出样
    IPC_SampleRackOutDataInit();
    //状态参数管理
    IPC_StateParamDataInit();
}
