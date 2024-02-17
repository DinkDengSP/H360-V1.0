/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-08 18:20:28
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-11 17:22:10
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SystemTaskConfig.h"
#include "ServiceTaskConfig.h"
#include "AppTaskConfig.h"
#include "ServiceStatePrivate.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskStart[STK_SIZE_TASK_START]@".dtcm_ram";
#pragma pack()

extern uint8_t programForceJumpFlag;

//启动任务的堆栈
OS_TCB tcbTaskStart@".dtcm_ram";

//板上外设初始化
void BoardDeviceInit(void);
//需要根据参数初始化外设
void BoardDeviceInitByParam(void);

//服务任务的配置
const static OS_TASK_CREATE_CONFIG serviceTaskConfigArray[] = {
    //CAN2数据处理
    {&tcbTaskServiceCan2Recv            ,NAME_TASK_SERVICE_CAN2_RECV                ,TaskFuncServiceCan2Recv                ,PRIO_TASK_SERVICE_CAN2_RECV                ,&stackBufferTaskServiceCan2Recv[0]             ,STK_SIZE_TASK_SERVICE_CAN2_RECV / 10               ,STK_SIZE_TASK_SERVICE_CAN2_RECV                ,MCU_CAN_MSG_FIFO_LENGTH        },    
    //系统辅助任务,例如调试,心跳等                              
    {&tcbTaskServiceAssist              ,NAME_TASK_SERVICE_ASSIST                   ,TaskFuncServiceAssist                  ,PRIO_TASK_SERVICE_ASSIST                   ,&stackBufferTaskServiceAssist[0]               ,STK_SIZE_TASK_SERVICE_ASSIST / 10                  ,STK_SIZE_TASK_SERVICE_ASSIST                   ,1                              },                                      
    //液路自动化任务                                                                
    {&tcbTaskServiceLiquidAuto          ,NAME_TASK_SERVICE_LIQUID_AUTO              ,TaskFuncServiceLiquidAuto              ,PRIO_TASK_SERVICE_LIQUID_AUTO              ,&stackBufferTaskServiceLiquidAuto[0]           ,STK_SIZE_TASK_SERVICE_LIQUID_AUTO / 10             ,STK_SIZE_TASK_SERVICE_LIQUID_AUTO              ,1                              },                            
    //样本针任务                                                                
    {&tcbTaskServiceNeedleSample        ,NAME_TASK_SERVICE_NEEDLE_SAMPLE            ,TaskFuncServiceNeedleSample            ,PRIO_TASK_SERVICE_NEEDLE_SAMPLE            ,&stackBufferTaskServiceNeedleSample[0]         ,STK_SIZE_TASK_SERVICE_NEEDLE_SAMPLE / 10           ,STK_SIZE_TASK_SERVICE_NEEDLE_SAMPLE            ,1                              },                        
    //试剂针任务                                                                
    {&tcbTaskServiceNeedleReagent       ,NAME_TASK_SERVICE_NEEDLE_REAGENT           ,TaskFuncServiceNeedleReagent           ,PRIO_TASK_SERVICE_NEEDLE_REAGENT           ,&stackBufferTaskServiceNeedleReagent[0]        ,STK_SIZE_TASK_SERVICE_NEEDLE_REAGENT / 10          ,STK_SIZE_TASK_SERVICE_NEEDLE_REAGENT           ,1                              },                      
    //磁珠针任务                                                                
    {&tcbTaskServiceNeedleBead          ,NAME_TASK_SERVICE_NEEDLE_BEAD              ,TaskFuncServiceNeedleBead              ,PRIO_TASK_SERVICE_NEEDLE_BEAD              ,&stackBufferTaskServiceNeedleBead[0]           ,STK_SIZE_TASK_SERVICE_NEEDLE_BEAD / 10             ,STK_SIZE_TASK_SERVICE_NEEDLE_BEAD              ,1                              },                            
    //清洗针任务                                                                
    {&tcbTaskServiceNeedleWash          ,NAME_TASK_SERVICE_NEEDLE_WASH              ,TaskFuncServiceNeedleWash              ,PRIO_TASK_SERVICE_NEEDLE_WASH              ,&stackBufferTaskServiceNeedleWash[0]           ,STK_SIZE_TASK_SERVICE_NEEDLE_WASH / 10             ,STK_SIZE_TASK_SERVICE_NEEDLE_WASH              ,1                              },                            
    //反应盘任务                                                                
    {&tcbTaskServicePlateReaction       ,NAME_TASK_SERVICE_PLATE_REACTION           ,TaskFuncServicePlateReaction           ,PRIO_TASK_SERVICE_PLATE_REACTION           ,&stackBufferTaskServicePlateReaction[0]        ,STK_SIZE_TASK_SERVICE_PLATE_REACTION / 10          ,STK_SIZE_TASK_SERVICE_PLATE_REACTION           ,1                              },                      
    //试剂盘任务                                                                
    {&tcbTaskServicePlateReagent        ,NAME_TASK_SERVICE_PLATE_REAGENT            ,TaskFuncServicePlateReagent            ,PRIO_TASK_SERVICE_PLATE_REAGENT            ,&stackBufferTaskServicePlateReagent[0]         ,STK_SIZE_TASK_SERVICE_PLATE_REAGENT / 10           ,STK_SIZE_TASK_SERVICE_PLATE_REAGENT            ,1                              },                        
    //清洗盘任务                                                                
    {&tcbTaskServicePlateWash           ,NAME_TASK_SERVICE_PLATE_WASH               ,TaskFuncServicePlateWash               ,PRIO_TASK_SERVICE_PLATE_WASH               ,&stackBufferTaskServicePlateWash[0]            ,STK_SIZE_TASK_SERVICE_PLATE_WASH / 10              ,STK_SIZE_TASK_SERVICE_PLATE_WASH               ,1                              },                              
    //新杯机械手                                                                
    {&tcbTaskServiceHandNewCup          ,NAME_TASK_SERVICE_HAND_NEW_CUP             ,TaskFuncServiceHandNewCup              ,PRIO_TASK_SERVICE_HAND_NEW_CUP             ,&stackBufferTaskServiceHandNewCup[0]           ,STK_SIZE_TASK_SERVICE_HAND_NEW_CUP / 10            ,STK_SIZE_TASK_SERVICE_HAND_NEW_CUP             ,1                              },                          
    //清洗机械手                                                                
    {&tcbTaskServiceHandWash            ,NAME_TASK_SERVICE_HAND_WASH                ,TaskFuncServiceHandWash                ,PRIO_TASK_SERVICE_HAND_WASH                ,&stackBufferTaskServiceHandWash[0]             ,STK_SIZE_TASK_SERVICE_HAND_WASH / 10               ,STK_SIZE_TASK_SERVICE_HAND_WASH                ,1                              },                                
    //磁珠摇匀                                                              
    {&tcbTaskServiceMixBead             ,NAME_TASK_SERVICE_MIX_BEAD                 ,TaskFuncServiceMixBead                 ,PRIO_TASK_SERVICE_MIX_BEAD                 ,&stackBufferTaskServiceMixBead[0]              ,STK_SIZE_TASK_SERVICE_MIX_BEAD / 10                ,STK_SIZE_TASK_SERVICE_MIX_BEAD                 ,1                              },                                  
    //试剂摇匀                                                              
    {&tcbTaskServiceMixReagent          ,NAME_TASK_SERVICE_MIX_REAGENT              ,TaskFuncServiceMixReagent              ,PRIO_TASK_SERVICE_MIX_REAGENT              ,&stackBufferTaskServiceMixReagent[0]           ,STK_SIZE_TASK_SERVICE_MIX_REAGENT / 10             ,STK_SIZE_TASK_SERVICE_MIX_REAGENT              ,1                              },                            
    //清洗摇匀                                                              
    {&tcbTaskServiceMixWash             ,NAME_TASK_SERVICE_MIX_WASH                 ,TaskFuncServiceMixWash                 ,PRIO_TASK_SERVICE_MIX_WASH                 ,&stackBufferTaskServiceMixWash[0]              ,STK_SIZE_TASK_SERVICE_MIX_WASH / 10                ,STK_SIZE_TASK_SERVICE_MIX_WASH                 ,1                              },                                  
    //缓冲A液注液                               
    {&tcbTaskServiceBufferSolutionInject,NAME_TASK_SERVICE_BUFFER_SOLUTION_INJECT   ,TaskFuncServiceBufferSolutionInject    ,PRIO_TASK_SERVICE_BUFFER_SOLUTION_INJECT   ,&stackBufferTaskServiceBufferSolutionInject[0] ,STK_SIZE_TASK_SERVICE_BUFFER_SOLUTION_INJECT / 10  ,STK_SIZE_TASK_SERVICE_BUFFER_SOLUTION_INJECT   ,1                              },      
    //杯栈托盘管理                              
    {&tcbTaskServiceCupTrayStackManage  ,NAME_TASK_SERVICE_CUP_TRAY_STACK_MANAGE    ,TaskFuncServiceCupTrayStackManage      ,PRIO_TASK_SERVICE_CUP_TRAY_STACK_MANAGE    ,&stackBufferTaskServiceCupTrayStackManage[0]   ,STK_SIZE_TASK_SERVICE_CUP_TRAY_STACK_MANAGE / 10   ,STK_SIZE_TASK_SERVICE_CUP_TRAY_STACK_MANAGE    ,1                              },        
    //测量模块                              
    {&tcbTaskServiceMeasureModule       ,NAME_TASK_SERVICE_MEASURE_MODULE           ,TaskFuncServiceMeasureModule           ,PRIO_TASK_SERVICE_MEASURE_MODULE           ,&stackBufferTaskServiceMeasureModule[0]        ,STK_SIZE_TASK_SERVICE_MEASURE_MODULE / 10          ,STK_SIZE_TASK_SERVICE_MEASURE_MODULE           ,1                              },                                     
    //温控板                                    
    {&tcbTaskServiceTemperatureControl  ,NAME_TASK_SERVICE_TEMPERATURE_CONTROL      ,TaskFuncServiceTemperatureControl      ,PRIO_TASK_SERVICE_TEMPERATURE_CONTROL      ,&stackBufferTaskServiceTemperatureControl[0]   ,STK_SIZE_TASK_SERVICE_TEMPERATURE_CONTROL / 10     ,STK_SIZE_TASK_SERVICE_TEMPERATURE_CONTROL      ,1                              },            
    //文件系统日志                                    
    {&tcbTaskServiceFatFsLog            ,NAME_TASK_SERVICE_FATFS_LOG                ,TaskFuncServiceFatFsLog                ,PRIO_TASK_SERVICE_FATFS_LOG                ,&stackBufferTaskServiceFatFsLog[0]             ,STK_SIZE_TASK_SERVICE_FATFS_LOG / 10               ,STK_SIZE_TASK_SERVICE_FATFS_LOG                ,512                            },
    //文件系统日志                                    
    {&tcbTaskServiceTaskSyn             ,NAME_TASK_SERVICE_TASK_SYNCHRONIZATION     ,TaskFuncServiceTaskSyn                 ,PRIO_TASK_SERVICE_TASK_SYNCHRONIZATION     ,&stackBufferTaskServiceTaskSyn[0]              ,STK_SIZE_TASK_SERVICE_TASK_SYNCHRONIZATION / 10    ,STK_SIZE_TASK_SERVICE_TASK_SYNCHRONIZATION     ,1                              },
};
//服务任务的数量
#define COUNT_SERVICE_TASK (sizeof(serviceTaskConfigArray) / sizeof(serviceTaskConfigArray[0]))

//应用任务的配置
const static OS_TASK_CREATE_CONFIG appTaskConfigArray[] = {
    //CAN1接收数据处理任务
    {&tcbTaskAppCan1Recv        ,NAME_TASK_APP_CAN1_RECV    ,TaskFuncAppCan1Recv    ,PRIO_TASK_APP_CAN1_RECV    ,&stackBufferTaskAppCan1Recv[0] ,STK_SIZE_TASK_APP_CAN1_RECV / 10   ,STK_SIZE_TASK_APP_CAN1_RECV    ,MCU_CAN_MSG_FIFO_LENGTH     }, 
    {&tcbTaskAppCan1Send        ,NAME_TASK_APP_CAN1_SEND    ,TaskFuncAppCan1Send    ,PRIO_TASK_APP_CAN1_SEND    ,&stackBufferTaskAppCan1Send[0] ,STK_SIZE_TASK_APP_CAN1_SEND / 10   ,STK_SIZE_TASK_APP_CAN1_SEND    ,MCU_CAN_MSG_FIFO_LENGTH     }, 
    //应用流程测试任务
    {&tcbTaskAppTest            ,NAME_TASK_APP_TEST         ,TaskFuncAppTest        ,PRIO_TASK_APP_TEST         ,&stackBufferTaskAppTest[0]     ,STK_SIZE_TASK_APP_TEST / 10        ,STK_SIZE_TASK_APP_TEST         , 128                        },      
};
//应用任务的数量
#define COUNT_APP_TASK (sizeof(appTaskConfigArray) / sizeof(appTaskConfigArray[0]))

//IPC数据结构初始化
void ServiceIpcDataInit(void);

//日志文件通道
static LOG_FILE_CHANNEL_SELECT logFileChannel = LOG_FILE_CHANNEL_SELECT_USART;
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
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP|OS_OPT_TASK_NO_TLS,                //任务选项
                     (OS_ERR *)&os_err);                                               //存放该函数错误时的返回值
    }
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
                     (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP|OS_OPT_TASK_NO_TLS,            //任务选项
                     (OS_ERR *)&os_err);                                           //存放该函数错误时的返回值
    }
    //服务层IPC数据结构初始化
        ServiceIpcDataInit();
    //服务层系统状态和系统参数初始化
        ServiceStateInit();
        ServiceParamInit();
    //退出临界区
        CPU_CRITICAL_EXIT();
    //需要根据参数初始化外设
        BoardDeviceInitByParam();
    //设置日志文件通道
        ServiceStateWriteAssistSetLogChannel(logFileChannel);
    //删除start_task任务自身
        OSTaskDel((OS_TCB *)0, &os_err);
}

//需要根据参数初始化外设
void BoardDeviceInitByParam(void)
{
    int32_t paramUtil = 0;
    //读条码枪版本参数
    ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_PLATE_REAGENT,INDEX_SUB_SERVICE_PARAM_PLATE_REAGENT_BARCODE_SCAN_VERSION,&paramUtil);
    //条码枪初始化
    if(paramUtil == 0)
    {
        //只有在使用条码枪的情况下,日志端口才有的选
        ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_PLATE_REAGENT,INDEX_SUB_SERVICE_PARAM_PLATE_REAGENT_RFID_LOG_CHANNEL,&paramUtil);
        //日志使用RS1,也就是串口3
        if(paramUtil == 0)
        {
            ServiceStateWritePlateReagentRFIDLogChannel(PLATE_REAGENT_RFID_LOG_CHANNEL0);
            //初始化RS1 USART3 Version0 LOG
            BoardRS_Init(BOARD_RS1_UART3_VERSION0, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE, NULL);
        }
        //日志使用RS2,也就是串口1
        else
        {
            ServiceStateWritePlateReagentRFIDLogChannel(PLATE_REAGENT_RFID_LOG_CHANNEL1);
            //初始化RS2 USART1 Version0 LOG
            BoardRS_Init(BOARD_RS2_UART1_VERSION0, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE, NULL);
        }
        //扫码用条码枪
        ServiceStateWritePlateReagentBarCodeScanVersion(PLATE_REAGENT_BARCODE_VERSION0);
        //条码枪串口初始化
        BoardQRScanCM200Init();

    }
    //RFID初始化
    else if(paramUtil == 1)
    {
        //如果启用了RFID则日志端口必须用RS2,也就是串口1
        ServiceStateWritePlateReagentRFIDLogChannel(PLATE_REAGENT_RFID_LOG_CHANNEL1);
        //初始化RS2 USART1 Version0 LOG
        BoardRS_Init(BOARD_RS2_UART1_VERSION0, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE, NULL);
        //扫码用RFID
        ServiceStateWritePlateReagentBarCodeScanVersion(PLATE_REAGENT_BARCODE_VERSION1); 
        //初始化RS1 USART3 Version1 RFID 
        QRScanRFIDInit();     
    }
    //日志端口选择是串口还是SD卡
    ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_PLATE_REAGENT,INDEX_SUB_SERVICE_PARAM_PLATE_REAGENT_LOG_CHANNEL_UART_OR_SDCARD,&paramUtil);
    //参数选择是SD卡,并且初始化成功才启用
    if(paramUtil == 1 && logFileChannel == LOG_FILE_CHANNEL_SELECT_SD_CARD)
    {
        logFileChannel = LOG_FILE_CHANNEL_SELECT_SD_CARD;
    }
    //其他情况都有串口输出
    else
    {
        logFileChannel = LOG_FILE_CHANNEL_SELECT_USART;
    }
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
    //模块号识别初始化
        BoardModuleID_Init();
    //随机数初始化
        MCU_RandomInit();
    //还剩下一个串口,目前还不知道做啥用,RS4 UART6
        BoardRS_Init(BOARD_RS4_UART6, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE, NULL);
    //获取打印模块号
        // SystemPrintf("ModuleNO : %d\r\n",BoardModuleRead());
    //压力传感器初始化
        BoardPressureSensorInit();
    //显示一下当前芯片温度
        // SystemPrintf("CurrentChipTemp : %2.2f\r\n",MCU_ADC_ReadChipTemp());
    //RTC初始化
        do
        {
            deviceResult = MCU_RTC_Init();
            if(deviceResult == LH_ERR_NONE)
            {
                break;
            }
            // SystemPrintf("MCU_RTC_Init ErrorCode : 0X%08X\r\n",deviceResult);
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(100);
        }while(1);
    //SRAM初始化
        do
        {
            deviceResult = BoardSRAM_Init();
            if(deviceResult == LH_ERR_NONE)
            {
                break;
            }
            // SystemPrintf("BoardSRAM_Init ErrorCode : 0X%08X\r\n",deviceResult);
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
            // SystemPrintf("BoardMB85RS2MT_Init ErrorCode : 0X%08X\r\n",deviceResult);
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(100);
        }while(1);
        BoardVersion_WriteAppVersion();
    //W5500初始化
        do
        {
            deviceResult = BoardW5500_Init(NULL);
            if(deviceResult == LH_ERR_NONE)
            {
                break;
            }
            // SystemPrintf("BoardW5500_Init ErrorCode : 0X%08X\r\n",deviceResult);
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(100);
        }while(0);
    //SD卡初始化
        do
        {
            deviceResult = BoardSD_Init();
            if(deviceResult == LH_ERR_NONE)
            {
                logFileChannel = LOG_FILE_CHANNEL_SELECT_SD_CARD;
                break;
            }
            logFileChannel = LOG_FILE_CHANNEL_SELECT_USART;
            // SystemPrintf("BoardSD_Init ErrorCode : 0X%08X\r\n",deviceResult);
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(100);
        }while(0);//不要求初始化
    //读取强制跳转标志
        if(BoardIAP_ReadJumpFlag() == 0x5A)
        {
            programForceJumpFlag = 0xFF;
            BoardIAP_ClearJumpFlag();
        }
    //清除IAP标志
        BoardIAP_ClearFlag();
    //写入APP版本号
        BoardVersion_WriteAppVersion();
    //beep响铃展示,系统每次复位都会响铃,通知系统启动
        BoardBeepSetState(BEEP_ON);
    //延时发生,等待系统内部其他模块启动
        CoreDelayMs(500);
    //关闭蜂鸣器
        BoardBeepSetState(BEEP_OFF);
    //初始化完成,全部关闭
        BoardLedWrite(BOARD_LED_RED,BOARD_LED_DARK);
        BoardLedWrite(BOARD_LED_GREEN,BOARD_LED_DARK);
        BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_DARK);
}


//IPC数据结构初始化
void ServiceIpcDataInit(void)
{
    //辅助任务
    IPC_ServiceAssistDataInit();
    //缓冲液注液
    IPC_ServiceBufferSolutionInjectDataInit();
    //杯栈管理
    IPC_ServiceCupTrayStackManageDataInit();
    //新杯机械手
    IPC_ServiceHandNewCupDataInit();
    //清洗机械手
    IPC_ServiceHandWashDataInit();
    //液路自动
    IPC_ServiceLiquidAutoDataInit();
    //测量模块
    IPC_ServiceMeasureModuleDataInit();
    //磁珠混匀
    IPC_ServiceMixBeadDataInit();
    //试剂混匀
    IPC_ServiceMixReagentDataInit();
    //清洗混匀
    IPC_ServiceMixWashDataInit();
    //磁珠针
    IPC_ServiceNeedleBeadDataInit();
    //试剂针
    IPC_ServiceNeedleReagentDataInit();
    //样本针
    IPC_ServiceNeedleSampleDataInit();
    //清洗针
    IPC_ServiceNeedleWashDataInit();
    //反应盘
    IPC_ServicePlateReactionDataInit();
    //试剂盘
    IPC_ServicePlateReagentDataInit();
    //清洗盘
    IPC_ServicePlateWashDataInit();
    //温度控制
    IPC_ServiceTemperatureControlDataInit();
    //FatFsLog
    IPC_ServiceFatFsLogDataInit();
}
