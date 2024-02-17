/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:06:30
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-07 15:31:13
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceTaskRailTrans.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskServiceRailTrans[STK_SIZE_TASK_SERVICE_RAIL_TRANS]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskServiceRailTrans@".dtcm_ram";

//自动运行程序的执行周期,单位毫秒
#define RAIL_TRANS_AUTO_FUNC_RUNNING_PERIOD_MS            200
//系统循环自动程序初始化
static void ServiceTaskFuncRailTrans_Auto_Init(void);
#if(RAIL_TRANS_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void ServiceTaskFuncRailTrans_Auto_Proc(void);
#endif


//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncRailTrans_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr);
//2. 来自自身的消息
static void ServiceTaskFuncRailTrans_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr);


//任务函数
void TaskFuncServiceRailTrans(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //状态机初始化
    ServiceTaskFuncRailTrans_Auto_Init();
    #if(RAIL_TRANS_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(RAIL_TRANS_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= RAIL_TRANS_AUTO_FUNC_RUNNING_PERIOD_MS)
		{
			//上次发送时间和本次发送时间差了RAIL_TRANS_AUTO_FUNC_RUNNING_PERIOD_MS个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			ServiceTaskFuncRailTrans_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)RAIL_TRANS_AUTO_FUNC_RUNNING_PERIOD_MS,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(SYSTEM_CMD_SRC_CAN1 == taskCommandPtr->cmdSrc)
            {
                //从CAN1传过来的指令
                CAN1_CMD_RECV* can1RecvDatPtr = (CAN1_CMD_RECV*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncRailTrans_Can1Msg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(MEM_DTCM,can1RecvDatPtr);
            }
            else if(SYSTEM_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身其他任务传送来的指令
                SYSTEM_CMD_SELF* selfCmdDataPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncRailTrans_SelfMsg_Proc(selfCmdDataPtr);
                //处理完成
                UserMemFree(MEM_DTCM,selfCmdDataPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            taskCommandPtr->cmdDataPtr = NULL;
            UserMemFree(MEM_DTCM,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}

/************************************************自动运行程序************************************************/
//系统循环自动程序初始化
static void ServiceTaskFuncRailTrans_Auto_Init(void)
{
    ServiceImplRailTrans_EventInit();
}
#if(RAIL_TRANS_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统循环运行程序,处理自动化任务
static void ServiceTaskFuncRailTrans_Auto_Proc(void)
{
    ServiceImplRailTrans_EventRun(RAIL_TRANS_AUTO_FUNC_RUNNING_PERIOD_MS);
}
#endif



/*************************************************1. 来自CAN1的消息******************************************/
//轨道传送带复位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Reset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带电机走指定步数
void ServiceTaskFuncRailTrans_Can1Msg_Proc_StepMotorRunSteps(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带输入读取
void ServiceTaskFuncRailTrans_Can1Msg_Proc_InputRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带输出写入
void ServiceTaskFuncRailTrans_Can1Msg_Proc_OutputWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到进样1
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushIn1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到进样2
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushIn2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到急诊位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2Emergency(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到待测位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2WaitTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到测试区取架位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TestingCatch(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到测试区吸样位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TestingAbsorb(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到测试区放架位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TestingPut(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到旋转位1
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2Rotate1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到旋转位2
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2Rotate2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到试管架扫码位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2RackScan(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到试管扫码位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TubeScan(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到重测位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2RepeatTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到出样位1
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushOut1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带移动到出样位2
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushOut2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带锁定电磁铁
void ServiceTaskFuncRailTrans_Can1Msg_Proc_LockEmergency(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带解锁电磁铁
void ServiceTaskFuncRailTrans_Can1Msg_Proc_UnlockEmergency(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道集成扫码
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2BarScanIntegrate(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到集成扫码指定位置
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2BarScanIntegrateTargetPos(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT RailTransCan1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOTOR_RESET              ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Reset                ,"Reset"                },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOTOR_RUN_STEPS          ,ServiceTaskFuncRailTrans_Can1Msg_Proc_StepMotorRunSteps    ,"StepMotorRunSteps"    },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_PUSHIN1           ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushIn1         ,"Move2PushIn1"         },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_PUSHIN2           ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushIn2         ,"Move2PushIn2"         },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_EMERGENCY         ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2Emergency       ,"Move2Emergency"       },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_WAIT_TEST         ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2WaitTest        ,"Move2WaitTest"        },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_TESTING_CATCH     ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TestingCatch    ,"Move2TestingCatch"    },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_TESTING_ABSORB    ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TestingAbsorb   ,"Move2TestingAbsorb"   },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_TESTING_PUT       ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TestingPut      ,"Move2TestingPut"      },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_ROTATE1           ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2Rotate1         ,"Move2Rotate1"         },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_ROTATE2           ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2Rotate2         ,"Move2Rotate2"         },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_RACK_BARSCAN      ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2RackScan        ,"Move2RackScan"        },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_TUBE_BARSCAN      ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TubeScan        ,"Move2TubeScan"        },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_REPEAT_TEST       ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2RepeatTest      ,"Move2RepeatTest"      },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_PUSHOUT1          ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushOut1        ,"Move2PushOut1"        },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_PUSHOUT2          ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushOut2        ,"Move2PushOut2"        },
    {CAN1_CMD_DEBUG_RAIL_TRANS_LOCK                     ,ServiceTaskFuncRailTrans_Can1Msg_Proc_LockEmergency        ,"LockEmergency"        },
    {CAN1_CMD_DEBUG_RAIL_TRANS_UNLOCK                   ,ServiceTaskFuncRailTrans_Can1Msg_Proc_UnlockEmergency      ,"UnlockEmergency"      },
    {CAN1_CMD_DEBUG_RAIL_TRANS_INPUT_READ               ,ServiceTaskFuncRailTrans_Can1Msg_Proc_InputRead            ,"InputRead"            },
    {CAN1_CMD_DEBUG_RAIL_TRANS_OUTPUT_WRITE             ,ServiceTaskFuncRailTrans_Can1Msg_Proc_OutputWrite          ,"OutputWrite"          },
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_BARSCAN           ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2BarScanIntegrate          ,"Move2BarScanIntegrate"},
    {CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_BARSCAN_TARGETPOS ,ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2BarScanIntegrateTargetPos,"Move2BarScanIntegrateTargetPos"},
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_RAIL_TRANS_CMD_COUNT        (sizeof(RailTransCan1CommandProcFuncPtrArray)/sizeof(RailTransCan1CommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncRailTrans_Can1Msg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_RAIL_TRANS_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == RailTransCan1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(RailTransCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                RailTransCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,RailTransCan1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//轨道传送带复位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Reset(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransReset(RAIL_RESET_CORRECT_ON,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //返回数据
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOTOR_RESET,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带电机走指定步数
void ServiceTaskFuncRailTrans_Can1Msg_Proc_StepMotorRunSteps(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //运行方向
    uint8_t dir = can1CommandPtr->recvDataBuffer[0];
    //运动步数
    uint32_t steps = Can1ProcUtilConvertArrayToUint32(can1CommandPtr->recvDataBuffer,1);
    //运动完坐标
    int32_t posAfterRun = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,dir,steps,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMotorRunSteps(dir,steps,&posAfterRun,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[4] = {0};
    Can1ProcUtilConvertInt32ToArray(returnData,0,posAfterRun);
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOTOR_RUN_STEPS,can1Error,reportLevelResult,actionResult,4,returnData);
}

//轨道传送带输入读取
void ServiceTaskFuncRailTrans_Can1Msg_Proc_InputRead(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //输入序号
    uint8_t pinIndex = can1CommandPtr->recvDataBuffer[0];
    //读取到的输入值
    uint8_t pinValue = 0;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,pinIndex,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransInputRead(pinIndex,&pinValue,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[2] = {0};
    returnData[0] = pinIndex;
    returnData[1] = pinValue;
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_INPUT_READ,can1Error,reportLevelResult,actionResult,2,returnData);
}

//轨道传送带输出写入
void ServiceTaskFuncRailTrans_Can1Msg_Proc_OutputWrite(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    //输出序号
    uint8_t pinIndex = can1CommandPtr->recvDataBuffer[0];
    //输出值
    uint8_t pinValue = can1CommandPtr->recvDataBuffer[1];
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,pinIndex,pinValue,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransOutPutWrite(pinIndex,pinValue,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    uint8_t returnData[1] = {0};
    returnData[0] = pinIndex;
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_OUTPUT_WRITE,can1Error,reportLevelResult,actionResult,1,returnData);
}

//轨道传送带移动到进样1
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushIn1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2PushIn1(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_PUSHIN1,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到进样2
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushIn2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2PushIn2(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_PUSHIN2,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到急诊位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2Emergency(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2EmergencyCall(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_EMERGENCY,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到待测位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2WaitTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    RAIL_WAITTEST_NO waitTestNo = (RAIL_WAITTEST_NO)(can1CommandPtr->recvDataBuffer[1]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,waitTestNo,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2WaitTest(moduleNo,waitTestNo,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_WAIT_TEST,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到测试区取架位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TestingCatch(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    RAIL_TUBE_POS tubeOffset = (RAIL_TUBE_POS)(can1CommandPtr->recvDataBuffer[1]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,tubeOffset,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2TestingCatch(moduleNo,tubeOffset,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_TESTING_CATCH,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到测试区吸样位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TestingAbsorb(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    RAIL_TUBE_POS tubeOffset = (RAIL_TUBE_POS)(can1CommandPtr->recvDataBuffer[1]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,tubeOffset,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2TestingAbsorb(moduleNo,tubeOffset,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_TESTING_ABSORB,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到测试区放架位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TestingPut(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    RAIL_TUBE_POS tubeOffset = (RAIL_TUBE_POS)(can1CommandPtr->recvDataBuffer[1]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,tubeOffset,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2TestingPut(moduleNo,tubeOffset,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_TESTING_PUT,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到旋转位1
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2Rotate1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2Rotate1(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_ROTATE1,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到旋转位2
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2Rotate2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2Rotate2(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_ROTATE2,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到试管架扫码位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2RackScan(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2RackBarScan(RAIL_BAR_SCAN_POS_MID,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_RACK_BARSCAN,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到试管扫码位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2TubeScan(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_TUBE_POS tubePos = (RAIL_TUBE_POS)(can1CommandPtr->recvDataBuffer[0]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,tubePos,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2TubeBarScan(tubePos,RAIL_BAR_SCAN_POS_MID,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_TUBE_BARSCAN,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到重测位
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2RepeatTest(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(can1CommandPtr->recvDataBuffer[0]);
    RAIL_RETEST_NO repeatTestNo = (RAIL_RETEST_NO)(can1CommandPtr->recvDataBuffer[1]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,repeatTestNo,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2RepeatTest(moduleNo,repeatTestNo,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_REPEAT_TEST,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到出样位1
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushOut1(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2PushOut1(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_PUSHOUT1,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到出样位2
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2PushOut2(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2PushOut2(&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_PUSHOUT2,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带锁定电磁铁
void ServiceTaskFuncRailTrans_Can1Msg_Proc_LockEmergency(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransLockSet(RAIL_OUTPUT_STATE_VALID,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_LOCK,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带解锁电磁铁
void ServiceTaskFuncRailTrans_Can1Msg_Proc_UnlockEmergency(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //解析指令参数
    asm("nop");
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransLockSet(RAIL_OUTPUT_STATE_INVALID,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_UNLOCK,can1Error,reportLevelResult,actionResult,0,NULL);
}

//轨道传送带移动到扫码位并扫码
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2BarScanIntegrate(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    uint8_t *barScanResultData = NULL;
    /* 申请内存 */
    do
    {
        barScanResultData = UserMemMalloc(MEM_SRAM,200);
        if(barScanResultData == NULL)
        {
            CoreDelayMinTick();
        }
    }while(barScanResultData == NULL);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2BarScanIntegrate(barScanResultData,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_BARSCAN,can1Error,reportLevelResult,actionResult,162,barScanResultData);
    /* 释放内存 */
    if(barScanResultData != NULL)
    {
        UserMemFree(MEM_SRAM,barScanResultData);
    }
}
//轨道移动到集成扫码指定位置
void ServiceTaskFuncRailTrans_Can1Msg_Proc_Move2BarScanIntegrateTargetPos(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //底层错误代码
    LH_ERR actionResult = LH_ERR_NONE;
    //中位机错误代码
    CAN1_REPORT_ERR_CODE can1Error;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportLevelResult;
    /* 参数解析 */
    RAIL_BAR_SCAN_POS scanPos = RAIL_BAR_SCAN_POS_START;
    scanPos = (RAIL_BAR_SCAN_POS)(can1CommandPtr->recvDataBuffer[0]);
    //打印调试代码
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,scanPos,0,0,0,0);
    //调用实现函数
    actionResult = ServiceImplRailTransMove2BarScanPosIntegrate(scanPos,&can1Error,&reportLevelResult);
    //打印结果代码
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
    //该返回的数据返回
    Can1SendDebugResponseMsg(CAN1_CMD_DEBUG_RAIL_TRANS_MOVE_2_BARSCAN_TARGETPOS,can1Error,reportLevelResult,actionResult,0,NULL);
}

/*************************************************2. 来自自身的消息******************************************/
//轨道运转复位
static LH_ERR RailTransSelfCommandProcFunc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到进样1
static LH_ERR RailTransSelfCommandProcFunc_Move2PushIn1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到进样2
static LH_ERR RailTransSelfCommandProcFunc_Move2PushIn2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到出样1
static LH_ERR RailTransSelfCommandProcFunc_Move2PushOut1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到出样2
static LH_ERR RailTransSelfCommandProcFunc_Move2PushOut2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到急诊区
static LH_ERR RailTransSelfCommandProcFunc_Move2EmergencyCall(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到旋转位1
static LH_ERR RailTransSelfCommandProcFunc_Move2Rotate1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到旋转位2
static LH_ERR RailTransSelfCommandProcFunc_Move2Rotate2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到距离比较近的旋转位
static LH_ERR RailTransSelfCommandProcFunc_Move2RotateAuto(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到试管架扫码位
static LH_ERR RailTransSelfCommandProcFunc_Move2RackBarScan(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到指定试管扫码位
static LH_ERR RailTransSelfCommandProcFunc_Move2TubeBarScan(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到指定模块待测位
static LH_ERR RailTransSelfCommandProcFunc_Move2WaitTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到指定模块重测位
static LH_ERR RailTransSelfCommandProcFunc_Move2RepeatTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到指定模块测试区放架位
static LH_ERR RailTransSelfCommandProcFunc_Move2TestingPut(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到指定模块测试区取架位
static LH_ERR RailTransSelfCommandProcFunc_Move2TestingCatch(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到指定模块测试区吸样位
static LH_ERR RailTransSelfCommandProcFunc_Move2TestingAbsorb(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带获取急诊区状态
static LH_ERR RailTransSelfCommandProcFunc_GetEmergencyState(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道传送带急诊锁定
static LH_ERR RailTransSelfCommandProcFunc_LockSet(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道集成扫码
static LH_ERR RailTransSelfCommandProcFunc_Move2BarScanIntegrate(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//轨道运行到集成扫码指定位置-起始,结束
static LH_ERR RailTransSelfCommandProcFunc_Move2BarScanPosIntegrate(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr);
//回调函数数组
const SYSTEM_TASK_SELF_MSG_PROC_UNIT RailTransSelfCommandProcFuncPtrArray[] = {
    {RailTransSelfCommandProcFunc_Reset                             , "Reset"                   },
    {RailTransSelfCommandProcFunc_Move2PushIn1                      , "Move2PushIn1"            },
    {RailTransSelfCommandProcFunc_Move2PushIn2                      , "Move2PushIn2"            },
    {RailTransSelfCommandProcFunc_Move2PushOut1                     , "Move2PushOut1"           },
    {RailTransSelfCommandProcFunc_Move2PushOut2                     , "Move2PushOut2"           },
    {RailTransSelfCommandProcFunc_Move2EmergencyCall                , "Move2EmergencyCall"      },
    {RailTransSelfCommandProcFunc_Move2Rotate1                      , "Move2Rotate1"            },
    {RailTransSelfCommandProcFunc_Move2Rotate2                      , "Move2Rotate2"            },
    {RailTransSelfCommandProcFunc_Move2RotateAuto                   , "Move2RotateAuto"         },
    {RailTransSelfCommandProcFunc_Move2RackBarScan                  , "Move2RackBarScan"        },
    {RailTransSelfCommandProcFunc_Move2TubeBarScan                  , "Move2TubeBarScan"        },
    {RailTransSelfCommandProcFunc_Move2WaitTest                     , "Move2WaitTest"           },
    {RailTransSelfCommandProcFunc_Move2RepeatTest                   , "Move2RepeatTest"         },
    {RailTransSelfCommandProcFunc_Move2TestingPut                   , "Move2TestingPut"         },
    {RailTransSelfCommandProcFunc_Move2TestingCatch                 , "Move2TestingCatch"       },
    {RailTransSelfCommandProcFunc_Move2TestingAbsorb                , "Move2TestingAbsorb"      },
    {RailTransSelfCommandProcFunc_GetEmergencyState                 , "GetEmergencyState"       },
    {RailTransSelfCommandProcFunc_LockSet                           , "LockSet"                 },
    {RailTransSelfCommandProcFunc_Move2BarScanIntegrate             , "Move2BarScanIntegrate"   },
    {RailTransSelfCommandProcFunc_Move2BarScanPosIntegrate          , "Move2BarScanPosIntegrate"},
};
//自身支持指令数量
#define TASK_SELF_MSG_RAIL_TRANS_CMD_COUNT  ((sizeof(RailTransSelfCommandProcFuncPtrArray))/(sizeof(RailTransSelfCommandProcFuncPtrArray[0])))

//匹配发送过来的指令序号
static void ServiceTaskFuncRailTrans_SelfMsg_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	IPC_DATA_RAIL_TRANS* selfCmdResultDataPtr = (IPC_DATA_RAIL_TRANS*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (selfCmdResultDataPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	selfCmdResultDataPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_RAIL_TRANS_CMD_COUNT)
	{
        SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,(uint8_t*)"unsupport",
                                selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                                selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                                );
		//不支持的指令
		actionResult = LH_ERR_BOARD_MAIN_COMM_IPC_CMD_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = RailTransSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,&(selfCmdResultDataPtr->can1ErrorCode),&(selfCmdResultDataPtr->can1ReportErrorLevel),
                                                                                            RailTransSelfCommandProcFuncPtrArray[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    SystemDebugShowMsgEnd((uint8_t*)RailTransDebugEndMsg,actionResult);
	//处理完成,设置状态
	selfCmdResultDataPtr->state = STATE_IPC_CMD_COMPLETE;
	selfCmdResultDataPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(selfCmdResultDataPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//轨道运转复位
static LH_ERR RailTransSelfCommandProcFunc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_RESET_CORRECT_FLAG resetFlag = (RAIL_RESET_CORRECT_FLAG)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,resetFlag,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransReset(resetFlag,can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到进样1
static LH_ERR RailTransSelfCommandProcFunc_Move2PushIn1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2PushIn1(can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到进样2
static LH_ERR RailTransSelfCommandProcFunc_Move2PushIn2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2PushIn2(can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到出样1
static LH_ERR RailTransSelfCommandProcFunc_Move2PushOut1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2PushOut1(can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到出样2
static LH_ERR RailTransSelfCommandProcFunc_Move2PushOut2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2PushOut2(can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到急诊区
static LH_ERR RailTransSelfCommandProcFunc_Move2EmergencyCall(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2EmergencyCall(can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到旋转位1
static LH_ERR RailTransSelfCommandProcFunc_Move2Rotate1(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2Rotate1(can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到旋转位2
static LH_ERR RailTransSelfCommandProcFunc_Move2Rotate2(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2Rotate2(can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到距离比较近的旋转位
static LH_ERR RailTransSelfCommandProcFunc_Move2RotateAuto(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2RotateAuto(can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到试管架扫码位
static LH_ERR RailTransSelfCommandProcFunc_Move2RackBarScan(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_BAR_SCAN_POS scanPos = (RAIL_BAR_SCAN_POS)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,scanPos,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2RackBarScan(scanPos,can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到指定试管扫码位
static LH_ERR RailTransSelfCommandProcFunc_Move2TubeBarScan(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_TUBE_POS tubeIndex = (RAIL_TUBE_POS)(selfCmdPtr->commandParam1);
    RAIL_BAR_SCAN_POS scanPos = (RAIL_BAR_SCAN_POS)(selfCmdPtr->commandParam2);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,tubeIndex,scanPos,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2TubeBarScan(tubeIndex,scanPos,can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到指定模块待测位
static LH_ERR RailTransSelfCommandProcFunc_Move2WaitTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_WAITTEST_NO waitTestNo = (RAIL_WAITTEST_NO)(selfCmdPtr->commandParam2);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,waitTestNo,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2WaitTest(moduleNo,waitTestNo,can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到指定模块重测位
static LH_ERR RailTransSelfCommandProcFunc_Move2RepeatTest(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_RETEST_NO repeatTestNo = (RAIL_RETEST_NO)(selfCmdPtr->commandParam2);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,repeatTestNo,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2RepeatTest(moduleNo,repeatTestNo,can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到指定模块测试区放架位
static LH_ERR RailTransSelfCommandProcFunc_Move2TestingPut(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_TUBE_POS tubeIndex = (RAIL_TUBE_POS)(selfCmdPtr->commandParam2);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,tubeIndex,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2TestingPut(moduleNo,tubeIndex,can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到指定模块测试区取架位
static LH_ERR RailTransSelfCommandProcFunc_Move2TestingCatch(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_TUBE_POS tubeIndex = (RAIL_TUBE_POS)(selfCmdPtr->commandParam2);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,tubeIndex,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2TestingCatch(moduleNo,tubeIndex,can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}

//轨道运行到指定模块测试区吸样位
static LH_ERR RailTransSelfCommandProcFunc_Move2TestingAbsorb(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_MODULE_POS moduleNo = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_TUBE_POS tubeIndex = (RAIL_TUBE_POS)(selfCmdPtr->commandParam2);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,moduleNo,tubeIndex,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2TestingAbsorb(moduleNo,tubeIndex,can1ErrorPtr,reportLevelResultPtr);;
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    //返回结果数据
    return actionResult;
}

//轨道传送带获取急诊区状态
static LH_ERR RailTransSelfCommandProcFunc_GetEmergencyState(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    //返回执行结果
    return actionResult;
}

//轨道传送带急诊锁定
static LH_ERR RailTransSelfCommandProcFunc_LockSet(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析传过来的参数
    RAIL_OUTPUT_STATE lockSet = (RAIL_OUTPUT_STATE)(selfCmdPtr->commandParam1);
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,lockSet,0,0,0,0);
    //调用函数实现
    actionResult = ServiceImplRailTransLockSet(lockSet,can1ErrorPtr,reportLevelResultPtr);
    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    //返回结果数据
    return actionResult;
}

//轨道运行到指定试管扫码位
static LH_ERR RailTransSelfCommandProcFunc_Move2BarScanIntegrate(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    uint8_t *barScanResultData = NULL;
    /* 申请内存 */
    do
    {
        barScanResultData = UserMemMalloc(MEM_SRAM,200);
        if(barScanResultData == NULL)
        {
            CoreDelayMinTick();
        }
    }while(barScanResultData == NULL);
   
    //调用函数实现
    actionResult = ServiceImplRailTransMove2BarScanIntegrate(barScanResultData,can1ErrorPtr,reportLevelResultPtr);

    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    /* 释放内存 */
    if(barScanResultData != NULL)
    {
        UserMemFree(MEM_SRAM,barScanResultData);
    }
    return actionResult;
}

//轨道运行到集成扫码指定位置-起始,结束
static LH_ERR RailTransSelfCommandProcFunc_Move2BarScanPosIntegrate(SYSTEM_CMD_SELF* selfCmdPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //动作开始,打印开始信息,参数信息也一并打印
    SystemDebugShowMsgStart((uint8_t*)RailTransDebugStartMsg,detailDebugMsgPtr,0,0,0,0,0);
    RAIL_BAR_SCAN_POS scanPos = RAIL_BAR_SCAN_POS_START;
    scanPos = (RAIL_BAR_SCAN_POS)(selfCmdPtr->commandParam1);
    //调用函数实现
    actionResult = ServiceImplRailTransMove2BarScanPosIntegrate(scanPos,can1ErrorPtr,reportLevelResultPtr);

    //更新一下急诊位状态
    if(actionResult == LH_ERR_NONE)
    {
        actionResult = ServiceImplRailTransGetEmergencyState(can1ErrorPtr,reportLevelResultPtr);
    }
    return actionResult;
}
