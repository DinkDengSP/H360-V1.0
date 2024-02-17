#include "SystemTaskConfig.h"
#include "bsp_user.h"
#include "StepMotor.h"
#include "SelfBoardID.h"
#include "ServiceIpcInc.h"
#include "AppIpcInc.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START]@".ccram";
#pragma pack()

//启动任务的堆栈
OS_TCB tcbTaskStart@".ccram";

//板上硬件初始化
static void BoardDeviceInit(void);
//Service层IPC调用结构初始化
static void ServiceIPC_DataInit(void);
//App层IPC调用结构初始化
static void AppIPC_DataInit(void);

//系统服务任务配置数组
static const OS_TASK_CREATE_CONFIG ServiceTaskArray[] = {
    {&tcbServiceTaskCan1Recv, NAME_SERVICE_TASK_CAN1_RECV, ServiceTaskFuncCan1Recv, 
        PRIO_SERVICE_TASK_CAN1_RECV, &stackBufferServiceTaskCan1Recv[0],
        STK_SIZE_SERVICE_TASK_CAN1_RECV / 10, STK_SIZE_SERVICE_TASK_CAN1_RECV, 
        MCU_CAN_RECEIVE_BUFFER_LENGTH},//CAN总线接收数据处理任务
    {&tcbServiceTaskHeart, NAME_SERVICE_TASK_HEART, ServiceTaskFuncHeart, 
        PRIO_SERVICE_TASK_HEART, &stackBufferServiceTaskHeart[0],
        STK_SIZE_SERVICE_TASK_HEART / 10, STK_SIZE_SERVICE_TASK_HEART, 
        2},//心跳任务
    {&tcbServiceTaskDebugIAP, NAME_SERVICE_TASK_DEBUG_IAP, ServiceTaskFuncDebugIAP, 
        PRIO_SERVICE_TASK_DEBUG_IAP, &stackBufferServiceTaskDebugIAP[0],
        STK_SIZE_SERVICE_TASK_DEBUG_IAP / 10, STK_SIZE_SERVICE_TASK_DEBUG_IAP, 
        2},//调试升级任务
    {&tcbServiceTaskInputRead, NAME_SERVICE_TASK_INPUT_READ, ServiceTaskFuncInputRead, 
        PRIO_SERVICE_TASK_INPUT_READ, &stackBufferServiceTaskInputRead[0],
        STK_SIZE_SERVICE_TASK_INPUT_READ / 10, STK_SIZE_SERVICE_TASK_INPUT_READ, 
        2},//输入读取任务
    {&tcbServiceTaskOutputWrite, NAME_SERVICE_TASK_OUTPUT_WRITE, ServiceTaskFuncOutputWrite, 
        PRIO_SERVICE_TASK_OUTPUT_WRITE, &stackBufferServiceTaskOutputWrite[0],
        STK_SIZE_SERVICE_TASK_OUTPUT_WRITE / 10, STK_SIZE_SERVICE_TASK_OUTPUT_WRITE, 
        2},//输出写入任务
    {&tcbServiceTaskSerialUserRS1, NAME_SERVICE_TASK_SERIAL_USER_RS1, ServiceTaskFuncSerialUserRS1, 
        PRIO_SERVICE_TASK_SERIAL_USER_RS1, &stackBufferServiceTaskSerialUserRS1[0],
        STK_SIZE_SERVICE_TASK_SERIAL_USER_RS1 / 10, STK_SIZE_SERVICE_TASK_SERIAL_USER_RS1, 
        2},//用户应用串口任务
    {&tcbServiceTaskSerialDebugRS2, NAME_SERVICE_TASK_SERIAL_DEBUG_RS2, ServiceTaskFuncSerialDebugRS2, 
        PRIO_SERVICE_TASK_SERIAL_DEBUG_RS2, &stackBufferServiceTaskSerialDebugRS2[0],
        STK_SIZE_SERVICE_TASK_SERIAL_DEBUG_RS2 / 10, STK_SIZE_SERVICE_TASK_SERIAL_DEBUG_RS2, 
        2},//系统调试串口任务
    {&tcbServiceTaskStepMotor1, NAME_SERVICE_TASK_STEP_MOTOR1, ServiceTaskFuncServiceTaskStepMotor1, 
        PRIO_SERVICE_TASK_STEP_MOTOR1, &stackBufferServiceTaskStepMotor1[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR1 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR1, 
        2},//步进电机1控制任务
    {&tcbServiceTaskStepMotor2, NAME_SERVICE_TASK_STEP_MOTOR2, ServiceTaskFuncServiceTaskStepMotor2, 
        PRIO_SERVICE_TASK_STEP_MOTOR2, &stackBufferServiceTaskStepMotor2[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR2 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR2, 
        2},//步进电机2控制任务
    {&tcbServiceTaskStepMotor3, NAME_SERVICE_TASK_STEP_MOTOR3, ServiceTaskFuncServiceTaskStepMotor3, 
        PRIO_SERVICE_TASK_STEP_MOTOR3, &stackBufferServiceTaskStepMotor3[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR3 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR3, 
        2},//步进电机3控制任务
    {&tcbServiceTaskStepMotor4, NAME_SERVICE_TASK_STEP_MOTOR4, ServiceTaskFuncServiceTaskStepMotor4, 
        PRIO_SERVICE_TASK_STEP_MOTOR4, &stackBufferServiceTaskStepMotor4[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR4 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR4, 
        2},//步进电机4控制任务
    {&tcbServiceTaskStepMotor5, NAME_SERVICE_TASK_STEP_MOTOR5, ServiceTaskFuncServiceTaskStepMotor5, 
        PRIO_SERVICE_TASK_STEP_MOTOR5, &stackBufferServiceTaskStepMotor5[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR5 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR5, 
        2},//步进电机5控制任务
    {&tcbServiceTaskStepMotor6, NAME_SERVICE_TASK_STEP_MOTOR6, ServiceTaskFuncServiceTaskStepMotor6, 
        PRIO_SERVICE_TASK_STEP_MOTOR6, &stackBufferServiceTaskStepMotor6[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR6 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR6, 
        2},//步进电机6控制任务
    {&tcbServiceTaskStepMotor7, NAME_SERVICE_TASK_STEP_MOTOR7, ServiceTaskFuncServiceTaskStepMotor7, 
        PRIO_SERVICE_TASK_STEP_MOTOR7, &stackBufferServiceTaskStepMotor7[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR7 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR7, 
        2},//步进电机7控制任务
    {&tcbServiceTaskStepMotor8, NAME_SERVICE_TASK_STEP_MOTOR8, ServiceTaskFuncServiceTaskStepMotor8, 
        PRIO_SERVICE_TASK_STEP_MOTOR8, &stackBufferServiceTaskStepMotor8[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR8 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR8, 
        2},//步进电机8控制任务
    {&tcbServiceTaskStepMotor9, NAME_SERVICE_TASK_STEP_MOTOR9, ServiceTaskFuncServiceTaskStepMotor9, 
        PRIO_SERVICE_TASK_STEP_MOTOR9, &stackBufferServiceTaskStepMotor9[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR9 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR9, 
        2}, //步进电机9控制任务
    {&tcbServiceTaskStepMotor10, NAME_SERVICE_TASK_STEP_MOTOR10, ServiceTaskFuncServiceTaskStepMotor10, 
        PRIO_SERVICE_TASK_STEP_MOTOR10, &stackBufferServiceTaskStepMotor10[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR10 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR10, 
        2},//步进电机10控制任务
    {&tcbServiceTaskStepMotor11, NAME_SERVICE_TASK_STEP_MOTOR11, ServiceTaskFuncServiceTaskStepMotor11, 
        PRIO_SERVICE_TASK_STEP_MOTOR11, &stackBufferServiceTaskStepMotor11[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR11 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR11, 
        2},//步进电机11控制任务
    {&tcbServiceTaskStepMotor12, NAME_SERVICE_TASK_STEP_MOTOR12, ServiceTaskFuncServiceTaskStepMotor12, 
        PRIO_SERVICE_TASK_STEP_MOTOR12, &stackBufferServiceTaskStepMotor12[0],
        STK_SIZE_SERVICE_TASK_STEP_MOTOR12 / 10, STK_SIZE_SERVICE_TASK_STEP_MOTOR12, 
        2},//步进电机12控制任务
    {&tcbServiceTaskParam, NAME_SERVICE_TASK_PARAM, ServiceTaskFuncServiceTaskParam, 
        PRIO_SERVICE_TASK_PARAM, &stackBufferServiceTaskParam[0],
        STK_SIZE_SERVICE_TASK_PARAM / 10, STK_SIZE_SERVICE_TASK_PARAM, 
        2},//系统参数读写任务
};
//系统服务任务的数量,用于初始化
#define COUNT_SERVICE_TASK       (sizeof(ServiceTaskArray)/sizeof(ServiceTaskArray[0]))


//系统应用任务配置数组
static const OS_TASK_CREATE_CONFIG AppTaskArray[] = {
    {&tcbAppTaskPushIn1, NAME_APP_TASK_PUSH_IN1, AppTaskFuncPushIn1, 
        PRIO_APP_TASK_PUSH_IN1, &stackBufferAppTaskPushIn1[0],
        STK_SIZE_APP_TASK_PUSH_IN1 / 10, STK_SIZE_APP_TASK_PUSH_IN1, 
        2},//应用的其他任务
    {&tcbAppTaskPushIn2, NAME_APP_TASK_PUSH_IN2, AppTaskFuncPushIn2, 
        PRIO_APP_TASK_PUSH_IN2, &stackBufferAppTaskPushIn2[0],
        STK_SIZE_APP_TASK_PUSH_IN2 / 10, STK_SIZE_APP_TASK_PUSH_IN2, 
        2},
    {&tcbAppTaskPushOut1, NAME_APP_TASK_PUSH_OUT1, AppTaskFuncPushOut1, 
        PRIO_APP_TASK_PUSH_OUT1, &stackBufferAppTaskPushOut1[0],
        STK_SIZE_APP_TASK_PUSH_OUT1 / 10, STK_SIZE_APP_TASK_PUSH_OUT1, 
        2},
    {&tcbAppTaskPushOut2, NAME_APP_TASK_PUSH_OUT2, AppTaskFuncPushOut2, 
        PRIO_APP_TASK_PUSH_OUT2, &stackBufferAppTaskPushOut2[0],
        STK_SIZE_APP_TASK_PUSH_OUT2 / 10, STK_SIZE_APP_TASK_PUSH_OUT2, 
        2},
    {&tcbAppTaskRailTrans, NAME_APP_TASK_RAIL_TRANS, AppTaskFuncRailTrans, 
        PRIO_APP_TASK_RAIL_TRANS, &stackBufferAppTaskRailTrans[0],
        STK_SIZE_APP_TASK_RAIL_TRANS / 10, STK_SIZE_APP_TASK_RAIL_TRANS, 
        2},
    {&tcbAppTaskRailHand, NAME_APP_TASK_RAIL_HAND, AppTaskFuncRailHand, 
        PRIO_APP_TASK_RAIL_HAND, &stackBufferAppTaskRailHand[0],
        STK_SIZE_APP_TASK_RAIL_HAND / 10, STK_SIZE_APP_TASK_RAIL_HAND, 
        2},
    {&tcbAppTaskRailComplex, NAME_APP_TASK_RAIL_COMPLEX, AppTaskFuncRailComplex, 
        PRIO_APP_TASK_RAIL_COMPLEX, &stackBufferAppTaskRailComplex[0],
        STK_SIZE_APP_TASK_RAIL_COMPLEX / 10, STK_SIZE_APP_TASK_RAIL_COMPLEX, 
        2},
    {&tcbAppTaskBarScan, NAME_APP_TASK_BAR_SCAN, AppTaskFuncBarScan, 
        PRIO_APP_TASK_BAR_SCAN, &stackBufferAppTaskBarScan[0],
        STK_SIZE_APP_TASK_BAR_SCAN / 10, STK_SIZE_APP_TASK_BAR_SCAN, 
        2},
    {&tcbAppTaskParam, NAME_APP_TASK_PARAM, AppTaskFuncParam, 
        PRIO_APP_TASK_PARAM, &stackBufferAppTaskParam[0],
        STK_SIZE_APP_TASK_PARAM / 10, STK_SIZE_APP_TASK_PARAM, 
        2},
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
    BoardDeviceInit();
    //进入临界区
    CPU_CRITICAL_ENTER();

    /************************************创建系统任务***********************/
    for (indexUtil = 0; indexUtil < COUNT_SERVICE_TASK; indexUtil++)
    {
        OSTaskCreate((OS_TCB *)ServiceTaskArray[indexUtil].tcbTaskPtr,           //任务控制块
                     (CPU_CHAR *)ServiceTaskArray[indexUtil].taskNamePtr,        //任务名字
                     (OS_TASK_PTR)ServiceTaskArray[indexUtil].taskFuncPtr,       //任务函数
                     (void *)0,                                                        //传递给任务函数的参数
                     (OS_PRIO)ServiceTaskArray[indexUtil].taskPrio,              //任务优先级
                     (CPU_STK *)ServiceTaskArray[indexUtil].taskStkBasePtr,      //任务堆栈基地址
                     (CPU_STK_SIZE)ServiceTaskArray[indexUtil].taskStkSizeLimit, //任务堆栈深度限位
                     (CPU_STK_SIZE)ServiceTaskArray[indexUtil].taskStkSize,      //任务堆栈大小
                     (OS_MSG_QTY)ServiceTaskArray[indexUtil].msgQueenSize,       //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                     (OS_TICK)0,                                                       //当使能时间片轮转时的时间片长度，为0时为默认长度，
                     (void *)0,                                                        //用户补充的存储区
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,                //任务选项
                     (OS_ERR *)&os_err);                                               //存放该函数错误时的返回值
    } 

    /**************************Service API调用需要的数据初始化**********************/
    ServiceIPC_DataInit();
    
    /******************************创建应用任务******************************/
    for (indexUtil = 0; indexUtil < COUNT_APP_TASK; indexUtil++)
    {
        OSTaskCreate((OS_TCB *)AppTaskArray[indexUtil].tcbTaskPtr,           //任务控制块
                     (CPU_CHAR *)AppTaskArray[indexUtil].taskNamePtr,        //任务名字
                     (OS_TASK_PTR)AppTaskArray[indexUtil].taskFuncPtr,       //任务函数
                     (void *)0,                                                        //传递给任务函数的参数
                     (OS_PRIO)AppTaskArray[indexUtil].taskPrio,              //任务优先级
                     (CPU_STK *)AppTaskArray[indexUtil].taskStkBasePtr,      //任务堆栈基地址
                     (CPU_STK_SIZE)AppTaskArray[indexUtil].taskStkSizeLimit, //任务堆栈深度限位
                     (CPU_STK_SIZE)AppTaskArray[indexUtil].taskStkSize,      //任务堆栈大小
                     (OS_MSG_QTY)AppTaskArray[indexUtil].msgQueenSize,       //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                     (OS_TICK)0,                                                       //当使能时间片轮转时的时间片长度，为0时为默认长度，
                     (void *)0,                                                        //用户补充的存储区
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,                //任务选项
                     (OS_ERR *)&os_err);                                               //存放该函数错误时的返回值
    } 

    /**************************App API调用需要的数据初始化**********************/
    AppIPC_DataInit();

    //退出临界区
    CPU_CRITICAL_EXIT();
    //删除start_task任务自身
    OSTaskDel((OS_TCB *)0, &os_err);
}

void BoardDeviceInit(void)
{
    LH_ERR err;
    //初始化RTT
    BoardRTT_Init();
    //LED初始化
    BoardLedInit(BOARD_LED_RED, BOARD_LED_LIGHT);
    BoardLedInit(BOARD_LED_GREEN, BOARD_LED_LIGHT);
    BoardLedInit(BOARD_LED_YELLOW, BOARD_LED_LIGHT);
    //串口初始化,系统串口,用于系统调试,应用串口在串口任务中进行初始化,因为每块板卡的具体实现不一样
    BoardRS_Init(RS_PORT_DEBUG,115200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,
                                MCU_UART_HARD_CONTROL_NONE,SystemUart3RecvCallBack);
    //当设置的板卡ID不是通用板的时候,检测板卡ID,必须和程序设定的板卡ID匹配
    BOARD_ID boardid;
    //主板ID初始化
    do
    {
        boardid = BoardKeyID_Init();
        if(boardid != SELF_BOARD_ID)
        {
            BoardLedToogle(BOARD_LED_RED);
            LowLevelPrintf("IAP BoardID_Init Failed\r\n");
            CoreDelayMs(100);
        }
    }while(boardid != SELF_BOARD_ID);

    //CAN总线初始化
    MCU_CAN1_Init(SELF_CAN_BPS,BoardKeyID_GetCurrent(),OS_Can1ReceiveDataCallFunc);
    
    //CPLD初始化
    do
    {
        //空函数的初始化,将不会带电机初始化
        err = BoardCPLD_Init(StepMotorCPLD_CallBackProcessWithStateMachine);
        if(err != LH_ERR_NONE)
        {
            BoardLedToogle(BOARD_LED_RED);
            LowLevelPrintf("APP BoardCPLD_Init Failed\r\n");
            CoreDelayMs(500);
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
        if(err != LH_ERR_NONE)
        {
            BoardLedToogle(BOARD_LED_RED);
            LowLevelPrintf("APP BoardMB85RS2MT_Init Failed\r\n");
            CoreDelayMs(1000);
        }
    } while (err != LH_ERR_NONE);
    //写入APP版本号
    BoardVersion_WriteAppVersion();
    //编码器接口初始化
    BoardEncoderInit(BOARD_ENCODER_1);
    BoardEncoderInit(BOARD_ENCODER_2);
    //SRAM初始化
    do
    {
        err = BoardSRAM_Init();
        if(err != LH_ERR_NONE)
        {
            BoardLedToogle(BOARD_LED_RED);
            LowLevelPrintf("APP BoardSRAM_Init Failed\r\n");
            CoreDelayMs(1500);
        }
    } while (err != LH_ERR_NONE); 
    //内存管理初始化
    UserMemInit(SRAM_IN);
    UserMemInit(SRAM_CCM);
    UserMemInit(SRAM_IS62);
    //清除IAP标志,当跳转到APP的时候自动清除,这样调试程序的时候可以直接步入APP
    BoardIAP_ClearFlag();
    //步进电机初始化
    StepMotorVarInit();
    //APP参数初始化
    AppParamInit();
    //初始化之后将灯设置为初始状态
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_DARK);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_DARK);
    BoardLedWrite(BOARD_LED_GREEN,BOARD_LED_DARK);
    //开始运行
    LowLevelPrintf("APP Start\r\n");
}

//Service层IPC调用结构初始化
static void ServiceIPC_DataInit(void)
{
    IPC_InputDataInit();//状态输入
    IPC_OutputDataInit();//数据输出
    IPC_StepMotor1DataInit();//步进电机1
    IPC_StepMotor2DataInit();//步进电机2
    IPC_StepMotor3DataInit();//步进电机3
    IPC_StepMotor4DataInit();//步进电机4
    IPC_StepMotor5DataInit();//步进电机5
    IPC_StepMotor6DataInit();//步进电机6
    IPC_StepMotor7DataInit();//步进电机7
    IPC_StepMotor8DataInit();//步进电机8
    IPC_StepMotor9DataInit();//步进电机9
    IPC_StepMotor10DataInit();//步进电机10
    IPC_StepMotor11DataInit();//步进电机11
    IPC_StepMotor12DataInit();//步进电机12
    IPC_SerialUserRS1DataInit();//应用串口
}

//App层IPC调用结构初始化
static void AppIPC_DataInit(void)
{
    IPC_PushIn1DataInit();//进样1
    IPC_PushIn2DataInit();//进样2
    IPC_PushOut1DataInit();//出样1
    IPC_PushOut2DataInit();//出样2
    IPC_RailTransDataInit();//轨道传输基础结构
    IPC_AppParamDataInit();//参数读写结构
    IPC_RailHandDataInit();//机械手基础结构
    IPC_RailComplexDataInit();//机械手和轨道结构
    IPC_BarScanDataInit();//条码扫描
}
