/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-16 19:29:26
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-06 11:10:07
**FilePath: \LH_TC_APP_V2.3\TaskMain\SystemTaskStart.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SystemTaskConfig.h"
#include "bsp_user.h"
#include "SelfBoardID.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START];
#pragma pack()

//启动任务的堆栈
OS_TCB tcbTaskStart;

//板上硬件初始化
static void BoardDeviceInit(void);
//ServiceAction层IPC调用结构初始化
static void ServiceIPC_DataInit(void);

//系统服务任务配置数组
static const OS_TASK_CREATE_CONFIG serviceTaskArray[] = {
    {&tcbServiceTaskCanRecv, NAME_SERVICE_TASK_CAN_RECV, ServiceTaskFuncCanRecv, 
        PRIO_SERVICE_TASK_CAN_RECV, &stackBufferServiceTaskCanRecv[0],
        STK_SIZE_SERVICE_TASK_CAN_RECV / 10, STK_SIZE_SERVICE_TASK_CAN_RECV, 
        MCU_CAN_RECEIVE_BUFFER_LENGTH},//CAN总线接收数据处理任务
    {&tcbServiceTaskHeart, NAME_SERVICE_TASK_HEART, ServiceTaskFuncHeart, 
        PRIO_SERVICE_TASK_HEART, &stackBufferServiceTaskHeart[0],
        STK_SIZE_SERVICE_TASK_HEART / 10, STK_SIZE_SERVICE_TASK_HEART, 
        2},//传感器状态检测任务
    {&tcbServiceTaskSensor, NAME_SERVICE_TASK_SENSOR, ServiceTaskFuncSensor, 
        PRIO_SERVICE_TASK_SENSOR, &stackBufferServiceTaskSensor[0],
        STK_SIZE_SERVICE_TASK_SENSOR / 10, STK_SIZE_SERVICE_TASK_SENSOR, 
        2},//系统状态检测任务
    {&tcbServiceTaskEnvir, NAME_SERVICE_TASK_ENVIR, ServiceTaskFuncEnvir, 
        PRIO_SERVICE_TASK_ENVIR, &stackBufferServiceTaskEnvir[0],
        STK_SIZE_SERVICE_TASK_ENVIR / 10, STK_SIZE_SERVICE_TASK_ENVIR, 
        2},//自动运行任务
    {&tcbServiceTaskParam, NAME_SERVICE_TASK_PARAM, ServiceTaskFuncParam, 
        PRIO_SERVICE_TASK_PARAM, &stackBufferServiceTaskParam[0],
        STK_SIZE_SERVICE_TASK_PARAM / 10, STK_SIZE_SERVICE_TASK_PARAM, 
        2},//参数读写
    {&tcbServiceTaskPlateReaction, NAME_SERVICE_TASK_PLATE_REACTION, ServiceTaskFuncPlateReaction, 
        PRIO_SERVICE_TASK_PLATE_REACTION, &stackBufferServiceTaskPlateReaction[0],
        STK_SIZE_SERVICE_TASK_PLATE_REACTION / 10, STK_SIZE_SERVICE_TASK_PLATE_REACTION, 
        2},//试剂盘运行任务
    {&tcbServiceTaskPlateReagent, NAME_SERVICE_TASK_PLATE_REAGENT, ServiceTaskFuncPlateReagent, 
        PRIO_SERVICE_TASK_PLATE_REAGENT, &stackBufferServiceTaskPlateReagent[0],
        STK_SIZE_SERVICE_TASK_PLATE_REAGENT / 10, STK_SIZE_SERVICE_TASK_PLATE_REAGENT, 
        2},//反应盘运行任务
    {&tcbServiceTaskPlateWash, NAME_SERVICE_TASK_PLATE_WASH, ServiceTaskFuncPlateWash, 
        PRIO_SERVICE_TASK_PLATE_WASH, &stackBufferServiceTaskPlateWash[0],
        STK_SIZE_SERVICE_TASK_PLATE_WASH / 10, STK_SIZE_SERVICE_TASK_PLATE_WASH, 
        2},//清洗盘运行任务
    {&tcbServiceTaskDebugIAP, NAME_SERVICE_TASK_DEBUG_IAP, ServiceTaskFuncDebugIAP, 
        PRIO_SERVICE_TASK_DEBUG_IAP, &stackBufferServiceTaskDebugIAP[0],
        STK_SIZE_SERVICE_TASK_DEBUG_IAP / 10, STK_SIZE_SERVICE_TASK_DEBUG_IAP, 
        2},//调试升级任务
};
//系统服务任务的数量,用于初始化
#define COUNT_SERVICE_TASK       (sizeof(serviceTaskArray)/sizeof(serviceTaskArray[0]))


//系统应用任务配置数组
static const OS_TASK_CREATE_CONFIG appTaskArray[] = {
    {&tcbAppTaskSystemMsg, NAME_APP_TASK_SYSTEM_MSG, AppTaskFuncSystemMsg, 
        PRIO_APP_TASK_SYSTEM_MSG, &stackBufferAppTaskSystemMsg[0],
        STK_SIZE_APP_TASK_SYSTEM_MSG / 10, STK_SIZE_APP_TASK_SYSTEM_MSG, 
        2},//传感器状态检测任务
};
//系统应用任务的数量,用于初始化
#define COUNT_APP_TASK       (sizeof(appTaskArray)/sizeof(appTaskArray[0]))




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
    BoardDeviceInit();
    //进入临界区
    CPU_CRITICAL_ENTER();

    /************************************创建系统任务***********************/
    for (indexUtil = 0; indexUtil < COUNT_SERVICE_TASK; indexUtil++)
    {
        OSTaskCreate((OS_TCB *)serviceTaskArray[indexUtil].tcbTaskPtr,           //任务控制块
                     (CPU_CHAR *)serviceTaskArray[indexUtil].taskNamePtr,        //任务名字
                     (OS_TASK_PTR)serviceTaskArray[indexUtil].taskFuncPtr,       //任务函数
                     (void *)0,                                                        //传递给任务函数的参数
                     (OS_PRIO)serviceTaskArray[indexUtil].taskPrio,              //任务优先级
                     (CPU_STK *)serviceTaskArray[indexUtil].taskStkBasePtr,      //任务堆栈基地址
                     (CPU_STK_SIZE)serviceTaskArray[indexUtil].taskStkSizeLimit, //任务堆栈深度限位
                     (CPU_STK_SIZE)serviceTaskArray[indexUtil].taskStkSize,      //任务堆栈大小
                     (OS_MSG_QTY)serviceTaskArray[indexUtil].msgQueenSize,       //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                     (OS_TICK)0,                                                       //当使能时间片轮转时的时间片长度，为0时为默认长度，
                     (void *)0,                                                        //用户补充的存储区
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,                //任务选项
                     (OS_ERR *)&os_err);                                               //存放该函数错误时的返回值
    } 
    //创建应用任务
    for (indexUtil = 0; indexUtil < COUNT_APP_TASK; indexUtil++)
    {
        OSTaskCreate((OS_TCB *)appTaskArray[indexUtil].tcbTaskPtr,           //任务控制块
                     (CPU_CHAR *)appTaskArray[indexUtil].taskNamePtr,        //任务名字
                     (OS_TASK_PTR)appTaskArray[indexUtil].taskFuncPtr,       //任务函数
                     (void *)0,                                                        //传递给任务函数的参数
                     (OS_PRIO)appTaskArray[indexUtil].taskPrio,              //任务优先级
                     (CPU_STK *)appTaskArray[indexUtil].taskStkBasePtr,      //任务堆栈基地址
                     (CPU_STK_SIZE)appTaskArray[indexUtil].taskStkSizeLimit, //任务堆栈深度限位
                     (CPU_STK_SIZE)appTaskArray[indexUtil].taskStkSize,      //任务堆栈大小
                     (OS_MSG_QTY)appTaskArray[indexUtil].msgQueenSize,       //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                     (OS_TICK)0,                                                       //当使能时间片轮转时的时间片长度，为0时为默认长度，
                     (void *)0,                                                        //用户补充的存储区
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,                //任务选项
                     (OS_ERR *)&os_err);                                               //存放该函数错误时的返回值
    } 

    /**************************ServiceAction API调用需要的数据初始化**********************/
    ServiceIPC_DataInit();
    
    /******************************创建应用任务******************************/
    /**************************AppLogic API调用需要的数据初始化**********************/
    //退出临界区
    CPU_CRITICAL_EXIT();
    //删除start_task任务自身
    OSTaskDel((OS_TCB *)0, &os_err);
}

void BoardDeviceInit(void)
{
    LH_ERR err;
    //LED初始化
    BoardSystemLedInit(BOARD_LED_RED,IO_SIGNAL_VALID);
    BoardSystemLedInit(BOARD_LED_GREEN,IO_SIGNAL_VALID);
    BoardSystemLedInit(BOARD_LED_YELLOW,IO_SIGNAL_VALID);
    //蜂鸣器初始化
    BoardBeepInit(IO_SIGNAL_INVALID);
    //风扇初始化
    BoardFanInitAll(IO_SIGNAL_INVALID);
    //串口初始化,系统串口,用于系统调试,应用串口在串口任务中进行初始化,因为每块板卡的具体实现不一样
    BoardRS_Init(BOARD_RS_PORT_1,115200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,NULL);
    //热电偶灯初始化
    BoardThermocoupleLedInitAll(IO_SIGNAL_INVALID);
    //电流检测初始化
    BoardElectricCurrentDetectInit();
    //DS18B20初始化
    BoardDS18B20Init();
    //MAX31856初始化
    BoardMax31856AllInit();
    //当设置的板卡ID不是通用板的时候,检测板卡ID,必须和程序设定的板卡ID匹配
    BOARD_ID boardid;
    //主板ID初始化
    do
    {
        boardid = BoardKeyID_Init();
        if(boardid != SELF_BOARD_ID)
        {
            BoardSystemLedToogle(BOARD_LED_RED);
            BoardBeepToogleState();
            SystemDebugPrintf("BoardID_Init Failed\r\n");
            CoreDelayMs(100);
        }
    }while(boardid != SELF_BOARD_ID);
    //关闭蜂鸣器
    BoardBeepWriteState(IO_SIGNAL_INVALID);
    //CAN总线初始化
    MCU_CAN1_Init(MCU_CAN_BAUD_1000K,BoardKeyID_GetCurrent(),OS_CanReceiveDataCallFunc);
    float currentBoardTemp = 0.0;
    //检测DS18B20状态
    do
    {
        err = BoardDS18B20CheckExist();
        if(err != LH_ERR_NONE)
        {
            //报警以及蜂鸣器叫起来
            BoardSystemLedToogle(BOARD_LED_RED);
            BoardBeepToogleState();
            SystemDebugPrintf("BoardDS18B20CheckExist Failed\r\n");
            CoreDelayMs(500);
        }
    } while (err != LH_ERR_NONE);
    //关闭蜂鸣器
    BoardBeepWriteState(IO_SIGNAL_INVALID);
    //获取当前温度
    do
    {
        //获取当前温度
        err = BoardDS18B20ReadCurrent(&currentBoardTemp);
        if(err != LH_ERR_NONE)
        {
            //重试一次获取温度
            CoreDelayMs(1000);
            err = BoardDS18B20ReadCurrent(&currentBoardTemp);
        }
        if(err != LH_ERR_NONE)
        {
            //报警以及蜂鸣器叫起来
            BoardSystemLedToogle(BOARD_LED_RED);
            BoardBeepToogleState();
            SystemDebugPrintf("BoardDS18B20ReadCurrent Failed\r\n");
            CoreDelayMs(500);
        }
    } while (err != LH_ERR_NONE);
    //关闭蜂鸣器
    BoardBeepWriteState(IO_SIGNAL_INVALID);
    //MAX31856设置初始化配置
    BoardMax31856SetConfigDefault(CHANNEL_MAX31856_PLATE_REAGENT);
    BoardMax31856SetConfigDefault(CHANNEL_MAX31856_PLATE_REACTION);
    BoardMax31856SetConfigDefault(CHANNEL_MAX31856_PLATE_WASH);
    BoardMax31856SetConfigDefault(CHANNEL_MAX31856_ENVIRMENT);
    //设置冷端温度
    BoardMax31856SetCoolTemp(CHANNEL_MAX31856_PLATE_REAGENT,currentBoardTemp);
    BoardMax31856SetCoolTemp(CHANNEL_MAX31856_PLATE_REACTION,currentBoardTemp);
    BoardMax31856SetCoolTemp(CHANNEL_MAX31856_PLATE_WASH,currentBoardTemp);
    BoardMax31856SetCoolTemp(CHANNEL_MAX31856_ENVIRMENT,currentBoardTemp);
    //三路控温初始化
    BoardPlateReagentDeviceInit();
    BoardPlateReactionDeviceInit();
    BoardPlateWashDeviceInit();
    //随机数初始化
    MCU_RandomInit();
    //铁电存储器初始化
    do
    {
        err = BoardMB85RS2MT_Init();
        if(err != LH_ERR_NONE)
        {
            BoardSystemLedToogle(BOARD_LED_RED);
            BoardBeepToogleState();
            SystemDebugPrintf("BoardMB85RS2MT_Init Failed\r\n");
            CoreDelayMs(1000);
        }
    } while (err != LH_ERR_NONE);
    //写入APP版本号
    BoardVersion_WriteAppVersion();
    //关闭蜂鸣器
    BoardBeepWriteState(IO_SIGNAL_INVALID);
    //清除IAP标志,当跳转到APP的时候自动清除,这样调试程序的时候可以直接步入APP
    BoardIAP_ClearFlag();
    //初始化为正常状态
    SystemModeInit(MODE_RELEASE);
    //内存管理初始化
    UserMemInit(SRAM_IN);
    UserMemInit(SRAM_CCM);
    //系统参数初始化
    ServiceUtilParamInit();
    //系统状态初始化
    ServiceMsgInit();
    //打开蜂鸣器,标识系统启动
    BoardBeepWriteState(IO_SIGNAL_VALID);
    //延时等待
    CoreDelayMs(1000);
    //关闭蜂鸣器
    BoardBeepWriteState(IO_SIGNAL_INVALID);
    //初始化之后将灯设置为初始状态
    BoardSystemLedWrite(BOARD_LED_RED,IO_SIGNAL_INVALID);
    BoardSystemLedWrite(BOARD_LED_GREEN,IO_SIGNAL_INVALID);
    BoardSystemLedWrite(BOARD_LED_YELLOW,IO_SIGNAL_INVALID);
    //开始运行
    SystemDebugPrintf("System Start\r\n");
}

//ServiceAction层IPC调用结构初始化
static void ServiceIPC_DataInit(void)
{
    
}
