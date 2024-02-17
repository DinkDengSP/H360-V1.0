/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:59:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-25 15:56:02
**FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppTaskCupTrayManage.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferAppTaskCupTrayManage[STK_SIZE_APP_TASK_CUP_TRAY_MANAGE]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskCupTrayManage@".ccram";

//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_CUP_TRAY_MANAGE_PERIOD        200
//模块定时任务初始化
static void AppTaskFuncCupTrayManage_Auto_Init(void);
#if(TIME_MS_AUTO_TASK_CUP_TRAY_MANAGE_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncCupTrayManage_Auto_Proc(void);
#endif

//模块CAN信息处理任务
static void AppTaskFuncCupTrayManage_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncCupTrayManage_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncCupTrayManage(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //模块定时任务初始化
    AppTaskFuncCupTrayManage_Auto_Init();
    #if(TIME_MS_AUTO_TASK_CUP_TRAY_MANAGE_PERIOD != 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_CUP_TRAY_MANAGE_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_CUP_TRAY_MANAGE_PERIOD)
		{
			//上次发送时间和本次发送时间差了10时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncCupTrayManage_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif

        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_CUP_TRAY_MANAGE_PERIOD,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(TASK_CMD_SRC_CAN == taskCommandPtr->cmdSrc)
            {
                //从can来的数据
                CAN_SLAVE_CHANNEL_CACHE* canRecvDatPtr = (CAN_SLAVE_CHANNEL_CACHE*)taskCommandPtr->cmdData;
                //对这个数据进行处理
                AppTaskFuncCupTrayManage_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncCupTrayManage_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}



/*********************************************自身定时任务处理*********************************************/
//模块定时任务初始化
static void AppTaskFuncCupTrayManage_Auto_Init(void)
{
    AppImplCupTrayManageStateMachineInit();
}
#if(TIME_MS_AUTO_TASK_CUP_TRAY_MANAGE_PERIOD != 0)
//模块定时执行任务
static void AppTaskFuncCupTrayManage_Auto_Proc(void)
{
    AppImplCupTrayManageStateMachineLoop();
}
#endif



/************************************************CAN总线接收到的消息的处理******************************************************/
//设置杯栈门状态
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_SetDoorState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯盘栈初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_NewStackInit(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//空杯盘栈初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_EmptyStackInit(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//推手初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_PushInit(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//夹手初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_ClampInit(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//垃圾桶下降
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_GarbageDownUnlock(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//垃圾桶上升
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_GarbegeUpLock(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//杯栈模块整体初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_InitAll(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//夹手释放
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_ClampOpen(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//夹手夹紧
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_ClampClose(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯盘栈上传一个新盘
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_UploadNewTray(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//空杯盘栈从测试区回收一个空盘
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_RecycleEmptyTray(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//推手推一个新盘到测试位
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_PushNewTrayToTest(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯盘栈上升指定层(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_NewStackUpSpecialLayerWithDoorOpenState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//新杯盘栈回零(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_NewStackReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//空杯盘上升指定层(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_EmptyStackUpSpecialLayerWithDoorOpenState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//空杯盘栈回零(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_EmptyStackReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//读取传感器讯息
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_ReadSensorState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机复位
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//指定步进电机走指定步数
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcFuncPtrArrayCupTrayManage[] = {
    {AppTaskFuncCupTrayManage_CanMsg_Proc_SetDoorState                              ,1  ,"SetDoorState"                             },//杯栈管理设置杯栈门状态
    {AppTaskFuncCupTrayManage_CanMsg_Proc_NewStackInit                              ,0  ,"NewStackInit"                             },//杯栈管理新杯盘栈初始化
    {AppTaskFuncCupTrayManage_CanMsg_Proc_EmptyStackInit                            ,0  ,"EmptyStackInit"                           },//杯栈管理空杯盘栈初始化
    {AppTaskFuncCupTrayManage_CanMsg_Proc_PushInit                                  ,0  ,"PushInit"                                 },//杯栈管理推手初始化
    {AppTaskFuncCupTrayManage_CanMsg_Proc_ClampInit                                 ,0  ,"ClampInit"                                },//杯栈管理夹手初始化
    {AppTaskFuncCupTrayManage_CanMsg_Proc_GarbageDownUnlock                         ,1  ,"GarbageDownUnlock"                        },//杯栈管理垃圾桶下降
    {AppTaskFuncCupTrayManage_CanMsg_Proc_GarbegeUpLock                             ,1  ,"GarbegeUpLock"                            },//杯栈管理垃圾桶上升
    {AppTaskFuncCupTrayManage_CanMsg_Proc_InitAll                                   ,0  ,"InitAll"                                  },//杯栈管理杯栈模块整体初始化
    {AppTaskFuncCupTrayManage_CanMsg_Proc_ClampOpen                                 ,0  ,"ClampOpen"                                },//杯栈管理夹手释放
    {AppTaskFuncCupTrayManage_CanMsg_Proc_ClampClose                                ,0  ,"ClampClose"                               },//杯栈管理夹手夹紧
    {AppTaskFuncCupTrayManage_CanMsg_Proc_UploadNewTray                             ,0  ,"UploadNewTray"                            },//杯栈管理新杯盘栈上传一个新盘
    {AppTaskFuncCupTrayManage_CanMsg_Proc_RecycleEmptyTray                          ,0  ,"RecycleEmptyTray"                         },//杯栈管理空杯盘栈从测试区回收一个空盘
    {AppTaskFuncCupTrayManage_CanMsg_Proc_PushNewTrayToTest                         ,0  ,"PushNewTrayToTest"                        },//杯栈管理推手推一个新盘到测试位
    {AppTaskFuncCupTrayManage_CanMsg_Proc_NewStackUpSpecialLayerWithDoorOpenState   ,1  ,"NewStackUpSpecialLayerWithDoorOpenState"  },//杯栈管理新杯盘栈上升指定层(门开到位状态)
    {AppTaskFuncCupTrayManage_CanMsg_Proc_NewStackReturnZero                        ,0  ,"NewStackReturnZero"                       },//杯栈管理新杯盘栈回零(门开到位状态)
    {AppTaskFuncCupTrayManage_CanMsg_Proc_EmptyStackUpSpecialLayerWithDoorOpenState ,1  ,"EmptyStackUpSpecialLayerWithDoorOpenState"},//杯栈管理空杯盘上升指定层(门开到位状态)
    {AppTaskFuncCupTrayManage_CanMsg_Proc_EmptyStackReturnZero                      ,0  ,"EmptyStackReturnZero "                    },//杯栈管理空杯盘栈回零(门开到位状态)
    {AppTaskFuncCupTrayManage_CanMsg_Proc_ReadSensorState                           ,0  ,"ReadSensorState"                          },//杯栈管理读取传感器讯息
    {AppTaskFuncCupTrayManage_CanMsg_Proc_StepMotorReset                            ,1  ,"StepMotorReset"                           },//杯栈管理指定步进电机复位
    {AppTaskFuncCupTrayManage_CanMsg_Proc_StepMotorRunSteps                         ,5  ,"StepMotorRunSteps"                        },//杯栈管理指定步进电机走指定步数
};
#define TASK_CAN_MSG_COMMAND_COUNT_CUP_TRAY_MANAGE  ((sizeof(AppTaskCanMsgProcFuncPtrArrayCupTrayManage))/(sizeof(AppTaskCanMsgProcFuncPtrArrayCupTrayManage[0])))

//模块CAN信息处理任务
static void AppTaskFuncCupTrayManage_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
{
    LH_ERR errorCode =LH_ERR_NONE;

    //首先,参数长度必须大于4
    if(canMsgCachePtr->currentLength < 4)
    {
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_LENGTH);
        //返回ACK错误
        return;
    }
    //读取指令码,识别指令码是否支持
    uint32_t commandValue = CanBaseReadDataConvertToUint32(canMsgCachePtr->recvCacheArray,0);
    if(commandValue >= TASK_CAN_MSG_COMMAND_COUNT_CUP_TRAY_MANAGE)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }
    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcFuncPtrArrayCupTrayManage[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcFuncPtrArrayCupTrayManage[commandValue].funcPtr(canMsgCachePtr,
                                                                                    AppTaskCanMsgProcFuncPtrArrayCupTrayManage[commandValue].debugDetailMsg);
    //打印结果
    AppShowEndMsg((uint8_t*)AppCupTrayManageDebugEndMsg,errorCode);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//设置杯栈门状态
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_SetDoorState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    STACK_MANAGE_DOOR_STATE doorState = (STACK_MANAGE_DOOR_STATE)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)doorState),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplCupTrayManageSetDoorState(doorState);
    //如果有数据需要返回,就在这里返回
    
    //返回结果
    return actionResult;
}

//新杯盘栈初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_NewStackInit(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageNewStackInit(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//空杯盘栈初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_EmptyStackInit(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageEmptyStackInit(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//推手初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_PushInit(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManagePushInit(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//夹手初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_ClampInit(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageClampInit(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//垃圾桶下降
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_GarbageDownUnlock(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    GARBAGE_SELECT garbageNo = (GARBAGE_SELECT)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageGarbageDownUnlock(garbageNo,&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//垃圾桶上升
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_GarbegeUpLock(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    GARBAGE_SELECT garbageNo = (GARBAGE_SELECT)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageGarbegeUpLock(garbageNo,&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//杯栈模块整体初始化
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_InitAll(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageInitAll(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//夹手释放
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_ClampOpen(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageClampOpen(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//夹手夹紧
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_ClampClose(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageClampClose(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//新杯盘栈上传一个新盘
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_UploadNewTray(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageUploadNewTray(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//空杯盘栈从测试区回收一个空盘
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_RecycleEmptyTray(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageRecycleEmptyTray(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//推手推一个新盘到测试位
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_PushNewTrayToTest(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManagePushNewTrayToTest(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//新杯盘栈上升指定层(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_NewStackUpSpecialLayerWithDoorOpenState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    uint8_t layer = (uint8_t)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)layer),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageNewStackUpSpecialLayerWithDoorOpenState(layer,&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//新杯盘栈回零(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_NewStackReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageNewStackReturnZero(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//空杯盘上升指定层(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_EmptyStackUpSpecialLayerWithDoorOpenState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    uint8_t layer = (uint8_t)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)layer),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenState(layer,&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//空杯盘栈回零(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_EmptyStackReturnZero(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //uint8_t layer = (uint8_t)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageEmptyStackReturnZero(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//读取传感器讯息
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_ReadSensorState(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //XXXX xxxx = (XXXX)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageReadSensorState(&stackManageState);
    //如果有数据需要返回,就在这里返回
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[6] = {0};
        dataBufferSend[0] = (uint8_t)stackManageState.newCupTrayCount;
        dataBufferSend[1] = (uint8_t)stackManageState.emptyTrayCount;
        dataBufferSend[2] = (uint8_t)stackManageState.newTraySensorState;
        dataBufferSend[3] = (uint8_t)stackManageState.testTraySensorState;
        dataBufferSend[4] = (uint8_t)stackManageState.garbage1SensorState;
        dataBufferSend[5] = (uint8_t)stackManageState.garbage2SensorState;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE,dataBufferSend,6);
    }
    //返回结果
    return actionResult;
}

//指定步进电机复位
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_StepMotorReset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_MOTOR_CUP_TRAY_STACK_MANAGE motorIndex = (INDEX_MOTOR_CUP_TRAY_STACK_MANAGE)(cachePtrLocal->recvCacheArray[4]);
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)motorIndex),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplCupTrayManageStepMotorReset(motorIndex);
    //如果有数据需要返回,就在这里返回
    
    //返回结果
    return actionResult;
}

//指定步进电机走指定步数
static LH_ERR AppTaskFuncCupTrayManage_CanMsg_Proc_StepMotorRunSteps(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //参数解析
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    INDEX_MOTOR_CUP_TRAY_STACK_MANAGE motorIndex = (INDEX_MOTOR_CUP_TRAY_STACK_MANAGE)(cachePtrLocal->recvCacheArray[4]);
    int32_t runSteps = CanBaseReadDataConvertToInt32(cachePtrLocal->recvCacheArray,5);
    int32_t posAfterRun;
    
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)motorIndex),((int32_t)runSteps),((int32_t)0),((int32_t)0),((int32_t)0));
    
    //真实函数调用
    actionResult = AppImplCupTrayManageStepMotorRunSteps(motorIndex,runSteps,&posAfterRun);
    if(actionResult == LH_ERR_NONE)
    {
        uint8_t dataBufferSend[5] = {0};
        dataBufferSend[0] = (uint8_t)motorIndex;
        CanBaseWriteInt32DataToBuffer(dataBufferSend,1,posAfterRun);
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_BASE_DATA_MODULE_MOTOR,dataBufferSend,5);
    }
    //返回结果
    return actionResult;
}





/************************************************自身分层API调用的消息的处理**********************************/
//设置杯栈门状态
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_SetDoorState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯盘栈初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_NewStackInit(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//空杯盘栈初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_EmptyStackInit(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//推手初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_PushInit(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//夹手初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_ClampInit(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//垃圾桶下降
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_GarbageDownUnlock(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//垃圾桶上升
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_GarbegeUpLock(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//杯栈模块整体初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_InitAll(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//夹手释放
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_ClampOpen(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//夹手夹紧
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_ClampClose(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯盘栈上传一个新盘
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_UploadNewTray(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//空杯盘栈从测试区回收一个空盘
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_RecycleEmptyTray(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//推手推一个新盘到测试位
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_PushNewTrayToTest(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯盘栈上升指定层(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_NewStackUpSpecialLayerWithDoorOpenState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//新杯盘栈回零(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_NewStackReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//空杯盘上升指定层(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_EmptyStackUpSpecialLayerWithDoorOpenState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//空杯盘栈回零(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_EmptyStackReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//读取传感器讯息
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_ReadSensorState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);


//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcFuncPtrArrayCupTrayManage[] = {
    {AppTaskFuncCupTrayManage_Self_Proc_SetDoorState                                    ,"SetDoorState"                              },//杯栈管理设置杯栈门状态
    {AppTaskFuncCupTrayManage_Self_Proc_NewStackInit                                    ,"NewStackInit"                              },//杯栈管理新杯盘栈初始化
    {AppTaskFuncCupTrayManage_Self_Proc_EmptyStackInit                                  ,"EmptyStackInit"                            },//杯栈管理空杯盘栈初始化
    {AppTaskFuncCupTrayManage_Self_Proc_PushInit                                        ,"PushInit"                                  },//杯栈管理推手初始化
    {AppTaskFuncCupTrayManage_Self_Proc_ClampInit                                       ,"ClampInit"                                 },//杯栈管理夹手初始化
    {AppTaskFuncCupTrayManage_Self_Proc_GarbageDownUnlock                               ,"GarbageDownUnlock"                         },//杯栈管理垃圾桶下降
    {AppTaskFuncCupTrayManage_Self_Proc_GarbegeUpLock                                   ,"GarbegeUpLock"                             },//杯栈管理垃圾桶上升
    {AppTaskFuncCupTrayManage_Self_Proc_InitAll                                         ,"InitAll"                                   },//杯栈管理杯栈模块整体初始化
    {AppTaskFuncCupTrayManage_Self_Proc_ClampOpen                                       ,"ClampOpen"                                 },//杯栈管理夹手释放
    {AppTaskFuncCupTrayManage_Self_Proc_ClampClose                                      ,"ClampClose"                                },//杯栈管理夹手夹紧
    {AppTaskFuncCupTrayManage_Self_Proc_UploadNewTray                                   ,"UploadNewTray"                             },//杯栈管理新杯盘栈上传一个新盘
    {AppTaskFuncCupTrayManage_Self_Proc_RecycleEmptyTray                                ,"RecycleEmptyTray"                          },//杯栈管理空杯盘栈从测试区回收一个空盘
    {AppTaskFuncCupTrayManage_Self_Proc_PushNewTrayToTest                               ,"PushNewTrayToTest"                         },//杯栈管理推手推一个新盘到测试位
    {AppTaskFuncCupTrayManage_Self_Proc_NewStackUpSpecialLayerWithDoorOpenState         ,"NewStackUpSpecialLayerWithDoorOpenState"   },//杯栈管理新杯盘栈上升指定层(门开到位状态)
    {AppTaskFuncCupTrayManage_Self_Proc_NewStackReturnZero                              ,"NewStackReturnZero"                        },//杯栈管理新杯盘栈回零(门开到位状态)
    {AppTaskFuncCupTrayManage_Self_Proc_EmptyStackUpSpecialLayerWithDoorOpenState       ,"EmptyStackUpSpecialLayerWithDoorOpenState" },//杯栈管理空杯盘上升指定层(门开到位状态)
    {AppTaskFuncCupTrayManage_Self_Proc_EmptyStackReturnZero                            ,"EmptyStackReturnZero "                     },//杯栈管理空杯盘栈回零(门开到位状态)
    {AppTaskFuncCupTrayManage_Self_Proc_ReadSensorState                                 ,"ReadSensorState"                           },//杯栈管理读取传感器讯息
};

//自身支持指令数量
#define TASK_SELF_MSG_COMMAND_COUNT_CUP_TRAY_MANAGE  ((sizeof(AppTaskSelfMsgProcFuncPtrArrayCupTrayManage))/(sizeof(AppTaskSelfMsgProcFuncPtrArrayCupTrayManage[0])))


//模块自身转发消息处理任务
static void AppTaskFuncCupTrayManage_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_CUP_TRAY_MANAGE* cmdResultPtr = (DATA_IPC_CUP_TRAY_MANAGE*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_COMMAND_COUNT_CUP_TRAY_MANAGE)
	{
       AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)"Unsupport",selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,
                        selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcFuncPtrArrayCupTrayManage[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                                                                                                        AppTaskSelfMsgProcFuncPtrArrayCupTrayManage[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //模拟时序,假动作
    AppShowEndMsg((uint8_t*)AppCupTrayManageDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}

//设置杯栈门状态
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_SetDoorState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    STACK_MANAGE_DOOR_STATE doorState = (STACK_MANAGE_DOOR_STATE)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    actionResult = AppImplCupTrayManageSetDoorState(doorState);
    return actionResult;
}

//新杯盘栈初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_NewStackInit(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageNewStackInit(&stackManageState);
    return actionResult;
}

//空杯盘栈初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_EmptyStackInit(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageEmptyStackInit(&stackManageState);
    return actionResult;
}

//推手初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_PushInit(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManagePushInit(&stackManageState);
    return actionResult;
}

//夹手初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_ClampInit(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageClampInit(&stackManageState);
    return actionResult;
}

//垃圾桶下降
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_GarbageDownUnlock(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    GARBAGE_SELECT garbageNo = (GARBAGE_SELECT)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(garbageNo),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageGarbageDownUnlock(garbageNo,&stackManageState);
    return actionResult;
}

//垃圾桶上升
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_GarbegeUpLock(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    GARBAGE_SELECT garbageNo = (GARBAGE_SELECT)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(garbageNo),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageGarbegeUpLock(garbageNo,&stackManageState);
    return actionResult;
}

//杯栈模块整体初始化
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_InitAll(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageInitAll(&stackManageState);
    return actionResult;
}

//夹手释放
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_ClampOpen(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageClampOpen(&stackManageState);
    return actionResult;
}

//夹手夹紧
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_ClampClose(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageClampClose(&stackManageState);
    return actionResult;
}

//新杯盘栈上传一个新盘
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_UploadNewTray(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageUploadNewTray(&stackManageState);
    return actionResult;
}

//空杯盘栈从测试区回收一个空盘
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_RecycleEmptyTray(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageRecycleEmptyTray(&stackManageState);
    return actionResult;
}

//推手推一个新盘到测试位
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_PushNewTrayToTest(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManagePushNewTrayToTest(&stackManageState);
    return actionResult;
}

//新杯盘栈上升指定层(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_NewStackUpSpecialLayerWithDoorOpenState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    uint8_t layer = (uint8_t)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(layer),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageNewStackUpSpecialLayerWithDoorOpenState(layer,&stackManageState);
    return actionResult;
}

//新杯盘栈回零(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_NewStackReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageNewStackReturnZero(&stackManageState);
    return actionResult;
}

//空杯盘上升指定层(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_EmptyStackUpSpecialLayerWithDoorOpenState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    uint8_t layer = (uint8_t)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(layer),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenState(layer,&stackManageState);
    return actionResult;
}

//空杯盘栈回零(门开到位状态)
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_EmptyStackReturnZero(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageEmptyStackReturnZero(&stackManageState);
    return actionResult;
}

//读取传感器讯息
static LH_ERR AppTaskFuncCupTrayManage_Self_Proc_ReadSensorState(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR actionResult = LH_ERR_NONE;
    //解析动作参数
    //XXXX xxxx = (XXXX)(selfCmdPtr->commandParam1);

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppCupTrayManageDebugStartMsg,(uint8_t*)detailDebugMsgPtr,(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0),(int32_t)(0));
    //调用真实实现
    STACK_MANAGE_RESULT stackManageState;
    actionResult = AppImplCupTrayManageReadSensorState(&stackManageState);
    return actionResult;
}

