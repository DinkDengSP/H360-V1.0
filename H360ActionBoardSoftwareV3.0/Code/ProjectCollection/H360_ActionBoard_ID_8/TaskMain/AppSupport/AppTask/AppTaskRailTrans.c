/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-13 14:02:44
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppTask\AppTaskRailTrans.c
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
CPU_STK stackBufferAppTaskRailTrans[STK_SIZE_APP_TASK_RAIL_TRANS]@".ccram";
#pragma pack()

//任务控制块
OS_TCB tcbAppTaskRailTrans@".ccram";


//模块定时任务运行周期
#define TIME_MS_AUTO_TASK_RAIL_TRANS_PERIOD        200
//模块定时执行任务初始化
static void AppTaskFuncRailTrans_Auto_Init(void);
//模块定时执行任务
static void AppTaskFuncRailTrans_Auto_Proc(void);

//模块CAN信息处理任务
static void AppTaskFuncRailTrans_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsg);
//模块自身转发消息处理任务
static void AppTaskFuncRailTrans_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr);

//任务函数
void AppTaskFuncRailTrans(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    TASK_CMD* taskCommandPtr = NULL;
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    //自动执行任务初始化
    AppTaskFuncRailTrans_Auto_Init();
    while(1)
    {
        #if(TIME_MS_AUTO_TASK_RAIL_TRANS_PERIOD != 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= TIME_MS_AUTO_TASK_RAIL_TRANS_PERIOD)
		{
			//上次发送时间和本次发送时间差了TIME_MS_AUTO_TASK_RAIL_TRANS_PERIOD个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncRailTrans_Auto_Proc();
		}

        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif
        //任务从队列中读取指令
        taskCommandPtr = OSTaskQPend((OS_TICK)TIME_MS_AUTO_TASK_RAIL_TRANS_PERIOD,
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
                AppTaskFuncRailTrans_CanMsg_Proc(canRecvDatPtr);
                //处理完成
                canRecvDatPtr->currentState = CAN_SLAVE_CHANNEL_IDLE;
            }
            else if(TASK_CMD_SRC_SELF == taskCommandPtr->cmdSrc)
            {
                //从自身来的数据
                SYSTEM_CMD_SELF* selfCmdPtr = (SYSTEM_CMD_SELF*)taskCommandPtr->cmdData;
                //自身指令处理
                AppTaskFuncRailTrans_Self_Proc(selfCmdPtr);
                //测试完成,释放自身申请的内存
                UserMemFree(SRAM_CCM,selfCmdPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            UserMemFree(SRAM_CCM,taskCommandPtr);
		}
    }
}



/*********************************************自身定时任务处理*********************************************/
//模块定时执行任务初始化
static void AppTaskFuncRailTrans_Auto_Init(void)
{
    return;
}
//模块定时执行任务
static void AppTaskFuncRailTrans_Auto_Proc(void)
{
    return;
}



/************************************************CAN总线接收到的消息的处理******************************************************/
//轨道复位,参数决定是否带零位偏移
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到进样1
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2PushIn1(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到进样2
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2PushIn2(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到出样1
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2PushOut1(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到出样2
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2PushOut2(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到急诊位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2EmergencyCall(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到旋转位1
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2Rotate1(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到旋转位2
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2Rotate2(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到靠近的旋转位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2RotateAuto(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到试管架扫码位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2RackBarScan(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到指定试管扫码位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2TubeBarScan(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到待测位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleWaitTest(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到重测位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleReTest(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到测试区放架位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleTestingPut(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到测试区取架位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleTestingCatch(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到测试区吸样位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleTestingAbsorb(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);
//轨道移动到扫码位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2BarScanIntegrate(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr);


//配置参数,代表系统回调列表以及参数需求
/****前一个是处理函数***************************************参数的最小长度*****/
static const SYSTEM_TASK_CAN_MSG_PROC_UNIT AppTaskCanMsgProcRailTransSetting[] = {
    {AppTaskFuncRailTrans_CanMsg_Proc_Reset                                ,           1           ,"Reset"                             },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2PushIn1                         ,           0           ,"Move2PushIn1"                      },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2PushIn2                         ,           0           ,"Move2PushIn2"                      },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2PushOut1                        ,           0           ,"Move2PushOut1"                     },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2PushOut2                        ,           0           ,"Move2PushOut2"                     },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2EmergencyCall                   ,           0           ,"Move2EmergencyCall"                },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2Rotate1                         ,           0           ,"Move2Rotate1"                      },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2Rotate2                         ,           0           ,"Move2Rotate2"                      },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2RotateAuto                      ,           0           ,"Move2RotateAuto"                   },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2RackBarScan                     ,           1           ,"Move2RackBarScan"                  },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2TubeBarScan                     ,           2           ,"Move2TubeBarScan"                  },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleWaitTest           ,           2           ,"Move2SpecialModuleWaitTest"        },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleReTest             ,           2           ,"Move2SpecialModuleReTest"          },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleTestingPut         ,           2           ,"Move2SpecialModuleTestingPut"      },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleTestingCatch       ,           2           ,"Move2SpecialModuleTestingCatch"    },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleTestingAbsorb      ,           2           ,"Move2SpecialModuleTestingAbsorb"   },
    {AppTaskFuncRailTrans_CanMsg_Proc_Move2BarScanIntegrate                ,           1           ,"Move2SpecialModuleTestingAbsorb"   },
};
#define TASK_CAN_MSG_RAIL_TRANS_COMMAND_COUNT  ((sizeof(AppTaskCanMsgProcRailTransSetting))/(sizeof(AppTaskCanMsgProcRailTransSetting[0])))

//模块CAN信息处理任务
static void AppTaskFuncRailTrans_CanMsg_Proc(CAN_SLAVE_CHANNEL_CACHE* canMsgCachePtr)
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
    if(commandValue >= TASK_CAN_MSG_RAIL_TRANS_COMMAND_COUNT)
    {
        //指令不支持
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_UNSUPPORT);
        //返回ACK错误
        return;
    }

    //识别长度是否支持,前四个字节是指令码
    if((4 +AppTaskCanMsgProcRailTransSetting[commandValue].paramDataMinLength) > canMsgCachePtr->currentLength)
    {
        //参数长度不够
        CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_CMD_PARAM_LOSS);
        //返回ACK错误
        return;
    }

    //到这里,经过基本校验,可以进入响应流程了
    CanSlaveSendAckWithRecvCache(canMsgCachePtr,CAN_BASE_ACK_OK);
    //函数调用
    errorCode = AppTaskCanMsgProcRailTransSetting[commandValue].funcPtr(canMsgCachePtr,
                    AppTaskCanMsgProcRailTransSetting[commandValue].debugDetailMsg);

    //打印结果
    AppShowEndMsg((uint8_t*)AppRailTransDebugEndMsg,errorCode);
    //返回结果代码
    CanSlaveSendResultWithRecvCache(canMsgCachePtr,errorCode);
}

//轨道复位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Reset(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //复位修正
    RAIL_RESET_CORRECT_FLAG resetCorrectFlag = RAIL_RESET_CORRECT_OFF;
    resetCorrectFlag = ((RAIL_RESET_CORRECT_FLAG)(cachePtrLocal->recvCacheArray[4]));

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransReset(resetCorrectFlag);
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到进样1
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2PushIn1(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2PushIn1();
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到进样2
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2PushIn2(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2PushIn2();
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到出样1
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2PushOut1(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2PushOut1();
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到出样2
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2PushOut2(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2PushOut2();
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到急诊位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2EmergencyCall(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2EmergencyCall();
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到旋转位1
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2Rotate1(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2Rotate1();
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到旋转位2
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2Rotate2(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2Rotate2();
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到靠近的旋转位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2RotateAuto(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2RotateAuto();
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到试管架扫码位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2RackBarScan(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //试管架扫码位置
    RAIL_BAR_SCAN_POS barScanPos = RAIL_BAR_SCAN_POS_START;
    barScanPos = ((RAIL_BAR_SCAN_POS)(cachePtrLocal->recvCacheArray[4]));

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2RackBarScan(barScanPos);
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到指定试管扫码位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2TubeBarScan(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //试管编号
    RAIL_TUBE_POS tubeIndex = RAIL_TUBE_POS_1;
    tubeIndex = ((RAIL_TUBE_POS)(cachePtrLocal->recvCacheArray[4]));
    //试管偏移
    RAIL_BAR_SCAN_POS barScanPos = RAIL_BAR_SCAN_POS_START;
    barScanPos = ((RAIL_BAR_SCAN_POS)(cachePtrLocal->recvCacheArray[5]));

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2TubeBarScan(tubeIndex,barScanPos);
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到待测位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleWaitTest(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //模块号
    RAIL_MODULE_POS moduleNo = RAIL_MODULE_POS_COMMON;
    moduleNo = ((RAIL_MODULE_POS)(cachePtrLocal->recvCacheArray[4]));
    //待测位编号
    RAIL_WAITTEST_NO waitTestNo = RAIL_WAITTEST_1;
    waitTestNo = ((RAIL_WAITTEST_NO)(cachePtrLocal->recvCacheArray[5]));

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2SpecialModuleWaitTest(moduleNo,waitTestNo);
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到重测位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleReTest(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //模块号
    RAIL_MODULE_POS moduleNo = RAIL_MODULE_POS_COMMON;
    moduleNo = ((RAIL_MODULE_POS)(cachePtrLocal->recvCacheArray[4]));
    //待测位编号
    RAIL_RETEST_NO repeatTestNo = RAIL_RETEST_1;
    repeatTestNo = ((RAIL_RETEST_NO)(cachePtrLocal->recvCacheArray[5]));

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2SpecialModuleReTest(moduleNo,repeatTestNo);
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到测试区放架位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleTestingPut(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //模块号
    RAIL_MODULE_POS moduleNo = RAIL_MODULE_POS_COMMON;
    moduleNo = ((RAIL_MODULE_POS)(cachePtrLocal->recvCacheArray[4]));
    //试管编号
    RAIL_TUBE_POS tubeIndex = RAIL_TUBE_POS_1;
    tubeIndex = ((RAIL_TUBE_POS)(cachePtrLocal->recvCacheArray[5]));

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2SpecialModuleTestingPut(moduleNo,tubeIndex);
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到测试区取架位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleTestingCatch(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //模块号
    RAIL_MODULE_POS moduleNo = RAIL_MODULE_POS_COMMON;
    moduleNo = ((RAIL_MODULE_POS)(cachePtrLocal->recvCacheArray[4]));
    //试管编号
    RAIL_TUBE_POS tubeIndex = RAIL_TUBE_POS_1;
    tubeIndex = ((RAIL_TUBE_POS)(cachePtrLocal->recvCacheArray[5]));

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2SpecialModuleTestingCatch(moduleNo,tubeIndex);
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到测试区吸样位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2SpecialModuleTestingAbsorb(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;

    //模块号
    RAIL_MODULE_POS moduleNo = RAIL_MODULE_POS_COMMON;
    moduleNo = ((RAIL_MODULE_POS)(cachePtrLocal->recvCacheArray[4]));
    //试管编号
    RAIL_TUBE_POS tubeIndex = RAIL_TUBE_POS_1;
    tubeIndex = ((RAIL_TUBE_POS)(cachePtrLocal->recvCacheArray[5]));

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2SpecialModuleTestingAbsorb(moduleNo,tubeIndex);
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

//轨道移动到扫码位
static LH_ERR AppTaskFuncRailTrans_CanMsg_Proc_Move2BarScanIntegrate(void* canMsgCachePtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //转换出缓存指针
    CAN_SLAVE_CHANNEL_CACHE* cachePtrLocal = (CAN_SLAVE_CHANNEL_CACHE*)canMsgCachePtr;
    //试管偏移
    RAIL_BAR_SCAN_POS barScanPos = RAIL_BAR_SCAN_POS_START;
    barScanPos = ((RAIL_BAR_SCAN_POS)(cachePtrLocal->recvCacheArray[4]));

    //轨道返回数据
    RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
    RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,((int32_t)barScanPos),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));

    //写入数据
    errorCode = AppImplRailTransMove2BarScanIntegrate(barScanPos);
    if(errorCode == LH_ERR_NONE)
    {
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //返回当前位置
        uint8_t dataBufferSend[2] = {0};
        dataBufferSend[0] = (uint8_t)railMainPos;
        dataBufferSend[1] = (uint8_t)railSubRegion;
        //返回数据包
        CanSlaveSendDataPackWithRecvCache(cachePtrLocal,CAN_SUB_DATA_MODULE_RAIL_POS,dataBufferSend,2);
    }
    //返回结果
    return errorCode;
}

/************************************************自身分层API调用的消息的处理**********************************/
//轨道复位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到进样1
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2PushIn1(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到进样2
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2PushIn2(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到出样1
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2PushOut1(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到出样2
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2PushOut2(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道运转到急诊位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2EmergencyCall(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到旋转位1
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2Rotate1(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到旋转位2
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2Rotate2(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到靠现在位置最近的旋转位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2RotateAuto(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到试管架扫码位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2RackBarScan(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到试管扫码位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2TubeBarScan(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到待测位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleWaitTest(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到重测位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleReTest(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到测试区放架位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleTestingPut(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到测试区取架位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleTestingCatch(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);
//轨道移动到测试吸样位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleTestingAbsorb(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr);

//配置参数,代表系统回调列表以及参数需求
static const SYSTEM_TASK_SELF_MSG_PROC_UNIT AppTaskSelfMsgProcRailTransSetting[] = {
    {AppTaskFuncRailTrans_Self_Proc_Reset                               ,"Reset"                          },
    {AppTaskFuncRailTrans_Self_Proc_Move2PushIn1                        ,"Move2PushIn1"                   },
    {AppTaskFuncRailTrans_Self_Proc_Move2PushIn2                        ,"Move2PushIn2"                   },
    {AppTaskFuncRailTrans_Self_Proc_Move2PushOut1                       ,"Move2PushOut1"                  },
    {AppTaskFuncRailTrans_Self_Proc_Move2PushOut2                       ,"Move2PushOut2"                  },
    {AppTaskFuncRailTrans_Self_Proc_Move2EmergencyCall                  ,"Move2EmergencyCall"             },
    {AppTaskFuncRailTrans_Self_Proc_Move2Rotate1                        ,"Move2Rotate1"                   },
    {AppTaskFuncRailTrans_Self_Proc_Move2Rotate2                        ,"Move2Rotate2"                   },
    {AppTaskFuncRailTrans_Self_Proc_Move2RotateAuto                     ,"Move2RotateAuto"                },
    {AppTaskFuncRailTrans_Self_Proc_Move2RackBarScan                    ,"Move2RackBarScan"               },
    {AppTaskFuncRailTrans_Self_Proc_Move2TubeBarScan                    ,"Move2TubeBarScan"               },
    {AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleWaitTest          ,"Move2SpecialModuleWaitTest"     },
    {AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleReTest            ,"Move2SpecialModuleReTest"       },
    {AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleTestingPut        ,"Move2SpecialModuleTestingPut"   },
    {AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleTestingCatch      ,"Move2SpecialModuleTestingCatch" },
    {AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleTestingAbsorb     ,"Move2SpecialModuleTestingAbsorb"},
};

//自身支持指令数量
#define TASK_SELF_MSG_RAIL_TRANS_COMMAND_COUNT  ((sizeof(AppTaskSelfMsgProcRailTransSetting))/(sizeof(AppTaskSelfMsgProcRailTransSetting[0])))


//模块自身转发消息处理任务
static void AppTaskFuncRailTrans_Self_Proc(SYSTEM_CMD_SELF* selfCmdPtr)
{
    OS_ERR err;
	LH_ERR actionResult;
	//指令过程与结果对象
	DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)selfCmdPtr->commandDataSpecialChannel;
	//进入响应流程,申请互斥信号量
	OSMutexPend (cmdResultPtr->dataMutexPtr,0,OS_OPT_PEND_BLOCKING,0,&err);
	//设置状态,开始处理,等待返回
	cmdResultPtr->state = STATE_IPC_WAIT_RETURN;

	//检查序号,不能大于最大支持
	if(selfCmdPtr->commandIndex >= TASK_SELF_MSG_RAIL_TRANS_COMMAND_COUNT)
	{
       AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)"Unsupport",
                    selfCmdPtr->commandParam1,selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,
                    selfCmdPtr->commandParam4,selfCmdPtr->commandParam5
                    );
		//不支持的指令
		actionResult = LH_ERR_IPC_UNSUPPORT;
	}
	else
	{
        //调用相关指令处理
		actionResult = AppTaskSelfMsgProcRailTransSetting[selfCmdPtr->commandIndex].funcPtr(selfCmdPtr,
                            AppTaskSelfMsgProcRailTransSetting[selfCmdPtr->commandIndex].debugDetailMsg);
	}
    //打印结果
    AppShowEndMsg((uint8_t*)AppRailTransDebugEndMsg,actionResult);
	
	//处理完成,设置状态
	cmdResultPtr->state = STATE_IPC_CMD_COMPLETE;
	cmdResultPtr->resultValue = actionResult;
	//释放信号量
	OSMutexPost(cmdResultPtr->dataMutexPtr,OS_OPT_POST_NONE,&err);
    return;
}


//轨道复位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Reset(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //复位修正
    RAIL_RESET_CORRECT_FLAG resetCorrectFlag = (RAIL_RESET_CORRECT_FLAG)(selfCmdPtr->commandParam1);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransReset(resetCorrectFlag);
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到进样1
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2PushIn1(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2PushIn1();
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到进样2
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2PushIn2(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2PushIn2();
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到出样1
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2PushOut1(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2PushOut1();
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到出样2
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2PushOut2(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2PushOut2();
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道运转到急诊位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2EmergencyCall(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2EmergencyCall();
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到旋转位1
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2Rotate1(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2Rotate1();
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到旋转位2
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2Rotate2(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2Rotate2();
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到靠现在位置最近的旋转位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2RotateAuto(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2RotateAuto();
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到试管架扫码位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2RackBarScan(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //试管架扫码位置
    RAIL_BAR_SCAN_POS barScanPos = (RAIL_BAR_SCAN_POS)(selfCmdPtr->commandParam1);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2RackBarScan(barScanPos);
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}   

//轨道移动到试管扫码位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2TubeBarScan(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //试管扫码位置
    RAIL_TUBE_POS tubeIndex = (RAIL_TUBE_POS)(selfCmdPtr->commandParam1);
    RAIL_BAR_SCAN_POS barScanPos = (RAIL_BAR_SCAN_POS)(selfCmdPtr->commandParam2);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2TubeBarScan(tubeIndex,barScanPos);
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到待测位,参数位模块号和待测位序号
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleWaitTest(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //参数解析
    RAIL_MODULE_POS railModule = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_WAITTEST_NO waitTestNo = (RAIL_WAITTEST_NO)(selfCmdPtr->commandParam2);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2SpecialModuleWaitTest(railModule,waitTestNo);
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到重测位,参数位模块号和重测位序号
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleReTest(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //参数解析
    RAIL_MODULE_POS railModule = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_RETEST_NO repeatTestNo = (RAIL_RETEST_NO)(selfCmdPtr->commandParam2);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2SpecialModuleReTest(railModule,repeatTestNo);
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到测试区放架位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleTestingPut(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //参数解析
    RAIL_MODULE_POS railModule = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_TUBE_POS tubeIndex = (RAIL_TUBE_POS)(selfCmdPtr->commandParam2);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2SpecialModuleTestingPut(railModule,tubeIndex);
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到测试区取架位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleTestingCatch(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //参数解析
    RAIL_MODULE_POS railModule = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_TUBE_POS tubeIndex = (RAIL_TUBE_POS)(selfCmdPtr->commandParam2);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2SpecialModuleTestingCatch(railModule,tubeIndex);
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}

//轨道移动到测试吸样位
static LH_ERR AppTaskFuncRailTrans_Self_Proc_Move2SpecialModuleTestingAbsorb(SYSTEM_CMD_SELF* selfCmdPtr,uint8_t* detailDebugMsgPtr)
{
    LH_ERR errCode = LH_ERR_NONE;
    //动作开始,打印开始信息
    AppShowStartMsg((uint8_t*)AppRailTransDebugStartMsg,(uint8_t*)detailDebugMsgPtr,selfCmdPtr->commandParam1,
                                selfCmdPtr->commandParam2,selfCmdPtr->commandParam3,selfCmdPtr->commandParam4,selfCmdPtr->commandParam5);
    //参数解析
    RAIL_MODULE_POS railModule = (RAIL_MODULE_POS)(selfCmdPtr->commandParam1);
    RAIL_TUBE_POS tubeIndex = (RAIL_TUBE_POS)(selfCmdPtr->commandParam2);
    //解析参数
    DATA_IPC_RAIL_TRANS* cmdResultPtr = (DATA_IPC_RAIL_TRANS*)(selfCmdPtr->commandDataSpecialChannel);
    //调用函数
    errCode = AppImplRailTransMove2SpecialModuleTestingAbsorb(railModule,tubeIndex);
    if(errCode == LH_ERR_NONE)
    {
        //轨道返回数据
        RAIL_MODULE_POS railMainPos = RAIL_MODULE_POS_COMMON;
        RAIL_SUB_MODULE railSubRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
        //获取当前位置
        AppImplRailTransReadCurrentModuleAndRegion(&railMainPos,&railSubRegion);
        //写入返回数据中
        cmdResultPtr->mainRailModuleNo = railMainPos;
        cmdResultPtr->subRailRegion = railSubRegion;
    }
    return errCode;
}






