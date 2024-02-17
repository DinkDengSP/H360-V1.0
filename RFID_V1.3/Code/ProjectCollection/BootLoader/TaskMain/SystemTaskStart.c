#include "TaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START];
#pragma pack()

//启动任务的堆栈
OS_TCB tcbTaskStart;

//板上外设初始化
void BoardDeviceInit(void);


//应用任务的配置
const static OS_TASK_CREATE_CONFIG appTaskConfigArray[] = {
    //CAN接收任务
    {&tcbTaskCanRecv,NAME_TASK_CAN_RECV ,TaskFuncCanRecv    ,PRIO_TASK_CAN_RECV ,&stackBufferTaskCanRecv[0] ,STK_SIZE_TASK_CAN_RECV / 10    ,STK_SIZE_TASK_CAN_RECV ,8},
    //uart1接收任务
    {&tcbTaskUart1Recv,NAME_TASK_UART1_RECV ,TaskFuncUart1Recv    ,PRIO_TASK_UART1_RECV ,&stackBufferTaskUart1Recv[0] ,STK_SIZE_TASK_UART1_RECV / 10    ,STK_SIZE_TASK_UART1_RECV ,8},
    //心跳任务
    {&tcbTaskHeart  ,NAME_TASK_HEART    ,TaskFuncHeart      ,PRIO_TASK_HEART    ,&stackBufferTaskHeart[0]   ,STK_SIZE_TASK_HEART / 10       ,STK_SIZE_TASK_HEART    ,8},
};
//应用任务的数量
#define COUNT_APP_TASK (sizeof(appTaskConfigArray) / sizeof(appTaskConfigArray[0]))


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
    /******************************************************创建应用服务任务***********************************************/
    for (indexUtil = 0; indexUtil < COUNT_APP_TASK; indexUtil++)
    {
        OSTaskCreate((OS_TCB *)appTaskConfigArray[indexUtil].tcbTaskPtr,           //任务控制块
                     (CPU_CHAR *)appTaskConfigArray[indexUtil].taskNamePtr,        //任务名字
                     (OS_TASK_PTR)appTaskConfigArray[indexUtil].taskFuncPtr,       //任务函数
                     (void *)0,                                                    //传递给任务函数的参数
                     (OS_PRIO)appTaskConfigArray[indexUtil].taskPrio,              //任务优先级
                     (CPU_STK *)appTaskConfigArray[indexUtil].taskStkBasePtr,      //任务堆栈基地址
                     (CPU_STK_SIZE)appTaskConfigArray[indexUtil].taskStkSizeLimit, //任务堆栈深度限位
                     (CPU_STK_SIZE)appTaskConfigArray[indexUtil].taskStkSize,      //任务堆栈大小
                     (OS_MSG_QTY)appTaskConfigArray[indexUtil].msgQueenSize,       //任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                     (OS_TICK)0,                                                   //当使能时间片轮转时的时间片长度，为0时为默认长度，
                     (void *)0,                                                    //用户补充的存储区
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,            //任务选项
                     (OS_ERR *)&os_err);                                           //存放该函数错误时的返回值
    }
    //退出临界区
        CPU_CRITICAL_EXIT();
    //删除start_task任务自身
        OSTaskDel((OS_TCB *)0, &os_err);
}



//板上外设初始化
void BoardDeviceInit(void)
{
    LH_ERR deviceInitResult = LH_ERR_NONE;
    //LED初始化
        BoardLedInit(BOARD_LED_GREEN,OUTPUT_VALID);
        BoardLedInit(BOARD_LED_YELLOW,OUTPUT_VALID);
        BoardLedInit(BOARD_LED_RED,OUTPUT_VALID);
    //蜂鸣器初始化
        BoardBeepInit();
    //RC522初始化
        BoardRC522_Init();
    //初始化MB85RS256
       BoardMB85RS256_Init();
    //复位并配置RC522
        do
        {
            deviceInitResult = BoardRC522_Reset();
            if(deviceInitResult != LH_ERR_NONE)
            {
                SystemPrintf("BoardRC522_Reset ErrorCode : 0X%08X\r\n",deviceInitResult);
                CoreDelayMs(200);
                BoardLedToogle(BOARD_LED_RED);
            }
        }while(deviceInitResult != LH_ERR_NONE);
   //铁电存储器自检
       do
       {
           deviceInitResult = BoardMB85RS256_Check();
           if(deviceInitResult != LH_ERR_NONE)
           {
               SystemPrintf("BoardMB85RS256_Check ErrorCode : 0X%08X\r\n",deviceInitResult);
               CoreDelayMs(200);
               BoardLedToogle(BOARD_LED_RED);
           }
       }while(deviceInitResult != LH_ERR_NONE);
    //初始化内存管理
        UserMemInit(MEM_MAIN);
    //初始化系统参数讯息
        AppParamInit();
    //获取板卡ID
        uint8_t boardID = (uint8_t)(AppParamReadBoardType(APP_UTIL_PARAM_BOARD_TYPE_INDEX_BOARD_NO));
    //CAN初始化
        MCU_CAN_Init(SELF_CAN_BPS,boardID,OS_CanReceiveDataCallFunc);
    //初始化CAN协议栈
        CanProcDataInit();
    //IAP升级检查
        BoardIapInit();
    //系统启动完成  
        SystemPrintf("BoardDeviceInit Success,BoardID %d\r\n",canBoardID);
    //蜂鸣器打开
        BoardBeepOn();
        CoreDelayMs(500);
        //关闭蜂鸣器
        BoardBeepOff();
    //关闭LED灯光
        BoardLedWrite(BOARD_LED_GREEN,OUTPUT_INVALID);
        BoardLedWrite(BOARD_LED_YELLOW,OUTPUT_INVALID);
        BoardLedWrite(BOARD_LED_RED,OUTPUT_INVALID);


}
