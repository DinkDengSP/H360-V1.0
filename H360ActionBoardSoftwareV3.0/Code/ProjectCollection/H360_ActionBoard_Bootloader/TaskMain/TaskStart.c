#include "stm32f4xx.h"
#include "TaskConfig.h"
#include <cpu.h>
#include "bsp_user.h"
#include "StepMotor.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START]@".ccram";
#pragma pack()
//启动任务的堆栈
OS_TCB tcbTaskStart@".ccram";

//板上硬件初始化
void BoardDeviceInit(void);

//服务任务的配置
const static OS_TASK_CREATE_CONFIG serviceTaskConfigArray[] = {
    {&tcbServiceTaskCan1Receive, NAME_SERVICE_TASK_CAN1_RECEIVE, ServiceTaskFuncCan1Receive, PRIO_SERVICE_TASK_CAN1_RECEIVE, &stackBufferServiceTaskCan1Receive[0],
     STK_SIZE_SERVICE_TASK_CAN1_RECEIVE / 10, STK_SIZE_SERVICE_TASK_CAN1_RECEIVE, MCU_CAN_RECEIVE_BUFFER_LENGTH}, //CAN2数据处理
    {&tcbServiceTaskHeart, NAME_SERVICE_TASK_HEART, ServiceTaskFuncHeart, PRIO_SERVICE_TASK_HEART, &stackBufferServiceTaskHeart[0],
     STK_SIZE_SERVICE_TASK_HEART / 10, STK_SIZE_SERVICE_TASK_HEART, 2},//心跳任务
    {&tcbServiceTaskDebugIAP, NAME_SERVICE_TASK_DEBUG_IAP, ServiceTaskFuncDebugIAP, PRIO_SERVICE_TASK_DEBUG_IAP, &stackBufferServiceTaskDebugIAP[0],
     STK_SIZE_SERVICE_TASK_DEBUG_IAP / 10, STK_SIZE_SERVICE_TASK_DEBUG_IAP, 2},//调试IAP任务
};
//服务任务的数量
#define COUNT_SERVICE_TASK (sizeof(serviceTaskConfigArray) / sizeof(serviceTaskConfigArray[0]))

//启动任务函数
void TaskFuncStart(void *p_arg)
{
    (void)p_arg;
    uint16_t indexUtil = 0;

    OS_ERR os_err;
    //状态保存寄存器
    CPU_SR_ALLOC();
    //默认为空的追踪函数
    OS_TRACE_INIT();
    /* 使能定时器以及启动定时器运行 */
    BSP_OS_TickEnable();

//统计状态初始化
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&os_err);
#endif

//中断时间测量初始化
#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    //板上硬件初始化
    BoardDeviceInit();

    //进入临界区
    CPU_CRITICAL_ENTER();

    /**************************************************创建系统服务任务**************************************************/
    for (indexUtil = 0; indexUtil < COUNT_SERVICE_TASK; indexUtil++)
    {
        OSTaskCreate((OS_TCB *)serviceTaskConfigArray[indexUtil].tcbTaskPtr,           //任务控制块
                     (CPU_CHAR *)serviceTaskConfigArray[indexUtil].taskNamePtr,        //任务名字
                     (OS_TASK_PTR)serviceTaskConfigArray[indexUtil].taskFuncPtr,       //任务函数
                     (void *)0,                                                        //传递给任务函数的参数
                     (OS_PRIO)serviceTaskConfigArray[indexUtil].taskPrio,              //任务优先级
                     (CPU_STK *)serviceTaskConfigArray[indexUtil].taskStkBasePtr,      //任务堆栈基地址
                     (CPU_STK_SIZE)serviceTaskConfigArray[indexUtil].taskStkSizeLimit, //任务堆栈深度限位
                     (CPU_STK_SIZE)serviceTaskConfigArray[indexUtil].taskStkSize,      //任务堆栈大小
                     (OS_MSG_QTY)serviceTaskConfigArray[indexUtil].msgQueenSize,       //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                     (OS_TICK)0,                                                       //当使能时间片轮转时的时间片长度，为0时为默认长度，
                     (void *)0,                                                        //用户补充的存储区
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,                //任务选项
                     (OS_ERR *)&os_err);                                               //存放该函数错误时的返回值
    }
    //退出临界区
    CPU_CRITICAL_EXIT();

    //删除start_task任务自身
    OSTaskDel((OS_TCB *)0, &os_err);
}

//板上硬件初始化
void BoardDeviceInit(void)
{
    LH_ERR err;
    //初始化RTT
    BoardRTT_Init();
    //LED初始化
    BoardLedInit(BOARD_LED_RED, BOARD_LED_LIGHT);
    BoardLedInit(BOARD_LED_GREEN, BOARD_LED_LIGHT);
    BoardLedInit(BOARD_LED_YELLOW, BOARD_LED_LIGHT);
    //主板ID初始化
    BoardKeyID_Init();
    //CAN总线初始化
    MCU_CAN1_Init(MCU_CAN_BAUD_1000K, BoardKeyID_GetCurrent(), OS_Can1ReceiveDataCallFunc);
    //串口初始化,用户串口的初始化在串口任务中自己初始化
    //系统串口,用于通过串口进行调试
    BoardRS_Init(RS_PORT_DEBUG, 115200, MCU_UART_LENGTH8, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,
                 MCU_UART_HARD_CONTROL_NONE, NULL);
    //CPLD初始化
    do
    {
        //空函数的初始化,将不会带电机初始化
        err = BoardCPLD_Init(NULL);
        if (err != LH_ERR_NONE)
        {
            BoardLedToogle(BOARD_LED_RED);
            LowLevelPrintf("IAP BoardCPLD_Init Failed\r\n");
            CoreDelayMs(100);
        }
    } while (err != LH_ERR_NONE);
    //板上IO初始化
    BoardInOutAllInitOnce();
    //中断初始化
    BoardExtiResetAll();
    //随机数初始化
    MCU_RandomInit();
    //铁电存储器初始化
    do
    {
        err = BoardMB85RS2MT_Init();
        if (err != LH_ERR_NONE)
        {
            BoardLedToogle(BOARD_LED_RED);
            LowLevelPrintf("IAP BoardMB85RS2MT_Init Failed\r\n");
            CoreDelayMs(100);
        }
    } while (err != LH_ERR_NONE);
    //写入BOOT版本号
    BoardVersion_WriteBootVersion();
    //编码器接口初始化
    BoardEncoderInit(BOARD_ENCODER_1);
    BoardEncoderInit(BOARD_ENCODER_2);
    //SRAM初始化
    do
    {
        err = BoardSRAM_Init();
        if (err != LH_ERR_NONE)
        {
            BoardLedToogle(BOARD_LED_RED);
            LowLevelPrintf("IAP BoardSRAM_Init Failed\r\n");
            CoreDelayMs(100);
        }
    } while (err != LH_ERR_NONE);
    //内存管理初始化
    UserMemInit(SRAM_IN);
    UserMemInit(SRAM_CCM);
    UserMemInit(SRAM_IS62);
    //IAP模块初始化,决定是否运行到APP
    BoardIAP_Init();
    //初始化之后将灯设置为初始状态
    BoardLedWrite(BOARD_LED_RED, BOARD_LED_DARK);
    BoardLedWrite(BOARD_LED_YELLOW, BOARD_LED_DARK);
    BoardLedWrite(BOARD_LED_GREEN, BOARD_LED_DARK);
    //开始运行
    LowLevelPrintf("IAP Start\r\n");
}
