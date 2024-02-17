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
    //CAN总线接收数据处理任务
    {&tcbServiceTaskCanRecv, NAME_SERVICE_TASK_CAN_RECV, ServiceTaskFuncCanRecv, PRIO_SERVICE_TASK_CAN_RECV, &stackBufferServiceTaskCanRecv[0],
        STK_SIZE_SERVICE_TASK_CAN_RECV / 10, STK_SIZE_SERVICE_TASK_CAN_RECV, MCU_CAN_RECEIVE_BUFFER_LENGTH},
    //传感器状态检测任务
    {&tcbServiceTaskHeart, NAME_SERVICE_TASK_HEART, ServiceTaskFuncHeart, PRIO_SERVICE_TASK_HEART, &stackBufferServiceTaskHeart[0],
        STK_SIZE_SERVICE_TASK_HEART / 10, STK_SIZE_SERVICE_TASK_HEART, 2},
    //系统状态检测任务
    {&tcbServiceTaskSensor, NAME_SERVICE_TASK_SENSOR, ServiceTaskFuncSensor, PRIO_SERVICE_TASK_SENSOR, &stackBufferServiceTaskSensor[0],
        STK_SIZE_SERVICE_TASK_SENSOR / 10, STK_SIZE_SERVICE_TASK_SENSOR, 2},
    //自动运行任务
    {&tcbServiceTaskIap, NAME_SERVICE_TASK_IAP, ServiceTaskFuncIap, PRIO_SERVICE_TASK_IAP, &stackBufferServiceTaskIap[0],
        STK_SIZE_SERVICE_TASK_IAP / 10, STK_SIZE_SERVICE_TASK_IAP, 2},
};
//系统服务任务的数量,用于初始化
#define COUNT_SERVICE_TASK       (sizeof(serviceTaskArray)/sizeof(serviceTaskArray[0]))


//系统应用任务配置数组

//系统应用任务的数量,用于初始化



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
    //串口初始化,系统串口,用于系统调试,应用串口在串口任务中进行初始化,因为每块板卡的具体实现不一样
    BoardRS_Init(BOARD_RS_PORT_1,115200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,NULL);
    //风扇初始化
    BoardFanInitAll(IO_SIGNAL_INVALID);
    //蜂鸣器初始化
    BoardBeepInit(IO_SIGNAL_INVALID);
    //热电偶灯初始化
    BoardThermocoupleLedInitAll(IO_SIGNAL_INVALID);
    //当设置的板卡ID不是通用板的时候,检测板卡ID,必须和程序设定的板卡ID匹配
    BOARD_ID boardid;
    //主板ID初始化
    do
    {
        boardid = BoardKeyID_Init();
        //这是APP,要检测ID
        if(boardid != SELF_BOARD_ID)
        {
            BoardSystemLedToogle(BOARD_LED_RED);
            SystemDebugPrintf("BoardID_Init Failed\r\n");
            CoreDelayMs(100);
        }
    }while(boardid != SELF_BOARD_ID);
    //CAN总线初始化
    MCU_CAN1_Init(MCU_CAN_BAUD_1000K,BoardKeyID_GetCurrent(),OS_CanReceiveDataCallFunc);
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
            SystemDebugPrintf("BoardMB85RS2MT_Init Failed\r\n");
            CoreDelayMs(1000);
        }
    } while (err != LH_ERR_NONE);
    //写入BOOT版本号
    BoardVersion_WriteBootVersion();
    //初始化IAP,决定跳转还是继续Boot
    BoardIAP_Init();
    //内存管理初始化
    UserMemInit(SRAM_IN);
    UserMemInit(SRAM_CCM);
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
