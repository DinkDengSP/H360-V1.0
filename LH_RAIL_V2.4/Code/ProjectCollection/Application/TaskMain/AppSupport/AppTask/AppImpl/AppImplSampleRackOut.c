/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:52:49
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-02 16:46:37
**FilePath: \App\TaskMain\AppSupport\AppTask\AppImpl\AppImplSampleRackOut.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplSampleRackOut.h"

//自动循环事件初始化
void AppImplSampleRackOut_EventInit(void)
{
    asm("nop");
}

//自动循环事件
void AppImplSampleRackOut_EventRun(uint32_t periodTimeMs)
{
    asm("nop");
}

/**********************************************************************辅助功能函数***********************************************************************/
//发送出样指令
static LH_ERR AppImplSampleRackOutSendPushOutCommand(uint8_t pushOut1Enable,uint8_t pushOut2Enable,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //调用IPC执行,出样1
        if(pushOut1Enable == 1)
        {
            errorCode = IPC_RailPushOut1PushOnceWithReturnZeroWhileAck();
            if(errorCode != LH_ERR_NONE)
            {
                //错误代码打印
                SystemDebugShowMsgError(errorCode);
                *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_OUT_UNDEFINE;
                *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
                return errorCode;
            }
        }
    //调用IPC执行,出样2
        if(pushOut2Enable == 1)
        {
            errorCode = IPC_RailPushOut2PushOnceWithReturnZeroWhileAck();
            if(errorCode != LH_ERR_NONE)
            {
                //错误代码打印
                SystemDebugShowMsgError(errorCode);
                *can1ErrorPtr = CAN1_REPORT_ERR_CODE_SAMPLE_OUT_UNDEFINE;
                *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
                return errorCode;
            }
        }
    return LH_ERR_NONE;
}

//检测出样返回
static LH_ERR AppImplSampleRackOutWithCommandReturn(uint8_t pushOut1Enable,uint8_t pushOut2Enable,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    //出样1
        LH_ERR errorPushOut1 = LH_ERR_NONE;
        CAN1_REPORT_ERR_CODE can1ErrorReportPushOut1 = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL can1ReportLevelPushOut1 = CAN1_REPORT_ERR_LEVEL_NORMAL;
        if(pushOut1Enable == 1)
        {
            //等待出样1返回
            errorPushOut1 = IPC_RailPushOut1WaitLastReturn(&can1ErrorReportPushOut1,&can1ReportLevelPushOut1);
            if(errorPushOut1 != LH_ERR_NONE)
            {
                //错误代码打印
                SystemDebugShowMsgError(errorPushOut1);
            }
        }
    //出样2
        LH_ERR errorPushOut2 = LH_ERR_NONE;
        CAN1_REPORT_ERR_CODE can1ErrorReportPushOut2 = CAN1_REPORT_OK;
        CAN1_REPORT_ERR_LEVEL can1ReportLevelPushOut2 = CAN1_REPORT_ERR_LEVEL_NORMAL;
        if(pushOut2Enable == 1)
        {
            //等待出样2返回
            errorPushOut2 = IPC_RailPushOut2WaitLastReturn(&can1ErrorReportPushOut2,&can1ReportLevelPushOut2);
            if(errorPushOut2 != LH_ERR_NONE)
            {
                //错误代码打印
                SystemDebugShowMsgError(errorPushOut2);
            }
            
        }
    //综合出样1和出样2的结果进行返回
        if(can1ErrorReportPushOut1 != CAN1_REPORT_OK)
        {
            *can1ErrorPtr = can1ErrorReportPushOut1;
            *reportLevelResultPtr = can1ReportLevelPushOut1;
            return errorPushOut1;
        }
        if(can1ErrorReportPushOut2 != CAN1_REPORT_OK)
        {
            *can1ErrorPtr = can1ErrorReportPushOut2;
            *reportLevelResultPtr = can1ReportLevelPushOut2;
            return errorPushOut2;
        }
    return LH_ERR_NONE;
}

/******************************************************************直接对接指令的函数************************************************************************/
//出样出样一次,根据配置决定是哪一个出样 0 同时 1出样1  2出样2
LH_ERR AppImplSampleRackOutPushOnce(uint8_t pushConfig,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *can1ErrorPtr = CAN1_REPORT_OK;
        *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //出样1选择
        uint8_t pushOut1EnableFlag = 0;
        uint8_t pushOut2EnableFlag = 0;
    //解析出样参数,计算出样配置
        if(pushConfig == 0)
        {
            /* 自动选择 */
            if(PUSH_OUT_RACK_PUT_EXIST == AppStateUtilGetPushOut1RackExistFlag())
            {
                pushOut1EnableFlag = 1;
                AppStateUtilSetPushOut1RackExistFlag(PUSH_OUT_RACK_PUT_NONE);
            }
            if(PUSH_OUT_RACK_PUT_EXIST == AppStateUtilGetPushOut2RackExistFlag())
            {
                pushOut2EnableFlag = 1;
                AppStateUtilSetPushOut2RackExistFlag(PUSH_OUT_RACK_PUT_NONE);
            }
            //如果一个都没选，那么就两个一起出
            if((pushOut1EnableFlag == 0)&&(pushOut2EnableFlag == 0))
            {
                pushOut1EnableFlag = 1;
                pushOut2EnableFlag = 1;
            }
        }
        else if(pushConfig == 1)
        {
            //手动选择出样1
            pushOut1EnableFlag = 1;
            AppStateUtilSetPushOut1RackExistFlag(PUSH_OUT_RACK_PUT_NONE);
        }
        else if(pushConfig == 2)
        {
            //手动选择出样2
            pushOut2EnableFlag = 1;
            AppStateUtilSetPushOut2RackExistFlag(PUSH_OUT_RACK_PUT_NONE);
        }
    //发送出样指令
        errorCode = AppImplSampleRackOutSendPushOutCommand(pushOut1EnableFlag,pushOut2EnableFlag,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待返回
        errorCode = AppImplSampleRackOutWithCommandReturn(pushOut1EnableFlag,pushOut2EnableFlag,can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    return LH_ERR_NONE;
}
