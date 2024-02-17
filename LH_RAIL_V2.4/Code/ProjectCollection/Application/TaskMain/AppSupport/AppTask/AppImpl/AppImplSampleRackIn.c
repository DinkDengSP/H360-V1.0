/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:52:25
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-22 11:17:04
**FilePath: \App\TaskMain\AppSupport\AppTask\AppImpl\AppImplSampleRackIn.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplSampleRackIn.h"
#include "AppIpcSampleRackMove.h"

//样本架进架标志位
uint8_t sampleRackInFlag = 0;
//样本架进架数量
uint8_t sampleRackInCount = 0;
//样本架进架最大次数
#define SAMPLE_RACK_IN_COUNT_MAX    3

//自动运行任务的周期,单位毫秒
#define SAMPLE_RACK_IN_EVENT_PERIOD_TIME_MS          200
//自动运行任务的触发次数,决定是否执行任务
static uint32_t countSampleRackInEventTrigCount = 0;

//急诊架检测进样
static LH_ERR ServiceImplSampleRackIn_EventJustRunEmergencyCallRack(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //更新一下传感器
        errorCode = IPC_RailTransGetCurrentEmergencyCallStateWhileReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //检测急诊架子是否能取
        if((RAIL_SENSOR_VALID == ServiceStateRailTransReadEmergencyCallDoorClose())&(RAIL_SENSOR_VALID == ServiceStateRailTransReadEmergencyCallRackExist()))
        {
            //有架子,发指令取取架子
            errorCode = IPC_SampleRackMoveCatchEmergencyCallToRackBarScanWhileReturn(can1ErrorPtr,reportLevelResultPtr);
            if(errorCode == LH_ERR_NONE)
            {
                //取到了架子,停止进架
                sampleRackInFlag = 0;
                sampleRackInCount = 0;
            }
            return errorCode;
        }
    return LH_ERR_NONE;
}

//样本台进架
static LH_ERR ServiceImplSampleRackIn_EventJustRunPushIn(uint8_t* pushin1TrigFlagPtr,uint8_t* pushin2TrigFlagPtr,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检测进样1的状态,看看是不能推进,托盘在位就能推
        if((PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState())||
            (PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState()))
        {
            //进样1推进一次
            errorCode = IPC_RailPushIn1PushOnceWhileAck();
            if(errorCode != LH_ERR_NONE)
            {
                *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_FAIL;
                *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
                return errorCode;
            }
            *pushin1TrigFlagPtr = 1;
        }
        else
        {
            //上报进样1托盘丢失
            Can1ReportErrorCode(CAN1_REPORT_ERR_LEVEL_WARNING,CAN1_REPORT_ERR_CODE_PUSHIN1_TRAY_LOSS,LH_ERR_BOARD_MAIN_RAIL_PUSH_IN1_TRAY_LOSS);
        }
    //检测进样2的状态,看看能不能推进,托盘在位就能推
        if((PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState())||
            (PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState()))
        {
            //进样2推进一次
            errorCode = IPC_RailPushIn2PushOnceWhileAck();
            if(errorCode != LH_ERR_NONE)
            {
                *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_FAIL;
                *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
                return errorCode;
            }
            *pushin2TrigFlagPtr = 1;
        }
        else
        {
            //上报进样2托盘丢失
            Can1ReportErrorCode(CAN1_REPORT_ERR_LEVEL_WARNING,CAN1_REPORT_ERR_CODE_PUSHIN2_TRAY_LOSS,LH_ERR_BOARD_MAIN_RAIL_PUSH_IN2_TRAY_LOSS);
        }
    //检测推进结果
    //进样1,2按钮状态写无效 
        ServiceStatePushIn1WritePushButtonState(RAIL_SENSOR_INVALID);
        ServiceStatePushIn2WritePushButtonState(RAIL_SENSOR_INVALID);
    //进样1
        if(*pushin1TrigFlagPtr == 1)
        {
            //进样1结果检查
            errorCode = IPC_RailPushIn1WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
            if(errorCode != LH_ERR_NONE)
            {
                return errorCode;
            }
        }
    //进样2
        if(*pushin2TrigFlagPtr == 1)
        {
            //进样2结果检查
            errorCode = IPC_RailPushIn2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
            if(errorCode != LH_ERR_NONE)
            {
                return errorCode;
            }
            //检查进样1推进中,进样2按钮是否触发
            else if((ServiceStatePushIn2ReadPushButtonState() == RAIL_SENSOR_VALID) && (*pushin1TrigFlagPtr == 1))
            {
                *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_CANCEL;
                *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_WARNING;
                return LH_ERR_BOARD_MAIN_RAIL_PUSH_IN_CANCEL;
            }
        }
    return LH_ERR_NONE;
}

//机械手取架
static LH_ERR ServiceImplSampleRackIn_EventJustRunCatchPushIn(uint8_t pushin1Enable,uint8_t pushin2Enable,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //根据推进结果决定从哪抓
    //进样1
        if(pushin1Enable == 1)
        {
            //进样1,只要存在位有效就抓取
            if(RAIL_SENSOR_VALID == ServiceStatePushIn1ReadInPlace())
            {
                //到位触发均有效
                errorCode = IPC_SampleRackMoveCatchPushInToRackBarScanWhileReturn(0,can1ErrorPtr,reportLevelResultPtr);
                if(errorCode == LH_ERR_NONE)
                {
                    //取到了架子,停止进架
                    sampleRackInFlag = 0;
                    sampleRackInCount = 0;
                }
                return errorCode;
            }
        }
    //进样2
        if(pushin2Enable == 1)
        {
            //进样2,只要存在位有效就抓取
            if(RAIL_SENSOR_VALID == ServiceStatePushIn2ReadInPlace())
            {
                //到位触发均有效
                errorCode = IPC_SampleRackMoveCatchPushInToRackBarScanWhileReturn(1,can1ErrorPtr,reportLevelResultPtr);
                if(errorCode == LH_ERR_NONE)
                {
                    //取到了架子,停止进架
                    sampleRackInFlag = 0;
                    sampleRackInCount = 0;
                }
                return errorCode;
            }
        }
    return LH_ERR_NONE;
}

//执行进样检测
static LH_ERR ServiceImplSampleRackIn_EventJustRun(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //连续进架
    if((sampleRackInFlag == 1)&&(sampleRackInCount < SAMPLE_RACK_IN_COUNT_MAX))
    {
        //启动一次进架
        //首先检查急诊位,满足条件就去急诊取架
        errorCode = ServiceImplSampleRackIn_EventJustRunEmergencyCallRack(can1ErrorPtr,reportLevelResultPtr);
        if((LH_ERR_NONE != errorCode)||(sampleRackInFlag == 0))
        {
            //抓取到架子或者急诊检测出错,推出
            return errorCode;
        }
        //急诊没架子,进样进架一次
        uint8_t pushin1TrigFlag = 0;
        uint8_t pushin2TrigFlag = 0;
        errorCode = ServiceImplSampleRackIn_EventJustRunPushIn(&pushin1TrigFlag,&pushin2TrigFlag,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //进架失败,退出
            return errorCode;
        }
        //走到这里,说明进样推到了架子,抓架子
        errorCode = ServiceImplSampleRackIn_EventJustRunCatchPushIn(pushin1TrigFlag,pushin2TrigFlag,can1ErrorPtr,reportLevelResultPtr);
        if((LH_ERR_NONE != errorCode)||(sampleRackInFlag == 0))
        {
            //抓取到架子或者急诊检测出错,推出
            return errorCode;
        }
        //如果进架过程没报错,就增加进架次数
        sampleRackInCount++;
        if(sampleRackInCount >= SAMPLE_RACK_IN_COUNT_MAX)
        {
            //进架超过限制次数,进架失败
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_FAIL;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_WARNING;
            errorCode = LH_ERR_BOARD_MAIN_RAIL_PUSH_IN_RETRY_MAX;
            //发送机械手空闲
            Can1RailSendHandStateIdleWithSelf();
            return errorCode;
        }
    }
    return LH_ERR_NONE;
}


//自动循环事件初始化
void AppImplSampleRackIn_EventInit(void)
{
    sampleRackInFlag = 0;
    sampleRackInCount = 0;
}

//自动循环事件
void AppImplSampleRackIn_EventRun(uint32_t periodTimeMs)
{
    CAN1_REPORT_ERR_CODE can1Error = CAN1_REPORT_OK;
    CAN1_REPORT_ERR_LEVEL reportLevelResult = CAN1_REPORT_ERR_LEVEL_NORMAL;
    LH_ERR errorcode = LH_ERR_NONE;
    if(periodTimeMs == 0)
    {
        return;
    }
    //心跳时间计时器增加
    countSampleRackInEventTrigCount++;
    //如果执行周期大于事件周期,立即运行,如果计数器事件次数大于事件次数,也执行
    if((periodTimeMs >= SAMPLE_RACK_IN_EVENT_PERIOD_TIME_MS)||(countSampleRackInEventTrigCount >= (uint32_t)(SAMPLE_RACK_IN_EVENT_PERIOD_TIME_MS/periodTimeMs)))
    {
        countSampleRackInEventTrigCount = 0;
        //执行一次自动循环任务
        errorcode = ServiceImplSampleRackIn_EventJustRun(&can1Error,&reportLevelResult);
        if(errorcode != LH_ERR_NONE)
        {
            Can1ReportErrorCode(reportLevelResult,can1Error,errorcode);
            //停止进架
            sampleRackInFlag = 0;
            sampleRackInCount = 0;
        }
    }
}



/*************************************轨道主控自身使用的指令*****************************************/
//开始进样
LH_ERR AppImplSampleRackInStart(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //打开进架标志位
        if(sampleRackInFlag == 0)
        {
            sampleRackInFlag = 1;
            sampleRackInCount = 0;
        }
    //发送机械手忙
        Can1RailSendHandStateBusyWithSelf();
    return LH_ERR_NONE;
}

//结束进样
LH_ERR AppImplSampleRackInStop(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //关闭进架标志位
        if(sampleRackInFlag == 1)
        {
            sampleRackInFlag = 0;
            sampleRackInCount = 0;
        }
    //发送机械手空闲
        Can1RailSendHandStateIdleWithSelf();
    return LH_ERR_NONE;
}

//整机复位
LH_ERR AppImplSampleRackInMachineReset(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //第一轮复位
    //整机复位,集成下位机复位
        errorCode = IPC_RailComplexResetAllWhileReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //条码枪检测有无,条码枪检测是否存在
        errorCode = IPC_RailBarScanCheckExistWhileReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //第二轮复位
    //进样1复位
        errorCode = IPC_RailPushIn1ResetWhileAck(RAIL_RESET_CORRECT_ON);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_UNDEFINE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //进样2复位
        errorCode = IPC_RailPushIn2ResetWhileAck(RAIL_RESET_CORRECT_ON);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_UNDEFINE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //出样1复位
        errorCode = IPC_RailPushOut1ResetWhileAck(RAIL_RESET_CORRECT_ON);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_UNDEFINE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //出样2复位
        errorCode = IPC_RailPushOut2ResetWhileAck(RAIL_RESET_CORRECT_ON);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_UNDEFINE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //机械手三维复位
        errorCode = IPC_RailHandResetAllWhileAck();
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_UNDEFINE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //等待机械手三维复位完成
        errorCode = IPC_RailHandWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //轨道复位
        errorCode = IPC_RailTransResetWhileAck(RAIL_RESET_CORRECT_ON);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_UNDEFINE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    //等待轨道复位完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            return errorCode;
        }
    //进样1相关错误代码
        LH_ERR errorCodePushIn1 = LH_ERR_NONE;
        CAN1_REPORT_ERR_CODE can1ErrorPushIn1 = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResultPushIn1 = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //等待进样1复位完成
        errorCodePushIn1 = IPC_RailPushIn1WaitLastReturn(&can1ErrorPushIn1,&reportLevelResultPushIn1);
        if(errorCodePushIn1 != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            //错误只有在加样停止级以上才会造成停机
            if((reportLevelResultPushIn1 == CAN1_REPORT_ERR_LEVEL_NORMAL_STOP)||(reportLevelResultPushIn1 == CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP))
            {
                *can1ErrorPtr = can1ErrorPushIn1;
                *reportLevelResultPtr = reportLevelResultPushIn1;
                return errorCodePushIn1;
            }
        }
    //进样2相关错误代码
        LH_ERR errorCodePushIn2 = LH_ERR_NONE;
        CAN1_REPORT_ERR_CODE can1ErrorPushIn2 = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResultPushIn2 = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //等待进样2复位完成
        errorCodePushIn2 = IPC_RailPushIn2WaitLastReturn(&can1ErrorPushIn2,&reportLevelResultPushIn2);
        if(errorCodePushIn2 != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            //错误只有在加样停止级以上的时候才会造成停级
            if((reportLevelResultPushIn2 == CAN1_REPORT_ERR_LEVEL_NORMAL_STOP)||(reportLevelResultPushIn2 == CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP))
            {
                *can1ErrorPtr = can1ErrorPushIn2;
                *reportLevelResultPtr = reportLevelResultPushIn2;
                return errorCodePushIn2;
            }
        }
    //出样1相关错误代码
        LH_ERR errorCodePushOut1 = LH_ERR_NONE;
        CAN1_REPORT_ERR_CODE can1ErrorPushOut1 = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResultPushOut1 = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //等待出样1复位完成
        errorCodePushOut1 = IPC_RailPushOut1WaitLastReturn(&can1ErrorPushOut1,&reportLevelResultPushOut1);
        if(errorCodePushOut1 != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            //错误只有在加样停止级以上的时候才会造成停级
            if((reportLevelResultPushOut1 == CAN1_REPORT_ERR_LEVEL_NORMAL_STOP)||(reportLevelResultPushOut1 == CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP))
            {
                *can1ErrorPtr = can1ErrorPushOut1;
                *reportLevelResultPtr = reportLevelResultPushOut1;
                return errorCodePushOut1;
            }
        }
    //出样2相关错误代码
        LH_ERR errorCodePushOut2 = LH_ERR_NONE;
        CAN1_REPORT_ERR_CODE can1ErrorPushOut2 = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL reportLevelResultPushOut2 = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //等待出样2复位完成
        errorCodePushOut2 = IPC_RailPushOut2WaitLastReturn(&can1ErrorPushOut2,&reportLevelResultPushOut2);
        if(errorCodePushOut2 != LH_ERR_NONE)
        {
            //错误代码打印
            SystemDebugShowMsgError(errorCode);
            //错误只有在加样停止级以上的时候才会造成停级
            if((reportLevelResultPushOut2 == CAN1_REPORT_ERR_LEVEL_NORMAL_STOP)||(reportLevelResultPushOut2 == CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP))
            {
                *can1ErrorPtr = can1ErrorPushOut2;
                *reportLevelResultPtr = reportLevelResultPushOut2;
                return errorCodePushOut2;
            }
        }
    //清除系统内部存储的试管架ID讯息
        AppStateClearMsgRackID();
    //综合判定复位结果
    //先判定进样结果
        if((errorCodePushIn1 != LH_ERR_NONE)&&(errorCodePushIn2 != LH_ERR_NONE))
        {
            //进样1进样2同时存在错误,错误等级升级成为加样停止级别
            reportLevelResultPushIn1 = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            reportLevelResultPushIn2 = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        if(errorCodePushIn1 != LH_ERR_NONE)
        {
            //进样1错误
            Can1ReportErrorCode(reportLevelResultPushIn1,can1ErrorPushIn1,errorCodePushIn1);
        }
        if(errorCodePushIn2 != LH_ERR_NONE)
        {
            //进样2错误
            Can1ReportErrorCode(reportLevelResultPushIn2,can1ErrorPushIn2,errorCodePushIn2);
        }
        if((errorCodePushIn1 != LH_ERR_NONE)&&(errorCodePushIn2 != LH_ERR_NONE))
        {
            //双进样同时异常,复位失败
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_COMPLEX_RESET_PUSN_IN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return LH_ERR_BOARD_MAIN_RAIL_RESET_PUSH_IN_ALL_WARNING;
        }
    //后判定出样结果
        if((errorCodePushOut1 != LH_ERR_NONE)&&(errorCodePushOut2 != LH_ERR_NONE))
        {
            //出样1出样2同时异常,错误等级升级为加样停止级
            reportLevelResultPushOut1 = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            reportLevelResultPushOut2 = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        if(errorCodePushOut1 != LH_ERR_NONE)
        {
            //出样1错误
            Can1ReportErrorCode(reportLevelResultPushOut1,can1ErrorPushOut1,errorCodePushOut1);
        }
        if(errorCodePushOut2 != LH_ERR_NONE)
        {
            //出样2错误
            Can1ReportErrorCode(reportLevelResultPushOut2,can1ErrorPushOut2,errorCodePushOut2);
        }
        if((errorCodePushOut1 != LH_ERR_NONE)&&(errorCodePushOut2 != LH_ERR_NONE))
        {
            //双出样同时异常,无法复位
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_COMPLEX_RESET_PUSN_OUT;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return LH_ERR_BOARD_MAIN_RAIL_RESET_PUSH_OUT_ALL_WARNING;
        }
    //运行到此处,复位可以完成
        return LH_ERR_NONE;
}

//进样并上传进架状态
LH_ERR AppImplSampleRackInReportWithoutCatch(SAMPLE_RACK_IN_POS* sampleRackInPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    uint8_t retryIndex = 0;
    //进样标志
    uint8_t pushin1TrigFlag = 0, pushin2TrigFlag = 0;
    //连续进样多次
    for(retryIndex = 0;retryIndex < SAMPLE_RACK_IN_COUNT_MAX;retryIndex++)
    {
        //更新一下急诊传感器
            errorCode = IPC_RailTransGetCurrentEmergencyCallStateWhileReturn(can1ErrorPtr,reportLevelResultPtr);
            if(errorCode != LH_ERR_NONE)
            {
                return errorCode;
            }
        //检测急诊架子是否能取
            if((RAIL_SENSOR_VALID == ServiceStateRailTransReadEmergencyCallDoorClose())&(RAIL_SENSOR_VALID == ServiceStateRailTransReadEmergencyCallRackExist()))
            {
                //回传架子在急诊
                *sampleRackInPos = SAMPLE_RACK_IN_EMERGENCY;
                return errorCode;
            }
        //检测进样1的状态,看看是不能推进,能推就推,托盘在位就能推
            if((PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState())||
                (PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn1ReadCurrentModuleState()))
            {
                //进样1推进一次
                errorCode = IPC_RailPushIn1PushOnceWhileAck();
                if(errorCode != LH_ERR_NONE)
                {
                    *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_FAIL;
                    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
                    return errorCode;
                }
                pushin1TrigFlag = 1;
            }
            else
            {
                //上报进样1托盘丢失
                Can1ReportErrorCode(CAN1_REPORT_ERR_LEVEL_WARNING,CAN1_REPORT_ERR_CODE_PUSHIN1_TRAY_LOSS,LH_ERR_BOARD_MAIN_RAIL_PUSH_IN1_TRAY_LOSS);
            }
            
        //检测进样2的状态,看看是不能推进,能推就推,托盘在位就能推
            if((PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState())||
                (PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST == ServiceStatePushIn2ReadCurrentModuleState()))
            {
                //进样2推进一次
                errorCode = IPC_RailPushIn2PushOnceWhileAck();
                if(errorCode != LH_ERR_NONE)
                {
                    *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_FAIL;
                    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
                    return errorCode;
                }
                pushin2TrigFlag = 1;
            }
            else
            {
                //上报进样2托盘丢失
                Can1ReportErrorCode(CAN1_REPORT_ERR_LEVEL_WARNING,CAN1_REPORT_ERR_CODE_PUSHIN2_TRAY_LOSS,LH_ERR_BOARD_MAIN_RAIL_PUSH_IN2_TRAY_LOSS);
            }
        //进样1,2按钮状态写无效
            ServiceStatePushIn1WritePushButtonState(RAIL_SENSOR_INVALID);
            ServiceStatePushIn2WritePushButtonState(RAIL_SENSOR_INVALID);
        //进样1推架子要等待推完
            if(pushin1TrigFlag == 1)
            {
                //进样1结果检查
                errorCode = IPC_RailPushIn1WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
                if(errorCode != LH_ERR_NONE)
                {
                    return errorCode;
                }
            }
        //进样2推架子要等待推完
            if(pushin2TrigFlag == 1)
            {
                //进样2结果检查
                errorCode = IPC_RailPushIn2WaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
                if(errorCode != LH_ERR_NONE)
                {
                    return errorCode;
                }
                //检查进样1推进中,进样2按钮是否触发
                else if((ServiceStatePushIn2ReadPushButtonState() == RAIL_SENSOR_VALID) && (pushin1TrigFlag == 1))
                {
                    *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_CANCEL;
                    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_WARNING;
                    return LH_ERR_BOARD_MAIN_RAIL_PUSH_IN_CANCEL;
                }
            }
        //检查进样1推完之后的状态
            if(pushin1TrigFlag == 1)
            {
                if(RAIL_SENSOR_VALID == ServiceStatePushIn1ReadInPlace())
                {
                    //到位触发均有效
                    *sampleRackInPos = SAMPLE_RACK_IN_PUSH_IN_1;
                    return errorCode;
                }
            }
        //进样2
            if(pushin2TrigFlag == 1)
            {
                //进样2
                if(RAIL_SENSOR_VALID == ServiceStatePushIn2ReadInPlace())
                {
                    //到位触发均有效
                    *sampleRackInPos = SAMPLE_RACK_IN_PUSH_IN_2;
                    return errorCode;
                }
            }
        //完成一次检测,等待一会进行下一轮
            CoreDelayMs(500);
        //清除状态
            pushin1TrigFlag = 0;
            pushin2TrigFlag = 0;
    }
    //运行到这里没发现架子,上报错误
    *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_IN_FAIL;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_WARNING;
    return LH_ERR_BOARD_MAIN_RAIL_PUSH_IN_RETRY_MAX;
}
